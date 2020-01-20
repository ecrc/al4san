CC =gcc
LINK = $(CC) 

#LDFLAGS = -O3 
CFLAGS = -g -ldl -Wall -Wno-unused-function -DAL4SAN_USE_CUDA -DAL4SAN_CUDA_ASYNC1



#F77BLASL= -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -fopenmp -lpthread -lm -lgfortran




LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread  -L/opt/ecrc/cuda/9.0/lib64/ -lcuda -lcudart -lcublas -L/home/omairyrm/al4san-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-starpu-gpu-gcc/installdir/lib -lchameleon -lchameleon_starpu -lcoreblas -lcudablas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.3.3.28Oct.2019/install/lib -lstarpu-1.3




INCLUDES  =  -I/home/omairyrm/al4san-switch/al4san-dev/build/installdir/include -I/home/omairyrm/chameleon/build-starpu-gpu-gcc/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.3.3.28Oct.2019/install/include/starpu/1.3/ -I/home/omairyrm/parsec/build/installdir/include


LOADLIBES = $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  -c $*.c -o $@
