cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/installdir -DBLAS_LIBRARIES="-Wl,--no-as-needed;-L${MKLROOT}/lib;-lmkl_core;-lmkl_sequential;-lmkl_intel_lp64;-lpthread;-lm;-ldl"  -DMPI_C_LIBRARIES="-L/opt/cray/pe/mpt/7.7.11/gni/mpich-cray/90/;-lmpich" -DPARSEC_LIBRARIES="-L/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/lib64;-lparsec" -DAL4SAN_LIBS=" -L/project/k1205/omairyrm/al4san-parsec/al4san-dev/build-parsec/install/lib;-lal4san;-lal4san_parsec"