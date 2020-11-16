/**
 *
 * @file al4san_context.h
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
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2012-09-15
 *
 */

  /**
   *
   * @brief AL4SAN context
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.1.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
   **/

#ifndef _AL4SAN_CONTEXT_H_
#define _AL4SAN_CONTEXT_H_

#include "al4san/struct.h"

/**
 *  Routines to handle threads context
 */
#ifdef __cplusplus
extern "C" {
#endif

AL4SAN_context_t* al4san_context_create  (char *runtime);
AL4SAN_context_t* al4san_context_self    ();
int              al4san_context_destroy ();

#ifdef __cplusplus
}
#endif

#endif
