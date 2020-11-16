/**
 *
 * @file runtime_context.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 
 * @brief AL4SAN PaRSEC asynchronous context routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 * author Reazul Hoque
 * author Mathieu Faverge
 * date 2017-01-12
  * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 */
#include <stdlib.h>
#include "al4san_parsec.h"

/**
 *  Create new context
 */
void AL4SAN_Parsec_context_create( AL4SAN_context_t *al4san )
{
    /* In case of PaRSEC, this is done in init */
    al4san->scheduler = AL4SAN_RUNTIME_SCHED_PARSEC;
    return;
}

/**
 *  Clean the context
 */
void AL4SAN_Parsec_context_destroy( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 *
 */
void AL4SAN_Parsec_enable(AL4SAN_enum lever)
{
    switch (lever)
    {
    case AL4SAN_PROFILING_MODE:
        break;
    default:
        return;
    }
    return;
}

/**
 *
 */
void AL4SAN_Parsec_disable(AL4SAN_enum lever)
{
    switch (lever)
    {
    case AL4SAN_PROFILING_MODE:
        break;
    default:
        return;
    }
    return;
}
