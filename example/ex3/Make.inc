CC =gcc
LINK = $(CC) 

## VARIABLES 
## It is recommended to have AL4SAN_INSTALLDIR/lib/pkgconfig in PKG_CONFIG_PATH env variable

AL4SAN_INSTALLDIR = /mnt/data/gonzalea/devel/ecrc/al4san-dev/build-quark-starpu/installdir
QUARK_INSTALLDIR = /home/gonzalea/devel/ecrc/al4san-dev/build-quark-starpu/quark
CUDA_DIR ?= /usr/local/cuda
BLASLIBS = -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_gf_lp64 -lmkl_gnu_thread -lmkl_core -lpthread -lm -ldl




AL4SAN_LIBS = $(shell pkg-config --libs al4san)
AL4SAN_LIBS += $(shell pkg-config --libs chameleon libstarpu)
AL4SAN_LIBS += -L$(CUDA_DIR/lib) -lcuda -lcudart -lcublas
AL4SAN_LIBS += $(BLASLIBS)

LDFLAGS = -O3 -fopenmp
CFLAGS = -g -ldl -Wall -DAL4SAN_USE_CUDA -DAL4SAN_CUDA_ASYNC1 -fopenmp


#F77BLASL= -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -fopenmp -lpthread -lm -lgfortran

#QUARK
#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib/ -lmpi  -L/home/omairyrm/runtime-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-quark/installdir/lib -lchameleon -lchameleon_quark -lcoreblas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.2.6/install/lib -lstarpu-1.2 -lstarpumpi-1.2

#STARPU
#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread  -L/home/omairyrm/runtime-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-starpu/installdir/lib -lchameleon -lchameleon_starpu -lcoreblas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.2.6/install/lib -lstarpu-1.2 

#STARPU mpi
#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib/ -lmpi  -L/home/omairyrm/runtime-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-starpu/installdir/lib -lchameleon -lchameleon_starpu -lcoreblas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.2.6/install/lib -lstarpu-1.2 -lstarpumpi-1.2

#STARPU-gpu
#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread  -L/opt/ecrc/cuda/9.0/lib64/ -lcuda -lcudart -lcublas -L/home/omairyrm/runtime-switch/al4san-dev/build-1.3/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/modified-chameleon/chameleon/build-starpu-1.3-GPU/installdir/lib -lchameleon -lchameleon_starpu -lcoreblas -lcudablas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.3.3.28Oct.2019/install/lib -lstarpu-1.3

#parsec
#LIBS  =  -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib/ -lmpi  -L/home/omairyrm/runtime-switch/al4san-dev/build/installdir/lib -lal4san -lal4san_quark  -lal4san_starpu -lal4san_parsec  -L/home/omairyrm/chameleon/build-parsec/installdir/lib -lchameleon -lchameleon_parsec -lcoreblas -L/home/omairyrm/quark -lquark -L/home/omairyrm/parsec/build/installdir/lib -lparsec -L/home/omairyrm/starpu-1.2.6/install/lib -lstarpu-1.2 -lstarpumpi-1.2

#QUARK
#INCLUDES  =  -I/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/include -I/home/omairyrm/runtime-switch/al4san-dev/build/installdir/include -I/home/omairyrm/chameleon/build-quark/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.2.6/install/include/starpu/1.2/ -I/home/omairyrm/parsec/build/installdir/include

#STARPU
#INCLUDES  =  -I/home/omairyrm/runtime-switch/al4san-dev/build/installdir/include -I/home/omairyrm/chameleon/build-starpu/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.2.6/install/include/starpu/1.2/ -I/home/omairyrm/parsec/build/installdir/include

#STARPU-mpi
#INCLUDES  =  -I/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/include -I/home/omairyrm/runtime-switch/al4san-dev/build/installdir/include -I/home/omairyrm/chameleon/build-starpu/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.2.6/install/include/starpu/1.2/ -I/home/omairyrm/parsec/build/installdir/include

#STARPU-gpu
#INCLUDES  =  -I/home/omairyrm/runtime-switch/al4san-dev/build-1.3/installdir/include -I/home/omairyrm/modified-chameleon/chameleon/build-starpu-1.3-GPU/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.3.3.28Oct.2019/install/include/starpu/1.3/ -I/home/omairyrm/parsec/build/installdir/include

#parsec
#INCLUDES  =  -I/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/include -I/home/omairyrm/runtime-switch/al4san-dev/build/installdir/include -I/home/omairyrm/chameleon/build-parsec/installdir/include -I/home/omairyrm/quark/ -I/home/omairyrm/parsec/build/installdir/include -I/home/omairyrm/starpu-1.2.6/install/include/starpu/1.2/ -I/home/omairyrm/parsec/build/installdir/include

INCLUDES = $(shell pkg-config --cflags al4san)
INCLUDES += $(shell pkg-config --cflags chameleon libstarpu)
LIBS = $(LDFLAGS)
LIBS += $(AL4SAN_LIBS)

INCLUDES += -I$(AL4SAN_INSTALLDIR)/include
INCLUDES += -I$(QUARK_INSTALLDIR)/include
INCLUDES += -I../../runtime/parsec/include/
INCLUDES += -I../../runtime/quark/include/
INCLUDES += -I$(CUDA_DIR)/include
###LOADLIBES = $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  -c $*.c -o $@
