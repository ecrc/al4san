/**
 *
 * @file al4san_async.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 * @brief AL4SAN asynchronous routines
 ***
 * @author Jakub Kurzak
 * @author Cedric Castagnede
 * @date 2010-11-15
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 */

#ifndef _AL4SAN_ASYNC_H_
#define _AL4SAN_ASYNC_H_

#include "al4san/struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Internal routines
 */
int al4san_request_fail(AL4SAN_sequence_t *sequence, AL4SAN_request_t *request, int error);

#if defined(AL4SAN_SCHED_OPENMP)
#define PRAGMA(x) _Pragma(#x)

#define al4san_sequence_create(al4san)                           \
        PRAGMA(omp parallel)                                               \
         {                                                                  \
           PRAGMA(omp master)                                               \
            {                                                               \
             AL4SAN_sequence_t *sequence;                                   \
              if ((sequence = malloc(sizeof(AL4SAN_sequence_t)))==NULL)     \
                  al4san_error("al4san_sequence_create", "malloc() failed"); \
              (sequence)->status = AL4SAN_SUCCESS

#define al4san_sequence_destroy(al4san, sequence)                           \
          }                                                                 \
           }                                                                \
         free(sequence)


#else
AL4SAN_sequence_t* al4san_sequence_create  (AL4SAN_context_t *AL4SAN);
int al4san_sequence_destroy (AL4SAN_context_t *AL4SAN, AL4SAN_sequence_t *sequence);
#endif
int al4san_sequence_wait    (AL4SAN_context_t *AL4SAN, AL4SAN_sequence_t *sequence);

#ifdef __cplusplus
}
#endif

#endif
