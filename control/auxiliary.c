/**
 *
 * @file auxiliary.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * author Jakub Kurzak
 * author Piotr Luszczek
 * author Emmanuel Agullo
 * author Cedric Castagnede
 * date 2012-09-15
 *
 ***
 *
 * @defgroup Auxiliary
 * @brief Group auxiliary routines exposed to users
 *
 */
  /**
   *
   * @brief AL4SAN auxiliary routines
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
   **/
#include "control/al4san_common.h"
#include "control/al4san_auxiliary.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action without severe consequences.
 *  Problems occuring while AL4SAN is being used correctly.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void al4san_warning(const char *func_name, const char *msg_text)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL)
        al4san_fatal_error("al4san_warning", "AL4SAN not initialized");
    if (al4san->warnings_enabled)
        fprintf(stderr, "AL4SAN WARNING: %s(): %s\n", func_name, msg_text);
}

/**
 *
 *  Indicates a recoverable problem.
 *  User's erroneous action with potentially severe consequences.
 *  Problems occuring due to incorrect use of AL4SAN.
 *  Context aware.
 *
 * @param[in] func_name
 *          Function location where warning occurred
 *
 * @param[in] msg_text
 *          Warning message to display.
 *
 */
void al4san_error(const char *func_name, const char *msg_text)
{
    fprintf(stderr, "AL4SAN ERROR: %s(): %s\n", func_name, msg_text);
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
void al4san_fatal_error(const char *func_name, const char *msg_text)
{
    fprintf(stderr, "AL4SAN FATAL ERROR: %s(): %s\n", func_name, msg_text);
    exit(0);
}

/**
 *  Returns core id
 */
int al4san_rank(AL4SAN_context_t *al4san)
{
    return AL4SAN_Runtime_thread_rank( al4san );
}

/**
 *
 * @ingroup Auxiliary
 *
 *  AL4SAN_Version - Reports AL4SAN version number.
 *
 ******************************************************************************
 *
 * @param[out] ver_major
 *          AL4SAN major version number.
 *
 * @param[out] ver_minor
 *          AL4SAN minor version number.
 *
 * @param[out] ver_micro
 *          AL4SAN micro version number.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Version(int *ver_major, int *ver_minor, int *ver_micro)
{
    if (! ver_major && ! ver_minor && ! ver_micro)
        return  AL4SAN_ERR_ILLEGAL_VALUE;

    if (ver_major)
        *ver_major = AL4SAN_VERSION_MAJOR;

    if (ver_minor)
        *ver_minor = AL4SAN_VERSION_MINOR;

    if (ver_micro)
        *ver_micro = AL4SAN_VERSION_MICRO;

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Auxiliary
 *
 *  AL4SAN_Element_Size - Reports the size in bytes of a AL4SAN precision type
 *  (e.g. Al4sanInteger, Al4sanRealFloat, etc).
 *
 ******************************************************************************
 *
 * @param[in] type
 *          AL4SAN element type, can be one of the following:
 *          - Al4sanByte
 *          - Al4sanInteger
 *          - Al4sanRealFloat
 *          - Al4sanRealDouble
 *          - Al4sanComplexFloat
 *          - Al4sanComplexDouble
 *
 ******************************************************************************
 *
 * @return
 *          \retval Element size in bytes
 *
 */
int AL4SAN_Element_Size(int type)
{
    switch(type) {
        case Al4sanByte:          return          1;
        case Al4sanInteger:       return   sizeof(int);
        case Al4sanRealFloat:     return   sizeof(float);
        case Al4sanRealDouble:    return   sizeof(double);
        case Al4sanComplexFloat:  return 2*sizeof(float);
        case Al4sanComplexDouble: return 2*sizeof(double);
        default: al4san_fatal_error("AL4SAN_Element_Size", "undefined type");
                 return AL4SAN_ERR_ILLEGAL_VALUE;

    }
}

/**
 *
 * @ingroup Auxiliary
 *
 *  AL4SAN_My_Mpi_Rank - Return the MPI rank of the calling process.
 *
 ******************************************************************************
 *
 ******************************************************************************
 *
 * @return
 *          \retval MPI rank
 *
 */
int AL4SAN_My_Mpi_Rank(void)
{
#if defined(AL4SAN_USE_MPI)
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Finalize()", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    return AL4SAN_MPI_RANK;
#else
    return AL4SAN_SUCCESS;
#endif
}

/**
 *  Display a progress percentage in stderr
 */
void al4san_update_progress(int currentValue, int maximumValue) {
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
// By default it is al4san_update_progress()
// The user can change it with AL4SAN_Set_update_progress_callback()
void (*al4san_update_progress_callback)(int, int) = al4san_update_progress;

int AL4SAN_Set_update_progress_callback(void (*p)(int, int)) {
  al4san_update_progress_callback = p;
  return AL4SAN_SUCCESS;
}

