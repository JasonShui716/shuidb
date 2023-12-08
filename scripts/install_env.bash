#!/bin/bash
set -x

LLVM_VERSION=16
script_path="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" -- $LLVM_VERSION all
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-$LLVM_VERSION 300
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-$LLVM_VERSION 300
sudo update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-$LLVM_VERSION 300
sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-$LLVM_VERSION 300
sudo update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-$LLVM_VERSION 300
sudo update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-$LLVM_VERSION 300
clang -v
clang++ -v

cp $script_path/pre-commit $script_path/../.git/hooks/pre-commit
