/**
 *
 * @file al4san_runtime.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 *
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 * 
 * @brief AL4SAN Quark runtime main header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#ifndef _AL4SAN_QUARK_H_
#define _AL4SAN_QUARK_H_

#include <quark.h>
#include <string.h>
#include "runtime_task.h"

#include "control/al4san_common.h"

BEGIN_C_DECLS

typedef struct runtime_internal_s {
    Quark_Task_Flags flags;
    Quark *quark;
} runtime_internal_t;

typedef void (al4san_codelet) (Quark *);


END_C_DECLS
#endif /* _AL4SAN_QUARK_H_ */
