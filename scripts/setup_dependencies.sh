#!/bin/bash

DEPENDENCY_DIR=../external

cd $DEPENDENCY_DIR || exit 1
git submodule update --init --recursive || exit 7
for dir in ./*; do
    (
        cd "$dir" || exit 1
        if [ "$dir" = "./Criterion" ]; then
            git pull origin master
        else
            git pull origin main
        fi
    )
done

if [ "$1" = "" ]; then
    NUM_THREADS=1
else
    NUM_THREADS=$1
fi

# Install dependencies for criterion
sudo dnf install meson libffi-devel libgit2-devel || exit 1

# Build and install matlibr and openblas
cd matlibr/scripts || exit 2
source install_openblas.sh "$NUM_THREADS" || exit 3
cd ../scripts || exit 4
source install.sh "$NUM_THREADS" || exit 5
echo "Successfully installed matlibr and OpenBLAS"

cd ../../../external || exit 6
 
# Build and install yatpool
cd yatpool/scripts || exit 1
source install.sh "$NUM_THREADS" || exit 1
echo "Successfully installed yatpool"

cd ../../../external || exit 6

# Build and install criterion
cd ./Criterion || exit 1
meson build
sudo meson install -C build
sudo ldconfig
echo "Successfully installed criterion"

cd ../../scripts || exit 6
