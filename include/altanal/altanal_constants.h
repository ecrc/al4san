/**
 *
 * @file altanal_constants.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon global constants
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 */
#ifndef _ALTANAL_CONSTANTS_H_
#define _ALTANAL_CONSTANTS_H_

/**
 *
 * @brief Altanal constants - CBLAS & LAPACK
 *  The naming and numbering is consistent with:
 *
 *    1) CBLAS from Netlib (http://www.netlib.org/blas/blast-forum/cblas.tgz),
 *    2) C Interface to LAPACK from Netlib (http://www.netlib.org/lapack/lapwrapc/).
 *
 */
#define AltanalByte              0
#define AltanalInteger           1
#define AltanalRealFloat         2
#define AltanalRealDouble        3
#define AltanalComplexFloat      4
#define AltanalComplexDouble     5

/**
 *  ALTANAL constants - boolean
 */
#define ALTANAL_FALSE  0
#define ALTANAL_TRUE   1

#define ALTANAL_CPU    ((1ULL)<<1)
#define ALTANAL_CUDA   ((1ULL)<<3)

/**
 *  State machine switches
 */
#define ALTANAL_WARNINGS        1
#define ALTANAL_ERRORS          2
#define ALTANAL_AUTOTUNING      3
#define ALTANAL_DAG             4
#define ALTANAL_PROFILING_MODE  5
#define ALTANAL_PARALLEL_MODE   6
#define ALTANAL_BOUND           7
#define ALTANAL_PROGRESS        8
//#define ALTANAL_GEMM3M          9

/**
 *  ALTANAL constants - configuration parameters
 */
#define ALTANAL_CONCURRENCY       1


/**
 *  ALTANAL constants - success & error codes
 */
#define ALTANAL_SUCCESS                 0
#define ALTANAL_ERR_NOT_INITIALIZED  -101
#define ALTANAL_ERR_REINITIALIZED    -102
#define ALTANAL_ERR_NOT_SUPPORTED    -103
#define ALTANAL_ERR_ILLEGAL_VALUE    -104
#define ALTANAL_ERR_NOT_FOUND        -105
#define ALTANAL_ERR_OUT_OF_RESOURCES -106
#define ALTANAL_ERR_INTERNAL_LIMIT   -107
#define ALTANAL_ERR_UNALLOCATED      -108
#define ALTANAL_ERR_FILESYSTEM       -109
#define ALTANAL_ERR_UNEXPECTED       -110
#define ALTANAL_ERR_SEQUENCE_FLUSHED -111

/**
 * Kernels options
 */
#define ALTANAL_PRIORITY_MIN  0
#define ALTANAL_PRIORITY_MAX  INT_MAX


/**
 *  Scheduler properties
 */
#define PRIORITY        16
#define CALLBACK        17
#define REDUX           18

/**
 *  ALTANAL ???
 */
#define ALTANAL_REQUEST_INITIALIZER {ALTANAL_SUCCESS}

#endif
