/**
 *
 * @file altanal_runtime.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon Quark runtime main header
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 * 
 * @brief ALTANAL Quark runtime main header
 *
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/
#ifndef _ALTANAL_QUARK_H_
#define _ALTANAL_QUARK_H_

#include <quark.h>
#include "runtime_task.h"

#include "control/common.h"

BEGIN_C_DECLS

typedef struct runtime_internal_s {
    Quark_Task_Flags flags;
    Quark *quark;
} runtime_internal_t;

typedef void (altanal_codelet) (Quark *);
void altanal_insert_task( altanal_codelet* codelet, ALTANAL_option_t *options, ...);
void altanal_unpack_arg(ALTANAL_arg_list *altanal_arg, ...);


END_C_DECLS
#endif /* _ALTANAL_QUARK_H_ */
