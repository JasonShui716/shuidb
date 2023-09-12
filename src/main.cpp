/*
 Copyright 2023 Jason Shui

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

#include "debugger.h"
#include "fs_utils.hpp"
#include "linenoise.h"
#include "output_utils.hpp"
#include "string_utils.hpp"

using namespace shuidb;

static void handle_signal_quit(int signal) {
  PR(ERROR) << "Received signal " << signal << ", quitting";
  exit(0);
}

void handle_command(Debugger& dbg, const std::string& line) {
  auto args = utils::split(line, ' ');
  auto command = args[0];

  if (utils::starts_with(command, "r") || utils::starts_with(command, "run")) {
    // TODO: Currently, it will break at the entry point of the program
    dbg.RunProc();
  } else if (utils::starts_with(command, "c")) {
    dbg.ContinueExecution();
  } else if (utils::starts_with(command, "q") ||
             utils::starts_with(command, "exit")) {
    dbg.Quit();
  } else if (utils::starts_with(command, "b")) {
    std::string addr_str = args[1];
    auto addr = std::stol(addr_str, 0, 16);
    dbg.SetBreakPointAtAddress(addr);
  } else if (utils::starts_with(command, "reg") ||
             utils::starts_with(command, "info reg")) {
    dbg.DumpRegisters();
  } else if (utils::starts_with(command, "h")) {
    PR(INFO) << "Commands:";
    PR(INFO) << "c: continue";
    PR(INFO) << "q: quit";
    PR(INFO) << "r: run";
    PR(INFO) << "b <addr>: set breakpoint at address <addr>";
    PR(INFO) << "reg / info reg: dump registers";
  } else {
    PR(ERROR) << "Unknown command";
  }
}

int main(int argc, char** argv) {
  signal(SIGINT, handle_signal_quit);
  signal(SIGTERM, handle_signal_quit);

  if (argc < 2) {
    PR(ERROR) << "Programe name not specified";
    return -1;
  }

  PR(INFO) << "Starting shuidb";

  auto prog = argv[1];
  if (!utils::file_exists(prog)) {
    PR(ERROR) << "File " << prog << " does not exist";
    return -1;
  }

  Debugger dbg(prog);
  char* line = nullptr;
  while ((line = linenoise("shuidb> ")) != nullptr) {
    handle_command(dbg, line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}