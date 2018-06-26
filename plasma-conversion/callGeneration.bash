#!/bin/bash
###
#
# @file callGeneration.bash
#
# @copyright 2009-2015 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
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
#  @date 2015-11-30
#
###

# plasma release to download
plasma_relname=2.8.0
plasma_rel=plasma_$plasma_relname.tar.gz
plasma_url=http://icl.cs.utk.edu/projectsfiles/plasma/pubs/$plasma_rel
if [[ ! -e $plasma_rel ]]; then
    echo "$plasma_rel does not exists, y to download it"
    wget $plasma_url
fi
if [[ ! -e plasma_$plasma_relname ]]; then
    tar zxf $plasma_rel
fi

# Define the paths
# ----------------

# where the script is called
plasma_conv_dir="$PWD"
# source files of the conversion (plasma -> altanal)
plasma_dir="$PWD/plasma_$plasma_relname"
# source of the licence
licence_dir="$PWD/insert-licence"
# source of the original altanal
# ex in this plasma_conversion directory: svn checkout https://scm.gforge.inria.fr/anonscm/svn/altanal/trunk/altanal
altanal_dir="$PWD/.."
# source of the converted altanal
altanal_tmp="$PWD/altanal"

if [[ ! -e $plasma_dir ]]; then
    echo "plasma_dir is set to $plasma_dir which does not exist, please set the variable plasma_dir in the script $0"
    exit $?
fi
if [[ ! -e $licence_dir ]]; then
    echo "licence_dir is set to $licence_dir which does not exist, please set the variable licence_dir in the script $0"
    exit $?
fi
if [[ ! -e $altanal_dir ]]; then
    echo "altanal_dir is set to $altanal_dir which does not exist, try to download it"
    svn export svn+ssh://fpruvost@scm.gforge.inria.fr/svnroot/altanal/trunk/altanal
    if [[ ! -e $altanal_dir ]]; then
        echo "altanal_dir is set to $altanal_dir which does not exist, please set the variable altanal_dir in the script $0 and check that this path exists"
        exit $?
    fi
fi
if [[ ! -e $altanal_tmp ]]; then
    echo "altanal_tmp is set to $altanal_tmp which does not exist, lets create it"
    mkdir $altanal_tmp
    if [[ ! -e $altanal_tmp ]]; then
        echo "altanal_tmp is set to $altanal_tmp which does not exist, please set the variable altanal_tmp in the script $0"
        exit $?
    fi
fi

# Definition of files list
# ------------------------
list_file="
include/core_zblas.h
include/core_zcblas.h
include/plasma_z.h
include/plasma_zc.h
control/compute_z.h
control/workspace_z.c
control/plasma_zcf90.F90
control/plasma_zf77.c
control/plasma_zf90.F90
control/plasma_zf90_wrappers.F90

core_blas/core_dzasum.c
core_blas/core_zgemm.c
core_blas/core_zgessq.c
core_blas/core_zhemm.c
core_blas/core_zhessq.c
core_blas/core_zher2k.c
core_blas/core_zherk.c
core_blas/core_zpemv.c
core_blas/core_zsymm.c
core_blas/core_zsyr2k.c
core_blas/core_zsyrk.c
core_blas/core_zsyssq.c
core_blas/core_ztrasm.c
core_blas/core_ztrmm.c
core_blas/core_ztrsm.c
core_blas/core_ztrssq.c
core_blas/core_zgeadd.c
core_blas/core_zgelqt.c
core_blas/core_zgeqrt.c
core_blas/core_zgessm.c
core_blas/core_zgetrf.c
core_blas/core_zgetrf_incpiv.c
core_blas/core_zgetf2_nopiv.c
core_blas/core_zgetrf_nopiv.c
core_blas/core_zlacpy.c
core_blas/core_zlag2c.c
core_blas/core_zlange.c
core_blas/core_zlanhe.c
core_blas/core_zlansy.c
core_blas/core_zlantr.c
core_blas/core_zlaset2.c
core_blas/core_zlaset.c
core_blas/core_zlauum.c
core_blas/core_zpamm.c
core_blas/core_zparfb.c
core_blas/core_zplghe.c
core_blas/core_zplgsy.c
core_blas/core_zplrnt.c
core_blas/core_zpltmg.c
core_blas/core_zpotrf.c
core_blas/core_zssssm.c
core_blas/core_ztrtri.c
core_blas/core_ztslqt.c
core_blas/core_ztsmlq.c
core_blas/core_ztsmqr.c
core_blas/core_ztsqrt.c
core_blas/core_ztstrf.c
core_blas/core_zttlqt.c
core_blas/core_zttmlq.c
core_blas/core_zttmqr.c
core_blas/core_zttqrt.c
core_blas/core_zunmlq.c
core_blas/core_zunmqr.c

core_blas-qwrapper/qwrapper_dzasum.c
core_blas-qwrapper/qwrapper_zgemm.c
core_blas-qwrapper/qwrapper_zgessq.c
core_blas-qwrapper/qwrapper_zhemm.c
core_blas-qwrapper/qwrapper_zher2k.c
core_blas-qwrapper/qwrapper_zherk.c
core_blas-qwrapper/qwrapper_zpemv.c
core_blas-qwrapper/qwrapper_zplssq.c
core_blas-qwrapper/qwrapper_zhessq.c
core_blas-qwrapper/qwrapper_zsymm.c
core_blas-qwrapper/qwrapper_zsyr2k.c
core_blas-qwrapper/qwrapper_zsyrk.c
core_blas-qwrapper/qwrapper_zsyssq.c
core_blas-qwrapper/qwrapper_ztrasm.c
core_blas-qwrapper/qwrapper_ztrmm.c
core_blas-qwrapper/qwrapper_ztrsm.c
core_blas-qwrapper/qwrapper_ztrssq.c
core_blas-qwrapper/qwrapper_zgeadd.c
core_blas-qwrapper/qwrapper_zgelqt.c
core_blas-qwrapper/qwrapper_zgeqrt.c
core_blas-qwrapper/qwrapper_zgessm.c
core_blas-qwrapper/qwrapper_zgetrf.c
core_blas-qwrapper/qwrapper_zgetrf_incpiv.c
core_blas-qwrapper/qwrapper_zgetf2_nopiv.c
core_blas-qwrapper/qwrapper_zgetrf_nopiv.c
core_blas-qwrapper/qwrapper_zlacpy.c
core_blas-qwrapper/qwrapper_zlag2c.c
core_blas-qwrapper/qwrapper_zlange.c
core_blas-qwrapper/qwrapper_zlanhe.c
core_blas-qwrapper/qwrapper_zlansy.c
core_blas-qwrapper/qwrapper_zlantr.c
core_blas-qwrapper/qwrapper_zlaset2.c
core_blas-qwrapper/qwrapper_zlaset.c
core_blas-qwrapper/qwrapper_zlauum.c
core_blas-qwrapper/qwrapper_zpamm.c
core_blas-qwrapper/qwrapper_zparfb.c
core_blas-qwrapper/qwrapper_zplghe.c
core_blas-qwrapper/qwrapper_zplgsy.c
core_blas-qwrapper/qwrapper_zplrnt.c
core_blas-qwrapper/qwrapper_zpltmg.c
core_blas-qwrapper/qwrapper_zpotrf.c
core_blas-qwrapper/qwrapper_zssssm.c
core_blas-qwrapper/qwrapper_ztrtri.c
core_blas-qwrapper/qwrapper_ztslqt.c
core_blas-qwrapper/qwrapper_ztsmlq.c
core_blas-qwrapper/qwrapper_ztsmqr.c
core_blas-qwrapper/qwrapper_ztsqrt.c
core_blas-qwrapper/qwrapper_ztstrf.c
core_blas-qwrapper/qwrapper_zttlqt.c
core_blas-qwrapper/qwrapper_zttmlq.c
core_blas-qwrapper/qwrapper_zttmqr.c
core_blas-qwrapper/qwrapper_zttqrt.c
core_blas-qwrapper/qwrapper_zunmlq.c

compute/pzgemm.c
compute/pzhemm.c
compute/pzher2k.c
compute/pzherk.c
compute/pzsymm.c
compute/pzsyr2k.c
compute/pzsyrk.c
compute/pztrmm.c
compute/pztrsm.c
compute/pztrsmpl.c
compute/pzgeadd.c
compute/pzgelqf.c
compute/pzgelqfrh.c
compute/pzgeqrf.c
compute/pzgeqrfrh.c
compute/pzgetrf_incpiv.c
compute/pzgetrf_nopiv.c
compute/pzlacpy.c
compute/pzlag2c.c
compute/pzlange.c
compute/pzlanhe.c
compute/pzlansy.c
compute/pzlantr.c
compute/pzlaset2.c
compute/pzlaset.c
compute/pzlauum.c
compute/pzplghe.c
compute/pzplgsy.c
compute/pzpltmg.c
compute/pzpotrf.c
compute/pztrtri.c
compute/pzunglq.c
compute/pzunglqrh.c
compute/pzungqr.c
compute/pzungqrrh.c
compute/pzunmlq.c
compute/pzunmlqrh.c
compute/pzunmqr.c
compute/pzunmqrrh.c

compute/zgels.c
compute/zgelqs.c
compute/zgelqf.c
compute/zgemm.c
compute/zgeqrf.c
compute/zgeqrs.c
compute/zgetrf_incpiv.c
compute/zgetrf_nopiv.c
compute/zgetrs_incpiv.c
compute/zgesv_incpiv.c
compute/zhemm.c
compute/zher2k.c
compute/zherk.c
compute/zsymm.c
compute/zsyr2k.c
compute/zsyrk.c
compute/ztrmm.c
compute/ztrsm.c
compute/ztrsmpl.c
compute/zlacpy.c
compute/zlange.c
compute/zlanhe.c
compute/zlansy.c
compute/zlantr.c
compute/zlaset.c
compute/zlauum.c
compute/zplghe.c
compute/zplgsy.c
compute/zplrnt.c
compute/zpltmg.c
compute/zposv.c
compute/zpotrf.c
compute/zpotri.c
compute/zpotrs.c
compute/ztrtri.c
compute/zunglq.c
compute/zungqr.c
compute/zunmlq.c
compute/zunmqr.c

testing/testing_zgemm.c
testing/testing_zhemm.c
testing/testing_zher2k.c
testing/testing_zherk.c
testing/testing_zlange.c
testing/testing_zsymm.c
testing/testing_zsyr2k.c
testing/testing_zsyrk.c
testing/testing_ztrmm.c
testing/testing_ztrsm.c
testing/testing_zpemv.c
testing/testing_zposv.c
testing/testing_zpotri.c
testing/testing_zgels.c
testing/testing_zgesv_incpiv.c

timing/time_zgemm.c
timing/time_zgemm_tile.c
timing/time_ztrsm.c
timing/time_zgels.c
timing/time_zgels_tile.c
timing/time_zgeqrf.c
timing/time_zgeqrf_tile.c
timing/time_zgetrf_incpiv.c
timing/time_zgetrf_incpiv_tile.c
timing/time_zgetrf_nopiv.c
timing/time_zgetri_tile.c
timing/time_zposv.c
timing/time_zposv_tile.c
timing/time_zpotrf.c
timing/time_zpotrf_tile.c
timing/time_zpotri_tile.c
timing/time_zgesv_incpiv.c
timing/time_zgesv_incpiv_tile.c
"
# COREBLAS not used
# -----------------
#core_blas/core_zbrdalg.c
#core_blas/core_zgbelr.c
#core_blas/core_zgblrx.c
#core_blas/core_zgbrce.c
#core_blas/core_zgessq.c
#core_blas/core_zgetrip.c
#core_blas/core_zgetrf_reclap.c
#core_blas/core_zgetrf_rectil.c
#core_blas/core_zhbelr.c
#core_blas/core_zhblrx.c
#core_blas/core_zhbrce.c
#core_blas/core_zhbtype1cb.c
#core_blas/core_zhbtype2cb.c
#core_blas/core_zhbtype3cb.c
#core_blas/core_zhegst.c
#core_blas/core_zherfb.c
#core_blas/core_zlarfb_gemm.c
#core_blas/core_zlarfx_tbrd.c
#core_blas/core_zlarfy.c
#core_blas/core_zlaswp.c
#core_blas/core_zlatro.c
#core_blas/core_zshift.c
#core_blas/core_zswpab.c
#core_blas/core_ztrdalg.c
#core_blas/core_ztsmlq_corner.c
#core_blas/core_ztsmlq_hetra1.c
#core_blas/core_ztsmqr_corner.c
#core_blas/core_ztsmqr_hetra1.c
#core_blas/core_ztsrfb.c

# PCOMPUTE not used
# -----------------
#compute/pzgebrd_ge2tb.c
#compute/pzgebrd_tb2bd.c
#compute/pzgetmi2.c
#compute/pzgetrf_reclap.c
#compute/pzgetrf_rectil.c
#compute/pzhbcpy_t2bl.c
#compute/pzhegst.c
#compute/pzherbt.c
#compute/pzhetrd_hb2st.c
#compute/pzhetrd_he2hb.c
#compute/pzlarft_blgtrd.c
#compute/pzlaswp.c
#compute/pzlaswpc.c
#compute/pztrsmrv.c
#compute/pzunmqr_blgtrd.c
#
#compute/pzbarrier.c
#compute/pzpack.c
#compute/pzshift.c

# COMPUTE not used
# ----------------
#compute/zgetrf.c
#compute/zgetri.c
#compute/zcgels.c
#compute/zcgesv.c
#compute/zcposv.c
#compute/zcungesv.c
#compute/zgebrd.c
#compute/zgecfi2.c
#compute/zgecfi2.h
#compute/zgecfi.c
#compute/zgesv.c
#compute/zgesvd.c
#compute/zgetmi.c
#compute/zgetrs.c
#compute/zheev.c
#compute/zheevd.c
#compute/zhegst.c
#compute/zhegv.c
#compute/zhegvd.c
#compute/zhetrd.c
#compute/zlaswp.c
#compute/zlaswpc.c
#compute/ztrsmrv.c

# TIMING not used
# ---------------
#timing/time_zgetrf.c
#timing/time_zgetrf_tile.c
#timing/time_zcgesv.c
#timing/time_zcgesv_tile.c
#timing/time_zcposv.c
#timing/time_zcposv_tile.c
#timing/time_zgebrd_tile.c
#timing/time_zgetrf_reclap.c
#timing/time_zgetrf_rectil.c
#timing/time_zgecfi.c
#timing/time_zgesvd_tile.c
#timing/time_zheevd_tile.c
#timing/time_zheev_tile.c
#timing/time_zhegv_tile.c
#timing/time_zlapack2tile.c
#timing/time_zgesv.c
#timing/time_zgesv_tile.c

# TESTING not used
# ----------------
#testing/testing_zcgels.c
#testing/testing_zcgesv.c
#testing/testing_zcposv.c
#testing/testing_zcungesv.c
#testing/testing_zgecfi.c
#testing/testing_zgesv.c
#testing/testing_zgesvd.c
#testing/testing_zgetmi.c
#testing/testing_zgetri.c
#testing/testing_zheev.c
#testing/testing_zheevd.c
#testing/testing_zhegst.c
#testing/testing_zhegv.c
#testing/testing_zhegvd.c
#testing/testing_zlange.c

# TIMING not used
# ----------------
#timing/zauxiliary.h
#timing/zauxiliary.c

# Génération des sources
# ----------------------
python ${plasma_conv_dir}/generate_altanalSrc.py -o ${altanal_tmp} -p ${plasma_dir} -f "${list_file}" --force

# Copie de la licence en entête des fichiers générés
current_dir=$PWD
cd ${altanal_tmp}
${licence_dir}/insert-licence-altanal.sh
cd ${current_dir}

# Recopie de magma_altanal
# ----------------------
python ${plasma_conv_dir}/generate_altanalSrc.py -o ${altanal_dir} -m ${altanal_tmp} --force
