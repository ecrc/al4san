/**
 *
 * @file context.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon context management routines
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 ***
 *
 * @defgroup Options
 * @brief Group routines exposed to users to handle options
 *
 */

#include <stdlib.h>
#if defined( _WIN32 ) || defined( _WIN64 )
#include "control/altanalwinthread.h"
#else
#include <pthread.h>
#endif

#include "control/common.h"
#include "control/auxiliary.h"
#include "control/context.h"
#include "altanal/altanal_runtime.h"

//#if !defined(ALTANAL_SIMULATION)
//#include "coreblas.h"
//#endif

/**
 *  Global data
 */
/* master threads context lookup table */
static ALTANAL_context_t *altanal_ctxt = NULL;

/**
 *  Create new context
 */
ALTANAL_context_t *altanal_context_create()
{
    ALTANAL_context_t *altanal;

    if ( altanal_ctxt != NULL ) {
        altanal_error("altanal_context_create", "a context is already existing\n");
        return NULL;
    }

    altanal = (ALTANAL_context_t*)malloc(sizeof(ALTANAL_context_t));
    if (altanal == NULL) {
        altanal_error("altanal_context_create", "malloc() failed");
        return NULL;
    }

    altanal->nworkers           = 1;
    altanal->ncudas             = 0;
    altanal->nthreads_per_worker= 1;

    altanal->warnings_enabled     = ALTANAL_TRUE;
    altanal->autotuning_enabled   = ALTANAL_TRUE;
    altanal->parallel_enabled     = ALTANAL_FALSE;
    altanal->profiling_enabled    = ALTANAL_FALSE;
    altanal->progress_enabled     = ALTANAL_FALSE;


    /* Initialize scheduler */
    ALTANAL_Runtime_context_create(altanal);

    altanal_ctxt = altanal;
    return altanal;
}


/**
 *  Return context for a thread
 */
ALTANAL_context_t *altanal_context_self()
{
    return altanal_ctxt;
}

/**
 *  Clean the context
 */
int altanal_context_destroy(){

    ALTANAL_Runtime_context_destroy(altanal_ctxt);
    free(altanal_ctxt);
    altanal_ctxt = NULL;

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  ALTANAL_Enable - Enable ALTANAL feature.
 *
 *******************************************************************************
 *
 * @param[in] option
 *          Feature to be enabled:
 *          @arg ALTANAL_WARNINGS   printing of warning messages,
 *          @arg ALTANAL_AUTOTUNING autotuning for tile size and inner block size.
 *          @arg ALTANAL_PROFILING_MODE  activate profiling of kernels
 *          @arg ALTANAL_PROGRESS  activate progress indicator
 *          @arg ALTANAL_GEMM3M  Use z/cgemm3m for complexe matrix-matrix products
 *
 *******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Enable(ALTANAL_enum option)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Enable", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }

    switch (option)
    {
        case ALTANAL_WARNINGS:
            altanal->warnings_enabled = ALTANAL_TRUE;
            break;
        case ALTANAL_AUTOTUNING:
            altanal->autotuning_enabled = ALTANAL_TRUE;
            break;
        case ALTANAL_PROFILING_MODE:
            altanal->profiling_enabled = ALTANAL_TRUE;
            break;
        case ALTANAL_PROGRESS:
            altanal->progress_enabled = ALTANAL_TRUE;
            break;
/*        case ALTANAL_GEMM3M:
#if defined(CBLAS_HAS_ZGEMM3M) && !defined(ALTANAL_SIMULATION)
            set_coreblas_gemm3m_enabled(1);
#else
            altanal_error("ALTANAL_Enable", "cannot enable GEMM3M (not available in cblas)");
#endif
            break;*/
        /* case ALTANAL_PARALLEL: */
        /*     altanal->parallel_enabled = ALTANAL_TRUE; */
        /*     break; */
        default:
            altanal_error("ALTANAL_Enable", "illegal parameter value");
            return ALTANAL_ERR_ILLEGAL_VALUE;
        case ALTANAL_BOUND:
            break;
    }

    /* Enable at the lower level if required */
    ALTANAL_Runtime_enable( option );

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  ALTANAL_Disable - Disable ALTANAL feature.
 *
 *******************************************************************************
 *
 * @param[in] option
 *          Feature to be disabled:
 *          @arg ALTANAL_WARNINGS   printing of warning messages,
 *          @arg ALTANAL_AUTOTUNING autotuning for tile size and inner block size.
 *          @arg ALTANAL_PROFILING_MODE  deactivate profiling of kernels
 *          @arg ALTANAL_PROGRESS  deactivate progress indicator
 *          @arg ALTANAL_GEMM3M  Use z/cgemm3m for complexe matrix-matrix products
 *
 *******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Disable(ALTANAL_enum option)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Disable", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    switch ( option )
    {
        case ALTANAL_WARNINGS:
            altanal->warnings_enabled = ALTANAL_FALSE;
            break;
        case ALTANAL_AUTOTUNING:
            altanal->autotuning_enabled = ALTANAL_FALSE;
            break;
        case ALTANAL_PROFILING_MODE:
            altanal->profiling_enabled = ALTANAL_FALSE;
            break;
        case ALTANAL_PROGRESS:
            altanal->progress_enabled = ALTANAL_FALSE;
            break;
/*        case ALTANAL_GEMM3M:
#if defined(CBLAS_HAS_ZGEMM3M) && !defined(ALTANAL_SIMULATION)
            set_coreblas_gemm3m_enabled(0);
#endif
            break;*/
        case ALTANAL_PARALLEL_MODE:
            altanal->parallel_enabled = ALTANAL_FALSE;
            break;
        default:
            altanal_error("ALTANAL_Disable", "illegal parameter value");
            return ALTANAL_ERR_ILLEGAL_VALUE;
    }

    /* Disable at the lower level if required */
    ALTANAL_Runtime_disable( option );

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  ALTANAL_Set - Set ALTANAL parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg ALTANAL_TILE_SIZE:        size matrix tile,
 *          @arg ALTANAL_INNER_BLOCK_SIZE: size of tile inner block,
 *
 * @param[in] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Set(ALTANAL_enum param, int value)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Set", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        default:
            altanal_error("ALTANAL_Set", "unknown parameter");
            return ALTANAL_ERR_ILLEGAL_VALUE;
    }

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Options
 *
 *  ALTANAL_Get - Get value of ALTANAL parameter.
 *
 *******************************************************************************
 *
 * @param[in] param
 *          Feature to be enabled:
 *          @arg ALTANAL_TILE_SIZE:        size matrix tile,
 *          @arg ALTANAL_INNER_BLOCK_SIZE: size of tile inner block,
 *
 * @param[out] value
 *          Value of the parameter.
 *
 *******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Get(ALTANAL_enum param, int *value)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Get", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    switch (param) {
        default:
            altanal_error("ALTANAL_Get", "unknown parameter");
            return ALTANAL_ERR_ILLEGAL_VALUE;
    }

    return ALTANAL_SUCCESS;
}
