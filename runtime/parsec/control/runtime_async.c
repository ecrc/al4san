/**
 *
 * @file runtime_async.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * author Reazul Hoque
 * author Mathieu Faverge
 * date 2017-01-12
 *
 */
/**
 *
 * @brief AL4SAN PaRSEC asynchronous routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
**/ 
#include <stdlib.h>
#include "al4san_runtime.h"

/**
 *  Create a sequence
 */
int AL4SAN_Runtime_sequence_create( AL4SAN_context_t  *al4san,
                             AL4SAN_sequence_t *sequence )
{
    parsec_context_t  *parsec        = (parsec_context_t *)(al4san->schedopt);
    parsec_taskpool_t *parsec_dtd_tp = parsec_dtd_taskpool_new();

    parsec_enqueue( parsec, (parsec_taskpool_t *)parsec_dtd_tp );
    sequence->schedopt = parsec_dtd_tp;

    parsec_context_start(parsec);

    return AL4SAN_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int AL4SAN_Runtime_sequence_destroy( AL4SAN_context_t  *al4san,
                              AL4SAN_sequence_t *sequence )
{
    parsec_context_t  *parsec = (parsec_context_t *)(al4san->schedopt);
    parsec_taskpool_t *parsec_dtd_tp = (parsec_taskpool_t *)(sequence->schedopt);

    assert( parsec_dtd_tp );
    parsec_dtd_taskpool_wait( parsec, parsec_dtd_tp );
    parsec_taskpool_free( parsec_dtd_tp );

    sequence->schedopt = NULL;
    return AL4SAN_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int AL4SAN_Runtime_sequence_wait( AL4SAN_context_t  *al4san,
                           AL4SAN_sequence_t *sequence )
{
    parsec_context_t  *parsec = (parsec_context_t *)al4san->schedopt;
    parsec_taskpool_t *parsec_dtd_tp = (parsec_taskpool_t *) sequence->schedopt;

    assert( parsec_dtd_tp );
    parsec_dtd_taskpool_wait( parsec, parsec_dtd_tp );

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
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    (void)al4san;
    return;
}