#!/bin/bash

sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
sudo apt install -y libc++abi-17-dev libc++-17-dev lldb-17 clangd-17
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-17 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-17 100
sudo update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-17 100
sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-17 100
clang -v
clang++ -v