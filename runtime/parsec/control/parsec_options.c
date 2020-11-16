/**
 *
 * @file runtime_options.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 * @brief AL4SAN PaRSEC options routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * author Reazul Hoque
 * author Mathieu Faverge
 * date 2017-01-12
  * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 */
#include <stdio.h>
#include <stdlib.h>
#include "al4san_parsec.h"

void AL4SAN_Parsec_options_init( AL4SAN_option_t *options, AL4SAN_context_t *al4san,
                           AL4SAN_sequence_t *sequence, AL4SAN_request_t *request )
{
    options->sequence   = sequence;
    options->request    = request;
    options->profiling  = AL4SAN_PROFILING == AL4SAN_TRUE;
    options->parallel   = AL4SAN_PARALLEL == AL4SAN_TRUE;
    options->priority   = AL4SAN_PRIORITY_MIN;
//    options->nb         = AL4SAN_NB;
    options->ws_wsize   = 0;
    options->ws_hsize   = 0;
    options->ws_worker  = NULL;
    options->ws_host    = NULL;
    return;
}

void AL4SAN_Parsec_options_finalize( AL4SAN_option_t *options, AL4SAN_context_t *al4san )
{
    (void)options;
    (void)al4san;
    return;
}

int AL4SAN_Parsec_options_ws_alloc( AL4SAN_option_t *options, size_t worker_size, size_t host_size )
{
    options->ws_wsize = worker_size;
    options->ws_hsize = host_size;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Parsec_options_ws_free( AL4SAN_option_t *options )
{
    options->ws_wsize = 0;
    options->ws_hsize = 0;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Parsec_Workspace_Create(AL4SAN_workspace_t *workspace, size_t lworkspace,
                            size_t dtyp)
{
}
int AL4SAN_Parsec_Workspace_Destroy(AL4SAN_workspace_t *workspace)
{
}

