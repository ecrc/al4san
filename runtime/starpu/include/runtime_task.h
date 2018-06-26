/**
 *
 * @file runtime_task.h
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL StarPU task header
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

#if defined(TANAL_USE_MPI)
#include "control/common.h"
#include <starpu_mpi.h>
#else
#include <starpu.h>
#endif
#include "runtime_codelet.h"

BEGIN_C_DECLS
static void (*pre_submit_hook)(struct starpu_task *task) = NULL;

#define ARG_END 0   
#define ON 1
#define OFF 0
#define ALTANAL_CUDA_ASYNC STARPU_CUDA_ASYNC
#define ALTANAL_undefined (1>>35) 
#define  ALTANAL_CUDA_FLG ((11ULL)<<10) 

typedef enum { ALTANAL_INPUT=STARPU_R,
 ALTANAL_OUTPUT=STARPU_W,
 ALTANAL_INOUT=STARPU_RW,
 ALTANAL_VALUE=STARPU_VALUE,
 ALTANAL_DATA_ARRAY=STARPU_DATA_ARRAY,
 ALTANAL_DATA_MODE_ARRAY=STARPU_DATA_MODE_ARRAY,
 ALTANAL_CL_ARGS_NFREE=STARPU_CL_ARGS_NFREE,
 ALTANAL_SCRATCH=STARPU_SCRATCH,
 ALTANAL_REDUX=STARPU_REDUX,
 ALTANAL_COMMUTE=STARPU_COMMUTE,
 ALTANAL_SSEND=STARPU_SSEND,
 ALTANAL_LOCALITY=STARPU_LOCALITY,
 ALTANAL_ACCESS_MODE_MAX=STARPU_ACCESS_MODE_MAX,
 ALTANAL_CL_ARGS=STARPU_CL_ARGS,
 ALTANAL_CALLBACK=STARPU_CALLBACK,
 ALTANAL_CALLBACK_WITH_ARG=STARPU_CALLBACK_WITH_ARG,
 ALTANAL_CALLBACK_ARG=STARPU_CALLBACK_ARG,
 ALTANAL_PROLOGUE_CALLBACK=STARPU_PROLOGUE_CALLBACK,
 ALTANAL_PROLOGUE_CALLBACK_ARG=STARPU_PROLOGUE_CALLBACK_ARG,
 ALTANAL_PROLOGUE_CALLBACK_POP=STARPU_PROLOGUE_CALLBACK_POP,
 ALTANAL_PROLOGUE_CALLBACK_POP_ARG=STARPU_PROLOGUE_CALLBACK_POP_ARG,
 ALTANAL_EXECUTE_ON_NODE=STARPU_EXECUTE_ON_NODE,
 ALTANAL_EXECUTE_ON_DATA=STARPU_EXECUTE_ON_DATA,
 ALTANAL_EXECUTE_ON_WORKER=STARPU_EXECUTE_ON_WORKER,
 ALTANAL_WORKER_ORDER=STARPU_WORKER_ORDER,
 ALTANAL_SCHED_CTX=STARPU_SCHED_CTX,
 ALTANAL_HYPERVISOR_TAG=STARPU_HYPERVISOR_TAG,
 ALTANAL_POSSIBLY_PARALLEL=STARPU_POSSIBLY_PARALLEL,
 ALTANAL_FLOPS=STARPU_FLOPS,
 ALTANAL_TAG=STARPU_TAG,
 ALTANAL_TAG_ONLY=STARPU_TAG_ONLY,
 ALTANAL_NODE_SELECTION_POLICY=STARPU_NODE_SELECTION_POLICY,
               //ALTANAL_CUDA_ASYNC=STARPU_CUDA_ASYNC 
}altanal_dependency_t;


typedef enum { ALTANAL_PRIORITY=STARPU_PRIORITY,
#if defined(ALTANAL_ALTANAL_CODELETS_HAVE_NAME)
 ALTANAL_LABEL=STARPU_NAME
#else
 ALTANAL_LABEL=ALTANAL_undefined
#endif
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
               //parsec
 ALTANAL_REF=ALTANAL_undefined,
 ALTANAL_AFFINITY=ALTANAL_undefined, 
 ALTANAL_DONT_TRACK=ALTANAL_undefined, 
 ALTANAL_PASSED_BY_REF=ALTANAL_undefined
} altanal_runtime_undefined;
#define ALTANAL_NMAXARGS 30

typedef struct ALTANAL_Task
{
  int num_arg;
  int arg_depenency[ALTANAL_NMAXARGS];  
  int arg_size[ALTANAL_NMAXARGS];

} ALTANAL_task_t;


typedef struct altanal_arg_list_s
{
  void *cl_arg;    
  void **descr;
}ALTANAL_arg_list;

/*
starpu task macro
*/

#define  _ALTANAL_CODELETS_CHECK() do {} while(0)

//starpu codelelts
#define ALTANAL_CODELETS_NAME(n) &cl_##n

//callback
#define ALTANAL_CALLBACK(name) options->profiling ? cl_##name##_callback : NULL
#define altanal_matrix_get_nx(num) starpu_matrix_get_nx(task->handles[num])
#define altanal_matrix_get_ny(num) starpu_matrix_get_ny(task->handles[num])

#define _STARPU_MSG(fmt, ...) do { fprintf(stderr, STARPU_DEBUG_PREFIX"[%s] " fmt ,__starpu_func__ ,## __VA_ARGS__); } while(0)
/****************/
#if defined(__KNC__) || defined(__KNF__)
#define STARPU_DEBUG_PREFIX "[starpu-mic]"
#else
#define STARPU_DEBUG_PREFIX "[starpu]"
#endif

#define _STARPU_DISP(fmt, ...) do { if (!_starpu_silent) {fprintf(stderr, STARPU_DEBUG_PREFIX"[%s] " fmt ,__starpu_func__ ,## __VA_ARGS__); }} while(0)

/********************/
#define _STARPU_FUT_TASK_BUILD_START    0x5170
#define _STARPU_FUT_TASK_BUILD_END      0x5171

#ifdef STARPU_USE_FXT
#define _STARPU_TRACE_TASK_BUILD_START()        \
FUT_DO_PROBE1(_STARPU_FUT_TASK_BUILD_START, _starpu_gettid());
#define _STARPU_TRACE_TASK_BUILD_END()  \
FUT_DO_PROBE1(_STARPU_FUT_TASK_BUILD_END, _starpu_gettid());
#else // !STARPU_USE_FXT
#define _STARPU_TRACE_TASK_BUILD_START()                do {} while(0)
#define _STARPU_TRACE_TASK_BUILD_END()                  do {} while(0)
#endif


#if defined(ALTANAL_USE_MPI)
extern int _starpu_debug_rank;

#ifdef STARPU_USE_FXT
#define _STARPU_TRACE_TASK_MPI_DECODE_START() \
FUT_DO_PROBE1(_STARPU_FUT_TASK_MPI_DECODE_START, _starpu_gettid());
#define _STARPU_TRACE_TASK_MPI_DECODE_END() \
FUT_DO_PROBE1(_STARPU_FUT_TASK_MPI_DECODE_END, _starpu_gettid());
#define _STARPU_TRACE_TASK_MPI_PRE_START()	\
FUT_DO_PROBE1(_STARPU_FUT_TASK_MPI_PRE_START, _starpu_gettid());
#define _STARPU_TRACE_TASK_MPI_PRE_END()	\
FUT_DO_PROBE1(_STARPU_FUT_TASK_MPI_PRE_END, _starpu_gettid());
#else // !STARPU_USE_FXT
#define _STARPU_TRACE_TASK_MPI_DECODE_START()   do {} while(0)
#define _STARPU_TRACE_TASK_MPI_DECODE_END()   do {} while(0)
#define _STARPU_TRACE_TASK_MPI_PRE_START()		do {} while(0)
#define _STARPU_TRACE_TASK_MPI_PRE_END()		do {} while(0)
#endif

#define _STARPU_MPI_MALLOC(ptr, size) do { ptr = malloc(size); STARPU_MPI_ASSERT_MSG(ptr != NULL, "Cannot allocate %ld bytes\n", (long) size); } while (0)
#define _STARPU_MPI_REALLOC(ptr, size) do { void *_new_ptr = realloc(ptr, size); STARPU_MPI_ASSERT_MSG(_new_ptr != NULL, "Cannot reallocate %ld bytes\n", (long) size); ptr = _new_ptr; } while (0)

#ifdef STARPU_MPI_VERBOSE
  #  define _STARPU_MPI_DEBUG(level, fmt, ...) \
do \
{               \
  if (!_starpu_silent && _starpu_debug_level_min <= level && level <= _starpu_debug_level_max)  \
    {             \
      if (_starpu_debug_rank == -1) starpu_mpi_comm_rank(MPI_COMM_WORLD, &_starpu_debug_rank); \
      fprintf(stderr, "%*s[%d][starpu_mpi][%s:%d] " fmt , (_starpu_debug_rank+1)*4, "", _starpu_debug_rank, __starpu_func__ , __LINE__,## __VA_ARGS__); \
      fflush(stderr); \
    }     \
  } while(0);
#else 
#  define _STARPU_MPI_DEBUG(level, fmt, ...)    do { } while(0) 
#endif

#ifdef STARPU_MPI_EXTRA_VERBOSE
#  define _STARPU_MPI_LOG_IN()             do { if (!_starpu_silent) { \
  if (_starpu_debug_rank == -1) starpu_mpi_comm_rank(MPI_COMM_WORLD, &_starpu_debug_rank);                        \
  fprintf(stderr, "%*s[%d][starpu_mpi][%s:%d] -->\n", (_starpu_debug_rank+1)*4, "", _starpu_debug_rank, __starpu_func__ , __LINE__); \
  fflush(stderr); }} while(0)
#  define _STARPU_MPI_LOG_OUT()            do { if (!_starpu_silent) { \
  if (_starpu_debug_rank == -1) starpu_mpi_comm_rank(MPI_COMM_WORLD, &_starpu_debug_rank);                        \
  fprintf(stderr, "%*s[%d][starpu_mpi][%s:%d] <--\n", (_starpu_debug_rank+1)*4, "", _starpu_debug_rank, __starpu_func__, __LINE__ ); \
  fflush(stderr); }} while(0)
#else
#  define _STARPU_MPI_LOG_IN()
#  define _STARPU_MPI_LOG_OUT()
#endif

#ifdef STARPU_NO_ASSERT
#  define STARPU_MPI_ASSERT_MSG(x, msg, ...)	do { if (0) { (void) (x); }} while(0)
#else
#  if defined(__CUDACC__) && defined(STARPU_HAVE_WINDOWS)
  int _starpu_debug_rank;
#    define STARPU_MPI_ASSERT_MSG(x, msg, ...)									\
  do													\
  { 													\
    if (STARPU_UNLIKELY(!(x))) 									\
      {												\
       if (_starpu_debug_rank == -1) starpu_mpi_comm_rank(MPI_COMM_WORLD, &_starpu_debug_rank); \
       fprintf(stderr, "\n[%d][starpu_mpi][%s][assert failure] " msg "\n\n", _starpu_debug_rank, __starpu_func__, ## __VA_ARGS__); *(int*)NULL = 0; \
     } \
   } while(0)
#  else
#    define STARPU_MPI_ASSERT_MSG(x, msg, ...)	\
   do \
   { \
    if (STARPU_UNLIKELY(!(x))) \
      { \
       if (_starpu_debug_rank == -1) starpu_mpi_comm_rank(MPI_COMM_WORLD, &_starpu_debug_rank); \
       fprintf(stderr, "\n[%d][starpu_mpi][%s][assert failure] " msg "\n\n", _starpu_debug_rank, __starpu_func__, ## __VA_ARGS__); \
     } \
     assert(x); \
   } while(0)

#  endif
#endif

#endif

   END_C_DECLS

#endif /* _TASKS_H_ */
