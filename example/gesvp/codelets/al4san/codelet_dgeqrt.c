#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
//#include <runtime/al4san_parsec.h>


AL4SAN_TASK_CPU(geqrt, geqrt_cpu_func)

void EIG_AL4SAN_CORE_dgeqrt(AL4SAN_option_t *options,
                        int m, int n, int ib, int nb,
                        const AL4SAN_desc_t *A, int Am, int An, int lda,
                        const AL4SAN_desc_t *T, int Tm, int Tn, int ldt )
{
    (void)nb;


  /*  AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_RW(A, Am, An);
    AL4SAN_ACCESS_W(T, Tm, Tn);
    AL4SAN_END_ACCESS_DECLARATION;*/


   AL4SAN_Insert_Task(AL4SAN_TASK(geqrt), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                      &m,                                          sizeof(int),
        AL4SAN_VALUE,                      &n,                                          sizeof(int),
        AL4SAN_VALUE,                      &ib,                                         sizeof(int),
        AL4SAN_INOUT,                      AL4SAN_ADDR(A, double, Am, An), AL4SAN_DEP,
        AL4SAN_VALUE,                      &lda,                                        sizeof(int),
        AL4SAN_OUTPUT,                     AL4SAN_ADDR(T, double, Tm, Tn), AL4SAN_DEP,
        AL4SAN_VALUE,                      &ldt,                                        sizeof(int),
        // max( nb * (ib+1), ib * (ib+nb) )
        AL4SAN_SCRATCH,                    options->ws_worker,                          sizeof(double)*options->ws_wsize,
        AL4SAN_PRIORITY,                   options->priority,                           sizeof(int),
        AL4SAN_LABEL,                      "geqrt" ,                                   sizeof(char),
        ARG_END);

}


#if !defined(CHAMELEON_SIMULATION)
void geqrt_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int m;
    int n;
    int ib;
    double *A;
    int lda;
    double *T;
    int ldt;
    double *TAU, *WORK;
    double *ptr;

    AL4SAN_Unpack_Arg(al4san_arg, &m, &n, &ib, &A, &lda, &T, &ldt, &TAU); ;

    WORK = TAU + al4san_max( m, n );
//    CORE_dlaset( Al4sanUpperLower, ib, n, 0., 0., T, ldt );
    CORE_dgeqrt(m, n, ib, A, lda, T, ldt, TAU, WORK);
}
#endif /* !defined(CHAMELEON_SIMULATION) */
