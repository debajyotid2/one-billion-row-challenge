#!/bin/sh

cd ..

if [ -d build ]; then
    rm -rf build
fi
{ mkdir build; cd build; }|| { echo "Failed to create build directory."; exit 1; }
cmake -DCMAKE_BUILD_TYPE=Debug .. || exit 2
make || exit 3
