module unload cray-libsci1
module switch PrgEnv-cray/6.0.5 PrgEnv-intel
module load cmake/3.13.4
export CRAYPE_LINK_TYPE=dynamic
export PKG_CONFIG_PATH=/lustre/project/k1205/akbudak/codes/plasma-installer_2.8.0/install-intel/lib/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/lib64/:$LD_LIBRARY_PATH
