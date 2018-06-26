/**
 *
 * @file runtime_control.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC control routines
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

#if defined(ALTANAL_USE_MPI)
#include <mpi.h>
#endif

/**
 * Initialize ALTANAL
 */
int ALTANAL_Runtime_init( ALTANAL_context_t *altanal,
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
    altanal->parallel_enabled = ALTANAL_TRUE;
    altanal->schedopt = (void *)parsec_init(default_ncores, argc, NULL);

    if(NULL != altanal->schedopt) {
        altanal->nworkers = ncpus;
        altanal->nthreads_per_worker = nthreads_per_worker;
        hres = 0;
    }

    free(argc);

    (void)ncudas;
    return hres;
}

/**
 * Finalize ALTANAL
 */
void ALTANAL_Runtime_finalize( ALTANAL_context_t *altanal )
{
    parsec_context_t *parsec = (parsec_context_t*)altanal->schedopt;
    parsec_fini(&parsec);
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
 * Barrier ALTANAL.
 */
void ALTANAL_Runtime_barrier( ALTANAL_context_t *altanal )
{
    parsec_context_t *parsec = (parsec_context_t*)(altanal->schedopt);
    // This will be a problem with the fake tasks inserted to detect end of DTD algorithms
    parsec_context_wait( parsec );
    return;
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
    (void)altanal;
    return 0;
}

/**
 * Thread rank.
 */
int ALTANAL_Runtime_thread_size( ALTANAL_context_t *altanal )
{
    // TODO: fixme
    //return vpmap_get_nb_total_threads();
    (void)altanal;
    return 1;
}

/**
 *  This returns the rank of this process
 */
int ALTANAL_Runtime_comm_rank( ALTANAL_context_t *altanal )
{
    int rank = 0;
#if defined(ALTANAL_USE_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    (void)altanal;
    return rank;
}

/**
 *  This returns the size of the distributed computation
 */
int ALTANAL_Runtime_comm_size( ALTANAL_context_t *altanal )
{
    int size = 0;
#if defined(ALTANAL_USE_MPI)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

    (void)altanal;
    return size;
}

/**
 *  Flush cached data from runtime descriptor
 */
void ALTANAL_Runtime_flush()
{
 return;
}