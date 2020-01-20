
/**
 *
 * @file runtime_async.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 * @brief AL4san OpenMP  sequence source codes
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
 */

#include <stdlib.h>
#include "al4san_openmp.h"



/*******************************************************************************
 *  Wait for the completion of a sequence
 **/
int AL4SAN_Openmp_sequence_wait( AL4SAN_context_t *al4san, 
                           AL4SAN_sequence_t *sequence )
{
    (void)al4san;
    (void)sequence;
    #pragma omp taskwait
    return AL4SAN_SUCCESS;
}

/*******************************************************************************
 *  Terminate a sequence
 **/
void AL4SAN_Openmp_sequence_flush( AL4SAN_context_t  *al4san, 
                           AL4SAN_sequence_t *sequence, 
                           AL4SAN_request_t *request, 
                           int status)
{
    (void)al4san;
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    #pragma omp taskwait

   // #pragma omp flush
    return;
}

