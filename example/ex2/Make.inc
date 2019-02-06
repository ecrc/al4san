CC =gcc
LINK = $(CC) 

#LDFLAGS = -O3 
CFLAGS = -g



#F77BLASL= -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -fopenmp -lpthread -lm -lgfortran

#QUARK
#LIBS  =  -L/home/omairyrm/al4san-dev/build-quark/installdir/lib -lal4san -lal4san_quark  -L/home/omairyrm/chameleon/build-quark/installdir/lib -lchameleon -lchameleon_quark -lcoreblas -L/home/omairyrm/quark -lquark -L/usr/lib -lm -llapacke -lblas -lhwloc -lpthread #`llvm-config --libs`

#STARPU
#LIBS  =  -L/home/omairyrm/al4san-dev/build-starpu/installdir/lib -lal4san -lal4san_starpu  -L/home/omairyrm/chameleon/build-starpu/installdir/lib -lchameleon -lchameleon_starpu -lcoreblas -L/home/omairyrm/starpu-1.2.6/install/lib -lstarpu-1.2 -L/usr/lib -lm -llapacke -lblas -lrt -lhwloc -lpthread

LIBS  =  -L/home/omairyrm/al4san-dev/build-parsec/installdir/lib -lal4san -lal4san_parsec  -L/home/omairyrm/chameleon/build-parsec/installdir/lib -lchameleon -lchameleon_parsec -lcoreblas -L/home/omairyrm/parsec-mat/parsec/build/installdir/lib/ -lparsec -L/opt/ecrc/openmpi/3.0.0-gcc-5.5.0/ub16/lib/ -lmpi -L/usr/lib -lm -llapacke -lblas -lrt -lhwloc -lpthread

#QUARK
#INCLUDES  =  -I/home/omairyrm/al4san-dev/build-quark/installdir/include -I/home/omairyrm/chameleon/build-quark/installdir/include -I/home/omairyrm/quark/

#STARPU
#INCLUDES  =  -I/home/omairyrm/al4san-dev/build-starpu/installdir/include -I/home/omairyrm/chameleon/build-starpu/installdir/include -I/home/omairyrm/starpu-1.2.6/install/include/starpu/1.2/

INCLUDES  =  -I/home/omairyrm/al4san-dev/build-parsec/installdir/include -I/home/omairyrm/chameleon/build-parsec/installdir/include -I/home/omairyrm/parsec-mat/parsec/build/installdir/include/parsec -I/home/omairyrm/parsec-mat/parsec/build/installdir/include

LOADLIBES = $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  -c $*.c -o $@
