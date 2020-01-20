
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>

AL4SAN_TASK_CPU(tsmqr_hetra1, tsmqr_hetra1_cpu_func)

void EIG_AL4SAN_CORE_dtsmqr_hetra1(AL4SAN_option_t *options,
                              int side, int trans,
                              int m1, int n1, int m2, int n2, int k, int ib, int nb,
                              const AL4SAN_desc_t *A1, int A1m, int A1n, int lda1,
                              const AL4SAN_desc_t *A2, int A2m, int A2n, int lda2,
                              const AL4SAN_desc_t *V,  int Vm,  int Vn,  int ldv,
                              const AL4SAN_desc_t *T,  int Tm,  int Tn,  int ldt)
{

    int ldwork = side == Al4sanLeft ? ib : nb;



    AL4SAN_Insert_Task(AL4SAN_TASK(tsmqr_hetra1),       (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                                     &side,                                          sizeof(int),
        AL4SAN_VALUE,                                     &trans,                                         sizeof(int),
        AL4SAN_VALUE,                                     &m1,                                            sizeof(int),
        AL4SAN_VALUE,                                     &n1,                                            sizeof(int),
        AL4SAN_VALUE,                                     &m2,                                            sizeof(int),
        AL4SAN_VALUE,                                     &n2,                                            sizeof(int),
        AL4SAN_VALUE,                                     &k,                                             sizeof(int),
        AL4SAN_VALUE,                                     &ib,                                            sizeof(int),
        AL4SAN_INOUT,                                     AL4SAN_ADDR(A1, double, A1m, A1n), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &lda1,                                          sizeof(int),
        AL4SAN_INOUT,                                     AL4SAN_ADDR(A2, double, A2m, A2n), AL4SAN_DEP,
        AL4SAN_VALUE,                                     &lda2,                                          sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(V, double, Vm, Vn),    AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldv,                                           sizeof(int),
        AL4SAN_INPUT,                                     AL4SAN_ADDR(T, double, Tm, Tn),    AL4SAN_DEP,
        AL4SAN_VALUE,                                     &ldt,                                           sizeof(int),
        AL4SAN_SCRATCH,                                   options->ws_worker,                                 sizeof(double)*options->ws_wsize,
        AL4SAN_VALUE,                                     &ldwork,                                        sizeof(int),
        AL4SAN_PRIORITY,                                  options->priority,                                  sizeof(int),
        AL4SAN_LABEL,                                     "tsmqr_hetra1",                                sizeof(char),
        ARG_END);
}

void tsmqr_hetra1_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int side;
    int trans;
    int m1;
    int n1;
    int m2;
    int n2;
    int k;
    int ib;
    double *A1;
    int lda1;
    double *A2;
    int lda2;
    double *V;
    int ldv;
    double *T;
    int ldt;
    /* TODO: manage workspace */
    double *WORK;
    int ldwork;

    AL4SAN_Unpack_Arg(al4san_arg, &side, &trans, &m1, &n1, &m2, &n2, &k,
                                &ib, &A1, &lda1, &A2, &lda2, &V, &ldv, &T, &ldt, &WORK, &ldwork);

    CORE_dtsmqr_hetra1(side, trans, m1, n1, m2, n2, k,
                       ib, A1, lda1, A2, lda2, V, ldv, T, ldt, WORK, ldwork);
}
