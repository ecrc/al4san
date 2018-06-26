/**
 *
 * @file runtime_async.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU asynchronous routines
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#include <stdlib.h>
#include "altanal_runtime.h"

/**
 *  Create a sequence
 */
int ALTANAL_Runtime_sequence_create( ALTANAL_context_t  *altanal,
                             ALTANAL_sequence_t *sequence )
{
    (void)altanal;
    (void)sequence;
    return ALTANAL_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int ALTANAL_Runtime_sequence_destroy( ALTANAL_context_t  *altanal,
                              ALTANAL_sequence_t *sequence )
{
    (void)altanal;
    (void)sequence;
    return ALTANAL_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int ALTANAL_Runtime_sequence_wait( ALTANAL_context_t  *altanal,
                           ALTANAL_sequence_t *sequence )
{
    (void)altanal;
    (void)sequence;

    if (altanal->progress_enabled) {
        ALTANAL_Runtime_progress(altanal);
    }

    starpu_task_wait_for_all();
#if defined(ALTANAL_USE_MPI)
    starpu_mpi_barrier(MPI_COMM_WORLD);
#endif
    return ALTANAL_SUCCESS;
}

/**
 *  Terminate a sequence
 */
void ALTANAL_Runtime_sequence_flush( ALTANAL_context_t *altanal,
                             ALTANAL_sequence_t *sequence,
                             ALTANAL_request_t  *request,
                             int status )
{
    (void)altanal;
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    return;
}
