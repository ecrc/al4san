/**
 *
 * @file potrf.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon step2 example header
 *
 * @version 1.1.0
 * @author Florent Pruvost
 * @date 2019-02-06
 *
 */
#ifndef _potrf_h_
#define _potrf_h_

/* Common include for all steps of the tutorial */



/* Specific includes for potrf */

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
#include <al4san/timer.h>
#include "control/al4san_descriptor.h"
#include "runtime/al4san_runtime.h"
//#include <lapacke.h>
#include <mkl.h>
#include <sys/time.h>
#include <sys/times.h>
#if defined( _WIN32 ) || defined( _WIN64 )
#include <windows.h>
#else  /* Non-Windows */
#include <unistd.h>
#include <sys/resource.h>
#endif

/* Common functions for all steps of the tutorial */
static void get_thread_count(int *thrdnbr) {
#if defined WIN32 || defined WIN64
    sscanf( getenv( "NUMBER_OF_PROCESSORS" ), "%d", thrdnbr );
#else
    *thrdnbr = sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

/* define complexity of algorithms - see Lawn 41 page 120 */
#define FMULS_POTRF(__n) ((double)(__n) * (((1. / 6.) * (double)(__n) + 0.5) * (double)(__n) + (1. / 3.)))
#define FADDS_POTRF(__n) ((double)(__n) * (((1. / 6.) * (double)(__n)      ) * (double)(__n) - (1. / 6.)))
#define FMULS_TRSM(__m, __n) (0.5 * (double)(__n) * (double)(__m) * ((double)(__m)+1.))
#define FADDS_TRSM(__m, __n) (0.5 * (double)(__n) * (double)(__m) * ((double)(__m)-1.))


/*
    Cholesky Task Headers
*/

void Task_dpotrf( const AL4SAN_option_t *options,
                        al4san_uplo_t uplo, int n, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        int iinfo );
void Task_dgemm( const AL4SAN_option_t *options,
                        al4san_trans_t transA, al4san_trans_t transB,
                        int m, int n, int k, int nb,
                        double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
                        double beta, const AL4SAN_desc_t *C, int Cm, int Cn, int ldc );
void TASK_dplgsy( const AL4SAN_option_t *options,
                        double bump, int m, int n, AL4SAN_desc_t *A, int Am, int An, int lda,
                        int bigM, int m0, int n0, unsigned long long int seed );
void Task_dsyrk( const AL4SAN_option_t *options,
                       al4san_uplo_t uplo, al4san_trans_t trans,
                       int n, int k, int nb,
                       double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                       double beta, const AL4SAN_desc_t *C, int Cm, int Cn, int ldc );
void Task_dtrsm( const AL4SAN_option_t *options,
                       al4san_side_t side, al4san_uplo_t uplo, al4san_trans_t transA, al4san_diag_t diag,
                       int m, int n, int nb,
                       double alpha, const AL4SAN_desc_t *A, int Am, int An, int lda,
                       const AL4SAN_desc_t *B, int Bm, int Bn, int ldb );
void CORE_dplgsy( double bump, int m, int n, double *A, int lda,
                  int bigM, int m0, int n0, unsigned long long int seed );

int dplgsy_Tile( double bump, al4san_uplo_t uplo,
                           AL4SAN_desc_t *A,
                           unsigned long long int seed );
int dplgsy_Tile_Async( double  bump,
                                 al4san_uplo_t            uplo,
                                 AL4SAN_desc_t           *A,
                                 unsigned long long int seed,
                                 AL4SAN_sequence_t    *sequence,
                                 AL4SAN_request_t     *request );

void pdplgsy( double bump, al4san_uplo_t uplo, AL4SAN_desc_t *A,
                    unsigned long long int seed,
                    AL4SAN_sequence_t *sequence, AL4SAN_request_t *request );
void gemm_cpu_func( AL4SAN_arg_list *al4san_arg);
void potrf_cpu_fun(AL4SAN_arg_list *al4san_arg);
void trsm_cpu_func(AL4SAN_arg_list *al4san_arg);
void syrk_cpu_func(AL4SAN_arg_list *al4san_arg);
void plgsy_cpu_fun(AL4SAN_arg_list *al4san_arg);

char runtime[20];
/* Integer parameters for step2 */
enum iparam_step2 {
    IPARAM_THRDNBR,        /* Number of cores                            */
    IPARAM_GPUS,        /* Number of gpus                            */
    IPARAM_N,              /* Number of columns of the matrix            */
    IPARAM_NRHS,           /* Number of RHS                              */
    IPARAM_NB,           /* Number of NB                              */
    IPARAM_P,           /* Number of P                              */
    IPARAM_Q,           /* Number of Q                              */
    /* End */
    IPARAM_SIZEOF
};

/* Specific routines used in step2.c main program */

/**
 * Initialize integer parameters
 */
static void init_iparam(int iparam[IPARAM_SIZEOF]){
    iparam[IPARAM_THRDNBR       ] = -1;
    iparam[IPARAM_GPUS          ] = -1;
    iparam[IPARAM_N             ] = 500;
    iparam[IPARAM_NRHS          ] = 1;
    iparam[IPARAM_NB            ] = 10;
    iparam[IPARAM_P            ] = 1;
    iparam[IPARAM_Q            ] = 1;

}

/**
 * Print how to use the program
 */
static void show_help(char *prog_name) {
    printf( "Usage:\n%s [options]\n\n", prog_name );
    printf( "Options are:\n"
            "  --help           Show this help\n"
            "\n"
            "  --n=X            dimension (N). (default: 500)\n"
            "  --nrhs=X         number of RHS. (default: 1)\n"
            "  --nb=X         number of RHS. (default: 10)\n"
            "\n"
            "  --threads=X      Number of CPU workers (default: _SC_NPROCESSORS_ONLN)\n"
            "\n");
}


static int startswith(const char *s, const char *prefix) {
    size_t n = strlen( prefix );
    if (strncmp( s, prefix, n ))
        return 0;
    return 1;
}

/**
 * Read arguments following step2 program call
 */
static void read_args(int argc, char *argv[], int *iparam){
    int i;
    for (i = 1; i < argc && argv[i]; ++i) {
        if ( startswith( argv[i], "--help") || startswith( argv[i], "-help") ||
             startswith( argv[i], "--h") || startswith( argv[i], "-h") ) {
            show_help( argv[0] );
            exit(0);
        } else if (startswith( argv[i], "--n=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_N]) );
        } else if (startswith( argv[i], "--nrhs=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_NRHS]) );
        } else if (startswith( argv[i], "--nb=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_NB]) );
        } else if (startswith( argv[i], "--threads=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_THRDNBR]) );
        } else if (startswith( argv[i], "--gpus=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_GPUS]) );
        }else if (startswith( argv[i], "--p=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_P]) );
        } else if (startswith( argv[i], "--q=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%d", &(iparam[IPARAM_Q]) );
        } else if (startswith( argv[i], "--runtime=" )) {
            sscanf( strchr( argv[i], '=' ) + 1, "%s", runtime);
        } else {
            fprintf( stderr, "Unknown option: %s\n", argv[i] );
        }
    }
}

/**
 * Print a header message to summarize main parameters
 */
static void print_header(char *prog_name, int * iparam) {
#if defined(CHAMELEON_SIMULATION)
    double    eps = 0.;
#else
    double    eps = LAPACKE_dlamch_work( 'e' );
#endif
  if(AL4SAN_My_Mpi_Rank()==0){

    printf( "#\n"
            "# AL4SAN %d.%d.%d, %s\n"
            "# Nb threads: %d\n"
            "# Nb gpus:    %d\n"
            "# Nb Prows:    %d\n"
            "# Nb Qcols:    %d\n"
            "# N:          %d\n"
            "# NB:         %d\n"
            "# eps:        %e\n"
            "# Activated runtime: %s\n"
            "#\n",
            AL4SAN_VERSION_MAJOR,
            AL4SAN_VERSION_MINOR,
            AL4SAN_VERSION_MICRO,
            prog_name,
            iparam[IPARAM_THRDNBR],
            iparam[IPARAM_GPUS],
            iparam[IPARAM_P],
            iparam[IPARAM_Q],
            iparam[IPARAM_N],
            iparam[IPARAM_NB],
            eps,
            runtime );

    printf( "#      M       N  K/NRHS      NB  seconds   Gflop/s\n");
    printf( "#%7d %7d %7d %7d ", iparam[IPARAM_N], iparam[IPARAM_N], iparam[IPARAM_NRHS], iparam[IPARAM_NB]);
    fflush( stdout );
}
    return;
}



#endif /* _step2_h_ */

