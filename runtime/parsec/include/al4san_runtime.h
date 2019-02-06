/**
 *
 * @file al4san_runtime.h
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
 * @version 1.0.1
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
#include "runtime_task.h"
#include "control/al4san_common.h"

BEGIN_C_DECLS

typedef struct parsec_dtd_funcptr_t al4san_codelet;


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
//#define AL4SAN_ADDR( desc, type, m, n ) ( (type*)AL4SAN_Runtime_data_getaddr( desc, m, n ) )

#define GET_MACRO_ADDR(_1,_2,_3,_4, NAME,...) NAME
#define AL4SAN_ADDR(...) GET_MACRO_ADDR(__VA_ARGS__, AL4SAN_ADDR4, AL4SAN_ADDR3, AL4SAN_ADDR2)(__VA_ARGS__)

#define GET_MACRO_RUNTIME_ACCESS_R(_1,_2,_3, NAME,...) NAME
#define AL4SAN_RUNTIME_ACCESS_R(...) GET_MACRO_RUNTIME_ACCESS_R(__VA_ARGS__, AL4SAN_RUNTIME_ACCESS_R3, AL4SAN_RUNTIME_ACCESS_R2, AL4SAN_RUNTIME_ACCESS_R1)(__VA_ARGS__)

#define GET_MACRO_RUNTIME_ACCESS_W(_1,_2,_3, NAME,...) NAME
#define AL4SAN_RUNTIME_ACCESS_W(...) GET_MACRO_RUNTIME_ACCESS_W(__VA_ARGS__, AL4SAN_RUNTIME_ACCESS_W3, AL4SAN_RUNTIME_ACCESS_W2, AL4SAN_RUNTIME_ACCESS_W1)(__VA_ARGS__)

#define GET_MACRO_RUNTIME_ACCESS_RW(_1,_2,_3, NAME,...) NAME
#define AL4SAN_RUNTIME_ACCESS_RW(...) GET_MACRO_RUNTIME_ACCESS_RW(__VA_ARGS__, AL4SAN_RUNTIME_ACCESS_RW3, AL4SAN_RUNTIME_ACCESS_RW2, AL4SAN_RUNTIME_ACCESS_RW1)(__VA_ARGS__)


#define AL4SAN_ADDR4( desc, type, m, n ) ( (type*)AL4SAN_Runtime_data_getaddr( desc, m, n ) )
#define AL4SAN_ADDR3( desc, type, m) ( (type*)AL4SAN_Runtime_data_getaddr( desc, m, 0 ) )
#define AL4SAN_ADDR2( desc, type) ( (type*)AL4SAN_Runtime_data_getaddr( desc, 0, 0 ) )

#define AL4SAN_RUNTIME_BEGIN_ACCESS_DECLARATION

#define AL4SAN_RUNTIME_ACCESS_R3(A, Am, An)

#define AL4SAN_RUNTIME_ACCESS_R2(A, Am)

#define AL4SAN_RUNTIME_ACCESS_R1(A)

#define AL4SAN_RUNTIME_ACCESS_W3(A, Am, An)

#define AL4SAN_RUNTIME_ACCESS_W2(A, Am)

#define AL4SAN_RUNTIME_ACCESS_W1(A)

#define AL4SAN_RUNTIME_ACCESS_RW3(A, Am, An)

#define AL4SAN_RUNTIME_ACCESS_RW2(A, Am)

#define AL4SAN_RUNTIME_ACCESS_RW1(A)

#define AL4SAN_RUNTIME_RANK_CHANGED(rank)

#define AL4SAN_RUNTIME_END_ACCESS_DECLARATION

#define AL4SAN_Runtime_getStream(_stream_)  

END_C_DECLS

#endif /* _AL4SAN_PARSEC_H_ */
