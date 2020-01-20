

#include <al4san.h>
#include <runtime/al4san_quark.h>
#include <runtime/al4san_starpu.h>



AL4SAN_TASK_CPU(laset, laset_cpu_func)

void EIG_AL4SAN_CORE_dlaset(AL4SAN_option_t *options,
                       int uplo, int M, int N,
                       double alpha, double beta,
                       const AL4SAN_desc_t *A, int Am, int An, int LDA)
{



    AL4SAN_Insert_Task(AL4SAN_TASK(laset), (AL4SAN_option_t*)options,
        AL4SAN_VALUE,                       &uplo,                                         sizeof(int),
        AL4SAN_VALUE,                       &M,                                            sizeof(int),
        AL4SAN_VALUE,                       &N,                                            sizeof(int),
        AL4SAN_VALUE,                       &alpha,                                        sizeof(double),
        AL4SAN_VALUE,                       &beta,                                         sizeof(double),
        AL4SAN_OUTPUT,                      AL4SAN_ADDR(A, double, Am, An),                AL4SAN_DEP,
        AL4SAN_VALUE,                       &LDA,                                          sizeof(int),
        AL4SAN_PRIORITY,                    options->priority,                             sizeof(int),
        AL4SAN_LABEL,                       "laset",                                      sizeof(char),
        AL4SAN_COLOR,                       "orange",                                      sizeof(char),
        ARG_END);
}



#if !defined(CHAMELEON_SIMULATION)
void laset_cpu_func(AL4SAN_arg_list *al4san_arg)
{
    int uplo;
    int M;
    int N;
    double alpha;
    double beta;
    double *A;
    int LDA;

   AL4SAN_Unpack_Arg(al4san_arg, &uplo, &M, &N, &alpha, &beta, &A, &LDA);

    CORE_dlaset(uplo, M, N, alpha, beta, A, LDA);
}
#endif /* !defined(CHAMELEON_SIMULATION) */
