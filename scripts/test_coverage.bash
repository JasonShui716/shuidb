#!/bin/bash
set -e

open=false
for param in $@
do
  if [[ "$param" == "--open" ]]
  then
    open=true
  fi
done

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/..

rm -rf build coverage
mkdir -p build && pushd build
cmake -DCMAKE_BUILD_TYPE=Coverage ..
make -j8
bash $root_dir/scripts/run_all_tests.bash

llvm-profdata merge -sparse default.profraw -o main.profdata
llvm-cov show --format=html test/debugger_test -instr-profile=main.profdata --output-dir=../coverage ../src/ ../include/
if [[ "$open" == "true" ]]
then
  xdg-open ../coverage/index.html
fi
popd
