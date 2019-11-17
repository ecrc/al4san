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
#include "starpu_task.h"

#ifdef __cplusplus
#define AL4SAN_CODELETS_ALL_CPU(cl_name, cpu_func_name, cuda_func_name, _original_location_)    \
    struct starpu_perfmodel cl_##cl_name##_fake = {                           \
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        "fake"#cl_name,/*const char *symbol;*/                                 \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
        {},\
        {},\
	{},\
	{},\
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        ""#cl_name/*const char *symbol;*/                                     \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        (_original_location_),                                                \
        {},/*int (*can_execute)(unsigned workerid,                            \
                    struct starpu_task *task, unsigned nimpl);*/              \
        {},/*STARPU_SEQ,//enum starpu_codelet_type type;*/                    \
        0 ,/*0,//int max_parallelism;*/                                       \
        {},/*starpu_cpu_func_t cpu_func STARPU_DEPRECATED;*/                  \
        {},/*starpu_cuda_func_t cuda_func STARPU_DEPRECATED;*/                \
        {},/*starpu_opencl_func_t opencl_func STARPU_DEPRECATED;*/            \
        {((cpu_func_name))}, /*starpu_cpu_func_t 
                               cpu_funcs[STARPU_MAXIMPLEMENTATIONS]; */       \
        {}, /*starpu_cuda_func_t 
                                cuda_funcs[STARPU_MAXIMPLEMENTATIONS];*/      \
        {}, /*CODELET_CUDA_FLAGS_GWAS(cuda_flags),char 
                                cuda_flags[STARPU_MAXIMPLEMENTATIONS];*/      \
        {},/*starpu_opencl_func_t opencl_funcs[STARPU_MAXIMPLEMENTATIONS];*/  \
        {},/*char opencl_flags[STARPU_MAXIMPLEMENTATIONS];*/                  \
        {},/*starpu_mic_func_t mic_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*starpu_scc_func_t scc_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*const char *cpu_funcs_name[STARPU_MAXIMPLEMENTATIONS];*/         \
        {}./*((_nbuffers)),*/                                                 \
        {},/*modes //enum starpu_data_access_mode modes[STARPU_NMAXBUFS];*/   \
        {},/*dyn_modes //enum starpu_data_access_mode *dyn_modes;*/           \
        {},/*specific_nodes //unsigned specific_nodes;*/                      \
        {},/*nodes          //int nodes[STARPU_NMAXBUFS];*/                   \
        {},/*dyn_nodes      //int *dyn_nodes;*/                               \
        &cl_##cl_name##_model, /*struct starpu_perfmodel *model*/             \
        {},/* energy_model //struct starpu_perfmodel *energy_model;*/         \
        {},/* per_worker_stats//unsigned long per_worker_stats
                                [STARPU_NMAXWORKERS];*/                       \
        #cl_name               /*const char *name*/                           \
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
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        "fake"#cl_name,/*const char *symbol;*/                                 \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
        {},\
        {},\
	{},\
	{},\
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        ""#cl_name/*const char *symbol;*/                                     \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        (_original_location_),                                                \
        {},/*int (*can_execute)(unsigned workerid,                            \
                    struct starpu_task *task, unsigned nimpl);*/              \
        {},/*STARPU_SEQ,//enum starpu_codelet_type type;*/                    \
        0 ,/*0,//int max_parallelism;*/                                       \
        {},/*starpu_cpu_func_t cpu_func STARPU_DEPRECATED;*/                  \
        {},/*starpu_cuda_func_t cuda_func STARPU_DEPRECATED;*/                \
        {},/*starpu_opencl_func_t opencl_func STARPU_DEPRECATED;*/            \
        {}, /*starpu_cpu_func_t 
                               cpu_funcs[STARPU_MAXIMPLEMENTATIONS]; */       \
        {((cuda_func_name))}, /*starpu_cuda_func_t 
                                cuda_funcs[STARPU_MAXIMPLEMENTATIONS];*/      \
        {}, /*CODELET_CUDA_FLAGS_GWAS(cuda_flags), char 
                                cuda_flags[STARPU_MAXIMPLEMENTATIONS];*/      \
        {},/*starpu_opencl_func_t opencl_funcs[STARPU_MAXIMPLEMENTATIONS];*/  \
        {},/*char opencl_flags[STARPU_MAXIMPLEMENTATIONS];*/                  \
        {},/*starpu_mic_func_t mic_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*starpu_scc_func_t scc_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*const char *cpu_funcs_name[STARPU_MAXIMPLEMENTATIONS];*/         \
        {}, /*((_nbuffers)) */                                                        \
        {},/*modes //enum starpu_data_access_mode modes[STARPU_NMAXBUFS];*/   \
        {},/*dyn_modes //enum starpu_data_access_mode *dyn_modes;*/           \
        {},/*specific_nodes //unsigned specific_nodes;*/                      \
        {},/*nodes          //int nodes[STARPU_NMAXBUFS];*/                   \
        {},/*dyn_nodes      //int *dyn_nodes;*/                               \
        &cl_##cl_name##_model, /*struct starpu_perfmodel *model*/             \
        {},/* energy_model //struct starpu_perfmodel *energy_model;*/         \
        {},/* per_worker_stats//unsigned long per_worker_stats
                                [STARPU_NMAXWORKERS];*/                       \
        #cl_name               /*const char *name*/                           \
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
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        "fake"#cl_name,/*const char *symbol;*/                                 \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
        {},\
        {},\
	{},\
	{},\
    };                                                                        \
                                                                              \
    struct starpu_perfmodel cl_##cl_name##_model = {                          \
        STARPU_HISTORY_BASED,/*enum starpu_perfmodel_type type;*/             \
        {},/*double (*cost_function)(struct starpu_task *, unsigned nimpl);*/ \
        {},/*double (*arch_cost_function)(struct starpu_task *,               \
                     struct starpu_perfmodel_arch * arch, unsigned nimpl);*/  \
        {},/*size_t (*size_base)(struct starpu_task *, unsigned nimpl);*/     \
        {},/*uint32_t (*footprint)(struct starpu_task *);*/                   \
        ""#cl_name/*const char *symbol;*/                                     \
        /*unsigned is_loaded;*/                                               \
        /*unsigned benchmarking;*/                                            \
        /*unsigned is_init;*/                                                 \
        /*starpu_perfmodel_state_t state;*/                                   \
    };                                                                        \
                                                                              \
    struct starpu_codelet cl_##cl_name = {                                    \
        (_original_location_),                                                \
        {},/*int (*can_execute)(unsigned workerid,                            \
                    struct starpu_task *task, unsigned nimpl);*/              \
        {},/*STARPU_SEQ,//enum starpu_codelet_type type;*/                    \
        0 ,/*0,//int max_parallelism;*/                                       \
        {},/*starpu_cpu_func_t cpu_func STARPU_DEPRECATED;*/                  \
        {},/*starpu_cuda_func_t cuda_func STARPU_DEPRECATED;*/                \
        {},/*starpu_opencl_func_t opencl_func STARPU_DEPRECATED;*/            \
        {((cpu_func_name))}, /*starpu_cpu_func_t 
                               cpu_funcs[STARPU_MAXIMPLEMENTATIONS]; */       \
        {((cuda_func_name))}, /*starpu_cuda_func_t 
                                cuda_funcs[STARPU_MAXIMPLEMENTATIONS];*/      \
        {}/*CODELET_CUDA_FLAGS_GWAS(cuda_flags), char 
                                cuda_flags[STARPU_MAXIMPLEMENTATIONS];*/      \
        {},/*starpu_opencl_func_t opencl_funcs[STARPU_MAXIMPLEMENTATIONS];*/  \
        {},/*char opencl_flags[STARPU_MAXIMPLEMENTATIONS];*/                  \
        {},/*starpu_mic_func_t mic_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*starpu_scc_func_t scc_funcs[STARPU_MAXIMPLEMENTATIONS];*/        \
        {},/*const char *cpu_funcs_name[STARPU_MAXIMPLEMENTATIONS];*/         \
        {} /*((_nbuffers)),      */                                           \
        {},/*modes //enum starpu_data_access_mode modes[STARPU_NMAXBUFS];*/   \
        {},/*dyn_modes //enum starpu_data_access_mode *dyn_modes;*/           \
        {},/*specific_nodes //unsigned specific_nodes;*/                      \
        {},/*nodes          //int nodes[STARPU_NMAXBUFS];*/                   \
        {},/*dyn_nodes      //int *dyn_nodes;*/                               \
        &cl_##cl_name##_model, /*struct starpu_perfmodel *model*/             \
        {},/* energy_model //struct starpu_perfmodel *energy_model;*/         \
        {},/* per_worker_stats//unsigned long per_worker_stats
                                [STARPU_NMAXWORKERS];*/                       \
        #cl_name               /*const char *name*/                           \
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
#else
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
#endif //C++
#if defined(AL4SAN_SIMULATION)
#define AL4SAN_CODELETS_CPU(name, cpu_func_name)                          \
  AL4SAN_CODELETS_ALL_CPU( name,  (starpu_cpu_func_t) 1, NULL, STARPU_CPU)
#else
#define AL4SAN_CODELETS_CPU(name, cpu_func_name)                             \
  static void cl_##name##_cpu(void *descr[], void *cl_arg)                    \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    AL4SAN_Starpu_arg_list starpu_arg;                                      \
    starpu_arg.descr=descr;           \
    starpu_arg.cl_arg=cl_arg;         \
    al4san_arg.Starpu_arg_list=(AL4SAN_Starpu_arg_list*) &starpu_arg;        \
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
    AL4SAN_Starpu_arg_list starpu_arg;                                      \
    starpu_arg.descr=descr;           \
    starpu_arg.cl_arg=cl_arg;         \
    al4san_arg.Starpu_arg_list=(AL4SAN_Starpu_arg_list*) &starpu_arg;        \
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
    AL4SAN_Starpu_arg_list starpu_arg;                                      \
    starpu_arg.descr=descr;           \
    starpu_arg.cl_arg=cl_arg;         \
    al4san_arg.Starpu_arg_list=(AL4SAN_Starpu_arg_list*) &starpu_arg;        \
    cpu_func_name(&al4san_arg);                                              \
  }\
  static void cl_##name##_cuda(void *descr[], void *cl_arg)                   \
  {                                                                           \
    AL4SAN_arg_list al4san_arg;                                             \
    AL4SAN_Starpu_arg_list starpu_arg;                                      \
    starpu_arg.descr=descr;           \
    starpu_arg.cl_arg=cl_arg;         \
    al4san_arg.Starpu_arg_list=(AL4SAN_Starpu_arg_list*) &starpu_arg;        \
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
#define AL4SAN_STARPU_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_STARPU_TASK_GPU(name,  cuda_func_name) \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_GPU(name,  (starpu_cuda_func_t) 1)

#define AL4SAN_STARPU_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU_GPU(name,  (starpu_cpu_func_t) 1, (starpu_cuda_func_t) 1)

#define AL4SAN_STARPU_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_STARPU_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_STARPU_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  (starpu_cpu_func_t) 1)

#define AL4SAN_STARPU_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#endif
#elif defined(AL4SAN_USE_CUDA)
#define AL4SAN_STARPU_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_STARPU_TASK_GPU(name,  cuda_func_name) \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_GPU(name,  cuda_func_name)

#define AL4SAN_STARPU_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
     void cuda_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU_GPU(name,  cpu_func_name, cuda_func_name)

#define AL4SAN_STARPU_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
#else
#define AL4SAN_STARPU_TASK_CPU(name,  cpu_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_STARPU_TASK_CPU_GPU(name,  cpu_func_name, cuda_func_name) \
    void cpu_func_name(AL4SAN_arg_list *al4san_arg); \
    AL4SAN_CODELETS_CPU(name,  cpu_func_name)

#define AL4SAN_STARPU_TASK_HEADER(name)  AL4SAN_CODELETS_ALL_HEADER(name)
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
