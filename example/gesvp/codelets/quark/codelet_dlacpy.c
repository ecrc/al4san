/**
 *
 * @file qwrapper_dlacpy.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Julien Langou
 * @author Henricus Bouwmeester
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:02:40 2016
 *
 **/
#include <lapacke.h>
#include <al4san.h>
#include <runtime/al4san_quark.h>
/***************************************************************************//**
 *
 **/
void EIG_CORE_dlacpy_quark(Quark *quark);
void EIG_AL4SAN_CORE_dlacpy(AL4SAN_option_t *options,
                       int uplo, int m, int n, int nb,
                       const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double *B, int ldb)
{
     runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);

    QUARK_Insert_Task(opt->quark, EIG_CORE_dlacpy_quark, (Quark_Task_Flags*)opt,
        sizeof(int),                &uplo,  QUARK_VALUE,
        sizeof(int),                        &m,     QUARK_VALUE,
        sizeof(int),                        &n,     QUARK_VALUE,
        sizeof(double)*nb*nb,    AL4SAN_ADDR(A, double, Am, An),  QUARK_INPUT,
        sizeof(int),                        &lda,   QUARK_VALUE,
        sizeof(double)*nb*nb,    B,                 QUARK_OUTPUT,
        sizeof(int),                        &ldb,   QUARK_VALUE,
        0);

}

/***************************************************************************//**
 *
 **/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak EIG_CORE_dlacpy_quark = PEIG_CORE_dlacpy_quark
#define EIG_CORE_dlacpy_quark PEIG_CORE_dlacpy_quark
#endif
void EIG_CORE_dlacpy_quark(Quark *quark)
{
    int uplo;
    int M;
    int N;
    const double *A;
    int LDA;
    double *B;
    int LDB;
    quark_unpack_args_7(quark, uplo, M, N, A, LDA, B, LDB);

    char u;
    if(uplo==Al4sanUpper)
            u='U';
    else
           u='L';     
    LAPACKE_dlacpy_work(
        LAPACK_COL_MAJOR,
        u,
        M, N, A, LDA, B, LDB);
}

