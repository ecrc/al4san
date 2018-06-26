/**
 *
 * @file async.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon asynchronous management header
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#ifndef _ALTANAL_ASYNC_H_
#define _ALTANAL_ASYNC_H_

#include "altanal/altanal_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Internal routines
 */
int altanal_request_fail     (ALTANAL_sequence_t *sequence, ALTANAL_request_t *request, int error);
int altanal_sequence_create  (ALTANAL_context_t *ALTANAL, ALTANAL_sequence_t **sequence);
int altanal_sequence_destroy (ALTANAL_context_t *ALTANAL, ALTANAL_sequence_t *sequence);
int altanal_sequence_wait    (ALTANAL_context_t *ALTANAL, ALTANAL_sequence_t *sequence);

#ifdef __cplusplus
}
#endif

#endif
