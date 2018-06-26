/**
 *
 * @file runtime_options.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU options routines
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "altanal_runtime.h"

void ALTANAL_Runtime_options_init( ALTANAL_option_t *option, ALTANAL_context_t *altanal,
                           ALTANAL_sequence_t *sequence, ALTANAL_request_t *request )
{
    option->sequence   = sequence;
    option->request    = request;
    option->profiling  = ALTANAL_PROFILING == ALTANAL_TRUE;
    option->parallel   = ALTANAL_PARALLEL == ALTANAL_TRUE;
    option->priority   = ALTANAL_PRIORITY_MIN;
    //option->nb         = ALTANAL_NB;
    option->ws_wsize   = 0;
    option->ws_hsize   = 0;
    option->ws_worker  = NULL;
    option->ws_host    = NULL;
    return;
}

void ALTANAL_Runtime_options_finalize( ALTANAL_option_t *option, ALTANAL_context_t *altanal )
{
    (void)option;
    (void)altanal;
    return;
}

int ALTANAL_Runtime_options_ws_alloc( ALTANAL_option_t *options, size_t worker_size, size_t host_size )
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
        ret = ALTANAL_Runtime_starpu_ws_alloc((ALTANAL_starpu_ws_t**)&(options->ws_host),
                                      host_size, ALTANAL_CUDA, ALTANAL_HOST_MEM);
    }
    return ret;
}

int ALTANAL_Runtime_options_ws_free( ALTANAL_option_t *options )
{
    int ret = 0;
    if ( options->ws_worker != NULL ) {
        starpu_data_unregister_submit((starpu_data_handle_t)(options->ws_worker));
        options->ws_worker = NULL;
    }
    if ( options->ws_host != NULL ) {
        starpu_task_wait_for_all();
        ret = ALTANAL_Runtime_starpu_ws_free( (ALTANAL_starpu_ws_t*)(options->ws_host) );
        options->ws_host = NULL;
    }
    return ret;
}
