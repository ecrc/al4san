/**
 *
 * @file async.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 * *
 * @brief AL4SAN asynchronous routines
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * 2date 2012-09-15
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 * @defgroup Sequences
 * @brief Group routines exposed to users to handle asynchronous tasks execution
 *
 */

#include <stdlib.h>
#include "control/al4san_common.h"
#include "al4san/runtime.h"

/**
 *  Register an exception.
 */
int al4san_request_fail(AL4SAN_sequence_t *sequence, AL4SAN_request_t *request, int status)
{
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    return status;
}



/**
 *  Create a sequence
 */
#if !defined(AL4SAN_SCHED_OPENMP)
AL4SAN_sequence_t* al4san_sequence_create(AL4SAN_context_t *al4san)
{


    AL4SAN_sequence_t *sequence;
    if ((sequence = malloc(sizeof(AL4SAN_sequence_t))) == NULL) {
        al4san_error("al4san_sequence_create", "malloc() failed");
     //   return AL4SAN_ERR_OUT_OF_RESOURCES;
    }
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_sequence_create( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_sequence_create( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_PARSEC     
    if(al4san->scheduler==2)
         AL4SAN_Parsec_sequence_create( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_OPENMP    
    if(al4san->scheduler==3)
         AL4SAN_Openmp_sequence_create( al4san, sequence );
#endif
    
    sequence->status = AL4SAN_SUCCESS;
    return sequence;
}

/**
 *  Destroy a sequence
 */
int al4san_sequence_destroy(AL4SAN_context_t *al4san, AL4SAN_sequence_t *sequence)
{
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_sequence_destroy( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_sequence_destroy( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_PARSEC    
    if(al4san->scheduler==2)
         AL4SAN_Parsec_sequence_destroy( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_OPENMP  
    if(al4san->scheduler==3)
         AL4SAN_Openmp_sequence_destroy( al4san, sequence );
#endif

    free(sequence);
    return AL4SAN_SUCCESS;
}
#endif
/**
 *  Wait for the completion of a sequence
 */
int al4san_sequence_wait(AL4SAN_context_t *al4san, AL4SAN_sequence_t *sequence)
{

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_sequence_wait( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_sequence_wait( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_PARSEC    
    if(al4san->scheduler==2)
         AL4SAN_Parsec_sequence_wait( al4san, sequence );
#endif
#ifdef AL4SAN_SCHED_OPENMP  
    if(al4san->scheduler==3)
         AL4SAN_Openmp_sequence_wait( al4san, sequence );
#endif

    return AL4SAN_SUCCESS;
}

/**
 *
 * @ingroup Sequences
 *
 *  AL4SAN_Sequence_Create - Create a squence.
 *
 ******************************************************************************
 *
 * @param[out] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
#if !defined(AL4SAN_SCHED_OPENMP)

//int AL4SAN_Sequence_Create(AL4SAN_sequence_t **sequence)
AL4SAN_sequence_t* AL4SAN_Sequence_Create()
{
    AL4SAN_context_t *al4san;
    int status;

    al4san = al4san_context_self();

    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Create", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }
    AL4SAN_sequence_t *sequence= al4san_sequence_create(al4san);
    return sequence;
}

/**
 *
 * @ingroup Sequences
 *
 *  AL4SAN_Sequence_Destroy - Destroy a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Sequence_Destroy(AL4SAN_sequence_t *sequence)
{
    AL4SAN_context_t *al4san;
    int status;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Destroy", "AL4SAN not initialized");
 //       return AL4SAN_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Destroy", "NULL sequence");
   //     return AL4SAN_ERR_UNALLOCATED;
    }
    status = al4san_sequence_destroy(al4san, sequence);
    return status;
}
#endif
/**
 *
 * @ingroup Sequences
 *
 *  AL4SAN_Sequence_Wait - Wait for the completion of a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Sequence_Wait(AL4SAN_sequence_t *sequence)
{
    AL4SAN_context_t *al4san;
    int status;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Wait", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Wait", "NULL sequence");
        return AL4SAN_ERR_UNALLOCATED;
    }
    status = al4san_sequence_wait(al4san, sequence);
    return status;
}

/**
 *
 * @ingroup Sequences
 *
 *  AL4SAN_Sequence_Flush - Terminate a sequence.
 *
 ******************************************************************************
 *
 * @param[in] sequence
 *          Identifies a set of routines sharing common exception handling.
 *
 * @param[in] request
 *          The flush request.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Sequence_Flush(AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Flush", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    if (sequence == NULL) {
        al4san_fatal_error("AL4SAN_Sequence_Flush", "NULL sequence");
        return AL4SAN_ERR_UNALLOCATED;
    }
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_sequence_flush( al4san->schedopt, sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);
#endif
#ifdef AL4SAN_SCHED_STARPU
    else if(al4san->scheduler==1)
         AL4SAN_Starpu_sequence_flush( al4san->schedopt, sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);
#endif
#ifdef AL4SAN_SCHED_PARSEC 
    if(al4san->scheduler==2)
         AL4SAN_Parsec_sequence_flush( al4san->schedopt, sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);
#endif
#ifdef AL4SAN_SCHED_OPENMP
    if(al4san->scheduler==3)
         AL4SAN_Openmp_sequence_flush( al4san->schedopt, sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);
#endif

    return AL4SAN_SUCCESS;
}



