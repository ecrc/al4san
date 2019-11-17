
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
//#include <runtime/al4san_parsec.h>

AL4SAN_TASK_CPU(tpqrt, tpqrt_cpu_func)



void
 EIG_AL4SAN_CORE_dtsqrt(AL4SAN_option_t *options,
                    int m, int n, int ib, int nb,
                    const AL4SAN_desc_t *A1, int A1m, int A1n, int lda1,
                    const AL4SAN_desc_t *A2, int A2m, int A2n, int lda2,
                    const AL4SAN_desc_t *T, int Tm, int Tn, int ldt )
{
    return  EIG_AL4SAN_CORE_dtpqrt( options, m, n, 0, ib, nb,
                               A1, A1m, A1n, lda1,
                               A2, A2m, A2n, lda2,
                               T,  Tm,  Tn,  ldt );
}

void EIG_AL4SAN_CORE_dtpqrt(AL4SAN_option_t *options,
                        int m, int n, int l, int ib, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *B, int Bm, int Bn, int ldb,
                        const AL4SAN_desc_t *T, int Tm, int Tn, int ldt )
{

    /*AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_RW(A, Am, An);
    AL4SAN_ACCESS_RW(B, Bm, Bn);
    AL4SAN_ACCESS_W(T, Tm, Tn);
    AL4SAN_END_ACCESS_DECLARATION;*/

    int shapeB = ( l == 0 ) ? 0 : (AL4SAN_REGION_U | AL4SAN_REGION_D);


   AL4SAN_Insert_Task(AL4SAN_TASK(tpqrt),                          (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                                               &m,                                          sizeof(int),
        AL4SAN_VALUE,                                               &n,                                          sizeof(int),
        AL4SAN_VALUE,                                               &l,                                          sizeof(int),
        AL4SAN_VALUE,                                               &ib,                                         sizeof(int),
        AL4SAN_INOUT,          AL4SAN_ADDR(A, double, Am, An), AL4SAN_DEP,
        AL4SAN_VALUE,                                               &lda,                                        sizeof(int),
        AL4SAN_INOUT,  AL4SAN_ADDR(B, double, Bm, Bn), AL4SAN_DEP,
        AL4SAN_VALUE,                                               &ldb,                                        sizeof(int),
        AL4SAN_OUTPUT,                                              AL4SAN_ADDR(T, double, Tm, Tn), AL4SAN_DEP,
        AL4SAN_VALUE,                                               &ldt,                                        sizeof(int),
        //Other options
        AL4SAN_SCRATCH,                                             options->ws_worker,                              sizeof(double)*options->ws_wsize,
        AL4SAN_PRIORITY,                                            options->priority,                               sizeof(int),
        #if defined(CHAMELEON_USE_MPI)
        AL4SAN_EXECUTE_ON_NODE,                                     B->get_rankof(B, Bm, Bn),                    sizeof(int),
        #endif
        AL4SAN_LABEL,                                               "tpqrt",                                    sizeof(char),
        ARG_END);

    (void)ib; (void)nb;
}

#if !defined(CHAMELEON_SIMULATION)
void tpqrt_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int M;
    int N;
    int L;
    int ib;
    double *A;
    int lda;
    double *B;
    int ldb;
    double *T;
    int ldt;
    double *WORK;

    AL4SAN_Unpack_Arg(al4san_arg,  &M, &N, &L, &ib,
                          &A, &lda, &B, &ldb, &T, &ldt, &WORK );
//    CORE_dlaset( Al4sanUpperLower, ib, N, 0., 0., T, ldt );
    CORE_dtpqrt( M, N, L, ib,
                 A, lda, B, ldb, T, ldt, WORK );
}
#endif /* !defined(CHAMELEON_SIMULATION) */
