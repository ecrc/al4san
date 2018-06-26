/**
 *
 * @file altanal_runtime.h
 *
 * @copyright 2009-2015 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon PaRSEC runtime header
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Reazul Hoque
 * @date 2018-02-06
 *
 * @brief ALTANAL PaRSEC runtime main header
 *
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/
#ifndef _ALTANAL_PARSEC_H_
#define _ALTANAL_PARSEC_H_

#include <parsec.h>
#include <parsec/parsec_internal.h>
#include <parsec/execution_stream.h>
#include <parsec/interfaces/superscalar/insert_function.h>
#include <parsec/interfaces/superscalar/insert_function_internal.h>
#include <parsec/utils/debug.h>
#include <parsec/data_dist/matrix/matrix.h>

/* Undefined PaRSEC definition of BLKLDD */
#undef BLKLDD
#include "runtime_task.h"
#include "control/common.h"

BEGIN_C_DECLS

typedef struct parsec_dtd_funcptr_t altanal_codelet;
void  altanal_insert_task(altanal_codelet *codelet,  ALTANAL_option_t *options, ...);
void altanal_unpack_arg(ALTANAL_arg_list *altanal_arg, ...);



END_C_DECLS

#endif /* _ALTANAL_PARSEC_H_ */
