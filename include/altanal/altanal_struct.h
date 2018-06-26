/**
 *
 * @file altanal_struct.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon structures
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 */
#ifndef _ALTANAL_STRUCT_H_
#define _ALTANAL_STRUCT_H_

#include "altanal/altanal_config.h"
#include "altanal/altanal_types.h"
#include "altanal/altanal_kernels.h"

BEGIN_C_DECLS

/**
 * RUNTIME headers to include types of :
 *         - QUARK
 *         - PaRSEC
 *         - StarPU
 */
typedef enum altanal_sched_e {
  ALTANAL_RUNTIME_SCHED_QUARK,
  ALTANAL_RUNTIME_SCHED_PARSEC,
  ALTANAL_RUNTIME_SCHED_STARPU,
} ALTANAL_sched_t;


typedef void *ALTANAL_codelet;
typedef void *ALTANAL_arg;

/**
 *  ALTANAL request uniquely identifies each asynchronous function call.
 */
typedef struct altanal_context_s {
    ALTANAL_sched_t      scheduler;
    int                nworkers;
    int                ncudas;
    int                nthreads_per_worker;
#if defined(ALTANAL_USE_MPI)
    int                my_mpi_rank;
    int                mpi_comm_size;
#endif
    int                world_size;
    int                group_size;

    /* Boolean flags */
    ALTANAL_bool         warnings_enabled;
    ALTANAL_bool         autotuning_enabled;
    ALTANAL_bool         parallel_enabled;
    ALTANAL_bool         profiling_enabled;
    ALTANAL_bool         progress_enabled;

    void              *schedopt;           // structure for runtimes
    int                mpi_outer_init;     // MPI has been initialized outside our functions
} ALTANAL_context_t;


/**
 *  ALTANAL request uniquely identifies each asynchronous function call.
 */
typedef struct altanal_request_s {
    ALTANAL_enum status; // ALTANAL_SUCCESS or appropriate error code
} ALTANAL_request_t;


/**
 *  ALTANAL sequence uniquely identifies a set of asynchronous function calls
 *  sharing common exception handling.
 */
typedef struct altanal_sequence_s {
    ALTANAL_bool       status;    /* ALTANAL_SUCCESS or appropriate error code */
    ALTANAL_request_t *request;   /* failed request                          */
    void            *schedopt;
} ALTANAL_sequence_t;


/**
 *  ALTANAL options
 */
typedef struct altanal_option_s {
    ALTANAL_sequence_t *sequence;
    ALTANAL_request_t  *request;
    int               profiling;
    int               parallel;
    int               priority;
    int               nb;
    size_t            ws_wsize;
    size_t            ws_hsize;
    void             *ws_worker;  /*> Workspace located on the worker        */
    void             *ws_host;    /*> Workspace *always* located on the host */
    void             *schedopt;
} ALTANAL_option_t;

END_C_DECLS

#endif /* __ALTANAL_H__ */
