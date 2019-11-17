/**
 *
 * @file al4san_quark.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
 *                      All rights reserved.
 ***
 *
 *
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 * 
 * @brief AL4SAN Quark runtime main header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#ifndef _AL4SAN_QUARK_H_
#define _AL4SAN_QUARK_H_

#include <quark.h>
#include <string.h>
#include "quark_task.h"

#include "control/al4san_common.h"

BEGIN_C_DECLS

typedef struct runtime_internal_s {
    Quark_Task_Flags flags;
    Quark *quark;
} runtime_internal_t;

typedef void (al4san_quark_codelet) (Quark *);

/*
 * Access to block pointer and leading dimension
 */
#define GET_QUARK_MACRO_ADDR(_1,_2,_3,_4, NAME,...) NAME
#define AL4SAN_QAURK_ADDR(...) GET_QUARK_MACRO_ADDR(__VA_ARGS__, AL4SAN_QAURK_ADDR4, AL4SAN_QAURK_ADDR3, AL4SAN_QAURK_ADDR2)(__VA_ARGS__)

#define GET_QUARK_MACRO_ACCESS_R(_1,_2,_3, NAME,...) NAME
#define AL4SAN_QUARK_ACCESS_R(...) GET_QUARK_MACRO_ACCESS_R(__VA_ARGS__, AL4SAN_QUARK_ACCESS_R3, AL4SAN_QUARK_ACCESS_R2, AL4SAN_QUARK_ACCESS_R1)(__VA_ARGS__)

#define GET_QUARK_MACRO_ACCESS_W(_1,_2,_3, NAME,...) NAME
#define AL4SAN_QUARK_ACCESS_W(...) GET_QUARK_MACRO__ACCESS_W(__VA_ARGS__, AL4SAN_QUARK_ACCESS_W3, AL4SAN_QUARK_ACCESS_W2, AL4SAN_QUARK_ACCESS_W1)(__VA_ARGS__)

#define GET_QUARK_MACRO_ACCESS_RW(_1,_2,_3, NAME,...) NAME
#define AL4SAN_QUARK_ACCESS_RW(...) GET_QUARK_MACRO_ACCESS_RW(__VA_ARGS__, AL4SAN_QUARK_ACCESS_RW3, AL4SAN_QUARK_ACCESS_RW2, AL4SAN_QUARK_ACCESS_RW1)(__VA_ARGS__)


#define AL4SAN_QAURK_ADDR4( desc, type, m, n ) ( (type*)AL4SAN_Quark_data_getaddr( desc, m, n ) )
#define AL4SAN_QAURK_ADDR3( desc, type, m) ( (type*)AL4SAN_Quark_data_getaddr( desc, m, 0 ) )
#define AL4SAN_QAURK_ADDR2( desc, type) ( (type*)AL4SAN_Quark_data_getaddr( desc, 0, 0 ) )

#define AL4SAN_QUARK_BEGIN_ACCESS_DECLARATION

#define AL4SAN_QUARK_ACCESS_R3(A, Am, An)

#define AL4SAN_QUARK_ACCESS_R2(A, Am)

#define AL4SAN_QUARK_ACCESS_R1(A)

#define AL4SAN_QUARK_ACCESS_W3(A, Am, An)

#define AL4SAN_QUARK_ACCESS_W2(A, Am)

#define AL4SAN_QUARK_ACCESS_W1(A)

#define AL4SAN_QUARK_ACCESS_RW3(A, Am, An)

#define AL4SAN_QUARK_ACCESS_RW2(A, Am)

#define AL4SAN_QUARK_ACCESS_RW1(A)

#define AL4SAN_QUARK_RANK_CHANGED(rank)

#define AL4SAN_QUARK_END_ACCESS_DECLARATION

#define AL4SAN_QUARK_getStream(_stream_)  

END_C_DECLS
#endif /* _AL4SAN_QUARK_H_ */
