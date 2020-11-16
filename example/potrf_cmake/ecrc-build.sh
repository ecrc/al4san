#parsec mpi
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="-L/opt/ecrc/openmpi/3.0.0-gcc-7.2.0/ub16/lib;-lmpi" -DPARSEC_LIBRARIES="-L/home/omairyrm/parsec-collective/parsec/build/installdir/lib;-lparsec" -DAL4SAN_LIBS=" -L/home/omairyrm/al4san-dev/build/installdir/lib;-lal4san;-lal4san_parsec"

#starpu shared
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DSTARPU_INCDIR="/home/omairyrm/al4san-dev/starpu-1.2-install/include/starpu/1.2/" -DAL4SAN_INCDIR="/home/omairyrm/al4san-dev/build/install/include" -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="" -DSTARPU_LIBRARIES="-L/home/omairyrm/al4san-dev/starpu-1.2-install/lib;-lstarpu-1.2" -DAL4SAN_LIBS="-L/home/omairyrm/al4san-dev/build/install/lib;-lal4san;-lal4san_starpu"

#starpu mpi
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DSTARPU_INCDIR="/home/omairyrm/al4san-dev/starpu-1.2-install-mpi/include/starpu/1.2/" -DAL4SAN_INCDIR="/home/omairyrm/al4san-dev/build-mpi/install/include" -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="-L/opt/ecrc/openmpi/3.0.0-gcc-7.2.0/ub16/lib;-lmpi" -DSTARPU_LIBRARIES="-L/home/omairyrm/al4san-dev/starpu-1.2-install-mpi/lib/;-lstarpu-1.2;-lstarpumpi-1.2" -DAL4SAN_LIBS="-L/home/omairyrm/al4san-dev/build-mpi/install/lib;-lal4san;-lal4san_starpu"

#quark 
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DQUARK_INCDIR="/home/omairyrm/al4san-dev/quark" -DAL4SAN_INCDIR="/home/omairyrm/al4san-dev/build/install/include" -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="-L/opt/ecrc/openmpi/3.0.0-gcc-7.2.0/ub16/lib;-lmpi" -DQUARK_LIBRARIES="-L/home/omairyrm/al4san-dev/quark;-lquark" -DAL4SAN_LIBS="-L/home/omairyrm/al4san-dev/build/install/lib;-lal4san;-lal4san_quark;-lquark"


#openmp
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DOPENMP_INCDIR="" -DAL4SAN_INCDIR="/home/omairyrm/al4san-dev/build/installdir/include" -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="" -DOPENMP_LIBRARIES="-fopenmp" -DAL4SAN_LIBS="-L/home/omairyrm/al4san-dev/build/installdir/lib;-lal4san;-lal4san_openmp;-fopenmp"



cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DOPENMP_INCDIR="" -DAL4SAN_INCDIR="/home/omairyrm/al4san-dev/build/installdir/include" -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="" -DOPENMP_LIBRARIES="-fopenmp" -DAL4SAN_LIBS="-L/home/omairyrm/al4san-dev/build/installdir/lib;-lal4san;-lal4san_openmp;-fopenmp"

