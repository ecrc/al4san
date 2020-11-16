/**
 *
 * @file al4san.h
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
 * @brief AL4SAN main header for the high level API
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *@author Mathieu Faverge
 *@author Cedric Augonnet
 *@author Cedric Castagnede
 *@author Florent Pruvost
 *@date 2012-09-15
 *@version 1.1.0
 *@author Rabab Alomairy
 *@date 2019-02-06
 *
 **/
#ifndef _AL4SAN_H_
#define _AL4SAN_H_

#include "al4san/config.h"
#include "al4san/types.h"
#include "al4san/struct.h"
#include "al4san/constants.h"
#include "al4san/task.h"
#include "al4san/runtime.h"

BEGIN_C_DECLS

/* Auxiliary */

/**
 * @name AL4SAN Auxiliary functions
 * @{
 *    These functions ensure a reliable and robust interface.
 */

int AL4SAN_Version           (int *ver_major, int *ver_minor, int *ver_micro);
int AL4SAN_My_Mpi_Rank       (void);
/**
 * @ingroup Auxiliary
 *  Indicates a recoverable problem.
 *  User's erroneous action without severe consequences.
 *  Problems occuring while AL4SAN is being used correctly.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void AL4SAN_Warning      (const char *func_name, const char* msg_text);
/**
 * @ingroup Auxiliary
 *  Indicates a recoverable problem.
 *  User's erroneous action with potentially severe consequences.
 *  Problems occuring due to incorrect use of AL4SAN.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */

void AL4SAN_Error        (const char *func_name, const char* msg_text);
/**
 * @ingroup Auxiliary
 *  Unexpected behavior within the library.
 *  Unrecoverable user errors.
 *  Context oblivious.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void AL4SAN_Fatal_Error  (const char *func_name, const char* msg_text);
int AL4SAN_Element_Size(int type);
/**
 * @name AL4SAN Context functions
 * @{
 *    These functions manipulate and control the context data structure before the runtime is
 *    started and after the runtime is stopped.
 */
AL4SAN_context_t* AL4SAN_Init              (char *runtime, int nworkers, int ncudas);
AL4SAN_context_t* AL4SAN_InitPar           (char *runtime, int nworkers, int ncudas, int nthreads_per_worker);
int AL4SAN_Finalize          (void);
int AL4SAN_Pause             (void);
int AL4SAN_Resume            (void);
int AL4SAN_Distributed_Start (void);
int AL4SAN_Distributed_Stop  (void);
int AL4SAN_Barrier           (void);
int AL4SAN_Progress          (void);
int AL4SAN_Thread_Rank       (void);
int AL4SAN_Thread_Size       (void);
int AL4SAN_Comm_Size         (void);
int AL4SAN_Comm_Rank         (void);
//int AL4SAN_Distributed_Size  (int *size);
//int AL4SAN_Distributed_Rank  (int *rank);
int AL4SAN_GetThreadNbr      (void);
int AL4SAN_Enable  (AL4SAN_enum option);
int AL4SAN_Disable (AL4SAN_enum option);
int AL4SAN_Set     (AL4SAN_enum param, int  value);
int AL4SAN_Get     (AL4SAN_enum param, int *value);
AL4SAN_context_t*  AL4SAN_Switch_Runtime(char *runtime, int ncpus, int ncudas);
void AL4SAN_Init_Processor_Grid(int p, int q);

/**
 * @name AL4SAN task functions
 * @{
 *    This group handling packing and unpacking routines.
 */
/*
    * Insert Task function: Define Task and it is dependencies
    *  @param[in] First argument AL4SAN_TASK macro with task name
    *  @param[in] options argument which holds sequence data sturcture
    *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
 */


int AL4SAN_Insert_Task(AL4SAN_codelet codelet_quark, AL4SAN_codelet codelet_starpu, AL4SAN_codelet codelet_parsec,  AL4SAN_codelet codelet_openmp, AL4SAN_option_t *options,...);
/*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

int AL4SAN_Unpack_Arg(AL4SAN_arg_list* al4san_arg, ...);

/**
 * @name AL4SAN descriptor functions
 * @{
 *    This group handling data interface routines.
 */
void AL4SAN_User_Tag_Size(int, int) ;
int AL4SAN_Desc_Create  (AL4SAN_desc_t **desc, void *mat, al4san_flttype_t dtyp,
                        int mb, int nb, int bsiz, int lm, int ln,
                        int i, int j, int m, int n, int p, int q);
int AL4SAN_Matrix_Create( AL4SAN_desc_t **descptr, void *mat, al4san_flttype_t dtyp, int order, int mb, int nb,
                           int bld, int lm, int ln, int ld);
int AL4SAN_Vector_Create( AL4SAN_desc_t **descptr, void *vec, al4san_flttype_t dtyp, int mb,
                           int lm);
int AL4SAN_Scaler_Create( AL4SAN_desc_t **descptr, void *scaler, al4san_flttype_t dtyp);
int AL4SAN_Desc_Create_User(AL4SAN_desc_t **desc, void *mat, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                           int lm, int ln, int i, int j, int m, int n, int p, int q,
                           void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                           int (*get_blkldd)( const AL4SAN_desc_t*, int ),
                           int (*get_rankof)( const AL4SAN_desc_t*, int, int ));
int AL4SAN_Desc_Create_OOC(AL4SAN_desc_t **desc, al4san_flttype_t dtyp,
                          int mb, int nb, int bsiz, int lm, int ln,
                          int i, int j, int m, int n, int p, int q);
int AL4SAN_Desc_Create_OOC_User(AL4SAN_desc_t **desc, al4san_flttype_t dtyp,
                               int mb, int nb, int bsiz, int lm, int ln,
                               int i, int j, int m, int n, int p, int q,
                               int (*get_rankof)( const AL4SAN_desc_t*, int, int ));
int AL4SAN_Desc_Destroy (AL4SAN_desc_t **desc);
//void AL4SAN_Malloc(void**A, size_t size);
void AL4SAN_Free( void  *ptr, size_t size );
int AL4SAN_Desc_Acquire (AL4SAN_desc_t  *desc);
int AL4SAN_Desc_Release (AL4SAN_desc_t  *desc);
void AL4SAN_Flush();
int AL4SAN_Desc_Flush   (AL4SAN_desc_t  *desc, AL4SAN_sequence_t *sequence);
void AL4SAN_Data_Flush( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int An );
void AL4SAN_Matrix_Flush( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int An );
void AL4SAN_Vector_Flush( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am);
void AL4SAN_Scaler_Flush( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A);
void AL4SAN_Data_Migrate( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void AL4SAN_Matrix_Migrate( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int An, int new_rank );
void AL4SAN_Vector_Migrate( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int new_rank );
void AL4SAN_Scaler_Migrate( const AL4SAN_sequence_t *sequence, const AL4SAN_desc_t *A, int Am, int new_rank );
void AL4SAN_Data_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);
void AL4SAN_Matrix_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m, int n);
void AL4SAN_Vector_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m);
void AL4SAN_Scaler_Getaddr( const AL4SAN_desc_t *desc, void **ptr);


void *AL4SAN_Data_getaddr( const AL4SAN_desc_t *desc, int m, int n);
void *AL4SAN_Matrix_getaddr( const AL4SAN_desc_t *desc, int m, int n);
void *AL4SAN_Vector_getaddr( const AL4SAN_desc_t *desc, int m);
void *AL4SAN_Scaler_getaddr( const AL4SAN_desc_t *desc);

int AL4SAN_Data_Getrank( const AL4SAN_desc_t *desc, int m, int n);
int AL4SAN_Matrix_Getrank( const AL4SAN_desc_t *desc, int m, int n);
int AL4SAN_Vector_Getrank( const AL4SAN_desc_t *desc, int m);
int AL4SAN_Scaler_Getrank( const AL4SAN_desc_t *desc, void **ptr);

void AL4SAN_Broadcast(AL4SAN_sequence_t *sequence, int myrank,
                        int root, void* tile_root, int arena_index,
                        int *dest_ranks, int dest_rank_idx);

void AL4SAN_Broadcast_ID(int32_t bcast_id, AL4SAN_sequence_t *sequence, int myrank,
                        int root, void* tile_root, int arena_index,
                        int *dest_ranks, int dest_rank_idx);
/**
 * @name AL4SAN options functions
 * @{
 *    This group handling tasks routines.
 */
int AL4SAN_Options_Init(AL4SAN_option_t *options, AL4SAN_sequence_t *sequence, AL4SAN_request_t *request);
int AL4SAN_Options_Finalize(AL4SAN_option_t *options);
int AL4SAN_Options_Workspace_Alloc( AL4SAN_option_t *options, size_t worker_size, size_t host_size );
int AL4SAN_Options_Workspace_Free(AL4SAN_option_t *options );


/**
 * @name AL4SAN sequence functions
 * @{
 *    This group handling stream of tasks.
 */
#if defined (AL4SAN_SCHED_OPENMP)
#define PRAGMA(x) _Pragma(#x)
#define AL4SAN_Sequence_Create()                                        \
         malloc(sizeof(AL4SAN_sequence_t));                              \
              (sequence)->status = AL4SAN_SUCCESS;                        \
        PRAGMA(omp parallel)                                               \
         {                                                                  \
           PRAGMA(omp master)                                               \
            {                                                               

#define AL4SAN_Sequence_Destroy(sequence)                           \
          }                                                                 \
           }                                                                \
         free(sequence)  
#else

 /**
 * @brief Create a sequence structure associated to a given context.
 *
 * @retval sequence handel on success.
 */

AL4SAN_sequence_t * AL4SAN_Sequence_Create();

/**
 * @brief Destroy the sequence structure.
 *
 *
 * @param[in,out] sequence
 *            On entry the sequence structure.
 *            On exit, the scheduler specifics of the sequence have been
 *            destroyed.
 *
 * @retval AL4SAN_SUCCESS on success.
 */

int AL4SAN_Sequence_Destroy (AL4SAN_sequence_t *sequence);
#endif
int AL4SAN_Sequence_Wait    (AL4SAN_sequence_t *sequence);


/**
 * @brief Flush the sequence structure.
 *
 *
 * @param[in,out] sequence
 *            On entry the sequence structure.
 *            On exit, the scheduler specifics of the sequence have been
 *            destroyed.
 *
 * @retval AL4SAN_SUCCESS on success.
 */

int AL4SAN_Sequence_Flush(AL4SAN_sequence_t *sequence, AL4SAN_request_t *request);

/**
 * @brief Create Workspace.
 *
 *
 * @param[in,out] workspace
 * @param[in] lworkspace workspace length
 * @param[in] size in byte
 * @retval AL4SAN_SUCCESS on success.
 */

int AL4SAN_Workspace_Create(AL4SAN_workspace_t *workspace, size_t lworkspace, size_t dtyp);

/**
 * @brief Destroy Workspace.
 *
 *
 * @param[in,out] workspace
 * @retval AL4SAN_SUCCESS on success.
 */

int AL4SAN_Workspace_Destroy(AL4SAN_workspace_t *workspace);


END_C_DECLS

#endif
