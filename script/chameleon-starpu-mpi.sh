#!/bin/bash -le

#export PKG_CONFIG_PATH=/home/omairyrm/starpu-1.2.6/install/lib/pkgconfig/:$PKG_CONFIG_PATH
#export PKG_CONFIG_PATH=/home/omairyrm/al4san-dev/build-starpu-mpi/installdir/lib/pkgconfig:$PKG_CONFIG_PATH

module list

cd

cd chameleon-inria/chameleon

rm -rf build-starpu-mpi

mkdir -p build-starpu-mpi

cd build-starpu-mpi

rm -rf *

cmake ..  -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DCHAMELEON_SCHED_AL4SAN_STARPU=ON -DCHAMELEON_ENABLE_CUDA=OFF -DCHAMELEON_USE_CUDA=OFF  -DCHAMELEON_USE_MPI=ON -DCMAKE_BUILD_TYPE=Debug 

make clean

make -j && make install

export PKG_CONFIG_PATH=$PWD/installdir/lib/pkgconfig:$PKG_CONFIG_PATH
