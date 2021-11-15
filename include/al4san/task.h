/**
 *
 * @file task.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
**
   * @brief AL4SAN task routines exposed to the users
   *
   * @version 1.1.0
   * @author Rabab Alomairy
   * @date 2019-02-06
   *
  **/
#ifndef _AL4SAN_TASK_H_
#define _AL4SAN_TASK_H_

#include "al4san/config.h"
#include "al4san/types.h"
#include "al4san/constants.h"
BEGIN_C_DECLS

//#define INIT_VALUE 111
extern int ARG_END;
extern int AL4SAN_DEP;
extern int AL4SAN_CUDA_ASYNC;
extern int AL4SAN_CUDA_FLG;
extern int AL4SAN_INPUT;
extern int AL4SAN_OUTPUT;
extern int AL4SAN_INOUT;
extern int AL4SAN_SCRATCH;
extern int AL4SAN_REDUX;
extern int AL4SAN_COMMUTE;
extern int AL4SAN_SSEND;
extern int AL4SAN_ACCESS_MODE_MAX;
extern int AL4SAN_VALUE;
extern int AL4SAN_DATA_ARRAY;
extern int AL4SAN_DATA_MODE_ARRAY;
extern int AL4SAN_CL_ARGS_NFREE;
extern int AL4SAN_CL_ARGS;
extern int AL4SAN_CALLBACK;
extern int AL4SAN_CALLBACK_WITH_ARG;
extern int AL4SAN_CALLBACK_ARG;
extern int AL4SAN_PROLOGUE_CALLBACK;
extern int AL4SAN_PROLOGUE_CALLBACK_ARG;
extern int AL4SAN_PROLOGUE_CALLBACK_POP;
extern int AL4SAN_PROLOGUE_CALLBACK_POP_ARG;
extern int AL4SAN_EXECUTE_ON_NODE;
extern int AL4SAN_EXECUTE_ON_DATA;
extern int AL4SAN_EXECUTE_ON_WORKER;
extern int AL4SAN_WORKER_ORDER;
extern int AL4SAN_SCHED_CTX;
extern int AL4SAN_HYPERVISOR_TAG;
extern int AL4SAN_POSSIBLY_PARALLEL;
extern int AL4SAN_FLOPS;
extern int AL4SAN_TAG;
extern int AL4SAN_TAG_ONLY;
extern int AL4SAN_NODE_SELECTION_POLICY;
extern int AL4SAN_TASK_DEPS_ARRAY;
extern int AL4SAN_EXECUTE_WHERE;
extern int AL4SAN_COLOR;
extern int AL4SAN_LOCALITY;
extern int AL4SAN_PRIORITY;
extern int AL4SAN_LABEL;
extern int AL4SAN_REGION_0;
extern int AL4SAN_REGION_1;
extern int AL4SAN_REGION_2;
extern int AL4SAN_REGION_3;
extern int AL4SAN_REGION_4;
extern int AL4SAN_REGION_5;
extern int AL4SAN_REGION_6;
extern int AL4SAN_REGION_7;
               //ldu region
extern int AL4SAN_REGION_L;
extern int AL4SAN_REGION_D;
extern int AL4SAN_REGION_U;
               //flags
extern int AL4SAN_LOCK_TO_THREAD;
extern int AL4SAN_SEQUENCE;
extern int AL4SAN_THREAD_COUNT;
extern int AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING;
extern int AL4SAN_LOCK_TO_THREAD_MASK;
//int AL4SAN_COLOR = INIT_VALUE;
               //data dep
extern int AL4SAN_NODEP;
extern int AL4SAN_NOLOCALITY;
extern int AL4SAN_ACCUMULATOR;
extern int AL4SAN_NOACCUMULATOR;
extern int AL4SAN_GATHERV;
extern int AL4SAN_NOGATHERV;
               //parsec
extern int AL4SAN_REF;
extern int AL4SAN_AFFINITY; 
extern int AL4SAN_DONT_TRACK;
extern int AL4SAN_PASSED_BY_REF;

extern int AL4SAN_FULL_TILE;

typedef struct al4san_arg_list_s
{
    void* Quark_arg_list;
    void* Starpu_arg_list;
    void* Parsec_arg_list;
    void* Openmp_arg_list;

} AL4SAN_arg_list;

#define GET_MACRO_ADDR(_1,_2,_3,_4, NAME,...) NAME
#define AL4SAN_ADDR(...) GET_MACRO_ADDR(__VA_ARGS__, AL4SAN_ADDR4, AL4SAN_ADDR3, AL4SAN_ADDR2)(__VA_ARGS__)


#define AL4SAN_ADDR4( desc, type, m, n ) ( (type*)AL4SAN_Data_getaddr( desc, m, n ) ), desc->schedopt
#define AL4SAN_ADDR3( desc, type, m) ( (type*)AL4SAN_Data_getaddr( desc, m, 0 ) ), desc->schedopt
#define AL4SAN_ADDR2( desc, type) ( (type*)AL4SAN_Data_getaddr( desc, 0, 0 ) ), desc->schedopt

#define AL4SAN_TASK_HEADER(name)\
	AL4SAN_QUARK_TASK_HEADER(name)\
	AL4SAN_STARPU_TASK_HEADER(name)\
        AL4SAN_OPENMP_TASK_HEADER(name)

#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
extern void *QUARK_func, *PARSEC_func, *OPENMP_func;
#define AL4SAN_TASK(name) QUARK_func, &cl_##name, PARSEC_func, OPENMP_func
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
extern void *OPENMP_func;
#define AL4SAN_TASK(name) QUARK_##name##_func, &cl_##name, PARSEC_##name##_func, OPENMP_func
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
extern void *OPENMP_func, *PARSEC_func;
#define AL4SAN_TASK(name) QUARK_##name##_func, &cl_##name, PARSEC_func, OPENMP_func
#endif

#if !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
extern void *QUARK_func, *STARPU_func, *OPENMP_func;
#define AL4SAN_TASK(name) QUARK_func, STARPU_func, PARSEC_##name##_func, OPENMP_func
#endif

#if !defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
extern void *OPENMP_func, *STARPU_func, *PARSEC_func;
#define AL4SAN_TASK(name) QUARK_##name##_func, STARPU_func, PARSEC_func, OPENMP_func
#endif

#if !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
extern void *QUARK_func, *STARPU_func, *PARSEC_func;
#define AL4SAN_TASK(name) QUARK_func, STARPU_func, PARSEC_func,  OPENMP_##name##_func
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK(name) QUARK_##name##_func, &cl_##name, PARSEC_##name##_func, OPENMP_##name##_func
#endif


// STARPU
#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_STARPU_TASK_CPU(name, cpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_STARPU_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_STARPU_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

// QUARK
#if defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_QUARK_TASK_CPU(name, cpu_func_name)
#endif

#if defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_QUARK_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_QUARK_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

// PARSEC
#if defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU(name, cpu_func_name)
#endif

#if defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

// OPENMP
#if defined(AL4SAN_SCHED_OPENMP) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_OPENMP_TASK_CPU(name, cpu_func_name)
#endif

#if defined(AL4SAN_SCHED_OPENMP) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_OPENMP_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_OPENMP) && !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED__PARSEC)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_OPENMP_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_QUARK_TASK_CPU(name, cpu_func_name)\
        AL4SAN_STARPU_TASK_CPU(name, cpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU(name, cpu_func_name)\
        AL4SAN_OPENMP_TASK_CPU(name, cpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_QUARK_TASK_CPU(name, cpu_func_name)\
        AL4SAN_STARPU_TASK_CPU(name, cpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU(name, cpu_func_name)
#endif
//starpu and quark
#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU(name, cpu_func_name)\
        AL4SAN_QUARK_TASK_CPU(name, cpu_func_name)\
        AL4SAN_STARPU_TASK_CPU(name, cpu_func_name)
#endif
#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_QUARK_TASK_GPU(name, gpu_func_name)\
        AL4SAN_STARPU_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_QUARK_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_STARPU_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_QUARK_TASK_GPU(name, gpu_func_name)\
        AL4SAN_STARPU_TASK_GPU(name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_GPU(name, gpu_func_name)\
        AL4SAN_OPENMP_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_GPU(name, gpu_func_name)\
        AL4SAN_QUARK_TASK_GPU(name, gpu_func_name)\
        AL4SAN_STARPU_TASK_GPU(name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_GPU(name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_QUARK_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_STARPU_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_OPENMP_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_QUARK_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_STARPU_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
        AL4SAN_PARSEC_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_getStream(_stream_)\
        AL4SAN_QUARK_getStream(_stream_)\
        AL4SAN_Starpu_getStream(_stream_)\
        AL4SAN_OPENMP_getStream(_stream_)\
        AL4SAN_PARSEC_getStream(_stream_)
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) &&  defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_getStream(_stream_)\
        AL4SAN_QUARK_getStream(_stream_)\
        AL4SAN_Starpu_getStream(_stream_)\
        AL4SAN_PARSEC_getStream(_stream_)
#endif

#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_getStream(_stream_)\
        AL4SAN_Starpu_getStream(_stream_)
#endif  

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) &&  !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#define AL4SAN_getStream(_stream_)\
        AL4SAN_QUARK_getStream(_stream_)\
        AL4SAN_Starpu_getStream(_stream_)
#endif   
END_C_DECLS

#endif /* __AL4SAN_H__ */
