/**
 *
 * @file al4san/codelet_zpotrf.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon zpotrf AL4SAN codelet
 *
 * @version 1.0.1
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 1.0.0
 * @author Hatem Ltaief
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @precisions normal z -> c d s
 *
 */

#include "../potrf.h"


/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/

AL4SAN_TASK_CPU(potrf, potrf_cpu_fun)


void INSERT_Task_dpotrf( const AL4SAN_option_t *options,
                        cham_uplo_t uplo, int n, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        int iinfo )
{

          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies
          */

   AL4SAN_Insert_Task(AL4SAN_TASK(potrf), (AL4SAN_option_t * )options,         
        AL4SAN_VALUE,                      &uplo,                                         sizeof(int),
        AL4SAN_VALUE,                      &n,                                            sizeof(int),
        AL4SAN_INOUT | AL4SAN_AFFINITY,    AL4SAN_ADDR(A,  double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                      &lda,                                          sizeof(int),
        AL4SAN_VALUE,                      &(options->sequence),                          sizeof(AL4SAN_sequence_t*),
        AL4SAN_VALUE,                      &(options->request),                           sizeof(AL4SAN_request_t*),
        AL4SAN_VALUE,                      &iinfo,                                        sizeof(int), 
        AL4SAN_PRIORITY,                   options->priority,                             sizeof(int),
        AL4SAN_LABEL,                      "zpotrf",                                      sizeof(char),
        AL4SAN_COLOR,                      "green",                                       sizeof(char),
        ARG_END);

}


void potrf_cpu_fun(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int n;
    double *A;
    int lda;
    int iinfo;
    int info = 0;
    AL4SAN_sequence_t* sequence;
    AL4SAN_request_t* request;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

     AL4SAN_Unpack_Arg(al4san_arg, &uplo, &n, &A, &lda, &sequence, &request, &iinfo);
    CORE_dpotrf(uplo, n, A, lda, &info);


}
