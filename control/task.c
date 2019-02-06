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


int AL4SAN_Insert_Task(AL4SAN_codelet codelet,  AL4SAN_option_t *options,...)
{
    va_list varg_list;

    va_start(varg_list, options);
   
    AL4SAN_Runtime_insert_task(codelet, options, varg_list);
    
    va_end(varg_list);

    return AL4SAN_SUCCESS;
}

/*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */

int AL4SAN_Unpack_Arg(AL4SAN_arg args, ...)
{     
    va_list varg_list;

    va_start(varg_list, args);

    AL4SAN_Runtime_unpack_arg(args, varg_list);

    va_end(varg_list);

    return AL4SAN_SUCCESS;

}
