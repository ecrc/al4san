/**
 *
 * @file runtime_profiling.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU profiling routines
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#include <math.h>
#include "altanal_runtime.h"
#if defined(HAVE_STARPU_FXT_PROFILING)
#include <starpu_fxt.h>
#endif

#ifdef ALTANAL_ENABLE_PRUNING_STATS
unsigned long ALTANAL_Runtime_total_tasks   = 0;
unsigned long ALTANAL_Runtime_exec_tasks    = 0;
unsigned long ALTANAL_Runtime_comm_tasks    = 0;
unsigned long ALTANAL_Runtime_changed_tasks = 0;
#endif

double ALTANAL_Runtime_get_time(){
    return starpu_timing_now()*1e-6;
}

/**
 *  Set iteration numbers for traces
 */
void ALTANAL_Runtime_iteration_push( ALTANAL_context_t *altanal, unsigned long iteration )
{
    (void)altanal;
#if defined(HAVE_STARPU_ITERATION_PUSH)
    starpu_iteration_push(iteration);
#endif
}

void ALTANAL_Runtime_iteration_pop( ALTANAL_context_t *altanal )
{
    (void)altanal;
#if defined(HAVE_STARPU_ITERATION_PUSH)
    starpu_iteration_pop();
#endif
}

void ALTANAL_Runtime_start_profiling(){
#if defined(HAVE_STARPU_FXT_PROFILING)
	starpu_fxt_start_profiling();
#else
    fprintf(stderr, "Profiling throught FxT has not been enabled in StarPU runtime (configure StarPU with --with-fxt)\n");
#endif
}

void ALTANAL_Runtime_stop_profiling(){
#if defined(HAVE_STARPU_FXT_PROFILING)
	starpu_fxt_stop_profiling();
#else
    fprintf(stderr, "Profiling throught FxT has not been enabled in StarPU runtime (configure StarPU with --with-fxt)\n");
#endif
}

void ALTANAL_Runtime_start_stats(){
#ifdef ALTANAL_ENABLE_PRUNING_STATS
    ALTANAL_Runtime_total_tasks = 0;
    ALTANAL_Runtime_exec_tasks = 0;
    ALTANAL_Runtime_comm_tasks = 0;
    ALTANAL_Runtime_changed_tasks = 0;
#endif
}

void ALTANAL_Runtime_stop_stats(){
#ifdef ALTANAL_ENABLE_PRUNING_STATS
    fprintf( stderr, "\ntasks: %lu = exec: %lu + comm: %lu + changed: %lu\n",
             ALTANAL_Runtime_total_tasks, ALTANAL_Runtime_exec_tasks, ALTANAL_Runtime_comm_tasks, ALTANAL_Runtime_changed_tasks );
#endif
}

void ALTANAL_Runtime_profiling_display_info(const char *kernel_name, measure_t perf[STARPU_NMAXWORKERS])
{
    int header = 1;
    unsigned worker;
    for (worker = 0; worker < starpu_worker_get_count(); worker++)
    {
        if (perf[worker].n > 0)
        {
            if ( header ) {
                fprintf(stderr, "Performance for kernel %s\n", kernel_name);
                header = 0;
            }
            char workername[128];
            starpu_worker_get_name(worker, workername, 128);

            long   n    = perf[worker].n;
            double sum  = perf[worker].sum;
            double sum2 = perf[worker].sum2;

            double avg = sum / n;
            double sd  = sqrt((sum2 - (sum*sum)/n)/n);

            fprintf(stderr, "\t%s\t%.2lf\t%.2lf\t%ld\n", workername, avg, sd, n);
        }
    }
}

void ALTANAL_Runtime_profiling_display_efficiency(void)
{
    fprintf(stderr, "Efficiency\n");

    double max_total_time = 0.0;
    unsigned worker;

    for (worker = 0; worker < starpu_worker_get_count(); worker++)
    {
        char workername[128];
        starpu_worker_get_name(worker, workername, 128);

        struct starpu_profiling_worker_info info;
        starpu_profiling_worker_get_info(worker, &info);

        double executing_time = starpu_timing_timespec_to_us(&info.executing_time);
        double total_time = starpu_timing_timespec_to_us(&info.total_time);

        max_total_time = (total_time > max_total_time)?total_time:max_total_time;

        float overhead = 100.0 - (100.0*executing_time/total_time);
        fprintf(stderr, "\t%s\ttotal %.2lf s\texec %.2lf s\toverhead %.2lf%%\n",
                workername, total_time*1e-6, executing_time*1e-6, overhead);
    }

    fprintf(stderr, "Total execution time: %.2lf us\n", max_total_time);
}

void ALTANAL_Runtime_schedprofile_display(void)
{
    fprintf(stderr, "\n");
    ALTANAL_Runtime_profiling_display_efficiency();

    /* Display bus consumption */
    starpu_profiling_bus_helper_display_summary();
}

void ALTANAL_Runtime_kernelprofile_display(void)
{
#if defined(PRECISION_z)
    ALTANAL_Runtime_zdisplay_allprofile();
#endif
#if defined(PRECISION_c)
    ALTANAL_Runtime_cdisplay_allprofile();
#endif
#if defined(PRECISION_d)
    ALTANAL_Runtime_ddisplay_allprofile();
#endif
#if defined(PRECISION_s)
    ALTANAL_Runtime_sdisplay_allprofile();
#endif
}
