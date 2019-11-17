/**
 *
 * @file first_stage.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon zpotrf wrappers
 *
 * @version 1.0.0
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 1.0.0
 * @author Jakub Kurzak
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2010-11-15
 * @precisions normal z -> s d c
 *
 */

#include <sys/time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <al4san.h>
#include "control/al4san_descriptor.h"
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
#include <al4san.h>
#include "control/common.h"
int Eig_First_Stage_Tile (int itype, int jobz, int uplo,  AL4SAN_desc_t *A, AL4SAN_desc_t *B, 
		double *W, AL4SAN_desc_t *T)  
{
       AL4SAN_context_t *al4sanctxt;
      AL4SAN_sequence_t *sequence = NULL;
      AL4SAN_request_t* request = AL4SAN_REQUEST_INITIALIZER;

	int status;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
      al4san_fatal_error("Eig_First_Stage_Tile", "AL4SAN not initialized");
        }

    sequence = AL4SAN_Sequence_Create();

	Eig_First_Stage_Tile_Async(itype, jobz, uplo, A, B, W, T, sequence, &request);

	AL4SAN_Desc_Flush( A, sequence );
	AL4SAN_Desc_Flush( B, sequence );
	AL4SAN_Desc_Flush( T, sequence );  //TODO

	AL4SAN_Sequence_Wait(sequence);
        status = sequence->status;
        AL4SAN_Sequence_Destroy( sequence );
	return status;
}


int Eig_First_Stage_Tile_Async( int itype, int jobz, int uplo, AL4SAN_desc_t *A, AL4SAN_desc_t *B, 
		double *W, AL4SAN_desc_t *T, AL4SAN_sequence_t *sequence, AL4SAN_request_t *request )
{
	//	printf("\n I am in Eig_first_stage_Tile_Async\n");
        AL4SAN_context_t *al4sanctxt;
	AL4SAN_desc_t D, *Dptr = NULL;

       al4sanctxt = al4san_context_self();
	if (al4sanctxt == NULL) {
		al4san_fatal_error("Eig_first_stage_Tile_Async", "AL4SAN not initialized");
		return AL4SAN_ERR_NOT_INITIALIZED;
	}
	if (sequence == NULL) {
		al4san_fatal_error("Eig_first_stage_Tile_Async", "NULL sequence");
		return AL4SAN_ERR_UNALLOCATED;
	}
	if (request == NULL) {
		al4san_fatal_error("Eig_first_stage_Tile_Async", "NULL request");
		return AL4SAN_ERR_UNALLOCATED;
	}


#if defined(EIG_TIMER)
	al4san_sequence_wait( al4sanctxt, sequence );
	float tt1 = get_time();
#endif

	eig_dpotrf( uplo, B, sequence, request);
	cham_eig_pdsygst(itype, uplo, A, B, sequence, request);

#if defined(EIG_TIMER)
        al4san_sequence_wait( al4sanctxt, sequence );
	float tt2 = get_time() - tt1;
	float ttime = tt2;
	printf("\n  Finish transformation timing= %lf", ttime);
#endif

	int   N  = A->m;
	int    NB= A->mb;

	double* E = malloc( (N - 1) * sizeof(double) );
	if (E == NULL) {
		al4san_error("Eig_First_Stage_Tile_Async", "malloc(E) failed");
		free(E);
		return AL4SAN_ERR_OUT_OF_RESOURCES;
	}
#if defined(EIG_TIMER)        
	al4san_sequence_wait( al4sanctxt, sequence );
	const float t1 = get_time();
#endif

#if defined(AL4SAN_COPY_DIAG)
    {
//        chameleon_ddesc_alloc_diag( &D, A->mb, A->m, A->n, A->p, A->q );
        int diag_m = al4san_min(  A->m,  A->n );

        al4san_desc_init( &D, AL4SAN_MAT_ALLOC_GLOBAL, Al4sanRealDouble,  A->mb, A->mb, (A->mb*A->mb),
                                       diag_m, A->nb, 0, 0, diag_m, A->mb, 1, 1,
                                       al4san_getaddr_diag,
                                       al4san_getblkldd_ccrb,
                                       al4san_getrankof_2d_diag);
        Dptr = &D;
    }
#endif
       eig_pdsytrd_sy2sb(uplo, A, T, Dptr,
                             sequence, request);

#if defined(EIG_TIMER)
        al4san_sequence_wait( al4sanctxt, sequence );
	const float t2 = get_time() - t1;
	float time = t2;
	printf("\n  Finish Band red timing= %lf\n", time);
#endif

    if (Dptr != NULL) {
        al4san_desc_destroy( Dptr );
    }
	return AL4SAN_SUCCESS;

}

