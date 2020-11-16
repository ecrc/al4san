# AL4SAN:  Abstraction Layer For Standardizing APIs of task-based eNgines.
AL4SAN stands for an Abstraction Layer library For Standardizing APIs of task-based eNgines. 
AL4SAN is designed as a lightweight software library, which provides a collection of APIs to unify the expression of tasks and their data dependencies from existing dynamic engines. AL4SAN supports various dynamic runtime systems relying on compiler infrastructure technology and on library-defined APIs. It features an abstraction of task-based engines and, therefore, enables a single-code application to assess various runtimes and their respective scheduling components, while further leveraging the user-obliviousness of the underlying complex hardware configurations. 

This abstraction paradigm improves user productivity by facilitating the design of high performance algorithms on different type of architectures:
shared-memory systems, GPU-based nodes, distributed-memory hardware configurations, etc.

![Handout](doc/al4san-handout-final.png)

## Get AL4SAN

  To use latest development of AL4SAN, please clone the master
  branch. Note that AL4SAN contains a ~git submodule~ *al4san_cmake*.
  To get sources please use these commands:

```sh
    git clone https://github.com/ecrc/al4san
    cd al4san
    git submodule update --recursive --init
```

## Build AL4SAN

  AL4SAN uses CMake as a build generator, you need to call `cmake` in order to create the Makefiles

### QUARK
   You can get quark from our fork https://github.com/ecrc/quark

```sh
    git clone https://github.com/ecrc/quark
    cd quark
    git checkout switchruntime
    make
    make install prefix=<quark-installation-prefix>

    # AL4SAN
    cd al4san
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=<install-prefix> -DAL4SAN_SCHED_QUARK=ON -DQUARK_INCDIR=<quark-install-prefix>/include -DQUARK_LIBDIR=<quark-install-prefix>/lib -DAL4SAN_ENABLE_EXAMPLE=ON
    make
    make test
    make install
```

### StarPU
```sh
    # AL4SAN
    cd al4san
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=<install-prefix> -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_ENABLE_CUDA=OFF -DAL4SAN_USE_CUDA=OFF
    make
    make install
```

### PaRSEC
  You can get PaRSEC from https://bitbucket.org/icldistcomp/parsec
```sh
    # AL4SAN
    cd al4san
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=<install-prefix> -DAL4SAN_SCHED_PARSEC=ON -DAL4SAN_USE_MPI=ON
    make
    make install
```

### OpenMP-LLVM
```sh
    # LLVM
    export CC=clang
    export CXX=clang++
    export FC=flang

    # AL4SAN
    cd al4san
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=<install-prefix> -DAL4SAN_SCHED_OPENMP=ON -DAL4SAN_ENABLE_EXAMPLE=ON
    make
    make test
    make install
```
## References
1. AL4SAN: Abstraction Layer For Standardizing APIs of Task-Based Engines,
R. Alomairy, H. Ltaief, M. Abduljabbar & D. Keyes. IEEE Transactions on
Parallel and Distributed Systems, vol. 31, no. 11, pp. 2482-2495, 1 Nov. 2020,
doi: 10.1109/TPDS.2020.2992923.

2. AL4SAN: Abstraction Layer For Standardizing APIs of Task-Based Engines,
R. Alomairy, H. Ltaief, M. Abduljabbar & D. Keyes. Poster at ISC High Performance 
conference, 2020. Available at: https://www.youtube.com/watch?v=xgrQT-igXp8

## Acknowledgment
The authors would like to thank Yu Pei from Innovative Computing Laboratory at University of Tennessee for his helping to extend AL4SAN to support PaRSEC (DTD) runtime system in shared- and distributed- memory system.  

## Contact
Rabab Alomairy: rabab.omairy@kaust.edu.sa

Hatem Ltaief: hatem.ltaief@kaust.edu.sa

Mustafa Abduljabbar: mustafa.abduljabbar@kaust.edu.sa 


