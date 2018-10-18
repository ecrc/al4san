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
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
  **/
#ifndef _AL4SAN_STRUCT_H_
#define _AL4SAN_STRUCT_H_

#include "al4san/config.h"
#include "al4san/types.h"

BEGIN_C_DECLS

/**
 * RUNTIME headers to include types of :
 *         - QUARK
 *         - PaRSEC
 *         - StarPU
 *         - OpenMP
 */

typedef enum al4san_sched_e {
  AL4SAN_RUNTIME_SCHED_QUARK,
  AL4SAN_RUNTIME_SCHED_PARSEC,
  AL4SAN_RUNTIME_SCHED_STARPU,
  AL4SAN_RUNTIME_SCHED_OPENMP
} AL4SAN_sched_t;


typedef void *AL4SAN_codelet;
typedef void *AL4SAN_arg;

/**
 *  AL4SAN request uniquely identifies each asynchronous function call.
 */
typedef struct al4san_context_s {
    AL4SAN_sched_t      scheduler;
    int                nworkers;
    int                ncudas;
    int                nthreads_per_worker;
#if defined(AL4SAN_USE_MPI)
    int                my_mpi_rank;
    int                mpi_comm_size;
#endif
    int                world_size;
    int                group_size;

    /* Boolean flags */
    AL4SAN_bool         warnings_enabled;
    AL4SAN_bool         autotuning_enabled;
    AL4SAN_bool         parallel_enabled;
    AL4SAN_bool         profiling_enabled;
    AL4SAN_bool         progress_enabled;

    void              *schedopt;           // structure for runtimes
    int                mpi_outer_init;     // MPI has been initialized outside our functions
} AL4SAN_context_t;


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
