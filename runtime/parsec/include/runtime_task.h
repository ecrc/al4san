/**
 *
 * @file runtime_task.h
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL PaRSEC task header
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


#include "control/common.h"


BEGIN_C_DECLS
#define ALTANAL_undefined 1<<35
#define ON 1
#define OFF 0
#define ARG_END PARSEC_DTD_ARG_END 


typedef enum { ALTANAL_INPUT=INPUT,
     ALTANAL_OUTPUT=OUTPUT,
     ALTANAL_INOUT=INOUT,
     ALTANAL_VALUE=VALUE,
     ALTANAL_REF=REF,
     ALTANAL_SCRATCH=SCRATCH,
     ALTANAL_AFFINITY=AFFINITY, 
     ALTANAL_DONT_TRACK=DONT_TRACK, 
     ALTANAL_PASSED_BY_REF=PASSED_BY_REF}
     altanal_dependency_t;

     typedef enum { ALTANAL_PRIORITY=1<<30,
          ALTANAL_LABEL=1<<31
     }altanal_task_flags_t;

     typedef enum { 
               //Data region
          ALTANAL_REGION_0=ALTANAL_undefined,
          ALTANAL_REGION_1=ALTANAL_undefined,
          ALTANAL_REGION_2=ALTANAL_undefined,
          ALTANAL_REGION_3=ALTANAL_undefined,
          ALTANAL_REGION_4=ALTANAL_undefined,
          ALTANAL_REGION_5=ALTANAL_undefined,
          ALTANAL_REGION_6=ALTANAL_undefined,
          ALTANAL_REGION_7=ALTANAL_undefined,
               //ldu region
          ALTANAL_REGION_L=ALTANAL_undefined,
          ALTANAL_REGION_D=ALTANAL_undefined,
          ALTANAL_REGION_U=ALTANAL_undefined,
               //flags
          ALTANAL_LOCK_TO_THREAD=ALTANAL_undefined,
          ALTANAL_SEQUENCE=ALTANAL_undefined,
          ALTANAL_THREAD_COUNT=ALTANAL_undefined,
          ALTANAL_THREAD_SET_TO_MANUAL_SCHEDULING=ALTANAL_undefined,
          ALTANAL_LOCK_TO_THREAD_MASK=ALTANAL_undefined,
          ALTANAL_COLOR=ALTANAL_undefined,
               //data dep
          ALTANAL_NODEP=ALTANAL_undefined,
          ALTANAL_NOLOCALITY=ALTANAL_undefined,
          ALTANAL_ACCUMULATOR=ALTANAL_undefined,
          ALTANAL_NOACCUMULATOR=ALTANAL_undefined,
          ALTANAL_GATHERV=ALTANAL_undefined,
          ALTANAL_NOGATHERV=ALTANAL_undefined,
          
                //arry
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
          
               //CUDA async option
          ALTANAL_CUDA_FLG=ALTANAL_undefined
     } altanal_runtime_undefined;


     typedef struct altanal_arg_list_s
     {
          parsec_task_t *this_task
     } ALTANAL_arg_list;

     typedef struct ALTANAL_Task_Flags_s {
      int task_priority;
      char *task_label;
 }ALTANAL_Task_Flags;

#define ALTANAL_CODELETS_NAME(name) CORE_##name##_func

//callback
#define ALTANAL_CALLBACK(name) NULL
#define altanal_matrix_get_nx(num) NULL
#define altanal_matrix_get_ny(num) NULL

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define ALTANAL_CODELETS(...) GET_MACRO(__VA_ARGS__, ALTANAL_CODELETS3, ALTANAL_CODELETS2)(__VA_ARGS__)

#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
 void cpu_func_name(ALTANAL); \
 int CORE_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {   (void)es; ALTANAL_arg_list altanal_arg; altanal_arg.this_task=this_task; cpu_func_name(&altanal_arg); return PARSEC_HOOK_RETURN_DONE;} 

#define ALTANAL_CODELETS2(name,  cpu_func_name) \
 void cpu_func_name(ALTANAL); \
 int CORE_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {   (void)es; ALTANAL_arg_list altanal_arg; altanal_arg.this_task=this_task; cpu_func_name(&altanal_arg); return PARSEC_HOOK_RETURN_DONE;} 

 END_C_DECLS

#endif /* _TASKS_H_ */

