#!/bin/bash

DEP_DIR=../external

cd $DEP_DIR || exit 1
git submodule update --init --recursive || exit 7
git submodule foreach git pull origin main || exit 7

if [ "$1" = "" ]; then
    NUM_THREADS=1
else
    NUM_THREADS=$1
fi

# Build and install matlibr and openblas
cd matlibr/scripts || exit 2
source install_openblas.sh "$NUM_THREADS" || exit 3
cd ../scripts || exit 4
source install.sh "$NUM_THREADS" || exit 5
echo "Successfully installed matlibr and OpenBLAS"

cd ../../../scripts || exit 6
cd $DEP_DIR || exit 7

# Build and install yatpool
cd yatpool/scripts || exit 1
source install.sh "$NUM_THREADS" || exit 1
echo "Successfully installed yatpool"

cd ../../../scripts || exit 6
