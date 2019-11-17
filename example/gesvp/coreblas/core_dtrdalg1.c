/**
 *
 * @file core_dtrdalg1.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2013-07-04
 * @generated d Fri Apr  1 11:02:32 2016
 *
 **/
#include <lapacke.h>
#include "common.h"

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dtrdalg1 is a part of the tridiagonal reduction algorithm (bulgechasing)
 *  It correspond to a local driver of the kernels that should be executed on a
 *  single core.
 *
 *******************************************************************************
 *
 * @param[in] n
 *          The order of the matrix A. n >= 0.
 *
 * @param[in] nb
 *          The size of the Bandwidth of the matrix A,
 *          which correspond to the tile size. nb >= 0.
 *
 * @param[in,out] A
 *          double array, dimension (lda,n)
 *          On entry, the (nb+1)-by-n band lower hermetian matrix to
 *          be reduced to tridiagonal.
 *          On exit, the diagonal and first subdiagonal of A are over-
 *          written by the corresponding elements of the tridiagonal.
 *
 * @param[in] lda
 *          (input) INTEGER
 *          The leading dimension of the array A.  LDA >= max(1,nb+1).
 *
 * @param[out] V
 *          double array, dimension (n) if wantz=0
 *          or ldv*Vblksiz*blkcnt if wantz>0.
 *          The scalar elementary reflectors are written in this
 *          array.
 *
 * @param[out] TAU
 *          double array, dimension (n) if wantz=0
 *          or Vblksiz*Vblksiz*blkcnt if wantz>0.
 *          The scalar factors of the elementary reflectors are written
 *          in this array.
 *
 * @param[in] Vblksiz
 *          Local parameter to Plasma. It correspond to the local bloccking
 *          of the applyQ2 used to apply the orthogonal matrix Q2.
 *
 * @param[in] wantz
 *          integer tobe 0 or 1. if wantz=0 the V and TAU are not stored on
 *          only they are kept for next step then overwritten.
 *
 * @param[in] i
 *          Integer that refer to the current sweep. (outer loop).
 *
 * @param[in] sweepid
 *          Integer that refer to the sweep to chase.(inner loop).
 *
 * @param[in] m
 *          Integer that refer to a sweep step, to ensure order dependencies.
 *
 * @param[in] grsiz
 *          Integer that refer to the size of a group.
 *          group mean the number of kernel that should be executed sequentially
 *          on the same core.
 *          group size is a trade-off between locality (cache reuse) and parallelism.
 *          a small group size increase parallelism while a large group size increase
 *          cache reuse.
 *
 * @param[in] work
 *          Workspace of size nb. Used by the core_dsbtype[123]cb.
 *
 ******************************************************************************/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dtrdalg1 = PCORE_dtrdalg1
#define CORE_dtrdalg1 PCORE_dtrdalg1
#endif
void CORE_dtrdalg1( int n, int nb,
                    double *A, int lda,
                    double *V,
                    double *TAU,
                    int Vblksiz, int wantz,
                    int i, int sweepid, int m, int grsiz,
                    double *work)
{
    int k, shift=3;
    int myid, colpt, stind, edind, blklastind, stepercol;

    k = shift / grsiz;
    stepercol = (k*grsiz == shift) ? k : k+1;
    for (k = 0; k < grsiz; k++){
        myid = (i-sweepid)*(stepercol*grsiz) +(m-1)*grsiz + k+1;
        if(myid%2 ==0) {
            colpt      = (myid/2) * nb + 1 + sweepid - 1;
            stind      = colpt - nb + 1;
            edind      = min(colpt, n);
            blklastind = colpt;
        } else {
            colpt      = ((myid+1)/2)*nb + 1 +sweepid -1 ;
            stind      = colpt-nb+1;
            edind      = min(colpt,n);
            if( (stind>=edind-1) && (edind==n) )
                blklastind = n;
            else
                blklastind = 0;
        }

        if( myid == 1 )
           CORE_dsbtype1cb(n, nb, A, lda, V, TAU, stind-1, edind-1, sweepid-1, Vblksiz, wantz, work);
        else if(myid%2 == 0)
           CORE_dsbtype2cb(n, nb, A, lda, V, TAU, stind-1, edind-1, sweepid-1, Vblksiz, wantz, work);
        else /*if(myid%2 == 1)*/
           CORE_dsbtype3cb(n, nb, A, lda, V, TAU, stind-1, edind-1, sweepid-1, Vblksiz, wantz, work);

        if(blklastind >= (n-1))  break;
    }
}
