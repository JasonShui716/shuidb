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

#include <string>
#include <vector>

namespace shuidb {
namespace utils {

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string::size_type start = 0;
  std::string::size_type end = s.find(delimiter);

  while (end != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + 1;
    end = s.find(delimiter, start);
  }

  tokens.push_back(s.substr(start));

  return tokens;
}

bool starts_with(const std::string &s, const std::string &prefix) {
  return s.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), s.begin());
}

}  // namespace utils
}  // namespace shuidb
