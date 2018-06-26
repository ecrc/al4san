/**
 *
 * @file runtime_context.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC context routines
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
 *
 */
#include <stdlib.h>
#include "altanal_runtime.h"

/**
 *  Create new context
 */
void ALTANAL_Runtime_context_create( ALTANAL_context_t *altanal )
{
    /* In case of PaRSEC, this is done in init */
    altanal->scheduler = ALTANAL_RUNTIME_SCHED_PARSEC;
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
void ALTANAL_Runtime_enable(ALTANAL_enum lever)
{
    switch (lever)
    {
    case ALTANAL_PROFILING_MODE:
        break;
    default:
        return;
    }
    return;
}

/**
 *
 */
void ALTANAL_Runtime_disable(ALTANAL_enum lever)
{
    switch (lever)
    {
    case ALTANAL_PROFILING_MODE:
        break;
    default:
        return;
    }
    return;
}
