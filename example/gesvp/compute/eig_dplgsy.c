/**
 *
 * @file pdplgsy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon dplgsy parallel algorithm
 *
 * @version 0.9.2
 * @comment This file is a copy of pdplgsy.c,
            wich has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 0.9.2
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @author Rade Mathis
 * @author Florent Pruvost
 * @date 2014-11-16
 * @generated d Tue Apr  2 10:58:14 2019
 *
 */
#include <al4san.h>
#include "control/common.h"

#define A(m,n) A,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  chameleon_pdplgsy - Generate a random symmetric (positive definite if 'bump' is large enough) half-matrix by tiles.
 */
void eig_pdplgsy( double bump, int uplo, AL4SAN_desc_t *A,
                    unsigned long long int seed)
{
     AL4SAN_context_t *al4sanctxt;
    AL4SAN_sequence_t *sequence = NULL;
    AL4SAN_request_t* request = AL4SAN_REQUEST_INITIALIZER;

    AL4SAN_option_t options;

    int m, n;
    int ldam;
    int tempmm, tempnn;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdplgsy", "AL4SAN not initialized");
        }

    sequence = AL4SAN_Sequence_Create();

   AL4SAN_Options_Init(&options, sequence, request);

    for (m = 0; m < A->mt; m++) {
        tempmm = m == A->mt-1 ? A->m-m*A->mb : A->mb;
        ldam = BLKLDD(A, m);

        /*
         * ChamLower
         */
        if (uplo == ChamLower) {
            for (n = 0; n <= m; n++) {
                tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;

                options.priority = m + n;
                EIG_AL4SAN_CORE_dplgsy(
                    &options,
                    bump, tempmm, tempnn, A(m, n), ldam,
                    A->m, m*A->mb, n*A->nb, seed );
            }
        }
        /*
         * ChamUpper
         */
        else if (uplo == ChamUpper) {
            for (n = m; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;

                options.priority = m + n;
                EIG_AL4SAN_CORE_dplgsy(
                    &options,
                    bump, tempmm, tempnn, A(m, n), ldam,
                    A->m, m*A->mb, n*A->nb, seed );
            }
        }
        /*
         * ChamUpperLower
         */
        else {
            for (n = 0; n < A->nt; n++) {
                tempnn = n == A->nt-1 ? A->n-n*A->nb : A->nb;

                EIG_AL4SAN_CORE_dplgsy(
                    &options,
                    bump, tempmm, tempnn, A(m, n), ldam,
                    A->m, m*A->mb, n*A->nb, seed );
            }
        }
    }
      AL4SAN_Options_Finalize(&options);
      AL4SAN_Desc_Flush( A, sequence );
      AL4SAN_Sequence_Wait(sequence);
      AL4SAN_Sequence_Destroy( sequence );
}
