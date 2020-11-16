/**
 *
 * @file struct.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 *
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 */
/**
   *
   * @brief AL4SAN structures
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.1.0
   * @author Rabab Alomairy
   * @date 2019-02-06
   *
  **/
#ifndef _AL4SAN_STRUCT_H_
#define _AL4SAN_STRUCT_H_

#include "al4san/config.h"
#include "al4san/types.h"
#include "al4san/constants.h"
#if defined(AL4SAN_USE_MPI)
#include<mpi.h>
#endif
BEGIN_C_DECLS

/**
 * RUNTIME headers to include types of :
 *         - QUARK
 *         - PaRSEC
 *         - StarPU
 *         - OpenMP
 */

typedef enum al4san_sched_e {
  AL4SAN_RUNTIME_SCHED_QUARK = 0,
  AL4SAN_RUNTIME_SCHED_STARPU = 1,
  AL4SAN_RUNTIME_SCHED_PARSEC = 2,
  AL4SAN_RUNTIME_SCHED_OPENMP = 3,
} AL4SAN_sched_t;


typedef void *AL4SAN_codelet;
typedef void *AL4SAN_arg;

/**
 *  AL4SAN request uniquely identifies each asynchronous function call.
 */
typedef struct al4san_context_s {
    AL4SAN_sched_t      scheduler;
    AL4SAN_sched_t      starpu_schd;
    int                nworkers;
    int                ncudas;
    int                nthreads_per_worker;
#if defined(AL4SAN_USE_MPI)
    int                my_mpi_rank;
    int                mpi_comm_size;
    int                pcols;
    int                prows;
#endif
    int                world_size;
    int                group_size;

    /* Boolean flags */
    AL4SAN_bool         warnings_enabled;
    AL4SAN_bool         autotuning_enabled;
    AL4SAN_bool         parallel_enabled;
    AL4SAN_bool         profiling_enabled;
    AL4SAN_bool         progress_enabled;
    int householder;        // "domino" (flat) or tree-based (reduction) Householder
    int translation;        // In place or Out of place layout conversion

    int                nb;
    int                ib;
    void              *schedopt;           // structure for runtimes
    int                mpi_outer_init;     // MPI has been initialized outside our functions
#if defined(AL4SAN_USE_MPI)
    MPI_Comm           newcomm;
#endif
} AL4SAN_context_t;

/**
 *  Tile matrix descriptor
 *
 *  Matrices are stored in a contiguous data chunk containning in order
 *  A11, A21, A12, A22 with :
 *
 *           n1      n2
 *      +----------+---+
 *      |          |   |    With m1 = lm - (lm%mb)
 *      |          |   |         m2 = lm%mb
 *  m1  |    A11   |A12|         n1 = ln - (ln%nb)
 *      |          |   |         n2 = ln%nb
 *      |          |   |
 *      +----------+---+
 *  m2  |    A21   |A22|
 *      +----------+---+
 *
 */
struct al4san_desc_s;
typedef struct al4san_desc_s AL4SAN_desc_t;

struct al4san_desc_s {
    // function to get chameleon tiles address
    void *(*get_blkaddr)( const AL4SAN_desc_t*, int, int );
    // function to get chameleon tiles leading dimension
    int   (*get_blkldd )( const AL4SAN_desc_t*, int );
    // function to get chameleon tiles MPI rank
    int   (*get_rankof) ( const AL4SAN_desc_t*, int, int );
    void *mat;        // pointer to the beginning of the matrix
    size_t A21;       // pointer to the beginning of the matrix A21
    size_t A12;       // pointer to the beginning of the matrix A12
    size_t A22;       // pointer to the beginning of the matrix A22
    //cham_storage_t styp;  // storage layout of the matrix
    //cham_flttype_t dtyp;  // precision of the matrix
    unsigned int styp;
    unsigned int dtyp;
    int mb;           // number of rows in a tile
    int nb;           // number of columns in a tile
    int bsiz;         // size in elements including padding
    int lm;         // number of rows of the entire matrix
    int ln;           // number of columns of the entire matrix
    int lmt;          // number of tile rows of the entire matrix - derived parameter
    int lnt;          // number of tile columns of the entire matrix - derived parameter
    int i;            // row index to the beginning of the submatrix
    int j;            // column index to the beginning of the submatrix
    int m;            // number of rows of the submatrix
    int n;            // number of columns of the submatrix
    int mt;           // number of tile rows of the submatrix - derived parameter
    int nt;           // number of tile columns of the submatrix - derived parameter
                      // Data for distributed cases
    int p;            // number of rows of the 2D distribution grid
    int q;            // number of columns of the 2D distribution grid
    int llm;          // number of rows of the 2D distribution grid
    int lln;          // number of columns of the 2D distribution grid
    int llm1;         // number of tile rows of the A11 matrix - derived parameter
    int lln1;         // number of tile columns of the A11 matrix - derived parameter
    int llmt;         // number of tile rows of the local (to a node) matrix
    int llnt;         // number of tile columns of the local (to a node) matrix
    int id;           // identification number of the descriptor
    int occurences;   // identify main matrix desc (occurances=1) or
                      // submatrix desc (occurances>1) to avoid unregistering
                      // GPU data twice
    int use_mat;      // 1 if we have a pointer to the overall data mat - else 0
    int alloc_mat;    // 1 if we handle the allocation of mat - else 0
    int register_mat; // 1 if we have to register mat - else 0 (handled by the application)
    int myrank;       // MPI rank of the descriptor
    int ooc;          // 1 if the matrix is not to fit in memory
    void *schedopt;   // scheduler (QUARK|StarPU) specific structure
};

/**
 *  AL4SAN request uniquely identifies each asynchronous function call.
 */
typedef struct al4san_request_s {
    AL4SAN_enum status; // AL4SAN_SUCCESS or appropriate error code
} AL4SAN_request_t;


/**
 *  AL4SAN sequence uniquely identifies a set of asynchronous function calls
 *  sharing common exception handling.
 */
typedef struct al4san_sequence_s {
    AL4SAN_bool       status;    /* AL4SAN_SUCCESS or appropriate error code */
    AL4SAN_request_t *request;   /* failed request                          */
    void            *schedopt;
} AL4SAN_sequence_t;


/**
 *  AL4SAN options
 */
typedef struct al4san_option_s {
    AL4SAN_sequence_t *sequence;
    AL4SAN_request_t  *request;
    int               profiling;
    int               parallel;
    int               priority;
    size_t            ws_wsize;
    size_t            ws_hsize;
    void             *ws_worker;  /*> Workspace located on the worker        */
    void             *ws_host;    /*> Workspace *always* located on the host */
    void             *schedopt;
} AL4SAN_option_t;


/**
 *  AL4SAN Workspace
 */

typedef struct {
    void **spaces;      ///< array of nthread pointers to workspaces
    size_t lworkspace;  ///< length in elements of workspace on each core
    int nthread;        ///< number of threads
    size_t dtyp; ///< precision of the workspace
} AL4SAN_workspace_t;


END_C_DECLS

#endif /* __AL4SAN_H__ */
