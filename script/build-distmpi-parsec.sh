#!/bin/bash
repo=al4san-dev
hwloc_install_dir=$HOME/al4san-dev/hwloc-install-mpi
parsec_install_dir=$HOME/al4san-dev/parsec-install-mpi


currentdir=`pwd`
run_clone=0
set_pkgconfig_runtime_libs=1
run_update_submodules=0
run_module_setup=1
compile_hwloc=0
compile_parsec=1
compile_cham=0
compile_starsh=0
compile_starshcore=0
compile_al4san=1
compile_hcore=1
pause_info(){
    echo "Please press enter key to proceed"; read
    echo "================="
}
pause_step(){
    echo "Please press enter key to proceed"; read
    #echo "================="
}

if [ $run_clone -eq 1 ]; then
if [ ! -d $repo ]; then
    git clone git@github.com:ecrc/$repo.git
    pause_step
else
    echo "Echo \"$repo\" exists so I am not cloning from github"
    pause_info
fi
fi
if [ $set_pkgconfig_runtime_libs -eq 1 ]; then
    export PKG_CONFIG_PATH=$hwloc_install_dir/lib/pkgconfig:$PKG_CONFIG_PATH
    export PKG_CONFIG_PATH=$parsec_install_dir/lib/pkgconfig:$PKG_CONFIG_PATH
fi
cd $currentdir/$repo
if [ $run_update_submodules -eq 1 ];then 
    git submodule update --init --recursive
fi
if [ $run_module_setup -eq 1 ]; then
    . script/modules-cs.sh
    pause_info
fi
if [ $compile_hwloc -eq 1 ];then
    if [  ! -f "hwloc-1.11.13.tar.gz" ]; then
	wget https://download.open-mpi.org/release/hwloc/v1.11/hwloc-1.11.13.tar.gz
        tar -zxvf hwloc-1.11.13.tar.gz
    fi
    if [ -d "hwloc-1.11.13" ]; then
    	rm -rf hwloc-1.11.13
    fi
    tar -zxvf hwloc-1.11.13.tar.gz
    cd hwloc-1.11.13
    [[ -d $hwloc_install_dir ]] || mkdir -p $hwloc_install_dir
    ./configure --prefix=$hwloc_install_dir --disable-libxml2 --disable-pci --enable-shared=yes --enable-static=yes
    make -j
    make -j install
    if [ -d "$hwloc_install_dir/lib/pkgconfig" ]; then
        export PKG_CONFIG_PATH=$hwloc_install_dir/lib/pkgconfig:$PKG_CONFIG_PATH
    fi
    pause_info
fi
cd $currentdir/$repo
if [ $compile_parsec -eq 1 ];then
    git clone https://rabab53@bitbucket.org/yupei2/parsec-future.git
    cd parsec-future
    git checkout dtd_enable_user_trimming
     mkdir build
     cd build
    [[ -d $parsec_install_dir ]] || mkdir -p $parsec_install_dir
    cmake .. -DCMAKE_BUILD_TYPE=Release -DDPLASMA_PRECISIONS="d" -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=$parsec_install_dir -DPARSEC_GPU_WITH_CUDA=OFF -DPARSEC_HAVE_CUDA=OFF -DPARSEC_PROF_TRACE=OFF
    make -j
    make -j install
    if [ -d "$parsec_install_dir/lib/pkgconfig" ]; then
        export PKG_CONFIG_PATH=$parsec_install_dir/lib64/pkgconfig:$PKG_CONFIG_PATH
    fi
    pause_info
fi
cd $currentdir/$repo
if [ $compile_cham -eq 1 ];then
     git clone https://github.com/Rabab53/modified-chameleon.git
    cd modified-chameleon
    cd chameleon
    git checkout rabab/newchameleon
    if [ -d build-mpi ]; then
        rm -rf build-mpi
    fi
    mkdir build-mpi
    cd build-mpi
#    cmake .. -DCMAKE_INSTALL_PREFIX=$currentdir/$repo/modified-chameleon/chameleon/build-mpi/install -DCMAKE_COLOR_MAKEFILE:BOOL=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DBUILD_SHARED_LIBS=OFF -DCHAMELEON_ENABLE_EXAMPLE=ON -DCHAMELEON_ENABLE_TESTING=ON -DCHAMELEON_ENABLE_TIMING=ON -DCHAMELEON_USE_MPI=ON -DCHAMELEON_USE_CUDA=OFF -DCHAMELEON_SCHED_QUARK=OFF -DCHAMELEON_SCHED_STARPU=ON \
#    -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_intel_lp64;-lmkl_core;-lmkl_sequential;-lpthread;-lm;-ldl" -DBLAS_COMPILER_FLAGS="-m64;-I${MKLROOT}/include" -DLAPACK_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_intel_lp64;-lmkl_core;-lmkl_sequential;-lpthread;-lm;-ldl" -DCBLAS_DIR="${MKLROOT}" -DLAPACKE_DIR="${MKLROOT}" -DTMG_DIR="${MKLROOT}" \
#    -DMPI_C_COMPILER=`which mpicc`

    cmake .. -DCMAKE_INSTALL_PREFIX=$currentdir/$repo/modified-chameleon/chameleon/build-mpi/install -DCMAKE_COLOR_MAKEFILE:BOOL=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DBUILD_SHARED_LIBS=OFF -DCHAMELEON_ENABLE_EXAMPLE=ON -DCHAMELEON_ENABLE_TESTING=ON -DCHAMELEON_ENABLE_TIMING=ON -DCHAMELEON_USE_MPI=ON -DCHAMELEON_USE_CUDA=OFF -DCHAMELEON_SCHED_QUARK=OFF -DCHAMELEON_SCHED_STARPU=ON 
    make -j install
    if [ -d $currentdir/$repo/modified-chameleon/chameleon/build-mpi/install ]; then
        export PKG_CONFIG_PATH=$currentdir/$repo/modified-chameleon/chameleon/build-mpi/install/lib/pkgconfig/:$PKG_CONFIG_PATH
    fi
    pause_info
fi
cd $currentdir/$repo
if [ $compile_al4san -eq 1 ];then
    rm -rf build-parsec-mpi
    mkdir build-parsec-mpi
    cd build-parsec-mpi
    cmake .. -DCMAKE_INSTALL_PREFIX=$currentdir/$repo/build-parsec-mpi/install  -DAL4SAN_SCHED_PARSEC=ON -DAL4SAN_USE_MPI=ON -DAL4SAN_ENABLE_CUDA=OFF -DAL4SAN_USE_CUDA=OFF
    make -j
    make install
    if [ -d $currentdir/$repo/build-parsec-mpi/install ]; then
        export PKG_CONFIG_PATH=$currentdir/$repo/build-parsec-mpi/install/lib/pkgconfig/:$PKG_CONFIG_PATH
    fi

fi

echo "Everything has finished, returning to initial folder \"$currentdir\""
cd $currentdir
