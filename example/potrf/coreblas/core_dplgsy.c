/**
 *
 * @file core_dplgsy.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2019 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon core_dplgsy CPU kernel
 *
 * @version 0.9.2
 * @comment This file has been automatically generated
 *          from Plasma 2.5.0 for CHAMELEON 0.9.2
 * @author Piotr Luszczek
 * @author Pierre Lemarinier
 * @author Mathieu Faverge
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2014-11-16
 * @generated from /home/omairyrm/al4san-dev/modified-chameleon/chameleon/coreblas/compute/core_zplgsy.c, normal z -> d, Sat Jun 27 18:00:10 2020
 *
 */
#include "../potrf.h"

/*
 Rnd64seed is a global variable but it doesn't spoil thread safety. All matrix
 generating threads only read Rnd64seed. It is safe to set Rnd64seed before
 and after any calls to create_tile(). The only problem can be caused if
 Rnd64seed is changed during the matrix generation time.
 */

//static unsigned long long int Rnd64seed = 100;
#define Rnd64_A 6364136223846793005ULL
#define Rnd64_C 1ULL
#define RndF_Mul 5.4210108624275222e-20f
#define RndD_Mul 5.4210108624275222e-20

#if defined(PRECISION_z) || defined(PRECISION_c)
#define NBELEM   2
#else
#define NBELEM   1
#endif

static unsigned long long int
Rnd64_jump(unsigned long long int n, unsigned long long int seed ) {
  unsigned long long int a_k, c_k, ran;
  int i;

  a_k = Rnd64_A;
  c_k = Rnd64_C;

  ran = seed;
  for (i = 0; n; n >>= 1, i++) {
    if (n & 1)
      ran = a_k * ran + c_k;
    c_k *= (a_k + 1);
    a_k *= a_k;
  }

  return ran;
}


//  CORE_dplgsy - Generate a tile for random symmetric (positive definite if 'bump' is large enough) matrix.

void CORE_dplgsy( double bump, int m, int n, double *A, int lda,
                  int bigM, int m0, int n0, unsigned long long int seed )
{
    double *tmp = A;
    int64_t i, j;
    unsigned long long int ran, jump;

    jump = (unsigned long long int)m0 + (unsigned long long int)n0 * (unsigned long long int)bigM;

    /*
     * Tile diagonal
     */
    if ( m0 == n0 ) {
        for (j = 0; j < n; j++) {
            ran = Rnd64_jump( NBELEM * jump, seed );

            for (i = j; i < m; i++) {
                *tmp = 0.5f - ran * RndF_Mul;
                ran  = Rnd64_A * ran + Rnd64_C;
#if defined(PRECISION_z) || defined(PRECISION_c)
                *tmp += I*(0.5f - ran * RndF_Mul);
                ran   = Rnd64_A * ran + Rnd64_C;
#endif
                tmp++;
            }
            tmp  += (lda - i + j + 1);
            jump += bigM + 1;
        }

        for (j = 0; j < n; j++) {
            A[j+j*lda] += bump;

            for (i=0; i<j; i++) {
                A[lda*j+i] = A[lda*i+j];
            }
        }
    }
    /*
     * Lower part
     */
    else if ( m0 > n0 ) {
        for (j = 0; j < n; j++) {
            ran = Rnd64_jump( NBELEM * jump, seed );

            for (i = 0; i < m; i++) {
                *tmp = 0.5f - ran * RndF_Mul;
                ran  = Rnd64_A * ran + Rnd64_C;
#if defined(PRECISION_z) || defined(PRECISION_c)
                *tmp += I*(0.5f - ran * RndF_Mul);
                ran   = Rnd64_A * ran + Rnd64_C;
#endif
                tmp++;
            }
            tmp  += (lda - i);
            jump += bigM;
        }
    }
    /*
     * Upper part
     */
    else if ( m0 < n0 ) {
        /* Overwrite jump */
        jump = (unsigned long long int)n0 + (unsigned long long int)m0 * (unsigned long long int)bigM;

        for (i = 0; i < m; i++) {
            ran = Rnd64_jump( NBELEM * jump, seed );

            for (j = 0; j < n; j++) {
                A[j*lda+i] = 0.5f - ran * RndF_Mul;
                ran = Rnd64_A * ran + Rnd64_C;
#if defined(PRECISION_z) || defined(PRECISION_c)
                A[j*lda+i] += I*(0.5f - ran * RndF_Mul);
                ran = Rnd64_A * ran + Rnd64_C;
#endif
            }
            jump += bigM;
        }
    }
}
