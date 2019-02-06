/**
 *
 * @file task.c
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief AL4SAN Global Options routines
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "control/al4san_auxiliary.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Options_Init - initialization of global options.
 *
 ******************************************************************************
 *
 * @param[in, out] options
 *          options argument which holds sequence data sturcture.
 *
 * @param[in, out] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] request
 *          status
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Options_Init(AL4SAN_option_t *options, AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
	AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Options_Init", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
	AL4SAN_Runtime_options_init( options, al4san, sequence, request );

	return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Options_Finalize - finalize the global options.
 *
 ******************************************************************************
 *
 * @param[in, out] options
 *          options argument which holds sequence data sturcture.
 *
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Options_Finalize(AL4SAN_option_t *options)
{
	AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Options_Finalize", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
	AL4SAN_Runtime_options_finalize( options, al4san);

	return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Options_Workspace_Alloc - Workspace Allocation.
 *
 ******************************************************************************
 *
 * @param[in, out] options
 *          options argument which holds sequence data sturcture.
 *
 * @param[in] worker_size
 *            The the size of the worker.
 *
 * @param[in] host_size
 *            The the size of the host.
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Options_Workspace_Alloc( AL4SAN_option_t *options, size_t worker_size, size_t host_size )
{
    options->ws_wsize = worker_size;
    options->ws_hsize = host_size;
    AL4SAN_Runtime_options_ws_alloc( options, worker_size, host_size );
    return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Options_Workspace_Free - Workspace Dellocation.
 *
 ******************************************************************************
 *
 * @param[in, out] options
 *          options argument which holds sequence data sturcture.
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Options_Workspace_Free(AL4SAN_option_t *options )
{
	AL4SAN_Runtime_options_ws_free(options);
    return AL4SAN_SUCCESS;
}