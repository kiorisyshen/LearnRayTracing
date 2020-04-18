#!/bin/bash
set -e

git submodule update --init ThirdParty/spdlog
git submodule update --init ThirdParty/stb

mkdir -p build
# rm -rf build/*
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j2