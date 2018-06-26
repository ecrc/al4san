/**
 *
 * @file altanal_runtime.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon runtimes API
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Augonnet
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 */
#ifndef _ALTANAL_ALTANAL_Runtime_H_
#define _ALTANAL_ALTANAL_Runtime_H_

#include "altanal/altanal_config.h"
#include "altanal/altanal_struct.h"

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
 *            The Altanal context to initialize for the runtime.
 */
void
ALTANAL_Runtime_context_create( ALTANAL_context_t *ctxt );

/**
 * @brief Destroy the specific options in the context after this last one has
 * been stop.
 *
 * @param[in,out] ctxt
 *            The Altanal context in which the runtime specific options must
 *            be destroyed
 */
void
ALTANAL_Runtime_context_destroy( ALTANAL_context_t *ctxt );

/**
 * @brief Enable a global option of the runtime.
 * @warning Should be called only by ALTANAL_Enable()
 *
 * @param[in] option
 *            @arg ALTANAL_PROFILING_MODE: start the profiling mode of the runtime.
 */
void
ALTANAL_Runtime_enable( ALTANAL_enum option );

/**
 * @brief Disable a global option of the runtime.
 * @warning Should be called only by ALTANAL_Disable()
 *
 * @param[in] option
 *            @arg ALTANAL_PROFILING_MODE: stop the profiling mode of the runtime.
 */
void
ALTANAL_Runtime_disable( ALTANAL_enum option );

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
 *            The Altanal context in which to initialize the runtime support.
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
ALTANAL_Runtime_init( ALTANAL_context_t *ctxt,
              int ncpus,
              int ncudas,
              int nthreads_per_worker );

/**
 * @brief Finalize the scheduler used for the computations.
 *
 * @param[in,out] ctxt
 *            The Altanal context for which the runtime system must be shut down.
 */
void
ALTANAL_Runtime_finalize( ALTANAL_context_t *ctxt );

/**
 * @brief Suspend the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Altanal context for which the suspension must be made.
 */
void
ALTANAL_Runtime_pause( ALTANAL_context_t *ctxt );

/**
 * @brief Resume the processing of new tasks submitted to the runtime system.
 *
 * @param[in] ctxt
 *            The Altanal context for which the execution must be resumed.
 */
void
ALTANAL_Runtime_resume( ALTANAL_context_t *ctxt );

/**
 * @brief Wait for completion of all tasks submitted to the runtime.
 *
 * @param[in] ctxt
 *            The Altanal context in which the task completion is performed.
 */
void
ALTANAL_Runtime_barrier( ALTANAL_context_t *ctxt );

/**
 * @brief Show the progress of the computations when enabled.
 *
 * @param[in] ctxt
 *            The Altanal context for which the context needs to be printed.
 */
void
ALTANAL_Runtime_progress( ALTANAL_context_t *ctxt );

/**
 * @brief Get the rank of the current worker for the runtime.
 *
 * @param[in] ctxt
 *            The Altanal context for which the thread rank is asked.
 *
 * @retval The rank of the current thread in the runtime.
 */
int
ALTANAL_Runtime_thread_rank( ALTANAL_context_t *ctxt );

/**
 * @brief Get the number of CPU workers of the runtime.
 *
 * @param[in] ctxt
 *            The Altanal context for which the number of workers is requested
 *
 * @retval The number of threads currently used by the runtime.
 */
int
ALTANAL_Runtime_thread_size( ALTANAL_context_t *ctxt );

/**
 * @brief Get the MPI comm rank of the current process related to the runtime.
 *
 * @param[in] ctxt
 *            The Altanal context for which the rank is asked.
 *
 * @retval The rank of the process in the communicator known by the runtime.
 */
int
ALTANAL_Runtime_comm_rank( ALTANAL_context_t *ctxt );

/**
 * @brief Get the MPI comm size related to the runtime.
 *
 * @param[in] ctxt
 *            The Altanal context for which the communicator size is asked.
 *
 * @retval The size of the communicator known by the runtime.
 */
int
ALTANAL_Runtime_comm_size( ALTANAL_context_t *ctxt );

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
ALTANAL_Runtime_comm_set_tag_sizes( int user_tag_width,
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
 *            The Altanal context in which the sequence is created.
 *
 * @param[in,out] sequence
 *            On entry the allocated Altanal sequence structure, and on exit
 *            the scheduler specifics for the sequence have been initialized.
 *
 * @retval ALTANAL_SUCCESS on success.
 * @retval ALTANAL_ERR_OUT_OF_RESOURCES, if the sequence could not be created.
 */
int
ALTANAL_Runtime_sequence_create( ALTANAL_context_t  *ctxt,
                         ALTANAL_sequence_t *sequence );

/**
 * @brief Destroy the sequence structure.
 *
 * @param[in] ctxt
 *            The Altanal context to which the sequence belongs.
 *
 * @param[in,out] sequence
 *            On entry the sequence structure.
 *            On exit, the scheduler specifics of the sequence have been
 *            destroyed.
 *
 * @retval ALTANAL_SUCCESS on success.
 */
int
ALTANAL_Runtime_sequence_destroy( ALTANAL_context_t  *ctxt,
                          ALTANAL_sequence_t *sequence);

/**
 * @brief Wait for completion of all tasks in the given sequence.
 *
 * @param[in] ctxt
 *            The Altanal context to which the sequence belongs.
 *
 * @param[in] sequence
 *            The sequence that gathers a set of tasks to complete.
 *
 * @retval ALTANAL_SUCCESS on success.
 * @retval The algorithm return code on failure.
 */
int
ALTANAL_Runtime_sequence_wait( ALTANAL_context_t  *ctxt,
                       ALTANAL_sequence_t *sequence );

/**
 * @brief Cancel all remaining tasks in the given sequence.
 *
 * @param[in] ctxt
 *            The Altanal context to which the sequence belongs.
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
ALTANAL_Runtime_sequence_flush( ALTANAL_context_t  *ctxt,
                        ALTANAL_sequence_t *sequence,
                        ALTANAL_request_t  *request,
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
ALTANAL_Runtime_malloc( size_t size );

/**
 * @brief Free allocated memory through ALTANAL_Runtime_malloc() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void
ALTANAL_Runtime_free( void *ptr, size_t size );

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
//void
//ALTANAL_Runtime_desc_create( ALTANAL_desc_t *desc );

/**
 * @brief Finalize runtime specific data structure of a given descriptor.
 *
 * @param[in,out] desc
 *            The descriptor to finalize.
 */
//void
//ALTANAL_Runtime_desc_destroy( ALTANAL_desc_t *desc );

/**
 * @brief Acquire in main memory an up-to-date copy of the data described by the
 * descriptor for read-write access.
 *
 * The application must call this function prior to accessing registered data
 * from main memory outside tasks. ALTANAL_Runtime_desc_release() must be called once
 * the application does not need to access the data anymore.
 *
 * @param[in] desc
 *            The descriptor to acquire.
 *
 * @retval ALTANAL_SUCCESS on success
 */
//int
//ALTANAL_Runtime_desc_acquire( const ALTANAL_desc_t *desc );

/**
 * @brief Release the data described by the descriptor to be used by the runtime
 * tasks again.
 *
 * This function releases the data acquired by the application either by
 * ALTANAL_Runtime_desc_acquire() or by ALTANAL_Runtime_desc_acquire_async() to the runtime.
 *
 * @param[in] desc
 *            The descriptor to release.
 *
 * @retval ALTANAL_SUCCESS on success
 */
//int
//ALTANAL_Runtime_desc_release( const ALTANAL_desc_t *desc );

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
//void
//ALTANAL_Runtime_desc_flush( const ALTANAL_desc_t     *desc,
//                    const ALTANAL_sequence_t *sequence );

/**
 * @brief Flush all data submitted to the runtime systems from the distributed
 * cache, and/or mark for eviction from the GPU memory.
 *
 * This function flushes all data from the distributed cache of the runtime system.
 */
void
ALTANAL_Runtime_flush( );

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
//void
//ALTANAL_Runtime_data_flush( const ALTANAL_sequence_t *sequence,
//                    const ALTANAL_desc_t *A, int Am, int An );

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
/*#if defined(ALTANAL_USE_MIGRATE)
void
ALTANAL_Runtime_data_migrate( const ALTANAL_sequence_t *sequence,
                      const ALTANAL_desc_t *A, int Am, int An, int new_rank );
#else
static inline void
ALTANAL_Runtime_data_migrate( const ALTANAL_sequence_t *sequence,
                      const ALTANAL_desc_t *A, int Am, int An, int new_rank ) {
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
#endif*/

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
//void *
//ALTANAL_Runtime_data_getaddr( const ALTANAL_desc_t *A, int Am, int An );

/**
 * @}
 *
 * @name RUNTIME Insert task options management
 * @{
 */
void  ALTANAL_Runtime_options_init     (ALTANAL_option_t*, ALTANAL_context_t*, ALTANAL_sequence_t*, ALTANAL_request_t*);
void  ALTANAL_Runtime_options_finalize (ALTANAL_option_t*, ALTANAL_context_t *);
int   ALTANAL_Runtime_options_ws_alloc (ALTANAL_option_t*, size_t, size_t);
int   ALTANAL_Runtime_options_ws_free  (ALTANAL_option_t*);
/* int   ALTANAL_Runtime_options_ws_gethost   (ALTANAL_option_t*); */
/* int   ALTANAL_Runtime_options_ws_getdevice (ALTANAL_option_t*); */

/**
 * @}
 *
 * @name RUNTIME Kernel locality management
 * @{
 */
void ALTANAL_Runtime_zlocality_allrestore ();
void ALTANAL_Runtime_clocality_allrestore ();
void ALTANAL_Runtime_dlocality_allrestore ();
void ALTANAL_Runtime_slocality_allrestore ();

void ALTANAL_Runtime_zlocality_allrestrict(uint32_t);
void ALTANAL_Runtime_zlocality_onerestrict(ALTANAL_kernel_t, uint32_t);
void ALTANAL_Runtime_zlocality_onerestore (ALTANAL_kernel_t);

void ALTANAL_Runtime_clocality_allrestrict(uint32_t);
void ALTANAL_Runtime_clocality_onerestrict(ALTANAL_kernel_t, uint32_t);
void ALTANAL_Runtime_clocality_onerestore (ALTANAL_kernel_t);

void ALTANAL_Runtime_dlocality_allrestrict(uint32_t);
void ALTANAL_Runtime_dlocality_onerestrict(ALTANAL_kernel_t, uint32_t);
void ALTANAL_Runtime_dlocality_onerestore (ALTANAL_kernel_t);

void ALTANAL_Runtime_slocality_allrestrict(uint32_t);
void ALTANAL_Runtime_slocality_onerestrict(ALTANAL_kernel_t, uint32_t);
void ALTANAL_Runtime_slocality_onerestore (ALTANAL_kernel_t);

/**
 * @}
 *
 * @name RUNTIME Profiling
 * @{
 */
void   ALTANAL_Runtime_schedprofile_display ();
void   ALTANAL_Runtime_kernelprofile_display();
double ALTANAL_Runtime_get_time();

void  ALTANAL_Runtime_iteration_push     (ALTANAL_context_t*, unsigned long iteration);
void  ALTANAL_Runtime_iteration_pop      (ALTANAL_context_t*);

void ALTANAL_Runtime_start_profiling();
void ALTANAL_Runtime_stop_profiling();

void ALTANAL_Runtime_start_stats();
void ALTANAL_Runtime_stop_stats();

void ALTANAL_Runtime_zdisplay_allprofile ();
void ALTANAL_Runtime_zdisplay_oneprofile (ALTANAL_kernel_t);
void ALTANAL_Runtime_cdisplay_allprofile ();
void ALTANAL_Runtime_cdisplay_oneprofile (ALTANAL_kernel_t);
void ALTANAL_Runtime_ddisplay_allprofile ();
void ALTANAL_Runtime_ddisplay_oneprofile (ALTANAL_kernel_t);
void ALTANAL_Runtime_sdisplay_allprofile ();
void ALTANAL_Runtime_sdisplay_oneprofile (ALTANAL_kernel_t);

int ALTANAL_Runtime_insert_task(ALTANAL_codelet codelet,  ALTANAL_option_t *options, va_list varg_list);
int ALTANAL_Runtime_unpack_arg(ALTANAL_arg args, va_list varg_list);

/**
 * @}
 */

END_C_DECLS

#endif /* _ALTANAL_ALTANAL_Runtime_H_ */
