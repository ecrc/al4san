/**
 *
 * @file eig_dtrsm.c
 *
 * @copyright 2009-2015 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Al4san dtrsm parallel algorithm
 *
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2014-11-16
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @generated d Tue Apr  2 10:58:06 2019
 *
 */
#include <al4san.h>
#include "control/common.h"

#define A(m,n) A,  m,  n
#define B(m,n) B,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  Parallel tile triangular solve - dynamic scheduling
 */
void eig_pdtrsm(int side, int uplo, int trans, int diag,
                         double alpha, AL4SAN_desc_t *A, AL4SAN_desc_t *B,
                         AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
   /*
     * Define AL4SAN handle for seqeunce to manage groupe of threads.
     * Define AL4SAN handle for options to set glabel task options and set the sequence handle.
     * Define AL4SAN handle for request status.
   */
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options;

    int k, m, n;
    int ldak, ldam, ldan, ldbk, ldbm;
    int tempkm, tempkn, tempmm, tempnn;

    double zone       = (double) 1.0;
    double mzone      = (double)-1.0;
    double minvalpha  = (double)-1.0 / alpha;
    double lalpha;

     al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdtrsm", "AL4SAN not initialized");
        }

    AL4SAN_Options_Init(&options, sequence, request);
    /*
     *  Al4sanLeft / Al4sanUpper / Al4sanNoTrans
     */
    if (side == Al4sanLeft) {
        if (uplo == Al4sanUpper) {
            if (trans == Al4sanNoTrans) {
                for (k = 0; k < B->mt; k++) {
                    tempkm = k == 0 ? B->m-(B->mt-1)*B->mb : B->mb;
                    ldak = BLKLDD(A, B->mt-1-k);
                    ldbk = BLKLDD(B, B->mt-1-k);
                    lalpha = k == 0 ? alpha : zone;
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempkm, tempnn, A->mb,
                            lalpha, A(B->mt-1-k, B->mt-1-k), ldak,  /* lda * tempkm */
                                    B(B->mt-1-k,        n), ldbk); /* ldb * tempnn */
                    }
                    AL4SAN_Data_Flush( sequence, A(B->mt-1-k, B->mt-1-k) );
                    for (m = k+1; m < B->mt; m++) {
                        ldam = BLKLDD(A, B->mt-1-m);
                        ldbm = BLKLDD(B, B->mt-1-m);
                        for (n = 0; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, Al4sanNoTrans,
                                B->mb, tempnn, tempkm, A->mb,
                                mzone,  A(B->mt-1-m, B->mt-1-k), ldam,
                                        B(B->mt-1-k, n       ), ldbk,
                                lalpha, B(B->mt-1-m, n       ), ldbm);
                        }
                        AL4SAN_Data_Flush( sequence, A(B->mt-1-m, B->mt-1-k) );
                    }
                    for (n = 0; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, B(B->mt-1-k, n) );
                    }
                }
            }
            /*
             *  Al4sanLeft / Al4sanUpper / Al4san[Conj]Trans
             */
            else {
                for (k = 0; k < B->mt; k++) {
                    tempkm = k == B->mt-1 ? B->m-k*B->mb : B->mb;
                    ldak = BLKLDD(A, k);
                    ldbk = BLKLDD(B, k);
                    lalpha = k == 0 ? alpha : zone;
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempkm, tempnn, A->mb,
                            lalpha, A(k, k), ldak,
                                    B(k, n), ldbk);
                    }
                    AL4SAN_Data_Flush( sequence, A(k, k) );
                    for (m = k+1; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        for (n = 0; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                tempmm, tempnn, B->mb, A->mb,
                                mzone,  A(k, m), ldak,
                                        B(k, n), ldbk,
                                lalpha, B(m, n), ldbm);
                        }
                        AL4SAN_Data_Flush( sequence, A(k, m) );
                    }
                    for (n = 0; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, B(k, n) );
                    }

                }
            }
        }
        /*
         *  Al4sanLeft / Al4sanLower / Al4sanNoTrans
         */
        else {
            if (trans == Al4sanNoTrans) {
                for (k = 0; k < B->mt; k++) {
                    tempkm = k == B->mt-1 ? B->m-k*B->mb : B->mb;
                    ldak = BLKLDD(A, k);
                    ldbk = BLKLDD(B, k);
                    lalpha = k == 0 ? alpha : zone;
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempkm, tempnn, A->mb,
                            lalpha, A(k, k), ldak,
                                    B(k, n), ldbk);
                    }
                    AL4SAN_Data_Flush( sequence, A(k, k) );
                    for (m = k+1; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldam = BLKLDD(A, m);
                        ldbm = BLKLDD(B, m);
                        for (n = 0; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, Al4sanNoTrans,
                                tempmm, tempnn, B->mb, A->mb,
                                mzone,  A(m, k), ldam,
                                        B(k, n), ldbk,
                                lalpha, B(m, n), ldbm);
                        }
                        AL4SAN_Data_Flush( sequence, A(m, k) );
                    }
                    for (n = 0; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, B(k, n) );
                    }
                }
            }
            /*
             *  Al4sanLeft / Al4sanLower / Al4san[Conj]Trans
             */
            else {
                for (k = 0; k < B->mt; k++) {
                    tempkm = k == 0 ? B->m-(B->mt-1)*B->mb : B->mb;
                    ldak = BLKLDD(A, B->mt-1-k);
                    ldbk = BLKLDD(B, B->mt-1-k);
                    lalpha = k == 0 ? alpha : zone;
                    for (n = 0; n < B->nt; n++) {
                        tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempkm, tempnn, A->mb,
                            lalpha, A(B->mt-1-k, B->mt-1-k), ldak,
                                    B(B->mt-1-k,        n), ldbk);
                    }
                    AL4SAN_Data_Flush( sequence, A(B->mt-1-k, B->mt-1-k) );
                    for (m = k+1; m < B->mt; m++) {
                        ldbm = BLKLDD(B, B->mt-1-m);
                        for (n = 0; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                trans, Al4sanNoTrans,
                                B->mb, tempnn, tempkm, A->mb,
                                mzone,  A(B->mt-1-k, B->mt-1-m), ldak,
                                        B(B->mt-1-k, n       ), ldbk,
                                lalpha, B(B->mt-1-m, n       ), ldbm);
                        }
                        AL4SAN_Data_Flush( sequence, A(B->mt-1-k, B->mt-1-m) );
                    }
                    for (n = 0; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, B(B->mt-1-k, n) );
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
                for (k = 0; k < B->nt; k++) {
                    tempkn = k == B->nt-1 ? B->n-k*B->nb : B->nb;
                    ldak = BLKLDD(A, k);
                    lalpha = k == 0 ? alpha : zone;
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempkn, A->mb,
                            lalpha, A(k, k), ldak,  /* lda * tempkn */
                                    B(m, k), ldbm); /* ldb * tempkn */
                    }
                    AL4SAN_Data_Flush( sequence, A(k, k) );
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        for (n = k+1; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, Al4sanNoTrans,
                                tempmm, tempnn, B->mb, A->mb,
                                mzone,  B(m, k), ldbm,  /* ldb * B->mb   */
                                        A(k, n), ldak,  /* lda * tempnn */
                                lalpha, B(m, n), ldbm); /* ldb * tempnn */
                        }
                        AL4SAN_Data_Flush( sequence, B(m, k) );
                    }
                    for (n = k+1; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, A(k, n) );
                    }
                }
            }
            /*
             *  Al4sanRight / Al4sanUpper / Al4san[Conj]Trans
             */
            else {
                for (k = 0; k < B->nt; k++) {
                    tempkn = k == 0 ? B->n-(B->nt-1)*B->nb : B->nb;
                    ldak = BLKLDD(A, B->nt-1-k);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempkn, A->mb,
                            alpha, A(B->nt-1-k, B->nt-1-k), ldak,  /* lda * tempkn */
                                   B(       m, B->nt-1-k), ldbm); /* ldb * tempkn */
                        AL4SAN_Data_Flush( sequence, A(B->nt-1-k, B->nt-1-k) );

                        for (n = k+1; n < B->nt; n++) {
                            ldan = BLKLDD(A, B->nt-1-n);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, B->nb, tempkn, A->mb,
                                minvalpha, B(m,        B->nt-1-k), ldbm,  /* ldb  * tempkn */
                                           A(B->nt-1-n, B->nt-1-k), ldan, /* A->mb * tempkn (Never last row) */
                                zone,      B(m,        B->nt-1-n), ldbm); /* ldb  * B->nb   */
                        }
                        AL4SAN_Data_Flush( sequence, B(m,        B->nt-1-k) );
                    }
                    for (n = k+1; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, A(B->nt-1-n, B->nt-1-k) );
                    }
                }
            }
        }
        /*
         *  Al4sanRight / Al4sanLower / Al4sanNoTrans
         */
        else {
            if (trans == Al4sanNoTrans) {
                for (k = 0; k < B->nt; k++) {
                    tempkn = k == 0 ? B->n-(B->nt-1)*B->nb : B->nb;
                    ldak = BLKLDD(A, B->nt-1-k);
                    lalpha = k == 0 ? alpha : zone;
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempkn, A->mb,
                            lalpha, A(B->nt-1-k, B->nt-1-k), ldak,  /* lda * tempkn */
                                    B(       m, B->nt-1-k), ldbm); /* ldb * tempkn */
                        AL4SAN_Data_Flush( sequence, A(B->nt-1-k, B->nt-1-k) );

                        for (n = k+1; n < B->nt; n++) {
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, Al4sanNoTrans,
                                tempmm, B->nb, tempkn, A->mb,
                                mzone,  B(m,        B->nt-1-k), ldbm,  /* ldb * tempkn */
                                        A(B->nt-1-k, B->nt-1-n), ldak,  /* lda * B->nb   */
                                lalpha, B(m,        B->nt-1-n), ldbm); /* ldb * B->nb   */
                        }
                        AL4SAN_Data_Flush( sequence, B(m,        B->nt-1-k) );
                    }
                    for (n = k+1; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, A(B->nt-1-k, B->nt-1-n) );
                    }
                }
            }
            /*
             *  Al4sanRight / Al4sanLower / Al4san[Conj]Trans
             */
            else {
                for (k = 0; k < B->nt; k++) {
                    tempkn = k == B->nt-1 ? B->n-k*B->nb : B->nb;
                    ldak = BLKLDD(A, k);
                    for (m = 0; m < B->mt; m++) {
                        tempmm = m == B->mt-1 ? B->m-m*B->mb : B->mb;
                        ldbm = BLKLDD(B, m);
                        EIG_AL4SAN_CORE_dtrsm(
                            &options,
                            side, uplo, trans, diag,
                            tempmm, tempkn, A->mb,
                            alpha, A(k, k), ldak,  /* lda * tempkn */
                                   B(m, k), ldbm); /* ldb * tempkn */
                        AL4SAN_Data_Flush( sequence, A(k, k) );

                        for (n = k+1; n < B->nt; n++) {
                            tempnn = n == B->nt-1 ? B->n-n*B->nb : B->nb;
                            ldan = BLKLDD(A, n);
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, trans,
                                tempmm, tempnn, B->mb, A->mb,
                                minvalpha, B(m, k), ldbm,  /* ldb  * tempkn */
                                           A(n, k), ldan, /* ldan * tempkn */
                                zone,      B(m, n), ldbm); /* ldb  * tempnn */
                        }
                        AL4SAN_Data_Flush( sequence, B(m, k) );
                    }
                    for (n = k+1; n < B->nt; n++) {
                        AL4SAN_Data_Flush( sequence, A(n, k) );
                    }

                }
            }
        }
    }
    AL4SAN_Options_Finalize(&options);
}

