#!/bin/bash

# Create build folder if not exists
if [[ ! -d "build" ]]; then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Relese ..
else
    cd build
fi

# Build the app
make VERBOSE=0 || exit 1

# Build the tests
make test VERBOSE=0 || exit 1

# Run the app
./app

# Run the tests
./tests

# Run valgrind memory leak tests to app
valgrind --leak-check=yes ./app

# Run valgrind memory leak tests to tests.
valgrind --leak-check=yes ./tests