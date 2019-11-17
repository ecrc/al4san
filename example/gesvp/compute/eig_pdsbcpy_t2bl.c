/**
 *
 * @file pdsbcpy_t2bl.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Azzam Haidar
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:03:00 2016
 *
 **/
#include <al4san.h>
#include <runtime/al4san_quark.h>
//#define A(m,n)   BLKADDR(A, double, m, n)
#define A(m, n)  A, m, n
#define AB(m_,n_)   &(AB[(m_) + LDAB*((n_)*nb) ])
#define AL4SAN_BLKLDD(A, k) A->get_blkldd( A, k )

/***************************************************************************//**
 *  Parallel copy of a band matrix from full NxN tile storage to band storage (NxLDAB).
 *  As this function is internal and the space is the same for either Lower or Upper so
 *  ALWAYS it convert to Lower band and then the bulge chasing will
 *  always work with a Lower band matrix
 **/
void eig_pdsbcpy_t2bl_quark(int uplo, AL4SAN_desc_t *A,
                               double *AB, int LDAB,
                               AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
    AL4SAN_context_t *al4sanctxt;

    int j;
    int ldaj, ldx;
    int nb=A->mb;
    int tempjm, tempjn;
    int minmn = min(A->mt, A->nt);
    AL4SAN_option_t options;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("eig_pdsbcpy_t2bl_quark", "AL4SAN not initialized");
    }
    /*=============================================
     * NOTE :
     * this function transform the Lower/Upper Tile
     * band matrix to LOWER Band storage matrix.
     * For Lower it copy it directly.
     * For Upper it transposed during the copy.
     *=============================================*/
   AL4SAN_Options_Init(&options, sequence, request);
    ldx = LDAB-1;

    /* copy Lower to Lower */
    if ( uplo == Al4sanLower ) {
       for (j = 0; j < minmn; j++){
           tempjm = j == A->mt-1 ? A->m-j*A->mb : A->mb;
           tempjn = j == A->nt-1 ? A->n-j*A->nb : A->nb;
           ldaj = AL4SAN_BLKLDD(A, j);
           EIG_AL4SAN_CORE_dlacpy(
               &options,
               Al4sanLower, tempjm, tempjn, nb,
               A(j, j), ldaj, AB(0, j), ldx);

           if( j<minmn-1 ){
               tempjm = (j+1) == A->mt-1 ? A->m-(j+1)*A->mb : A->mb;
               tempjn = j == A->nt-1 ? A->n-j*A->nb : A->nb;
               ldaj = AL4SAN_BLKLDD(A, j+1);
               EIG_AL4SAN_CORE_dlacpy(
                   &options,
                   Al4sanUpper, tempjm, tempjn, nb,
                   A(j+1, j), ldaj, AB(nb, j), ldx);

           }
       }
    }
    /* transpose Upper when copying it to Lower */
    else if ( uplo == Al4sanUpper ) {
       for (j = 0; j < minmn; j++){
           tempjm = j == A->mt-1 ? A->m-j*A->mb : A->mb;
           tempjn = j == A->nt-1 ? A->n-j*A->nb : A->nb;
           ldaj = AL4SAN_BLKLDD(A, j);
           EIG_AL4SAN_CORE_dlatro(
               &options, 
               Al4sanUpper, Al4sanTrans, tempjm, tempjn, nb,
               A(j, j), ldaj, AB(0, j), ldx);

           if(j<minmn-1){
               tempjm = j == A->mt-1 ? A->m-j*A->mb : A->mb;
               tempjn = (j+1) == A->nt-1 ? A->n-(j+1)*A->nb : A->nb;
               ldaj = AL4SAN_BLKLDD(A, j);
                  EIG_AL4SAN_CORE_dlatro(
                   &options,
                   Al4sanLower, Al4sanTrans, tempjm, tempjn, nb,
                   A(j, j+1), ldaj, AB(nb, j), ldx);
           }
       }
    }
AL4SAN_Options_Finalize(&options);
}
#undef AB
#undef A
