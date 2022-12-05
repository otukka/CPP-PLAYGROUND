#!/bin/bash

# Build docker image if not present
if ! docker images|grep cpp-playground -q; then
    docker build --no-cache -t cpp-playground .
fi

# Create build directory if not exists
if [ ! -d ${PWD}/build ]; then
    mkdir ${PWD}/build
fi

# Run the build
if docker images|grep cpp-playground -q; then
docker run -it --rm  \
-u builder:builder  \
-v ${PWD}/src:/work/src \
-v ${PWD}/inc:/work/inc \
-v ${PWD}/Catch2:/work/Catch2 \
-v ${PWD}/tests:/work/tests \
-v ${PWD}/ThreadSubmodule:/work/ThreadSubmodule \
-v ${PWD}/build:/work/build \
-v ${PWD}/CMakeLists.txt:/work/CMakeLists.txt \
-v ${PWD}/run.sh:/work/run.sh \
-t cpp-playground \
/bin/bash -C /work/run.sh
fi
