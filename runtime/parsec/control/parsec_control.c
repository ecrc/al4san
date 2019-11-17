/**
 *
 * @file runtime_control.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 *
 * author Reazul Hoque
 * author Mathieu Faverge
 * date 2017-01-12
 *
 */
/**
 *
 * @brief AL4SAN PaRSEC control routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
**/
#include <stdio.h>
#include <stdlib.h>
#include "al4san_parsec.h"

#if defined(AL4SAN_USE_MPI)
#include <mpi.h>
#endif

/**
 * Initialize AL4SAN
 */
int AL4SAN_Parsec_init( AL4SAN_context_t *al4san,
                  int ncpus,
                  int ncudas,
                  int nthreads_per_worker )
{
    int hres = -1, default_ncores = -1;
    int *argc = (int *)malloc(sizeof(int));
    *argc = 0;

    /* Initializing parsec context */
    if( 0 < ncpus ) {
        default_ncores = ncpus;
    }
    al4san->parallel_enabled = AL4SAN_TRUE;
    al4san->schedopt = (void *)parsec_init(default_ncores, argc, NULL);

    if(NULL != al4san->schedopt) {
        al4san->nworkers = ncpus;
        al4san->nthreads_per_worker = nthreads_per_worker;
        hres = 0;
    }

    free(argc);

    (void)ncudas;
    return hres;
}

/**
 * Finalize AL4SAN
 */
void AL4SAN_Parsec_finalize( AL4SAN_context_t *al4san )
{
    parsec_context_t *parsec = (parsec_context_t*)al4san->schedopt;
    parsec_fini(&parsec);
    return;
}

/**
 *  To suspend the processing of new tasks by workers
 */
void AL4SAN_Parsec_pause( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 *  This is the symmetrical call to AL4SAN_Parsec_pause,
 *  used to resume the workers polling for new tasks.
 */
void AL4SAN_Parsec_resume( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 * Barrier AL4SAN.
 */
void AL4SAN_Parsec_barrier( AL4SAN_context_t *al4san )
{
    parsec_context_t *parsec = (parsec_context_t*)(al4san->schedopt);
    // This will be a problem with the fake tasks inserted to detect end of DTD algorithms
    parsec_context_wait( parsec );
    return;
}

/**
 *  Display a progress information when executing the tasks
 */
void AL4SAN_Parsec_progress( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 * Thread rank.
 */
int AL4SAN_Parsec_thread_rank( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return 0;
}

/**
 * Thread rank.
 */
int AL4SAN_Parsec_thread_size( AL4SAN_context_t *al4san )
{
    // TODO: fixme
    //return vpmap_get_nb_total_threads();
    (void)al4san;
    return 1;
}

/**
 *  This returns the rank of this process
 */
int AL4SAN_Parsec_comm_rank( AL4SAN_context_t *al4san )
{
    int rank = 0;
#if defined(AL4SAN_USE_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    (void)al4san;
    return rank;
}

/**
 *  This returns the size of the distributed computation
 */
int AL4SAN_Parsec_comm_size( AL4SAN_context_t *al4san )
{
    int size = 0;
#if defined(AL4SAN_USE_MPI)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

    (void)al4san;
    return size;
}

