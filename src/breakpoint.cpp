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

#include "breakpoint.h"

#include <sys/ptrace.h>

namespace shuidb {

void BreakPoint::Enable() {
  // std::lock_guard<std::mutex> lock(mutex_);

  auto data = ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr);
  original_data_ = data & 0xff;
  uint64_t int3 = 0xcc;
  uint64_t data_with_int3 = ((data & ~0xff) | int3);
  ptrace(PTRACE_POKEDATA, pid_, addr_, data_with_int3);
  enabled_ = true;
};

void BreakPoint::Disable() {
  // std::lock_guard<std::mutex> lock(mutex_);

  auto data = ptrace(PTRACE_PEEKDATA, pid_, addr_, nullptr);
  auto restored_data = ((data & ~0xff) | original_data_);
  ptrace(PTRACE_POKEDATA, pid_, addr_, restored_data);
  enabled_ = false;
};

}  // namespace shuidb