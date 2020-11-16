/**
 *
 * @file eig_pdsytrd_sy2sb.c
 *
 * @copyright 2009-2015 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 * @brief Al4san dtrsm parallel algorithm
 *
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @generated d Tue Apr  2 10:58:06 2019
 *
 */
#include <al4san.h>
#include <stdlib.h>
#include "control/al4san_common.h"
//#include "control/common.h"
#define A(m, n) A,  m,  n
#define T(m, n) T,  m,  n
#define D(k) &D, (k)-1, 0

#define AT(k) &AT, k, 0
#define AT2(k) &AT2, k, 0

#if defined(AL4SAN_COPY_DIAG)
#define E(m, n) E, m, 0
#else
#define E(m, n) A, m, n
#endif
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  Parallel tile BAND Tridiagonal Reduction - dynamic scheduler
 */
void eig_pdsytrd_sy2sb(int uplo,
                         AL4SAN_desc_t *A, AL4SAN_desc_t *T, AL4SAN_desc_t *E,
                         AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options;

    AL4SAN_desc_t D;
    AL4SAN_desc_t AT;
    size_t ws_worker = 0;
    size_t ws_host = 0;

    int k, m, n, i, j;
    int ldak, ldak1, ldam, ldan, ldaj, ldai;
    int tempkm, tempkn, tempmm, tempnn, tempjj;
    int ib;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("eig_pdsytrd_sy2sb", "AL4SAN not initialized");
    }
    AL4SAN_Options_Init(&options, sequence, request);
    ib = T->mb;

    /*
     * dgeqrt        = A->nb * (ib+1)
     * dormqr        = A->nb * ib
     * dtsqrt        = A->nb * (ib+1)
     * dtsmqr        = A->nb * ib
     * dsyrfb        = A->nb * ib
     * dtsmqr_hetra1 = A->nb * ib
     */
    ws_worker = A->nb * (ib+1);

    /* Allocation of temporary (scratch) working space */
#if defined(AL4SAN_USE_CUDA)
    /* Worker space
     *
     * dormqr =     A->nb * ib
     * dtsmqr = 2 * A->nb * ib
     * dsyrfb =     A->nb * ib
     */
    ws_worker = al4san_max( ws_worker, ib * A->nb * 2 );
#endif

    ws_worker *= sizeof(double);
    ws_host   *= sizeof(double);

    AL4SAN_Options_Workspace_Alloc( &options, ws_worker, ws_host );

    /* Copy of the diagonal tiles to keep the general version of the tile all along the computation */
    //chameleon_ddesc_alloc_diag( &D, A->mb, A->m, A->n, A->p, A->q );

     int diag_m = al4san_min( A->m,  A->n );
     al4san_desc_init( &D, AL4SAN_MAT_ALLOC_GLOBAL, Al4sanRealDouble,  A->mb, A->mb, (A->mb*A->mb),
                                       diag_m, A->nb, 0, 0, diag_m, A->mb, 1, 1,
                                       al4san_getaddr_diag, 
                                       al4san_getblkldd_ccrb, 
                                       al4san_getrankof_2d_diag);

     al4san_desc_init( &AT, AL4SAN_MAT_ALLOC_GLOBAL, Al4sanRealDouble, A->mb, A->nb, (A->mb*A->nb),
                         al4san_min(A->mt, A->nt) * A->mb, A->nb, 0, 0,
                         al4san_min(A->mt, A->nt) * A->mb, A->nb, 1, 1,
                         NULL, NULL, NULL );

    /* Let's extract the diagonal in a temporary copy that contains A and A' */
    for (k = 1; k < A->nt; k++){
        tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
        ldak = BLKLDD(A, k);

        EIG_AL4SAN_CORE_dhe2ge(&options,
                          uplo,
                          tempkn, tempkn, ldak,
                          A(k, k), ldak,
                          D(k),    ldak);
    }

    if (uplo == Al4sanLower) {
       for (k = 0; k < A->nt-1; k++){
           tempkm = k+1 == A->mt-1 ? A->m-(k+1)*A->mb : A->mb;
           tempkn = k   == A->nt-1 ? A->n- k   *A->nb : A->nb;
           ldak1 = BLKLDD(A, k+1);

           EIG_AL4SAN_CORE_dgeqrt(
               &options,
               tempkm, tempkn, ib, A->nb,
               A(k+1, k), ldak1,
               T(k+1, k), T->mb);

#if defined(AL4SAN_COPY_DIAG)
           EIG_AL4SAN_CORE_dlacpy1(
               &options,
               Al4sanLower, tempkm, tempkn, A->nb,
               A(k+1, k), ldak1,
               E(k+1, k), ldak1 );
#if defined(AL4SAN_USE_CUDA)
           EIG_AL4SAN_CORE_dlaset(
               &options,
               Al4sanUpper, tempkm, tempkn,
               0., 1.,
               E(k+1, k), ldak1 );
#endif
#endif
           /* LEFT and RIGHT on the symmetric diagonal block */
           EIG_AL4SAN_CORE_dsyrfb(
               &options,
               Al4sanLower,
               tempkm, tempkm, ib, A->nb,
               E(k+1, k), ldak1,
               T(k+1, k), T->mb,
               D(k+1),    ldak1);

           /* RIGHT on the remaining tiles until the bottom */
           for (m = k+2; m < A->mt ; m++) {
               tempmm = m == A->mt-1 ? A->m-m*A->mb : A->mb;
               ldam = BLKLDD(A, m);
               EIG_AL4SAN_CORE_dormqr(
                   &options,
                   Al4sanRight, Al4sanNoTrans,
                   tempmm, A->nb, tempkm, ib, A->nb,
                   E(k+1, k),   ldak1,
                   T(k+1, k),   T->mb,
                   A(m,   k+1), ldam);
           }

           for (m = k+2; m < A->mt; m++) {
               tempmm = m == A->mt-1 ? A->m-m*A->mb : A->mb;
               ldam = BLKLDD(A, m);
               options.priority = 1;
               EIG_AL4SAN_CORE_dtsqrt(
                   &options,
                   tempmm, A->nb, ib, A->nb,
                   A(k+1, k), ldak1,
                   A(m  , k), ldam,
                   T(m  , k), T->mb);
          //     options.priority = 0;

               /* LEFT */
               for (i = k+2; i < m; i++) {
                   ldai = BLKLDD(A, i);
               if (i==k+2) options.priority = (A->nt)-k-3;
               else options.priority = (A->nt)-i-4;

               /*EIG_AL4SAN_CORE_dlatro(
                   &options,
                   Al4sanUpperLower, Al4sanTrans,
                   tempmm, A->nb, A->nb,
                   A(k+1, i), ldam,
                   AT2(i),  ldak1);*/

                   EIG_AL4SAN_CORE_dtsmqr_hetra1(
                       &options,
                       Al4sanLeft, Al4sanTrans,
                       A->mb, A->nb, tempmm, A->nb, A->nb, ib, A->nb,
                       A(i, k+1), ldai,
                       A(m,   i), ldam,
                       A(m,   k), ldam,
                       T(m,   k), T->mb);
               }

               /* RIGHT */
               for (j = m+1; j < A->mt ; j++) {
                   tempjj = j == A->mt-1 ? A->m-j*A->mb : A->mb;
                   ldaj = BLKLDD(A, j);
                   EIG_AL4SAN_CORE_dtsmqr(
                       &options,
                       Al4sanRight, Al4sanNoTrans,
                       tempjj, A->nb, tempjj, tempmm, A->nb, ib, A->nb,
                       A(j, k+1), ldaj,
                       A(j,   m), ldaj,
                       A(m,   k), ldam,
                       T(m,   k), T->mb);
               }

               /* LEFT->RIGHT */
               options.priority = 1;
               /**
                * Compute the update of the following:
                *
                *     A1   A2'
                *     A2   A3
                *
                * with A1 and A3 two diagonal tiles. This is the tsmqr_corner
                * from plasma split in 4 tasks
                */
               /*  Copy the transpose of A2 (m, k+1): AT(k) <- A2' = A2(k+1, m) */
               EIG_AL4SAN_CORE_dlatro1(
                   &options,
                   Al4sanUpperLower, Al4sanTrans,
                   tempmm, A->nb, A->nb,
                   A(m, k+1), ldam,
                   AT(m),  ldak1);
               /*  Left application on |A1| */
               /*                      |A2| */
               EIG_AL4SAN_CORE_dtsmqr(
                   &options,
                   Al4sanLeft, Al4sanTrans,
                   A->mb, A->nb, tempmm, A->nb, A->nb, ib, A->nb,
                   D(k+1), ldak1,
                   A(m, k+1), ldam,
                   A(m,   k), ldam,
                   T(m,   k), T->mb);

               /*  Left application on | A2'| */
               /*                      | A3 | */
               EIG_AL4SAN_CORE_dtsmqr(
                   &options,
                   Al4sanLeft, Al4sanTrans,
                   A->mb, tempmm, tempmm, tempmm, A->nb, ib, A->nb,
                   AT(m), ldak1,
                   D(m) , ldam,
                   A(m,  k), ldam,
                   T(m,  k), T->mb);

               /*  Right application on | A1 A2' | */
               EIG_AL4SAN_CORE_dtsmqr(
                   &options,
                   Al4sanRight, Al4sanNoTrans,
                   A->mb, A->nb, A->mb, tempmm, A->nb, ib, A->nb,
                   D(k+1), ldak1,
                   AT(m) , ldak1,
                   A(m,   k), ldam,
                   T(m,   k), T->mb);

               /*  Right application on | A2 A3 | */
               EIG_AL4SAN_CORE_dtsmqr(
                   &options,
                   Al4sanRight, Al4sanNoTrans,
                   tempmm, A->nb, tempmm, tempmm, A->nb, ib, A->nb,
                   A(m, k+1), ldam,
                   D(m)  , ldam,
                   A(m,   k), ldam,
                   T(m,   k), T->mb);

               options.priority = 0;
           }

       }
    }

    /* Copy-back into A */
    for (k = 1; k < A->nt; k++){
        tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
        ldak = BLKLDD(A, k);
        EIG_AL4SAN_CORE_dlacpy1(&options,
                          uplo,
                          tempkn, tempkn, ldak,
                          D(k), ldak,
                          A(k, k), ldak);
    }

    AL4SAN_Options_Workspace_Free(&options);
    AL4SAN_Options_Finalize(&options);

//    AL4SAN_Sequence_Wait(sequence);
    al4san_desc_destroy( &D );
    al4san_desc_destroy( &AT );
    (void)E;
}
