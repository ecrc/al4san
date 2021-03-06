/**
 *
 * @file al4san/codelet_dsymm.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon dsymm AL4SAN codelet
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-09-19
 * @generated d Wed Jan 16 11:13:56 2019
 *
 */
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
//#include <runtime/al4san_parsec.h>
/**
 *
 * @ingroup CORE_CHAMELEON_Complex64_t
 *
 */

AL4SAN_TASK_CPU_GPU(symm, symm_cpu_func, symm_cuda_func)

void EIG_AL4SAN_CORE_dsymm(AL4SAN_option_t *options,
                       int side, int uplo,
                       int m, int n, int nb,
                       double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                       const AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
                       double beta, const AL4SAN_desc_t *C, int Cm, int Cn, int ldc )
{
    (void)nb;

    /*AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_R(A, Am, An);
    AL4SAN_ACCESS_R(B, Bm, Bn);
    AL4SAN_ACCESS_RW(C, Cm, Cn);
    AL4SAN_END_ACCESS_DECLARATION;*/


    AL4SAN_Insert_Task(AL4SAN_TASK(symm), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                        &side,                                       sizeof(int),
        AL4SAN_VALUE,                        &uplo,                                       sizeof(int),
        AL4SAN_VALUE,                        &m,                                          sizeof(int),
        AL4SAN_VALUE,                        &n,                                          sizeof(int),
        AL4SAN_VALUE,                        &alpha,                                      sizeof(double),
        AL4SAN_INPUT,                        AL4SAN_ADDR(A, double, Am, An),              AL4SAN_DEP,
        AL4SAN_VALUE,                        &lda,                                        sizeof(int),
        AL4SAN_INPUT,                        AL4SAN_ADDR(B, double, Bm, Bn),              AL4SAN_DEP,
        AL4SAN_VALUE,                        &ldb,                                        sizeof(int),
        AL4SAN_VALUE,                        &beta,                                       sizeof(double),
        AL4SAN_INOUT | AL4SAN_AFFINITY,      AL4SAN_ADDR(C, double, Cm, Cn),              AL4SAN_DEP,
        AL4SAN_VALUE,                        &ldc,                                        sizeof(int),
        AL4SAN_CUDA_FLG,                     ON,                                          sizeof(int),
        AL4SAN_PRIORITY,                     options->priority,                           sizeof(int),
        AL4SAN_LABEL,                        "symm",                                     sizeof(char),
        ARG_END);

}


#if !defined(CHAMELEON_SIMULATION)
void symm_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int uplo;
    int M;
    int N;
    double alpha;
    double *A;
    int LDA;
    double *B;
    int LDB;
    double beta;
    double *C;
    int LDC;

    AL4SAN_Unpack_Arg(al4san_arg, &side, &uplo, &M, &N, &alpha, &A, &LDA, &B, &LDB, &beta, &C, &LDC);
    CORE_dsymm(side, uplo,
        M, N,
        alpha, A, LDA,
        B, LDB,
        beta, C, LDC);
}

#ifdef AL4SAN_USE_CUDA
void symm_cuda_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int uplo;
    int M;
    int N;
    double alpha;
    const double *A;
    int LDA;
    const double *B;
    int LDB;
    double beta;
    double *C;
    int LDC;

    AL4SAN_Unpack_Arg(al4san_arg, &side, &uplo, &M, &N, &alpha, &A, &LDA, &B, &LDB, &beta, &C, &LDC);

    AL4SAN_getStream(stream);

    CUDA_dsymm(
        side, uplo,
        M, N,
        &alpha, A, LDA,
        B, LDB,
        &beta, C, LDC,
        stream);

#ifndef AL4SAN_CUDA_ASYNC1
    cudaStreamSynchronize( stream );
#endif

    return;
}
#endif /* CHAMELEON_USE_CUDA */
#endif /* !defined(CHAMELEON_SIMULATION) */
