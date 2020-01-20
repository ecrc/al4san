/**
 *
 * @file al4san_constants.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Al4san global constants
 *
 * @version 1.0.1
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2019-02-06
 *
 */
  /**
   *
   * @brief AL4SAN global constants
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.1
   * @author Rabab Alomairy
   * @date 2019-02-06
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
/**
 * @brief Matrix floating point arithmetic
 */
typedef enum al4san_flttype_e {
    Al4sanByte          = 0,
    Al4sanInteger       = 1,
    Al4sanRealFloat     = 2,
    Al4sanRealDouble    = 3,
    Al4sanComplexFloat  = 4,
    Al4sanComplexDouble = 5,
} al4san_flttype_t;

/**
 * @brief Matrix tile storage
 */
typedef enum al4san_storage_e {
    Al4sanCM   = 101,
    Al4sanRM   = 102,
    Al4sanCCRB = 103,
    Al4sanCRRB = 104,
    Al4sanRCRB = 105,
    Al4sanRRRB = 106,
} al4san_storage_t;

/**
 * @brief Transpostion
 */
typedef enum al4san_trans_e {
    Al4sanNoTrans   = 111, /**< Use A         */
    Al4sanTrans     = 112, /**< Use A^t       */
    Al4sanConjTrans = 113  /**< Use conj(A^t) */
} al4san_trans_t;

/**
 * @brief Upper/Lower part
 */
typedef enum al4san_uplo_e {
    Al4sanUpper      = 121, /**< Use lower triangle of A */
    Al4sanLower      = 122, /**< Use upper triangle of A */
    Al4sanUpperLower = 123  /**< Use the full A          */
} al4san_uplo_t;

/**
 * @brief Diagonal
 */
typedef enum al4san_diag_e {
    Al4sanNonUnit = 131, /**< Diagonal is non unitary */
    Al4sanUnit    = 132  /**< Diagonal is unitary     */
} al4san_diag_t;

/**
 * @brief Side of the operation
 */
typedef enum al4san_side_e {
    Al4sanLeft  = 141, /**< Apply operator on the left  */
    Al4sanRight = 142  /**< Apply operator on the right */
} al4san_side_t;

/**
 * @brief Norms
 */
typedef enum al4san_normtype_e {
    Al4sanOneNorm       = 171, /**< One norm:       max_j( sum_i( |a_{ij}| ) )   */
    /* Al4sanRealOneNorm   = 172, */
    /* Al4sanTwoNorm       = 173, */
    Al4sanFrobeniusNorm = 174, /**< Frobenius norm: sqrt( sum_{i,j} (a_{ij}^2) ) */
    Al4sanInfNorm       = 175, /**< Inifinite norm: max_i( sum_j( |a_{ij}| ) )   */
    /* Al4sanRealInfNorm   = 176, */
    Al4sanMaxNorm       = 177, /**< Inifinite norm: max_{i,j}( | a_{ij} | )      */
    /* Al4sanRealMaxNorm   = 178 */
} al4san_normtype_t;

/**
 * @brief Random distribution for matrix generator
 */
typedef enum al4san_dist_e {
    Al4sanDistUniform   = 201,
    Al4sanDistSymmetric = 202,
    Al4sanDistNormal    = 203,
} al4san_dist_t;

/**
 * @brief Eigen and singular values generator format
 */
#define Al4sanHermGeev        241
#define Al4sanHermPoev        242
#define Al4sanNonsymPosv      243
#define Al4sanSymPosv         244

#define Al4sanNoPacking       291
#define Al4sanPackSubdiag     292
#define Al4sanPackSupdiag     293
#define Al4sanPackColumn      294
#define Al4sanPackRow         295
#define Al4sanPackLowerBand   296
#define Al4sanPackUpeprBand   297
#define Al4sanPackAll         298

/**
 * @brief Singular/Eigen vector job description
 */
typedef enum al4san_job_e {
    Al4sanNoVec = 301,
    Al4sanVec   = 302,
    Al4sanIvec  = 303,
} al4san_job_t;

/**
 * @brief Algorithm Direction
 */
typedef enum al4san_dir_e {
    Al4sanDirForward  = 391, /**< Forward direction   */
    Al4sanDirBackward = 392, /**< Backward direction  */
} al4san_dir_t;

/**
 * @brief Direction of the main vectors as for the householder reflectors in QR/LQ factorizations.
 */
typedef enum al4san_store_e {
    Al4sanColumnwise = 401, /**< Column wise storage  */
    Al4sanRowwise    = 402, /**< Row wise storage     */
} al4san_store_t;


#define Al4sanTrd            1001
#define Al4sanBrd            1002

#define Al4sanW               501
#define Al4sanA2              502

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
#define AL4SAN_GEMM3M          9

/**
 *  AL4SAN constants - configuration parameters
 */
#define AL4SAN_CONCURRENCY       1
#define AL4SAN_TILE_SIZE         2
#define AL4SAN_INNER_BLOCK_SIZE  3
#define AL4SAN_HOUSEHOLDER_MODE  5
#define AL4SAN_HOUSEHOLDER_SIZE  6
#define AL4SAN_TRANSLATION_MODE  7

/**
 * @brief QR/LQ factorization trees
 */
typedef enum al4san_householder_e {
    Al4sanFlatHouseholder = 1,
    Al4sanTreeHouseholder = 2,
} al4san_householder_t;

/**
 * @brief Translation types
 */
typedef enum al4san_translation_e {
    Al4sanInPlace    = 1,
    Al4sanOutOfPlace = 2,
} al4san_translation_t;

/**
 * @brief Constant to describe how to initialize the mat pointer in descriptors
 */
#define AL4SAN_MAT_ALLOC_GLOBAL NULL
#define AL4SAN_MAT_ALLOC_TILE   ((void*)-1)
#define AL4SAN_MAT_OOC          ((void*)-2)

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

#define AL4SAN_Col_Major 132
#define AL4SAN_Row_Major 142

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

#endif /* _AL4SAN_CONSTANTS_H_ */
