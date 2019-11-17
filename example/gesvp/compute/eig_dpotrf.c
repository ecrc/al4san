/**
 *
 * @file pdpotrf.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief eig al4san dpotrf parallel algorithm
 *
 * @version 0.9.2
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 0.9.2
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @author Florent Pruvost
 * @date 2014-11-16
 * @generated d Tue Apr  2 10:58:14 2019
 *
 */
#include <al4san.h>

#define A(m,n) A,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  Parallel tile Cholesky factorization - dynamic scheduling
 */
void eig_dpotrf(int uplo, AL4SAN_desc_t *A, AL4SAN_sequence_t *sequence, AL4SAN_sequence_t *request)
{

    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options;

    int k, m, n;
    int ldak, ldam, ldan;
    int tempkm, tempmm, tempnn;
    size_t ws_host   = 0;

    double zone  = (double) 1.0;
    double mzone = (double)-1.0;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("eig_dpotrf", "AL4SAN not initialized");
    }

    AL4SAN_Options_Init(&options, sequence, request);

    AL4SAN_Options_Workspace_Alloc( &options, 0, ws_host );

    /*
     *  ChamLower
     */
    if (uplo == Al4sanLower) {
        for (k = 0; k < A->mt; k++) {

            tempkm = k == A->mt-1 ? A->m-k*A->mb : A->mb;
            ldak = BLKLDD(A, k);

            options.priority = 2*A->mt - 2*k;
            EIG_AL4SAN_CORE_dpotrf(
                &options,
                Al4sanLower, tempkm, A->mb,
                A(k, k), ldak, A->nb*k);

            for (m = k+1; m < A->mt; m++) {
                tempmm = m == A->mt-1 ? A->m-m*A->mb : A->mb;
                ldam = BLKLDD(A, m);

                options.priority = 2*A->mt - 2*k - m;
                EIG_AL4SAN_CORE_dtrsm(
                    &options,
                    Al4sanRight, Al4sanLower, Al4sanTrans, Al4sanNonUnit,
                    tempmm, A->mb, A->mb,
                    zone, A(k, k), ldak,
                          A(m, k), ldam);
            }
            AL4SAN_Data_Flush( sequence, A(k, k) );

            for (n = k+1; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;
                ldan = BLKLDD(A, n);

                options.priority = 2*A->mt - 2*k - n;
                EIG_AL4SAN_CORE_dsyrk(
                    &options,
                    Al4sanLower, Al4sanNoTrans,
                    tempnn, A->nb, A->mb,
                    -1.0, A(n, k), ldan,
                     1.0, A(n, n), ldan);

                for (m = n+1; m < A->mt; m++) {
                    tempmm = m == A->mt-1 ? A->m - m*A->mb : A->mb;
                    ldam = BLKLDD(A, m);

                    options.priority = 2*A->mt - 2*k - n - m;
                    EIG_AL4SAN_CORE_dgemm(
                        &options,
                        Al4sanNoTrans, Al4sanTrans,
                        tempmm, tempnn, A->mb, A->mb,
                        mzone, A(m, k), ldam,
                               A(n, k), ldan,
                        zone,  A(m, n), ldam);
                }
                AL4SAN_Data_Flush( sequence, A(n, k) );
            }
        }
    }
    /*
     *  ChamUpper
     */
    else {
        for (k = 0; k < A->nt; k++) {

            tempkm = k == A->nt-1 ? A->n-k*A->nb : A->nb;
            ldak = BLKLDD(A, k);

            options.priority = 2*A->nt - 2*k;
            EIG_AL4SAN_CORE_dpotrf(
                &options,
                Al4sanUpper,
                tempkm, A->mb,
                A(k, k), ldak, A->nb*k);

            for (n = k+1; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n - n*A->nb : A->nb;

                options.priority = 2*A->nt - 2*k - n;
                EIG_AL4SAN_CORE_dtrsm(
                    &options,
                    Al4sanLeft, Al4sanUpper, Al4sanTrans, Al4sanNonUnit,
                    A->mb, tempnn, A->mb,
                    zone, A(k, k), ldak,
                          A(k, n), ldak);
            }
            AL4SAN_Data_Flush( sequence, A(k, k) );

            for (m = k+1; m < A->mt; m++) {
                tempmm = m == A->mt-1 ? A->m - m*A->mb : A->mb;
                ldam = BLKLDD(A, m);

                options.priority = 2*A->nt - 2*k  - m;
                EIG_AL4SAN_CORE_dsyrk(
                    &options,
                    Al4sanUpper, Al4sanTrans,
                    tempmm, A->mb, A->mb,
                    -1.0, A(k, m), ldak,
                     1.0, A(m, m), ldam);

                for (n = m+1; n < A->nt; n++) {
                    tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;

                    options.priority = 2*A->nt - 2*k - n - m;
                    EIG_AL4SAN_CORE_dgemm(
                        &options,
                        Al4sanTrans, Al4sanNoTrans,
                        tempmm, tempnn, A->mb, A->mb,
                        mzone, A(k, m), ldak,
                               A(k, n), ldak,
                        zone,  A(m, n), ldam);
                }
                AL4SAN_Data_Flush( sequence, A(k, m) );
            }

        }
    }
    AL4SAN_Options_Workspace_Free(&options);
    AL4SAN_Options_Finalize(&options);

    AL4SAN_Desc_Flush( A, sequence );

}
