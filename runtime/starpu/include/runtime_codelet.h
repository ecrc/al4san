 /**
 *
 * @file runtime_codelets.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 * @brief Chameleon StarPU codelets main header
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 * @brief ALTANAL StarPU codelets main header
 *
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/
#ifndef _ALTANAL_CODELETS_H_
#define _ALTANAL_CODELETS_H_

#include "altanal/altanal_config.h"
#include "runtime_codelet_profile.h"

#ifdef ALTANAL_CUDA_ASYNC
#define CODELET_CUDA_FLAGS(flags) .cuda_flags = {(flags)},
#else
#define CODELET_CUDA_FLAGS(flags)
#endif

#define ALTANAL_CODELETS_ALL_CPU(cl_name, cpu_func_name, cuda_func_name, _original_location_)	\
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
    
#define ALTANAL_CODELETS_ALL_GPU(cl_name, cpu_func_name, cuda_func_name, _original_location_) \
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

#if defined(ALTANAL_SIMULATION)
#define ALTANAL_CODELETS_CPU(name, cpu_func_name)                          \
  ALTANAL_CODELETS_ALL_CPU( name,  (starpu_cpu_func_t) 1, NULL, STARPU_CPU)
#else
#define ALTANAL_CODELETS_CPU(name, cpu_func_name)                             \
  static void cl_##name##_cpu(void *descr[], void *cl_arg)                    \
  {                                                                           \
    ALTANAL_arg_list altanal_arg;                                             \
    altanal_arg.descr=descr;                                                  \
    altanal_arg.cl_arg=cl_arg;                                                \
    cpu_func_name(&altanal_arg);                                              \
  }                                                                           \
  ALTANAL_CODELETS_ALL_CPU( name, cpu_func_name, NULL, STARPU_CPU)         
 
#endif
 

#define ALTANAL_CODELETS_GPU(name, cpu_func_name, cuda_func_name) \
   static void cl_##name##_cpu(void *descr[], void *cl_arg)                   \
  {                                                                           \
    ALTANAL_arg_list altanal_arg;                                             \
    altanal_arg.descr=descr;                                                  \
    altanal_arg.cl_arg=cl_arg;                                                \
    cpu_func_name(&altanal_arg);                                              \
  }\
  static void cl_##name##_cuda(void *descr[], void *cl_arg)                   \
  {                                                                           \
    ALTANAL_arg_list altanal_arg;                                             \
    altanal_arg.descr=descr;                                                  \
    altanal_arg.cl_arg=cl_arg;                                                \
    cuda_func_name(&altanal_arg);                                             \
  }                                                                           \
  ALTANAL_CODELETS_ALL_GPU( name,  cpu_func_name, cuda_func_name, STARPU_CPU  | STARPU_CUDA)


#define ALTANAL_CODELETS_ALL_HEADER(name)                                             \
     ALTANAL_CL_CB_HEADER(name);                                            \
     void cl_##name##_load_fake_model(void);                                  \
     void cl_##name##_restore_model(void);                                    \
     extern struct starpu_codelet cl_##name;                                  \
     void cl_##name##_restrict_where(uint32_t where);                         \
     void cl_##name##_restore_where(void);

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define ALTANAL_CODELETS(...) GET_MACRO(__VA_ARGS__, ALTANAL_CODELETS3, ALTANAL_CODELETS2)(__VA_ARGS__)

#if defined(ALTANAL_SIMULATION)
#if defined(ALTANAL_USE_CUDA)
#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_GPU(name,  (starpu_cpu_func_t) 1, (starpu_cuda_func_t) 1)

#define ALTANAL_CODELETS2(name,  cpu_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define ALTANAL_CODELETS_HEADER(name)  ALTANAL_CODELETS_ALL_HEADER(name)
#else
#define ALTANAL_CODELETS2(name,  cpu_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)
    
#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_GPU(name,  (starpu_cpu_func_t) 1)

#define ALTANAL_CODELETS_HEADER(name)  ALTANAL_CODELETS_ALL_HEADER(name)
#endif
#elif defined(ALTANAL_USE_CUDA)
#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_GPU(name,  cpu_func_name, cuda_func_name)

#define ALTANAL_CODELETS2(name,  cpu_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_CPU(name,  cpu_func_name)

#define ALTANAL_CODELETS_HEADER(name)  ALTANAL_CODELETS_ALL_HEADER(name)
#else
#define ALTANAL_CODELETS2(name,  cpu_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_CPU(name,  cpu_func_name)

#define ALTANAL_CODELETS3(name,  cpu_func_name, cuda_func_name) \
    ALTANAL_CODELETS_ALL_HEADER(name)\
    ALTANAL_CODELETS_CPU(name,  cpu_func_name)

#define ALTANAL_CODELETS_HEADER(name)  ALTANAL_CODELETS_ALL_HEADER(name)
#endif

#define SALTANAL_CODELETS_HEADER(name)                ALTANAL_CODELETS_HEADER(s##name)
#define DALTANAL_CODELETS_HEADER(name)                ALTANAL_CODELETS_HEADER(d##name)
#define CALTANAL_CODELETS_HEADER(name)                ALTANAL_CODELETS_HEADER(c##name)
#define ZALTANAL_CODELETS_HEADER(name)                ALTANAL_CODELETS_HEADER(z##name)

#define SALTANAL_CODELETS_CPU_HEADER(name)        ALTANAL_CODELETS_CPU_HEADER(s##name)
#define DALTANAL_CODELETS_CPU_HEADER(name)        ALTANAL_CODELETS_CPU_HEADER(d##name)
#define CALTANAL_CODELETS_CPU_HEADER(name)        ALTANAL_CODELETS_CPU_HEADER(c##name)
#define ZALTANAL_CODELETS_CPU_HEADER(name)        ALTANAL_CODELETS_CPU_HEADER(z##name)


#endif /* _ALTANAL_CODELETS_H_ */
