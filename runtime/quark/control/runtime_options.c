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
 * @brief Chameleon Quark options routines
 *
 * @version 1.0.0
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2011-10-29
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "altanal_runtime.h"

void ALTANAL_Runtime_options_init( ALTANAL_option_t *options, ALTANAL_context_t *altanal,
                           ALTANAL_sequence_t *sequence, ALTANAL_request_t *request )
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
    options->profiling  = ALTANAL_PROFILING == ALTANAL_TRUE;
    options->parallel   = ALTANAL_PARALLEL == ALTANAL_TRUE;
    options->priority   = ALTANAL_PRIORITY_MIN;

//    options->nb         = ALTANAL_NB;

    options->ws_wsize   = 0;
    options->ws_hsize   = 0;
    options->ws_worker  = NULL;
    options->ws_host    = NULL;

    /* quark in options */
    qopt->quark = (Quark*)(altanal->schedopt);
    options->schedopt = qopt;

    QUARK_Task_Flag_Set((Quark_Task_Flags*)qopt, TASK_SEQUENCE, (intptr_t)(sequence->schedopt));

    return;
}

void ALTANAL_Runtime_options_finalize( ALTANAL_option_t *options, ALTANAL_context_t *altanal )
{
    /* we can free the task_flags without waiting for quark
       because they should have been copied for every task */
    (void)altanal;
    free( options->schedopt );
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
