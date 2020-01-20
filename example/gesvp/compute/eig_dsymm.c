/**
 *
 * @file eig_dsymm.c
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
#define BLKLDD(A, k) A->get_blkldd( A, k )
#define A(m,n) A,  m,  n
#define B(m,n) B,  m,  n
#define C(m,n) C,  m,  n
/**
 *  Parallel tile symmetric matrix-matrix multiplication - dynamic scheduling
 */
void eig_pdsymm(int side, int uplo,
                          double alpha, AL4SAN_desc_t *A, AL4SAN_desc_t *B,
                          double beta, AL4SAN_desc_t *C,
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
    int ldak, ldam, ldan, ldbk, ldbm, ldcm;
    int tempmm, tempnn, tempkn, tempkm;

    double zbeta;
    double zone = (double)1.0;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdplgsy", "AL4SAN not initialized");
        }

   AL4SAN_Options_Init(&options, sequence, request);

    /*
     *  Al4sanLeft
     */
    if (side == Al4sanLeft) {
        for (k = 0; k < C->mt; k++) {
            tempkm = k == C->mt-1 ? C->m-k*C->mb : C->mb;
            ldak = BLKLDD(A, k);
            ldbk = BLKLDD(B, k);
            zbeta = k == 0 ? beta : zone;

            for (n = 0; n < C->nt; n++) {
                tempnn = n == C->nt-1 ? C->n-n*C->nb : C->nb;

                for (m = 0; m < C->mt; m++) {
                    tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                    ldam = BLKLDD(A, m);
                    ldcm = BLKLDD(C, m);

                    /*
                     *  Al4sanLeft / ChamLower
                     */
                    if (uplo == Al4sanLower) {
                        if (k < m) {
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, ChamNoTrans,
                                tempmm, tempnn, tempkm, A->mb,
                                alpha, A(m, k), ldam,
                                       B(k, n), ldbk,
                                zbeta, C(m, n), ldcm);
                        }
                        else {
                            if (k == m) {
                                EIG_AL4SAN_CORE_dsymm(
                                    &options,
                                    side, uplo,
                                    tempmm, tempnn, A->mb,
                                    alpha, A(k, k), ldak,
                                           B(k, n), ldbk,
                                    zbeta, C(m, n), ldcm);
                            }
                            else {
                                EIG_AL4SAN_CORE_dgemm(
                                    &options,
                                    Al4sanTrans, ChamNoTrans,
                                    tempmm, tempnn, tempkm, A->mb,
                                    alpha, A(k, m), ldak,
                                           B(k, n), ldbk,
                                    zbeta, C(m, n), ldcm);
                            }
                        }
                    }
                    /*
                     *  Al4sanLeft / ChamUpper
                     */
                    else {
                        if (k < m) {
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanTrans, ChamNoTrans,
                                tempmm, tempnn, tempkm, A->mb,
                                alpha, A(k, m), ldak,
                                       B(k, n), ldbk,
                                zbeta, C(m, n), ldcm);
                        }
                        else {
                            if (k == m) {
                                EIG_AL4SAN_CORE_dsymm(
                                    &options,
                                    side, uplo,
                                    tempmm, tempnn, A->mb,
                                    alpha, A(k, k), ldak,
                                           B(k, n), ldbk,
                                    zbeta, C(m, n), ldcm);
                            }
                            else {
                                EIG_AL4SAN_CORE_dgemm(
                                    &options,
                                    Al4sanNoTrans, ChamNoTrans,
                                    tempmm, tempnn, tempkm, A->mb,
                                    alpha, A(m, k), ldam,
                                           B(k, n), ldbk,
                                    zbeta, C(m, n), ldcm);
                            }
                        }
                    }
                }
                AL4SAN_Data_Flush( sequence, B(k, n) );
            }
            if (uplo == Al4sanLower) {
                for (n = 0; n <= k; n++) {
                    AL4SAN_Data_Flush( sequence, A(k, n) );
                }
            }
            else {
                for (m = 0; m <= k; m++) {
                    AL4SAN_Data_Flush( sequence, A(m, k) );
                }
            }
        }
    }
    /*
     *  Al4sanRight
     */
    else {
        for (k = 0; k < C->nt; k++) {
            tempkn = k == C->nt-1 ? C->n-k*C->nb : C->nb;
            ldak = BLKLDD(A, k);
            zbeta = k == 0 ? beta : zone;

            for (m = 0; m < C->mt; m++) {
                tempmm = m == C->mt-1 ? C->m-m*C->mb : C->mb;
                ldbm = BLKLDD(B, m);
                ldcm = BLKLDD(C, m);

                for (n = 0; n < C->nt; n++) {
                    tempnn = n == C->nt-1 ? C->n-n*C->nb : C->nb;
                    ldan = BLKLDD(A, n);

                    /*
                     *  Al4sanRight / ChamLower
                     */
                    if (uplo == Al4sanLower) {
                        if (k < n) {
                           EIG_AL4SAN_CORE_dgemm(
                               &options,
                               Al4sanNoTrans, ChamTrans,
                               tempmm, tempnn, tempkn, A->mb,
                               alpha, B(m, k), ldbm,
                                      A(n, k), ldan,
                               zbeta, C(m, n), ldcm);
                        }
                        else {
                            if (k == n) {
                               EIG_AL4SAN_CORE_dsymm(
                                   &options,
                                   Al4sanRight, uplo,
                                   tempmm, tempnn, A->mb,
                                   alpha, A(k, k), ldak,
                                          B(m, k), ldbm,
                                   zbeta, C(m, n), ldcm);
                            }
                            else {
                                EIG_AL4SAN_CORE_dgemm(
                                    &options,
                                    Al4sanNoTrans, ChamNoTrans,
                                    tempmm, tempnn, tempkn, A->mb,
                                    alpha, B(m, k), ldbm,
                                           A(k, n), ldak,
                                    zbeta, C(m, n), ldcm);
                            }
                        }
                    }
                    /*
                     *  Al4sanRight / ChamUpper
                     */
                    else {
                        if (k < n) {
                            EIG_AL4SAN_CORE_dgemm(
                                &options,
                                Al4sanNoTrans, ChamNoTrans,
                                tempmm, tempnn, tempkn, A->mb,
                                alpha, B(m, k), ldbm,
                                       A(k, n), ldak,
                                zbeta, C(m, n), ldcm);
                        }
                        else {
                            if (k == n) {
                                EIG_AL4SAN_CORE_dsymm(
                                    &options,
                                    Al4sanRight, uplo,
                                    tempmm, tempnn, A->mb,
                                    alpha, A(k, k), ldak,
                                           B(m, k), ldbm,
                                    zbeta, C(m, n), ldcm);
                            }
                            else {
                                EIG_AL4SAN_CORE_dgemm(
                                    &options,
                                    Al4sanNoTrans, ChamTrans,
                                    tempmm, tempnn, tempkn, A->mb,
                                    alpha, B(m, k), ldbm,
                                           A(n, k), ldan,
                                    zbeta, C(m, n), ldcm);
                            }
                        }
                    }
                }
                AL4SAN_Data_Flush( sequence, B(m, k) );
            }
            if (uplo == Al4sanLower) {
                for (n = 0; n <= k; n++) {
                    AL4SAN_Data_Flush( sequence, A(k, n) );
                }
            }
            else {
                for (m = 0; m <= k; m++) {
                    AL4SAN_Data_Flush( sequence, A(m, k) );
                }
            }
        }
    }
      AL4SAN_Options_Finalize(&options);
}
