/**
 *
 * @file parsec_task.h
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 * @brief AL4SAN PaRSEC task header
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


#include "control/al4san_common.h"


BEGIN_C_DECLS
#define AL4SAN_PARSEC_UNDEFINED 0x0000FF
#define AL4SAN_PARSEC_UNDEFINED_MASK 0XFF0000
//#define AL4SAN_DEP 101
#define ON 1
#define OFF 0
//#define ARG_END PARSEC_DTD_ARG_END 


/*typedef enum { AL4SAN_INPUT=INPUT,
     AL4SAN_OUTPUT=OUTPUT,
     AL4SAN_INOUT=INOUT,
     AL4SAN_VALUE=VALUE,
     AL4SAN_REF=REF,
     AL4SAN_SCRATCH=SCRATCH,
     AL4SAN_AFFINITY=AFFINITY, 
     AL4SAN_DONT_TRACK=DONT_TRACK, 
     AL4SAN_PASSED_BY_REF=PASSED_BY_REF
    }al4san_parsec_dependency_t;

typedef enum { AL4SAN_PRIORITY=1<<30,
     AL4SAN_LABEL=1<<31
     }al4san_parsec_task_flags_t;

typedef enum { 
               //Data region
     AL4SAN_REGION_0=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_1=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_2=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_3=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_4=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_5=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_6=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_7=AL4SAN_PARSEC_UNDEFINED,
               //ldu region
     AL4SAN_REGION_L=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_D=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_REGION_U=AL4SAN_PARSEC_UNDEFINED,
               //flags
     AL4SAN_LOCK_TO_THREAD=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_SEQUENCE=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_THREAD_COUNT=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_LOCK_TO_THREAD_MASK=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_COLOR=AL4SAN_PARSEC_UNDEFINED,
               //data dep
     AL4SAN_NODEP=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_NOLOCALITY=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_ACCUMULATOR=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_NOACCUMULATOR=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_GATHERV=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_NOGATHERV=AL4SAN_PARSEC_UNDEFINED,
          
                //arry
     AL4SAN_DATA_ARRAY=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_DATA_MODE_ARRAY=AL4SAN_PARSEC_UNDEFINED,
               //args
     AL4SAN_CL_ARGS_NFREE=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_CL_ARGS=AL4SAN_PARSEC_UNDEFINED,

     AL4SAN_REDUX=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_COMMUTE=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_SSEND=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_ACCESS_MODE_MAX=AL4SAN_PARSEC_UNDEFINED,
               //callback
     AL4SAN_CALLBACK=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_CALLBACK_WITH_ARG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_CALLBACK_ARG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_PARSEC_UNDEFINED,
               //rank
     AL4SAN_EXECUTE_ON_NODE=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_EXECUTE_ON_DATA=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_EXECUTE_ON_WORKER=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_WORKER_ORDER=AL4SAN_PARSEC_UNDEFINED,

     AL4SAN_SCHED_CTX=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_HYPERVISOR_TAG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_POSSIBLY_PARALLEL=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_FLOPS=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_TAG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_TAG_ONLY=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_NODE_SELECTION_POLICY=AL4SAN_PARSEC_UNDEFINED,
           //CUDA async option
     AL4SAN_CUDA_FLG=AL4SAN_PARSEC_UNDEFINED,
     AL4SAN_LOCALITY=AL4SAN_PARSEC_UNDEFINED
     } al4san_parsec_undefined;*/


/*typedef struct al4san_parsec_arg_list_s
     {
          parsec_task_t *this_task;
     } AL4SAN_Parsec_arg_list;
*/
typedef struct AL4SAN_Parsec_Task_Flags_s {
      int task_priority;
      char *task_label;
 }AL4SAN_Parsec_Task_Flags;

//#define AL4SAN_CODELETS_NAME(name) CORE_##name##_func

 //TO be used instead of AL4SAN_CODELETS_NAME
//#define AL4SAN_TASK(name) PARSEC_##name##_func

#define AL4SAN_PARSEC_TASK_HEADER(name)

//callback
//#define AL4SAN_CALLBACK(name) NULL
#define al4san_parsec_matrix_get_nx(num) NULL
#define al4san_parsec_matrix_get_ny(num) NULL

/*#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define AL4SAN_CODELETS(...) GET_MACRO(__VA_ARGS__, AL4SAN_CODELETS3, AL4SAN_CODELETS2)(__VA_ARGS__)

#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
 void cpu_func_name(AL4SAN_Parsec_arg_list *al4san_arg); \
 int CORE_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {   (void)es; AL4SAN_Parsec_arg_list al4san_arg; al4san_arg.this_task=this_task; cpu_func_name(&al4san_arg); return PARSEC_HOOK_RETURN_DONE;} 

#define AL4SAN_CODELETS2(name,  cpu_func_name) \
 void cpu_func_name(AL4SAN_Parsec_arg_list *al4san_arg); \
 int CORE_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {   (void)es; AL4SAN_Parsec_arg_list al4san_arg; al4san_arg.this_task=this_task; cpu_func_name(&al4san_arg); return PARSEC_HOOK_RETURN_DONE;} 
*/
 //TO be used instead of AL4SAN_CODELETS
/*#define AL4SAN_PARSEC_TASK_CPU(name, cpu_func_name)\
void cpu_func_name(AL4SAN_Parsec_arg_list *al4san_arg); \
int PARSEC_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {\
     (void)es; AL4SAN_arg_list *al4san_arg; \
     AL4SAN_Parsec_arg_list *parsecarg=al4san_arg->Parsec_arg_list;\
     parsecarg->this_task=this_task; cpu_func_name(al4san_arg);\
     return PARSEC_HOOK_RETURN_DONE;} 

#define AL4SAN_PARSEC_TASK_GPU(name, gpu_func_name)
 
#define AL4SAN_PARSEC_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
void cpu_func_name(AL4SAN_Parsec_arg_list *al4san_arg); \
int PARSEC_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {\
     (void)es; AL4SAN_arg_list *al4san_arg; \
     AL4SAN_Parsec_arg_list *parsecarg=al4san_arg->Parsec_arg_list;\
     parsecarg->this_task=this_task; cpu_func_name(al4san_arg);\
     return PARSEC_HOOK_RETURN_DONE;}
*/
 END_C_DECLS

#endif /* _TASKS_H_ */

