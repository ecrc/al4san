/**
 *
 * @file runtime.h
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
 * @author Mathieu Faverge
 * @author Cedric Augonnet
 * @author Cedric Castagnede
 * @date 2011-06-01
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @brief AL4SAN runtimes API
 *
 * AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 **/
#ifndef _AL4SAN_AL4SAN_Runtime_H_
#define _AL4SAN_AL4SAN_Runtime_H_

#include "al4san/config.h"
#include "al4san/struct.h"
#include "al4san/task.h"
#include "al4san/constants.h"
#include <stdarg.h>
BEGIN_C_DECLS

/**
 * @name RUNTIME Context functions
 * @{
 *    These functions manipulate the context data structure before the runtime is
 *    started and after the runtime is stopped.
 */

/**
 * @brief QUARK Create the runtime specific options in the context before starting it
 *
 * @param[in,out] ctxt
 *            The Al4san context to initialize for the runtime.
 */
void
AL4SAN_Quark_context_create( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Create the runtime specific options in the context before starting it
 *
 * @param[in,out] ctxt
 *            The Al4san context to initialize for the runtime.
 */
void
AL4SAN_Starpu_context_create( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Create the runtime specific options in the context before starting it
 *
 * @param[in,out] ctxt
 *            The Al4san context to initialize for the runtime.
 */
void
AL4SAN_Parsec_context_create( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Create the runtime specific options in the context before starting it
 *
 * @param[in,out] ctxt
 *            The Al4san context to initialize for the runtime.
 */
void
AL4SAN_Openmp_context_create( AL4SAN_context_t *ctxt );

/**
 * @brief QUARK Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Al4san context in which the runtime specific options must
 *            be destroyed
 */
void
AL4SAN_Quark_context_destroy( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Al4san context in which the runtime specific options must
 *            be destroyed
 */
void
AL4SAN_Starpu_context_destroy( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Al4san context in which the runtime specific options must
 *            be destroyed
 */
void
AL4SAN_Parsec_context_destroy( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Al4san context in which the runtime specific options must
 *            be destroyed
 */
void
AL4SAN_Openmp_context_destroy( AL4SAN_context_t *ctxt );

/**
 * @brief QUARK Enable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Enable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
AL4SAN_Quark_enable( AL4SAN_enum option );
/**
 * @brief StarPU Enable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Enable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
AL4SAN_Starpu_enable( AL4SAN_enum option );
/**
 * @brief PaRSEC Enable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Enable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
AL4SAN_Parsec_enable( AL4SAN_enum option );
/**
 * @brief OpenMP Enable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Enable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
AL4SAN_Openmp_enable( AL4SAN_enum option );

/**
 * @brief QUARK Disable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Disable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
AL4SAN_Quark_disable( AL4SAN_enum option );
/**
 * @brief StarPU Disable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Disable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
AL4SAN_Starpu_disable( AL4SAN_enum option );
/**
 * @brief PaRSEC Disable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Disable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
AL4SAN_Parsec_disable( AL4SAN_enum option );
/**
 * @brief OpenMP Disable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Disable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
AL4SAN_Openmp_disable( AL4SAN_enum option );
/**
 * @}
 *
 * @name RUNTIME Control functions
 * @{
 *   These functions control the global behavior of the runtime.
 */

/**
 * @brief QAURK Initialize the scheduler with the given parameters
 *
 * @param[in,out] ctxt
 *            The Al4san context in which to initialize the runtime support.
 *
 * @param[in] ncpus
 *            Defines the total number of cores given to the runtime per
 *            node. Including cuda and communication workers for runtimes that
 *            dedicates cores to this. ncpus > 0, or -1 to target the whole machine.
 *
 * @param[in] ncudas
 *            Defines the number of CUDA devices used by node. If the runtime
 *            uses one core dedicated to each CUDA device, they will be taken
 *            from ncpus. If ncpus > 0, ncudas < ncpus. -1 to target all the
 *            CUDA devices available.
 *
 * @param[in] nthreads_per_worker
 *            Defines the number of threads per worker when multi-threaded tasks
 *            are enabled.  This is used to exploit parallel BLAS kernels, and
 *            defines a better binding of the workers.
 *            -1 to disable, or > 0 to enable.
 *
 * @retval -1 on failure to initialize the runtime.
 * @retval >0 on success to initialize the runtime.
 *
 */
int
AL4SAN_Quark_init( AL4SAN_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );
/**
 * @brief StarPU Initialize the scheduler with the given parameters
 *
 * @param[in,out] ctxt
 *            The Al4san context in which to initialize the runtime support.
 *
 * @param[in] ncpus
 *            Defines the total number of cores given to the runtime per
 *            node. Including cuda and communication workers for runtimes that
 *            dedicates cores to this. ncpus > 0, or -1 to target the whole machine.
 *
 * @param[in] ncudas
 *            Defines the number of CUDA devices used by node. If the runtime
 *            uses one core dedicated to each CUDA device, they will be taken
 *            from ncpus. If ncpus > 0, ncudas < ncpus. -1 to target all the
 *            CUDA devices available.
 *
 * @param[in] nthreads_per_worker
 *            Defines the number of threads per worker when multi-threaded tasks
 *            are enabled.  This is used to exploit parallel BLAS kernels, and
 *            defines a better binding of the workers.
 *            -1 to disable, or > 0 to enable.
 *
 * @retval -1 on failure to initialize the runtime.
 * @retval >0 on success to initialize the runtime.
 *
 */
int
AL4SAN_Starpu_init( AL4SAN_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );
/**
 * @brief PaRSEC Initialize the scheduler with the given parameters
 *
 * @param[in,out] ctxt
 *            The Al4san context in which to initialize the runtime support.
 *
 * @param[in] ncpus
 *            Defines the total number of cores given to the runtime per
 *            node. Including cuda and communication workers for runtimes that
 *            dedicates cores to this. ncpus > 0, or -1 to target the whole machine.
 *
 * @param[in] ncudas
 *            Defines the number of CUDA devices used by node. If the runtime
 *            uses one core dedicated to each CUDA device, they will be taken
 *            from ncpus. If ncpus > 0, ncudas < ncpus. -1 to target all the
 *            CUDA devices available.
 *
 * @param[in] nthreads_per_worker
 *            Defines the number of threads per worker when multi-threaded tasks
 *            are enabled.  This is used to exploit parallel BLAS kernels, and
 *            defines a better binding of the workers.
 *            -1 to disable, or > 0 to enable.
 *
 * @retval -1 on failure to initialize the runtime.
 * @retval >0 on success to initialize the runtime.
 *
 */
int
AL4SAN_Parsec_init( AL4SAN_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );
/**
 * @brief OpenMP Initialize the scheduler with the given parameters
 *
 * @param[in,out] ctxt
 *            The Al4san context in which to initialize the runtime support.
 *
 * @param[in] ncpus
 *            Defines the total number of cores given to the runtime per
 *            node. Including cuda and communication workers for runtimes that
 *            dedicates cores to this. ncpus > 0, or -1 to target the whole machine.
 *
 * @param[in] ncudas
 *            Defines the number of CUDA devices used by node. If the runtime
 *            uses one core dedicated to each CUDA device, they will be taken
 *            from ncpus. If ncpus > 0, ncudas < ncpus. -1 to target all the
 *            CUDA devices available.
 *
 * @param[in] nthreads_per_worker
 *            Defines the number of threads per worker when multi-threaded tasks
 *            are enabled.  This is used to exploit parallel BLAS kernels, and
 *            defines a better binding of the workers.
 *            -1 to disable, or > 0 to enable.
 *
 * @retval -1 on failure to initialize the runtime.
 * @retval >0 on success to initialize the runtime.
 *
 */
int
AL4SAN_Openmp_init( AL4SAN_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );

/**
 * @brief QUARK Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Al4san context for which the runtime system must be shut down.
 */
void
AL4SAN_Quark_finalize( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Al4san context for which the runtime system must be shut down.
 */
void
AL4SAN_Starpu_finalize( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Al4san context for which the runtime system must be shut down.
 */
void
AL4SAN_Parsec_finalize( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Al4san context for which the runtime system must be shut down.
 */
void
AL4SAN_Openmp_finalize( AL4SAN_context_t *ctxt );

/**
 * @brief QUARK Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the suspension must be made.
 */
void
AL4SAN_Quark_pause( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the suspension must be made.
 */
void
AL4SAN_Starpu_pause( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the suspension must be made.
 */
void
AL4SAN_Parsec_pause( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the suspension must be made.
 */
void
AL4SAN_Openmp_pause( AL4SAN_context_t *ctxt );

/**
 * @brief QUARK Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the execution must be resumed.
 */
void
AL4SAN_Quark_resume( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the execution must be resumed.
 */
void
AL4SAN_Starpu_resume( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the execution must be resumed.
 */
void
AL4SAN_Parsec_resume( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the execution must be resumed.
 */
void
AL4SAN_Openmp_resume( AL4SAN_context_t *ctxt );
/**
 * @brief QUARK Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context in which the task completion is performed.
 */
void
AL4SAN_Quark_barrier( AL4SAN_context_t *ctxt );
/**
 * @brief StarPU Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context in which the task completion is performed.
 */
void
AL4SAN_Starpu_barrier( AL4SAN_context_t *ctxt );
/**
 * @brief PaRSEC Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context in which the task completion is performed.
 */
void
AL4SAN_Parsec_barrier( AL4SAN_context_t *ctxt );
/**
 * @brief OpenMP Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context in which the task completion is performed.
 */
void
AL4SAN_Openmp_barrier( AL4SAN_context_t *ctxt );
/**
 * @brief Show the progress of the computations when enabled.
 *
 * @param[in] ctxt
 *            The Al4san context for which the context needs to be printed.
 */
void
AL4SAN_Quark_progress( AL4SAN_context_t *ctxt );
void
AL4SAN_Starpu_progress( AL4SAN_context_t *ctxt );
void
AL4SAN_Parsec_progress( AL4SAN_context_t *ctxt );
void
AL4SAN_Openmp_progress( AL4SAN_context_t *ctxt );
/**
 * @brief Get the rank of the current worker for the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the thread rank is asked.
 *
 * @retval The rank of the current thread in the runtime.
 */
int
AL4SAN_Quark_thread_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Starpu_thread_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Parsec_thread_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Openmp_thread_rank( AL4SAN_context_t *ctxt );
/**
 * @brief Get the number of CPU workers of the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the number of workers is requested
 *
 * @retval The number of threads currently used by the runtime.
 */
int
AL4SAN_Quark_thread_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Starpu_thread_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Parsec_thread_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Openmp_thread_size( AL4SAN_context_t *ctxt );
/**
 * @brief Get the MPI comm rank of the current process related to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the rank is asked.
 *
 * @retval The rank of the process in the communicator known by the runtime.
 */
int
AL4SAN_Quark_comm_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Starpu_comm_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Parsec_comm_rank( AL4SAN_context_t *ctxt );
int
AL4SAN_Openmp_comm_rank( AL4SAN_context_t *ctxt );
/**
 * @brief Get the MPI comm size related to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the communicator size is asked.
 *
 * @retval The size of the communicator known by the runtime.
 */
int
AL4SAN_Quark_comm_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Starpu_comm_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Parsec_comm_size( AL4SAN_context_t *ctxt );
int
AL4SAN_Openmp_comm_size( AL4SAN_context_t *ctxt );
/**
 * @brief Set the data tags bounds based on runtime tags limit.
 *
 * @param[in] user_tag_width
 *            Total number of bits used to defined the data tags (31 by default).
 *            Must be larger than 20.
 *
 * @param[in] user_tag_sep
 *            The number of bits dedicated to identify the pieces of data per
 *            descriptor.  (user_tag_width - user_tag_sep) defines the number of
 *            bits used to enumerate the descriptors.
 */
void
AL4SAN_Quark_comm_set_tag_sizes( int user_tag_width,
                            int user_tag_sep );
void
AL4SAN_Starpu_comm_set_tag_sizes( int user_tag_width,
                            int user_tag_sep );
void
AL4SAN_Parsec_comm_set_tag_sizes( int user_tag_width,
                            int user_tag_sep );
void
AL4SAN_Openmp_comm_set_tag_sizes( int user_tag_width,
                            int user_tag_sep );
/**
 * @}
 *
 * @name RUNTIME Asynchonous functionalities
 * @{
 *    These functions manage the sequences of tasks. A sequence is a subset of
 *    related tasks belonging to a same problem.
 */

/**
 * @brief Create a sequence structure associated to a given context.
 *
 * @param[in] ctxt
 *            The Al4san context in which the sequence is created.
 *
 * @param[in,out] sequence
 *            On entry the allocated Al4san sequence structure, and on exit
 *            the scheduler specifics for the sequence have been initialized.
 *
 * @retval AL4SAN_SUCCESS on success.
 * @retval AL4SAN_ERR_OUT_OF_RESOURCES, if the sequence could not be created.
 */
int
AL4SAN_Quark_sequence_create( AL4SAN_context_t  *ctxt, AL4SAN_sequence_t* sequence);
int
AL4SAN_Starpu_sequence_create( AL4SAN_context_t  *ctxt, AL4SAN_sequence_t* sequence);
int
AL4SAN_Parsec_sequence_create( AL4SAN_context_t  *ctxt, AL4SAN_sequence_t* sequence);
int
AL4SAN_Openmp_sequence_create( AL4SAN_context_t  *ctxt, AL4SAN_sequence_t* sequence);
/**
 * @brief Destroy the sequence structure.
 *
 * @param[in] ctxt
 *            The Al4san context to which the sequence belongs.
 *
 * @param[in,out] sequence
 *            On entry the sequence structure.
 *            On exit, the scheduler specifics of the sequence have been
 *            destroyed.
 *
 * @retval AL4SAN_SUCCESS on success.
 */
int
AL4SAN_Quark_sequence_destroy( AL4SAN_context_t  *ctxt,
                          AL4SAN_sequence_t *sequence);
int
AL4SAN_Starpu_sequence_destroy( AL4SAN_context_t  *ctxt,
                          AL4SAN_sequence_t *sequence);
int
AL4SAN_Parsec_sequence_destroy( AL4SAN_context_t  *ctxt,
                          AL4SAN_sequence_t *sequence);
int
AL4SAN_Openmp_sequence_destroy( AL4SAN_context_t  *ctxt,
                          AL4SAN_sequence_t *sequence);
/**
 * @brief Wait for completion of all tasks in the given sequence.
 *
 * @param[in] ctxt
 *            The Al4san context to which the sequence belongs.
 *
 * @param[in] sequence
 *            The sequence that gathers a set of tasks to complete.
 *
 * @retval AL4SAN_SUCCESS on success.
 * @retval The algorithm return code on failure.
 */
int
AL4SAN_Quark_sequence_wait( AL4SAN_context_t  *ctxt,
                       AL4SAN_sequence_t *sequence );
int
AL4SAN_Starpu_sequence_wait( AL4SAN_context_t  *ctxt,
                       AL4SAN_sequence_t *sequence );
int
AL4SAN_Parsec_sequence_wait( AL4SAN_context_t  *ctxt,
                       AL4SAN_sequence_t *sequence );
int
AL4SAN_Openmp_sequence_wait( AL4SAN_context_t  *ctxt,
                       AL4SAN_sequence_t *sequence );
/**
 * @brief Cancel all remaining tasks in the given sequence.
 *
 * @param[in] ctxt
 *            The Al4san context to which the sequence belongs.
 *
 * @param[in,out] sequence
 *            The sequence that gathers the set of tasks to cancel.
 *            On exit, the sequence holds the error code of the algorithm and
 *            the request that failed in the sequence.
 *
 * @param[in,out] request
 *            The request that failed in the sequence.
 *            On exit, the return status is set to status.
 *
 * @param[in] status
 *            The return status of the task that failed in the request given
 *            above.
 */
void
AL4SAN_Quark_sequence_flush( AL4SAN_context_t  *ctxt,
                        AL4SAN_sequence_t *sequence,
                        AL4SAN_request_t  *request,
                        int               status );
void
AL4SAN_Starpu_sequence_flush( AL4SAN_context_t  *ctxt,
                        AL4SAN_sequence_t *sequence,
                        AL4SAN_request_t  *request,
                        int               status );
void
AL4SAN_Parsec_sequence_flush( AL4SAN_context_t  *ctxt,
                        AL4SAN_sequence_t *sequence,
                        AL4SAN_request_t  *request,
                        int               status );
void
AL4SAN_Openmp_sequence_flush( AL4SAN_context_t  *ctxt,
                        AL4SAN_sequence_t *sequence,
                        AL4SAN_request_t  *request,
                        int               status );
/**
 * @}
 *
 * @name RUNTIME Memory management
 * @{
 ***
 * @brief Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.

 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */
void *
AL4SAN_Quark_malloc( size_t size );
void *
AL4SAN_Starpu_malloc( size_t size );
void *
AL4SAN_Parsec_malloc( size_t size );
void *
AL4SAN_Openmp_malloc( size_t size );
/**
 * @}
 *
 * @name RUNTIME Memory management
 * @{
 ***
 * @brief QUARK Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.
 * @param[in, out] A
 *            Pointer to allocated memory
 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */
void 
AL4SAN_Quark_malloc_ptr(void**A, size_t size );

/**
 *
 * @brief StarPU Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.
 * @param[in, out] A
 *            Pointer to allocated memory
 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */
void 
AL4SAN_Starpu_malloc_ptr(void**A, size_t size );


/**
 *
 * @brief PaRSEC Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.
 * @param[in, out] A
 *            Pointer to allocated memory
 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */
void 
AL4SAN_Parsec_malloc_ptr(void**A, size_t size );

/**
 *
 * @brief OpenMP Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.
 * @param[in, out] A
 *            Pointer to allocated memory
 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */
void 
AL4SAN_Openmp_malloc_ptr(void**A, size_t size );
/**
 * @brief QUARK Free allocated memory through AL4SAN_Runtime_malloc() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void
AL4SAN_Quark_free( void *ptr, size_t size );
/**
 * @brief StarPU Free allocated memory through AL4SAN_Runtime_malloc() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void
AL4SAN_Starpu_free( void *ptr, size_t size );
/**
 * @brief PaRSEC Free allocated memory through AL4SAN_Runtime_malloc() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void
AL4SAN_Parsec_free( void *ptr, size_t size );
/**
 * @brief OpenMP Free allocated memory through AL4SAN_Runtime_malloc() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void
AL4SAN_Openmp_free( void *ptr, size_t size );
/**
 * @}
 *
 * @name RUNTIME Descriptor functions
 * @{
 ***
 * @brief Initialize runtime specific data structure to a given descriptor.
 *
 * @param[in,out] desc
 *            The descriptor to initialize.
 */
void
AL4SAN_Quark_desc_create( AL4SAN_desc_t *desc );
void
AL4SAN_Starpu_desc_create( AL4SAN_desc_t *desc );
void
AL4SAN_Parsec_desc_create( AL4SAN_desc_t *desc );
void
AL4SAN_Openmp_desc_create( AL4SAN_desc_t *desc );
/**
 * @brief Finalize runtime specific data structure of a given descriptor.
 *
 * @param[in,out] desc
 *            The descriptor to finalize.
 */
void
AL4SAN_Quark_desc_destroy( AL4SAN_desc_t *desc );
void
AL4SAN_Starpu_desc_destroy( AL4SAN_desc_t *desc );
void
AL4SAN_Parsec_desc_destroy( AL4SAN_desc_t *desc );
void
AL4SAN_Openmp_desc_destroy( AL4SAN_desc_t *desc );
/**
 * @brief Acquire in main memory an up-to-date copy of the data described by the
 * descriptor for read-write access.
 *
 * The application must call this function prior to accessing registered data
 * from main memory outside tasks. RUNTIME_desc_release() must be called once
 * the application does not need to access the data anymore.
 *
 * @param[in] desc
 *            The descriptor to acquire.
 *
 * @retval CHAMELEON_SUCCESS on success
 */
int
AL4SAN_Quark_desc_acquire( const AL4SAN_desc_t *desc );
int
AL4SAN_Starpu_desc_acquire( const AL4SAN_desc_t *desc );
int
AL4SAN_Parsec_desc_acquire( const AL4SAN_desc_t *desc );
int
AL4SAN_Openmp_desc_acquire( const AL4SAN_desc_t *desc );
/**
 * @brief Release the data described by the descriptor to be used by the runtime
 * tasks again.
 *
 * This function releases the data acquired by the application either by
 * AL4SAN_Runtime_desc_acquire() or by AL4SAN_Runtime_desc_acquire_async() to the runtime.
 *
 * @param[in] desc
 *            The descriptor to release.
 *
 * @retval CHAMELEON_SUCCESS on success
 */
int
AL4SAN_Quark_desc_release( const AL4SAN_desc_t *desc );
int
AL4SAN_Starpu_desc_release( const AL4SAN_desc_t *desc );
int
AL4SAN_Parsec_desc_release( const AL4SAN_desc_t *desc );
int
AL4SAN_Openmp_desc_release( const AL4SAN_desc_t *desc );
/**
 * @brief Flush all pieces of data from a descriptor.
 *
 * This function marks all pieces of data from the descriptor as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] desc
 *            The descriptor to flush.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 */
void
AL4SAN_Quark_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence );
void
AL4SAN_Starpu_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence );
void
AL4SAN_Parsec_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence );
void
AL4SAN_Openmp_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence );
/**
 * @brief Flush all data submitted to the runtime systems from the distributed
 * cache, and/or mark for eviction from the GPU memory.
 *
 * This function flushes all data from the distributed cache of the runtime system.
 */
void
AL4SAN_Quark_flush( );
void
AL4SAN_Starpu_flush( );
void
AL4SAN_Parsec_flush( );
void
AL4SAN_Openmp_flush( );
/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 */
void
AL4SAN_Quark_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Starpu_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Parsec_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Openmp_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 */
void
AL4SAN_Quark_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Starpu_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Parsec_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
void
AL4SAN_Openmp_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An );
/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The coordinate of the piece of data in the vector
 */
void
AL4SAN_Quark_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am);
void
AL4SAN_Starpu_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am);
void
AL4SAN_Parsec_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am);
void
AL4SAN_Openmp_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am);
/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 */
void
AL4SAN_Quark_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A);
void
AL4SAN_Starpu_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A);
void
AL4SAN_Parsec_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A);
void
AL4SAN_Openmp_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A);

#if defined(AL4SAN_USE_MIGRATE)
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Quark_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Starpu_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Parsec_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Openmp_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Quark_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Starpu_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Parsec_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void
AL4SAN_Openmp_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank );
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Quark_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank );
void
AL4SAN_Starpu_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank );
void
AL4SAN_Parsec_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank );
void
AL4SAN_Openmp_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank );
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Quark_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank );
void
AL4SAN_Starpu_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank );
void
AL4SAN_Parsec_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank );
void
AL4SAN_Openmp_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank );
#else
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
static inline void
AL4SAN_Quark_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Starpu_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Parsec_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Openmp_data_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
static inline void
AL4SAN_Quark_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Starpu_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Parsec_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
static inline void
AL4SAN_Openmp_matrix_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
static inline void
AL4SAN_Quark_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}
static inline void
AL4SAN_Starpu_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}
static inline void
AL4SAN_Parsec_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}
static inline void
AL4SAN_Openmp_vector_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}
/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
static inline void
AL4SAN_Quark_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank ) {
    (void)sequence; (void)A; (void)new_rank;
}
static inline void
AL4SAN_Starpu_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank ) {
    (void)sequence; (void)A; (void)new_rank;
}
static inline void
AL4SAN_Parsec_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank ) {
    (void)sequence; (void)A; (void)new_rank;
}
static inline void
AL4SAN_Openmp_scaler_migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int new_rank ) {
    (void)sequence; (void)A; (void)new_rank;
}
#endif

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @retval The runtime handler address of the piece of data.
 */
void *
AL4SAN_Quark_data_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Starpu_data_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Parsec_data_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Openmp_data_getaddr( const AL4SAN_desc_t *A, int Am, int An );




/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in out] ptr
 *        Pointer to retrieved data
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @retval The runtime handler address of the piece of data.
 */

void AL4SAN_Quark_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);
void AL4SAN_Starpu_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);
void AL4SAN_Parsec_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);
void AL4SAN_Openmp_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @retval The runtime handler address of the piece of data.
 */
void *
AL4SAN_Quark_matrix_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Starpu_matrix_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Parsec_matrix_getaddr( const AL4SAN_desc_t *A, int Am, int An );
void *
AL4SAN_Openmp_matrix_getaddr( const AL4SAN_desc_t *A, int Am, int An );
/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @retval The runtime handler address of the piece of data.
 */
void *
AL4SAN_Quark_vector_getaddr( const AL4SAN_desc_t *A, int Am);
void *
AL4SAN_Starpu_vector_getaddr( const AL4SAN_desc_t *A, int Am);
void *
AL4SAN_Parsec_vector_getaddr( const AL4SAN_desc_t *A, int Am);
void *
AL4SAN_Openmp_vector_getaddr( const AL4SAN_desc_t *A, int Am);
/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @retval The runtime handler address of the piece of data.
 */
void *
AL4SAN_Quark_scaler_getaddr( const AL4SAN_desc_t *A);
void *
AL4SAN_Starpu_scaler_getaddr( const AL4SAN_desc_t *A);
void *
AL4SAN_Parsec_scaler_getaddr( const AL4SAN_desc_t *A);
void *
AL4SAN_Openmp_scaler_getaddr( const AL4SAN_desc_t *A);
/**
 * @}
 *
 * @name RUNTIME Insert task options management
 * @{
 */
/**
 * @brief QUARK init option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] sequence structure
 *
 * @param[in, out] request structure 
 *
 */
void  AL4SAN_Quark_options_init     (AL4SAN_option_t*, AL4SAN_context_t*, AL4SAN_sequence_t*, AL4SAN_request_t*);
/**
 * @brief StarPU init option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] sequence structure
 *
 * @param[in, out] request structure
 *
 */
void  AL4SAN_Starpu_options_init     (AL4SAN_option_t*, AL4SAN_context_t*, AL4SAN_sequence_t*, AL4SAN_request_t*);
/**
 * @brief PaRSEC init option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] sequence structure
 *
 * @param[in, out] request structure
 *
 */
void  AL4SAN_Parsec_options_init     (AL4SAN_option_t*, AL4SAN_context_t*, AL4SAN_sequence_t*, AL4SAN_request_t*);
/**
 * @brief OpenMP init option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] sequence structure
 *
 * @param[in, out] request structure
 *
 */
void  AL4SAN_Openmp_options_init     (AL4SAN_option_t*, AL4SAN_context_t*, AL4SAN_sequence_t*, AL4SAN_request_t*);

/**
 * @brief QUARK finialize option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] context structure
 *
 */
void  AL4SAN_Quark_options_finalize (AL4SAN_option_t*, AL4SAN_context_t *);
/**
 * @brief StarPU finialize option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] context structure
 *
 */
void  AL4SAN_Starpu_options_finalize (AL4SAN_option_t*, AL4SAN_context_t *);
/**
 * @brief PaRSEC finialize option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] context structure
 *
 */
void  AL4SAN_Parsec_options_finalize (AL4SAN_option_t*, AL4SAN_context_t *);
/**
 * @brief OpenMP finialize option structure
 *
 * @param[in,out] option structure
 *
 * @param[in, out] context structure
 *
 */
void  AL4SAN_Openmp_options_finalize (AL4SAN_option_t*, AL4SAN_context_t *);

int   AL4SAN_Quark_options_ws_alloc (AL4SAN_option_t*, size_t, size_t);
int   AL4SAN_Starpu_options_ws_alloc (AL4SAN_option_t*, size_t, size_t);
int   AL4SAN_Parsec_options_ws_alloc (AL4SAN_option_t*, size_t, size_t);
int   AL4SAN_Openmp_options_ws_alloc (AL4SAN_option_t*, size_t, size_t);

int   AL4SAN_Quark_options_ws_free  (AL4SAN_option_t*);
int   AL4SAN_Starpu_options_ws_free  (AL4SAN_option_t*);
int   AL4SAN_Parsec_options_ws_free  (AL4SAN_option_t*);
int   AL4SAN_Openmp_options_ws_free  (AL4SAN_option_t*);

/**
 * @brief QUARK insert task routines using AL4SAN_Runtime_insert_task() 
 *
 * @param[in,out] codelet
 *            The ptr to function or codelet structure that contains main kernal info. 
 *
 * @param[in] options
 *            The ptr  to global runtime option.
 *
 * @param[in] varg_list
 *            The varg_list to task information which conatin dependency direction, ptr or value, and the size in bytes.
 */

int AL4SAN_Quark_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list);
/**
 * @brief StarPU insert task routines using AL4SAN_Runtime_insert_task()
 *
 * @param[in,out] codelet
 *            The ptr to function or codelet structure that contains main kernal info.
 *
 * @param[in] options
 *            The ptr  to global runtime option.
 *
 * @param[in] varg_list
 *            The varg_list to task information which conatin dependency direction, ptr or value, and the size in bytes.
 */
int AL4SAN_Starpu_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list);
/**
 * @brief PaRSEC insert task routines using AL4SAN_Runtime_insert_task()
 *
 * @param[in,out] codelet
 *            The ptr to function or codelet structure that contains main kernal info.
 *
 * @param[in] options
 *            The ptr  to global runtime option.
 *
 * @param[in] varg_list
 *            The varg_list to task information which conatin dependency direction, ptr or value, and the size in bytes.
 */
int AL4SAN_Parsec_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list);
/**
 * @brief OpenMP insert task routines using AL4SAN_Runtime_insert_task()
 *
 * @param[in,out] codelet
 *            The ptr to function or codelet structure that contains main kernal info.
 *
 * @param[in] options
 *            The ptr  to global runtime option.
 *
 * @param[in] varg_list
 *            The varg_list to task information which conatin dependency direction, ptr or value, and the size in bytes.
 */
int AL4SAN_Openmp_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list);

/**
 * @brief QUARK unpack arguments  using AL4SAN_Runtime_unpack_arg() 
 *
 * @param[in,out] args
 *            The args contains reference to list of arguments. 
 *
 * @param[in] varg_list
 *            The varg_list contains list of varabile which points the value being unpacked.
 */

int AL4SAN_Quark_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list);
/**
 * @brief StarPU unpack arguments  using AL4SAN_Runtime_unpack_arg()
 *
 * @param[in,out] args
 *            The args contains reference to list of arguments.
 *
 * @param[in] varg_list
 *            The varg_list contains list of varabile which points the value being unpacked.
 */
int AL4SAN_Starpu_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list);
/**
 * @brief PaRSEC unpack arguments  using AL4SAN_Runtime_unpack_arg()
 *
 * @param[in,out] args
 *            The args contains reference to list of arguments.
 *
 * @param[in] varg_list
 *            The varg_list contains list of varabile which points the value being unpacked.
 */
int AL4SAN_Parsec_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list);
/**
 * @brief OpenMP unpack arguments  using AL4SAN_Runtime_unpack_arg()
 *
 * @param[in,out] args
 *            The args contains reference to list of arguments.
 *
 * @param[in] varg_list
 *            The varg_list contains list of varabile which points the value being unpacked.
 */
int AL4SAN_Openmp_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list);

/**
 * @brief QUARK initialize task's options
 *
 */
void AL4SAN_Quark_task_option_init();
/**
 * @brief StarPU initialize task's options
 *
 */
void AL4SAN_Starpu_task_option_init();
/**
 * @brief PaRSEC initialize task's options
 *
 */
void AL4SAN_Parsec_task_option_init();
/**
 * @brief OpenMP initialize task's options
 *
 */
void AL4SAN_Openmp_task_option_init();

/**
 * @}
 */

END_C_DECLS

#endif /* _AL4SAN_AL4SAN_Runtime_H_ */
