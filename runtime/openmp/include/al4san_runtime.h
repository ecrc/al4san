/**
 *
 * @copyright (c) 2009-2014 The University of Tennessee and The University
 *                          of Tennessee Research Foundation.
 *                          All rights reserved.
 * @copyright (c) 2012-2014 Inria. All rights reserved.
 * @copyright (c) 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria, Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 **/

/**
 *
 * @file al4san_runtime.h
 * 
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 **/
/**
 *
 * @brief AL4SAN OpenMP runtime heaer
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
**/ 

#ifndef _AL4SAN_OPENMP_H_
#define _AL4SAN_OPENMP_H_
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include "runtime_task.h"

#include "control/al4san_common.h"


typedef int (al4san_codelet) (kmp_int32, kmp_task_t *);


#endif /* _AL4SAN_OPENMP_H_ */
