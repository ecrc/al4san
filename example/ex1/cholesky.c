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
#include "cholesky.h"
#include <al4san.h>
#include "runtime/al4san_runtime.h"

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

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/
AL4SAN_TASK_CPU(potrf, potrf_tile)

void potrf_tile(AL4SAN_arg_list *al4san_arg)
{
   static int INFO;
   static const char L = 'L';
    int ts;
    int ld;
    double *A;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

   AL4SAN_Unpack_Arg(al4san_arg, &A, &ts, &ld);

   dpotrf_(&L, &ts, A, &ld, &INFO);
}

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/

AL4SAN_TASK_CPU(trsm, trsm_tile)

void trsm_tile(AL4SAN_arg_list *al4san_arg){
   static char LO = 'L', TR = 'T', NU = 'N', RI = 'R';
   static double DONE = 1.0;
    int ts, ld;
    double *A;
    double *B;

    /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

   AL4SAN_Unpack_Arg(al4san_arg, &A, &B, &ts, &ld);
   dtrsm_(&RI, &LO, &TR, &NU, &ts, &ts, &DONE, A, &ld, B, &ts);
}


/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/

AL4SAN_TASK_CPU(syrk, syrk_tile)
void syrk_tile(AL4SAN_arg_list *al4san_arg)
{
   static char LO = 'L', NT = 'N';
   static double DONE = 1.0, DMONE = -1.0;
    int ts, ld;
    double *A;
    double *B;

    /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

   AL4SAN_Unpack_Arg(al4san_arg, &A, &B, &ts, &ld);
   dsyrk_(&LO, &NT, &ts, &ts, &DMONE, A, &ts, &DONE, B, &ld);
}

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/

AL4SAN_TASK_CPU(gemm, gemm_tile)

void gemm_tile(AL4SAN_arg_list *al4san_arg)
{
   static const char TR = 'T', NT = 'N';
   static double DONE = 1.0, DMONE = -1.0;
     int tile_size, ld;
    double *A;
    double *B;
    double *C;
    /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
  */
   AL4SAN_Unpack_Arg(al4san_arg, &A, &B, &C, &tile_size, &ld);
   dgemm_(&NT, &TR,
          &tile_size, &tile_size, &tile_size,
          &DMONE, A, &ld,
          B, &ld,
          &DONE,
          C, &ld);
}

void cholesky_blocked(AL4SAN_option_t *options, const int ts, const int nt, double** Ah)
{


    int i, j, k;

    for (k = 0; k < nt; k++) {
        // Diagonal Block factorization and panel permutations

          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
          */
             AL4SAN_Insert_Task(AL4SAN_TASK(potrf), options, 
                                AL4SAN_INOUT, Ah[k*nt + k], AL4SAN_DEP,
                                AL4SAN_VALUE, &ts,          sizeof(int),
                                AL4SAN_VALUE, &ts,          sizeof(int),
                                ARG_END);
        // Triangular systems
        for (i = k + 1; i < nt; i++) {
          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
          */

            AL4SAN_Insert_Task(AL4SAN_TASK(trsm), options,
                               AL4SAN_INPUT, Ah[k*nt + k], AL4SAN_DEP,
                               AL4SAN_INOUT, Ah[k*nt + i], AL4SAN_DEP,
                               AL4SAN_VALUE, &ts,          sizeof(int),
                               AL4SAN_VALUE, &ts,          sizeof(int),
                               ARG_END);
        }

        // update trailing matrix
        for (i = k + 1; i < nt; i++) {
            for (j = k + 1; j < i; j++) {

          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
          */

                  AL4SAN_Insert_Task(AL4SAN_TASK(gemm), options,
                                     AL4SAN_INPUT, Ah[k*nt + i], AL4SAN_DEP,
                                     AL4SAN_INPUT, Ah[k*nt + j], AL4SAN_DEP,
                                     AL4SAN_INOUT, Ah[j*nt + i], AL4SAN_DEP,
                                     AL4SAN_VALUE, &ts,          sizeof(int),
                                     AL4SAN_VALUE, &ts,          sizeof(int),
                                     ARG_END);
            }

            /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
          */

            AL4SAN_Insert_Task(AL4SAN_TASK(syrk),options,
                               AL4SAN_INPUT, Ah[k*nt + i], AL4SAN_DEP,
                               AL4SAN_INOUT, Ah[i*nt + i], AL4SAN_DEP,
                               AL4SAN_VALUE, &ts,          sizeof(int),
                               AL4SAN_VALUE, &ts,          sizeof(int),
                               ARG_END);
        }
    }


}

int main(int argc, char* argv[])
{
   char *result[3] = {"n/a","sucessful","UNSUCCESSFUL"};
   const double eps = BLAS_dfpinfo( blas_eps );
   if ( argc < 5) {
      printf( "cholesky matrix_size block_size check threads\n" );
      exit( -1 );
   }
   const int  n = atoi(argv[1]); // matrix size
   const int ts = atoi(argv[2]); // tile size
   int check    = atoi(argv[3]); // check result?
   int num_reads= atoi(argv[4]);

    /****************
     Matrix Size should be divisible by tile size
    *****************/
   
    /* Initialize AL4SAN with main parameters */
   AL4SAN_context_t *al4san=AL4SAN_Init(num_reads, 0);

   /* 
     * Define AL4SAN handle for seqeunce to manage groupe of threads.
     * Define AL4SAN handle for options to set glabel task options and set the sequence handle. 
     * Define AL4SAN handle for request status. 
   */
   AL4SAN_sequence_t *sequence;
   AL4SAN_option_t options;
   AL4SAN_request_t* request = AL4SAN_REQUEST_INITIALIZER;

   // Allocate matrix
   double * const matrix = (double *) malloc(n * n * sizeof(double));
   assert(matrix != NULL);

   // Init matrix
   initialize_matrix(n, ts, matrix);


   // Allocate matrix
   double * const original_matrix = (double *) malloc(n * n * sizeof(double));
   assert(original_matrix != NULL);

   const int nt = n / ts;

   // Allocate blocked matrix
   double **Ah;
   Ah = (double **) malloc(nt * nt * sizeof(double *));
   assert(Ah != NULL);

   int j;
   for (j = 0; j < nt * nt; j++) {
      Ah[j]=(double *) malloc(ts * ts * sizeof(double));
      assert(Ah[j] != NULL);
   }

    int i;
   for (i = 0; i < n * n; i++ ) {
      original_matrix[i] = matrix[i];
   }



#ifdef VERBOSE
   printf ("Executing ...\n");
#endif

   convert_to_blocks(ts, nt, n,  (double (*)[n])matrix, (double* (*)[nt]) Ah);

   const float t1 = get_time();

   /*
     * Create sequence data sturcture 
     * Init task global options data sturcture
   */
   sequence=AL4SAN_Sequence_Create();
   AL4SAN_Runtime_options_init(&options,al4san, sequence, request);

    /* Cholesky factorization:
     * A is replaced by its factorization L or L^T depending on uplo */

   cholesky_blocked(&options, ts, nt,  Ah);

  /*
    * Use sequence for sync  
    * Destroy sequence
   */

   AL4SAN_Sequence_Wait(sequence);
   AL4SAN_Sequence_Destroy( sequence );

   const float t2 = get_time() - t1;
   convert_to_linear(ts, nt, n, (double* (*)[nt]) Ah, (double (*)[n]) matrix);

   if ( check ) {
      const char uplo = 'L';
      if ( check_factorization( n, original_matrix, matrix, n, uplo, eps) ) check++;
   }


   free(original_matrix);

   float time = t2;
   float gflops = (((1.0 / 3.0) * n * n * n) / ((time) * 1.0e+9));

   // Print results
#ifdef VERBOSE
   printf( "============ CHOLESKY RESULTS ============\n" );
   printf( "  matrix size:          %dx%d\n", n, n);
   printf( "  block size:           %dx%d\n", ts, ts);
   printf( "  number of threads:    %d\n", AL4SAN_Runtime_thread_size(al4san));
   printf( "  time (s):             %f\n", time);
   printf( "  performance (gflops): %f\n", gflops);
   printf( "==========================================\n" );
#else
   printf( "test:%s-%d-%d:threads:%2d:result:%s:gflops:%f\n", argv[0], n, ts, AL4SAN_Runtime_thread_size(al4san), result[check], gflops );
#endif



   for (j = 0; j < nt * nt; j++) {
      free(Ah[j]);
   }
   free(Ah);
   free(matrix);

   /*
    * Finilized options data 
   */
   AL4SAN_Runtime_options_finalize(&options, al4san);

   /* Finalize AL4SAN */
   AL4SAN_Finalize();

   // basic result status on return, depending on check value.
   switch (check) {
      case 1:
         return 0;
         break;
      case 2:
         return 1;
         break;
      default:
         return 0;
   }
}


