#!/bin/bash

build_path="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/../build
pushd $build_path
for file in ./test/*
do
    if [[ -f "$file" && -x "$file" ]]
    then
        "$file"
    fi
done
popd
