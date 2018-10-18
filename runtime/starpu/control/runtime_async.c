/**
 *
 * @file runtime_async.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 *
 * version 1.0.0
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2010-11-15
 *
 */
/**
 *
 * @brief AL4SAN StarPU asynchronous routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
**/
#include <stdlib.h>
#include "al4san_runtime.h"

/**
 *  Create a sequence
 */
int AL4SAN_Runtime_sequence_create( AL4SAN_context_t  *al4san,
                             AL4SAN_sequence_t *sequence )
{
    (void)al4san;
    (void)sequence;
    return AL4SAN_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int AL4SAN_Runtime_sequence_destroy( AL4SAN_context_t  *al4san,
                              AL4SAN_sequence_t *sequence )
{
    (void)al4san;
    (void)sequence;
    return AL4SAN_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int AL4SAN_Runtime_sequence_wait( AL4SAN_context_t  *al4san,
                           AL4SAN_sequence_t *sequence )
{
    (void)al4san;
    (void)sequence;

    if (al4san->progress_enabled) {
        AL4SAN_Runtime_progress(al4san);
    }

    starpu_task_wait_for_all();
#if defined(AL4SAN_USE_MPI)
    starpu_mpi_barrier(MPI_COMM_WORLD);
#endif
    return AL4SAN_SUCCESS;
}

/**
 *  Terminate a sequence
 */
void AL4SAN_Runtime_sequence_flush( AL4SAN_context_t *al4san,
                             AL4SAN_sequence_t *sequence,
                             AL4SAN_request_t  *request,
                             int status )
{
    (void)al4san;
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    return;
}
