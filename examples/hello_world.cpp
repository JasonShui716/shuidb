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
#include <iostream>
#include <ranges>

int main() {
  const auto nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int i : std::ranges::drop_view{nums, 2}) std::cout << i << ' ';
  std::cout << '\n';

  for (int i : nums | std::views::drop(2)) std::cout << i << ' ';
  std::cout << '\n';

  for (int i : std::views::iota(1, 10) | std::views::drop(2))
    std::cout << i << ' ';
  std::cout << '\n';
}