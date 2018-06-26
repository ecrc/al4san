/**
 *
 * @file async.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon asynchronous management routines
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 ***
 *
 * @defgroup Sequences
 * @brief Group routines exposed to users to handle asynchronous tasks execution
 *
 */
#include <stdlib.h>
#include "control/common.h"
#include "altanal/altanal_runtime.h"

/**
 *  Register an exception.
 */
int altanal_request_fail(ALTANAL_sequence_t *sequence, ALTANAL_request_t *request, int status)
{
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    return status;
}

/**
 *  Create a sequence
 */
int altanal_sequence_create(ALTANAL_context_t *altanal, ALTANAL_sequence_t **sequence)
{
    if ((*sequence = malloc(sizeof(ALTANAL_sequence_t))) == NULL) {
        altanal_error("ALTANAL_Sequence_Create", "malloc() failed");
        return ALTANAL_ERR_OUT_OF_RESOURCES;
    }

    ALTANAL_Runtime_sequence_create( altanal, *sequence );

    (*sequence)->status = ALTANAL_SUCCESS;
    return ALTANAL_SUCCESS;
}

/**
 *  Destroy a sequence
 */
int altanal_sequence_destroy(ALTANAL_context_t *altanal, ALTANAL_sequence_t *sequence)
{
    ALTANAL_Runtime_sequence_destroy( altanal, sequence );
    free(sequence);
    return ALTANAL_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int altanal_sequence_wait(ALTANAL_context_t *altanal, ALTANAL_sequence_t *sequence)
{
    ALTANAL_Runtime_sequence_wait( altanal, sequence );
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Sequences
 *
 *  ALTANAL_Sequence_Create - Create a squence.
 *
 ******************************************************************************
 *
 * @param[out] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Sequence_Create(ALTANAL_sequence_t **sequence)
{
    ALTANAL_context_t *altanal;
    int status;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Create", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    status = altanal_sequence_create(altanal, sequence);
    return status;
}

/**
 *
 * @ingroup Sequences
 *
 *  ALTANAL_Sequence_Destroy - Destroy a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Sequence_Destroy(ALTANAL_sequence_t *sequence)
{
    ALTANAL_context_t *altanal;
    int status;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Destroy", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Destroy", "NULL sequence");
        return ALTANAL_ERR_UNALLOCATED;
    }
    status = altanal_sequence_destroy(altanal, sequence);
    return status;
}

/**
 *
 * @ingroup Sequences
 *
 *  ALTANAL_Sequence_Wait - Wait for the completion of a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Sequence_Wait(ALTANAL_sequence_t *sequence)
{
    ALTANAL_context_t *altanal;
    int status;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Wait", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Wait", "NULL sequence");
        return ALTANAL_ERR_UNALLOCATED;
    }
    status = altanal_sequence_wait(altanal, sequence);
    return status;
}

/**
 *
 * @ingroup Sequences
 *
 *  ALTANAL_Sequence_Flush - Terminate a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 * @param[in] request
 *          The flush request.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Sequence_Flush(ALTANAL_sequence_t *sequence, ALTANAL_request_t *request)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Flush", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        altanal_fatal_error("ALTANAL_Sequence_Flush", "NULL sequence");
        return ALTANAL_ERR_UNALLOCATED;
    }

    ALTANAL_Runtime_sequence_flush( altanal->schedopt, sequence, request, ALTANAL_ERR_SEQUENCE_FLUSHED);

    return ALTANAL_SUCCESS;
}
