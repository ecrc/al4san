/**
 *
 * @file al4san/codelet_dlacpy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 * @brief Chameleon dlacpy AL4SAN codelet
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-09-19
 * @generated d Wed Jan 16 11:13:56 2019
 *
 */
/**
 *
 * @ingroup CORE_CHAMELEON_Complex64_t
 *
 */
#include <lapacke.h>
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>

AL4SAN_TASK_CPU(lacpy, lacpy_cpu_func)

void EIG_AL4SAN_CORE_dlacpy(AL4SAN_option_t *options,
                       int uplo, int m, int n, int nb,
                       const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double *B, int ldb)
{
    (void)nb;


    AL4SAN_Insert_Task(AL4SAN_TASK(lacpy), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &uplo,                                        sizeof(int),
        AL4SAN_VALUE,                       &m,                                           sizeof(int),
        AL4SAN_VALUE,                       &n,                                           sizeof(int),
        AL4SAN_INPUT,                       AL4SAN_ADDR(A, double, Am, An),               AL4SAN_DEP,
        AL4SAN_VALUE,                       &lda,                                         sizeof(int),
        AL4SAN_OUTPUT | AL4SAN_AFFINITY,    B,                                            AL4SAN_DEP,
        AL4SAN_VALUE,                       &ldb,                                         sizeof(int),
        AL4SAN_PRIORITY,                    options->priority,                            sizeof(int),
        AL4SAN_LABEL,                       "lacpy",                                     sizeof(char),
        AL4SAN_COLOR,                       "white",                                      sizeof(char),
        ARG_END);
}



#if !defined(CHAMELEON_SIMULATION)
 void lacpy_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int M;
    int N;
    const double *A;
    int LDA;
    double *B;
    int LDB;

    AL4SAN_Unpack_Arg(al4san_arg, &uplo, &M, &N, &A, &LDA,  &B, &LDB);

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
#endif /* !defined(CHAMELEON_SIMULATION) */
