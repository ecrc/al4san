/**
 *
 * @file constants.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 */
  /**
   *
   * @brief AL4SAN global constants
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
   **/
#ifndef _AL4SAN_CONSTANTS_H_
#define _AL4SAN_CONSTANTS_H_

/**
 *
 * @brief Al4san constants - CBLAS & LAPACK
 *  The naming and numbering is consistent with:
 *
 *    1) CBLAS from Netlib (http://www.netlib.org/blas/blast-forum/cblas.tgz),
 *    2) C Interface to LAPACK from Netlib (http://www.netlib.org/lapack/lapwrapc/).
 *
 */
#define Al4sanByte              0
#define Al4sanInteger           1
#define Al4sanRealFloat         2
#define Al4sanRealDouble        3
#define Al4sanComplexFloat      4
#define Al4sanComplexDouble     5

/**
 *  AL4SAN constants - boolean
 */
#define AL4SAN_FALSE  0
#define AL4SAN_TRUE   1

#define AL4SAN_CPU    ((1ULL)<<1)
#define AL4SAN_CUDA   ((1ULL)<<3)

/**
 *  State machine switches
 */
#define AL4SAN_WARNINGS        1
#define AL4SAN_ERRORS          2
#define AL4SAN_AUTOTUNING      3
#define AL4SAN_DAG             4
#define AL4SAN_PROFILING_MODE  5
#define AL4SAN_PARALLEL_MODE   6
#define AL4SAN_BOUND           7
#define AL4SAN_PROGRESS        8
//#define AL4SAN_GEMM3M          9

/**
 *  AL4SAN constants - configuration parameters
 */
#define AL4SAN_CONCURRENCY       1


/**
 *  AL4SAN constants - success & error codes
 */
#define AL4SAN_SUCCESS                 0
#define AL4SAN_ERR_NOT_INITIALIZED  -101
#define AL4SAN_ERR_REINITIALIZED    -102
#define AL4SAN_ERR_NOT_SUPPORTED    -103
#define AL4SAN_ERR_ILLEGAL_VALUE    -104
#define AL4SAN_ERR_NOT_FOUND        -105
#define AL4SAN_ERR_OUT_OF_RESOURCES -106
#define AL4SAN_ERR_INTERNAL_LIMIT   -107
#define AL4SAN_ERR_UNALLOCATED      -108
#define AL4SAN_ERR_FILESYSTEM       -109
#define AL4SAN_ERR_UNEXPECTED       -110
#define AL4SAN_ERR_SEQUENCE_FLUSHED -111

/**
 * Kernels options
 */
#define AL4SAN_PRIORITY_MIN  0
#define AL4SAN_PRIORITY_MAX  INT_MAX


/**
 *  Scheduler properties
 */
#define PRIORITY        16
#define CALLBACK        17
#define REDUX           18

/**
 *  AL4SAN 
 */
#define AL4SAN_REQUEST_INITIALIZER {AL4SAN_SUCCESS}

#endif
