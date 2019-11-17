#include "./timing.h"
#include <chameleon.h>
#include <runtime/starpu/chameleon_starpu.h>
#include <math.h>
double cWtime(void)
{
	struct timeval tp;
	gettimeofday( &tp, NULL );
	return tp.tv_sec + 1e-6 * tp.tv_usec;
}


int check_solution(int N, double *E1, double *E2, double eps);

#define maxx(a, b) ((a) > (b) ? (a) : (b))
#define minn(a, b) ((a) < (b) ? (a) : (b))

int
RunTest(int *iparam, double *dparam, real_Double_t *t_)
{
	int itype = 1;
	int vec   = Al4sanNoVec;
	double      eps = LAPACKE_dlamch_work('e');
	double *A1, *B1, *Acpy, *Bcpy;
	int uplo=ChamLower;

	mkl_set_threading_layer(MKL_THREADING_INTEL);
	omp_set_nested(1);
	mkl_set_dynamic(0);

	PASTE_CODE_IPARAM_LOCALS( iparam );
	LDA =  N;

//        CHAMELEON_Init( iparam[IPARAM_THRDNBR],
 //                     iparam[IPARAM_NCUDAS] );
         AL4SAN_context_t *al4san = AL4SAN_Init("Starpu", iparam[IPARAM_THRDNBR], iparam[IPARAM_NCUDAS]);
	int nbnode = 1;

	AL4SAN_desc_t *descchamA = NULL, *descchamB = NULL, *descchamQ = NULL, *descchamT = NULL;

	AL4SAN_Desc_Create(&descchamA,  NULL, Al4sanRealDouble,
			NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

	AL4SAN_Desc_Create(&descchamB,  NULL, Al4sanRealDouble,
			NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

	AL4SAN_Desc_Create(&descchamQ,  NULL, Al4sanRealDouble,
			NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

        MT = (M%NB==0) ? (M/NB) : (M/NB+1);
        NT = (N%NB==0) ? (N/NB) : (N/NB+1);

        AL4SAN_Desc_Create(&descchamT,  NULL, Al4sanRealDouble,
                        IB, NB,  IB*NB, MT*IB, NT*NB, 0, 0, MT*IB, NT*NB, 1, 1);


	if(check){
		Acpy = (double *)malloc(LDA*N*sizeof(double));
		Bcpy = (double *)malloc(LDA*N*sizeof(double));
	}


	CHAM_PASTE_CODE_ALLOCATE_MATRIX( W, 1, double, N, 1 );
	CHAM_PASTE_CODE_ALLOCATE_MATRIX( W1, 1, double, N, 1 );

	mkl_set_num_threads(1);

	eig_pdplgsy( (double)0.0, ChamUpperLower,  descchamA, 51 );
	eig_pdplgsy((double)N,   ChamUpperLower,  descchamB, 3753 );

	printf("\n \t N:%d, M:%d, K:%d, LDA:%d, NB:%d MB:%d \t\n", N, M, K, LDA, NB, MB);

	START_TIMING1();
	Eig_First_Stage_Tile (itype, vec,  uplo, descchamA, descchamB, W, descchamT);
	STOP_TIMING1();

	AL4SAN_Desc_Discharge(&descchamA);
	AL4SAN_Desc_Discharge(&descchamB);
	AL4SAN_Desc_Discharge(&descchamQ);
	AL4SAN_Desc_Discharge(&descchamT);
   //     CHAMELEON_Finalize();
        AL4SAN_Switch_Runtime("Quark", 28, 0);


	uplo  = Al4sanLower;

        AL4SAN_desc_t *descA = NULL, *descB=NULL, *descQ=NULL, *descT=NULL;
        AL4SAN_Desc_Create(&descA,  descchamA->mat, Al4sanRealDouble,
                        NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

        AL4SAN_Desc_Create(&descB,  descchamB->mat, Al4sanRealDouble,
                        NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

        AL4SAN_Desc_Create(&descQ,  descchamQ->mat, Al4sanRealDouble,
                        NB, NB,  NB*NB, LDA, N, 0, 0, N, N, 1, 1);

        AL4SAN_Desc_Create(&descT,  descchamT->mat, Al4sanRealDouble,
                        IB, NB,  IB*NB, MT*IB, NT*NB, 0, 0, MT*IB, NT*NB, 1, 1);

	START_TIMING2();
	EIG_Second_Stage_Tile( itype, vec, uplo, descA, descB, W, descT);
	STOP_TIMING2();

        if (vec == Al4sanVec) {
                CHAM_PASTE_CODE_FREE_MATRIX(descQ);
        }
        AL4SAN_Desc_Destroy(&descA);
        AL4SAN_Desc_Destroy(&descB);
        AL4SAN_Desc_Destroy(&descT);
        AL4SAN_Finalize();
	/* 
	 *     Checking
	 *                */

	mkl_set_num_threads(1);
	omp_set_num_threads(1); 
	double *Ainit, *Binit; 

      CHAMELEON_Init( iparam[IPARAM_THRDNBR],
                      iparam[IPARAM_NCUDAS] );
        if(check){
                CHAMELEON_dplgsy( (double)0.0, ChamUpperLower, N, Acpy, LDA, 51 );
                CHAMELEON_dplgsy( (double)N, ChamUpperLower, N, Bcpy, LDA, 3753 );

		A1    = (double *)malloc(LDA*N*sizeof(double));
		B1    = (double *)malloc(LDA*N*sizeof(double));
		memcpy(A1, Acpy, LDA*N*sizeof(double));
		memcpy(B1, Bcpy, LDA*N*sizeof(double));

	}

	if(check)
	{
		char v;
		if (vec=Al4sanVec)
			v='V';
		else
			v='N';
		char u;
		if(uplo==Al4sanUpper)
			u='U';
		else
			u='L';
        mkl_set_num_threads(28);
		int info= LAPACKE_dsygv( LAPACK_COL_MAJOR, 
				itype, v, u,
				N, A1, LDA, B1, LDA, W1);
		printf("\n info:%d\n", info);
        mkl_set_num_threads(1);
		check_solution(N, W1, W, eps);

		free(A1);
		free(B1);
		free(Acpy);
		free(Bcpy);
	}		
	/* Init Chameleon to deallocate matrices
	 */
	free( W );
	free( W1 );

        CHAMELEON_Finalize();
	return 0;
}
/*
   check the solution
 */
int check_solution(int N, double *E1, double *E2, double eps)
{
	int info_solution, i;
	double resid;
	double maxtmp;
	double maxel = fabs( fabs(E1[0]) - fabs(E2[0]) );
	double maxeig = maxx( fabs(E1[0]), fabs(E2[0]) );
	for (i=1; i<N; i++) {
		resid  = fabs(fabs(E1[i])-fabs(E2[i]));
		maxtmp = maxx(fabs(E1[i]), fabs(E2[i]));

		/* Update */
		maxeig = maxx(maxtmp, maxeig);
		maxel  = maxx(resid,  maxel );

	}

	maxel = maxel / (maxeig *N * eps);
	printf("\n ======================================================\n");
	printf(" | D - eigcomputed | / (|D| * N * eps) : %15.3E \n",  maxel);
	printf(" ======================================================\n");

	printf("============\n");
	printf("Checking the eigenvalues of A\n");
	if ( isnan(maxel) || isinf(maxel) || (maxel > 100) ) {
		printf("-- The eigenvalues are suspicious ! \n");
		info_solution = 1;
	}
	else{
		printf("-- The eigenvalues are CORRECT ! \n");
		info_solution = 0;
	}
	return info_solution;
}

