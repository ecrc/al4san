
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>

AL4SAN_TASK_CPU_GPU(ormqr, ormqr_cpu_func, ormqr_cuda_func)

void EIG_AL4SAN_CORE_dormqr(AL4SAN_option_t *options,
                        int side, int trans,
                        int m, int n, int k, int ib, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *T, int Tm, int Tn, int ldt,
                        const AL4SAN_desc_t *C, int Cm, int Cn, int ldc )
{



   AL4SAN_Insert_Task(AL4SAN_TASK(ormqr), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                                     &side,                                       sizeof(int),
        AL4SAN_VALUE,                                     &trans,                                      sizeof(int),
        AL4SAN_VALUE,                                     &m,                                          sizeof(int),
        AL4SAN_VALUE,                                     &n,                                          sizeof(int),
        AL4SAN_VALUE,                                     &k,                                          sizeof(int),
        AL4SAN_VALUE,                                     &ib,                                         sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(A, double, Am, An),                AL4SAN_DEP,
        AL4SAN_VALUE,                                     &lda,                                        sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(T, double, Tm, Tn),                AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldt,                                        sizeof(int),
        AL4SAN_INOUT,                                     AL4SAN_ADDR(C, double, Cm, Cn),                AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldc,                                        sizeof(int),
        // ib * nb
        AL4SAN_VALUE,                                     &nb,                                         sizeof(int),
        AL4SAN_SCRATCH,                                   options->ws_worker,                          sizeof(double)*options->ws_wsize,
        AL4SAN_CUDA_FLG,                                  ON,                                          sizeof(int),
        AL4SAN_PRIORITY,                                  options->priority,                               sizeof(int),
        AL4SAN_LABEL,                                     "dormqr",                                    sizeof(char),
        ARG_END);

}


void ormqr_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int trans;
    int m;
    int n;
    int k;
    int ib;
    const double *A;
    int lda;
    const double *T;
    int ldt;
    double *C;
    int ldc;
    double *WORK;
    int ldwork;

    AL4SAN_Unpack_Arg(al4san_arg,  &side, &trans, &m, &n, &k, &ib,
                         &A, &lda, &T, &ldt, &C, &ldc, &ldwork, &WORK);

    CORE_dormqr(side, trans, m, n, k, ib,
                A, lda, T, ldt, C, ldc, WORK, ldwork);
}

#if defined(AL4SAN_USE_CUDA)
void ormqr_cuda_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int trans;
    int m;
    int n;
    int k;
    int ib;
    const double *A, *T;
    double *C, *WORK;
    int lda, ldt, ldc, ldwork;

    AL4SAN_Unpack_Arg(al4san_arg,  &side, &trans, &m, &n, &k, &ib,
                         &A, &lda, &T, &ldt, &C, &ldc, &ldwork, &WORK);
    AL4SAN_getStream(stream);
    CUDA_dormqrt(
            side, trans, m, n, k, ib,
            A, lda, T, ldt, C, ldc, WORK, ldwork, stream );

#ifndef AL4SAN_CUDA_ASYNC1
    cudaStreamSynchronize( stream );
#endif
}
#endif /* defined(AL4SAN_USE_CUDA) */
