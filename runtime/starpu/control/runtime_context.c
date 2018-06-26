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
 * @brief Altanal StarPU context routines
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#include <stdlib.h>
#include "altanal_runtime.h"

#if (STARPU_MAJOR_VERSION > 1) || ((STARPU_MAJOR_VERSION == 1) && (STARPU_MINOR_VERSION >= 3))
/* Defined by StarPU as external function */
#else
#if ((STARPU_MAJOR_VERSION == 1) && (STARPU_MINOR_VERSION >= 2))
int _starpu_is_initialized(void);
#define starpu_is_initialized() _starpu_is_initialized()
#else
#define starpu_is_initialized() 0
#endif
#endif

/**
 *  Create new context
 */
void ALTANAL_Runtime_context_create( ALTANAL_context_t *altanal )
{
    starpu_conf_t *conf;

    altanal->scheduler = ALTANAL_RUNTIME_SCHED_STARPU;

    if (! starpu_is_initialized() ) {
        altanal->schedopt = (void*) malloc (sizeof(struct starpu_conf));
        conf = altanal->schedopt;

        starpu_conf_init( conf );
    }
    else {
        altanal->schedopt = NULL;
    }

    return;
}

/**
 *  Clean the context
 */
void ALTANAL_Runtime_context_destroy( ALTANAL_context_t *altanal )
{
    /* StarPU was already initialized by an external library */
    if (altanal->schedopt) {
        free(altanal->schedopt);
    }
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
            starpu_profiling_status_set(STARPU_PROFILING_ENABLE);
            break;
        case ALTANAL_BOUND:
            starpu_bound_start(0, 0);
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
            starpu_profiling_status_set(STARPU_PROFILING_DISABLE);
            break;
        case ALTANAL_BOUND:
            starpu_bound_stop();
            break;
        default:
            return;
    }
    return;
}
