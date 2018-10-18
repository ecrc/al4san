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
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2010-11-15
 *
 */
/**
 *
 * @brief AL4SAN StarPU context routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
**/
#include <stdlib.h>
#include "al4san_runtime.h"

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
void AL4SAN_Runtime_context_create( AL4SAN_context_t *al4san )
{
    starpu_conf_t *conf;

    al4san->scheduler = AL4SAN_RUNTIME_SCHED_STARPU;

    if (! starpu_is_initialized() ) {
        al4san->schedopt = (void*) malloc (sizeof(struct starpu_conf));
        conf = al4san->schedopt;

        starpu_conf_init( conf );
    }
    else {
        al4san->schedopt = NULL;
    }

    return;
}

/**
 *  Clean the context
 */
void AL4SAN_Runtime_context_destroy( AL4SAN_context_t *al4san )
{
    /* StarPU was already initialized by an external library */
    if (al4san->schedopt) {
        free(al4san->schedopt);
    }
    return;
}

/**
 *
 */
void AL4SAN_Runtime_enable( AL4SAN_enum lever )
{
    switch (lever)
    {
        case AL4SAN_PROFILING_MODE:
            starpu_profiling_status_set(STARPU_PROFILING_ENABLE);
            break;
        case AL4SAN_BOUND:
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
void AL4SAN_Runtime_disable( AL4SAN_enum lever )
{
    switch (lever)
    {
        case AL4SAN_PROFILING_MODE:
            starpu_profiling_status_set(STARPU_PROFILING_DISABLE);
            break;
        case AL4SAN_BOUND:
            starpu_bound_stop();
            break;
        default:
            return;
    }
    return;
}
