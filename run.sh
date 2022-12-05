#!/bin/bash

# Create build folder if not exists
if [[ ! -d "build" ]]; then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug .. || cd .. && rm -rf build
else
    cd build
fi

# Build the app
make || exit 1

# Build the tests
# make test VERBOSE=0 || exit 1

# Run the app
./app

# Run the tests
ctest

# Run valgrind memory leak tests to app
valgrind --leak-check=yes ./app
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./app
# nohup kcachegrind callgrind.out >/dev/null 2>&1 &

# Run valgrind memory leak tests to tests.
valgrind --leak-check=yes ./tests


