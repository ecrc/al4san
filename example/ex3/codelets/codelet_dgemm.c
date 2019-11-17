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
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @precisions normal z -> c d s
 *
 */

#include "../potrf.h"

//AL4SAN_TASK_HEADER(gemm)
AL4SAN_TASK_CPU_GPU(gemm, gemm_cpu_func, gemm_cuda_func)
//AL4SAN_QUARK_TASK_CPU(gemm, gemm_cpu_func)
//AL4SAN_STARPU_TASK_CPU(gemm, gemm_cpu_func)
//AL4SAN_PARSEC_TASK_CPU(gemm, gemm_cpu_func)
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

    AL4SAN_Unpack_Arg(al4san_arg, &transA, &transB, &m, &n, &k, &alpha, &A, &lda, &B, &ldb, &beta, &C, &ldc);
    CORE_dgemm(transA, transB,
               m, n, k,
               alpha, A, lda,
               B, ldb,
               beta, C, ldc);
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

    AL4SAN_Unpack_Arg(al4san_arg, &transA, &transB, &m, &n, &k, &alpha, &A, &lda, &B, &ldb, &beta, &C, &ldc);

    AL4SAN_getStream( stream );

    CUDA_dgemm(
        transA, transB,
        m, n, k,
        &alpha, A, lda,
        B, ldb,
        &beta,  C, ldc,
        stream);

#ifndef AL4SAN_CUDA_ASYNC1
    cudaStreamSynchronize( stream );
#endif

    return;
}
#endif // defined(CHAMELEON_USE_CUDA)

void INSERT_Task_dgemm( const AL4SAN_option_t *options,
                        cham_trans_t transA, cham_trans_t transB,
                        int m, int n, int k, int nb,
                        double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
                        double beta, const AL4SAN_desc_t *C, int Cm, int Cn, int ldc )
{
    (void)nb;

/*    AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_R(A, Am, An);
    AL4SAN_ACCESS_R(B, Bm, Bn);
    AL4SAN_ACCESS_RW(C, Cm, Cn);
    AL4SAN_END_ACCESS_DECLARATION;
*/
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
                       AL4SAN_CUDA_FLG,                 ON,                                            sizeof(int),
                       AL4SAN_PRIORITY,                 options->priority,                             sizeof(int),
//                       AL4SAN_CALLBACK,                 AL4SAN_CALLBACK(zgemm),                        sizeof(void),
                       AL4SAN_LABEL,                    "zgemm",                                       sizeof(char),
                       AL4SAN_COLOR,                    "yellow",                                      sizeof(char),
                       ARG_END);
}

