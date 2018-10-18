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
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "control/al4san_auxiliary.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"

int AL4SAN_Insert_Task(AL4SAN_codelet codelet,  AL4SAN_option_t *options,...)
{
    va_list varg_list;

    va_start(varg_list, options);
   
    AL4SAN_Runtime_insert_task(codelet, options, varg_list);
    
    va_end(varg_list);

    return AL4SAN_SUCCESS;
}

int AL4SAN_Unpack_Arg(AL4SAN_arg args, ...)
{     
    va_list varg_list;

    va_start(varg_list, args);

    AL4SAN_Runtime_unpack_arg(args, varg_list);

    va_end(varg_list);

    return AL4SAN_SUCCESS;

}
