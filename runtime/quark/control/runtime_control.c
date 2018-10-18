/**
 *
 * @file runtime_control.c
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
 * date 2012-09-15
 *
 */
/**
 *
 * @brief AL4SAN Quark control routines
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
#include "al4san_runtime.h"

/**
 *
 */
int AL4SAN_Runtime_init( AL4SAN_context_t *al4san,
                  int ncpus,
                  int ncudas,
                  int nthreads_per_worker )
{
    int hres = 0;
    if ( ncudas > 0 ) {
        al4san_warning( "AL4SAN_Runtime_init_scheduler(quark)", "GPUs are not supported for now");
    }

    if ( nthreads_per_worker > 0 ) {
        al4san_warning( "AL4SAN_Runtime_init_scheduler(quark)", "Multi-threaded kernels are not supported for now");
    }

    al4san->schedopt = (void*)QUARK_New( ncpus );

    return hres;
}

/**
 *
 */
void AL4SAN_Runtime_finalize( AL4SAN_context_t *al4san )
{
    QUARK_Delete((Quark*)(al4san->schedopt));
    return;
}

/**
 *  To suspend the processing of new tasks by workers
 */
void AL4SAN_Runtime_pause( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 *  This is the symmetrical call to AL4SAN_Runtime_pause,
 *  used to resume the workers polling for new tasks.
 */
void AL4SAN_Runtime_resume( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 *  Busy-waiting barrier
 */
void AL4SAN_Runtime_barrier( AL4SAN_context_t *al4san )
{
    QUARK_Barrier((Quark*)(al4san->schedopt));
}

/**
 *  Display a progress information when executing the tasks
 */
void AL4SAN_Runtime_progress( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 * Thread rank.
 */
int AL4SAN_Runtime_thread_rank( AL4SAN_context_t *al4san )
{
    return QUARK_Thread_Rank((Quark*)(al4san->schedopt));
}

/**
 * Number of threads.
 */
int AL4SAN_Runtime_thread_size( AL4SAN_context_t *al4san )
{
    (void)al4san;
    /*
     * TODO: should add a function to Quark to get the number of thread from the
     * data structure and not from the system function
     */
    return quark_get_numthreads();
}

/**
 *  The process rank
 */
int AL4SAN_Runtime_comm_rank( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return 0;
}

/**
 *  This returns the size of the distributed computation
 */
int AL4SAN_Runtime_comm_size( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return 1;
}


/**
 *  Flush cached data from runtime descriptor
 */
void AL4SAN_Runtime_flush()
{
 return;
}
