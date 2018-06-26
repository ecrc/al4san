/**
 *
 * @file altanal.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon main header
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Augonnet
 * @author Cedric Castagnede
 * @author Florent Pruvost
 * @date 2012-09-15
 *
 */
#ifndef _ALTANAL_H_
#define _ALTANAL_H_

/* ****************************************************************************
 * ALTANAL types and constants
 */
#include "altanal/altanal_config.h"
#include "altanal/altanal_types.h"
#include "altanal/altanal_struct.h"
#include "altanal/altanal_constants.h"
#include "altanal/altanal_kernels.h"


/* ****************************************************************************
 * ALTANAL runtime common API
 */
#include "altanal/altanal_runtime.h"


/* ****************************************************************************
 * ALTANAL Simulation mode
 */
#include "altanal/altanal_simulate.h"


/* ****************************************************************************
 * ALTANAL Functions
 */
BEGIN_C_DECLS

/* Auxiliary */
int ALTANAL_Version           (int *ver_major, int *ver_minor, int *ver_micro);
int ALTANAL_My_Mpi_Rank       (void);
int ALTANAL_Init              (int nworkers, int ncudas);
int ALTANAL_InitPar           (int nworkers, int ncudas, int nthreads_per_worker);
int ALTANAL_Finalize          (void);
int ALTANAL_Pause             (void);
int ALTANAL_Resume            (void);
int ALTANAL_Distributed_start (void);
int ALTANAL_Distributed_stop  (void);
int ALTANAL_Comm_size         (void);
int ALTANAL_Comm_rank         (void);
int ALTANAL_Distributed_start (void);
int ALTANAL_Distributed_stop  (void);
int ALTANAL_Distributed_size  (int *size);
int ALTANAL_Distributed_rank  (int *rank);
int ALTANAL_GetThreadNbr      (void);

//typedef void altanal_func;
int ALTANAL_Insert_Task(ALTANAL_codelet codelet,  ALTANAL_option_t *options,...);
int ALTANAL_Unpack_Arg(ALTANAL_arg args, ...);

/* Descriptor */
int ALTANAL_Element_Size(int type);
void ALTANAL_user_tag_size(int, int) ;


/* Options */
int ALTANAL_Enable  (ALTANAL_enum option);
int ALTANAL_Disable (ALTANAL_enum option);
int ALTANAL_Set     (ALTANAL_enum param, int  value);
int ALTANAL_Get     (ALTANAL_enum param, int *value);
int ALTANAL_Set_update_progress_callback(void (*p)(int, int)) ;

/* Sequences */
int ALTANAL_Sequence_Create  (ALTANAL_sequence_t **sequence);
int ALTANAL_Sequence_Destroy (ALTANAL_sequence_t *sequence);
int ALTANAL_Sequence_Wait    (ALTANAL_sequence_t *sequence);

END_C_DECLS

#endif
