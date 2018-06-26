/**
 *
 * @file runtime_task.h
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL Quark task header
 *
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/
#ifndef _TASKS_H_
#define _TASKS_H_

#include <quark.h>
#include<icl_list.h>

#include "control/common.h"

BEGIN_C_DECLS
#define ALTANAL_undefined 1 << 25 
#define ON 1
#define OFF 0

#define ALTANAL_NMAXARGS 30
#define ARG_END 0

typedef struct ALTANAL_Task
{
 int num_arg;
 int arg_depenency[ALTANAL_NMAXARGS];	
 int arg_size[ALTANAL_NMAXARGS];

} ALTANAL_task_t;

/*typedef struct ALTANAL_s
{
 Quark *quark;
} ALTANAL;*/

typedef struct altanal_arg_list_s
{
    Quark *quark;
} ALTANAL_arg_list;


typedef enum { ALTANAL_INPUT=INPUT,
     ALTANAL_OUTPUT=OUTPUT,
     ALTANAL_INOUT=INOUT,
     ALTANAL_VALUE=VALUE,
     ALTANAL_NODEP=NODEP,
     ALTANAL_SCRATCH=SCRATCH,
     ALTANAL_LOCALITY=LOCALITY,
     ALTANAL_NOLOCALITY=NOLOCALITY, 
     ALTANAL_ACCUMULATOR=ACCUMULATOR, 
     ALTANAL_NOACCUMULATOR=NOACCUMULATOR,
     ALTANAL_GATHERV=GATHERV,
     ALTANAL_NOGATHERV=NOGATHERV
}altanal_dependency_t;

typedef enum { ALTANAL_REGION_0=QUARK_REGION_0, 
     ALTANAL_REGION_1=QUARK_REGION_1, 
     ALTANAL_REGION_2=QUARK_REGION_2, 
     ALTANAL_REGION_3=QUARK_REGION_3,
     ALTANAL_REGION_4=QUARK_REGION_4, 
     ALTANAL_REGION_5=QUARK_REGION_5, 
     ALTANAL_REGION_6=QUARK_REGION_6, 
     ALTANAL_REGION_7=QUARK_REGION_7
}altanal_data_region_t;

typedef enum { ALTANAL_REGION_L=QUARK_REGION_0|QUARK_REGION_1|QUARK_REGION_2,
     ALTANAL_REGION_D=QUARK_REGION_3|QUARK_REGION_4,
     ALTANAL_REGION_U=QUARK_REGION_5|QUARK_REGION_6|QUARK_REGION_7 
} altanal_ldu_region_t;

typedef enum { ALTANAL_PRIORITY=TASK_PRIORITY,
     ALTANAL_LOCK_TO_THREAD=TASK_LOCK_TO_THREAD,
     ALTANAL_SEQUENCE=TASK_SEQUENCE,
     ALTANAL_THREAD_COUNT=TASK_THREAD_COUNT,
     ALTANAL_THREAD_SET_TO_MANUAL_SCHEDULING=THREAD_SET_TO_MANUAL_SCHEDULING,
     ALTANAL_LOCK_TO_THREAD_MASK=TASK_LOCK_TO_THREAD_MASK, 
     ALTANAL_LABEL=TASK_LABEL, 
     ALTANAL_COLOR=TASK_COLOR}
     altanal_task_flags_t;
typedef enum {  //arry
     ALTANAL_DATA_ARRAY=ALTANAL_undefined,
     ALTANAL_DATA_MODE_ARRAY=ALTANAL_undefined,
               //args
     ALTANAL_CL_ARGS_NFREE=ALTANAL_undefined,
     ALTANAL_CL_ARGS=ALTANAL_undefined,

     ALTANAL_REDUX=ALTANAL_undefined,
     ALTANAL_COMMUTE=ALTANAL_undefined,
     ALTANAL_SSEND=ALTANAL_undefined,
     ALTANAL_ACCESS_MODE_MAX=ALTANAL_undefined,
               //callback
     ALTANAL_CALLBACK=ALTANAL_undefined,
     ALTANAL_CALLBACK_WITH_ARG=ALTANAL_undefined,
     ALTANAL_CALLBACK_ARG=ALTANAL_undefined,
     ALTANAL_PROLOGUE_CALLBACK=ALTANAL_undefined,
     ALTANAL_PROLOGUE_CALLBACK_ARG=ALTANAL_undefined,
     ALTANAL_PROLOGUE_CALLBACK_POP=ALTANAL_undefined,
     ALTANAL_PROLOGUE_CALLBACK_POP_ARG=ALTANAL_undefined,
               //rank
     ALTANAL_EXECUTE_ON_NODE=ALTANAL_undefined,
     ALTANAL_EXECUTE_ON_DATA=ALTANAL_undefined,
     ALTANAL_EXECUTE_ON_WORKER=ALTANAL_undefined,
     ALTANAL_WORKER_ORDER=ALTANAL_undefined,

     ALTANAL_SCHED_CTX=ALTANAL_undefined,
     ALTANAL_HYPERVISOR_TAG=ALTANAL_undefined,
     ALTANAL_POSSIBLY_PARALLEL=ALTANAL_undefined,
     ALTANAL_FLOPS=ALTANAL_undefined,
     ALTANAL_TAG=ALTANAL_undefined,
     ALTANAL_TAG_ONLY=ALTANAL_undefined,
     ALTANAL_NODE_SELECTION_POLICY=ALTANAL_undefined,
     
               //parsec
     ALTANAL_REF=ALTANAL_undefined,
     ALTANAL_AFFINITY=ALTANAL_undefined,
     ALTANAL_DONT_TRACK=ALTANAL_undefined,
     ALTANAL_PASSED_BY_REF=ALTANAL_undefined,
     
               //CUDA async option not defined in quark
     ALTANAL_CUDA_FLG=ALTANAL_undefined
} altanal_runtime_undefined;

#define ALTANAL_CODELETS_NAME(name) CORE_##name##_func

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define ALTANAL_CODELETS(...) GET_MACRO(__VA_ARGS__, ALTANAL_CODELETS3, ALTANAL_CODELETS2)(__VA_ARGS__)

#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
void cpu_func_name(ALTANAL); \
void CORE_##name##_func (Quark *quark) {    ALTANAL_arg_list altanal_arg; altanal_arg.quark=quark; cpu_func_name(&altanal_arg);} 

#define ALTANAL_CODELETS2(name,  cpu_func_name) \
void cpu_func_name(ALTANAL); \
void CORE_##name##_func (Quark *quark) { ALTANAL_arg_list altanal_arg; altanal_arg.quark=quark; cpu_func_name(&altanal_arg);}

#define ALTANAL_CALLBACK(name) NULL
#define altanal_matrix_get_nx(num) NULL
#define altanal_matrix_get_ny(num) NULL

END_C_DECLS
#endif /* _TASKS_H_ */

