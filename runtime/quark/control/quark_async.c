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
 ***
 *
 * @author Jakub Kurzak
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2011-10-29
 *
 *
 *
 * @brief AL4SAN Quark Task asynchronous routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#include <stdlib.h>
#include "al4san_quark.h"

/**
 *  Create a sequence
 */
int AL4SAN_Quark_sequence_create( AL4SAN_context_t  *al4san,
                             AL4SAN_sequence_t *sequence )
{
    sequence->schedopt = (void*)QUARK_Sequence_Create((Quark*)(al4san->schedopt));

    if (sequence->schedopt == NULL) {
        al4san_error("AL4SAN_Sequence_Create", "QUARK_Sequence_Create() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }
    sequence->status = AL4SAN_SUCCESS;
    return AL4SAN_SUCCESS;
} 


/**
 *  Destroy a sequence
 */
int AL4SAN_Quark_sequence_destroy( AL4SAN_context_t  *al4san,
                              AL4SAN_sequence_t *sequence )
{
    QUARK_Sequence_Destroy( (Quark*)(al4san->schedopt),
                            (Quark_Sequence *)(sequence->schedopt) );
    return AL4SAN_SUCCESS;
}

/**
 *  Wait for the completion of a sequence
 */
int AL4SAN_Quark_sequence_wait( AL4SAN_context_t  *al4san,
                           AL4SAN_sequence_t *sequence )
{
    QUARK_Sequence_Wait( (Quark*)(al4san->schedopt),
                         (Quark_Sequence *)(sequence->schedopt) );
    return AL4SAN_SUCCESS;
}

/**
 *  Terminate a sequence
 */
void AL4SAN_Quark_sequence_flush( AL4SAN_context_t *al4san,
                             AL4SAN_sequence_t *sequence,
                             AL4SAN_request_t  *request,
                             int status )
{
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    QUARK_Sequence_Cancel( (Quark*)(al4san),
                           (Quark_Sequence *)(sequence->schedopt) );
}
