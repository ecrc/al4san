/**
 *
 * @file al4san/codelet_zgemm.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology
 *                      (KAUST).  All rights reserved.
 *
 ***
 *
 * @brief Chameleon zgemm AL4SAN codelet
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @precisions normal z -> c d s
 *
 */

#include "../potrf.h"


/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument cpu  user function name
  * @param[in] Second argument gpu user function name
*/

AL4SAN_TASK_CPU_GPU(gemm, gemm_cpu_func, gemm_cuda_func)

void gemm_cpu_func( AL4SAN_arg_list *al4san_arg)
{

    int transA;
    int transB;
    int m;
    int n;
    int k;
    double alpha;
    double *A;
    int lda;
    double *B;
    int ldb;
    double beta;
    double *C;
    int ldc;


   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */


    AL4SAN_Unpack_Arg(al4san_arg, &transA, &transB, &m, &n, &k, &alpha, &A, &lda, &B, &ldb, &beta, &C, &ldc);

    cblas_dgemm(
        CblasColMajor,
        (CBLAS_TRANSPOSE)transA, (CBLAS_TRANSPOSE)transB,
        m, n, k,
        (alpha), A, lda,
        B, ldb,
        (beta), C, ldc);

}

#ifdef AL4SAN_USE_CUDA
void gemm_cuda_func(AL4SAN_arg_list *al4san_arg)
{
    int transA;
    int transB;
    int m;
    int n;
    int k;
    double alpha;
    const double *A;
    int lda;
    const double *B;
    int ldb;
    double beta;
    double *C;
    int ldc;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

    AL4SAN_Unpack_Arg(al4san_arg, &transA, &transB, &m, &n, &k, &alpha, &A, &lda, &B, &ldb, &beta, &C, &ldc);

    AL4SAN_getStream( stream );

    CUDA_dgemm(
        transA, transB,
        m, n, k,
        &alpha, A, lda,
        B, ldb,
        &beta,  C, ldc,
        stream);

#ifndef AL4SAN_CUDA_ASYNC
    cudaStreamSynchronize( stream );
#endif

    return;
}
#endif // defined(AL4SAN_USE_CUDA)

void Task_dgemm( const AL4SAN_option_t *options,
                        al4san_trans_t transA, al4san_trans_t transB,
                        int m, int n, int k, int nb,
                        double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
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
    AL4SAN_ACCESS_R(B, Bm, Bn);
    AL4SAN_ACCESS_RW(C, Cm, Cn);
    AL4SAN_END_ACCESS_DECLARATION;

    AL4SAN_Insert_Task(AL4SAN_TASK(gemm), (AL4SAN_option_t*)options,
                       AL4SAN_VALUE,                    &transA,                                       sizeof(int),
                       AL4SAN_VALUE,                    &transB,                                       sizeof(int),
                       AL4SAN_VALUE,                    &m,                                            sizeof(int),
                       AL4SAN_VALUE,                    &n,                                            sizeof(int),
                       AL4SAN_VALUE,                    &k,                                            sizeof(int),
                       AL4SAN_VALUE,                    &alpha,                                        sizeof(double),
                       AL4SAN_INPUT,                    AL4SAN_ADDR(A, double, Am, An),   AL4SAN_DEP,
                       AL4SAN_VALUE,                    &lda,                                          sizeof(int),
                       AL4SAN_INPUT,                    AL4SAN_ADDR(B, double, Bm, Bn),   AL4SAN_DEP,
                       AL4SAN_VALUE,                    &ldb,                                          sizeof(int),
                       AL4SAN_VALUE,                    &beta,                                         sizeof(double),
                       AL4SAN_INOUT | AL4SAN_AFFINITY,  AL4SAN_ADDR(C, double, Cm, Cn),   AL4SAN_DEP,
                       AL4SAN_VALUE,                    &ldc,                                          sizeof(int),
#ifdef AL4SAN_USE_CUDA

                       AL4SAN_CUDA_FLG,                 ON,                                            sizeof(int),
#endif
                       AL4SAN_PRIORITY,                 options->priority,                             sizeof(int),
                       AL4SAN_LABEL,                    "zgemm",                                       sizeof(char),
//                       AL4SAN_COLOR,                    "yellow",                                      sizeof(char),
                       ARG_END);
}

