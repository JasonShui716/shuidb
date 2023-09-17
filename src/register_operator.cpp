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

#include "register_operator.h"

#include <sys/ptrace.h>
#include <sys/user.h>

#include <iomanip>

#include "output_utils.hpp"

namespace shuidb {

uint64_t RegisterOperator::GetRegisterValue(pid_t pid, Register reg) {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
  switch (reg) {
    case Register::RAX:
      return regs.rax;
    case Register::RBX:
      return regs.rbx;
    case Register::RCX:
      return regs.rcx;
    case Register::RDX:
      return regs.rdx;
    case Register::RDI:
      return regs.rdi;
    case Register::RSI:
      return regs.rsi;
    case Register::RBP:
      return regs.rbp;
    case Register::RSP:
      return regs.rsp;
    case Register::R8:
      return regs.r8;
    case Register::R9:
      return regs.r9;
    case Register::R10:
      return regs.r10;
    case Register::R11:
      return regs.r11;
    case Register::R12:
      return regs.r12;
    case Register::R13:
      return regs.r13;
    case Register::R14:
      return regs.r14;
    case Register::R15:
      return regs.r15;
    case Register::RIP:
      return regs.rip;
    case Register::RFLAGS:
      return regs.eflags;
    case Register::CS:
      return regs.cs;
    case Register::ORIG_RAX:
      return regs.orig_rax;
    case Register::FS_BASE:
      return regs.fs_base;
    case Register::GS_BASE:
      return regs.gs_base;
    case Register::FS:
      return regs.fs;
    case Register::GS:
      return regs.gs;
    case Register::SS:
      return regs.ss;
    case Register::DS:
      return regs.ds;
    case Register::ES:
      return regs.es;
  }
};

void RegisterOperator::SetRegisterValue(pid_t pid, Register reg,
                                        uint64_t value) {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
  switch (reg) {
    case Register::RAX:
      regs.rax = value;
      break;
    case Register::RBX:
      regs.rbx = value;
      break;
    case Register::RCX:
      regs.rcx = value;
      break;
    case Register::RDX:
      regs.rdx = value;
      break;
    case Register::RDI:
      regs.rdi = value;
      break;
    case Register::RSI:
      regs.rsi = value;
      break;
    case Register::RBP:
      regs.rbp = value;
      break;
    case Register::RSP:
      regs.rsp = value;
      break;
    case Register::R8:
      regs.r8 = value;
      break;
    case Register::R9:
      regs.r9 = value;
      break;
    case Register::R10:
      regs.r10 = value;
      break;
    case Register::R11:
      regs.r11 = value;
      break;
    case Register::R12:
      regs.r12 = value;
      break;
    case Register::R13:
      regs.r13 = value;
      break;
    case Register::R14:
      regs.r14 = value;
      break;
    case Register::R15:
      regs.r15 = value;
      break;
    case Register::RIP:
      regs.rip = value;
      break;
    case Register::RFLAGS:
      regs.eflags = value;
      break;
    case Register::CS:
      regs.cs = value;
      break;
    case Register::ORIG_RAX:
      regs.orig_rax = value;
      break;
    case Register::FS_BASE:
      regs.fs_base = value;
      break;
    case Register::GS_BASE:
      regs.gs_base = value;
      break;
    case Register::FS:
      regs.fs = value;
      break;
    case Register::GS:
      regs.gs = value;
      break;
    case Register::SS:
      regs.ss = value;
      break;
    case Register::DS:
      regs.ds = value;
      break;
    case Register::ES:
      regs.es = value;
      break;
  }
  ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
};

uint64_t RegisterOperator::GetRegisterValueFromDwarfRegister(pid_t pid,
                                                             int dwarf_r) {
  auto it = std::find_if(
      kRegisterDescriptors.begin(), kRegisterDescriptors.end(),
      [dwarf_r](const RegDescriptor& rd) { return rd.dwarf_r == dwarf_r; });

  if (it == kRegisterDescriptors.end()) {
    throw std::runtime_error("Unknown dwarf register");
  }

  return GetRegisterValue(pid, it->reg);
};

std::string RegisterOperator::GetRegisterName(Register reg) {
  auto it =
      std::find_if(kRegisterDescriptors.begin(), kRegisterDescriptors.end(),
                   [reg](const RegDescriptor& rd) { return rd.reg == reg; });

  if (it == kRegisterDescriptors.end()) {
    throw std::runtime_error("Unknown register");
  }

  return it->name;
};

std::optional<Register> RegisterOperator::GetRegisterFromName(const std::string& name) {
  auto it =
      std::find_if(kRegisterDescriptors.begin(), kRegisterDescriptors.end(),
                   [name](const RegDescriptor& rd) { return rd.name == name; });

  if (it == kRegisterDescriptors.end()) {
    return std::nullopt;
  }

  return it->reg;
};

void RegisterOperator::DumpRegisters(pid_t pid) {
  for (const auto& rd : kRegisterDescriptors) {
    auto value = GetRegisterValue(pid, rd.reg);
    PR_RAW << rd.name << " 0x" << std::setfill('0') << std::setw(16) << std::hex
           << value;
  }
};

bool RegisterOperator::IsRegisterValid(const std::string& reg_name) {
  auto it =
      std::find_if(kRegisterDescriptors.begin(), kRegisterDescriptors.end(),
                   [reg_name](const RegDescriptor& rd) {
                     return rd.name == reg_name;
                   });

  return it != kRegisterDescriptors.end();
};

}  // namespace shuidb