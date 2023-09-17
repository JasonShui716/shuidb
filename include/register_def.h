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

#include <array>
#include <string>

namespace shuidb {

enum class Register {
  RAX,
  RBX,
  RCX,
  RDX,
  RDI,
  RSI,
  RBP,
  RSP,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15,
  RIP,
  RFLAGS,
  CS,
  ORIG_RAX,
  FS_BASE,
  GS_BASE,
  FS,
  GS,
  SS,
  DS,
  ES,
};

struct RegDescriptor {
  Register reg;
  int dwarf_r;
  std::string name;
};

constexpr std::size_t kNumRegisters = 27;

// https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf
// Page 57
static const std::array<RegDescriptor, kNumRegisters> kRegisterDescriptors{{
    {Register::RAX, 0, "rax"},
    {Register::RBX, 3, "rbx"},
    {Register::RCX, 2, "rcx"},
    {Register::RDX, 1, "rdx"},
    {Register::RDI, 5, "rdi"},
    {Register::RSI, 4, "rsi"},
    {Register::RBP, 6, "rbp"},
    {Register::RSP, 7, "rsp"},
    {Register::R8, 8, "r8"},
    {Register::R9, 9, "r9"},
    {Register::R10, 10, "r10"},
    {Register::R11, 11, "r11"},
    {Register::R12, 12, "r12"},
    {Register::R13, 13, "r13"},
    {Register::R14, 14, "r14"},
    {Register::R15, 15, "r15"},
    {Register::RIP, -1, "rip"},
    {Register::RFLAGS, 49, "eflags"},
    {Register::CS, 51, "cs"},
    {Register::ORIG_RAX, -1, "orig_rax"},
    {Register::FS_BASE, 58, "fs_base"},
    {Register::GS_BASE, 59, "gs_base"},
    {Register::FS, 54, "fs"},
    {Register::GS, 55, "gs"},
    {Register::SS, 52, "ss"},
    {Register::DS, 53, "ds"},
    {Register::ES, 50, "es"},
}};

}  // namespace shuidb