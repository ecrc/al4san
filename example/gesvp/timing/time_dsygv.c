#include "./timing.h"
#include <chameleon.h>
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

        /* Initialize AL4SAN with main parameters */
         AL4SAN_context_t *al4san = AL4SAN_Init("Starpu", iparam[IPARAM_THRDNBR], iparam[IPARAM_NCUDAS]);
	int nbnode = 1;

	AL4SAN_desc_t *descchamA = NULL, *descchamB = NULL, *descchamQ = NULL, *descchamT = NULL;

    /*
     * Initialize the structure required for AL4SAN data interface
     * AL4SAN_desc_t is a structure wrapping your data allowing AL4SAN to get
     * pointers to tiles. A tile is a data subset of your matrix on which we
     * apply some optimized CPU/GPU kernels.
     * Notice that this routine suppose your matrix is a contiguous vector of
     * data (1D array), as a data you would give to BLAS/LAPACK.
     * Main arguments:
     *     - descA is a pointer to a descriptor, you need to give the address
     *     of this pointer
     *     - if you want to give your allocated matrix give its address,
     *     if not give a NULL pointer, the routine will allocate the memory
     *     and you access the matrix data with descA->mat
     *     - give the data type (Al4sanByte, Al4sanInteger, Al4sanRealFloat,
     *     Al4sanRealDouble, Al4sanComplexFloat, Al4sanComplexDouble)
     *     - number of rows in a block (tile)
     *     - number of columns in a block (tile)
     *     - number of elements in a block (tile)
     * The other parameters are specific, use:
     * AL4SAN_Desc_Create( ... , 0, 0, number of rows, number of columns, 1, 1);
     * Have a look to the documentation for details about these parameters.
     */
	AL4SAN_Matrix_Create(&descchamA,  NULL, Al4sanRealDouble,
			AL4SAN_Col_Major, NB, NB,  NB, N, N, LDA);

        AL4SAN_Matrix_Create(&descchamB,  NULL, Al4sanRealDouble,
                        AL4SAN_Col_Major, NB, NB,  NB, N, N, LDA);

	AL4SAN_Matrix_Create(&descchamQ,  NULL, Al4sanRealDouble,
			AL4SAN_Col_Major, NB, NB,  NB, N,  N, LDA);

        MT = (M%NB==0) ? (M/NB) : (M/NB+1);
        NT = (N%NB==0) ? (N/NB) : (N/NB+1);

        AL4SAN_Matrix_Create(&descchamT,  NULL, Al4sanRealDouble,
                        AL4SAN_Col_Major, IB, NB,  IB, MT*IB, NT*NB, MT*IB);

	CHAM_PASTE_CODE_ALLOCATE_MATRIX( W, 1, double, N, 1 );

	mkl_set_num_threads(1);

	eig_pdplgsy( (double)0.0, ChamUpperLower,  descchamA, 51 );
	eig_pdplgsy((double)N,   ChamUpperLower,  descchamB, 3753 );

	printf("\n \t N:%d, M:%d, K:%d, LDA:%d, NB:%d MB:%d \t\n", N, M, K, LDA, NB, MB);
        // Eigensolver first stage
	START_TIMING1();
	Eig_First_Stage_Tile (itype, vec,  uplo, descchamA, descchamB, W, descchamT);
	STOP_TIMING1();

	AL4SAN_Desc_Discharge(&descchamA);
	AL4SAN_Desc_Discharge(&descchamB);
	AL4SAN_Desc_Discharge(&descchamQ);
	AL4SAN_Desc_Discharge(&descchamT);
        
        AL4SAN_Switch_Runtime("Quark", 28, 0);


	uplo  = Al4sanLower;

        AL4SAN_desc_t *descA = NULL, *descB=NULL, *descQ=NULL, *descT=NULL;

        AL4SAN_Matrix_Create(&descA,  descchamA->mat, Al4sanRealDouble,
                        AL4SAN_Col_Major, NB, NB,  NB, N, N, LDA);

        AL4SAN_Matrix_Create(&descB,  descchamB->mat, Al4sanRealDouble,
                        AL4SAN_Col_Major, NB, NB,  NB, N, N, LDA);

        AL4SAN_Matrix_Create(&descQ,  descchamQ->mat, Al4sanRealDouble,
                        AL4SAN_Col_Major, NB, NB,  NB, N,  N, LDA);

        AL4SAN_Matrix_Create(&descT,  descchamT->mat, Al4sanRealDouble,
                        AL4SAN_Col_Major, IB, NB,  IB, MT*IB, NT*NB, MT*IB);

        // Eigensolver second stage plus computing eigenvalues
	START_TIMING2();
	EIG_Second_Stage_Tile( itype, vec, uplo, descA, descB, W, descT);
	STOP_TIMING2();

        if (vec == Al4sanVec) {
                CHAM_PASTE_CODE_FREE_MATRIX(descQ);
        }
        /* deallocate A B T and associated descriptors , ... */
        AL4SAN_Desc_Destroy(&descA);
        AL4SAN_Desc_Destroy(&descB);
        AL4SAN_Desc_Destroy(&descT);
        AL4SAN_Finalize();
	free( W );

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

