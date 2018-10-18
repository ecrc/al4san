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
 * author Mathieu Faverge
 * author Cedric Augonnet
 * author Cedric Castagnede
 * date 2011-06-01
 *
 */

/**
   *
   * @brief AL4SAN runtimes API
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
  **/
#ifndef _AL4SAN_AL4SAN_Runtime_H_
#define _AL4SAN_AL4SAN_Runtime_H_

#include "al4san/config.h"
#include "al4san/struct.h"

BEGIN_C_DECLS

/**
 * @name RUNTIME Context functions
 * @{
 *    These functions manipulate the context data structure before the runtime is
 *    started and after the runtime is stopped.
 */

/**
 * @brief Create the runtime specific options in the context before starting it
 *
 * @param[in,out] ctxt
 *            The Al4san context to initialize for the runtime.
 */
void
AL4SAN_Runtime_context_create( AL4SAN_context_t *ctxt );

/**
 * @brief Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Al4san context in which the runtime specific options must
 *            be destroyed
 */
void
AL4SAN_Runtime_context_destroy( AL4SAN_context_t *ctxt );

/**
 * @brief Enable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Enable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
AL4SAN_Runtime_enable( AL4SAN_enum option );

/**
 * @brief Disable a global option of the runtime.
 * @warning Should be called only by AL4SAN_Disable()
 *
 * @param[in] option
 *            @arg AL4SAN_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
AL4SAN_Runtime_disable( AL4SAN_enum option );

/**
 * @}
 *
 * @name RUNTIME Control functions
 * @{
 *   These functions control the global behavior of the runtime.
 */

/**
 * @brief Initialize the scheduler with the given parameters
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
AL4SAN_Runtime_init( AL4SAN_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );

/**
 * @brief Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Al4san context for which the runtime system must be shut down.
 */
void
AL4SAN_Runtime_finalize( AL4SAN_context_t *ctxt );

/**
 * @brief Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the suspension must be made.
 */
void
AL4SAN_Runtime_pause( AL4SAN_context_t *ctxt );

/**
 * @brief Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Al4san context for which the execution must be resumed.
 */
void
AL4SAN_Runtime_resume( AL4SAN_context_t *ctxt );

/**
 * @brief Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context in which the task completion is performed.
 */
void
AL4SAN_Runtime_barrier( AL4SAN_context_t *ctxt );

/**
 * @brief Show the progress of the computations when enabled.
 *
 * @param[in] ctxt
 *            The Al4san context for which the context needs to be printed.
 */
void
AL4SAN_Runtime_progress( AL4SAN_context_t *ctxt );

/**
 * @brief Get the rank of the current worker for the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the thread rank is asked.
 *
 * @retval The rank of the current thread in the runtime.
 */
int
AL4SAN_Runtime_thread_rank( AL4SAN_context_t *ctxt );

/**
 * @brief Get the number of CPU workers of the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the number of workers is requested
 *
 * @retval The number of threads currently used by the runtime.
 */
int
AL4SAN_Runtime_thread_size( AL4SAN_context_t *ctxt );

/**
 * @brief Get the MPI comm rank of the current process related to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the rank is asked.
 *
 * @retval The rank of the process in the communicator known by the runtime.
 */
int
AL4SAN_Runtime_comm_rank( AL4SAN_context_t *ctxt );

/**
 * @brief Get the MPI comm size related to the runtime.
 *
 * @param[in] ctxt
 *            The Al4san context for which the communicator size is asked.
 *
 * @retval The size of the communicator known by the runtime.
 */
int
AL4SAN_Runtime_comm_size( AL4SAN_context_t *ctxt );

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
AL4SAN_Runtime_comm_set_tag_sizes( int user_tag_width,
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
AL4SAN_Runtime_sequence_create( AL4SAN_context_t  *ctxt, AL4SAN_sequence_t* sequence);

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
AL4SAN_Runtime_sequence_destroy( AL4SAN_context_t  *ctxt,
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
AL4SAN_Runtime_sequence_wait( AL4SAN_context_t  *ctxt,
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
AL4SAN_Runtime_sequence_flush( AL4SAN_context_t  *ctxt,
                        AL4SAN_sequence_t *sequence,
                        AL4SAN_request_t  *request,
                        int               status );

/**
 * @}
 *
 * @name RUNTIME Insert task options management
 * @{
 */
void  AL4SAN_Runtime_options_init     (AL4SAN_option_t*, AL4SAN_context_t*, AL4SAN_sequence_t*, AL4SAN_request_t*);
void  AL4SAN_Runtime_options_finalize (AL4SAN_option_t*, AL4SAN_context_t *);
int   AL4SAN_Runtime_options_ws_alloc (AL4SAN_option_t*, size_t, size_t);
int   AL4SAN_Runtime_options_ws_free  (AL4SAN_option_t*);

/**
 * @brief insert task routines using AL4SAN_Runtime_insert_task() 
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

int AL4SAN_Runtime_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list);

/**
 * @brief unpack arguments  using AL4SAN_Runtime_unpack_arg() 
 *
 * @param[in,out] args
 *            The args contains reference to list of arguments. 
 *
 * @param[in] varg_list
 *            The varg_list contains list of varabile which points the value being unpacked.
 */

int AL4SAN_Runtime_unpack_arg(AL4SAN_arg args, va_list varg_list);

/**
 * @}
 */

END_C_DECLS

#endif /* _AL4SAN_AL4SAN_Runtime_H_ */
