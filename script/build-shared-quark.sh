#!/bin/bash
repo=al4san-dev
currentdir=`pwd`
hwloc_install_dir=$currentdir/al4san-dev/hwloc-install
quark_install_dir=$currentdir/al4san-dev/quark


currentdir=`pwd`
run_clone=0
set_pkgconfig_runtime_libs=1
run_update_submodules=0
run_module_setup=1
compile_hwloc=0
compile_quark=1
compile_cham=0
compile_al4san=1
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
    export PKG_CONFIG_PATH=$starpu_install_dir/lib/pkgconfig:$PKG_CONFIG_PATH
fi
cd  $currentdir/$repo
if [ $run_update_submodules -eq 1 ];then 
    git submodule update --init --recursive
fi
if [ $run_module_setup -eq 1 ]; then
    . script/modules-ecrc.sh
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
cd  $currentdir/$repo
if [ $compile_quark -eq 1 ];then
    git clone https://github.com/ecrc/quark
    cd quark
    git checkout switchruntime 
    make -j
    make -j install
    export LD_LIBRARY_PATH=$quark_install_dir:$LD_LIBRARY_PATH
    pause_info
fi
cd $currentdir/$repo
if [ $compile_cham -eq 1 ];then
     git clone https://github.com/Rabab53/modified-chameleon.git
    cd modified-chameleon
    cd chameleon
    git checkout rabab/newchameleon
    if [ -d build ]; then
        rm -rf build
    fi
    mkdir build 
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=$currentdir/$repo/modified-chameleon/chameleon/build/install -DCMAKE_COLOR_MAKEFILE:BOOL=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DBUILD_SHARED_LIBS=OFF -DCHAMELEON_ENABLE_EXAMPLE=ON -DCHAMELEON_ENABLE_TESTING=OFF -DCHAMELEON_ENABLE_TIMING=ON -DCHAMELEON_USE_MPI=OFF -DCHAMELEON_USE_CUDA=OFF -DCHAMELEON_SCHED_QUARK=OFF -DCHAMELEON_SCHED_STARPU=ON 
    make -j install
    if [ -d $currentdir/$repo/modified-chameleon/chameleon/build/install ]; then
        export PKG_CONFIG_PATH=$currentdir/$repo/modified-chameleon/chameleon/build/install/lib/pkgconfig/:$PKG_CONFIG_PATH
    fi
    pause_info
fi
cd $currentdir/$repo
if [ $compile_al4san -eq 1 ];then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=$currentdir/$repo/build/install -DAL4SAN_SCHED_QUARK=ON -DQUARK_INCDIR=$quark_install_dir  -DQUARK_LIBDIR=$quark_install_dir  -DCMAKE_COLOR_MAKEFILE:BOOL=ON -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
    make -j
    make install
    if [ -d $currentdir/$repo/build/install ]; then
        export PKG_CONFIG_PATH=$currentdir/$repo/build/install/lib/pkgconfig/:$PKG_CONFIG_PATH
    fi
fi

echo "Everything has finished, returning to initial folder \"$currentdir\""
cd $currentdir
