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

inline std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter) {
  std::vector<std::string> tokens;
  std::string::size_type start = 0;
  std::string::size_type end = s.find(delimiter);

  while (end != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }

  tokens.push_back(s.substr(start));

  return tokens;
}

inline std::vector<std::string> split(const std::string &s, char delimiter) {
  return split(s, std::string(1, delimiter));
}

inline bool starts_with(const std::string &s, const std::string &prefix) {
  return s.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), s.begin());
}

inline std::string trim(const std::string &s) {
  auto wsfront = std::find_if_not(s.begin(), s.end(),
                                  [](int c) { return std::isspace(c); });
  auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {
                  return std::isspace(c);
                }).base();
  return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

}  // namespace utils
}  // namespace shuidb
