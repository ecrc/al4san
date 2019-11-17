/**
 *
 * @file pdsygst.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:03:00 2016
 *
 **/
#include <al4san.h>
#include "control/al4san_common.h"
#include "control/common.h"
#define A(m,n) A, m, n
#define B(m,n) B, m, n
#define EIGBLKLDD(A, k) A->get_blkldd( A, k )
/***************************************************************************//**
 *  Parallel Transformation to standard eigenvalue problem  - dynamic scheduler
 **/
void cham_eig_pdsygst(int itype, cham_uplo_t uplo,
		AL4SAN_desc_t *A, AL4SAN_desc_t *B,
		AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
     AL4SAN_context_t *al4sanctxt;
     AL4SAN_option_t options;

	int k;
	int ldak, ldbk;
	int tempkn;
	static double done = 1.0;
	static double zone = 1.0;
	static double mzone = -1.0;
	static double zhalf = 0.5;
	static double mzhalf = -0.5;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("eig_pdplgsy", "AL4SAN not initialized");
      return AL4SAN_ERR_NOT_INITIALIZED;
        }

   AL4SAN_Options_Init(&options, sequence, request);

	if (itype == 1) {
		if (uplo == Al4sanLower) {
			/*
			 * Type 1 / Al4sanLower
			 */
			for (k = 0; k < A->nt; k++){
				tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
				ldak = EIGBLKLDD(A, k);
				ldbk = EIGBLKLDD(B, k);

				EIG_AL4SAN_CORE_dsygst(                    
						&options,
						itype, uplo, tempkn,
						A(k, k), ldak,
						B(k, k), ldbk,
						sequence, request, A->nb*k);

				if (k*A->nb+tempkn < A->n) {
			                eig_pdtrsm( Al4sanRight, uplo, Al4sanTrans, Al4sanNonUnit, zone, 
							al4san_desc_submatrix(B, k*B->nb,        k*B->nb, tempkn,            tempkn), 
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb, A->n-k*A->nb-tempkn, tempkn), 
							sequence, request );

					eig_pdsymm(Al4sanRight, uplo, mzhalf,
							al4san_desc_submatrix(A, k*A->nb,        k*A->nb, tempkn,            tempkn),
							al4san_desc_submatrix(B, k*B->nb+tempkn, k*B->nb, B->n-k*B->nb-tempkn, tempkn),
							zone,
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb, A->n-k*A->nb-tempkn, tempkn),
							sequence, request);

					eig_pdsyr2k(
							uplo, Al4sanNoTrans,
							mzone,
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb,        A->n-k*A->nb-tempkn, tempkn),
							al4san_desc_submatrix(B, k*B->nb+tempkn, k*B->nb,        B->n-k*B->nb-tempkn, tempkn),
							done,
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb+tempkn, A->n-k*A->nb-tempkn, A->n-k*A->nb-tempkn),
							sequence, request);

					eig_pdsymm(Al4sanRight, uplo,
							mzhalf,
							al4san_desc_submatrix(A, k*A->nb,        k*A->nb, tempkn,            tempkn),
							al4san_desc_submatrix(B, k*B->nb+tempkn, k*B->nb, B->n-k*B->nb-tempkn, tempkn),
							zone,
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb, A->n-k*A->nb-tempkn, tempkn),
							sequence, request);

				         eig_pdtrsm(Al4sanLeft, uplo, Al4sanNoTrans, Al4sanNonUnit, zone,
							al4san_desc_submatrix(B, k*B->nb+tempkn, k*B->nb+tempkn, B->n-k*B->nb-tempkn, B->n-k*B->nb-tempkn),
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb,        A->n-k*A->nb-tempkn, tempkn),
							sequence, request);
				}
			}
		}
		else {
			/*
			 * Type 1 / Al4sanUpper
			 */
			for (k = 0; k < A->nt; k++){
				tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
				ldak = EIGBLKLDD(A, k);
				ldbk = EIGBLKLDD(B, k);
				EIG_AL4SAN_CORE_dsygst(                    
						&options,
						itype, uplo, tempkn,
						A(k, k), ldak,
						B(k, k), ldbk,
						sequence, request, A->nb*k);

				if (k*A->nb+tempkn < A->n) {
					eig_pdtrsm(Al4sanLeft, uplo,
							Al4sanTrans, Al4sanNonUnit,
							zone,
							al4san_desc_submatrix(B, k*B->nb, k*B->nb,        tempkn, tempkn),
							al4san_desc_submatrix(A, k*A->nb, k*A->nb+tempkn, tempkn, A->n-k*A->nb-tempkn),
							sequence, request);

					eig_pdsymm(Al4sanLeft, uplo, mzhalf,
							al4san_desc_submatrix(A, k*A->nb, k*A->nb,        tempkn, tempkn),
							al4san_desc_submatrix(B, k*B->nb, k*B->nb+tempkn, tempkn, B->n-k*B->nb-tempkn),
							zone,
							al4san_desc_submatrix(A, k*A->nb, k*A->nb+tempkn, tempkn, A->n-k*A->nb-tempkn),
							sequence, request);

					eig_pdsyr2k(uplo, Al4sanTrans,
							mzone,
							al4san_desc_submatrix(A, k*A->nb,        k*A->nb+tempkn, tempkn,            A->n-k*A->nb-tempkn),
							al4san_desc_submatrix(B, k*B->nb,        k*B->nb+tempkn, tempkn,            B->n-k*B->nb-tempkn),
							done,
							al4san_desc_submatrix(A, k*A->nb+tempkn, k*A->nb+tempkn, A->n-k*A->nb-tempkn, A->n-k*A->nb-tempkn),
							sequence, request);

					eig_pdsymm(Al4sanLeft, uplo,
							mzhalf,
							al4san_desc_submatrix(A, k*A->nb, k*A->nb,        tempkn, tempkn),
							al4san_desc_submatrix(B, k*B->nb, k*B->nb+tempkn, tempkn, B->n-k*B->nb-tempkn),
							zone,
							al4san_desc_submatrix(A, k*A->nb, k*A->nb+tempkn, tempkn, A->n-k*A->nb-tempkn),
							sequence, request);

					eig_pdtrsm(Al4sanRight, uplo,
							Al4sanNoTrans, Al4sanNonUnit,
							zone,
							al4san_desc_submatrix(B, k*B->nb+tempkn, k*B->nb+tempkn, A->n-k*A->nb-tempkn, A->n-k*A->nb-tempkn),
							al4san_desc_submatrix(A, k*A->nb,        k*A->nb+tempkn, tempkn,            A->n-k*A->nb-tempkn),
							sequence, request);
				}
			}
		}
	}
	else{
		if (uplo == Al4sanLower) {
			/*
			 * Type 2 or 3  / Al4sanLower
			 */
			for (k = 0; k < A->nt; k++){
				tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
				ldak = EIGBLKLDD(A, k);
				ldbk = EIGBLKLDD(B, k);

				eig_pdtrmm(Al4sanRight, uplo,
						Al4sanNoTrans, Al4sanNonUnit,
						zone,
						al4san_desc_submatrix(B, 0,      0, k*B->nb, k*B->nb),
						al4san_desc_submatrix(A, k*A->nb, 0, tempkn, k*A->nb),
						sequence, request);

				eig_pdsymm(Al4sanLeft, uplo, zhalf,
						al4san_desc_submatrix(A, k*A->nb, k*A->nb, tempkn, tempkn),
						al4san_desc_submatrix(B, k*B->nb, 0,      tempkn, k*B->nb),
						zone,
						al4san_desc_submatrix(A, k*A->nb, 0,      tempkn, k*A->nb),
						sequence, request);

				eig_pdsyr2k(uplo, Al4sanTrans,
						zone,
						al4san_desc_submatrix(A, k*A->nb, 0, tempkn, k*A->nb),
						al4san_desc_submatrix(B, k*B->nb, 0, tempkn, k*B->nb),
						done,
						al4san_desc_submatrix(A, 0, 0, k*A->nb, k*A->nb),
						sequence, request);

				eig_pdsymm(Al4sanLeft, uplo, zhalf,
						al4san_desc_submatrix(A, k*A->nb, k*A->nb, tempkn, tempkn),
						al4san_desc_submatrix(B, k*B->nb, 0,      tempkn, k*B->nb),
						zone,
						al4san_desc_submatrix(A, k*A->nb, 0,      tempkn, k*A->nb),
						sequence, request);

				eig_pdtrmm(Al4sanLeft, uplo,
						Al4sanTrans, Al4sanNonUnit,
						zone,
						al4san_desc_submatrix(B, k*B->nb, k*B->nb, tempkn, tempkn),
						al4san_desc_submatrix(A, k*A->nb, 0,      tempkn, k*A->nb),
						sequence, request);

				EIG_AL4SAN_CORE_dsygst(                    
						&options,
						itype, uplo, tempkn,
						A(k, k), ldak,
						B(k, k), ldbk,
						sequence, request, A->nb*k);    
			}
		}
		else {
			/*
			 * Type 2 or 3  / Al4sanUpper
			 */
			for (k = 0; k < A->nt; k++){
				tempkn = k == A->nt-1 ? A->n-k*A->nb : A->nb;
				ldak = EIGBLKLDD(A, k);
				ldbk = EIGBLKLDD(B, k);

				eig_pdtrmm(Al4sanLeft, uplo,
						Al4sanNoTrans, Al4sanNonUnit,
						zone,
						al4san_desc_submatrix(B, 0, 0,      k*B->nb, k*B->nb),
						al4san_desc_submatrix(A, 0, k*A->nb, k*A->nb, tempkn),
						sequence, request);

				eig_pdsymm(Al4sanRight, uplo, zhalf,
						al4san_desc_submatrix(A, k*A->nb, k*A->nb, tempkn, tempkn),
						al4san_desc_submatrix(B, 0,      k*B->nb, k*B->nb, tempkn),
						zone,
						al4san_desc_submatrix(A, 0,      k*A->nb, k*A->nb, tempkn),
						sequence, request);

				eig_pdsyr2k(uplo, Al4sanNoTrans, zone,
						al4san_desc_submatrix(A, 0, k*A->nb, k*A->nb, tempkn),
						al4san_desc_submatrix(B, 0, k*B->nb, k*B->nb, tempkn),
						done,
						al4san_desc_submatrix(A, 0, 0,      k*A->nb, k*A->nb),
						sequence, request);

				eig_pdsymm(Al4sanRight, uplo, zhalf,
						al4san_desc_submatrix(A, k*A->nb, k*A->nb, tempkn, tempkn),
						al4san_desc_submatrix(B, 0,      k*B->nb, k*B->nb, tempkn),
						zone,
						al4san_desc_submatrix(A, 0,      k*A->nb, k*A->nb, tempkn),
						sequence, request);

				eig_pdtrmm(
						Al4sanRight, uplo,
						Al4sanTrans, Al4sanNonUnit,
						zone,
						al4san_desc_submatrix(B, k*B->nb, k*B->nb, tempkn, tempkn),
						al4san_desc_submatrix(A, 0,      k*A->nb, k*A->nb, tempkn),
						sequence, request);

				EIG_AL4SAN_CORE_dsygst(                    
						&options,
						itype, uplo, tempkn,
						A(k, k), ldak,
						B(k, k), ldbk,
						sequence, request, A->nb*k);
			}
		}
	}
      AL4SAN_Options_Finalize(&options);
}

