CC =
LINK = $(CC) 

#LDFLAGS = -O3 
#CFLAGS = -g -ldl -Wall -Wno-unused-function -DAL4SAN_USE_CUDA -DAL4SAN_CUDA_ASYNC1
CFLAGS =  -ldl -Wno-unused-parameter -multiply_defined 


#F77BLASL= -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -fopenmp -lpthread -lm -lgfortran

#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread  -L/opt/ecrc/cuda/9.0/lib64/ -lcuda -lcudart -lcublas -L/home/omairyrm/al4san-switch/al4san-dev/build/install/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-starpu-gpu-gcc/install/lib -lchameleon -lchameleon_starpu -lcoreblas -lcudablas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/install/lib -lparsec -L/home/omairyrm/starpu-1.3.3.28Oct.2019/install/lib -lstarpu-1.3

#starpu mpi
#LIBS  =   -lpthread  -L../../hwloc-install-mpi/lib/ -lhwloc -L/opt/ecrc/openmpi/4.0.1-gcc-8.3.0/ub16/lib -lmpi -L../../build-mpi/install/lib -lal4san -lal4san_starpu  -L../../starpu-1.3-install-mpi/lib/ -lstarpu-1.3 -lstarpumpi-1.3 -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core

#starpu
#LIBS  = -lpthread  -L../../hwloc-install/lib/ -lhwloc -L../../build/install/lib -lal4san -lal4san_starpu   -L../../starpu-1.3-install/lib/ -lstarpu-1.3 -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core

#parsec mpi
#LIBS  =   -lpthread  -lhwloc -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib -lmpi -L../../build-parsec/install/lib/ -lal4san  -lal4san_parsec   -L/opt/ecrc/parsec/master-gcc-5.5.0-mkl-openmpi-plasma-2.8.0/ub16/lib -lparsec -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -ldl

#parsec mpi
LIBS  =   -lpthread  -lhwloc -L/opt/intel/compilers_and_libraries_2019.5.281/linux/mpi/intel64/lib/release -lmpi -L../../build-parsec/install/lib/ -lal4san  -lal4san_parsec   -L/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/lib64/ -lparsec -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -ldl

#LIBS  =   -lpthread  -lhwloc -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib -lmpi -L../../build-parsec/install/lib -lal4san -lal4san_parsec   -L/opt/ecrc/parsec/master-gcc-5.5.0-mkl-openmpi-plasma-2.8.0/ub16/lib -lparsec -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -ldl

#quark

#LIBS  =   -lhwloc -lpthread  -L/home/omairyrm/quark -lquark -L../../build-quark/install/lib -lal4san -lal4san_quark    -L${MKLROOT}/lib/intel64 -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -ldl

#INCLUDES  =  -I/home/omairyrm/al4san-switch/al4san-dev/build/install/include -I/home/omairyrm/chameleon/build-starpu-gpu-gcc/install/include  -I/opt/ecrc/starpu/1.3.1-gcc-5.5.0-mkl-openmpi-4.0.1/ub16/include/starpu/1.3/

#starpu mpi
#INCLUDES  =  -I${MKLROOT}/include -I../../build-mpi/install/include   -I../../starpu-1.3-install-mpi/include/starpu/1.3/

#INCLUDES  = -I${MKLROOT}/include -I../../build/install/include   -I../../starpu-1.3-install/include/starpu/1.3/

#parsec mpi
INCLUDES  =  -I${MKLROOT}/include -I../../build-parsec/install/include  -I/opt/intel/compilers_and_libraries_2019.5.281/linux/mpi/intel64/include/ -I/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/include/ -I/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/include/parsec 

#INCLUDES  =  -I${MKLROOT}/include -I../../build-parsec2/installdir/include   -I/home/omairyrm/parsec-last/dplasma/build/installdir/include

#INCLUDES  = -I${MKLROOT}/include -I../../build-quark/install/include   -I/home/omairyrm/quark


LOADLIBES = $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  -c $*.c -o $@
