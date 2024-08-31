#!/bin/sh

cd ..

if [ -d build ]; then
    rm -rf build
fi
# mkdir build && cd build || { echo "Failed to create build directory."; exit 1; }
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
