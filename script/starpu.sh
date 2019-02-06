#!/bin/bash -le

#export PKG_CONFIG_PATH=/home/omairyrm/starpu-1.2.6/install/lib/pkgconfig/:$PKG_CONFIG_PATH

module list

cd ..

rm -rf build-starpu

mkdir -p build-starpu

cd build-starpu

rm -rf *

cmake ..  -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_USE_MPI=OFF -DAL4SAN_ENABLE_CUDA=OFF -DAL4SAN_USE_CUDA=OFF

make clean

make -j && make install

export PKG_CONFIG_PATH=$PWD/installdir/lib/pkgconfig:$PKG_CONFIG_PATH
