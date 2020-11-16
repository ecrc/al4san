/**
 *
 * @file al4san/codelet_dlatro.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon dlatro StarPU codelet
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-09-19
 * @generated d Wed Jan 16 11:13:57 2019
 *
 */
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>

AL4SAN_TASK_CPU(latro, latro_cpu_func)

void EIG_AL4SAN_CORE_dlatro(AL4SAN_option_t *options,
                       int uplo, int trans, int m, int n, int nb,
                       const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double *B, int ldb)
{

    AL4SAN_Insert_Task(AL4SAN_TASK(latro), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &uplo,                                        sizeof(int),
        AL4SAN_VALUE,                       &trans,                                       sizeof(int),
        AL4SAN_VALUE,                       &m,                                           sizeof(int),
        AL4SAN_VALUE,                       &n,                                           sizeof(int),
        AL4SAN_INPUT,                       AL4SAN_ADDR(A, double, Am, An),              AL4SAN_DEP,
        AL4SAN_VALUE,                       &lda,                                         sizeof(int),
        AL4SAN_OUTPUT | AL4SAN_AFFINITY,    B,                                             AL4SAN_DEP,
        AL4SAN_VALUE,                       &ldb,                                         sizeof(int),
        AL4SAN_PRIORITY,                    options->priority,                            sizeof(int),
        AL4SAN_LABEL,                       "dlatro",                                     sizeof(char),
        ARG_END);
}

void latro_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int trans;
    int M;
    int N;
    const double *A;
    int LDA;
    double *B;
    int LDB;
    AL4SAN_Unpack_Arg(al4san_arg, &uplo, &trans, &M, &N, &A, &LDA, &B, &LDB);
    CORE_dlatro(uplo, trans, M, N, A, LDA, B, LDB);
}
