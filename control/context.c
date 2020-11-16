/**
 *
 * @file context.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 * @brief AL4SAN context management  routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 * author Jakub Kurzak
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2012-09-15
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 */

#include <stdlib.h>
#include <pthread.h>

#include "control/al4san_common.h"
#include "control/al4san_auxiliary.h"
#include "control/al4san_context.h"
#include "al4san/runtime.h"
#include <string.h>
/**
 *  Global data
 */
/* master threads context lookup table */
static AL4SAN_context_t *al4san_ctxt = NULL;

/**
 *  Create new context
 */
AL4SAN_context_t *al4san_context_create(char *runtime)
{
    AL4SAN_context_t *al4san;

    if ( al4san_ctxt != NULL ) {
        al4san_error("al4san_context_create", "a context is already existing\n");
        return NULL;
    }

    al4san = (AL4SAN_context_t*)malloc(sizeof(AL4SAN_context_t));
    if (al4san == NULL) {
        al4san_error("al4san_context_create", "malloc() failed");
        return NULL;
    }

    al4san->nworkers           = 1;
    al4san->ncudas             = 0;
    al4san->nthreads_per_worker= 1;

    al4san->warnings_enabled     = AL4SAN_TRUE;
    al4san->autotuning_enabled   = AL4SAN_TRUE;
    al4san->parallel_enabled     = AL4SAN_FALSE;
    al4san->profiling_enabled    = AL4SAN_FALSE;
    al4san->progress_enabled     = AL4SAN_FALSE;

    /* Initialize scheduler */
#ifdef AL4SAN_SCHED_QUARK
    if(strcmp(runtime, "Quark")==0) {
    AL4SAN_Quark_context_create(al4san);
    }
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(strcmp(runtime, "Starpu")==0){
        AL4SAN_Starpu_context_create(al4san);
    }
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(strcmp(runtime, "Parsec")==0){
        AL4SAN_Parsec_context_create(al4san);
    }
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(strcmp(runtime, "Openmp")==0)
        AL4SAN_Openmp_context_create(al4san);
#endif

    al4san_ctxt = al4san;
    return al4san;
}


/**
 *  Return context for a thread
 */
AL4SAN_context_t *al4san_context_self()
{
    return al4san_ctxt;
}

/**
 *  Clean the context
 */
int al4san_context_destroy(){

    AL4SAN_context_t *al4san;
    int status;

    al4san = al4san_context_self();

    if (al4san == NULL) {
        al4san_fatal_error("al4san_context_destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_context_destroy(al4san_ctxt);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_context_destroy(al4san_ctxt);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_context_destroy(al4san_ctxt);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_context_destroy(al4san_ctxt);
#endif

    free(al4san_ctxt);
    al4san_ctxt = NULL;

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  AL4SAN_Enable - Enable AL4SAN feature.
 *
 *******************************************************************************
 *
 * @param[in] option
 *          Feature to be enabled:
 *          @arg AL4SAN_WARNINGS   printing of warning messages,
 *          @arg AL4SAN_AUTOTUNING autotuning for tile size and inner block size.
 *          @arg AL4SAN_PROFILING_MODE  activate profiling of kernels
 *          @arg AL4SAN_PROGRESS  activate progress indicator
 *
 *******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Enable(AL4SAN_enum option)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Enable", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    switch (option)
    {
        case AL4SAN_WARNINGS:
            al4san->warnings_enabled = AL4SAN_TRUE;
            break;
        case AL4SAN_AUTOTUNING:
            al4san->autotuning_enabled = AL4SAN_TRUE;
            break;
        case AL4SAN_PROFILING_MODE:
            al4san->profiling_enabled = AL4SAN_TRUE;
            break;
        case AL4SAN_PROGRESS:
            al4san->progress_enabled = AL4SAN_TRUE;
            break;
        default:
            al4san_error("AL4SAN_Enable", "illegal parameter value");
            return AL4SAN_ERR_ILLEGAL_VALUE;
        case AL4SAN_BOUND:
            break;
    }

    /* Enable at the lower level if required */

#ifdef AL4SAN_SCHED_QUARK    
    if(al4san->scheduler==0)
    AL4SAN_Quark_enable(option);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_enable(option);
#endif
#ifdef AL4SAN_SCHED_PARSEC     
    if(al4san->scheduler==2)
         AL4SAN_Parsec_enable(option);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_enable(option);
#endif

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  AL4SAN_Disable - Disable AL4SAN feature.
 *
 *******************************************************************************
 *
 * @param[in] option
 *          Feature to be disabled:
 *          @arg AL4SAN_WARNINGS   printing of warning messages,
 *          @arg AL4SAN_AUTOTUNING autotuning for tile size and inner block size.
 *          @arg AL4SAN_PROFILING_MODE  deactivate profiling of kernels
 *          @arg AL4SAN_PROGRESS  deactivate progress indicator
 *
 *******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Disable(AL4SAN_enum option)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Disable", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    switch ( option )
    {
        case AL4SAN_WARNINGS:
            al4san->warnings_enabled = AL4SAN_FALSE;
            break;
        case AL4SAN_AUTOTUNING:
            al4san->autotuning_enabled = AL4SAN_FALSE;
            break;
        case AL4SAN_PROFILING_MODE:
            al4san->profiling_enabled = AL4SAN_FALSE;
            break;
        case AL4SAN_PROGRESS:
            al4san->progress_enabled = AL4SAN_FALSE;
            break;
        case AL4SAN_PARALLEL_MODE:
            al4san->parallel_enabled = AL4SAN_FALSE;
            break;
        default:
            al4san_error("AL4SAN_Disable", "illegal parameter value");
            return AL4SAN_ERR_ILLEGAL_VALUE;
    }

    /* Disable at the lower level if required */
#ifdef AL4SAN_SCHED_QUARK    
    if(al4san->scheduler==0)
    AL4SAN_Quark_disable(option);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_disable(option);
#endif
#ifdef AL4SAN_SCHED_PARSEC     
    if(al4san->scheduler==2)
         AL4SAN_Parsec_disable(option);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
         AL4SAN_Openmp_disable(option);
#endif

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  AL4SAN_Set - Set AL4SAN parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg AL4SAN_TILE_SIZE:        size matrix tile,
 *          @arg AL4SAN_INNER_BLOCK_SIZE: size of tile inner block,
 *
 * @param[in] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Set(AL4SAN_enum param, int value)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Set", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        default:
            al4san_error("AL4SAN_Set", "unknown parameter");
            return AL4SAN_ERR_ILLEGAL_VALUE;
    }

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  AL4SAN_Get - Get value of AL4SAN parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg AL4SAN_TILE_SIZE:        size matrix tile,
 *          @arg AL4SAN_INNER_BLOCK_SIZE: size of tile inner block,
 *
 * @param[out] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Get(AL4SAN_enum param, int *value)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Get", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        default:
            al4san_error("AL4SAN_Get", "unknown parameter");
            return AL4SAN_ERR_ILLEGAL_VALUE;
    }

    return AL4SAN_SUCCESS;
}
