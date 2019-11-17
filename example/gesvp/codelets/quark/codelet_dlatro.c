/**
 *
 * @file qwrapper_dlatro.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:02:43 2016
 *
 **/
#include <al4san.h>
#include <runtime/al4san_quark.h>
/***************************************************************************//**
 *
 **/
void EIG_CORE_dlatro_quark(Quark *quark);
void EIG_AL4SAN_CORE_dlatro(AL4SAN_option_t *options,
                       int uplo, int trans, int m, int n, int nb,
                       const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double *B, int ldb)
{
     runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);

    QUARK_Insert_Task(opt->quark, EIG_CORE_dlatro_quark, (Quark_Task_Flags*)opt,
        sizeof(int),                &uplo,  QUARK_VALUE,
        sizeof(int),                &trans, QUARK_VALUE,
        sizeof(int),                        &m,     QUARK_VALUE,
        sizeof(int),                        &n,     QUARK_VALUE,
        sizeof(double)*nb*nb,    AL4SAN_ADDR(A, double, Am, An), QUARK_INPUT,
        sizeof(int),                        &lda,   QUARK_VALUE,
        sizeof(double)*nb*nb,    B,                 QUARK_OUTPUT,
        sizeof(int),                        &ldb,   QUARK_VALUE,
        0);
}

/***************************************************************************//**
 *
 **/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak EIG_CORE_dlatro_quark = PEIG_CORE_dlatro_quark
#define EIG_CORE_dlatro_quark PEIG_CORE_dlatro_quark
#endif
void EIG_CORE_dlatro_quark(Quark *quark)
{
    int uplo;
    int trans;
    int M;
    int N;
    const double *A;
    int LDA;
    double *B;
    int LDB;

    quark_unpack_args_8(quark, uplo, trans, M, N, A, LDA, B, LDB);
    CORE_dlatro(uplo, trans, M, N, A, LDA, B, LDB);
}
