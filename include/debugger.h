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

#pragma once

#include <sys/ptrace.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "breakpoint.h"
#include "type_def.h"

namespace shuidb {

class Debugger {
 public:
  Debugger(std::string prog) : prog_(prog), pid_(0){};
  Debugger(std::string prog, pid_t pid) : prog_(prog), pid_(pid){};
  ~Debugger();
  void RunProc();
  void ContinueExecution();
  void SetBreakPointAtAddress(std::intptr_t addr);
  std::vector<std::intptr_t> GetBreakPoints() const;
  void DumpRegisters() const;
  StatusType ReadRegister(const std::string& reg_name) const;
  StatusType WriteRegister(const std::string& reg_name, const uint64_t& val);
  pid_t GetPid() const;
  bool IsRunning() const;
  void Quit();

 private:
  std::string prog_;
  bool running_{false};
  std::mutex mutex_;
  pid_t pid_{0};
  std::unordered_map<std::intptr_t, std::shared_ptr<BreakPoint>> breakpoints_;

  void SetRun(pid_t pid);
  void SetStop();
};

}  // namespace shuidb