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

#include <iostream>
#include <string>
#include <unordered_map>

#define PR(x) PR_##x
#define PR_ERROR shuidb::utils::Print(std::cerr, shuidb::utils::Color::RED)
#define PR_INFO shuidb::utils::Print(std::cout, shuidb::utils::Color::CYAN)

namespace shuidb {
namespace utils {

enum class Color {
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  BRIGHT_BLACK,
  BRIGHT_RED,
  BRIGHT_GREEN,
  BRIGHT_YELLOW,
  BRIGHT_BLUE,
  BRIGHT_MAGENTA,
  BRIGHT_CYAN,
  BRIGHT_WHITE,
  RESET
};

struct Print {
 public:
  Print(std::ostream& os, Color c) : os_(os), color_(c){};
  template <typename T>
  Print& operator<<(const T& x) {
    os_ << color_codes[color_] << x << color_codes[Color::RESET];
    return *this;
  }

  ~Print() { os_ << std::endl; }

 private:
  std::ostream& os_;
  Color color_;
  std::unordered_map<Color, std::string> color_codes = {
      {Color::BLACK, "\033[30m"},        {Color::RED, "\033[31m"},
      {Color::GREEN, "\033[32m"},        {Color::YELLOW, "\033[33m"},
      {Color::BLUE, "\033[34m"},         {Color::MAGENTA, "\033[35m"},
      {Color::CYAN, "\033[36m"},         {Color::WHITE, "\033[37m"},
      {Color::BRIGHT_BLACK, "\033[90m"}, {Color::BRIGHT_RED, "\033[91m"},
      {Color::BRIGHT_GREEN, "\033[92m"}, {Color::BRIGHT_YELLOW, "\033[93m"},
      {Color::BRIGHT_BLUE, "\033[94m"},  {Color::BRIGHT_MAGENTA, "\033[95m"},
      {Color::BRIGHT_CYAN, "\033[96m"},  {Color::BRIGHT_WHITE, "\033[97m"},
      {Color::RESET, "\033[0m"}};
};

}  // namespace utils
}  // namespace shuidb