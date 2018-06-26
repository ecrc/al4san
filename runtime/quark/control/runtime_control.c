/**
 *
 * @file runtime_control.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon Quark control routines
 *
 * @version 1.0.0
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "altanal_runtime.h"

/**
 *
 */
int ALTANAL_Runtime_init( ALTANAL_context_t *altanal,
                  int ncpus,
                  int ncudas,
                  int nthreads_per_worker )
{
    int hres = 0;
    if ( ncudas > 0 ) {
        altanal_warning( "ALTANAL_Runtime_init_scheduler(quark)", "GPUs are not supported for now");
    }

    if ( nthreads_per_worker > 0 ) {
        altanal_warning( "ALTANAL_Runtime_init_scheduler(quark)", "Multi-threaded kernels are not supported for now");
    }

    altanal->schedopt = (void*)QUARK_New( ncpus );

    return hres;
}

/**
 *
 */
void ALTANAL_Runtime_finalize( ALTANAL_context_t *altanal )
{
    QUARK_Delete((Quark*)(altanal->schedopt));
    return;
}

/**
 *  To suspend the processing of new tasks by workers
 */
void ALTANAL_Runtime_pause( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return;
}

/**
 *  This is the symmetrical call to ALTANAL_Runtime_pause,
 *  used to resume the workers polling for new tasks.
 */
void ALTANAL_Runtime_resume( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return;
}

/**
 *  Busy-waiting barrier
 */
void ALTANAL_Runtime_barrier( ALTANAL_context_t *altanal )
{
    QUARK_Barrier((Quark*)(altanal->schedopt));
}

/**
 *  Display a progress information when executing the tasks
 */
void ALTANAL_Runtime_progress( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return;
}

/**
 * Thread rank.
 */
int ALTANAL_Runtime_thread_rank( ALTANAL_context_t *altanal )
{
    return QUARK_Thread_Rank((Quark*)(altanal->schedopt));
}

/**
 * Number of threads.
 */
int ALTANAL_Runtime_thread_size( ALTANAL_context_t *altanal )
{
    (void)altanal;
    /*
     * TODO: should add a function to Quark to get the number of thread from the
     * data structure and not from the system function
     */
    return quark_get_numthreads();
}

/**
 *  The process rank
 */
int ALTANAL_Runtime_comm_rank( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return 0;
}

/**
 *  This returns the size of the distributed computation
 */
int ALTANAL_Runtime_comm_size( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return 1;
}


/**
 *  Flush cached data from runtime descriptor
 */
void ALTANAL_Runtime_flush()
{
 return;
}