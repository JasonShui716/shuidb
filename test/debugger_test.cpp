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

#include "debugger.h"

#include <memory>

#include "gtest/gtest.h"
#include "utils/ps_utils.hpp"

namespace shuidb {
class DebuggerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    debugger_ = std::make_shared<Debugger>("examples/hello_world");
    ASSERT_EQ(debugger_->IsRunning(), false);
    debugger_->RunProc();
    ASSERT_EQ(debugger_->IsRunning(), true);
  }

  std::shared_ptr<Debugger> debugger_;
};

TEST_F(DebuggerTest, ContinueTest) {
  debugger_->ContinueExecution();
  ASSERT_EQ(debugger_->IsRunning(), false);
}

TEST_F(DebuggerTest, BreakPointTest) {
  debugger_->SetBreakPointAtAddress(
      utils::GetProcessLoadAddress(debugger_->GetPid()) + 0x01220);
  debugger_->ContinueExecution();
  ASSERT_EQ(debugger_->IsRunning(), true);
}

}  // namespace shuidb
