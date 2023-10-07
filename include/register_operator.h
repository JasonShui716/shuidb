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

#include <sys/user.h>
#include <unistd.h>

#include <optional>
#include <unordered_map>

#include "register_def.h"

namespace shuidb {

class RegisterOperator {
 public:
  static std::optional<std::unordered_map<Register, uint64_t>> GetRegisters(
      pid_t pid);
  static std::optional<uint64_t> GetRegisterValue(pid_t pid, Register reg);
  static uint64_t GetRegisterValue(const user_regs_struct& regs,
                                   const Register reg);
  static void SetRegisterValue(pid_t pid, Register reg, uint64_t value);
  static std::optional<uint64_t> GetRegisterValueFromDwarfRegister(pid_t pid,
                                                                   int dwarf_r);
  static std::string GetRegisterName(Register reg);
  static std::optional<Register> GetRegisterFromName(const std::string& name);
};

}  // namespace shuidb