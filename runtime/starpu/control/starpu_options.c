/**
 *
 * @file runtime_options.c
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
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2010-11-15
 *
 */
/**
 *
 * @brief AL4SAN StarPU options routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
**/
#include <stdio.h>
#include <stdlib.h>
#include "al4san_starpu.h"

void AL4SAN_Starpu_options_init( AL4SAN_option_t *option, AL4SAN_context_t *al4san,
                           AL4SAN_sequence_t *sequence, AL4SAN_request_t *request )
{
    option->sequence   = sequence;
    option->request    = request;
    option->profiling  = AL4SAN_PROFILING == AL4SAN_TRUE;
    option->parallel   = AL4SAN_PARALLEL == AL4SAN_TRUE;
    option->priority   = AL4SAN_PRIORITY_MIN;
    //option->nb         = AL4SAN_NB;
    option->ws_wsize   = 0;
    option->ws_hsize   = 0;
    option->ws_worker  = NULL;
    option->ws_host    = NULL;
    return;
}

void AL4SAN_Starpu_options_finalize( AL4SAN_option_t *option, AL4SAN_context_t *al4san )
{
    (void)option;
    (void)al4san;
    return;
}

int AL4SAN_Starpu_options_ws_alloc( AL4SAN_option_t *options, size_t worker_size, size_t host_size )
{
    int ret = 0;
    if ( worker_size > 0 ) {
        options->ws_wsize = worker_size;
        starpu_vector_data_register((starpu_data_handle_t*)(&(options->ws_worker)),
                                    -1, (uintptr_t)NULL,
                                    worker_size, sizeof(char));
    }
    if ( host_size > 0 ) {
        options->ws_hsize = host_size;
        ret = AL4SAN_Starpu_starpu_ws_alloc((AL4SAN_starpu_ws_t**)&(options->ws_host),
                                      host_size, AL4SAN_CUDA, AL4SAN_HOST_MEM);
    }
    return ret;
}

int AL4SAN_Starpu_options_ws_free( AL4SAN_option_t *options )
{
    int ret = 0;
    if ( options->ws_worker != NULL ) {
        starpu_data_unregister_submit((starpu_data_handle_t)(options->ws_worker));
        options->ws_worker = NULL;
    }
    if ( options->ws_host != NULL ) {
        starpu_task_wait_for_all();
        ret = AL4SAN_Starpu_starpu_ws_free( (AL4SAN_starpu_ws_t*)(options->ws_host) );
        options->ws_host = NULL;
    }
    return ret;
}

int AL4SAN_Starpu_Workspace_Create(AL4SAN_workspace_t *workspace, size_t lworkspace,
                            size_t dtyp)
{

}
int AL4SAN_Starpu_Workspace_Destroy(AL4SAN_workspace_t *workspace)
{

}


