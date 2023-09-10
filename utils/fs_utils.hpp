#pragma once
#include <sys/stat.h>

#include <string>

namespace shuidb {
namespace utils {

bool file_exists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

}  // namespace utils
}  // namespace shuidb
