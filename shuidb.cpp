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
#include <ranges>

#include "debugger.h"
#include "linenoise.h"
#include "type_def.h"
#include "utils/fs_utils.hpp"
#include "utils/output_utils.hpp"
#include "utils/string_utils.hpp"

using namespace shuidb;

static void handle_signal_quit(int signal) {
  PR(ERROR) << "Received signal " << signal << ", quitting";
  exit(0);
}

template <typename View>
void handle_reg_command(Debugger& dbg, const View& args) {
  // Actually we don't need to use View here, but I just want to try it out
  // The original command should be like following:
  //  1. begin with `reg`, i.e. `reg r rax`, `reg w rax 0x1234` or `reg`
  //  2. begin with `info reg`, i.e. `info reg rax`, `info reg`
  // But after changing to View, only arguments after `reg` or `info reg` are
  // passed in

  // Since we have to iterate through the arguments, we use `cnt` as a state ma-
  // chine to indicate the current argument index
  int cnt = 0;
  OperationType op_type = OperationType::kRead;
  StatusType status_type = StatusType::kIncomplete;
  std::string write_reg_name;
  for (auto& arg : args) {
    switch (cnt) {
      // 0: command name, i.e. `r` or `w`
      //  if not command name, it may be register name from `info reg`
      case 0: {
        auto reg_command = utils::trim(arg);
        if (utils::starts_with(reg_command, "r") ||
            utils::starts_with(reg_command, "get")) {
          op_type = OperationType::kRead;
        } else if (utils::starts_with(reg_command, "w") ||
                   utils::starts_with(reg_command, "set")) {
          op_type = OperationType::kWrite;
        } else {
          // try to treat it as register name
          status_type = dbg.ReadRegister(arg);
          if (status_type == StatusType::kSuccess) {
            break;
          }
          PR(ERROR) << "Unknown register command";
          return;
        }
      } break;
      // 1: register name, i.e. `rax`
      case 1: {
        if (op_type == OperationType::kRead) {
          status_type = dbg.ReadRegister(arg);
        } else if (op_type == OperationType::kWrite) {
          write_reg_name = arg;
        }
      } break;
      // 2: if `w`, then it's the value to write, if `r`, then continue to
      // read the register
      case 2: {
        if (op_type == OperationType::kRead) {
          dbg.ReadRegister(arg);
        } else if (op_type == OperationType::kWrite) {
          status_type =
              dbg.WriteRegister(write_reg_name, std::stol(arg, 0, 16));
          if (status_type == StatusType::kSuccess) {
            PR(INFO) << "Write register " << write_reg_name << " to " << arg
                     << " success";
          }
        }
      } break;
      // >2: if `r`, then continue to read the register
      default: {
        if (op_type == OperationType::kRead) {
          dbg.ReadRegister(arg);
        } else if (op_type == OperationType::kWrite) {
          status_type = StatusType::kBadInput;
        }
      } break;
    }
    cnt++;
  }
  if (cnt == 0) {
    dbg.DumpRegisters();
  } else if (status_type == StatusType::kIncomplete) {
    PR(ERROR) << "Incomplete command";
  }
}

void handle_command(Debugger& dbg, const std::string& line) {
  auto args = utils::split(line, ' ');
  auto command = args[0];

  if (utils::starts_with(command, "c")) {
    dbg.ContinueExecution();
  } else if (utils::starts_with(command, "q") ||
             utils::starts_with(command, "exit")) {
    dbg.Quit();
  } else if (utils::starts_with(command, "b")) {
    if (args.size() < 2) {
      PR(ERROR) << "Address not specified";
      return;
    }
    std::string addr_str = args[1];
    auto addr = std::stol(addr_str, 0, 16);
    dbg.SetBreakPointAtAddress(addr);
  } else if (utils::starts_with(command, "reg")) {
    // `views::drop(1)` is used to drop the first element for the range view
    handle_reg_command(dbg, args | std::views::drop(1));
  } else if (utils::starts_with(command, "info")) {
    if (args.size() > 1) {
      auto info_name = utils::trim(args[1]);
      if (info_name == "reg") {
        handle_reg_command(dbg, args | std::views::drop(2));
      } else {
        PR(ERROR) << "Unknown info name";
      }
    } else {
      PR(ERROR) << "Info name not specified";
    }
  } else if (utils::starts_with(command, "r") ||
             utils::starts_with(command, "run")) {
    // TODO: Currently, it will break at the entry point of the program
    dbg.RunProc();
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