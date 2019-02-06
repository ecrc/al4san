pipeline {

/*
AL4SAN
*/

    agent none
    triggers {
        pollSCM('H/10 * * * *')
    }
    stages {
        stage('Parallel Build Stage') {
            failFast false
            parallel {
                stage ('build-starpu') {
                    agent { label 'jenkinsfile' }
                    steps {
                        sh '''#!/bin/bash -el
                        module purge
                        module load ecrc-extras
                        module load gcc/5.5.0
                        module load mkl/2018-update-2
                        module load starpu/1.2.4-gcc-5.5.0-mkl-openmpi-3.0.0

                        mkdir -p build-starpu
                        cd build-starpu
                        rm -rf ./*
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_ENABLE_CUDA=OFF -DAL4SAN_USE_CUDA=OFF
                        make
                        make install
                        '''
                    }
                }
                stage ('build-parsec') {
                    agent { label 'jenkinsfile' }
                    steps {
                        sh '''#!/bin/bash -el
                        module purge
                        module load ecrc-extras
                        module load gcc/5.5.0
                        module load mkl/2018-update-2
                        module load openmpi/3.0.0-gcc-5.5.0
                        module load parsec/master-gcc-5.5.0-mkl-openmpi-plasma-2.8.0

                        mkdir -p build-parsec
                        cd build-parsec
                        rm -rf ./*
                        # parsec
                        git clone https://bitbucket.org/icldistcomp/parsec.git parsec-src
                        export CPATH=$CPATH:$PWD/parsec-src
                        # chameleon
                        git clone https://gitlab.inria.fr/solverstack/chameleon.git chameleon-src
                        cd chameleon-src; git submodule update --init --recursive; mkdir -p build; cd build
                        cmake .. -DCMAKE_INSTALL_PREFIX=$WORKSPACE/chameleon -DCHAMELEON_SCHED="PARSEC" -DCHAMELEON_USE_MPI=ON
                        make -j 4
                        make install
                        export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$WORKSPACE/chameleon/lib/pkgconfig
                        cd $WORKSPACE/build-parsec

                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_PARSEC=ON -DAL4SAN_USE_MPI=ON -DAL4SAN_ENABLE_DOC=ON
                        make
                        make docs
                        make install
                        '''
                    }
                }
                stage ('build-quark') {
                    agent { label 'jenkinsfile' }
                    steps {
                        sh '''#!/bin/bash -el
                        module purge
                        module load ecrc-extras
                        module load gcc/5.5.0
                        module load mkl/2018-update-2

                        mkdir -p build-quark
                        cd build-quark
                        rm -rf ./*


                        # quark
                        git clone https://github.com/ecrc/quark
                        cd quark
                        make
                        make install prefix=$WORKSPACE/quark
                        cd $WORKSPACE/build-quark

                        # chameleon
                        git clone https://gitlab.inria.fr/solverstack/chameleon.git chameleon-src
                        cd chameleon-src; git submodule update --init --recursive; mkdir -p build; cd build
                        cmake .. -DCMAKE_INSTALL_PREFIX=$WORKSPACE/chameleon -DCHAMELEON_SCHED="QUARK" -DQUARK_INCDIR=$WORKSPACE/quark/include -DQUARK_LIBDIR=$WORKSPACE/quark/lib
                        make -j 4
                        make install
                        export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$WORKSPACE/chameleon/lib/pkgconfig
                        cd $WORKSPACE/build-quark

                        # al4san
                        rm -rf ./*
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_QUARK=ON -DQUARK_INCDIR=$WORKSPACE/quark/include -DQUARK_LIBDIR=$WORKSPACE/quark/lib -DAL4SAN_ENABLE_EXAMPLE=ON
                        make
                        make test
                        make install
                        '''
                    }
                }
                stage ('build-clang-openmp') {
                    agent { label 'jenkinsfile' }
                    steps {
                        sh '''#!/bin/bash -el
                        module purge
                        module load ecrc-extras
                        module load llvm/6.0.1-flang
                        module load mkl/2018-update-2


                        mkdir -p build-openmp
                        cd build-openmp

                        # al4san
                        export CC=clang
                        export CXX=clang++
                        export FC=flang
                        rm -rf ./*
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_OPENMP=ON -DAL4SAN_ENABLE_EXAMPLE=ON
                        make
                        make test
                        make install
                        '''
                    }
                }
            }
        }
    }
}
