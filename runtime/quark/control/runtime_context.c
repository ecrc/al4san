/**
 *
 * @file runtime_context.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon Quark context routines
 *
 * @version 1.0.0
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2011-10-29
 *
 */
#include <stdlib.h>
#include "altanal_runtime.h"

/**
 *  Create new context
 */
void ALTANAL_Runtime_context_create( ALTANAL_context_t *altanal )
{
    altanal->scheduler = ALTANAL_RUNTIME_SCHED_QUARK;
    /* Will require the static initialization if we want to use it in this code */
    return;
}

/**
 *  Clean the context
 */
void ALTANAL_Runtime_context_destroy( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return;
}

/**
 *
 */
void ALTANAL_Runtime_enable( ALTANAL_enum lever )
{
    switch (lever)
    {
        case ALTANAL_PROFILING_MODE:
            fprintf(stderr, "Profiling is not available with Quark\n");
            break;
        case ALTANAL_BOUND:
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
void ALTANAL_Runtime_disable( ALTANAL_enum lever )
{
    switch (lever)
    {
        case ALTANAL_PROFILING_MODE:
            fprintf(stderr, "Profiling is not available with Quark\n");
            break;
        case ALTANAL_BOUND:
            fprintf(stderr, "Bound computation is not available with Quark\n");
            break;
        default:
            return;
    }
    return;
}
