/**
 *
 * @file qwrapper_dtrdalg1.c
 *
 *  PLASMA core_blas quark wrapper
    QUARK_Task_Flag_Set(&task_flags, TASK_SEQUENCE, (intptr_t)sequence->quark_sequence);
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2013-07-04
 * @generated d Fri Apr  1 11:02:44 2016
 *
 **/
#include <lapacke.h>
//#include "common.h"
#include <al4san.h>
#include <runtime/al4san_quark.h>
/***************************************************************************//**
 *
 **/
void EIG_CORE_dtrdalg1_quark(Quark *quark);
void EIG_AL4SAN_CORE_dtrdalg1(AL4SAN_option_t *options,
                         int n,
                         int nb,
                         double *A,
                         int lda,
                         double *V,
                         double *TAU,
                         int Vblksiz, int wantz,
                         int i, int sweepid, int m, int grsiz,
                         int *PCOL, int *ACOL, int *MCOL)
{
     runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);
    QUARK_Insert_Task(opt->quark, EIG_CORE_dtrdalg1_quark,   (Quark_Task_Flags*)opt,
        sizeof(int),                      &n, QUARK_VALUE,
        sizeof(int),                     &nb, QUARK_VALUE,
        sizeof(double),        A,   QUARK_INPUT,
        sizeof(int),                    &lda, QUARK_VALUE,
        sizeof(double),        V,   QUARK_INPUT,
        sizeof(double),      TAU,   QUARK_INPUT,
        sizeof(int),                &Vblksiz, QUARK_VALUE,
        sizeof(int),                  &wantz, QUARK_VALUE,
        sizeof(int),                      &i, QUARK_VALUE,
        sizeof(int),                &sweepid, QUARK_VALUE,
        sizeof(int),                      &m, QUARK_VALUE,
        sizeof(int),                  &grsiz, QUARK_VALUE,
        sizeof(double)*nb,  NULL,             QUARK_SCRATCH,
        sizeof(int),                    PCOL, QUARK_INPUT,
        sizeof(int),                    ACOL, QUARK_INPUT,
        sizeof(int),                    MCOL, QUARK_OUTPUT | LOCALITY,
        0);

}

/***************************************************************************//**
 *
 **/
void EIG_CORE_dtrdalg1_quark(Quark *quark)
{
    int n, nb, lda, Vblksiz, wantz, i, sweepid, m, grsiz;
    double *A, *V, *TAU, *work;

    quark_unpack_args_13(quark, n, nb, A, lda, V, TAU, Vblksiz, wantz, i, sweepid, m, grsiz, work);
     CORE_dtrdalg1(n, nb, A, lda, V, TAU, Vblksiz, wantz, i, sweepid, m, grsiz, work);
     //CORE_dlatro(PlasmaUpper, PlasmaNoTrans, n, m, A, lda, V, m);
}


