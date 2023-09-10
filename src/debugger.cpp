#include "debugger.h"

#include <sys/wait.h>

#include "linenoise.h"
#include "output_utils.hpp"
#include "string_utils.hpp"

namespace shuidb {

Debugger::~Debugger(){};

void Debugger::Run() {
  int wait_status;
  waitpid(pid_, &wait_status, 0);

  char* line = nullptr;
  while ((line = linenoise("shuidb> ")) != nullptr) {
    HandleCommand(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

void Debugger::HandleCommand(const std::string& line) {
  auto args = utils::split(line, ' ');
  auto command = args[0];

  if (utils::starts_with(command, "c")) {
    ContinueExecution();
  } else if (utils::starts_with(command, "q") ||
             utils::starts_with(command, "exit")) {
    // FIXME: Not working fine, child process will continue to run
    exit(0);
  } else {
    PR(ERROR) << "Unknown command";
  }
}

void Debugger::ContinueExecution() {
  ptrace(PTRACE_CONT, pid_, nullptr, nullptr);

  int wait_status;
  waitpid(pid_, &wait_status, 0);
}

}  // namespace shuidb
