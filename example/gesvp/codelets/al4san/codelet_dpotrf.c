/**
 *
 * @file al4san/codelet_dpotrf.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon dpotrf AL4SAN codelet
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
//#include <runtime/al4san_parsec.h>
/**
 *
 * @ingroup CORE_ double
 *
 */
AL4SAN_TASK_CPU(potrf, potrf_cpu_fun)

void EIG_AL4SAN_CORE_dpotrf(AL4SAN_option_t *options,
                        int uplo, int n, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        int iinfo )
{

/*    AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_RW(A, Am, An);
    AL4SAN_END_ACCESS_DECLARATION;
*/

   AL4SAN_Insert_Task(AL4SAN_TASK(potrf), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                      &uplo,                                         sizeof(int),
        AL4SAN_VALUE,                      &n,                                            sizeof(int),
        AL4SAN_INOUT | AL4SAN_AFFINITY,    AL4SAN_ADDR(A,  double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                      &lda,                                          sizeof(int),
        AL4SAN_VALUE,                      &iinfo,                                        sizeof(int),
        AL4SAN_PRIORITY,                   options->priority,                             sizeof(int),
        AL4SAN_LABEL,                      "potrf",                                      sizeof(char),
        AL4SAN_COLOR,                      "green",                                       sizeof(char),
        ARG_END);

}


#if !defined(CHAMELEON_SIMULATION)
void potrf_cpu_fun(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int n;
     double *A;
    int lda;
    int iinfo;
    int info = 0;

     AL4SAN_Unpack_Arg(al4san_arg, &uplo, &n, &A, &lda, &iinfo);
    CORE_dpotrf(uplo, n, A, lda, &info);

}
#endif /* !defined(CHAMELEON_SIMULATION) */
