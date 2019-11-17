#ifndef ITIMING_H
#define ITIMING_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <chameleon.h>
#include <al4san.h>
#include <runtime/al4san_quark.h>
#define _TYPE  double
#define _PREC  double
#define _LAMCH LAPACKE_dlamch_work


#define _NAME  "sygv"
/* See Lawn 41 page 120 */
/* cholesky + 2 trsm's + trd */
#define _FMULS (((2. / 3.) * ((double)N * (double)N * (double)N)) + (N * (1.0 / 6.0 * N + 0.5) * N) + 2 * (N * N * (double)((N + 1) / 2.0) ))
#define _FADDS (((2. / 3.) * ((double)N * (double)N * (double)N)) + (N * (1.0 / 6.0 * N )      * N) + 2 * (N * N * (double)((N + 1) / 2.0) ))


/*
#define _FMULS ((double)N * (((1. / 6.) * (double)N + 0.5) * (double)N + (1. / 3.)))
#define _FADDS ((double)N * (((1. / 6.) * (double)N      ) * (double)N - (1. / 6.)))
 */
#include "../include/flops.h"

#include<mkl_lapacke.h>
//#include <lapacke.h>
//#include <core_blas.h>
//#include <cblas.h>
#include<mkl.h>
#if defined(EIG_SCHED_STARPU)
#include <starpu.h>
#endif /* defined(EIG_SCHED_STARPU) */
#if defined(EIG_SCHED_QUARK)
#include <quark.h>
#endif /* defined(EIG_SCHED_QUARK) */
#if defined(EIG_SCHED_PARSEC)
#include <parsec.h>
#endif /* defined(EIG_SCHED_PARSEC) */

//#include <plasma.h>
#include <quark.h>
//#include "plasmatypes.h"
//#include "descriptor.h"

typedef double real_Double_t;


#define EIG_INPLACE    1
#define EIG_OUTOFPLACE 2 

#if defined( _WIN32 ) || defined( _WIN64 )
#include <windows.h>
#include <time.h>
#include <sys/timeb.h>
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

double cWtime(void);

struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
	FILETIME         ft;
	unsigned __int64 tmpres = 0;
	static int       tzflag;

	if (NULL != tv)
	{
		GetSystemTimeAsFileTime(&ft);
		tmpres |=  ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |=  ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		tmpres /= 10;  /*convert into microseconds*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;

		tv->tv_sec  = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
	}
	if (NULL != tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime     = _daylight;
	}
	return 0;
}

#else  /* Non-Windows */
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif


enum iparam_t {
	IPARAM_THRDNBR,        /* Number of cores                            */
	IPARAM_THRDNBR_SUBGRP, /* Number of cores in a subgroup (NUMA node)  */
	IPARAM_NCUDAS,
	IPARAM_NMPI,
	IPARAM_P,
	IPARAM_Q,
	IPARAM_SCHEDULER,      /* What scheduler do we choose (dyn, stat)    */
	IPARAM_M,              /* Number of rows of the matrix               */
	IPARAM_N,              /* Number of columns of the matrix            */
	IPARAM_K,              /* RHS or K                                   */
	IPARAM_LDA,            /* Leading dimension of A                     */
	IPARAM_LDB,            /* Leading dimension of B                     */
	IPARAM_LDC,            /* Leading dimension of C                     */
	IPARAM_IB,             /* Inner-blocking size                        */
	IPARAM_NB,             /* Number of columns in a tile                */
	IPARAM_MB,             /* Number of rows in a tile                   */
	IPARAM_NITER,          /* Number of iteration of each test           */
	IPARAM_WARMUP,         /* Run one test to load dynamic libraries     */
	IPARAM_CHECK,          /* Checking activated or not                  */
	IPARAM_VERBOSE,        /* How much noise do we want?                 */
	IPARAM_AUTOTUNING,     /* Disable/enable autotuning                  */
	IPARAM_INPUTFMT,       /* Input format (Use only for getmi/gecfi)    */
	IPARAM_OUTPUTFMT,      /* Output format (Use only for getmi/gecfi)   */
	IPARAM_TRACE,          /* Generate trace on the first non warmup run */
	IPARAM_DAG,            /* Do we require to output the DOT file?      */
	IPARAM_ASYNC,          /* Asynchronous calls                         */
	IPARAM_MX,             /* */
	IPARAM_NX,             /* */
	IPARAM_RHBLK,          /* Householder reduction parameter for QR/LQ  */
	IPARAM_TNTPIV_MODE,    /* Tournament pivoting LU mode (LU or QR)     */
	IPARAM_TNTPIV_SIZE,    /* Tournament pivoting LU group size          */
	IPARAM_INPLACE,        /* InPlace/OutOfPlace translation mode        */
	IPARAM_MODE,           /* Eigenvalue generation mode                 */
	IPARAM_LAPACK,         /* Eigenvalue dstedc using or not LAPACK      */
	IPARAM_MATRIX,         /* FD matrix entry used for tridiagonal solvers */
	IPARAM_MATRIX_CHECK,   /* FD for checking results                    */
	IPARAM_SIZEOF,
	IPARAM_GEMM3M
};

enum dparam_timing {
	IPARAM_TIME,
	IPARAM_ANORM,
	IPARAM_BNORM,
	IPARAM_XNORM,
	IPARAM_RES,
	IPARAM_DNBPARAM
};
/*
   int min( int a, int b ){
   if (a<b) return(a); else return(b);
   }

   int max( int a, int b ){
   if (a>b) return(a); else return(b);
   }*/

#define PASTE_CODE_IPARAM_LOCALS(iparam)           \
	double  t;                                     \
double t1, t2;                                 \
int64_t M     = iparam[IPARAM_M];              \
int64_t N     = iparam[IPARAM_N];              \
int64_t K     = iparam[IPARAM_K];              \
int64_t NRHS  = K;                             \
int64_t LDA   = N;    \
int64_t LDB   = max(N, iparam[IPARAM_LDB]);    \
int64_t LDC   = max(K, iparam[IPARAM_LDC]);    \
int64_t IB    = iparam[IPARAM_IB];             \
int64_t MB    = iparam[IPARAM_MB];             \
int64_t NB    = iparam[IPARAM_NB];             \
int64_t MT    = (M%MB==0) ? (M/MB) : (M/MB+1); \
int64_t NT    = (N%NB==0) ? (N/NB) : (N/NB+1); \
int64_t P     = iparam[IPARAM_P];              \
int64_t Q     = iparam[IPARAM_Q];              \
int check     = iparam[IPARAM_CHECK];          \
int loud      = iparam[IPARAM_VERBOSE];        \
(void)M;(void)N;(void)K;(void)NRHS;            \
(void)LDA;(void)LDB;(void)LDC;                 \
(void)IB;(void)MB;(void)NB;(void)MT;(void)NT;  \
(void)check;(void)loud;


#define PASTE_CODE_IPARAM_LOCALS2(iparam)           \
M     = iparam[IPARAM_M];              \
N     = iparam[IPARAM_N];              \
K     = iparam[IPARAM_K];              \
NRHS  = K;                             \
LDA   = N;    \
LDB   = max(N, iparam[IPARAM_LDB]);    \
LDC   = max(K, iparam[IPARAM_LDC]);    \
IB    = iparam[IPARAM_IB];             \
MB    = iparam[IPARAM_MB];             \
NB    = iparam[IPARAM_NB];             \
MT    = (M%MB==0) ? (M/MB) : (M/MB+1); \
NT    = (N%NB==0) ? (N/NB) : (N/NB+1); \
P     = iparam[IPARAM_P];              \
Q     = iparam[IPARAM_Q];              \
check     = iparam[IPARAM_CHECK];          \
loud      = iparam[IPARAM_VERBOSE];        \
(void)M;(void)N;(void)K;(void)NRHS;            \
(void)LDA;(void)LDB;(void)LDC;                 \
(void)IB;(void)MB;(void)NB;(void)MT;(void)NT;  \
(void)check;(void)loud;


/* Paste code to allocate a matrix in desc if cond_init is true */
/*#define PASTE_CODE_ALLOCATE_MATRIX_TILE(_desc_, _cond_, _type_, _type2_, _lda_, _m_, _n_) \
	PLASMA_desc *_desc_ = NULL;                                         \
if( _cond_ ) {                                                      \
	_type_ *ptr = NULL;                                             \
	ptr = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_) );      \
	if ( ! ptr ) {                                                  \
		fprintf(stderr, "Our of Memory for %s\n", #_desc_);         \
		return -1;                                                  \
	}                                                               \
	PLASMA_Desc_Create(&(_desc_), ptr, _type2_, MB, NB, MB*NB, _lda_, _n_, 0, 0, _m_, _n_); \
}

#define PASTE_CODE_FREE_MATRIX(_desc_)                                  \
	if ( _desc_ != NULL ) {                                             \
		free(_desc_->mat);                                              \
	}                                                                   \
PLASMA_Desc_Destroy( &_desc_ );

#define PASTE_TILE_TO_LAPACK(_desc_, _name_, _cond_, _type_, _lda_, _n_) \
	_type_ *_name_ = NULL;                                              \
if ( _cond_ ) {                                                     \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_));    \
	if ( ! _name_ ) {                                               \
		fprintf(stderr, "Our of Memory for %s\n", #_name_);         \
		return -1;                                                  \
	}                                                               \
	PLASMA_Tile_to_Lapack(_desc_, (void*)_name_, _lda_);            \
}

#define PASTE_CODE_ALLOCATE_MATRIX(_name_, _cond_, _type_, _lda_, _n_)  \
	_type_ *_name_ = NULL;                                              \
if( _cond_ ) {                                                      \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_) );   \
	if ( ! _name_ ) {                                               \
		fprintf(stderr, "Our of Memory for %s\n", #_name_);         \
		return -1;                                                  \
	}                                                               \
}

#define PASTE_CODE_ALLOCATE_COPY(_name_, _cond_, _type_, _orig_, _lda_, _n_) \
	_type_ *_name_ = NULL;                                              \
if( _cond_ ) {                                                      \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_) );   \
	if ( ! _name_ ) {                                               \
		fprintf(stderr, "Our of Memory for %s\n", #_name_);         \
		return -1;                                                  \
	}                                                               \
	memcpy(_name_, _orig_, (_lda_) * (_n_) * sizeof(_type_) );      \
}
*/
/* Paste code to allocate a matrix in desc if cond_init is true */
#define CHAM_PASTE_CODE_ALLOCATE_MATRIX_TILE(_desc_, _cond_, _type_, _type2_, _lda_, _m_, _n_) \
	CHAM_desc_t *_desc_ = NULL;                                        \
int status ## _desc_ ;                                              \
if( _cond_ ) {                                                      \
	if (ooc)                                                        \
	status ## _desc_ = CHAMELEON_Desc_Create_OOC(&(_desc_), _type2_, MB, NB, MB*NB, _lda_, _n_, 0, 0, _m_, _n_, \
			P, Q);             \
	else if (!bigmat)                                               \
	status ## _desc_ = CHAMELEON_Desc_Create_User(&(_desc_), NULL, _type2_, MB, NB, MB*NB, _lda_, _n_, 0, 0, _m_, _n_, \
			P, Q, chameleon_getaddr_null, NULL, NULL); \
	else                                                            \
	status ## _desc_ = CHAMELEON_Desc_Create(&(_desc_), NULL, _type2_, MB, NB, MB*NB, _lda_, _n_, 0, 0, _m_, _n_, \
			P, Q);                 \
	if (status ## _desc_ != CHAMELEON_SUCCESS) return (status ## _desc_); \
}

#define CHAM_PASTE_CODE_FREE_MATRIX(_desc_)                                  \
	CHAMELEON_Desc_Destroy( &_desc_ );

#define CHAM_PASTE_TILE_TO_LAPACK(_desc_, _name_, _cond_, _type_, _lda_, _n_) \
	_type_ *_name_ = NULL;                                               \
if ( _cond_ ) {                                                      \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_));     \
	if ( ! _name_ ) {                                                \
		fprintf(stderr, "Out of Memory for %s\n", #_name_);          \
		return -1;                                                   \
	}                                                                \
	CHAMELEON_Tile_to_Lapack(_desc_, (void*)_name_, _lda_);              \
}

#define CHAM_PASTE_CODE_ALLOCATE_MATRIX(_name_, _cond_, _type_, _lda_, _n_)  \
	_type_ *_name_ = NULL;                                              \
if( _cond_ ) {                                                      \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_) );   \
	if ( ! _name_ ) {                                               \
		fprintf(stderr, "Out of Memory for %s\n", #_name_);         \
		return -1;                                                  \
	}                                                               \
}

#define CHAM_PASTE_CODE_ALLOCATE_COPY(_name_, _cond_, _type_, _orig_, _lda_, _n_) \
	_type_ *_name_ = NULL;                                                   \
if( _cond_ ) {                                                           \
	_name_ = (_type_*)malloc( (_lda_) * (_n_) * sizeof(_type_) );        \
	if ( ! _name_ ) {                                                    \
		fprintf(stderr, "Out of Memory for %s\n", #_name_);              \
		return -1;                                                       \
	}                                                                    \
	memcpy(_name_, _orig_, (_lda_) * (_n_) * sizeof(_type_) );           \
}

/*********************
 *
 * General Macros for timing
 *
 */
#define START_TIMING1()                          \
        t1=0;                                    \
	t1 = -cWtime();

#define STOP_TIMING1()                          \
        t1 += cWtime();

#define START_TIMING2()                          \
        t2=0;                                    \
        t2 = -cWtime();

#define STOP_TIMING2()                           \
	t2 += cWtime();                                \
*t_ = t1+t2;
#endif /* ITIMING_H */
