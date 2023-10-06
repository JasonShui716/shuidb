# shuidb [![Linux Build](https://github.com/JasonShui716/shuidb/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/JasonShui716/shuidb/actions/workflows/cmake-single-platform.yml)

![shuidb logo](https://s1.imagehub.cc/images/2023/10/06/logo_white_background8ba95866c8148f0e.jpeg)

A more simple & flexible debugger

Begin with `https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/`

But make it more OOP and modern

## Requirements
- clang libc++ >= 15.0
- cmake >= 3.15

## Env set
`sudo scripts/install_env.bash`

## Build
*You can refer to `.github/workflows` to learn more*
```
git submodule update --init
mkdir build && cd build
cmake ..
make
make test
```