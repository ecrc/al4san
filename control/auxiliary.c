/**
 *
 * @file auxiliary.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon auxiliary routines
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Piotr Luszczek
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 ***
 *
 * @defgroup Auxiliary
 * @brief Group auxiliary routines exposed to users
 *
 */

#include "control/common.h"
#include "control/auxiliary.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action without severe consequences.
 *  Problems occuring while ALTANAL is being used correctly.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void altanal_warning(const char *func_name, const char *msg_text)
{
    ALTANAL_context_t *altanal;

    altanal = altanal_context_self();
    if (altanal == NULL)
        altanal_fatal_error("altanal_warning", "ALTANAL not initialized");
    if (altanal->warnings_enabled)
        fprintf(stderr, "ALTANAL WARNING: %s(): %s\n", func_name, msg_text);
}

/**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action with potentially severe consequences.
 *  Problems occuring due to incorrect use of ALTANAL.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void altanal_error(const char *func_name, const char *msg_text)
{
    fprintf(stderr, "ALTANAL ERROR: %s(): %s\n", func_name, msg_text);
}

/**
 *
 *  Unexpected behavior within the library.
 *  Unrecoverable user errors.
 *  Context oblivious.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void altanal_fatal_error(const char *func_name, const char *msg_text)
{
    fprintf(stderr, "ALTANAL FATAL ERROR: %s(): %s\n", func_name, msg_text);
    exit(0);
}

/**
 *  Returns core id
 */
int altanal_rank(ALTANAL_context_t *altanal)
{
    return ALTANAL_Runtime_thread_rank( altanal );
}

/**
 *  Tune block size nb and internal block size ib
 */
int altanal_tune(ALTANAL_enum func, int M, int N, int NRHS)
{
    ALTANAL_context_t *altanal;
    altanal = altanal_context_self();
    if ( altanal && altanal->autotuning_enabled == ALTANAL_TRUE ) {
        altanal_warning( "altanal_tune", "Autotunning not available for now" );
    }
    (void)func;
    (void)M;
    (void)N;
    (void)NRHS;
    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Auxiliary
 *
 *  ALTANAL_Version - Reports ALTANAL version number.
 *
 ******************************************************************************
 *
 * @param[out] ver_major
 *          ALTANAL major version number.
 *
 * @param[out] ver_minor
 *          ALTANAL minor version number.
 *
 * @param[out] ver_micro
 *          ALTANAL micro version number.
 *
 ******************************************************************************
 *
 * @return
 *          \retval ALTANAL_SUCCESS successful exit
 *
 */
int ALTANAL_Version(int *ver_major, int *ver_minor, int *ver_micro)
{
    if (! ver_major && ! ver_minor && ! ver_micro)
        return  ALTANAL_ERR_ILLEGAL_VALUE;

    if (ver_major)
        *ver_major = ALTANAL_VERSION_MAJOR;

    if (ver_minor)
        *ver_minor = ALTANAL_VERSION_MINOR;

    if (ver_micro)
        *ver_micro = ALTANAL_VERSION_MICRO;

    return ALTANAL_SUCCESS;
}

/**
 *
 * @ingroup Auxiliary
 *
 *  ALTANAL_Element_Size - Reports the size in bytes of a ALTANAL precision type
 *  (e.g. AltanalInteger, AltanalRealFloat, etc).
 *
 ******************************************************************************
 *
 * @param[in] type
 *          ALTANAL element type, can be one of the following:
 *          - AltanalByte
 *          - AltanalInteger
 *          - AltanalRealFloat
 *          - AltanalRealDouble
 *          - AltanalComplexFloat
 *          - AltanalComplexDouble
 *
 ******************************************************************************
 *
 * @return
 *          \retval Element size in bytes
 *
 */
int ALTANAL_Element_Size(int type)
{
    switch(type) {
        case AltanalByte:          return          1;
        case AltanalInteger:       return   sizeof(int);
        case AltanalRealFloat:     return   sizeof(float);
        case AltanalRealDouble:    return   sizeof(double);
        case AltanalComplexFloat:  return 2*sizeof(float);
        case AltanalComplexDouble: return 2*sizeof(double);
        default: altanal_fatal_error("ALTANAL_Element_Size", "undefined type");
                 return ALTANAL_ERR_ILLEGAL_VALUE;

    }
}

/**
 *
 * @ingroup Auxiliary
 *
 *  ALTANAL_My_Mpi_Rank - Return the MPI rank of the calling process.
 *
 ******************************************************************************
 *
 ******************************************************************************
 *
 * @return
 *          \retval MPI rank
 *
 */
int ALTANAL_My_Mpi_Rank(void)
{
#if defined(ALTANAL_USE_MPI)
    ALTANAL_context_t *altanal = altanal_context_self();
    if (altanal == NULL) {
        altanal_error("ALTANAL_Finalize()", "ALTANAL not initialized");
        return ALTANAL_ERR_NOT_INITIALIZED;
    }
    return ALTANAL_MPI_RANK;
#else
    return ALTANAL_SUCCESS;
#endif
}

/**
 *  Display a progress percentage in stderr
 */
void altanal_update_progress(int currentValue, int maximumValue) {
    div_t res ;
    static int progress = -1; /* varie de 0 a 100 au cours du calcul concerne */

    if (maximumValue == 0) {
        res.quot = 100;
    }
    else {
        if (currentValue < (INT_MAX / 100) ) {
            res = div(currentValue*100, maximumValue);
        }
        else {
            /* Calcule le quotient de la division */
            res.quot = (int)( (long long)( currentValue * 100 ) / maximumValue );
        }
    }

    // Print the percentage
    if (res.quot > progress) {
        fprintf(stderr, "%3d%%\b\b\b\b", res.quot);
    }
    progress = res.quot;

    if (currentValue >= maximumValue) {
        progress = -1;
    }
}

// A function to display the progress indicator.
// By default it is altanal_update_progress()
// The user can change it with ALTANAL_Set_update_progress_callback()
void (*altanal_update_progress_callback)(int, int) = altanal_update_progress;

int ALTANAL_Set_update_progress_callback(void (*p)(int, int)) {
  altanal_update_progress_callback = p;
  return ALTANAL_SUCCESS;
}

