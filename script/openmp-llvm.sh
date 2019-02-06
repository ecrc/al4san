#!/bin/bash -le

module load ecrc-extras
module load llvm/6.0.1-flang
module list

export CC=clang
export CXX=clang++
export FC=flang

cd ..

rm -rf build-openmp-llvm

mkdir -p build-openmp-llvm

cd build-openmp-llvm

rm -rf *

cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_OPENMP=ON 

make clean

make -j && make install

export PKG_CONFIG_PATH=$PWD/installdir/lib/pkgconfig:$PKG_CONFIG_PATH
