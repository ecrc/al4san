pipeline {

/*
AL4SAN
*/

    agent none
    options {
        disableConcurrentBuilds()
        buildDiscarder(logRotator(numToKeepStr: '15'))
        timestamps()
    }
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
                        git checkout switchruntime 
                        make
                        make install prefix=$WORKSPACE/quark
                        cd $WORKSPACE/build-quark

                        # al4san
                        rm -rf ./*
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_QUARK=ON -DQUARK_INCDIR=$WORKSPACE/quark/include -DQUARK_LIBDIR=$WORKSPACE/quark/lib -DAL4SAN_ENABLE_EXAMPLE=OFF
                        make
                        #make test
                        make install
                        '''
                    }
                }
                stage ('build-quark-starpu') {
                    agent { label 'jenkinsfile-gpu' }
                    steps {
                        sh '''#!/bin/bash -el
                        module purge
                        module load ecrc-extras
                        module load gcc/5.5.0
                        module load mkl/2018-update-2
                        module load cmake/3.9.6
                        module load hwloc/2.0.3-gcc-5.5.0
                        module load cuda/9.0
                        module load starpu/1.3.3-gcc-5.5.0-cuda-9.0-mkl-openmpi-3.0.0

                        mkdir -p build-quark-starpu
                        cd build-quark-starpu
                        rm -rf ./*


                        # quark
                        git clone https://github.com/ecrc/quark
                        cd quark
                        git checkout switchruntime
                        make
                        make install prefix=$WORKSPACE/quark
                        cd $WORKSPACE/build-quark-starpu

                        # al4san
                        rm -rf ./*
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_QUARK=ON -DQUARK_INCDIR=$WORKSPACE/quark/include -DQUARK_LIBDIR=$WORKSPACE/quark/lib -DAL4SAN_ENABLE_EXAMPLE=OFF -DAL4SAN_SCHED_PARSEC=OFF -DAL4SAN_USE_MPI=OFF  -DAL4SAN_SCHED_STARPU=ON -DAL4SAN_ENABLE_CUDA=ON -DAL4SAN_USE_CUDA=ON


                        make
                        #make test
                        make install
                        '''
                    }
                }
/***   omit this stage for now
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
                        cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DAL4SAN_SCHED_OPENMP=ON -DAL4SAN_ENABLE_EXAMPLE=OFF
                        make
                        #make test
                        make install
                        '''
                    }
                }
*/
            }
        }
    }
    post {
        //always {
        //}
        //success {
        //}
        //unstable {
        //}
        //failure {
        //}
         unstable {
             emailext body: "${env.JOB_NAME} - Please go to ${env.BUILD_URL}", subject: "Jenkins Pipeline build is UNSTABLE", recipientProviders: [culprits()]
         }
         failure {
             emailext body: "${env.JOB_NAME} - Please go to ${env.BUILD_URL}", subject: "Jenkins Pipeline build FAILED", recipientProviders: [culprits(),requestor()]
         }
     }
}
