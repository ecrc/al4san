pipeline {

/*
AL4SAN
*/

    agent { label 'jenkinsfile' }
    triggers {
        pollSCM('H/10 * * * *')
    }
    stages {
        stage ('build-starpu') {
            steps {
                sh '''#!/bin/bash -el
                module purge
                module load ecrc-extras
                module load gcc/5.5.0
                module load mkl/2018-update-2
                module load starpu/1.2.4-gcc-5.5.0-mkl-openmpi-3.0.0

                mkdir -p build
                cd build
                rm -rf ./*
                cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_ENABLE_CUDA=OFF -DAL4SAN_USE_CUDA=OFF
                make
                make install
                '''
            }
        }
        stage ('build-parsec') {
            steps {
                sh '''#!/bin/bash -el
                module purge
                module load ecrc-extras
                module load gcc/5.5.0
                module load mkl/2018-update-2
                module load openmpi/3.0.0-gcc-5.5.0
                module load parsec/master-gcc-5.5.0-mkl-openmpi-plasma-2.8.0

                mkdir -p build
                cd build
                rm -rf ./*
                cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_PARSEC=ON -DAL4SAN_USE_MPI=ON -DAL4SAN_ENABLE_DOC=ON
                make
                make docs
                make install
                '''
            }
        }
        stage ('build-quark') {
            steps {
                sh '''#!/bin/bash -el
                module purge
                module load ecrc-extras
                module load gcc/5.5.0
                module load mkl/2018-update-2

                mkdir -p build
                cd build

                # quark
                git clone https://github.com/ecrc/quark
                cd quark
                make
                make install prefix=$WORKSPACE/quark
                cd ..

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
            steps {
                sh '''#!/bin/bash -el
                module purge
                module load ecrc-extras
                module load llvm/6.0.1-flang
                module load mkl/2018-update-2


                mkdir -p build
                cd build

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
