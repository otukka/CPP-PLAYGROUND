#!/bin/bash


BUILD_DIR="build"

# Create build folder if not exists
if [ ! -d $BUILD_DIR ]; then
    mkdir build
fi

# Init cmake if not init before
if [ "$(ls -A $BUILD_DIR)" ]; then
    cd build
else
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug .. || cd .. && rm -rf build
fi

make -j`nproc` || exit 1

make tests || exit 1

if [ $# -eq 0 ]; then
    ctest
else
    ctest -R "$1"
fi

