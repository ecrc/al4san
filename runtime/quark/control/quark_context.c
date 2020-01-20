/**
 *
 * @file runtime_context.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 *
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2011-10-29
 *
 **
 * @brief AL4SAN Quark context routines
 *  
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#include <stdlib.h>
#include "al4san_quark.h"

/**
 *  Create new context
 */
void AL4SAN_Quark_context_create( AL4SAN_context_t *al4san )
{
    al4san->scheduler = AL4SAN_RUNTIME_SCHED_QUARK;
    /* Will require the static initialization if we want to use it in this code */
    return;
}

/**
 *  Clean the context
 */
void AL4SAN_Quark_context_destroy( AL4SAN_context_t *al4san )
{
    (void)al4san;
    return;
}

/**
 *
 */
void AL4SAN_Quark_enable( AL4SAN_enum lever )
{
    switch (lever)
    {
        case AL4SAN_PROFILING_MODE:
            fprintf(stderr, "Profiling is not available with Quark\n");
            break;
        case AL4SAN_BOUND:
            fprintf(stderr, "Bound computation is not available with Quark\n");
            break;
        default:
            return;
    }
    return;
}

/**
 *
 */
void AL4SAN_Quark_disable( AL4SAN_enum lever )
{
    switch (lever)
    {
        case AL4SAN_PROFILING_MODE:
            fprintf(stderr, "Profiling is not available with Quark\n");
            break;
        case AL4SAN_BOUND:
            fprintf(stderr, "Bound computation is not available with Quark\n");
            break;
        default:
            return;
    }
    return;
}
