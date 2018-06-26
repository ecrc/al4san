/**
 *
 * @file task.c
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL Task routines
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "control/auxiliary.h"
#include "control/common.h"
#include "altanal/altanal_runtime.h"

/****************************/

int ALTANAL_Insert_Task(ALTANAL_codelet codelet,  ALTANAL_option_t *options,...)
{
    va_list varg_list;

    va_start(varg_list, options);
   
    ALTANAL_Runtime_insert_task(codelet, options, varg_list);
    
    va_end(varg_list);

    return ALTANAL_SUCCESS;
}

int ALTANAL_Unpack_Arg(ALTANAL_arg args, ...)
{     
    va_list varg_list;

    va_start(varg_list, args);

   ALTANAL_Runtime_unpack_arg(args, varg_list);

    va_end(varg_list);

    return ALTANAL_SUCCESS;

}
