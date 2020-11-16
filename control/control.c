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
 * @defgroup Context
 * @brief AL4SAN context routines are exposed to the users
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2012-09-15
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "control/al4san_auxiliary.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"
#include <string.h>
/**
 *
 * @ingroup Context 
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
AL4SAN_context_t*  AL4SAN_Init(char *runtime, int cores, int gpus)
{
    return AL4SAN_InitPar(runtime, cores, gpus, -1);
}

/**
 *
 * @ingroup Context
 *
 *  AL4SAN_InitPar - Initialize AL4SAN.
 *
 ******************************************************************************
 *
 * @param[in] runtime name
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
 *          \retval AL4SAN context data structure
 *
 */
AL4SAN_context_t*  AL4SAN_InitPar(char *runtime, int ncpus, int ncudas, int nthreads_per_worker)
{
    AL4SAN_context_t *al4san;

    /* Create context and insert in the context map */
    al4san = al4san_context_create(runtime);
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
       al4san->starpu_schd=-5;

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0){
    AL4SAN_Quark_init( al4san, ncpus, ncudas, nthreads_per_worker );
    AL4SAN_Quark_task_option_init();
    }
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1){
         AL4SAN_Starpu_init( al4san, ncpus, ncudas, nthreads_per_worker );
    AL4SAN_Starpu_task_option_init();   
      al4san->starpu_schd=1;  
     }
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2){
         AL4SAN_Parsec_init( al4san, ncpus, ncudas, nthreads_per_worker );
         AL4SAN_Parsec_task_option_init();
      }
#endif
#ifdef AL4SAN_SCHED_OPENMP    
    if(al4san->scheduler==3){
         AL4SAN_Openmp_init( al4san, ncpus, ncudas, nthreads_per_worker );
         AL4SAN_Openmp_task_option_init();
     }
#endif

    //AL4SAN_Runtime_init( al4san, ncpus, ncudas, nthreads_per_worker );

#if defined(AL4SAN_USE_MPI)
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    al4san->my_mpi_rank   = AL4SAN_Quark_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         al4san->my_mpi_rank   = AL4SAN_Starpu_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         al4san->my_mpi_rank   = AL4SAN_Parsec_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_OPENMP  
    if(al4san->scheduler==3)
         al4san->my_mpi_rank   = AL4SAN_Openmp_comm_rank( al4san );
#endif

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    al4san->mpi_comm_size   = AL4SAN_Quark_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         al4san->mpi_comm_size   = AL4SAN_Starpu_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         al4san->mpi_comm_size   = AL4SAN_Parsec_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         al4san->mpi_comm_size    = AL4SAN_Openmp_comm_size( al4san );
#endif

    //al4san->mpi_comm_size = AL4SAN_Runtime_comm_size( al4san );
#endif

    return al4san;
}

/**
 *
 * @ingroup Context
 *
 *  AL4SAN_Switch_Runtime - Switching AL4SAN backend runtime.
 *
 ******************************************************************************
 *
 * @param[in] runtime name
 *
 * @param[in] ncpus
 *          Number of cores to use.
 *
 * @param[in] ncudas
 *          Number of cuda devices to use.
 *
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN context data structure
 *
 */
AL4SAN_context_t*  AL4SAN_Switch_Runtime(char *runtime, int ncpus, int ncudas)
{

    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Switch_Runtime()", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_flush();
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_flush();
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_flush();
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_flush();
#endif

    //AL4SAN_Runtime_flush();
#  if !defined(AL4SAN_SIMULATION)
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_barrier(al4san);
#endif

#  endif
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_finalize(al4san);
#endif
          /* Initialize scheduler */
#ifdef AL4SAN_SCHED_QUARK
    if(strcmp(runtime, "Quark")==0){
    AL4SAN_Quark_context_create(al4san);
    AL4SAN_Quark_init( al4san, ncpus, ncudas, -1 );
    AL4SAN_Quark_task_option_init();
    }
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(strcmp(runtime, "Starpu")==0){
        AL4SAN_Starpu_context_create(al4san);
        AL4SAN_Starpu_init( al4san, ncpus, ncudas, -1 );
        AL4SAN_Starpu_task_option_init(); 
    al4san->starpu_schd=1;
    }
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(strcmp(runtime, "Parsec")==0){
        AL4SAN_Parsec_context_create(al4san);
        AL4SAN_Parsec_init( al4san, ncpus, ncudas, -1 );
        AL4SAN_Parsec_task_option_init();
    }
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(strcmp(runtime, "Openmp")){
        AL4SAN_Openmp_context_create(al4san); 
        AL4SAN_Openmp_init( al4san, ncpus, ncudas, -1 );
        AL4SAN_Openmp_task_option_init();
    }
#endif

#if defined(AL4SAN_USE_MPI)

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    al4san->my_mpi_rank   = AL4SAN_Quark_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         al4san->my_mpi_rank   = AL4SAN_Starpu_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         al4san->my_mpi_rank   = AL4SAN_Parsec_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         al4san->my_mpi_rank   = AL4SAN_Openmp_comm_rank( al4san );
#endif

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    al4san->mpi_comm_size   = AL4SAN_Quark_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         al4san->mpi_comm_size   = AL4SAN_Starpu_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         al4san->mpi_comm_size   = AL4SAN_Parsec_comm_size( al4san );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         al4san->mpi_comm_size  = AL4SAN_Openmp_comm_size( al4san );
#endif

#endif

    return al4san;
}

/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_flush();
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_flush();
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_flush();
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_flush();
#endif

    //AL4SAN_Runtime_flush();
#  if !defined(AL4SAN_SIMULATION)
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_barrier(al4san);
#endif
#  endif
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_finalize(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_finalize(al4san);
#endif


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
 * @ingroup Context
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
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_pause(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU 
    if(al4san->scheduler==1)
         AL4SAN_Starpu_pause(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_pause(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_pause(al4san);
#endif

    //AL4SAN_Runtime_pause(al4san);
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Context
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
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_resume(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU 
    if(al4san->scheduler==1)
         AL4SAN_Starpu_resume(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_resume(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_resume(al4san);
#endif

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Context
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
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_barrier(al4san);
#endif
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_barrier(al4san);
#endif
    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Context
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

 //    AL4SAN_Runtime_barrier( al4san );

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_barrier(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_barrier(al4san);
#endif

    return AL4SAN_SUCCESS;     
}
/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_progress(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_progress(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_progress(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP
    if(al4san->scheduler==3) 
         AL4SAN_Openmp_progress(al4san);
#endif

    return AL4SAN_SUCCESS;          
}
/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_thread_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_thread_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_thread_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_thread_rank(al4san);
#endif         
}

/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP
    if(al4san->scheduler==3) 
         AL4SAN_Openmp_thread_size(al4san);
#endif 
}

/**
 *
 * @ingroup Context
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
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_comm_size(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_comm_size(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_comm_size(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_comm_size(al4san);
#endif
}

/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_comm_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_comm_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_comm_rank(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP
    if(al4san->scheduler==3) 
         AL4SAN_Openmp_comm_rank(al4san);
#endif

}

/**
 *
 * @ingroup Context
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

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_thread_size(al4san);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_thread_size(al4san);
#endif
}

/**
 *
 * @ingroup Context
 *
 *  AL4SAN_Init_Processor_Grid - Makes a new communicator to which topology information has been attached 
 * if distributed setting is activiated
 *
 * @param[in] number of row processors
 *
 * @param[in] number of col processors
 *          Number of cores to use. 
 *
 */
#if defined(AL4SAN_USE_MPI)
void AL4SAN_Init_Processor_Grid(int p, int q){

     AL4SAN_context_t *al4sanctxt;

        al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_error("AL4SAN_Init_Processor_Grid", "AL4SAN not initialized");
        //return CHAMELEON_ERR_NOT_INITIALIZED;
    }

    int rank, size;
    MPI_Comm comm;
    int dim[2], period[2], reorder;

    dim[0]=p; dim[1]=q;
    period[0]=1; period[1]=0;
    reorder=1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
    al4sanctxt->newcomm=comm;
}
#endif
