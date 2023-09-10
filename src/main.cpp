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

#include <sys/personality.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

#include "debugger.h"
#include "fs_utils.hpp"
#include "output_utils.hpp"

using namespace shuidb;

static void handle_signal_quit(int signal) {
  PR(ERROR) << "Received signal " << signal << ", quitting";
  exit(0);
}

int main(int argc, char **argv) {
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

  auto pid = fork();
  if (pid == 0) {
    // child process
    signal(SIGHUP, [](int) { exit(0); });
    personality(ADDR_NO_RANDOMIZE);
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);
  } else if (pid > 0) {
    // parent process
    Debugger dbg(prog, pid);
    dbg.Run();
    kill(pid, SIGTERM);
  } else {
    PR(ERROR) << "Fork failed";
    return -1;
  }
}