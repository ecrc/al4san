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
 * @version 1.1.0
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


AL4SAN_TASK_CPU(lacpy1, lacpy_cpu_func1)

void EIG_AL4SAN_CORE_dlacpy1(AL4SAN_option_t *options,
                         int uplo, int m, int n, int nb,
                         const AL4SAN_desc_t *A, int Am, int An, int lda,
                         const AL4SAN_desc_t *B, int Bm, int Bn, int ldb )
{
    (void)nb;


    AL4SAN_Insert_Task(AL4SAN_TASK(lacpy1), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &uplo,                                        sizeof(int),
        AL4SAN_VALUE,                       &m,                                           sizeof(int),
        AL4SAN_VALUE,                       &n,                                           sizeof(int),
        AL4SAN_INPUT,                       AL4SAN_ADDR(A, double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                       &lda,                                         sizeof(int),
        AL4SAN_OUTPUT,                      AL4SAN_ADDR(B, double, Bm, Bn),  AL4SAN_DEP,
        AL4SAN_VALUE,                       &ldb,                                         sizeof(int),
        AL4SAN_PRIORITY,                    options->priority,                     sizeof(int),
        AL4SAN_LABEL,                       "dlacpy",                                     sizeof(char),
        AL4SAN_COLOR,                       "white",                                      sizeof(char),
        ARG_END);
}


#if !defined(CHAMELEON_SIMULATION)
 void lacpy_cpu_func1(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int M;
    int N;
    int displA;
    int displB;
    const double *A;
    int LDA;
    double *B;
    int LDB;

    AL4SAN_Unpack_Arg(al4san_arg, &uplo, &M, &N, &A, &LDA, &B, &LDB);

    CORE_dlacpy(uplo, M, N, A, LDA, B , LDB);
}
#endif /* !defined(CHAMELEON_SIMULATION) */
