/**
 *
 * @file eig_dsyr2k.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 * @brief Al4sane dsymm parallel algorithm
 *
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2014-11-16
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @generated d Tue Apr  2 10:58:05 2019
 *
 */
#include <al4san.h>
#include "control/common.h"

#define A(m,n) A,  m,  n
#define B(m,n) B,  m,  n
#define C(m,n) C,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  Parallel tile Symmetric rank-k update - dynamic scheduling
 */
void eig_pdsyr2k(int uplo, int trans,
                          double alpha, AL4SAN_desc_t *A, AL4SAN_desc_t *B,
                          double beta,  AL4SAN_desc_t *C,
                          AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
   /*
     * Define AL4SAN handle for seqeunce to manage groupe of threads.
     * Define AL4SAN handle for options to set glabel task options and set the sequence handle.
     * Define AL4SAN handle for request status.
   */
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options;

    int m, n, k;
    int ldak, ldam, ldan, ldcm, ldcn;
    int ldbk, ldbm, ldbn;
    int tempnn, tempmm, tempkn, tempkm;

    double zone   = (double)1.0;
    double zbeta;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdsyr2k", "AL4SAN not initialized");
        }
 
    AL4SAN_Options_Init(&options, sequence, request);

    for (n = 0; n < C->nt; n++) {
        tempnn = n == C->nt-1 ? C->n-n*C->nb : C->nb;
        ldan = BLKLDD(A, n);
        ldbn = BLKLDD(B, n);
        ldcn = BLKLDD(C, n);
        /*
         *  Al4sanNoTrans
         */
        if (trans == Al4sanNoTrans) {
            for (k = 0; k < A->nt; k++) {
                tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                zbeta = k == 0 ? beta : zone;
                EIG_AL4SAN_CORE_dsyr2k(
                    &options,
                    uplo, trans,
                    tempnn, tempkn, A->mb,
                    alpha, A(n, k), ldan, /* ldan * K */
                           B(n, k), ldbn,
                    zbeta, C(n, n), ldcn); /* ldc  * N */
            }
            /*
             *  Al4sanNoTrans / Al4sanLower
             */
            if (uplo == Al4sanLower) {
                for (m = n+1; m < C->mt; m++) {
                    tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                    ldam = BLKLDD(A, m);
                    ldbm = BLKLDD(B, m);
                    ldcm = BLKLDD(C, m);
                    for (k = 0; k < A->nt; k++) {
                        tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                        zbeta = k == 0 ? beta : zone;
                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanTrans,
                            tempmm, tempnn, tempkn, A->mb,
                            alpha, A(m, k), ldam,  /* ldam * K */
                                   B(n, k), ldbn,  /* ldan * K */
                            zbeta, C(m, n), ldcm); /* ldc  * N */

                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanTrans,
                            tempmm, tempnn, tempkn, A->mb,
                            alpha, B(m, k), ldbm,  /* ldam * K */
                                   A(n, k), ldan,  /* ldan * K */
                            zone,  C(m, n), ldcm); /* ldc  * N */
                    }
                }
            }
            /*
             *  Al4sanNoTrans / Al4sanUpper
             */
            else {
                for (m = n+1; m < C->mt; m++) {
                    tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                    ldam = BLKLDD(A, m);
                    ldbm = BLKLDD(B, m);
                    for (k = 0; k < A->nt; k++) {
                        tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
                        zbeta = k == 0 ? beta : zone;
                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanTrans,
                            tempnn, tempmm, tempkn, A->mb,
                            alpha, A(n, k), ldan,  /* ldan * K */
                                   B(m, k), ldbm,  /* ldam * M */
                            zbeta, C(n, m), ldcn); /* ldc  * M */

                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanTrans,
                            tempnn, tempmm, tempkn, A->mb,
                            alpha, B(n, k), ldan,  /* ldan * K */
                                   A(m, k), ldam,  /* ldam * M */
                            zone,  C(n, m), ldcn); /* ldc  * M */
                    }
                }
            }
        }
        /*
         *  Al4san[Conj]Trans
         */
        else {
            for (k = 0; k < A->mt; k++) {
                tempkm = k == A->mt-1 ? A->m-k*A->mb : A->mb;
                ldak = BLKLDD(A, k);
                ldbk = BLKLDD(B, k);
                zbeta = k == 0 ? beta : zone;
                EIG_AL4SAN_CORE_dsyr2k(
                    &options,
                    uplo, trans,
                    tempnn, tempkm, A->mb,
                    alpha, A(k, n), ldak,  /* lda * N */
                           B(k, n), ldbk,
                    zbeta, C(n, n), ldcn); /* ldc * N */
            }
            /*
             *  Al4san[Conj]Trans / Al4sanLower
             */
            if (uplo == Al4sanLower) {
                for (m = n+1; m < C->mt; m++) {
                    tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                    ldcm = BLKLDD(C, m);
                    for (k = 0; k < A->mt; k++) {
                        tempkm = k == A->mt-1 ? A->m-k*A->mb : A->mb;
                        ldak = BLKLDD(A, k);
                        ldbk = BLKLDD(B, k);
                        zbeta = k == 0 ? beta : zone;
                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanNoTrans,
                            tempmm, tempnn, tempkm, A->mb,
                            alpha, A(k, m), ldak,  /* lda * M */
                                   B(k, n), ldbk,  /* lda * N */
                            zbeta, C(m, n), ldcm); /* ldc * N */

                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanNoTrans,
                            tempmm, tempnn, tempkm, A->mb,
                            alpha, B(k, m), ldbk,  /* lda * M */
                                   A(k, n), ldak,  /* lda * N */
                            zone,  C(m, n), ldcm); /* ldc * N */
                    }
                }
            }
            /*
             *  Al4san[Conj]Trans / Al4sanUpper
             */
            else {
                for (m = n+1; m < C->mt; m++) {
                    tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                    for (k = 0; k < A->mt; k++) {
                        tempkm = k == A->mt-1 ? A->m-k*A->mb : A->mb;
                        ldak = BLKLDD(A, k);
                        ldbk = BLKLDD(B, k);
                        zbeta = k == 0 ? beta : zone;
                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanNoTrans,
                            tempnn, tempmm, tempkm, A->mb,
                            alpha, A(k, n), ldak,  /* lda * K */
                                   B(k, m), ldbk,  /* lda * M */
                            zbeta, C(n, m), ldcn); /* ldc * M */

                        EIG_AL4SAN_CORE_dgemm(
                            &options,
                            trans, Al4sanNoTrans,
                            tempnn, tempmm, tempkm, A->mb,
                            alpha, B(k, n), ldbk,  /* lda * K */
                                   A(k, m), ldak,  /* lda * M */
                            zone,  C(n, m), ldcn); /* ldc * M */
                    }
                }
            }
        }
    }
    AL4SAN_Options_Finalize(&options);
}
