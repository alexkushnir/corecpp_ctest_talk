#!/bin/sh

ctest --build-and-test . ./build --build-generator Ninja --test-command ctest --test-dir test