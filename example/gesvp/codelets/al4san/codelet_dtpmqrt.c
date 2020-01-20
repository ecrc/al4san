
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>


void
EIG_AL4SAN_CORE_dtsmqr( AL4SAN_option_t *options,
                    int side, int trans,
                    int m1, int n1, int m2, int n2, int k, int ib, int nb,
                    const AL4SAN_desc_t *A1, int A1m, int A1n, int lda1,
                    const AL4SAN_desc_t *A2, int A2m, int A2n, int lda2,
                    const AL4SAN_desc_t *V, int Vm, int Vn, int ldv,
                    const AL4SAN_desc_t *T, int Tm, int Tn, int ldt )
{
    return EIG_AL4SAN_CORE_dtpmqrt( options, side, trans, m2, n2, k, 0, ib, nb,
                                V, Vm, Vn, ldv, T, Tm, Tn, ldt,
                                A1, A1m, A1n, lda1, A2, A2m, A2n, lda2 );
}

AL4SAN_TASK_CPU(tpmqrt, tpmqrt_cpu_func)


void EIG_AL4SAN_CORE_dtpmqrt(AL4SAN_option_t *options,
                         int side, int trans,
                         int m, int n, int k, int l, int ib, int nb,
                         const AL4SAN_desc_t *V, int Vm, int Vn, int ldv,
                         const AL4SAN_desc_t *T, int Tm, int Tn, int ldt,
                         const AL4SAN_desc_t *A, int Am, int An, int lda,
                         const AL4SAN_desc_t *B, int Bm, int Bn, int ldb )
{


        int shapeV = ( l == 0 ) ? 0 : (AL4SAN_REGION_U | AL4SAN_REGION_D);


   AL4SAN_Insert_Task(AL4SAN_TASK(tpmqrt),               (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                                     &side,                                       sizeof(int),
        AL4SAN_VALUE,                                     &trans,                                      sizeof(int),
        AL4SAN_VALUE,                                     &m,                                          sizeof(int),
        AL4SAN_VALUE,                                     &n,                                          sizeof(int),
        AL4SAN_VALUE,                                     &k,                                          sizeof(int),
        AL4SAN_VALUE,                                     &l,                                          sizeof(int),
        AL4SAN_VALUE,                                     &ib,                                         sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(V, double, Vm, Vn), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldv,                                        sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(T, double, Tm, Tn), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldt,                                        sizeof(int),
        AL4SAN_INOUT,                                     AL4SAN_ADDR(A, double, Am, An), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &lda,                                        sizeof(int),
        AL4SAN_INOUT,                                     AL4SAN_ADDR(B, double, Bm, Bn), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldb,                                        sizeof(int),
        //Other options
        AL4SAN_SCRATCH,                                   options->ws_worker,                              sizeof(double)*ib*nb,
        AL4SAN_VALUE,                                     &(options->ws_wsize),                            sizeof(size_t),
        AL4SAN_CUDA_FLG,                                  ON,                                          sizeof(int),
        AL4SAN_PRIORITY,                                  options->priority,                               sizeof(int),
#if defined(CHAMELEON_USE_MPI)
        AL4SAN_EXECUTE_ON_NODE,                           B->get_rankof(B, Bm, Bn),                    sizeof(int),
#endif
        AL4SAN_LABEL,                                     (( l == 0 ) ? "dtsmqr" : "dtpmqrt"),         sizeof(char),
        ARG_END);

    (void)ib; (void)nb;
}

void tpmqrt_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int trans;
    int M;
    int N;
    int K;
    int L;
    int ib;
    const double *V;
    int ldv;
    const double *T;
    int ldt;
    double *A;
    int lda;
    double *B;
    int ldb;
    double *WORK;
    size_t lwork;

   AL4SAN_Unpack_Arg(al4san_arg, &side, &trans, &M, &N, &K, &L, &ib,
                          &V, &ldv, &T, &ldt, &A, &lda, &B, &ldb, &WORK, &lwork );
    CORE_dtpmqrt( side, trans, M, N, K, L, ib,
                  V, ldv, T, ldt, A, lda, B, ldb, WORK );

   (void)lwork;
}

#if defined(AL4SAN_USE_CUDA)
void tpmqrt_cuda_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int trans;
    int M;
    int N;
    int K;
    int L;
    int ib;
    const double *V;
    int ldv;
    const double *T;
    int ldt;
    double *A;
    int lda;
    double *B;
    int ldb;
    double *W;
    size_t lwork;

    AL4SAN_Unpack_Arg(al4san_arg, &side, &trans, &M, &N, &K, &L, &ib,
                          &V, &ldv, &T, &ldt, &A, &lda, &B, &ldb, &W, &lwork );

    AL4SAN_getStream(stream);


    CUDA_dtpmqrt(
         side, trans, M, N, K, L, ib,
         V, ldv, T, ldt, A, lda, B, ldb,
            W, lwork, stream );


#ifndef AL4SAN_CUDA_ASYNC1
    cudaStreamSynchronize( stream );
#endif
}
#endif /* defined(CHAMELEON_USE_CUDA) */
