
#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>
#include <runtime/al4san_parsec.h>

/*
  * Preparing work's function:
  * @param[in] First argument is task name.
  * @param[in] Second argument user function name
*/
AL4SAN_TASK_CPU(gelqt, gelqt_cpu_func)

void EIG_AL4SAN_CORE_dgelqt(AL4SAN_option_t *options,
                       int m, int n, int ib, int nb,
                       const AL4SAN_desc_t *A, int Am, int An, int lda,
                       const AL4SAN_desc_t *T, int Tm, int Tn, int ldt)
{
    (void)nb;

    AL4SAN_BEGIN_ACCESS_DECLARATION;
    AL4SAN_ACCESS_RW(A, Am, An);
    AL4SAN_ACCESS_W(T, Tm, Tn);
    AL4SAN_END_ACCESS_DECLARATION;

          /*
            * Insert Task function:
            *  @param[in] First argument AL4SAN_TASK macro with task name
            *  @param[in] options argument which holds sequence data sturcture
            *  @param[in] Parameter list  of va_list type to represent data and the dependencies
          */

    AL4SAN_Insert_Task(AL4SAN_TASK(gelqt), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                        &m,                                           sizeof(int),
        AL4SAN_VALUE,                        &n,                                           sizeof(int),
        AL4SAN_VALUE,                        &ib,                                          sizeof(int),
        AL4SAN_INOUT,                        AL4SAN_ADDR(A, double, Am, An),  AL4SAN_DEP,
        AL4SAN_VALUE,                        &lda,                                         sizeof(int),
        AL4SAN_INPUT,                        AL4SAN_ADDR(T, double, Tm, Tn),  AL4SAN_DEP,
        AL4SAN_VALUE,                        &ldt,                                         sizeof(int),
        AL4SAN_SCRATCH,                      options->ws_worker,                           sizeof(double)*options->ws_wsize,
        AL4SAN_PRIORITY,                     options->priority,                            sizeof(int),
        AL4SAN_LABEL,                        "gelqt",                                     sizeof(char),
        ARG_END);
}

#if !defined(CHAMELEON_SIMULATION)
void gelqt_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int m;
    int n;
    int ib;
    double *A;
    int lda;
    double *T;
    int ldt;
    double *TAU, *WORK;

   /*
    * AL4SAN_Unpack_Arg:
    *  @param[in] First argument AL4SAN_arg that hold the packed data
    *  @param[in] Parameter list  of va_list type which holds list of arguments
 */
    AL4SAN_Unpack_Arg(al4san_arg, &m, &n, &ib, &A, &lda, &T,&ldt, &TAU);

    WORK = TAU + al4san_max( m, n );
    CORE_dgelqt(m, n, ib, A, lda, T, ldt, TAU, WORK);
}
#endif /* !defined(CHAMELEON_SIMULATION) */
