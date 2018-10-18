/**
 *
 * @file al4san_runtime.h
 *
 * @copyright 2009-2015 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 * author Mathieu Faverge
 * author Reazul Hoque
 * date 2018-02-06
 *
 * @brief AL4SAN PaRSEC runtime main header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#ifndef _AL4SAN_PARSEC_H_
#define _AL4SAN_PARSEC_H_

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
#include "control/al4san_common.h"

BEGIN_C_DECLS

typedef struct parsec_dtd_funcptr_t al4san_codelet;




END_C_DECLS

#endif /* _AL4SAN_PARSEC_H_ */
