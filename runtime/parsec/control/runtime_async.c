/**
 *
 * @file runtime_async.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC asynchronous routines
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
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
    parsec_context_t  *parsec        = (parsec_context_t *)(altanal->schedopt);
    parsec_taskpool_t *parsec_dtd_tp = parsec_dtd_taskpool_new();

    parsec_enqueue( parsec, (parsec_taskpool_t *)parsec_dtd_tp );
    sequence->schedopt = parsec_dtd_tp;

    parsec_context_start(parsec);

    return ALTANAL_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int ALTANAL_Runtime_sequence_destroy( ALTANAL_context_t  *altanal,
                              ALTANAL_sequence_t *sequence )
{
    parsec_context_t  *parsec = (parsec_context_t *)(altanal->schedopt);
    parsec_taskpool_t *parsec_dtd_tp = (parsec_taskpool_t *)(sequence->schedopt);

    assert( parsec_dtd_tp );
    parsec_dtd_taskpool_wait( parsec, parsec_dtd_tp );
    parsec_taskpool_free( parsec_dtd_tp );

    sequence->schedopt = NULL;
    return ALTANAL_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int ALTANAL_Runtime_sequence_wait( ALTANAL_context_t  *altanal,
                           ALTANAL_sequence_t *sequence )
{
    parsec_context_t  *parsec = (parsec_context_t *)altanal->schedopt;
    parsec_taskpool_t *parsec_dtd_tp = (parsec_taskpool_t *) sequence->schedopt;

    assert( parsec_dtd_tp );
    parsec_dtd_taskpool_wait( parsec, parsec_dtd_tp );

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
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    (void)altanal;
    return;
}
