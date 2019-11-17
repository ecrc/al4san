#!/bin/bash

module load mkl/2018-initial
module load gcc/5.5.0
module load cmake/3.9.6
module load hwloc/1.11.8-gcc-5.5.0
#module load openmpi/3.0.0-gcc-5.5.0
module load cuda/9.0
module load ecrc-extras

cd

git clone https://github.com/ecrc/quark.git && cd quark && git checkout switchruntime && make && make install

export QUARKDIR=$PWD

cd 

wget http://starpu.gforge.inria.fr/files/starpu-1.3.3/starpu-1.3.3.tar.gz

tar xvf starpu-1.3.3.tar.gz

cd starpu-1.3.3

mkdir install

./configure --prefix=$PWD/install --disable-opencl --enable-shared --disable-build-doc --disable-export-dynamic --disable-mpi-check  --disable-mpi

make -j && make install

export PKG_CONFIG_PATH=$PWD/install/lib/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
export STARPUDIR=$PWD
export STARPULIB=$PWD/install/lib
export STARPUINC=$PWD/install/include/starpu/1.3


cd

git clone git@gitlab.inria.fr:solverstack/chameleon.git

cd chameleon 

mkdir build

cd build

cmake ..  -DCMAKE_INSTALL_PREFIX=$PWD/install -DCHAMELEON_SCHED_STARPU=ON -DCHAMELEON_ENABLE_CUDA=ON -DCHAMELEON_USE_CUDA=ON 

make -j && make install

export PKG_CONFIG_PATH=$PWD/install/lib/pkgconfig:$PKG_CONFIG_PATH
export CHAMDIR=$PWD
export CHAMLIB=$PWD/install/lib
export CHAMINC=$PWD/install/include

cd

git clone https://github.com/ecrc/al4san-dev.git

cd al4san-dev

mkdir build

cd build

cmake .. -DAL4SAN_SCHED_QUARK=ON -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DQUARK_INCDIR=$QUARKDIR  -DQUARK_LIBDIR=$QUARKDIR -DAL4SAN_SCHED_PARSEC=OFF -DAL4SAN_USE_MPI=OFF  -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_ENABLE_CUDA=ON -DAL4SAN_USE_CUDA=ON

make -j && make install

export PKG_CONFIG_PATH=$PWD/install/lib/pkgconfig:$PKG_CONFIG_PATH
export AL4SANDIR=$PWD
export AL4SANLIB=$PWD/install/lib
export AL4SANINC=$PWD/install/include
