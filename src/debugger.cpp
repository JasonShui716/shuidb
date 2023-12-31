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

#include "debugger.h"

#include <sys/personality.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <iomanip>

#include "breakpoint.h"
#include "register_operator.h"
#include "utils/fs_utils.hpp"
#include "utils/output_utils.hpp"
#include "utils/ps_utils.hpp"
#include "utils/string_utils.hpp"

namespace shuidb {

Debugger::~Debugger() { Quit(); };

void Debugger::RunProc() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (IsRunning()) {
    PR(ERROR) << "Process is already running";
    return;
  }

  if (!utils::file_exists(prog_)) {
    PR(ERROR) << "File " << prog_ << " does not exist";
    throw std::runtime_error("File does not exist");
  }

  auto pid = fork();
  if (pid == 0) {
    // child process
    // disable ASLR
    PR(INFO) << "Child process pid: " << std::dec << getpid();
    PR(INFO) << "Pausing...";
    personality(ADDR_NO_RANDOMIZE);
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog_.c_str(), prog_.c_str(), nullptr);
  } else if (pid >= 1) {
    // parent process
    int wait_status;
    waitpid(pid_, &wait_status, 0);
    SetRun(pid);
  } else {
    PR(ERROR) << "Fork failed";
    exit(1);
  }
}

void Debugger::ContinueExecution() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (!IsRunning()) {
    PR(ERROR) << "Process is not running";
    return;
  }
  PR(INFO) << "Continue...";
  ptrace(PTRACE_CONT, pid_, nullptr, nullptr);

  int wait_status;
  waitpid(pid_, &wait_status, 0);
  if (WIFEXITED(wait_status)) {
    PR(INFO) << "Process exited";
    SetStop();
  } else if (WIFSTOPPED(wait_status)) {
    PR(INFO) << "Process stopped";
  } else {
    PR(ERROR) << "Unknown wait status";
  }
}

void Debugger::SetBreakPointAtAddress(std::intptr_t addr) {
  std::lock_guard<std::mutex> lock(mutex_);

  auto base_load_addr = utils::GetProcessLoadAddress(pid_);
  if (addr < base_load_addr) {
    PR(WARNING) << "Address 0x" << std::hex << addr << " is not in the program";
    addr += base_load_addr;
    PR(WARNING) << "Try to plus the base load address 0x" << std::hex
                << base_load_addr << ", get 0x" << std::hex << addr;
  }

  if (breakpoints_.find(addr) != breakpoints_.end()) {
    PR(INFO) << "Breakpoint at address 0x" << std::hex << addr
             << " already exists";
    return;
  }

  PR(INFO) << "Set breakpoint at address 0x" << std::hex << addr;
  auto bp = std::make_shared<BreakPoint>(pid_, addr);
  bp->Enable();
  breakpoints_[addr] = bp;
}

std::vector<std::intptr_t> Debugger::GetBreakPoints() const {
  std::vector<std::intptr_t> bp_addrs;
  std::ranges::transform(breakpoints_, std::back_inserter(bp_addrs),
                         [](const auto& pair) { return pair.first; });
  return bp_addrs;
}

std::optional<std::unordered_map<Register, uint64_t>> Debugger::GetRegisters()
    const {
  if (!IsRunning()) {
    PR(ERROR) << "Process is not running";
    return std::nullopt;
  }
  return RegisterOperator::GetRegisters(pid_);
}

void Debugger::DumpRegisters() const {
  if (!IsRunning()) {
    PR(ERROR) << "Process is not running";
    return;
  }
  auto registers_map = GetRegisters();
  PR(INFO) << "Registers:";
  for (const auto& [reg, val] : registers_map.value()) {
    PR(RAW) << RegisterOperator::GetRegisterName(reg) << " 0x" << std::hex
            << std::setfill('0') << std::setw(16) << val;
  }
}

StatusType Debugger::ReadRegister(const std::string& reg_name) const {
  if (!IsRunning()) {
    PR(ERROR) << "Process is not running";
    return StatusType::kNotRunning;
  }

  auto reg = RegisterOperator::GetRegisterFromName(utils::trim(reg_name));
  if (!reg.has_value()) {
    PR(ERROR) << "Unknown register name " << reg_name;
    return StatusType::kUnknownRegister;
  }
  auto reg_value = RegisterOperator::GetRegisterValue(pid_, reg.value());
  if (!reg_value.has_value()) {
    PR(ERROR) << "Failed to get register value";
    return StatusType::kFailed;
  }
  PR(INFO) << reg_name << " 0x" << std::setfill('0') << std::setw(16)
           << std::hex << reg_value.value();

  return StatusType::kSuccess;
}

StatusType Debugger::WriteRegister(const std::string& reg_name,
                                   const uint64_t& val) {
  if (!IsRunning()) {
    PR(ERROR) << "Process is not running";
    return StatusType::kNotRunning;
  }

  auto reg = RegisterOperator::GetRegisterFromName(utils::trim(reg_name));
  if (!reg.has_value()) {
    PR(ERROR) << "Unknown register name " << reg_name;
    return StatusType::kUnknownRegister;
  };
  RegisterOperator::SetRegisterValue(pid_, reg.value(), val);
  return StatusType::kSuccess;
}

void Debugger::Quit() {
  std::lock_guard<std::mutex> lock(mutex_);

  PR(INFO) << "Quitting";
  if (IsRunning()) {
    kill(pid_, SIGTERM);
    SetStop();
  }
}

void Debugger::SetRun(pid_t pid) {
  pid_ = pid;
  running_ = true;
}

void Debugger::SetStop() {
  pid_ = 0;
  running_ = false;
}

bool Debugger::IsRunning() const { return running_ && pid_ != 0; }

pid_t Debugger::GetPid() const { return pid_; }

}  // namespace shuidb
