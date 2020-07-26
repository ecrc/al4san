/**
 *
 * @file al4san/codelet_ztrsm.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon ztrsm AL4SAN codelet
 *
 * @version 1.0.1
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for AL4SAN 1.0.1
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
AL4SAN_TASK_CPU_GPU(trsm, trsm_cpu_func, trsm_cuda_func)


void Task_dtrsm( const AL4SAN_option_t *options,
                       al4san_side_t side, al4san_uplo_t uplo, al4san_trans_t transA, al4san_diag_t diag,
                       int m, int n, int nb,
                       double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                       const AL4SAN_desc_t *B, int Bm, int Bn, int ldb )
{

          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies
          */

    AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_R(A, Am, An);
    AL4SAN_ACCESS_RW(B, Bm, Bn);
    AL4SAN_END_ACCESS_DECLARATION;


    AL4SAN_Insert_Task(AL4SAN_TASK(trsm), (AL4SAN_option_t * )options,
        AL4SAN_VALUE,                      &side,                                        sizeof(int),         
        AL4SAN_VALUE,                      &uplo,                                        sizeof(int),
        AL4SAN_VALUE,                      &transA,                                      sizeof(int),
        AL4SAN_VALUE,                      &diag,                                        sizeof(int),        
        AL4SAN_VALUE,                      &m,                                           sizeof(int),
        AL4SAN_VALUE,                      &n,                                           sizeof(int),
        AL4SAN_VALUE,                      &alpha,                                       sizeof(double),
        AL4SAN_INPUT,                      AL4SAN_ADDR(A, double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                      &lda,                                         sizeof(int),
        AL4SAN_INOUT | AL4SAN_AFFINITY,    AL4SAN_ADDR(B, double, Bm, Bn),  AL4SAN_DEP,
        AL4SAN_VALUE,                      &ldb,                                         sizeof(int), 
#ifdef AL4SAN_USE_CUDA
        AL4SAN_CUDA_FLG,                   ON,                                           sizeof(int),
#endif
        AL4SAN_PRIORITY,                   options->priority,                            sizeof(int),
        AL4SAN_LABEL,                      "ztrsm",                                      sizeof(char),
        ARG_END);
}

void trsm_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int uplo;
    int transA;
    int diag;
    int m;
    int n;
    double alpha; 
    double *A;
    int lda;
    double *B;
    int ldb;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

    AL4SAN_Unpack_Arg(al4san_arg, &side, &uplo, &transA, &diag, &m, &n, &alpha, &A, &lda, &B, &ldb);

    cblas_dtrsm(
        CblasColMajor,
        (CBLAS_SIDE)side, (CBLAS_UPLO)uplo,
        (CBLAS_TRANSPOSE)transA, (CBLAS_DIAG)diag,
        m, n,
        (alpha), A, lda,
        B, ldb);

}
#ifdef AL4SAN_USE_CUDA
void trsm_cuda_func(AL4SAN_arg_list *al4san_arg) 
{
    int side;
    int uplo;
    int transA;
    int diag;
    int m;
    int n;
    double alpha;
    const double *A;
    int lda;
    double *B;
    int ldb;
   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */
   AL4SAN_Unpack_Arg(al4san_arg, &side, &uplo, &transA, &diag, &m, &n, &alpha, &A, &lda, &B, &ldb);

    AL4SAN_getStream(stream);

    CUDA_dtrsm(
        side, uplo, transA, diag,
        m, n,
        &alpha, A, lda,
        B, ldb,
        stream);

#ifndef AL4SAN_CUDA_ASYNC
    cudaStreamSynchronize( stream );
#endif

    return;
}
#endif /* AL4SAN_USE_CUDA */
