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
 * author Mathieu Faverge
 * author Cedric Augonnet
 * author Cedric Castagnede
 * date 2010-11-15
 *
 */
/**
 *
 * @brief AL4SAN StarPU  control routines
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
    starpu_conf_t *conf = (starpu_conf_t*)(al4san->schedopt);
    int hres = -1;

    /* StarPU was already initialized by an external library */
    if (conf == NULL) {
        return 0;
    }

    conf->ncpus = ncpus;
    conf->ncuda = ncudas;
    conf->nopencl = 0;

    /* By default, use the dmdas strategy */
    if (!getenv("STARPU_SCHED")) {
        if (conf->ncuda > 0) {
            conf->sched_policy_name = "dmdas";
        }
        else {
            /**
             * Set scheduling to "ws"/"lws" if no cuda devices used because it
             * behaves better on homogneneous architectures. If the user wants
             * to use another scheduling strategy, he can set STARPU_SCHED
             * env. var. to whatever he wants
             */
#if (STARPU_MAJOR_VERSION > 1) || ((STARPU_MAJOR_VERSION == 1) && (STARPU_MINOR_VERSION >= 2))
            conf->sched_policy_name = "lws";
#else
            conf->sched_policy_name = "ws";
#endif
        }
    }

    if ((ncpus == -1)||(nthreads_per_worker == -1))
    {
        al4san->parallel_enabled = AL4SAN_FALSE;

        hres = starpu_init( conf );
    }
    else {
        int worker;

        al4san->parallel_enabled = AL4SAN_TRUE;

        for (worker = 0; worker < ncpus; worker++)
            conf->workers_bindid[worker] = (worker+1)*nthreads_per_worker - 1;

        for (worker = 0; worker < ncpus; worker++)
            conf->workers_bindid[worker + ncudas] = worker*nthreads_per_worker;

        conf->use_explicit_workers_bindid = 1;

        hres = starpu_init( conf );

        al4san->nworkers = ncpus;
        al4san->nthreads_per_worker = nthreads_per_worker;
    }

#ifdef HAVE_STARPU_MALLOC_ON_NODE_SET_DEFAULT_FLAGS
    starpu_malloc_on_node_set_default_flags(STARPU_MAIN_RAM, STARPU_MALLOC_PINNED | STARPU_MALLOC_COUNT
#ifdef STARPU_MALLOC_SIMULATION_FOLDED
            | STARPU_MALLOC_SIMULATION_FOLDED
#endif
            );
#endif

#if defined(AL4SAN_USE_MPI)
    {
        int flag = 0;
#if !defined(AL4SAN_SIMULATION)
        MPI_Initialized( &flag );
#endif
        starpu_mpi_init(NULL, NULL, !flag);
    }
#endif

#if defined(AL4SAN_USE_CUDA) && !defined(AL4SAN_SIMULATION)
    starpu_cublas_init();
#endif

    return hres;
}

/**
 *
 */
void AL4SAN_Runtime_finalize( AL4SAN_context_t *al4san )
{
    (void)al4san;

    /* StarPU was already initialized by an external library */
    if ( al4san->schedopt == NULL ) {
        return;
    }

#if defined(AL4SAN_USE_MPI)
    starpu_mpi_shutdown();
#endif

#if defined(AL4SAN_USE_CUDA) && !defined(AL4SAN_SIMULATION)
    starpu_cublas_shutdown();
#endif

    starpu_shutdown();
    return;
}

/**
 *  To suspend the processing of new tasks by workers
 */
void AL4SAN_Runtime_pause( AL4SAN_context_t *al4san )
{
    (void)al4san;
    starpu_pause();
    return;
}

/**
 *  This is the symmetrical call to AL4SAN_Runtime_pause,
 *  used to resume the workers polling for new tasks.
 */
void AL4SAN_Runtime_resume( AL4SAN_context_t *al4san )
{
    (void)al4san;
    starpu_resume();
    return;
}

/**
 *  Busy-waiting barrier
 */
void AL4SAN_Runtime_barrier( AL4SAN_context_t *al4san )
{
    (void)al4san;
    starpu_task_wait_for_all();
#if defined(AL4SAN_USE_MPI)
    starpu_mpi_barrier(MPI_COMM_WORLD);
#endif
}

// Defined in control/auxilliary.c
extern void (*al4san_update_progress_callback)(int, int);

// no progress indicator for algorithms faster than 'PROGRESS_MINIMUM_DURATION' seconds
#define PROGRESS_MINIMUM_DURATION 10

/**
 *  Display a progress information when executing the tasks
 */
void AL4SAN_Runtime_progress( AL4SAN_context_t *al4san )
{
    int tasksLeft, current, timer = 0;
    int max;

#if defined(AL4SAN_USE_MPI)
    if ( al4san->my_mpi_rank != 0 ) {
        return;
    }
#endif

    max = starpu_task_nsubmitted();
    if ( max == 0 ) {
        return;
    }

    //  update_progress_callback(0, max);
    while ((tasksLeft = starpu_task_nsubmitted()) > 0) {
        current = max - tasksLeft;
        if (timer > PROGRESS_MINIMUM_DURATION) {
            al4san_update_progress_callback(current, max);
        }
        sleep(1);
        timer++;
    }
    if (timer > PROGRESS_MINIMUM_DURATION) {
        al4san_update_progress_callback(max, max);
    }

    (void)al4san;
    return;
}

/**
 * Thread rank.
 */
int AL4SAN_Runtime_thread_rank( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return starpu_worker_get_id();
}

/**
 * Number of threads.
 */
int AL4SAN_Runtime_thread_size( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return starpu_worker_get_count_by_type( STARPU_CPU_WORKER );
}

/**
 *  The process rank
 */
int AL4SAN_Runtime_comm_rank( AL4SAN_context_t *al4san )
{
    int rank = 0;

#if defined(AL4SAN_USE_MPI)
#  if defined(HAVE_STARPU_MPI_COMM_RANK)
    starpu_mpi_comm_rank( MPI_COMM_WORLD, &rank );
#  else
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
#  endif
#endif

    (void)al4san;
    return rank;
}

/**
 *  This returns the size of the distributed computation
 */
int AL4SAN_Runtime_comm_size( AL4SAN_context_t *al4san )
{
    int size;
#if defined(AL4SAN_USE_MPI)
#  if defined(HAVE_STARPU_MPI_COMM_RANK)
    starpu_mpi_comm_size( MPI_COMM_WORLD, &size );
#  else
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#  endif
#else
    size = 1;
#endif

    (void)al4san;
    return size;
}

/**
 *  Flush cached data from runtime descriptor
 */
void AL4SAN_Runtime_flush()
{
#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush_all_data(MPI_COMM_WORLD);
#endif
}
