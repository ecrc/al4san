/**
 *
 * @file core_dsbtype3cb.c
 *
 *  PLASMA core_blas kernel
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2011-05-15
 * @generated d Fri Apr  1 11:02:31 2016
 *
 **/
#include <lapacke.h>
#include "common.h"
#include "bulge-chasing.h"
#define A(m,n)   (A + LDA * (n) + ((m)-(n)))
#define V(m)     (V + (m))
#define TAU(m)   (TAU + (m))

/***************************************************************************//**
 *
 * @ingroup CORE_double
 *
 *  CORE_dsbtype3cb is a kernel that will operate on a region (triangle) of data
 *  bounded by st and ed. This kernel apply a left+right update on the hermitian
 *  triangle.  Note that this kernel is very similar to type1 but does not do an
 *  elimination.
 *
 *  All detail are available on technical report or SC11 paper.
 *  Azzam Haidar, Hatem Ltaief, and Jack Dongarra. 2011.
 *  Parallel reduction to condensed forms for symmetric eigenvalue problems
 *  using aggregated fine-grained and memory-aware kernels. In Proceedings
 *  of 2011 International Conference for High Performance Computing,
 *  Networking, Storage and Analysis (SC '11). ACM, New York, NY, USA, ,
 *  Article 8 , 11 pages.
 *  http://doi.acm.org/10.1145/2063384.2063394
 *
 *******************************************************************************
 *
 * @param[in] N
 *          The order of the matrix A.
 *
 * @param[in] NB
 *          The size of the band.
 *
 * @param[in, out] A
 *          A pointer to the matrix A of size (2*NB+1)-by-N.
 *
 * @param[in] LDA
 *          The leading dimension of the matrix A. LDA >= max(1,2*NB+1)
 *
 * @param[in] V
 *          double array, dimension N if eigenvalue only
 *          requested or (LDV*blkcnt*Vblksiz) if Eigenvectors requested
 *          The Householder reflectors are stored in this array.
 *
 * @param[in] TAU
 *          double array, dimension (N).
 *          The scalar factors of the Householder reflectors are stored
 *          in this array.
 *
 * @param[in] st
 *          A pointer to the start index where this kernel will operate.
 *
 * @param[in] ed
 *          A pointer to the end index where this kernel will operate.
 *
 * @param[in] sweep
 *          The sweep number that is eliminated. it serve to calculate the
 *          pointer to the position where to store the Vs and Ts.
 *
 * @param[in] Vblksiz
 *          constant which correspond to the blocking used when applying the Vs.
 *          it serve to calculate the pointer to the position where to store the
 *          Vs and Ts.
 *
 * @param[in] WANTZ
 *          constant which indicate if Eigenvalue are requested or both
 *          Eigenvalue/Eigenvectors.
 *
 * @param[in] WORK
 *          Workspace of size nb.
 *
 *******************************************************************************
 *
 * @return
 *          \retval PLASMA_SUCCESS successful exit
 *          \retval <0 if -i, the i-th argument had an illegal value
 *
 ******************************************************************************/

/***************************************************************************//**
 *          TYPE 3-BAND Lower-columnwise-Householder
 ***************************************************************************/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak CORE_dsbtype3cb = PCORE_dsbtype3cb
#define CORE_dsbtype3cb PCORE_dsbtype3cb
#endif
void CORE_dsbtype3cb(int N, int NB,
                     double *A, int LDA,
                     const double *V, const double *TAU,
                     int st, int ed, int sweep, int Vblksiz, int WANTZ,
                     double *WORK)
{
    int len, LDX;
    int blkid, vpos, taupos, tpos;

    if( WANTZ == 0 ) {
        vpos   = ((sweep+1)%2)*N + st;
        taupos = ((sweep+1)%2)*N + st;
    } else {
        eig_findVTpos(N, NB, Vblksiz, sweep, st,
                  &vpos, &taupos, &tpos, &blkid);
    }

    LDX = LDA-1;
    len = ed-st+1;

    /* Apply left and right on A(st:ed,st:ed)*/
    CORE_dlarfy(len, A(st,st), LDX, V(vpos), TAU(taupos), WORK);
    return;
}
/***************************************************************************/
#undef A
#undef V
#undef TAU

