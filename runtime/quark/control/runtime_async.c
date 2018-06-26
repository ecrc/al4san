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
 * @brief Chameleon Quark asynchronous routines
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2011-10-29
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
    sequence->schedopt = (void*)QUARK_Sequence_Create((Quark*)(altanal->schedopt));

    if (sequence->schedopt == NULL) {
        altanal_error("ALTANAL_Sequence_Create", "QUARK_Sequence_Create() failed");
        return ALTANAL_ERR_OUT_OF_RESOURCES;
    }
    sequence->status = ALTANAL_SUCCESS;
    return ALTANAL_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int ALTANAL_Runtime_sequence_destroy( ALTANAL_context_t  *altanal,
                              ALTANAL_sequence_t *sequence )
{
    QUARK_Sequence_Destroy( (Quark*)(altanal->schedopt),
                            (Quark_Sequence *)(sequence->schedopt) );
    return ALTANAL_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int ALTANAL_Runtime_sequence_wait( ALTANAL_context_t  *altanal,
                           ALTANAL_sequence_t *sequence )
{
    QUARK_Sequence_Wait( (Quark*)(altanal->schedopt),
                         (Quark_Sequence *)(sequence->schedopt) );
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
    QUARK_Sequence_Cancel( (Quark*)(altanal),
                           (Quark_Sequence *)(sequence->schedopt) );
}
