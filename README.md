# shuidb
Refer to  `https://blog.tartanllama.xyz/writing-a-linux-debugger-setup/`

But make it more OOP and modern

## Requirements
- clang libc++ >= 15.0
- cmake >= 3.15

## Env set
`sudo scripts/install_env.bash`

## Build
```
mkdir build && cd build
cmake ..
make
make test
```