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
 * @version 1.0.0
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


/*typedef enum { AL4SAN_INPUT=INPUT,
     AL4SAN_OUTPUT=OUTPUT,
     AL4SAN_INOUT=INOUT,
     AL4SAN_VALUE=VALUE,
     AL4SAN_NODEP=NODEP,
     AL4SAN_SCRATCH=SCRATCH,
     AL4SAN_LOCALITY=LOCALITY,
     AL4SAN_NOLOCALITY=NOLOCALITY, 
     AL4SAN_ACCUMULATOR=ACCUMULATOR, 
     AL4SAN_NOACCUMULATOR=NOACCUMULATOR,
     AL4SAN_GATHERV=GATHERV,
     AL4SAN_NOGATHERV=NOGATHERV
}al4san_quark_dependency_t;

typedef enum { AL4SAN_REGION_0=QUARK_REGION_0, 
     AL4SAN_REGION_1=QUARK_REGION_1, 
     AL4SAN_REGION_2=QUARK_REGION_2, 
     AL4SAN_REGION_3=QUARK_REGION_3,
     AL4SAN_REGION_4=QUARK_REGION_4, 
     AL4SAN_REGION_5=QUARK_REGION_5, 
     AL4SAN_REGION_6=QUARK_REGION_6, 
     AL4SAN_REGION_7=QUARK_REGION_7
}al4san_quark_data_region_t;

typedef enum { AL4SAN_REGION_L=QUARK_REGION_0|QUARK_REGION_1|QUARK_REGION_2,
     AL4SAN_REGION_D=QUARK_REGION_3|QUARK_REGION_4,
     AL4SAN_REGION_U=QUARK_REGION_5|QUARK_REGION_6|QUARK_REGION_7 
} al4san_quark_ldu_region_t;

typedef enum { AL4SAN_PRIORITY=TASK_PRIORITY,
     AL4SAN_LOCK_TO_THREAD=TASK_LOCK_TO_THREAD,
     AL4SAN_SEQUENCE=TASK_SEQUENCE,
     AL4SAN_THREAD_COUNT=TASK_THREAD_COUNT,
     AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=THREAD_SET_TO_MANUAL_SCHEDULING,
     AL4SAN_LOCK_TO_THREAD_MASK=TASK_LOCK_TO_THREAD_MASK, 
     AL4SAN_LABEL=TASK_LABEL, 
     AL4SAN_COLOR=TASK_COLOR}
     al4san_quark_task_flags_t;

typedef enum {  //arry
     AL4SAN_DATA_ARRAY=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_DATA_MODE_ARRAY=AL4SAN_QUARK_UNDEFINED,
               //args
     AL4SAN_CL_ARGS_NFREE=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_CL_ARGS=AL4SAN_QUARK_UNDEFINED,

     AL4SAN_REDUX=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_COMMUTE=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_SSEND=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_ACCESS_MODE_MAX=AL4SAN_QUARK_UNDEFINED,
               //callback
     AL4SAN_CALLBACK=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_CALLBACK_WITH_ARG=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_CALLBACK_ARG=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_QUARK_UNDEFINED,
               //rank
     AL4SAN_EXECUTE_ON_NODE=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_EXECUTE_ON_DATA=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_EXECUTE_ON_WORKER=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_WORKER_ORDER=AL4SAN_QUARK_UNDEFINED,

     AL4SAN_SCHED_CTX=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_HYPERVISOR_TAG=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_POSSIBLY_PARALLEL=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_FLOPS=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_TAG=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_TAG_ONLY=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_NODE_SELECTION_POLICY=AL4SAN_QUARK_UNDEFINED,
     
               //parsec
     AL4SAN_REF=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_AFFINITY=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_DONT_TRACK=AL4SAN_QUARK_UNDEFINED,
     AL4SAN_PASSED_BY_REF=AL4SAN_QUARK_UNDEFINED,
     
               //CUDA async option not defined in quark
     AL4SAN_CUDA_FLG=AL4SAN_QUARK_UNDEFINED
} al4san_quark_runtime_undefined;*/

//#define AL4SAN_CODELETS_NAME(name) CORE_##name##_func

 //TO be used instead of AL4SAN_CODELETS_NAME
//#define AL4SAN_TASK(name) QUARK_##name##_func

#define AL4SAN_QUARK_TASK_HEADER(name)

//callback
//#define AL4SAN_CALLBACK(name) NULL
#define al4san_quark_matrix_get_nx(num) NULL
#define al4san_quark_matrix_get_ny(num) NULL

/*#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define AL4SAN_CODELETS(...) GET_MACRO(__VA_ARGS__, AL4SAN_CODELETS3, AL4SAN_CODELETS2)(__VA_ARGS__)

#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
void cpu_func_name(AL4SAN_Quark_arg_list *al4san_arg); \
void CORE_##name##_func (Quark *quark) {    AL4SAN_Quark_arg_list al4san_arg; al4san_arg.quark=quark; cpu_func_name(&al4san_arg);} 

#define AL4SAN_CODELETS2(name,  cpu_func_name) \
void cpu_func_name(AL4SAN_Quark_arg_list *al4san_arg); \
void CORE_##name##_func (Quark *quark) { AL4SAN_Quark_arg_list al4san_arg; al4san_arg.quark=quark; cpu_func_name(&al4san_arg);}
*/

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

