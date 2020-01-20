/**
 *
 * @file al4san_auxiliary.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 * *
 * @brief AL4SAN auxiliary routines
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 * @author Jakub Kurzak
 * @author Piotr Luszczek
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2010-11-15
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
 */
#ifndef _AL4SAN_AUXILIARY_H_
#define _AL4SAN_AUXILIARY_H_

#include "al4san/struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Internal routines
 */
void al4san_warning      (const char *func_name, const char* msg_text);
void al4san_error        (const char *func_name, const char* msg_text);
void al4san_fatal_error  (const char *func_name, const char* msg_text);
int  al4san_rank         (AL4SAN_context_t *al4san);
int  al4san_tune         (AL4SAN_enum func, int M, int N, int NRHS);

#ifdef __cplusplus
}
#endif

#endif
