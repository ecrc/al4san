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
 * author Vijay Joshi
 * author Cedric Castagnede
 * date 2011-10-29
 *
 */
/**
 *
 * @brief AL4SAN Quark  options routines
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
#include "al4san_quark.h"

void AL4SAN_Quark_options_init( AL4SAN_option_t *options, AL4SAN_context_t *al4san,
                           AL4SAN_sequence_t *sequence, AL4SAN_request_t *request )
{
    /* Create the task flag */
    runtime_internal_t *qopt = (runtime_internal_t*) malloc(sizeof(runtime_internal_t));

    /* Initialize task_flags */
    memset( qopt, 0, sizeof(Quark_Task_Flags) );
    qopt->flags.task_lock_to_thread = -1;
    qopt->flags.task_thread_count   =  1;
    qopt->flags.thread_set_to_manual_scheduling = -1;
    /* Initialize options */
    options->sequence   = sequence;
    options->request    = request;
    options->profiling  = AL4SAN_PROFILING == AL4SAN_TRUE;
    options->parallel   = AL4SAN_PARALLEL == AL4SAN_TRUE;
    options->priority   = AL4SAN_PRIORITY_MIN;

    options->ws_wsize   = 0;
    options->ws_hsize   = 0;
    options->ws_worker  = NULL;
    options->ws_host    = NULL;
    /* quark in options */

    qopt->quark = (Quark*)(al4san->schedopt);
    options->schedopt = qopt;
    QUARK_Task_Flag_Set((Quark_Task_Flags*)qopt, TASK_SEQUENCE, (intptr_t)(sequence->schedopt));

    return;
}

void AL4SAN_Quark_options_finalize( AL4SAN_option_t *options, AL4SAN_context_t *al4san )
{
    /* we can free the task_flags without waiting for quark
       because they should have been copied for every task */
    (void)al4san;
    free( options->schedopt );
    return;
}

int AL4SAN_Quark_options_ws_alloc( AL4SAN_option_t *options, size_t worker_size, size_t host_size )
{
    options->ws_wsize = worker_size;
    options->ws_hsize = host_size;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Quark_options_ws_free( AL4SAN_option_t *options )
{
    options->ws_wsize = 0;
    options->ws_hsize = 0;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Workspace_Create(AL4SAN_workspace_t *workspace, size_t lworkspace,
                            size_t dtyp)
{
}
int AL4SAN_Workspace_Destroy(AL4SAN_workspace_t *workspace)
{
}
