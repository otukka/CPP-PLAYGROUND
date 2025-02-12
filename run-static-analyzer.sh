#!/bin/bash



# BUILD_DIR="build-clang"

# # Create build folder if not exists
# if [ ! -d $BUILD_DIR ]; then
#     mkdir $BUILD_DIR
#     cd $BUILD_DIR
#     scan-build cmake  ..
# else
#     cd $BUILD_DIR
# fi
# scan-build -v make app


# cd ..

BUILD_DIR="build-cppcheck"
# Create build folder if not exists
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    cmake -DENABLE_CPPCHECK=TRUE ..
else
    cd $BUILD_DIR
fi
make app -j`nproc` || exit 1

mkdir cppcheck
rm -rf cppcheck/*

for row in $(cat compile_commands.json | jq -r '.[] | @base64'); do
    _jq() {
     echo ${row} | base64 --decode | jq -r ${1}
    }

    cppcheck \
    --suppress=unmatchedsuppression:$(_jq '.file') \
    --suppress=missingIncludeSystem \
    --suppress=syntaxError \
    --suppress=ConfigurationNotChecked \
    --enable=all \
    --inconclusive \
    --force \
    --xml \
    --output-file=cppcheck/$(basename `echo $(_jq '.file')`).xml \
    $(_jq '.file')

done