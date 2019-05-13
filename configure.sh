#!/bin/bash
set -exo pipefail

cd $( dirname $( readlink -f "${0}" ) )

export CC=clang
export CXX=clang++

rm -rf build
mkdir build && cd build

mkdir release && cd release
cmake -DCMAKE_BUILD_TYPE=Release ../../src

cd ..

mkdir debug && cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ../../src
