/**
 *
 * @file qwrapper_dsygst.c
 *
 *  PLASMA core_blas quark wrapper
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.8.0
 * @author Hatem Ltaief
 * @date 2010-11-15
 * @generated d Fri Apr  1 11:02:42 2016
 *
 **/
#include <lapacke.h>
//#include "common.h"
#include <runtime/starpu/chameleon_starpu.h>
#include "control/common.h"
#undef COMPLEX
#define REAL

/***************************************************************************//**
 *
 **/

DCODELETS_HEADER(sygst)

void EIG_CORE_dsygst_starpu(const RUNTIME_option_t *options,
		int itype, cham_uplo_t uplo, int n,
		CHAM_desc_t *A, int Am, int An, int lda,
		CHAM_desc_t *B, int Bm, int Bn, int ldb,
		RUNTIME_sequence_t *sequence, RUNTIME_request_t *request,
		int iinfo)
{
	struct starpu_codelet *codelet = &cl_dsygst;

	starpu_insert_task(
			starpu_mpi_codelet(codelet),
			STARPU_VALUE,    &itype,            sizeof(int),
			STARPU_VALUE,    &uplo,             sizeof(int),
			STARPU_VALUE,    &n,                sizeof(int),
			STARPU_RW, RTBLKADDR(A, double, Am, An),
			STARPU_VALUE,    &lda,              sizeof(int),
#ifdef COMPLEX
			STARPU_RW,   RTBLKADDR(B, double, Bm, Bn),
#else
			STARPU_R,   RTBLKADDR(B, double, Bm, Bn),
#endif
			STARPU_VALUE,    &ldb,               sizeof(int),
			STARPU_VALUE,    &(options->sequence),       sizeof(RUNTIME_sequence_t*),
			STARPU_VALUE,    &(options->request),        sizeof(RUNTIME_request_t*),
			STARPU_VALUE,    &iinfo,                sizeof(int),
			0); 
//	printf("\n EIG_CORE_dsygst_starpu\n");
}
/***************************************************************************//**
 *
 **/
#if defined(PLASMA_HAVE_WEAK)
#pragma weak EIG_CORE_dsygst_starpu = PEIG_CORE_dsygst_starpu
#define EIG_CORE_dsygst_starpu PEIG_CORE_dsygst_starpu
#endif
static void cl_dsygst_cpu_func(void *descr[], void *cl_arg)
{
	int itype;
	int uplo;
	int n;
	double *A;
	int lda;
	double *B;
	int ldb;
	RUNTIME_sequence_t *sequence;
	RUNTIME_request_t *request;
	int iinfo;

	int info;
//	printf("\n cl_dsygst_cpu_func\n");
	A = (double *)STARPU_MATRIX_GET_PTR(descr[0]);
	B = (double *)STARPU_MATRIX_GET_PTR(descr[1]);

	starpu_codelet_unpack_args(cl_arg, &itype, &uplo, &n, &lda, &ldb, &sequence, &request, &iinfo);
	info = LAPACKE_dsygst_work(
			LAPACK_COL_MAJOR,
			itype,
			chameleon_lapack_const(uplo),
			n, A, lda, B, ldb);
	if (sequence->status == CHAMELEON_SUCCESS && info != 0)
		RUNTIME_sequence_flush(NULL, sequence, request, iinfo+info);
}

/*
 * Codelet definition
 */
CODELETS_CPU(dsygst, 2, cl_dsygst_cpu_func)
