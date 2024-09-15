#!/bin/sh

cd ..

# Whether to compile with gprof flags
if [ -z "$PROFILE" ]; then
    PROFILE=0
fi

# Whether to compile with debug flags
if [ -z "$DEBUG" ]; then
    DEBUG=0
fi

CMAKE_FLAGS=""

if [ "$PROFILE" = 1 ]; then
    echo "Building with profiling flags ..."
    CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg"
fi
if [ "$DEBUG" = 1 ]; then
    echo "Building with debug flags ..."
    CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_BUILD_TYPE=Debug"
fi

echo "$CMAKE_FLAGS"

if [ -d build ]; then
    rm -rf build
fi
{ mkdir build; cd build; } || { echo "Failed to create build directory."; exit 1; }
if [ "$CMAKE_FLAGS" = "" ]; then
    cmake -DCMAKE_BUILD_TYPE=Release ..
else
    cmake ${CMAKE_FLAGS} ..
fi
make
