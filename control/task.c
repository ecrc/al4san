/**
 *
 * @file task.c
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief AL4SAN Task routines
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "control/al4san_auxiliary.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"



/*
    * Insert Task function: Define Task and it is dependencies
    *  @param[in] First argument AL4SAN_TASK macro with task name
    *  @param[in] options argument which holds sequence data sturcture
    *  @param[in] Parameter list  of va_list type to represent data and the dependencies 
 */


int AL4SAN_Insert_Task(AL4SAN_codelet codelet_quark, AL4SAN_codelet codelet_starpu, AL4SAN_codelet codelet_parsec, AL4SAN_codelet codelet_openmp, AL4SAN_option_t *options,...)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Insert_Task", "AL4SAN not initialized");
        //eturn AL4SAN_ERR_NOT_INITIALIZED;
    }

    va_list varg_list;

    va_start(varg_list, options);

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_insert_task(codelet_quark, options, varg_list);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_insert_task(codelet_starpu, options, varg_list);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_insert_task(codelet_parsec, options, varg_list);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     AL4SAN_Openmp_insert_task(codelet_openmp, options, varg_list);
#endif
    
    va_end(varg_list);

    return AL4SAN_SUCCESS;
}

/*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

int AL4SAN_Unpack_Arg(AL4SAN_arg_list *args, ...)
{    
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_fatal_error("AL4SAN_Unpack_Arg", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

    va_list varg_list;

    va_start(varg_list, args);


#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_unpack_arg(args, varg_list);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_unpack_arg(args, varg_list);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_unpack_arg(args, varg_list);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     AL4SAN_Openmp_unpack_arg(args, varg_list);
#endif
    
    va_end(varg_list);

    return AL4SAN_SUCCESS;

}
