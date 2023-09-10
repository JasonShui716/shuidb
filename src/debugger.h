#pragma once

#include <sys/ptrace.h>

#include <string>

namespace shuidb {

class Debugger {
 public:
  Debugger(std::string prog, pid_t pid) : prog_(prog), pid_(pid){};
  ~Debugger();
  void Run();

 private:
  std::string prog_;
  pid_t pid_;

  void HandleCommand(const std::string& line);
  void ContinueExecution();
};

}  // namespace shuidb