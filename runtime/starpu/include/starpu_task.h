/**
 *
 * @file starpu_task.h
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ****
 *
 * @brief AL4SAN StarPU task header
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

#if defined(TANAL_USE_MPI)
#include "control/al4san_common.h"
#include <starpu_mpi.h>
#else
#include <starpu.h>
#endif
//#include "starpu_codelet.h"

BEGIN_C_DECLS
static void (*pre_submit_hook)(struct starpu_task *task) = NULL;

//#define ARG_END 0   
//#define AL4SAN_DEP 101
#define ON 1
#define OFF 0
//#define AL4SAN_CUDA_ASYNC STARPU_CUDA_ASYNC
#define AL4SAN_STARPU_UNDEFINED  0xFF0000
#define AL4SAN_STARPU_UNDEFINED_MASK 0x0000FF //0x00FFFF

#define AL4SAN_STARPU_NMAXARGS 30

typedef struct AL4SAN_Starpu_Task
{
  int num_arg;
  int arg_depenency[AL4SAN_STARPU_NMAXARGS];  
  int arg_size[AL4SAN_STARPU_NMAXARGS];

} AL4SAN_Starpu_task_t;


/*typedef struct al4san_starpu_arg_list_s
{
  void *cl_arg;    
  void **descr;
}AL4SAN_Starpu_arg_list;
*/

/*
starpu task macro
*/

#define  _AL4SAN_CODELETS_CHECK() do {} while(0)

//starpu codelelts
//#define AL4SAN_CODELETS_NAME(n) &cl_##n

 //TO be used instead of AL4SAN_CODELETS_NAME
//#define AL4SAN_TASK(name) &cl_##name


//callback
//#define AL4SAN_CALLBACK(name) options->profiling ? cl_##name##_callback : NULL
#define al4san_starpu_matrix_get_nx(num) starpu_matrix_get_nx(task->handles[num])
#define al4san_starpu_matrix_get_ny(num) starpu_matrix_get_ny(task->handles[num])

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


#if defined(AL4SAN_USE_MPI)
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
