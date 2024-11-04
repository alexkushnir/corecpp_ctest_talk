#!/bin/sh

cmake -S . -B ./build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build ./build && cd build/bin/Release && ctest