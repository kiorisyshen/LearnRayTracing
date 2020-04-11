#!/bin/bash
set -e

mkdir -p build
# rm -rf build/*
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j2