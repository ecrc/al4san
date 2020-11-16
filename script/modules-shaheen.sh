env=intel
#env=gnu
module swap PrgEnv-cray PrgEnv-$env
module unload cray-libsci
#module load intel
export CRAYPE_LINK_TYPE=dynamic
export C_INCLUDE_PATH=/opt/cray/pe/papi/5.7.0.2/include:$C_INCLUDE_PATH
export PATH=/project/k1205/omairyrm/sourcefiles/cmake-3.17.3/install/bin:$PATH
export PKG_CONFIG_PATH=/project/k1205/omairyrm/parsec_nov2020/dplasma/build/install_dir/lib64/pkgconfig:$PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/project/k1205/omairyrm/parsec_nov2020/al4san-dev/build/install/lib/pkgconfig/:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/project/k1205/omairyrm/parsec_nov2020/dplasma/build/install_dir/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/project/k1205/omairyrm/parsec_nov2020/al4san-dev/build/install/lib/:$LD_LIBRARY_PATH
