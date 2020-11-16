/**
 *
 * @file quark_task.h
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **
 *
 * @brief AL4SAN Quark task header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#ifndef _TASKS_H_
#define _TASKS_H_

#include <quark.h>

#include "control/al4san_common.h"

BEGIN_C_DECLS
#define AL4SAN_QUARK_UNDEFINED 0xFF0000 
#define AL4SAN_QUARK_UNDEFINED_MASK 0x000FFF
#define ON 1
#define OFF 0
//#define AL4SAN_DEP 101
#define AL4SAN_QUARK_NMAXARGS 30
//#define ARG_END 0

typedef struct AL4SAN_Quark_Task
{
 int num_arg;
 int arg_depenency[AL4SAN_QUARK_NMAXARGS];	
 int arg_size[AL4SAN_QUARK_NMAXARGS];

} AL4SAN_Quark_task_t;

/*typedef struct AL4SAN_s
{
 Quark *quark;
} AL4SAN;*/

typedef struct al4san_quark_arg_list_s
{
    Quark *quark;
} AL4SAN_Quark_arg_list;

#define AL4SAN_QUARK_TASK_HEADER(name)

//callback
//#define AL4SAN_CALLBACK(name) NULL
#define al4san_quark_matrix_get_nx(num) NULL
#define al4san_quark_matrix_get_ny(num) NULL

//TO be used instead of AL4SAN_CODELETS
#define AL4SAN_QUARK_TASK_CPU(name, cpu_func_name)\
void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
void QUARK_##name##_func (Quark *quark) { \
     AL4SAN_arg_list al4san_arg; \
     AL4SAN_Quark_arg_list quarkarg;\
     quarkarg.quark=quark;\
     al4san_arg.Quark_arg_list= (AL4SAN_Quark_arg_list*) &quarkarg;\
     cpu_func_name(&al4san_arg);}

#define AL4SAN_QUARK_TASK_GPU(name, gpu_func_name)
 
#define AL4SAN_QUARK_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
void QUARK_##name##_func (Quark *quark) { \
     AL4SAN_arg_list al4san_arg; \
     AL4SAN_Quark_arg_list quarkarg;\
     quarkarg.quark=quark;\
     al4san_arg.Quark_arg_list= (AL4SAN_Quark_arg_list*) &quarkarg;\
     cpu_func_name(&al4san_arg);}


END_C_DECLS
#endif /* _TASKS_H_ */

