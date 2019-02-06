/**
 *
 * @file control.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * author Jakub Kurzak
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2012-09-15
 *
 *
 *
 */

  /**
   *
   * @brief AL4SAN control routines
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.1
   * @author Rabab Alomairy
   * @date 2019-02-06
   *
   **/


#include <stdio.h>
#include <stdlib.h>
#include "control/al4san_auxiliary.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Init - Initialize AL4SAN.
 *
 ******************************************************************************
 *
 * @param[in] cores
 *          Number of cores to use.
 *
 * @param[in] gpus
 *          Number of cuda devices to use.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
AL4SAN_context_t*  AL4SAN_Init(int cores, int gpus)
{
    return AL4SAN_InitPar(cores, gpus, -1);
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_InitPar - Initialize AL4SAN.
 *
 ******************************************************************************
 *
 * @param[in] ncpus
 *          Number of cores to use.
 *
 * @param[in] ncudas
 *          Number of cuda devices to use.
 *
 * @param[in] nthreads_per_worker
 *          Number of threads per worker (cpu, cuda device).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
AL4SAN_context_t*  AL4SAN_InitPar(int ncpus, int ncudas, int nthreads_per_worker)
{
    AL4SAN_context_t *al4san;

    /* Create context and insert in the context map */
    al4san = al4san_context_create();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Init", "al4san_context_create() failed");
        //return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

#if defined(AL4SAN_USE_MPI)
#  if defined(AL4SAN_SIMULATION)
    /* Assuming that we don't initialize MPI ourself (which SMPI doesn't support anyway) */
    al4san->mpi_outer_init = 1;
#  else
    {
      int flag = 0, provided = 0;
      MPI_Initialized( &flag );
      al4san->mpi_outer_init = flag;
      if ( !flag ) {
          MPI_Init_thread( NULL, NULL, MPI_THREAD_MULTIPLE, &provided );
      }
    }
#  endif
#endif

    AL4SAN_Runtime_init( al4san, ncpus, ncudas, nthreads_per_worker );

#if defined(AL4SAN_USE_MPI)
    al4san->my_mpi_rank   = AL4SAN_Runtime_comm_rank( al4san );
    al4san->mpi_comm_size = AL4SAN_Runtime_comm_size( al4san );
#endif

    return al4san;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Finalize - Finalize AL4SAN.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Finalize(void)
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Finalize()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_Runtime_flush();
#  if !defined(AL4SAN_SIMULATION)
    AL4SAN_Runtime_barrier(al4san);
#  endif
    AL4SAN_Runtime_finalize( al4san );

#if defined(AL4SAN_USE_MPI)
    int flags;
    MPI_Finalized(&flags);
    if (!al4san->mpi_outer_init && !flags)
        MPI_Finalize();
#endif

    al4san_context_destroy();
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Pause - Suspend AL4SAN runtime to poll for new tasks.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Pause(void)
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Pause()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_Runtime_pause(al4san);
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Resume - Symmetrical call to AL4SAN_Pause,
 *  used to resume the workers polling for new tasks.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Resume(void)
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Resume()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_Runtime_resume(al4san);
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Distributed_start - Prepare the distributed processes for computation
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Distributed_Start(void)
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Finalize()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_Runtime_barrier (al4san);
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Distributed_stop - Clean the distributed processes after computation
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Distributed_Stop(void)
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Finalize()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_Runtime_barrier (al4san);
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Barrier
 *
 *
 */
int AL4SAN_Barrier()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Barrier()", "AL4SAN not initialized");
        return -1;
    }

     AL4SAN_Runtime_barrier( al4san );
    return AL4SAN_SUCCESS;     
}
/**
 *
 * @ingroup Control
 *
 *  AL4SAN_progress
 *
 *
 */
int AL4SAN_Progress()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_progress()", "AL4SAN not initialized");
        return -1;
    }

     AL4SAN_Runtime_progress( al4san );
    return AL4SAN_SUCCESS;          
}
/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Thread_rank - Return the rank of the thread
 *
 ******************************************************************************
 *
 * @retval The rank of the thread
 * @retval -1 if context not initialized
 *
 */
int AL4SAN_Thread_Rank()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_rank()", "AL4SAN not initialized");
        return -1;
    }

    return AL4SAN_Runtime_thread_rank( al4san );
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Thread_size - Return the number of the threads
 *
 ******************************************************************************
 *
 * @retval The number of the threads
 * @retval -1 if context not initialized
 *
 */
int AL4SAN_Thread_Size()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_size()", "AL4SAN not initialized");
        return -1;
    }

    return AL4SAN_Runtime_thread_size( al4san );
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Comm_size - Return the size of the distributed computation
 *
 ******************************************************************************
 *
 * @retval The size of the distributed computation
 * @retval -1 if context not initialized
 *
 */
int AL4SAN_Comm_Size()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_size()", "AL4SAN not initialized");
        return -1;
    }

    return AL4SAN_Runtime_comm_size( al4san );
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_Comm_rank - Return the rank of the distributed computation
 *
 ******************************************************************************
 *
 * @retval The rank of the distributed computation
 * @retval -1 if context not initialized
 *
 */
int AL4SAN_Comm_Rank()
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_rank()", "AL4SAN not initialized");
        return -1;
    }

    return AL4SAN_Runtime_comm_rank( al4san );
}

/**
 *
 * @ingroup Control
 *
 *  AL4SAN_GetThreadNbr - Return the number of CPU workers initialized by the
 *  runtime
 *
 ******************************************************************************
 *
 * @return
 *          \retval The number of CPU workers started
 *
 */
int AL4SAN_GetThreadNbr( )
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_GetThreadNbr()", "AL4SAN not initialized");
        return -1;
    }

    return AL4SAN_Runtime_thread_size( al4san );
}
