CC = gcc
LINK = $(CC) 

#LDFLAGS = -O3 
CFLAGS =  -O3 -DADD_ -DPLASMA_WITH_MKL  -fopenmp  -DPLASMA_HWLOC -DQUARK_HWLOC -DEIG_USE_CUDA -DPLASMA_ENABLE_TIMER -DAL4SAN_COPY_DIAG -DEIG_TIMER -DAL4SAN_USE_CUDA -DAL4SAN_CUDA_ASYNC1
#CFLAGS =  -O2  -DADD_ -DPLASMA_WITH_MKL  -fopenmp  -DPLASMA_HWLOC -DQUARK_HWLOC -DEIG_USE_CUDA -DCHAMELEON_COPY_DIAG

#starpu gpu gcc
INCLUDES  =  -I/home/omairyrm/switch-sygv-mix/switch-sygv/include  -I/home/omairyrm/al4san-switch/al4san-dev/build/installdir/include -I/home/omairyrm/quark -I/home/omairyrm/cham-release/chameleon/build/installdir/include/chameleon -I/home/omairyrm/cham-release/chameleon/build/installdir/include  -I/home/omairyrm/starpu-1.3.3/install/include/starpu/1.3/

#starpu gpu gcc
LIBS  =    -L/home/omairyrm/al4san-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark -lal4san_starpu  -L/opt/ecrc/cuda/9.0/lib64/ -lcuda -lcudart -lcublas -L/home/omairyrm/cham-release/chameleon/build/installdir/lib   -lcudablas -lchameleon -lchameleon_starpu  /home/omairyrm/cham-release/chameleon/build/installdir/lib/libcoreblas.a  -lcudablas  -L/home/omairyrm/starpu-1.3.3/install/lib -lstarpu-1.3 -L/home/omairyrm/quark -lquark -fopenmp -L/opt/intel/mkl/lib -lmkl_rt   -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_gf_lp64 -lmkl_gnu_thread -lmkl_core -lpthread -lm -ldl  -lhwloc -lcoreblas

LOADLIBES = $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  -c $*.c -o $@
