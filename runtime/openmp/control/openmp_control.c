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
* @brief AL4SAN OpenMP control routines
*
*  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
*
 *
 * version 1.0.0
 * author Vijay Joshi
 * author Cedric Castagnede
 * date 2012-09-15
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 */
#include <stdio.h>
#include <stdlib.h>
#include "al4san_openmp.h"


/*******************************************************************************
 * Initialize AL4SAN
 **/
int AL4SAN_Openmp_init(AL4SAN_context_t *al4san, 
                  int ncpus, 
                  int ncudas, 
                  int nthreads_per_worker)
{
    int hres = 0;
    if ( ncudas > 0 )
        al4san_warning( "AL4SAN_Openmp_init_scheduler(OpenMP)", "GPUs are not supported for now");

    if ( nthreads_per_worker > 0 )
        al4san_warning( "AL4SAN_Openmp_init_scheduler(OpenMP)", "Multi-threaded kernels are not supported for now");

    omp_set_num_threads(ncpus);

    al4san->world_size=ncpus;
    al4san->parallel_enabled = AL4SAN_TRUE;
return hres;
}

/*******************************************************************************
 * Finalize AL4SAN
 */
void AL4SAN_Openmp_finalize(AL4SAN_context_t *al4san)
{
    #pragma omp taskwait
    (void)al4san;
    return;
}
/*******************************************************************************
 *  To suspend the processing of new tasks by workers
 **/
void AL4SAN_Openmp_pause( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/*******************************************************************************
 *  This is the symmetrical call to AL4SAN_runtime_pause,
 *  used to resume the workers polling for new tasks.
 **/
void AL4SAN_Openmp_resume( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}
/*******************************************************************************
 * Barrier AL4SAN.
 **/
void AL4SAN_Openmp_barrier(AL4SAN_context_t *al4san)
{
        //  #pragma omp taskwait
    #pragma omp barrier
    (void)al4san;

}

//I added it, don't forget to add it to runrime.h
void AL4SAN_Openmp_barrier_on(AL4SAN_context_t *al4san, double *ptr)
{
    #pragma omp barrier
    (void)al4san;
}

/**
 *  Display a progress information when executing the tasks
 */
void AL4SAN_Openmp_progress( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/*******************************************************************************
 * Thread rank.
 **/
int AL4SAN_Openmp_thread_rank(AL4SAN_context_t *al4san)
{

    int thread_rank = omp_get_thread_num();
    (void)al4san;
    return thread_rank;
}

/**
 * Number of threads.
 */
int AL4SAN_Openmp_thread_size( AL4SAN_context_t *al4san )
{
    (void)al4san;
   int nworkers;
    #pragma omp parallel
    {
    nworkers=omp_get_num_threads();
    }    
    return nworkers;
}

/**
 *  The process rank
 */
int AL4SAN_Openmp_comm_rank( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return omp_get_thread_num();
}

/**
 *  This returns the size of the distributed computation
 */
int AL4SAN_Openmp_comm_size( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return 1;
}
