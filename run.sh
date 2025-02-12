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
    cmake -DCMAKE_BUILD_TYPE=Release .. || cd .. && rm -rf build
fi

echo
echo

# Build the app
make App -j`nproc`  VERBOSE=1 || exit 1

echo
echo

# Build the tests
make tests -j`nproc`  VERBOSE=1 || exit 1

# Run the app
./app

# Run the tests
# ctest

# Run valgrind memory leak tests to app
valgrind --leak-check=yes ./app

# valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./app
# nohup kcachegrind callgrind.out >/dev/null 2>&1 &

# Run valgrind memory leak tests to tests.
valgrind --leak-check=yes ./tests/tests


