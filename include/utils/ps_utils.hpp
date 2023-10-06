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

#include <unistd.h>

#include <fstream>
#include <iomanip>
namespace shuidb {
namespace utils {

inline std::intptr_t GetProcessLoadAddress(pid_t pid) {
  std::ifstream ifs("/proc/" + std::to_string(pid) + "/maps");
  std::string line;
  std::getline(ifs, line);
  auto addr = std::stol(line.substr(0, line.find('-')), 0, 16);
  return addr;
}

}  // namespace utils
}  // namespace shuidb