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

#include <stdint.h>
#include <unistd.h>

#include <atomic>
#include <mutex>

namespace shuidb {

class BreakPoint {
 public:
  BreakPoint() = default;
  BreakPoint(pid_t pid, std::intptr_t addr)
      : pid_(pid), addr_(addr), enabled_(false) {}
  void Enable();
  void Disable();
  bool IsEnabled() const;
  std::intptr_t GetAddress() const;

 private:
  pid_t pid_;
  std::intptr_t addr_;
  bool enabled_;
  uint8_t original_data_;
  //   std::mutex mutex_;
};

}  // namespace shuidb