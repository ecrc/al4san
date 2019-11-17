/**
 *
 * @file al4san/codelet_dlatro.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon dlatro StarPU codelet
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-09-19
 * @generated d Wed Jan 16 11:13:57 2019
 *
 */
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
//#include <runtime/al4san_parsec.h>
/**
 *
 * @ingroup CORE_CHAMELEON_Complex64_t
 *
 */

AL4SAN_TASK_CPU(dtrdalg1, dtrdalg1_cpu_func)

void EIG_AL4SAN_CORE_dtrdalg1(AL4SAN_option_t *options,
                         int n,
                         int nb,
                         double *A,
                         int lda,
                         double *V,
                         double *TAU,
                         int Vblksiz, int wantz,
                         int i, int sweepid, int m, int grsiz,
                         int *PCOL, int *ACOL, int *MCOL)
{

    /*AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_R(A, Am, An);
    AL4SAN_ACCESS_W(B, Bm, Bn);
    AL4SAN_END_ACCESS_DECLARATION;*/

    AL4SAN_Insert_Task(AL4SAN_TASK(dtrdalg1), (AL4SAN_option_t*)options,
	AL4SAN_VALUE,                          &n,                                            sizeof(int),
        AL4SAN_VALUE,                          &nb,                                           sizeof(int),
        AL4SAN_NODEP,                          A,                                              sizeof(double),
        AL4SAN_VALUE,                          &lda,                                          sizeof(int),
       AL4SAN_NODEP,                          V,                                              sizeof(double),
        AL4SAN_NODEP,                          TAU,                                            sizeof(double),
        AL4SAN_VALUE,                          &Vblksiz,                                      sizeof(int),
        AL4SAN_VALUE,                          &wantz,                                        sizeof(int),
        AL4SAN_VALUE,                          &i,                                            sizeof(int),
        AL4SAN_VALUE,                          &sweepid,                                      sizeof(int),
        AL4SAN_VALUE,                          &m,                                            sizeof(int),
        AL4SAN_VALUE,                          &grsiz,                                        sizeof(int),
        AL4SAN_SCRATCH,                        options->ws_worker,                                           sizeof(double)*options->ws_wsize,
        AL4SAN_INPUT,                          PCOL,                                           AL4SAN_DEP,
        AL4SAN_INPUT,                          ACOL,                                           AL4SAN_DEP,
        AL4SAN_OUTPUT | AL4SAN_LOCALITY,    MCOL,                                           AL4SAN_DEP,

        ARG_END);
}

#if !defined(CHAMELEON_SIMULATION)
void dtrdalg1_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int n, nb, lda, Vblksiz, wantz, i, sweepid, m, grsiz;
    double *A, *V, *TAU, *work;
    int *PCOL, *ACOL, *MCOL;
    AL4SAN_Unpack_Arg(al4san_arg,  &n, &nb, &A, &lda, &V, &TAU, &Vblksiz, &wantz, &i, &sweepid, &m, &grsiz, &work, &PCOL, &ACOL, &MCOL);
     CORE_dtrdalg1(n, nb, A, lda, V, TAU, Vblksiz, wantz, i, sweepid, m, grsiz, work);
}
#endif /* !defined(CHAMELEON_SIMULATION) */
