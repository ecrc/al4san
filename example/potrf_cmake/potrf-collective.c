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
   * @version 1.1.0
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
#include "potrf.h"

float get_time();
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


/* Cholesky factorization:
 * A is replaced by its factorization L or L^T depending on uplo */

int AL4SAN_cholesky(al4san_uplo_t uplo, AL4SAN_desc_t *A)
{

   /*
     * Define AL4SAN handle for seqeunce to manage groupe of threads.
     * Define AL4SAN handle for options to set glabel task options and set the sequence handle.
     * Define AL4SAN handle for request status.
   */

    AL4SAN_context_t *al4sanctxt;
    AL4SAN_sequence_t *sequence = NULL;
    AL4SAN_request_t* request = AL4SAN_REQUEST_INITIALIZER;
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


   /*
     * Create sequence data sturcture 
     * Init task global options data sturcture
   */
   sequence = AL4SAN_Sequence_Create();
   AL4SAN_Options_Init(&options, sequence, request);

       /* Variables used for collective */
        int root, num_dest_ranks, dest_rank_idx, flag;
        int *dest_ranks = (int*)malloc((A->q+A->p)*sizeof(int));;

        for (k = 0; k < A->nt; k++) {
            tempkm = k == A->nt-1 ? A->n-k*A->nb : A->nb;
            ldak = BLKLDD(A, k);

            if( AL4SAN_Data_Getrank( A, k, k) == AL4SAN_My_Mpi_Rank()) {
            options.priority = (A->nt - k) * (A->nt-k) * (A->nt - k);
                Task_dpotrf(
                &options,
                Al4sanUpper,
                tempkm, A->mb,
                A(k, k), ldak, A->nb*k);
            }
            /*
             * Broadcast the diagonal tile to the current panel
             */
            root =  AL4SAN_Data_Getrank( A, k, k);
            num_dest_ranks = A->q -1;

            dest_rank_idx = 0;
            flag = 0;
            for(int m = k+1; m < A->nt; m++) {
                int tile_rank =  AL4SAN_Data_Getrank( A, k, m);
                if(tile_rank == root) {flag = 1; continue;}
                dest_ranks[dest_rank_idx] = tile_rank;
                if(tile_rank == AL4SAN_My_Mpi_Rank()) flag = 1;
                ++dest_rank_idx;
                if(dest_rank_idx == A->q -1) break; /* this is to populate the destination ranks */
            }
               if( ( flag || (AL4SAN_My_Mpi_Rank() == root) ) && ( dest_rank_idx >= 1) ) {
              //if( ( dest_rank_idx >= 1) ) {
                int bcast_id = (1<<29) | (k* A->nt + k +k);
                AL4SAN_Broadcast_ID(
                        bcast_id, sequence, AL4SAN_My_Mpi_Rank(), root,
                        AL4SAN_Data_getaddr(A, k, k), AL4SAN_FULL_TILE,
                        dest_ranks, dest_rank_idx);
            }


            for (n = k+1; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n - n*A->nb : A->nb;

                options.priority = (A->nt - n) * (A->nt-n) * (A->nt - n) + 3 * ((2 * A->nt) - k - n - 1) * (n - k);
                if(  ( AL4SAN_Data_Getrank( A, k, n) == AL4SAN_My_Mpi_Rank() ||  AL4SAN_Data_Getrank( A, k, k) ==  AL4SAN_My_Mpi_Rank() )) {
                Task_dtrsm(
                    &options,
                    Al4sanLeft, Al4sanUpper, Al4sanConjTrans, Al4sanNonUnit,
                    A->mb, tempnn, A->mb,
                    zone, A(k, k), ldak,
                          A(k, n), ldak);
            }
                
            /*
            * Broadcast the TRSM tile to the descendant SYRK/GEMM tasks
            */
            root =  AL4SAN_Data_Getrank( A, k, n);
            num_dest_ranks = A->p + A->q -1;

            dest_rank_idx = 0;
            flag = 0;
            /* Loop over P and Q processes to gather the broadcast destinations */
            for(int i = k+1; i <= n; i++) {
                int tile_rank =  AL4SAN_Data_Getrank( A, i, n);
                if(tile_rank == root) {break;} /* we have loop over all the ranks in the column */
                dest_ranks[dest_rank_idx] = tile_rank;
                if(tile_rank == AL4SAN_My_Mpi_Rank()) flag = 1; /* flip the flag for the owner of the tile */
                ++dest_rank_idx;
                }

            int diag_rank =  AL4SAN_Data_Getrank( A, n, n);
            for(int j = n+1; j < A->nt; j++) {
                int tile_rank =  AL4SAN_Data_Getrank( A, n, j);
                if(tile_rank == diag_rank) {break;} /* we have loop over all the ranks in the column */
                dest_ranks[dest_rank_idx] = tile_rank;
                if(tile_rank == AL4SAN_My_Mpi_Rank()) flag = 1; /* flip the flag for the owner of the tile */
                ++dest_rank_idx;
                }

                   //if( ( dest_rank_idx >= 1) ) {
                if( ( flag || (AL4SAN_My_Mpi_Rank() == root) ) && ( dest_rank_idx >= 1) ) {
                     int bcast_id = (1<<29) | (k*A->nt+n+k); // m*nt+n+k
                    AL4SAN_Broadcast_ID(
                            bcast_id, sequence, AL4SAN_My_Mpi_Rank(), root,
                             AL4SAN_Data_getaddr(A, k, n), AL4SAN_FULL_TILE,
                            dest_ranks, dest_rank_idx);
                }
            }
            AL4SAN_Data_Flush( sequence, A(k, k) );

            for (m = k+1; m < A->mt; m++) {
                tempmm = m == A->mt-1 ? A->m - m*A->mb : A->mb;
                ldam = BLKLDD(A, m);

                options.priority = (A->nt - m) * (A->nt - m) * (A->nt - m) + 3 * (m - k);
            if( AL4SAN_Data_Getrank( A, m, m) == AL4SAN_My_Mpi_Rank() ||  AL4SAN_Data_Getrank( A, k, m) == AL4SAN_My_Mpi_Rank() ) {
                Task_dsyrk(
                    &options,
                    Al4sanUpper, Al4sanConjTrans,
                    tempmm, A->mb, A->mb,
                    -1.0, A(k, m), ldak,
                     1.0, A(m, m), ldam);
            }
                for (n = m+1; n < A->nt; n++) {
                    tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;
                    if( AL4SAN_Data_Getrank( A, m, n) == AL4SAN_My_Mpi_Rank()  ||  AL4SAN_Data_Getrank( A, k, m) == AL4SAN_My_Mpi_Rank()  ||  AL4SAN_Data_Getrank( A, k, n) == AL4SAN_My_Mpi_Rank() ) {
                    options.priority = (A->nt - m) * (A->nt - m) * (A->nt - m) + 3 * ((2 * A->nt) - m - n - 3) * (m - n) + 6 * (m - k);
                    Task_dgemm(
                        &options,
                        Al4sanTrans, Al4sanNoTrans,
                        tempmm, tempnn, A->mb, A->mb,
                        mzone, A(k, m), ldak,
                               A(k, n), ldak,
                        zone,  A(m, n), ldam);
                }
                }
                AL4SAN_Data_Flush( sequence, A(k, m) );
            }

        }

   /*
    * Finilized options data
   */
   AL4SAN_Options_Finalize(&options);

   AL4SAN_Desc_Flush( A, sequence );

  /*
    * Use sequence for sync
    * Destroy sequence
   */
   free(dest_ranks);
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
    al4san_uplo_t UPLO = Al4sanUpper; // where is stored L
    /* descriptors necessary for calling AL4SAN data descriptor interface  */
    AL4SAN_desc_t *descA = NULL,  *descAC = NULL, *descB = NULL, *descX = NULL;
    /* declarations to time the program and evaluate performances */
    double fmuls, fadds, flops, gflops, cpu_time= 0.0;

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
    fadds = (double)( FADDS_POTRF(N));
    fmuls = (double)( FMULS_POTRF(N));

    flops = 1e-9 * (fmuls + fadds);

    /* initialize the number of thread if not given by the user in argv
     * It makes sense only if this program is linked with pthread and
     * multithreaded BLAS and LAPACK */
    if ( iparam[IPARAM_THRDNBR] == -1 ) {
        get_thread_count( &(iparam[IPARAM_THRDNBR]) );
    }
    NCPU = iparam[IPARAM_THRDNBR];
    NGPU = iparam[IPARAM_GPUS];


    /* Initialize AL4SAN with main parameters */
   AL4SAN_context_t *al4san = AL4SAN_Init(runtime, NCPU, NGPU);

#if defined(AL4SAN_USE_MPI)
    al4san->prows=iparam[IPARAM_P];
    al4san->pcols=iparam[IPARAM_Q];
    AL4SAN_Init_Processor_Grid(al4san->prows, al4san->pcols);
#endif

    /* print informations to user */
    print_header( argv[0], iparam);
    /*
     * Allocate memory for our data using a C macro (see step2.h)
     *     - matrix A                   : size N x N
     */

    /*
     * Initialize the structure required for AL4SAN data interface
     * AL4SAN_desc_t is a structure wrapping your data allowing AL4SAN to get
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
     *     - give the data type (Al4sanByte, Al4sanInteger, Al4sanRealFloat,
     *     Al4sanRealDouble, Al4sanComplexFloat, Al4sanComplexDouble)
     *     - number of rows in a block (tile)
     *     - number of columns in a block (tile)
     *     - number of elements in a block (tile)
     * The other parameters are specific, use:
     * AL4SAN_Desc_Create( ... , 0, 0, number of rows, number of columns, 1, 1);
     * Have a look to the documentation for details about these parameters.
     */

    AL4SAN_Matrix_Create(&descA,  NULL, Al4sanRealDouble,
                      AL4SAN_Col_Major, NB, NB, NB, N, N, N);
    /* generate A matrix with random values such that it is spd*/
    dplgsy_Tile( (double)N, Al4sanUpperLower, descA, 51 );

    cpu_time = -AL4SAN_timer();
    /* Cholesky factorization:
     * A is replaced by its factorization L or L^T depending on uplo */
    AL4SAN_cholesky(UPLO, descA);

    cpu_time += AL4SAN_timer();
    /* print informations to user */

    /* print informations to user */
    gflops = flops / cpu_time;

  if(AL4SAN_My_Mpi_Rank()==0){
    printf( "%9.3f +- %9.2f\n", cpu_time, gflops);
    fflush( stdout );
    }

    /* deallocate A and associated descriptors descA, ... */
   AL4SAN_Desc_Destroy( &descA );

   /* Finalize AL4SAN */
   AL4SAN_Finalize();
   return 0;
}

