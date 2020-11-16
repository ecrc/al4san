/**
 *
 * @file al4san/codelet_dplgsy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon dplgsy AL4SAN codelet
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

/*   AL4SAN_TASK_dplgsy - Generate a tile for random symmetric (positive definite if 'bump' is large enough) matrix. */

AL4SAN_TASK_CPU(plgsy, plgsy_cpu_func)

void EIG_AL4SAN_CORE_dplgsy(AL4SAN_option_t *options,
                        double bump, int m, int n, const AL4SAN_desc_t *A, int Am, int An, int lda,
                        int bigM, int m0, int n0, unsigned long long int seed )
{

    AL4SAN_Insert_Task(AL4SAN_TASK(plgsy), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &bump,                                        sizeof(double),
        AL4SAN_VALUE,                       &m,                                           sizeof(int),
        AL4SAN_VALUE,                       &n,                                           sizeof(int),
        AL4SAN_OUTPUT | AL4SAN_AFFINITY,    AL4SAN_ADDR(A, double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                       &lda,                                         sizeof(int),
        AL4SAN_VALUE,                       &bigM,                                        sizeof(int),
        AL4SAN_VALUE,                       &m0,                                          sizeof(int),
        AL4SAN_VALUE,                       &n0,                                          sizeof(int),
        AL4SAN_VALUE,                       &seed,                                        sizeof(unsigned long long int),
        AL4SAN_PRIORITY,                    options->priority,                            sizeof(int),
        AL4SAN_LABEL,                       "plgsy",                                     sizeof(char),
        AL4SAN_COLOR,                       "white",                                      sizeof(char),
        ARG_END);


}

void plgsy_cpu_func(AL4SAN_arg_list *al4san_arg)
{

    double bump;
    int m;
    int n;
    double *A;
    int lda;
    int bigM;
    int m0;
    int n0;
    unsigned long long int seed;


   AL4SAN_Unpack_Arg(al4san_arg, &bump, &m, &n, &A, &lda, &bigM, &m0, &n0, &seed );

    CORE_dplgsy( bump, m, n, A, lda, bigM, m0, n0, seed );

}
