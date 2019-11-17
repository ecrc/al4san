/**
 *
 * @file cholesky.c
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/
/**
   * @file cholesky.c
   * @brief Cholesky Example
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.1
   * @author Rabab Alomairy
   * @date 2019-02-06
   *
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <pthread.h>
#include <al4san.h>
#include <chameleon.h>
#include <coreblas.h>
#include <coreblas/lapacke.h>
#include <coreblas/cblas.h>
#include <chameleon/timer.h>
#include "potrf.h"
/*
 * @briefThis example only factorizes using Cholesky using the C interface of
 * BLAS/LAPACK.
 * @details This program solves a linear system AX=B with matrix A symmetric
 * positive definite.
 * The matrix A is first factorized using the Cholesky factorization, A = LL^T.
 * Then the solution X is calculated thanks to forward and back substitutions.
 * We use the C interface of BLAS and LAPACK, that is, CBLAS and LAPACKE, to solve this problem.
 * This program is either sequential if it is linked with a sequential LAPACK library
 * or parallel if it is linked with a multithreaded LAPACK library.
 * The precision is: double
 */

#define A(m,n) A,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )

int AL4SAN_cholesky(cham_uplo_t uplo, AL4SAN_desc_t *A)
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_sequence_t *sequence = NULL;
    AL4SAN_request_t* request = RUNTIME_REQUEST_INITIALIZER;
    AL4SAN_option_t options;

    int status;
    int k, m, n;
    int ldak, ldam, ldan;
    int tempkm, tempmm, tempnn;
    size_t ws_host   = 0;

    double zone  = (double) 1.0;
    double mzone = (double)-1.0;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("AL4SAN_cholesky", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    //AL4SAN_Runtime_options_ws_alloc( &options, 0, ws_host );

   /*
     * Create sequence data sturcture 
     * Init task global options data sturcture
   */
   sequence = AL4SAN_Sequence_Create();
   AL4SAN_Options_Init(&options, sequence, request);

        for (k = 0; k < A->nt; k++) {
            tempkm = k == A->nt-1 ? A->n-k*A->nb : A->nb;
            ldak = BLKLDD(A, k);

            options.priority = 2*A->nt - 2*k;
            INSERT_Task_dpotrf(
                &options,
                ChamUpper,
                tempkm, A->mb,
                A(k, k), ldak, A->nb*k);

            for (n = k+1; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n - n*A->nb : A->nb;

                options.priority = 2*A->nt - 2*k - n;
                INSERT_Task_dtrsm(
                    &options,
                    ChamLeft, ChamUpper, ChamConjTrans, ChamNonUnit,
                    A->mb, tempnn, A->mb,
                    zone, A(k, k), ldak,
                          A(k, n), ldak);
            }
            AL4SAN_Data_Flush( sequence, A(k, k) );

            for (m = k+1; m < A->mt; m++) {
                tempmm = m == A->mt-1 ? A->m - m*A->mb : A->mb;
                ldam = BLKLDD(A, m);

                options.priority = 2*A->nt - 2*k  - m;
                INSERT_Task_dsyrk(
                    &options,
                    ChamUpper, ChamConjTrans,
                    tempmm, A->mb, A->mb,
                    -1.0, A(k, m), ldak,
                     1.0, A(m, m), ldam);

                for (n = m+1; n < A->nt; n++) {
                    tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;

                    options.priority = 2*A->nt - 2*k - n - m;
                    INSERT_Task_dgemm(
                        &options,
                        ChamConjTrans, ChamNoTrans,
                        tempmm, tempnn, A->mb, A->mb,
                        mzone, A(k, m), ldak,
                               A(k, n), ldak,
                        zone,  A(m, n), ldam);
                }
                AL4SAN_Data_Flush( sequence, A(k, m) );
            }

        }

  //AL4SAN_Runtime_options_ws_free(&options);
   AL4SAN_Options_Finalize(&options);

   AL4SAN_Desc_Flush( A, sequence );
   AL4SAN_Sequence_Wait(sequence);
   AL4SAN_Sequence_Destroy( sequence );


   return AL4SAN_SUCCESS;
}

int main(int argc, char* argv[]){
    size_t N; // matrix order
    int NB;   // number of rows and columns in tiles
    int NRHS; // number of RHS vectors
    int NCPU; // number of cores to use
    int NGPU; // number of gpus (cuda devices) to use
    int UPLO = ChamUpper; // where is stored L

    /* descriptors necessary for calling CHAMELEON tile interface  */
    AL4SAN_desc_t *descA = NULL,  *descAC = NULL, *descB = NULL, *descX = NULL;
    //CHAM_desc_t *descAC = NULL, *descB = NULL, *descX = NULL;
    /* declarations to time the program and evaluate performances */
    double fmuls, fadds, flops, gflops, cpu_time;

    /* variable to check the numerical results */
    double anorm, bnorm, xnorm, eps, res;
    int hres;

    /* initialize some parameters with default values */
    int iparam[IPARAM_SIZEOF];
    memset(iparam, 0, IPARAM_SIZEOF*sizeof(int));
    init_iparam(iparam);

    /* read arguments */
    read_args(argc, argv, iparam);
    N    = iparam[IPARAM_N];
    NRHS = iparam[IPARAM_NRHS];
    NB =  iparam[IPARAM_NB];
    /* compute the algorithm complexity to evaluate performances */
    fadds = (double)( FADDS_POTRF(N) + 2 * FADDS_TRSM(N,NRHS) );
    fmuls = (double)( FMULS_POTRF(N) + 2 * FMULS_TRSM(N,NRHS) );
    flops = 1e-9 * (fmuls + fadds);

    /* initialize the number of thread if not given by the user in argv
     * It makes sense only if this program is linked with pthread and
     * multithreaded BLAS and LAPACK */
    if ( iparam[IPARAM_THRDNBR] == -1 ) {
        get_thread_count( &(iparam[IPARAM_THRDNBR]) );
    }
    NCPU = iparam[IPARAM_THRDNBR];
    NGPU = 1;

    /* print informations to user */
    print_header( argv[0], iparam);

    /* Initialize AL4SAN with main parameters */
   AL4SAN_context_t *al4san = AL4SAN_Init("Starpu", NCPU, NGPU);
      printf("\b schuler:%d\n", al4san->scheduler);
    /* Initialize CHAMELEON with main parameters */

    int rc = CHAMELEON_Init( NCPU, NGPU );
    printf("\n CHAMELEON_Init:%d\n", rc);   
    /*
     * Allocate memory for our data using a C macro (see step2.h)
     *     - matrix A                   : size N x N
     *     - set of RHS vectors B       : size N x NRHS
     *     - set of solutions vectors X : size N x NRHS
     */

    /*
     * Initialize the structure required for CHAMELEON tile interface
     * CHAM_desc_t is a structure wrapping your data allowing CHAMELEON to get
     * pointers to tiles. A tile is a data subset of your matrix on which we
     * apply some optimized CPU/GPU kernels.
     * Notice that this routine suppose your matrix is a contiguous vector of
     * data (1D array), as a data you would give to BLAS/LAPACK.
     * Main arguments:
     *     - descA is a pointer to a descriptor, you need to give the address
     *     of this pointer
     *     - if you want to give your allocated matrix give its address,
     *     if not give a NULL pointer, the routine will allocate the memory
     *     and you access the matrix data with descA->mat
     *     - give the data type (ChamByte, ChamInteger, ChamRealFloat,
     *     ChamRealDouble, ChamComplexFloat, ChamComplexDouble)
     *     - number of rows in a block (tile)
     *     - number of columns in a block (tile)
     *     - number of elements in a block (tile)
     * The other parameters are specific, use:
     * CHAMELEON_Desc_Create( ... , 0, 0, number of rows, number of columns, 1, 1);
     * Have a look to the documentation for details about these parameters.
     */
    AL4SAN_Desc_Create(&descA,  NULL, Al4sanRealDouble,
                      NB, NB,  NB*NB, N, N, 0, 0, N, N, 1, 1);
    AL4SAN_Desc_Create(&descB,  NULL, Al4sanRealDouble,
                      NB, NB,  NB*NB, N, NRHS, 0, 0, N, NRHS, 1, 1);
    AL4SAN_Desc_Create(&descX,  NULL, Al4sanRealDouble,
                      NB, NB,  NB*NB, N, NRHS, 0, 0, N, NRHS, 1, 1);
    AL4SAN_Desc_Create(&descAC, NULL, Al4sanRealDouble,
                      NB, NB,  NB*NB, N, N, 0, 0, N, N, 1, 1);

   /* CHAMELEON_Desc_Create(&descA,  NULL, ChamRealDouble,
                      NB, NB,  NB*NB, N, N, 0, 0, N, N, 1, 1);
    CHAMELEON_Desc_Create(&descB,  NULL, ChamRealDouble,
                      NB, NB,  NB*NB, N, NRHS, 0, 0, N, NRHS, 1, 1);
    CHAMELEON_Desc_Create(&descX,  NULL, ChamRealDouble,
                      NB, NB,  NB*NB, N, NRHS, 0, 0, N, NRHS, 1, 1);
    CHAMELEON_Desc_Create(&descAC, NULL, ChamRealDouble,
                      NB, NB,  NB*NB, N, N, 0, 0, N, N, 1, 1);
*/
    /* generate A matrix with random values such that it is spd */
    CHAMELEON_dplgsy_Tile( (double)N, ChamUpperLower, (CHAM_desc_t*) descA, 51 );
    /* generate RHS */
    CHAMELEON_dplrnt_Tile( (CHAM_desc_t*) descB, 5673 );

    /* copy A before facto. in order to check the result */
    CHAMELEON_dlacpy_Tile(ChamUpperLower, (CHAM_desc_t*) descA, (CHAM_desc_t*) descAC);

    /* copy B in X before solving
     * same sense as memcpy(X, B, N*NRHS*sizeof(double)) but for descriptors */
    CHAMELEON_dlacpy_Tile(ChamUpperLower, (CHAM_desc_t*) descB, (CHAM_desc_t*) descX);

    /************************************************************/
    /* solve the system AX = B using the Cholesky factorization */
    /************************************************************/

    cpu_time = -CHAMELEON_timer();

    /* Cholesky factorization:
     * A is replaced by its factorization L or L^T depending on uplo */
    //CHAMELEON_dpotrf_Tile( UPLO, (CHAM_desc_t*) descA );
    printf("\nBefore AL4SAN_cholesky\n");
    AL4SAN_cholesky(UPLO, descA);

    cpu_time += CHAMELEON_timer();
    /* Solve:
     * B is stored in X on entry, X contains the result on exit.
     * Forward and back substitutions
     */

    CHAMELEON_dpotrs_Tile( UPLO, (CHAM_desc_t*) descA, (CHAM_desc_t*) descX );


    /* print informations to user */
    gflops = flops / cpu_time;
    printf( "%9.3f %9.2f\n", cpu_time, gflops);
    fflush( stdout );

    /************************************************************/
    /* check if solve is correct i.e. AX-B = 0                  */
    /************************************************************/

    /* compute norms to check the result */
    anorm = CHAMELEON_dlange_Tile( ChamInfNorm, (CHAM_desc_t*) descAC);
    bnorm = CHAMELEON_dlange_Tile( ChamInfNorm, (CHAM_desc_t*) descB);
    xnorm = CHAMELEON_dlange_Tile( ChamInfNorm, (CHAM_desc_t*) descX);

    /* compute A*X-B, store the result in B */
    CHAMELEON_dgemm_Tile( ChamNoTrans, ChamNoTrans,
                      1.0, (CHAM_desc_t*) descAC, (CHAM_desc_t*) descX, -1.0, (CHAM_desc_t*) descB );
    res = CHAMELEON_dlange_Tile( ChamInfNorm, (CHAM_desc_t*) descB );

    /* check residual and print a message */
    eps = LAPACKE_dlamch_work( 'e' );

    /*
     * if hres = 0 then the test succeed
     * else the test failed
     */
    hres = ( res / N / eps / (anorm * xnorm + bnorm ) > 100.0 );
    printf( "   ||Ax-b||       ||A||       ||x||       ||b|| ||Ax-b||/N/eps/(||A||||x||+||b||)  RETURN\n");
    if (hres) {
        printf( "%8.5e %8.5e %8.5e %8.5e                       %8.5e FAILURE \n",
            res, anorm, xnorm, bnorm,
            res / N / eps / (anorm * xnorm + bnorm ));
    }
    else {
        printf( "%8.5e %8.5e %8.5e %8.5e                       %8.5e SUCCESS \n",
            res, anorm, xnorm, bnorm,
            res / N / eps / (anorm * xnorm + bnorm ));
    }

    /* deallocate A, B, X, Acpy and associated descriptors descA, ... */
   AL4SAN_Desc_Destroy( &descA );
   AL4SAN_Desc_Destroy( &descB );
   AL4SAN_Desc_Destroy( &descX );
   AL4SAN_Desc_Destroy( &descAC );

   /*CHAMELEON_Desc_Destroy( &descA );
   CHAMELEON_Desc_Destroy( &descB );
   CHAMELEON_Desc_Destroy( &descX );
   CHAMELEON_Desc_Destroy( &descAC );
*/
    /*
     * Required semicolon to have at least one inst
     * before the end of OpenMP block.
     */
    ;
    /* Finalize CHAMELEON */
//    CHAMELEON_Finalize();
 
   /* Finalize AL4SAN */
   AL4SAN_Finalize();

}


