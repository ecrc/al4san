#!/bin/sh

# Please follow http://altanal.gforge.inria.fr/spack/spack.html for further instructions
git clone https://github.com/solverstack/spack.git
. spack/share/spack/setup-env.sh
spack install -v openblas
spack install -v starpu
spack load openblas
spack load starpu
