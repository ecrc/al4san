/**
 *
 * @file al4san_parsec.h
 *
 * @copyright 2009-2015 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 * author Mathieu Faverge
 * author Reazul Hoque
 * date 2018-02-06
 *
 * @brief AL4SAN PaRSEC runtime main header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#ifndef _AL4SAN_PARSEC_H_
#define _AL4SAN_PARSEC_H_

#include <parsec.h>
#include <parsec/parsec_internal.h>
#include <parsec/execution_stream.h>
#include <parsec/interfaces/superscalar/insert_function.h>
#include <parsec/interfaces/superscalar/insert_function_internal.h>
#include <parsec/utils/debug.h>
#include <parsec/data_dist/matrix/matrix.h>

/* Undefined PaRSEC definition of BLKLDD */
#undef BLKLDD
#include "parsec_task.h"
#include "control/al4san_common.h"

BEGIN_C_DECLS

typedef struct parsec_dtd_funcptr_t al4san_parsec_codelet;

typedef struct al4san_parsec_arg_list_s
     {
          parsec_task_t *this_task;
     } AL4SAN_Parsec_arg_list;


struct al4san_parsec_desc_s {
    parsec_data_collection_t super;
    int                      arena_index;
    AL4SAN_desc_t            *desc;
    parsec_data_t          **data_map;
};

typedef struct al4san_parsec_desc_s al4san_parsec_desc_t;

static inline int
al4san_parsec_get_arena_index(const AL4SAN_desc_t *desc) {
    return ((al4san_parsec_desc_t *)desc->schedopt)->arena_index;
}

/*
 * Access to block pointer and leading dimension
 */
//#define AL4SAN_ADDR( desc, type, m, n ) ( (type*)AL4SAN_Parsec_data_getaddr( desc, m, n ) )

#define GET_PARSEC_MACRO_ADDR(_1,_2,_3,_4, NAME,...) NAME
#define AL4SAN_PARSEC_ADDR(...) GET_PARSEC_MACRO_ADDR(__VA_ARGS__, AL4SAN_PARSEC_ADDR4, AL4SAN_PARSEC_ADDR3, AL4SAN_PARSEC_ADDR2)(__VA_ARGS__)

#define GET_MACRO_PARSEC_ACCESS_R(_1,_2,_3, NAME,...) NAME
#define AL4SAN_PARSEC_ACCESS_R(...) GET_MACRO_PARSEC_ACCESS_R(__VA_ARGS__, AL4SAN_PARSEC_ACCESS_R3, AL4SAN_PARSEC_ACCESS_R2, AL4SAN_PARSEC_ACCESS_R1)(__VA_ARGS__)

#define GET_MACRO_PARSEC_ACCESS_W(_1,_2,_3, NAME,...) NAME
#define AL4SAN_PARSEC_ACCESS_W(...) GET_MACRO_PARSEC_ACCESS_W(__VA_ARGS__, AL4SAN_PARSEC_ACCESS_W3, AL4SAN_PARSEC_ACCESS_W2, AL4SAN_PARSEC_ACCESS_W1)(__VA_ARGS__)

#define GET_MACRO_PARSEC_ACCESS_RW(_1,_2,_3, NAME,...) NAME
#define AL4SAN_PARSEC_ACCESS_RW(...) GET_MACRO_PARSEC_ACCESS_RW(__VA_ARGS__, AL4SAN_PARSEC_ACCESS_RW3, AL4SAN_PARSEC_ACCESS_RW2, AL4SAN_PARSEC_ACCESS_RW1)(__VA_ARGS__)


#define AL4SAN_PARSEC_ADDR4( desc, type, m, n ) ( (type*)AL4SAN_Parsec_data_getaddr( desc, m, n ) )
#define AL4SAN_PARSEC_ADDR3( desc, type, m) ( (type*)AL4SAN_Parsec_data_getaddr( desc, m, 0 ) )
#define AL4SAN_PARSEC_ADDR2( desc, type) ( (type*)AL4SAN_Parsec_data_getaddr( desc, 0, 0 ) )

#define AL4SAN_PARSEC_BEGIN_ACCESS_DECLARATION

#define AL4SAN_PARSEC_ACCESS_R3(A, Am, An)

#define AL4SAN_PARSEC_ACCESS_R2(A, Am)

#define AL4SAN_PARSEC_ACCESS_R1(A)

#define AL4SAN_PARSEC_ACCESS_W3(A, Am, An)

#define AL4SAN_PARSEC_ACCESS_W2(A, Am)

#define AL4SAN_PARSEC_ACCESS_W1(A)

#define AL4SAN_PARSEC_ACCESS_RW3(A, Am, An)

#define AL4SAN_PARSEC_ACCESS_RW2(A, Am)

#define AL4SAN_PARSEC_ACCESS_RW1(A)

#define AL4SAN_PARSEC_RANK_CHANGED(rank)

#define AL4SAN_PARSEC_END_ACCESS_DECLARATION

#define AL4SAN_PARSEC_getStream(_stream_)  


#define AL4SAN_PARSEC_TASK_CPU(name, cpu_func_name)\
int PARSEC_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {\
     (void)es; AL4SAN_arg_list al4san_arg; \
     AL4SAN_Parsec_arg_list parsecarg;\
     parsecarg.this_task=this_task;\
     al4san_arg.Parsec_arg_list=(AL4SAN_Parsec_arg_list*) &parsecarg;\
     cpu_func_name(&al4san_arg);\
     return PARSEC_HOOK_RETURN_DONE;}

#define AL4SAN_PARSEC_TASK_GPU(name, gpu_func_name)

#define AL4SAN_PARSEC_TASK_CPU_GPU(name, cpu_func_name, gpu_func_name)\
int PARSEC_##name##_func (parsec_execution_stream_t *es, parsec_task_t *this_task) {\
     (void)es; AL4SAN_arg_list al4san_arg; \
     AL4SAN_Parsec_arg_list parsecarg;\
     parsecarg.this_task=this_task;\
     al4san_arg.Parsec_arg_list=(AL4SAN_Parsec_arg_list*) &parsecarg;\
     cpu_func_name(&al4san_arg);\
     return PARSEC_HOOK_RETURN_DONE;}

END_C_DECLS

#endif /* _AL4SAN_PARSEC_H_ */
