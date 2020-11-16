/**
 *
 * @file openmp_task.h
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 * @brief AL4SAN OpenMP task header
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

//BEGIN_C_DECLS

#include "omp.h"

#include "control/al4san_common.h"
BEGIN_C_DECLS

#define AL4SAN_OPENMP_UNDEFINED  0xFF0000
#define AL4SAN_OPENMP_UNDEFINED_MASK 0x0000FF
//#define AL4SAN_DEP 101
#define ON 1
#define OFF 0
//#define ARG_END 0 
#define NUM_SHAREDS 8
#define AL4SAN_OPENMP_NMAXARGS 30
#define NUM_WORK 4

/*typedef enum { AL4SAN_INPUT=(1<<0),
     AL4SAN_OUTPUT=(1<<1),
     AL4SAN_INOUT=(AL4SAN_INPUT|AL4SAN_OUTPUT),
     AL4SAN_VALUE=(1<<2), 
    AL4SAN_SCRATCH=(1<<3)
    }al4san_openmp_dependency_t;

typedef enum { AL4SAN_PRIORITY=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_LABEL=AL4SAN_OPENMP_UNDEFINED
     }al4san_openmp_task_flags_t;

typedef enum { 
               //Data region
     AL4SAN_REGION_0=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_1=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_2=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_3=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_4=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_5=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_6=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_7=AL4SAN_OPENMP_UNDEFINED,
               //ldu region
     AL4SAN_REGION_L=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_D=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_REGION_U=AL4SAN_OPENMP_UNDEFINED,
               //flags
     AL4SAN_LOCK_TO_THREAD=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_SEQUENCE=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_THREAD_COUNT=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_LOCK_TO_THREAD_MASK=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_COLOR=AL4SAN_OPENMP_UNDEFINED,
               //data dep
     AL4SAN_NODEP=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_NOLOCALITY=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_ACCUMULATOR=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_NOACCUMULATOR=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_GATHERV=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_NOGATHERV=AL4SAN_OPENMP_UNDEFINED,
     
     AL4SAN_REF=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_AFFINITY=AL4SAN_OPENMP_UNDEFINED, 
     AL4SAN_DONT_TRACK=AL4SAN_OPENMP_UNDEFINED, 
     AL4SAN_PASSED_BY_REF=AL4SAN_OPENMP_UNDEFINED,     
                //arry
     AL4SAN_DATA_ARRAY=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_DATA_MODE_ARRAY=AL4SAN_OPENMP_UNDEFINED,
               //args
     AL4SAN_CL_ARGS_NFREE=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_CL_ARGS=AL4SAN_OPENMP_UNDEFINED,

     AL4SAN_REDUX=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_COMMUTE=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_SSEND=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_ACCESS_MODE_MAX=AL4SAN_OPENMP_UNDEFINED,
               //callback
     AL4SAN_CALLBACK=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_CALLBACK_WITH_ARG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_CALLBACK_ARG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_OPENMP_UNDEFINED,
               //rank
     AL4SAN_EXECUTE_ON_NODE=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_EXECUTE_ON_DATA=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_EXECUTE_ON_WORKER=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_WORKER_ORDER=AL4SAN_OPENMP_UNDEFINED,

     AL4SAN_SCHED_CTX=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_HYPERVISOR_TAG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_POSSIBLY_PARALLEL=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_FLOPS=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_TAG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_TAG_ONLY=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_NODE_SELECTION_POLICY=AL4SAN_OPENMP_UNDEFINED,
           //CUDA async option
     AL4SAN_CUDA_FLG=AL4SAN_OPENMP_UNDEFINED,
     AL4SAN_LOCALITY=AL4SAN_OPENMP_UNDEFINED
     } al4san_openmp_undefined;*/

//****************llvm struct*********************
typedef long kmp_intptr_t;
typedef int kmp_int32;

typedef char bool;

typedef struct ident {
    kmp_int32 reserved_1;   /**<  might be used in Fortran; see above  */
    kmp_int32 flags;        /**<  also f.flags; KMP_IDENT_xxx flags; KMP_IDENT_KMPC identifies this union member  */
    kmp_int32 reserved_2;   /**<  not really used in Fortran any more; see above */
#if USE_ITT_BUILD
                            /*  but currently used for storing region-specific ITT */
                            /*  contextual information. */
#endif /* USE_ITT_BUILD */
    kmp_int32 reserved_3;   /**< source[4] in Fortran, do not use for C++  */
    char const *psource;    /**< String describing the source location.
                            The string is composed of semi-colon separated fields which describe the source file,
                            the function and a pair of line numbers that delimit the construct.
                             */
} ident_t;

typedef struct kmp_depend_info {
   //kmp_intptr_t               base_addr;
     void* base_addr;
     size_t                     len;
     struct {
         bool                   in:1;
         bool                   out:1;
     } flags;
} kmp_depend_info_t;

struct kmp_task;
typedef kmp_int32 (* kmp_routine_entry_t)( kmp_int32, struct kmp_task * );

typedef struct shar{
void *ptr[NUM_SHAREDS];
void *workspace[NUM_WORK];
int workflage[NUM_WORK];
int pcounter;
int wkcounter;
}pshareds;

typedef struct kmp_task {                   /* GEH: Shouldn't this be aligned somehow? */
    pshareds*              shareds;            /**< pointer to block of pointers to shared vars   */
    //void * shareds;
    kmp_routine_entry_t routine;            /**< pointer to routine to call for executing task */
    kmp_int32           part_id;            /**< part id for the task                          */
    int ts;
    void *cl_arg;
    size_t cl_arg_size;
} kmp_task_t;

#ifdef __cplusplus
extern "C" {
#endif
kmp_int32  __kmpc_global_thread_num  ( ident_t * );
kmp_task_t*
__kmpc_omp_task_alloc( ident_t *loc_ref, kmp_int32 gtid, kmp_int32 flags,
                       size_t sizeof_kmp_task_t, size_t sizeof_shareds,
                       kmp_routine_entry_t task_entry );
void __kmpc_proxy_task_completed_ooo ( kmp_task_t *ptask );
kmp_int32 __kmpc_omp_task_with_deps ( ident_t *loc_ref, kmp_int32 gtid, kmp_task_t * new_task,
                                      kmp_int32 ndeps, kmp_depend_info_t *dep_list,
                                      kmp_int32 ndeps_noalias, kmp_depend_info_t *noalias_dep_list );
kmp_int32
__kmpc_omp_task( ident_t *loc_ref, kmp_int32 gtid, kmp_task_t * new_task );
#ifdef __cplusplus
}
#endif    

//************************************************

typedef struct al4san_openmp_arg_list_s
{
    kmp_task_t *task;
} AL4SAN_Openmp_arg_list;

typedef struct AL4SAN_Openmp_Task
{
  int num_arg;
  int arg_depenency[AL4SAN_OPENMP_NMAXARGS];
  int arg_size[AL4SAN_OPENMP_NMAXARGS];

} AL4SAN_Openmp_task_t;

struct al4san_openmp_pack_arg_data
{
        char *arg_buffer;
        size_t arg_buffer_size;
        size_t current_offset;
        int nargs;
};

#define AL4SAN_OPENMP_ASSERT_MSG(x, msg, ...) do {\
        if(!(x))\
        {\
        fprintf(stderr, "[al4san][abort][%s()@%s:%d] " msg "\n", __func__, __FILE__, __LINE__, ## __VA_ARGS__); \
        abort();\
        }\
}while(0)

#define AL4SAN_OPENMP_REALLOC(ptr, size) do { void *_new_ptr = realloc(ptr, size); AL4SAN_OPENMP_ASSERT_MSG(_new_ptr != NULL, "Cannot reallocate %ld bytes\n", (long) (size)); ptr=_new_ptr;} while (0)

//#define AL4SAN_CODELETS_NAME(name) OPENMP_##name##_func

 //TO be used instead of AL4SAN_CODELETS_NAME
//#define AL4SAN_TASK(name) OPENMP_##name##_func

#define AL4SAN_OPENMP_TASK_HEADER(name)

//callback
//#define AL4SAN_CALLBACK(name) NULL
#define al4san_openmp_matrix_get_nx(num) NULL
#define al4san_openmp_matrix_get_ny(num) NULL

/*#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define AL4SAN_CODELETS(...) GET_MACRO(__VA_ARGS__, AL4SAN_CODELETS3, AL4SAN_CODELETS2)(__VA_ARGS__)

#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
void cpu_func_name(AL4SAN_Openmp_arg_list *al4san_arg); \
int OPENMP_##name##_func (kmp_int32 gtid, kmp_task_t *task) { AL4SAN_Openmp_arg_list al4san_arg; al4san_arg.task=task; cpu_func_name(&al4san_arg); return 0; }

#define AL4SAN_CODELETS2(name,  cpu_func_name) \
void cpu_func_name(AL4SAN_Openmp_arg_list *al4san_arg); \
int OPENMP_##name##_func (kmp_int32 gtid, kmp_task_t *task) { AL4SAN_Openmp_arg_list al4san_arg; al4san_arg.task=task; cpu_func_name(&al4san_arg); return 0; }
*/

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user cpu function name
*/

 //TO be used instead of AL4SAN_CODELETS
#define AL4SAN_OPENMP_TASK_CPU(name, cpu_func_name)\
void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
int OPENMP_##name##_func (kmp_int32 gtid, kmp_task_t *task){\
  AL4SAN_arg_list al4san_arg; \
  AL4SAN_Openmp_arg_list openmparg;\
  openmparg.task=task;\
  al4san_arg.Openmp_arg_list= (AL4SAN_Openmp_arg_list*) &openmparg;\
  cpu_func_name(&al4san_arg);\
  al4san_openmp_workspace_destroy(&al4san_arg);\
  return 0; }

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user gpu function name
*/

#define AL4SAN_OPENMP_TASK_GPU(name, gpu_func_name)

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument cpu anf gpuuser function name
*/
 
#define AL4SAN_OPENMP_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
int OPENMP_##name##_func (kmp_int32 gtid, kmp_task_t *task){\
  AL4SAN_arg_list al4san_arg; \
  AL4SAN_Openmp_arg_list openmparg;\
  openmparg.task=task;\
  al4san_arg.Openmp_arg_list= (AL4SAN_Openmp_arg_list*) &openmparg;\
  cpu_func_name(&al4san_arg);\
  al4san_openmp_workspace_destroy(&al4san_arg);\
  return 0; }

END_C_DECLS

#endif /* _TASKS_H_ */
