/**
 *
 * @file pdtrmm.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Al4saneleon dtrmm parallel algorithm
 *
 * @version 0.9.2
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 0.9.2
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2014-11-16
 * @generated d Tue Apr  2 10:58:06 2019
 *
 */
#include <al4san.h>
#include "control/common.h"
#define BLKLDD(A, k) A->get_blkldd( A, k )
#define A(m,n) A,  m,  n
#define B(m,n) B,  m,  n

/**
 *  Parallel tile triangular matrix-matrix multiplication - dynamic scheduling
 */
void eig_pdtrmm(int side, int uplo,
                         int trans, int diag,
                         double alpha,  AL4SAN_desc_t *A, AL4SAN_desc_t *B,
                         AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{

    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options; 

    int k, m, n;
    int ldak, ldam, ldan, ldbk, ldbm;
    int tempkm, tempkn, tempmm, tempnn;

    double zone = (double)1.0;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdplgsy", "AL4SAN not initialized");
        }
 
   AL4SAN_Options_Init(&options, sequence, request);
    /*
     *  Al4sanLeft / Al4sanUpper / Al4sanNoTrans
     */
    if (side == Al4sanLeft) {
        if (uplo == Al4sanUpper) {
            if (trans == Al4sanNoTrans) {
                for (m = 0; m < B->mt; m++) {
                    tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                    ldbm = BLKLDD(B, m);
                    ldam = BLKLDD(A, m);
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(m, m), ldam,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = m+1; k < A->mt; k++) {
                            tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                            ldbk = BLKLDD(B, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                tempmm, tempnn, tempkn, A->mb,
                                alpha, A(m, k), ldam,
                                       B(k, n), ldbk,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
            /*
             *  Al4sanLeft / Al4sanUpper / Al4san[Conj]Trans
             */
            else {
                for (m = B->mt-1; m > -1; m--) {
                    tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                    ldbm = BLKLDD(B, m);
                    ldam = BLKLDD(A, m);
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(m, m), ldam,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = 0; k < m; k++) {
                            ldbk = BLKLDD(B, k);
                            ldak = BLKLDD(A, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                tempmm, tempnn, B->mb, A->mb,
                                alpha, A(k, m), ldak,
                                       B(k, n), ldbk,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
        }
        /*
         *  Al4sanLeft / Al4sanLower / Al4sanNoTrans
         */
        else {
            if (trans == Al4sanNoTrans) {
                for (m = B->mt-1; m > -1; m--) {
                    tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                    ldbm = BLKLDD(B, m);
                    ldam = BLKLDD(A, m);
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(m, m), ldam,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = 0; k < m; k++) {
                            ldbk = BLKLDD(B, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                tempmm, tempnn, B->mb, A->mb,
                                alpha, A(m, k), ldam,
                                       B(k, n), ldbk,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
            /*
             *  Al4sanLeft / Al4sanLower / Al4san[Conj]Trans
             */
            else {
                for (m = 0; m < B->mt; m++) {
                    tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                    ldbm = BLKLDD(B, m);
                    ldam = BLKLDD(A, m);
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(m, m), ldam,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = m+1; k < A->mt; k++) {
                            tempkm = k == A->mt-1 ? A->m-k*A->mb : A->mb;
                            ldak = BLKLDD(A, k);
                            ldbk = BLKLDD(B, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                tempmm, tempnn, tempkm, A->mb,
                                alpha, A(k, m), ldak,
                                       B(k, n), ldbk,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
        }
    }
    /*
     *  Al4sanRight / Al4sanUpper / Al4sanNoTrans
     */
    else {
        if (uplo == Al4sanUpper) {
            if (trans == Al4sanNoTrans) {
                for (n = B->nt-1; n > -1; n--) {
                    tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                    ldan = BLKLDD(A, n);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(n, n), ldan,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = 0; k < n; k++) {
                            ldak = BLKLDD(A, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, tempnn, B->mb, A->mb,
                                alpha, B(m, k), ldbm,
                                       A(k, n), ldak,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
            /*
             *  Al4sanRight / Al4sanUpper / Al4san[Conj]Trans
             */
            else {
                for (n = 0; n < B->nt; n++) {
                    tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                    ldan = BLKLDD(A, n);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(n, n), ldan,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = n+1; k < A->mt; k++) {
                            tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, tempnn, tempkn, A->mb,
                                alpha, B(m, k), ldbm,
                                       A(n, k), ldan,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
        }
        /*
         *  Al4sanRight / Al4sanLower / Al4sanNoTrans
         */
        else {
            if (trans == Al4sanNoTrans) {
                for (n = 0; n < B->nt; n++) {
                    tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                    ldan = BLKLDD(A, n);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(n, n), ldan,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = n+1; k < A->mt; k++) {
                            tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                            ldak = BLKLDD(A, k);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, tempnn, tempkn, A->mb,
                                alpha, B(m, k), ldbm,
                                       A(k, n), ldak,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
            /*
             *  Al4sanRight / Al4sanLower / Al4san[Conj]Trans
             */
            else {
                for (n = B->nt-1; n > -1; n--) {
                    tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                    ldan = BLKLDD(A, n);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrmm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempnn, A->mb,
                            alpha, A(n, n), ldan,  /* lda * tempkm */
                                   B(m, n), ldbm); /* ldb * tempnn */

                        for (k = 0; k < n; k++) {
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, tempnn, B->mb, A->mb,
                                alpha, B(m, k), ldbm,
                                       A(n, k), ldan,
                                zone,  B(m, n), ldbm);
                        }
                    }
                }
            }
        }
    }

    AL4SAN_Options_Finalize(&options);
}
