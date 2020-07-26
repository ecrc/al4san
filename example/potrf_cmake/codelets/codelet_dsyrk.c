/**
 *
 * @file al4san/codelet_zsyrk.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon zsyrk AL4SAN codelet
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
 *
 */

#include "../potrf.h"

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument cpu  user function name
  * @param[in] Second argument gpu user function name
*/

AL4SAN_TASK_CPU_GPU(syrk, syrk_cpu_func, syrk_cuda_func)

void Task_dsyrk( const AL4SAN_option_t *options,
                       al4san_uplo_t uplo, al4san_trans_t trans,
                       int n, int k, int nb,
                       double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double beta, const AL4SAN_desc_t *C, int Cm, int Cn, int ldc )
{
    (void)nb;
          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies
          */ 
    AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_R(A, Am, An);
    AL4SAN_ACCESS_RW(C, Cm, Cn);
    AL4SAN_END_ACCESS_DECLARATION;

   AL4SAN_Insert_Task(AL4SAN_TASK(syrk), (AL4SAN_option_t * )options,         
        AL4SAN_VALUE,                      &uplo,                                        sizeof(int),
        AL4SAN_VALUE,                      &trans,                                       sizeof(int),
        AL4SAN_VALUE,                      &n,                                           sizeof(int),
        AL4SAN_VALUE,                      &k,                                           sizeof(int),
        AL4SAN_VALUE,                      &alpha,                                       sizeof(double),
        AL4SAN_INPUT,                      AL4SAN_ADDR(A,  double, Am, An), AL4SAN_DEP,
        AL4SAN_VALUE,                      &lda,                                         sizeof(int),
        AL4SAN_VALUE,                      &beta,                                        sizeof(double),
        AL4SAN_INOUT | AL4SAN_AFFINITY,    AL4SAN_ADDR(C,  double, Cm, Cn), AL4SAN_DEP,
        AL4SAN_VALUE,                      &ldc,                                         sizeof(int), 
#ifdef AL4SAN_USE_CUDA
        AL4SAN_CUDA_FLG,                   ON,                                           sizeof(int),
#endif
        AL4SAN_PRIORITY,                   options->priority,                            sizeof(int),
        AL4SAN_LABEL,                      "zsyrk",                                      sizeof(char),
//        AL4SAN_COLOR,                      "red",                                        sizeof(char),
        ARG_END);

}

void syrk_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int trans;
    int n;
    int k;
    double alpha;
    double  *A;
    int lda;
    double beta;
    double *C;
    int ldc;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

   AL4SAN_Unpack_Arg(al4san_arg, &uplo, &trans, &n, &k, &alpha, &A, &lda, &beta, &C, &ldc);

    cblas_dsyrk(
        CblasColMajor,
        (CBLAS_UPLO)uplo, (CBLAS_TRANSPOSE)trans,
        n, k,
        (alpha), A, lda,
        (beta), C, ldc);

}

#ifdef AL4SAN_USE_CUDA
void syrk_cuda_func(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int trans;
    int n;
    int k;
    double alpha;
    const double *A;
    int lda;
    double beta;
    double *C;
    int ldc;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

   AL4SAN_Unpack_Arg(al4san_arg, &uplo, &trans, &n, &k, &alpha, &A, &lda, &beta, &C, &ldc);
    
    AL4SAN_getStream(stream);

    CUDA_dsyrk(
        uplo, trans,
        n, k,
        &alpha, A, lda,
        &beta, C, ldc,
        stream);

#ifndef AL4SAN_CUDA_ASYNC
    cudaStreamSynchronize( stream );
#endif

    return;
}
#endif /* AL4SAN_USE_CUDA */

