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
