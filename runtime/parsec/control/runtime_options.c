/**
 *
 * @file runtime_options.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC options routines
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "altanal_runtime.h"

void ALTANAL_Runtime_options_init( ALTANAL_option_t *options, ALTANAL_context_t *altanal,
                           ALTANAL_sequence_t *sequence, ALTANAL_request_t *request )
{
    options->sequence   = sequence;
    options->request    = request;
    options->profiling  = ALTANAL_PROFILING == ALTANAL_TRUE;
    options->parallel   = ALTANAL_PARALLEL == ALTANAL_TRUE;
    options->priority   = ALTANAL_PRIORITY_MIN;
//    options->nb         = ALTANAL_NB;
    options->ws_wsize   = 0;
    options->ws_hsize   = 0;
    options->ws_worker  = NULL;
    options->ws_host    = NULL;
    return;
}

void ALTANAL_Runtime_options_finalize( ALTANAL_option_t *options, ALTANAL_context_t *altanal )
{
    (void)options;
    (void)altanal;
    return;
}

int ALTANAL_Runtime_options_ws_alloc( ALTANAL_option_t *options, size_t worker_size, size_t host_size )
{
    options->ws_wsize = worker_size;
    options->ws_hsize = host_size;
    return ALTANAL_SUCCESS;
}

int ALTANAL_Runtime_options_ws_free( ALTANAL_option_t *options )
{
    options->ws_wsize = 0;
    options->ws_hsize = 0;
    return ALTANAL_SUCCESS;
}
