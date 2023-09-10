#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

#include "debugger.h"
#include "fs_utils.hpp"
#include "output_utils.hpp"

using namespace shuidb;

static void handle_signal_quit(int signal) {
  PR(ERROR) << "Received signal " << signal << ", quitting";
  exit(0);
}

int main(int argc, char **argv) {
  signal(SIGINT, handle_signal_quit);
  signal(SIGTERM, handle_signal_quit);

  if (argc < 2) {
    PR(ERROR) << "Programe name not specified";
    return -1;
  }

  PR(INFO) << "Starting shuidb";

  auto prog = argv[1];
  if (!utils::file_exists(prog)) {
    PR(ERROR) << "File " << prog << " does not exist";
    return -1;
  }

  auto pid = fork();
  if (pid == 0) {
    // child process
    signal(SIGHUP, [](int) { exit(0); });
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);
  } else if (pid > 0) {
    // parent process
    Debugger dbg(prog, pid);
    dbg.Run();
  } else {
    PR(ERROR) << "Fork failed";
    return -1;
  }
}