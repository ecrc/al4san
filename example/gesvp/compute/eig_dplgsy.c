/**
 *
 * @file eig_dplgsy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 * @brief Al4san dplgsy parallel algorithm
 *
 * @comment This file is a copy of pdplgsy.c,
            wich has been automatically generated
 *          from Plasma 2.5.0
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @author Rade Mathis
 * @author Florent Pruvost
 * @date 2014-11-16
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @generated d Tue Apr  2 10:58:14 2019
 *
 */
#include <al4san.h>
#include "control/common.h"

#define A(m,n) A,  m,  n
#define BLKLDD(A, k) A->get_blkldd( A, k )
/**
 *  eig_pdplgsy - Generate a random symmetric (positive definite if 'bump' is large enough) half-matrix by tiles.
 */
void eig_pdplgsy( double bump, int uplo, AL4SAN_desc_t *A,
                    unsigned long long int seed)
{
   /*
     * Define AL4SAN handle for seqeunce to manage groupe of threads.
     * Define AL4SAN handle for options to set glabel task options and set the sequence handle.
     * Define AL4SAN handle for request status.
   */
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
