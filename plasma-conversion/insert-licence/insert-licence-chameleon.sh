#!/bin/bash
###
#
# -- Inria
# -- (C) Copyright 2016
#
# This software is a computer program whose purpose is to process
# Matrices Over Runtime Systems @ Exascale (ALTANAL). More information
# can be found on the following website: http://www.inria.fr/en/teams/altanal.
#
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-C license and that you accept its terms.
#
###
#
#  @project MORSE
#  MORSE is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
# @version 1.0.0
#  @author Cedric Castagnede
#  @author Emmanuel Agullo
#  @author Mathieu Faverge
#  @author Florent Pruvost
#  @date 2012-07-13
#
###
for i in $(find . -name "*.c" -or -name "*.h" -not -path "*svn*" -not -name "altanal_fortran.h")
do
  cat $(dirname $0)/altanal_c.licence > $(dirname $0)/buffer.tmp
  cat $i >> $(dirname $0)/buffer.tmp
  cat $(dirname $0)/buffer.tmp > $i
  rm $(dirname $0)/buffer.tmp
done

for i in $(find . -name "*.f" -or -name "*.F" -or -name "*.f77" -or -name "*.F77" -or -name "*.f90" -or -name "*.F90" -or -name "altanal_fortran.h")
do
  cat $(dirname $0)/altanal_f.licence > $(dirname $0)/buffer.tmp
  cat $i >> $(dirname $0)/buffer.tmp
  cat $(dirname $0)/buffer.tmp > $i
  rm $(dirname $0)/buffer.tmp
done
