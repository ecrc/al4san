/**
 *
 * @file control.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon control routines
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 ***
 *
 * @defgroup Control
 * @brief Group routines exposed to users to control ALTANAL state
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "control/auxiliary.h"
#include "control/common.h"
#include "altanal/altanal_runtime.h"

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Init - Initialize ALTANAL.
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
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Init(int cores, int gpus)
{
    return ALTANAL_InitPar(cores, gpus, -1);
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_InitPar - Initialize ALTANAL.
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
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_InitPar(int ncpus, int ncudas, int nthreads_per_worker)
{
    ALTANAL_context_t *altanal;

    /* Create context and insert in the context map */
    altanal = altanal_context_create();
    if (altanal == NULL) {
        altanal_fatal_error("ALTANAL_Init", "altanal_context_create() failed");
        return ALTANAL_ERR_OUT_OF_RESOURCES;
    }

#if defined(ALTANAL_USE_MPI)
#  if defined(ALTANAL_SIMULATION)
    /* Assuming that we don't initialize MPI ourself (which SMPI doesn't support anyway) */
    altanal->mpi_outer_init = 1;
#  else
    {
      int flag = 0, provided = 0;
      MPI_Initialized( &flag );
      altanal->mpi_outer_init = flag;
      if ( !flag ) {
          MPI_Init_thread( NULL, NULL, MPI_THREAD_MULTIPLE, &provided );
      }
    }
#  endif
#endif

    ALTANAL_Runtime_init( altanal, ncpus, ncudas, nthreads_per_worker );

#if defined(ALTANAL_USE_MPI)
    altanal->my_mpi_rank   = ALTANAL_Runtime_comm_rank( altanal );
    altanal->mpi_comm_size = ALTANAL_Runtime_comm_size( altanal );
#endif

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Finalize - Finalize ALTANAL.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Finalize(void)
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Finalize()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    ALTANAL_Runtime_flush();
#  if !defined(ALTANAL_SIMULATION)
    ALTANAL_Runtime_barrier(altanal);
#  endif
    ALTANAL_Runtime_finalize( altanal );

#if defined(ALTANAL_USE_MPI)
    if (!altanal->mpi_outer_init)
        MPI_Finalize();
#endif

    altanal_context_destroy();
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Pause - Suspend ALTANAL runtime to poll for new tasks.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Pause(void)
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Pause()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    ALTANAL_Runtime_pause(altanal);
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Resume - Symmetrical call to ALTANAL_Pause,
 *  used to resume the workers polling for new tasks.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Resume(void)
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Resume()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    ALTANAL_Runtime_resume(altanal);
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Distributed_start - Prepare the distributed processes for computation
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Distributed_start(void)
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Finalize()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    ALTANAL_Runtime_barrier (altanal);
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Distributed_stop - Clean the distributed processes after computation
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Distributed_stop(void)
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Finalize()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    ALTANAL_Runtime_barrier (altanal);
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Comm_size - Return the size of the distributed computation
 *
 ******************************************************************************
 *
 * @retval The size of the distributed computation
 * @retval -1 if context not initialized
 *
 */
int ALTANAL_Comm_size()
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Comm_size()", "ALTANAL not initialized");
        return -1;
    }

    return ALTANAL_Runtime_comm_size( altanal );
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_Comm_rank - Return the rank of the distributed computation
 *
 ******************************************************************************
 *
 * @retval The rank of the distributed computation
 * @retval -1 if context not initialized
 *
 */
int ALTANAL_Comm_rank()
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Comm_rank()", "ALTANAL not initialized");
        return -1;
    }

    return ALTANAL_Runtime_comm_rank( altanal );
}

/**
 *
 * @ingroup Control
 *
 *  ALTANAL_GetThreadNbr - Return the number of CPU workers initialized by the
 *  runtime
 *
 ******************************************************************************
 *
 * @return
 *          \retval The number of CPU workers started
 *
 */
int ALTANAL_GetThreadNbr( )
{
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_GetThreadNbr()", "ALTANAL not initialized");
        return -1;
    }

    return ALTANAL_Runtime_thread_size( altanal );
}
