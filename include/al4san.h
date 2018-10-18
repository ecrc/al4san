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
 * author Mathieu Faverge
 * author Cedric Augonnet
 * author Cedric Castagnede
 * author Florent Pruvost
 * date 2012-09-15
 *
 */
/**
   *
   * @brief AL4SAN main header
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
  **/
#ifndef _AL4SAN_H_
#define _AL4SAN_H_

/* ****************************************************************************
 * AL4SAN types and constants
 */
#include "al4san/config.h"
#include "al4san/types.h"
#include "al4san/struct.h"
#include "al4san/constants.h"


/* ****************************************************************************
 * AL4SAN runtime common API
 */
#include "al4san/runtime.h"


/* ****************************************************************************
 * AL4SAN Functions
 */
BEGIN_C_DECLS

/* Auxiliary */
int AL4SAN_Version           (int *ver_major, int *ver_minor, int *ver_micro);
int AL4SAN_My_Mpi_Rank       (void);
AL4SAN_context_t* AL4SAN_Init              (int nworkers, int ncudas);
AL4SAN_context_t* AL4SAN_InitPar           (int nworkers, int ncudas, int nthreads_per_worker);
int AL4SAN_Finalize          (void);
int AL4SAN_Pause             (void);
int AL4SAN_Resume            (void);
int AL4SAN_Distributed_start (void);
int AL4SAN_Distributed_stop  (void);
int AL4SAN_Comm_size         (void);
int AL4SAN_Comm_rank         (void);
int AL4SAN_Distributed_start (void);
int AL4SAN_Distributed_stop  (void);
int AL4SAN_Distributed_size  (int *size);
int AL4SAN_Distributed_rank  (int *rank);
int AL4SAN_GetThreadNbr      (void);

//typedef void al4san_func;

/*
    * Insert Task function: Define Task and it is dependencies
    *  @param[in] First argument AL4SAN_TASK macro with task name
    *  @param[in] options argument which holds sequence data sturcture
    *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
 */


int AL4SAN_Insert_Task(AL4SAN_codelet codelet,  AL4SAN_option_t *options,...);

/*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

int AL4SAN_Unpack_Arg(AL4SAN_arg args, ...);

/* Descriptor */
int AL4SAN_Element_Size(int type);
void AL4SAN_user_tag_size(int, int) ;


/* Options */
int AL4SAN_Enable  (AL4SAN_enum option);
int AL4SAN_Disable (AL4SAN_enum option);
int AL4SAN_Set     (AL4SAN_enum param, int  value);
int AL4SAN_Get     (AL4SAN_enum param, int *value);
int AL4SAN_Set_update_progress_callback(void (*p)(int, int)) ;


/* Sequences */
#if defined (AL4SAN_SCHED_OPENMP)
#define PRAGMA(x) _Pragma(#x)
#define AL4SAN_Sequence_Create()                                        \
         malloc(sizeof(AL4SAN_sequence_t));                              \
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
