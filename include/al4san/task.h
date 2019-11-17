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
 */
/**
   *
   * @brief AL4SAN task options
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.1
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
int ARG_END;
int AL4SAN_DEP;
int AL4SAN_CUDA_ASYNC;
int AL4SAN_CUDA_FLG;
int AL4SAN_INPUT;
int AL4SAN_OUTPUT;
int AL4SAN_INOUT;
int AL4SAN_SCRATCH;
int AL4SAN_REDUX;
int AL4SAN_COMMUTE;
int AL4SAN_SSEND;
int AL4SAN_ACCESS_MODE_MAX;
int AL4SAN_VALUE;
int AL4SAN_DATA_ARRAY;
int AL4SAN_DATA_MODE_ARRAY;
int AL4SAN_CL_ARGS_NFREE;
int AL4SAN_CL_ARGS;
int AL4SAN_CALLBACK;
int AL4SAN_CALLBACK_WITH_ARG;
int AL4SAN_CALLBACK_ARG;
int AL4SAN_PROLOGUE_CALLBACK;
int AL4SAN_PROLOGUE_CALLBACK_ARG;
int AL4SAN_PROLOGUE_CALLBACK_POP;
int AL4SAN_PROLOGUE_CALLBACK_POP_ARG;
int AL4SAN_EXECUTE_ON_NODE;
int AL4SAN_EXECUTE_ON_DATA;
int AL4SAN_EXECUTE_ON_WORKER;
int AL4SAN_WORKER_ORDER;
int AL4SAN_SCHED_CTX;
int AL4SAN_HYPERVISOR_TAG;
int AL4SAN_POSSIBLY_PARALLEL;
int AL4SAN_FLOPS;
int AL4SAN_TAG;
int AL4SAN_TAG_ONLY;
int AL4SAN_NODE_SELECTION_POLICY;
int AL4SAN_TASK_DEPS_ARRAY;
int AL4SAN_EXECUTE_WHERE;
int AL4SAN_COLOR;
int AL4SAN_LOCALITY;
int AL4SAN_PRIORITY;
int AL4SAN_LABEL;
int AL4SAN_REGION_0;
int AL4SAN_REGION_1;
int AL4SAN_REGION_2;
int AL4SAN_REGION_3;
int AL4SAN_REGION_4;
int AL4SAN_REGION_5;
int AL4SAN_REGION_6;
int AL4SAN_REGION_7;
               //ldu region
int AL4SAN_REGION_L;
int AL4SAN_REGION_D;
int AL4SAN_REGION_U;
               //flags
int AL4SAN_LOCK_TO_THREAD;
int AL4SAN_SEQUENCE;
int AL4SAN_THREAD_COUNT;
int AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING;
int AL4SAN_LOCK_TO_THREAD_MASK;
//int AL4SAN_COLOR = INIT_VALUE;
               //data dep
int AL4SAN_NODEP;
int AL4SAN_NOLOCALITY;
int AL4SAN_ACCUMULATOR;
int AL4SAN_NOACCUMULATOR;
int AL4SAN_GATHERV;
int AL4SAN_NOGATHERV;
               //parsec
int AL4SAN_REF;
int AL4SAN_AFFINITY; 
int AL4SAN_DONT_TRACK;
int AL4SAN_PASSED_BY_REF;

//typedef void (al4san_codelet) (Quark *);
typedef struct al4san_arg_list_s
{
    void* Quark_arg_list;
    void* Starpu_arg_list;
    void* Parsec_arg_list;
    void* Openmp_arg_list;

} AL4SAN_arg_list;

#define GET_MACRO_ADDR(_1,_2,_3,_4, NAME,...) NAME
#define AL4SAN_ADDR(...) GET_MACRO_ADDR(__VA_ARGS__, AL4SAN_ADDR4, AL4SAN_ADDR3, AL4SAN_ADDR2)(__VA_ARGS__)

#define AL4SAN_ADDR4( desc, type, m, n ) ( (type*)AL4SAN_Data_getaddr( desc, m, n ) )
#define AL4SAN_ADDR3( desc, type, m) ( (type*)AL4SAN_Data_getaddr( desc, m, 0 ) )
#define AL4SAN_ADDR2( desc, type) ( (type*)AL4SAN_Data_getaddr( desc, 0, 0 ) )

#define AL4SAN_TASK_HEADER(name)\
	AL4SAN_QUARK_TASK_HEADER(name)\
	AL4SAN_STARPU_TASK_HEADER(name)\
        AL4SAN_OPENMP_TASK_HEADER(name)

#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
void *QUARK_func, *PARSEC_func, *OPENMP_func;
#define AL4SAN_TASK(name) QUARK_func, &cl_##name, PARSEC_func, OPENMP_func
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
void *OPENMP_func;
#define AL4SAN_TASK(name) QUARK_##name##_func, &cl_##name, PARSEC_##name##_func, OPENMP_func
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
void *OPENMP_func, *PARSEC_func;
#define AL4SAN_TASK(name) QUARK_##name##_func, &cl_##name, PARSEC_func, OPENMP_func
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
