/**
 *
 * @file Second_Stage.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 * @date 2010-11-15
 * @precisions normal z -> s d c
 *
 */
#include<mkl_lapacke.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <al4san.h>
#include <runtime/al4san_quark.h>
static float get_time()
{
        static double gtod_ref_time_sec = 0.0;

        struct timeval tv;
        gettimeofday(&tv, NULL);

        // If this is the first invocation of through dclock(), then initialize the
        // "reference time" global variable to the seconds field of the tv struct.
        if (gtod_ref_time_sec == 0.0)
                gtod_ref_time_sec = (double) tv.tv_sec;

        // Normalize the seconds field of the tv struct so that it is relative to the
        // "reference time" that was recorded during the first invocation of dclock().
        const double norm_sec = (double) tv.tv_sec - gtod_ref_time_sec;

        // Compute the number of seconds since the reference time.
        const double t = norm_sec + tv.tv_usec * 1.0e-6;

        return (float) t;
}
//#include <mkl.h>
/***************************************************************************//**
 *
 * @ingroup double_Tile
 *
 *  PLASMA_dsygv_Tile - Computes all eigenvalues and, optionally,
 *  eigenvectors of a complex generalized Hermitian-definite
 *  eigenproblem of the form: A*x=(lambda)*B*x, A*Bx=(lambda)*x, or
 *  B*A*x=(lambda)*x.
 *  Here A and B are assumed to be Hermitian and B is also
 *  positive definite.
 *
 *  Tile equivalent of PLASMA_dsygv().
 *  Operates on matrices stored by tiles.
 *  All matrices are passed through descriptors.
 *  All dimensions are taken from the descriptors.
 *
 *******************************************************************************
 *
 * @param[in] itype
 *          Intended usage:
 *          = 1: A*x=(lambda)*B*x
 *          = 2: A*Bx=(lambda)*x
 *          = 3: B*A*x=(lambda)*x
 *
 * @param[in] jobz
 *          Intended usage:
 *          = PlasmaNoVec: computes eigenvalues only;
 *          = PlasmaVec: computes eigenvalues and eigenvectors.
 *
 * @param[in] uplo
 *          Specifies whether the matrix A is upper triangular or
 *          lower triangular:
 *          = PlasmaUpper: Upper triangle of A and B are stored;
 *          = PlasmaLower: Lower triangle of A and B are stored.
 *
 * @param[in,out] A
 *          On entry, the symmetric (or Hermitian) matrix A.
 *          If uplo = PlasmaUpper, the leading N-by-N upper triangular
 *          part of A contains the upper triangular part of the matrix
 *          A, and the strictly lower triangular part of A is not
 *          referenced.
 *          If uplo = PlasmaLower, the leading N-by-N lower triangular
 *          part of A contains the lower triangular part of the matrix
 *          A, and the strictly upper triangular part of A is not
 *          referenced.
 *          On exit, if jobz = PlasmaVec, then if return value = 0, A
 *          contains the matrix Z of eigenvectors.
 *          The eigenvectors are normalized as follows:
 *          if ITYPE = 1 or 2, Z**T*B*Z = I;
 *          if ITYPE = 3,      Z**T*inv(B)*Z = I.
 *          If jobz = PlasmaNoVec, then on exit the lower triangle (if
 *          uplo = PlasmaLower) or the upper triangle (if uplo =
 *          PlasmaUpper) of A, including the diagonal, is destroyed.
 *
 * @param[in,out] B
 *          On entry, the symmetric (or Hermitian) positive definite
 *          matrix B.
 *          If uplo = PlasmaUpper, the leading N-by-N upper triangular
 *          part of B contains the upper triangular part of the matrix
 *          B, and the strictly lower triangular part of B is not
 *          referenced.
 *          If uplo = PlasmaLower, the leading N-by-N lower triangular
 *          part of B contains the lower triangular part of the matrix
 *          B, and the strictly upper triangular part of B is not
 *          referenced.
 *          On exit, if return value <= N, the part of B containing
 *          the matrix is overwritten by the triangular factor U or L
 *          from the Cholesky factorization B = U**T*U or B = L*L**T.
 *
 * @param[in,out] T
 *          On entry, descriptor as return by
*          PLASMA_Alloc_Workspace_dsygv
*          On exit, contains auxiliary factorization data.
*
* @param[out] W
*          On exit, if info = 0, the eigenvalues.
*
* @param[out] Q
*          On exit, if jobz = PlasmaVec and info = 0, the eigenvectors.
*
*******************************************************************************
*
* @return
*          \retval PLASMA_SUCCESS successful exit
*          \retval <0 if -i, the i-th argument had an illegal value
*          \retval <=N if INFO = i, plasma_dsygv failed to converge; i
*               off-diagonal elements of an intermediate tridiagonal
*               form did not converge to zero.
*          \retval >N if INFO = N + i, for 1 <= i <= N, then the leading
*                     minor of order i of B is not positive definite.
*                     The factorization of B could not be completed and
*                     no eigenvalues or eigenvectors were computed.
*
*******************************************************************************
*
* @sa PLASMA_dsygv
* @sa PLASMA_dsygv_Tile_Async
* @sa PLASMA_chegv_Tile
* @sa PLASMA_dsygv_Tile
* @sa PLASMA_ssygv_Tile
*
	******************************************************************************/
int EIG_Second_Stage_Tile(int itype, int jobz, int uplo,
		AL4SAN_desc_t *A, AL4SAN_desc_t *B, double *W,
		AL4SAN_desc_t *T)
{
        AL4SAN_context_t *al4sanctxt;
        AL4SAN_sequence_t *sequence = NULL;
        AL4SAN_request_t* request = AL4SAN_REQUEST_INITIALIZER;
	int status;

        al4sanctxt = al4san_context_self();
        if (al4sanctxt == NULL) {
        al4san_fatal_error("EIG_Second_Stage_Tile", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
          }
	sequence = AL4SAN_Sequence_Create();
        EIG_Second_Stage_Tile_Async(itype, jobz, uplo, A, B, W, T, sequence, &request);

	status = sequence->status;
      AL4SAN_Sequence_Wait(sequence);
      AL4SAN_Sequence_Destroy( sequence );  
	return status;
}

/***************************************************************************//**
 *
 * @ingroup double_Tile_Async
 *
 *  PLASMA_dsygv_Tile - Computes all eigenvalues and, optionally,
 *  eigenvectors of a complex generalized Hermitian-definite
 *  eigenproblem of the form: A*x=(lambda)*B*x, A*Bx=(lambda)*x, or
 *  B*A*x=(lambda)*x.
 *  Here A and B are assumed to be Hermitian and B is also
 *  positive definite.
 *
 *  Non-blocking equivalent of PLASMA_dsygv_Tile().
 *  May return before the computation is finished.
 *  Allows for pipelining of operations at runtime.
 *
 *******************************************************************************
 *
 * @param[in] sequence
 *          Identifies the sequence of function calls that this call belongs to
 *          (for completion checks and exception handling purposes).
 *
 * @param[out] request
 *          Identifies this function call (for exception handling purposes).
 *
 *******************************************************************************
 *
 * @sa PLASMA_dsygv
 * @sa PLASMA_dsygv_Tile
 * @sa PLASMA_chegv_Tile_Async
 * @sa PLASMA_dsygv_Tile_Async
 * @sa PLASMA_ssygv_Tile_Async
 *
 ******************************************************************************/
int EIG_Second_Stage_Tile_Async(int itype, int jobz, int uplo,
		AL4SAN_desc_t *descA, AL4SAN_desc_t *descB, double *W,
		AL4SAN_desc_t *T, AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
	int neig, status;
        AL4SAN_desc_t descT;
	int trans;
        AL4SAN_context_t *al4sanctxt;

        double *AB;
        double *E;
        int i, N, NB, LDAB;
        float timeB, timeblg, timeaplQ1, timeconv1, timeconv2, timeall, timelpk;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("EIG_Second_Stage_Tile_Async", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
	if (sequence == NULL) {
           al4san_fatal_error("EIG_Second_Stage_Tile_Async", "NULL sequence");
		return AL4SAN_ERR_NOT_INITIALIZED;
	}
	if (request == NULL) {
	   al4san_fatal_error("EIG_Second_Stage_Tile_Async", "NULL request");
		return AL4SAN_ERR_NOT_INITIALIZED;
	}
	/* Check sequence status */
	if (sequence->status == AL4SAN_SUCCESS)
		request->status = AL4SAN_SUCCESS;
	else
		return al4san_request_fail(sequence, request, AL4SAN_ERR_SEQUENCE_FLUSHED);

	/* Check input arguments */
	if (itype != 1 && itype != 2 && itype != 3) {
		al4san_error("EIG_Second_Stage_Tile_Async", "Illegal value of itype");
		return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
	}
	if (jobz != Al4sanNoVec && jobz != Al4sanVec) {
		al4san_error("EIG_Second_Stage_Tile_Async", "illegal value of jobz");
		return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
	}
	if (uplo != Al4sanLower && uplo != Al4sanUpper) {
		al4san_error("EIG_Second_Stage_Tile_Async", "illegal value of uplo");
		return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
	}
	if (descA->nb != descA->mb) {
		al4san_error("EIG_Second_Stage_Tile_Async", "only square tiles supported");
		return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
	}
	if ( (jobz == Al4sanVec) && (descA->nb != descA->mb) ) {
		al4san_error("EIG_Second_Stage_Tile_Async", "only square tiles supported");
		return al4san_request_fail(sequence, request, AL4SAN_ERR_ILLEGAL_VALUE);
	}

        N    = descA->m;
        NB   = min(descA->mb,descA->m);
        LDAB = 2*NB+1;
        /* Allocate workspace for band storage of the band matrix A and for the off diagonal after tridiagonalisation */
         AB = (double *)malloc(LDAB*N*sizeof(double));
         memset( AB, 0, LDAB * N * sizeof(double) );
            if (AB == NULL) {
              al4san_error("EIG_Second_Stage_Tile_Async", "alloc(AB) failed");
              free( AB);
              return AL4SAN_ERR_OUT_OF_RESOURCES;
              }
        E = (double *)malloc(N*sizeof(double));
        if (E == NULL) {
                al4san_error("EIG_Second_Stage_Tile_Async", "alloc(E) failed");
                free(E);
                return AL4SAN_ERR_OUT_OF_RESOURCES;
        }
#if defined(EIG_TIMER)
        AL4SAN_Sequence_Wait(sequence);
        timeB = get_time();
#endif
          eig_pdsbcpy_t2bl_quark(uplo, descA, AB, LDAB, sequence, request);

        status = sequence->status;
        if (status != AL4SAN_SUCCESS) {
                al4san_error("EIG_Second_Stage_Tile_Async","pdcopy");
                return status;
        }
        /*=======================================
         *  END of calling Reduction to BAND
         *=======================================*/

#if defined(EIG_TIMER)
       AL4SAN_Sequence_Wait(sequence);
        timeB = get_time() - timeB;
        printf("  Finish copying timing= %lf \n", timeB);
#endif

        /*=======================================
         *  calling bulge chasing
         *=======================================*/
        double *TAU2 = NULL;
        double *V2 = NULL;
        double *T2 = NULL;
        int Vblksiz, blkcnt, LDT, LDV;
        int WANTZ   = 0;
        int blguplo = Al4sanLower;

	char v;
	  if (jobz==Al4sanVec)
	     v='V';
         else
            v='N';
        /* int NE      = N; // for later use when a portion of the eigenvectors are requested*/
          WANTZ=0;
        /* Vblksiz correspond to the blocking used when applying V2 to the matrix Q
         * it is similar to IB in LAPACK DORMQR.
         * blkcnt is the number of losange or tile of Vs */
        /* Note that in case PlamaVec requested, the V2 and T2 are stored by the
         * bulgechasing function in a special format:
         * for V2s: it store the V2(LDV,Vblksiz) of each losange in a tile storage meaning
         * that V2_1 is stored then V2_2,..., V2_blkcnt.
         * blkcnt is the number of losange.
         * */
        Vblksiz = min(NB,48);
        LDT     = Vblksiz;
                TAU2   = (double *) malloc( 2*N* sizeof(double));
                V2     = (double *) malloc( 2*N* sizeof(double));
                if ( (TAU2 == NULL) || (V2 == NULL) ) {
                        al4san_error("EIG_Second_Stage_Tile_Async", "alloc() failed");
                        free(TAU2);
                        free(V2);
                        return AL4SAN_ERR_OUT_OF_RESOURCES;
                }
                memset(TAU2, 0, 2*N*sizeof(double));
                memset(V2,   0, 2*N*sizeof(double));

#if defined(EIG_TIMER)
        timeblg = get_time();
#endif
         eig_pdsytrd_hb2st_v1_quark(Al4sanLower, N, NB, Vblksiz, AB, LDAB, V2, TAU2, W,                                    E, WANTZ, sequence, request);
        /*=======================================
         *  END of calling bulge chasing
         *=======================================*/
#if defined(EIG_TIMER)
        AL4SAN_Sequence_Wait(sequence);
        timeblg = get_time() - timeblg;
        printf("  Finish Bulge       timing= %lf \n", timeblg);
#endif


#if defined(EIG_TIMER)
        timelpk = get_time();
#endif
        /*=======================================
         *  calling eigensolver
         *=======================================*/
        //plasma_setlapack_multithreads(plasma->world_size);
        /* call eigensolver using lapack routine for our resulting tridiag [D E] */
//mkl_set_num_threads(28);
        status = LAPACKE_dsteqr( LAPACK_COL_MAJOR, v,
                        N, W, E, NULL, descA->lm );
        if(status != 0){
                al4san_error("LAPACKE_dsteqr","ZSTEQR");
        }
        sequence->status = status;
//mkl_set_num_threads(1);
        //plasma_setlapack_sequential(plasma);
        /*=======================================
         *  END of calling eigensolver
         *=======================================*/

#if defined(EIG_TIMER)
       AL4SAN_Sequence_Wait(sequence);
        timelpk = get_time() - timelpk;
        printf("  Finish Eigensolver timing= %lf  WANTZ %d threads %d\n", timelpk, WANTZ,  al4sanctxt->world_size);
#endif
        free(V2);
        free(TAU2);
        free(E);
        free(AB);
	return AL4SAN_SUCCESS;
}
