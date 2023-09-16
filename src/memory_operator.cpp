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

#include "memory_operator.h"

#include <sys/ptrace.h>

namespace shuidb {

void MemoryOperator::WriteMemory(pid_t pid, uint64_t addr, uint64_t data) {
  ptrace(PTRACE_POKEDATA, pid, addr, data);
}

uint64_t MemoryOperator::ReadMemory(pid_t pid, uint64_t addr) {
  return ptrace(PTRACE_PEEKDATA, pid, addr, nullptr);
}

}  // namespace shuidb