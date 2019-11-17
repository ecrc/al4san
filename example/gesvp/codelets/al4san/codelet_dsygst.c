/**
 *
 * @file qwrapper_dsygst.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:02:42 2016
 *
 **/
#include <lapacke.h>
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
//#include <runtime/al4san_parsec.h>
#undef COMPLEX
#define REAL

/***************************************************************************//**
 *
 **/

AL4SAN_TASK_CPU(dsygst, dsygst_cpu_func)
void EIG_AL4SAN_CORE_dsygst(AL4SAN_option_t *options,
                int itype, int uplo, int n,
                AL4SAN_desc_t *A, int Am, int An, int lda,
                AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
                int iinfo)
{

    AL4SAN_Insert_Task(AL4SAN_TASK(dsygst), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &itype,                      sizeof(int),
        AL4SAN_VALUE,                       &uplo,                      sizeof(int),
        AL4SAN_VALUE,                       &n,                      sizeof(int),
        AL4SAN_INOUT,                       AL4SAN_ADDR(A, double, Am, An),   AL4SAN_DEP, 
        AL4SAN_VALUE,                       &lda,                      sizeof(int),
        AL4SAN_INPUT,                       AL4SAN_ADDR(B, double, Bm, Bn),   AL4SAN_DEP,
        AL4SAN_VALUE,                       &ldb,                      sizeof(int),
        AL4SAN_VALUE,                       &iinfo,                      sizeof(int),
       ARG_END);
}

/***************************************************************************//**
 *
 **/
void dsygst_cpu_func(AL4SAN_arg_list *al4san_arg)
{
        int itype;
        int uplo;
        int n;
        double *A;
        int lda;
        double *B;
        int ldb;
        int iinfo;

        int info;

        AL4SAN_Unpack_Arg(al4san_arg, &itype, &uplo, &n, &A, &lda, &B, &ldb, &iinfo);
   
        char u;
         if(uplo==Al4sanUpper)
            u='U';
         else
           u='L';
 
        info = LAPACKE_dsygst_work(
                        LAPACK_COL_MAJOR,
                        itype,
                        u,
                        n, A, lda, B, ldb);
}
