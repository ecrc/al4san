/**
 *
 * @file al4san_descriptor.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Al4san descriptor header
 *
 * @version 1.0.1
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2019-02-06
 *
 */
  /**
   *
   * @brief AL4SAN descriptor header
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.1
   * @author Rabab Alomairy
   * @date 2019-02-06
   *
   **/
#ifndef _AL4SAN_DESCRIPTOR_H_
#define _AL4SAN_DESCRIPTOR_H_

#include <assert.h>
#include "al4san/config.h"
#include "al4san/struct.h"
#include "control/al4san_auxiliary.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Internal routines
 */
inline static void* al4san_geteltaddr(const AL4SAN_desc_t *A, int m, int n, int eltsize);
inline static void* al4san_getaddr_cm    (const AL4SAN_desc_t *A, int m, int n);
inline static void* al4san_getaddr_ccrb  (const AL4SAN_desc_t *A, int m, int n);
inline static void* al4san_getaddr_null  (const AL4SAN_desc_t *A, int m, int n);
inline static void* al4san_getaddr_diag  (const AL4SAN_desc_t *A, int m, int n);
inline static int   al4san_getblkldd_cm  (const AL4SAN_desc_t *A, int m);
inline static int   al4san_getblkldd_ccrb(const AL4SAN_desc_t *A, int m);

/**
 *  Data distributions
 */
inline static int   al4san_getrankof_2d(const AL4SAN_desc_t *desc, int m, int n);
inline static int   al4san_getrankof_2d_diag(const AL4SAN_desc_t *desc, int m, int n);

int          al4san_desc_init     ( AL4SAN_desc_t *desc, void *mat,
                                       al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                                       int lm, int ln, int i, int j,
                                       int m,  int n,  int p, int q,
                                       void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                                       int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                                       int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) );
AL4SAN_desc_t* al4san_desc_submatrix( AL4SAN_desc_t *descA, int i, int j, int m, int n );
void         al4san_desc_destroy  ( AL4SAN_desc_t *desc );
int          al4san_desc_check    ( const AL4SAN_desc_t *desc );

/**
 *  Internal function to return address of block (m,n) with m,n = block indices
 */
inline static void* al4san_getaddr_ccrb(const AL4SAN_desc_t *A, int m, int n)
{
    size_t mm = m + A->i / A->mb;
    size_t nn = n + A->j / A->nb;
    size_t eltsize = AL4SAN_Element_Size(A->dtyp);
    size_t offset = 0;

#if defined(AL4SAN_USE_MPI)
    assert( A->myrank == A->get_rankof( A, mm, nn) );
    mm = mm / A->p;
    nn = nn / A->q;
#endif

    if (mm < (size_t)(A->llm1)) {
        if (nn < (size_t)(A->lln1))
            offset = (size_t)(A->bsiz) * (mm + (size_t)(A->llm1) * nn);
        else
            offset = A->A12 + ((size_t)(A->mb * (A->lln%A->nb)) * mm);
    }
    else {
        if (nn < (size_t)(A->lln1))
            offset = A->A21 + ((size_t)((A->llm%A->mb) * A->nb) * nn);
        else
            offset = A->A22;
    }

    return (void*)((intptr_t)A->mat + (offset*eltsize) );
}

/**
 *  Internal function to return address of block (m,n) with m,n = block indices
 */
inline static void *al4san_getaddr_cm(const AL4SAN_desc_t *A, int m, int n)
{
    size_t mm = m + A->i / A->mb;
    size_t nn = n + A->j / A->nb;
    size_t eltsize = AL4SAN_Element_Size(A->dtyp);
    size_t offset = 0;

#if defined(AL4SAN_USE_MPI)
    assert( A->myrank == A->get_rankof( A, mm, nn) );
    mm = mm / A->p;
    nn = nn / A->q;
#endif

    offset = (size_t)(A->llm * A->nb) * nn + (size_t)(A->mb) * mm;
    return (void*)((intptr_t)A->mat + (offset*eltsize) );
}

/**
 *  Internal function to return address of block (m,n) with m,n = block indices
 */
inline static void *al4san_getaddr_diag( const AL4SAN_desc_t *A, int m, int n )
{
    //assert( m == n );
    return al4san_getaddr_ccrb( A, m, 0 );
}

/**
 *  Internal function to return address of block (m,n) with m,n = block indices
 *  This version lets the runtime allocate on-demand.
 */
inline static void *al4san_getaddr_null(const AL4SAN_desc_t *A, int m, int n)
{
    (void)A; (void)m; (void)n;
    return NULL;
}

/**
 *  Internal function to return address of element A(m,n) with m,n = matrix indices
 */
inline static void* al4san_geteltaddr(const AL4SAN_desc_t *A, int m, int n, int eltsize) // Not used anywhere ?!
{
    size_t mm = (m + A->i)/A->mb;
    size_t nn = (n + A->j)/A->nb;
    size_t offset = 0;

#if defined(AL4SAN_USE_MPI)
    assert( A->myrank == A->get_rankof( A, mm, nn) );
    mm = mm / A->p;
    nn = nn / A->q;
#endif

    if (mm < (size_t)(A->llm1)) {
        if (nn < (size_t)(A->lln1))
            offset = A->bsiz*(mm+A->llm1*nn) + m%A->mb + A->mb*(n%A->nb);
        else
            offset = A->A12 + (A->mb*(A->lln%A->nb)*mm) + m%A->mb + A->mb*(n%A->nb);
    }
    else {
        if (nn < (size_t)(A->lln1))
            offset = A->A21 + ((A->llm%A->mb)*A->nb*nn) + m%A->mb + (A->llm%A->mb)*(n%A->nb);
        else
            offset = A->A22 + m%A->mb  + (A->llm%A->mb)*(n%A->nb);
    }
    return (void*)((intptr_t)A->mat + (offset*eltsize) );
}

/**
 *  Internal function to return the leading dimension of element A(m,*) with m,n = block indices
 */
inline static int al4san_getblkldd_ccrb(const AL4SAN_desc_t *A, int m)
{
    int mm = m + A->i / A->mb;
    return ( ((mm+1) == A->lmt) && ((A->lm % A->mb) != 0)) ? A->lm % A->mb : A->mb;
}

inline static int al4san_getblkldd_cm(const AL4SAN_desc_t *A, int m) {
    (void)m;
    return A->llm;
}

/**
 *  Internal function to return MPI rank of element A(m,n) with m,n = block indices
 */
inline static int al4san_getrankof_2d(const AL4SAN_desc_t *A, int m, int n)
{
    int mm = m + A->i / A->mb;
    int nn = n + A->j / A->nb;
    return (mm % A->p) * A->q + (nn % A->q);
}

/**
 *  Internal function to return MPI rank of element DIAG(m,0) with m,n = block indices
 */
inline static int al4san_getrankof_2d_diag(const AL4SAN_desc_t *A, int m, int n)
{
    int mm = m + A->i / A->mb;
    //assert( m == n );
    return (mm % A->p) * A->q + (mm % A->q);
}


/**
 * Detect if the tile is local or not
 */
inline static int al4san_desc_islocal( const AL4SAN_desc_t *A, int m, int n )
{
#if defined(AL4SAN_USE_MPI)
    return (A->myrank == A->get_rankof(A, m, n));
#else
    (void)A; (void)m; (void)n;
    return 1;
#endif /* defined(AL4SAN_USE_MPI) */
}

/**
 * Declare data accesses of codelets using these macros, for instance:
 * AL4SAN_BEGIN_ACCESS_DECLARATION
 * AL4SAN_ACCESS_R(A, Am, An)
 * AL4SAN_ACCESS_R(B, Bm, Bn)
 * AL4SAN_ACCESS_RW(C, Cm, Cn)
 * AL4SAN_END_ACCESS_DECLARATION
 */

#define GET_MACRO_ACCESS_R(_1,_2,_3, NAME,...) NAME
#define AL4SAN_ACCESS_R(...) GET_MACRO_ACCESS_R(__VA_ARGS__, AL4SAN_ACCESS_R3, AL4SAN_ACCESS_R2, AL4SAN_ACCESS_R1)(__VA_ARGS__)

#define GET_MACRO_ACCESS_W(_1,_2,_3, NAME,...) NAME
#define AL4SAN_ACCESS_W(...) GET_MACRO_ACCESS_W(__VA_ARGS__, AL4SAN_ACCESS_W3, AL4SAN_ACCESS_W2, AL4SAN_ACCESS_W1)(__VA_ARGS__)

#define GET_MACRO_ACCESS_RW(_1,_2,_3, NAME,...) NAME
#define AL4SAN_ACCESS_RW(...) GET_MACRO_ACCESS_RW(__VA_ARGS__, AL4SAN_ACCESS_RW3, AL4SAN_ACCESS_RW2, AL4SAN_ACCESS_RW1)(__VA_ARGS__)

#if defined(AL4SAN_SCHED_STARPU) 
#define AL4SAN_BEGIN_ACCESS_DECLARATION { \
    unsigned __al4san_need_submit = 0; \
    AL4SAN_STARPU_BEGIN_ACCESS_DECLARATION

#define AL4SAN_ACCESS_R3(A, Am, An) do { \
    if (al4san_desc_islocal(A, Am, An)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_R(A, Am, An); \
} while(0)

#define AL4SAN_ACCESS_R2(A, Am) do { \
    if (al4san_desc_islocal(A, Am, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_R(A, Am, 0); \
} while(0)

#define AL4SAN_ACCESS_R1(A) do { \
    if (al4san_desc_islocal(A, 0, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_R(A, 0, 0); \
} while(0)

#define AL4SAN_ACCESS_W3(A, Am, An) do { \
    if (al4san_desc_islocal(A, Am, An)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_W(A, Am, An); \
} while(0)

#define AL4SAN_ACCESS_W2(A, Am) do { \
    if (al4san_desc_islocal(A, Am, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_W(A, Am, 0); \
} while(0)

#define AL4SAN_ACCESS_W1(A) do { \
    if (al4san_desc_islocal(A, 0, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_W(A, 0, 0); \
} while(0)

#define AL4SAN_ACCESS_RW3(A, Am, An) do { \
    if (al4san_desc_islocal(A, Am, An)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_RW(A, Am, An); \
} while(0)

#define AL4SAN_ACCESS_RW2(A, Am) do { \
    if (al4san_desc_islocal(A, Am, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_RW(A, Am, 0); \
} while(0)

#define AL4SAN_ACCESS_RW1(A) do { \
    if (al4san_desc_islocal(A, 0, 0)) __al4san_need_submit = 1; \
    AL4SAN_STARPU_ACCESS_RW(A, 0, 0); \
} while(0)
#define AL4SAN_RANK_CHANGED(rank) do {\
    __al4san_need_submit = 1; \
    AL4SAN_STARPU_RANK_CHANGED(rank); \
} while (0)

#define AL4SAN_END_ACCESS_DECLARATION \
    AL4SAN_STARPU_END_ACCESS_DECLARATION; \
    if (!__al4san_need_submit) return; \
}
#else
#define AL4SAN_BEGIN_ACCESS_DECLARATION 
#define AL4SAN_ACCESS_R3(A, Am, An)
#define AL4SAN_ACCESS_R2(A, Am)
#define AL4SAN_ACCESS_R1(A) 
#define AL4SAN_ACCESS_W3(A, Am, An)
#define AL4SAN_ACCESS_W2(A, Am)
#define AL4SAN_ACCESS_W1(A)
#define AL4SAN_ACCESS_RW3(A, Am, An)
#define AL4SAN_ACCESS_RW2(A, Am)
#define AL4SAN_ACCESS_RW1(A) 
#define AL4SAN_RANK_CHANGED(rank)
#define AL4SAN_END_ACCESS_DECLARATION 
#endif

#ifdef __cplusplus
}
#endif

#endif /* _AL4SAN_DESCRIPTOR_H_ */
