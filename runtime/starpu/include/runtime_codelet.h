 /**
 *
 * @file runtime_codelets.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 *
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 * @brief AL4SAN StarPU codelets main header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#ifndef _AL4SAN_CODELETS_H_
#define _AL4SAN_CODELETS_H_

#include "al4san/config.h"

#define AL4SAN_CODELETS_ALL_CPU(cl_name, cpu_func_name, cuda_func_name, _original_location_)	\
    struct starpu_perfmodel cl_##cl_name##_fake = {                           \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = "fake_"#cl_name                                             \
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = ""#cl_name                                                  \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        .where     = (_original_location_),                                   \
        .cpu_func  = ((cl_##cl_name##_cpu)),                                  \
        .model     = &cl_##cl_name##_model,                                   \
        .name      = #cl_name                                                 \
    };                                                                        \
                                                                              \
    void cl_##cl_name##_restrict_where(uint32_t where)                        \
    {                                                                         \
      if ( cl_##cl_name.where & where )                                       \
        cl_##cl_name.where = (cl_##cl_name.where & where);                    \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_where(void)                                   \
    {                                                                         \
        cl_##cl_name.where = (_original_location_);                           \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_model(void)                                   \
    {                                                                         \
        cl_##cl_name.model = &cl_##cl_name##_model;                           \
    }

#define AL4SAN_CODELETS_ALL_GPU(cl_name, cpu_func_name, cuda_func_name, _original_location_) \
    struct starpu_perfmodel cl_##cl_name##_fake = {                           \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = "fake_"#cl_name                                             \
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = ""#cl_name                                                  \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        .where     = (_original_location_),                                   \
        .cuda_func = ((cl_##cl_name##_cuda)),                                      \
        .model     = &cl_##cl_name##_model,                                   \
        .name      = #cl_name                                                 \
    };                                                                        \
                                                                              \
    void cl_##cl_name##_restrict_where(uint32_t where)                        \
    {                                                                         \
      if ( cl_##cl_name.where & where )                                       \
        cl_##cl_name.where = (cl_##cl_name.where & where);                    \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_where(void)                                   \
    {                                                                         \
        cl_##cl_name.where = (_original_location_);                           \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_model(void)                                   \
    {                                                                         \
        cl_##cl_name.model = &cl_##cl_name##_model;                           \
    }

#define AL4SAN_CODELETS_ALL_CPU_GPU(cl_name, cpu_func_name, cuda_func_name, _original_location_) \
    struct starpu_perfmodel cl_##cl_name##_fake = {                           \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = "fake_"#cl_name                                             \
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        .type   = STARPU_HISTORY_BASED,                                       \
        .symbol = ""#cl_name                                                  \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        .where     = (_original_location_),                                   \
        .cpu_func  = ((cl_##cl_name##_cpu)),                                  \
        .cuda_func = ((cl_##cl_name##_cuda)),                                      \
        .model     = &cl_##cl_name##_model,                                   \
        .name      = #cl_name                                                 \
    };                                                                        \
                                                                              \
    void cl_##cl_name##_restrict_where(uint32_t where)                        \
    {                                                                         \
      if ( cl_##cl_name.where & where )                                       \
        cl_##cl_name.where = (cl_##cl_name.where & where);                    \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_where(void)                                   \
    {                                                                         \
        cl_##cl_name.where = (_original_location_);                           \
    }                                                                         \
                                                                              \
    void cl_##cl_name##_restore_model(void)                                   \
    {                                                                         \
        cl_##cl_name.model = &cl_##cl_name##_model;                           \
    }

#if defined(AL4SAN_SIMULATION)
#define AL4SAN_CODELETS_CPU(name, cpu_func_name)                          \
  AL4SAN_CODELETS_ALL_CPU( name,  (starpu_cpu_func_t) 1, NULL, STARPU_CPU)
#else
#define AL4SAN_CODELETS_CPU(name, cpu_func_name)                             \
  static void cl_##name##_cpu(void *descr[], void *cl_arg)                    \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    al4san_arg.descr=descr;                                                  \
    al4san_arg.cl_arg=cl_arg;                                                \
    cpu_func_name(&al4san_arg);                                              \
  }                                                                           \
  AL4SAN_CODELETS_ALL_CPU( name, cpu_func_name, NULL, STARPU_CPU)         
 
#endif
 
 #if defined(AL4SAN_SIMULATION)
#define AL4SAN_CODELETS_GPU(name, cuda_func_name)                          \
  AL4SAN_CODELETS_ALL_GPU( name,  NULL, (starpu_cuda_func_t) 1, STARPU_CUDA)
#else
#define AL4SAN_CODELETS_GPU(name, cuda_func_name)                             \
  static void cl_##name##_cuda(void *descr[], void *cl_arg)                    \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    al4san_arg.descr=descr;                                                  \
    al4san_arg.cl_arg=cl_arg;                                                \
    cuda_func_name(&al4san_arg);                                              \
  }                                                                           \
  AL4SAN_CODELETS_ALL_GPU( name, NULL, cuda_func_name, STARPU_CUDA)         
 
#endif

#if defined(AL4SAN_SIMULATION)
#define AL4SAN_CODELETS_CPU_GPU(name, cpu_func_name, cuda_func_name)        \
  AL4SAN_CODELETS_ALL_CPU_GPU( name,  (starpu_cpu_func_t) 1, (starpu_cuda_func_t) 1, STARPU_CPU  | STARPU_CUDA)
#else
#define AL4SAN_CODELETS_CPU_GPU(name, cpu_func_name, cuda_func_name) \
   static void cl_##name##_cpu(void *descr[], void *cl_arg)                   \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    al4san_arg.descr=descr;                                                  \
    al4san_arg.cl_arg=cl_arg;                                                \
    cpu_func_name(&al4san_arg);                                              \
  }\
  static void cl_##name##_cuda(void *descr[], void *cl_arg)                   \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    al4san_arg.descr=descr;                                                  \
    al4san_arg.cl_arg=cl_arg;                                                \
    cuda_func_name(&al4san_arg);                                             \
  }                                                                           \
  AL4SAN_CODELETS_ALL_CPU_GPU( name,  cpu_func_name, cuda_func_name, STARPU_CPU  | STARPU_CUDA)
#endif


#define AL4SAN_CODELETS_ALL_HEADER(name)                                             \
     AL4SAN_CL_CB_HEADER(name);                                            \
     void cl_##name##_load_fake_model(void);                                  \
     void cl_##name##_restore_model(void);                                    \
     extern struct starpu_codelet cl_##name;                                  \
     void cl_##name##_restrict_where(uint32_t where);                         \
     void cl_##name##_restore_where(void);

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define AL4SAN_CODELETS(...) GET_MACRO(__VA_ARGS__, AL4SAN_CODELETS3, AL4SAN_CODELETS2)(__VA_ARGS__)

#if defined(AL4SAN_SIMULATION)
#if defined(AL4SAN_USE_CUDA)
#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU_GPU(name,  (starpu_cpu_func_t) 1, (starpu_cuda_func_t) 1)

#define AL4SAN_CODELETS2(name,  cpu_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_CODELETS_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_CODELETS2(name,  cpu_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)
    
#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_CODELETS_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#endif
#elif defined(AL4SAN_USE_CUDA)
#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU_GPU(name,  cpu_func_name, cuda_func_name)

#define AL4SAN_CODELETS2(name,  cpu_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_CODELETS_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_CODELETS2(name,  cpu_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    AL4SAN_CODELETS_ALL_HEADER(name)\
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_CODELETS_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#endif

 //TO be used instead of AL4SAN_CODELETS
#if defined(AL4SAN_SIMULATION)
#if defined(AL4SAN_USE_CUDA)
#define AL4SAN_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_TASK_GPU(name,  cuda_func_name) \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_GPU(name,  (starpu_cuda_func_t) 1)

#define AL4SAN_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU_GPU(name,  (starpu_cpu_func_t) 1, (starpu_cuda_func_t) 1)

#define AL4SAN_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#endif
#elif defined(AL4SAN_USE_CUDA)
#define AL4SAN_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_TASK_GPU(name,  cuda_func_name) \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_GPU(name,  cuda_func_name)

#define AL4SAN_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU_GPU(name,  cpu_func_name, cuda_func_name)

#define AL4SAN_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#endif

/*#define SAL4SAN_CODELETS_HEADER(name)                AL4SAN_CODELETS_HEADER(s##name)
#define DAL4SAN_CODELETS_HEADER(name)                AL4SAN_CODELETS_HEADER(d##name)
#define CAL4SAN_CODELETS_HEADER(name)                AL4SAN_CODELETS_HEADER(c##name)
#define ZAL4SAN_CODELETS_HEADER(name)                AL4SAN_CODELETS_HEADER(z##name)*/

#define SAL4SAN_CODELETS_CPU_HEADER(name)        AL4SAN_CODELETS_CPU_HEADER(s##name)
#define DAL4SAN_CODELETS_CPU_HEADER(name)        AL4SAN_CODELETS_CPU_HEADER(d##name)
#define CAL4SAN_CODELETS_CPU_HEADER(name)        AL4SAN_CODELETS_CPU_HEADER(c##name)
#define ZAL4SAN_CODELETS_CPU_HEADER(name)        AL4SAN_CODELETS_CPU_HEADER(z##name)

#define AL4SAN_CL_CB_HEADER(name)                        \
    struct starpu_perfmodel*cl_##name##_save;    \
    struct starpu_perfmodel cl_##name##_fake;    \
    extern void cl_##name##_callback();                        \
    extern void profiling_display_##name##_info(void);


#endif /* _AL4SAN_CODELETS_H_ */
