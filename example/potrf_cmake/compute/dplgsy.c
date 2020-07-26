/**
 *
 * @file dplgsy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon dplgsy wrappers
 *
 * @version 0.9.2
 * @comment This file is a copy of dplgsy.c,
 *          wich has been automatically generated
 *          from Plasma 2.5.0 for AL4SAN 0.9.2
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @author Rade Mathis
 * @author Florent Pruvost
 * @date 2014-11-16
 * @generated from /home/omairyrm/modified-al4san/chameleon/compute/zplgsy.c, normal z -> d, Sun Jun 14 16:04:55 2020
 *
 */

#include "../potrf.h"


/**
 ********************************************************************************
 *
 * @ingroup double_Tile
 *
 *  AL4SAN_dplgsy_Tile - Generate a random symmetric (positive definite if 'bump' is large enough) half-matrix by tiles.
 *  Tile equivalent of AL4SAN_dplgsy().
 *  Operates on matrices stored by tiles.
 *  All matrices are passed through descriptors.
 *  All dimensions are taken from the descriptors.
 *
 *******************************************************************************
 *
 * @param[in] bump
 *          The value to add to the diagonal to be sure
 *          to have a positive definite matrix.
 *
 * @param[in] uplo
 *          The half of the matrix that will be generated.
 *
 * @param[in] A
 *          On exit, The random symmetric matrix A generated.
 *
 * @param[in] seed
 *          The seed used in the random generation.
 *
 *******************************************************************************
 *
 * @retval AL4SAN_SUCCESS successful exit
 *
 *******************************************************************************
 *
 * @sa AL4SAN_dplgsy
 * @sa AL4SAN_dplgsy_Tile_Async
 * @sa AL4SAN_cplgsy_Tile
 * @sa AL4SAN_dplgsy_Tile
 * @sa AL4SAN_splgsy_Tile
 * @sa AL4SAN_dplgsy_Tile
 *
 */
int dplgsy_Tile( double bump, al4san_uplo_t uplo,
                           AL4SAN_desc_t *A,
                           unsigned long long int seed )
{
    AL4SAN_context_t *chamctxt;
    AL4SAN_sequence_t *sequence = NULL;
    AL4SAN_request_t request = AL4SAN_REQUEST_INITIALIZER;
    int status;

    chamctxt = al4san_context_self();
    if (chamctxt == NULL) {
        al4san_fatal_error("dplgsy_Tile", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
   sequence = AL4SAN_Sequence_Create();

    dplgsy_Tile_Async( bump, uplo, A, seed, sequence, &request );

    AL4SAN_Desc_Flush( A, sequence );


   AL4SAN_Sequence_Wait(sequence);
    status = sequence->status;

   AL4SAN_Sequence_Destroy( sequence );
   

    return status;
}

/**
 ********************************************************************************
 *
 * @ingroup double_Tile_Async
 *
 *  AL4SAN_dplgsy_Tile_Async - Generate a random symmetric (positive definite if 'bump' is large enough) half-matrix by tiles.
 *  Non-blocking equivalent of AL4SAN_dplgsy_Tile().
 *  May return before the computation is finished.
 *  Allows for pipelining of operations at runtime.
 *
 *******************************************************************************
 *
 * @param[in] sequence
 *          Identifies the sequence of function calls that this call belongs to
 *          (for completion checks and exception handling purposes).
 *
 * @param[out] request
 *          Identifies this function call (for exception handling purposes).
 *
 *******************************************************************************
 *
 * @sa AL4SAN_dplgsy
 * @sa AL4SAN_dplgsy_Tile
 * @sa AL4SAN_cplgsy_Tile_Async
 * @sa AL4SAN_dplgsy_Tile_Async
 * @sa AL4SAN_splgsy_Tile_Async
 * @sa AL4SAN_dplgsy_Tile_Async
 * @sa AL4SAN_dplgsy_Tile_Async
 *
 */
int dplgsy_Tile_Async( double  bump,
                                 al4san_uplo_t            uplo,
                                 AL4SAN_desc_t           *A,
                                 unsigned long long int seed,
                                 AL4SAN_sequence_t    *sequence,
                                 AL4SAN_request_t     *request )
{
    AL4SAN_context_t *al4sanctxt;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("AL4SAN_dplgsy_Tile", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        al4san_fatal_error("AL4SAN_dplgsy_Tile", "NULL sequence");
        return AL4SAN_ERR_UNALLOCATED;
    }
    if (request == NULL) {
        al4san_fatal_error("AL4SAN_dplgsy_Tile", "NULL request");
        return AL4SAN_ERR_UNALLOCATED;
    }
    /* Check sequence status */
    if (sequence->status == AL4SAN_SUCCESS) {
        request->status = AL4SAN_SUCCESS;
    }
    else {
        return al4san_request_fail(sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);
    }

    /* Check descriptors for correctness */
    if (al4san_desc_check(A) != AL4SAN_SUCCESS) {
        al4san_error("AL4SAN_dplgsy_Tile", "invalid descriptor");
        return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
    }
    /* Check input arguments */
    if (A->nb != A->mb) {
        al4san_error("AL4SAN_dplgsy_Tile", "only square tiles supported");
        return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
    }

    /* Quick return */
    if (al4san_min( A->m, A->n ) == 0)
        return AL4SAN_SUCCESS;

    pdplgsy( bump, uplo, A, seed, sequence, request );

    return AL4SAN_SUCCESS;
}

