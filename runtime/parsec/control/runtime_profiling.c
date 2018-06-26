/**
 *
 * @file runtime_profiling.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC profiling routines
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
 *
 */
#include "altanal_runtime.h"
#include "altanal/altanal_timer.h"

double ALTANAL_Runtime_get_time(){
    return ALTANAL_timer();
}

void ALTANAL_Runtime_start_profiling()
{
    altanal_warning("ALTANAL_Runtime_start_profiling()", "FxT profiling is not available with PaRSEC\n");
}

void ALTANAL_Runtime_stop_profiling()
{
    altanal_warning("ALTANAL_Runtime_stop_profiling()", "FxT profiling is not available with PaRSEC\n");
}

void ALTANAL_Runtime_start_stats()
{
    altanal_warning("ALTANAL_Runtime_start_stats()", "pruning stats are not available with PaRSEC\n");
}

void ALTANAL_Runtime_stop_stats()
{
    altanal_warning("ALTANAL_Runtime_stop_stats()", "pruning stats are not available with PaRSEC\n");
}

void ALTANAL_Runtime_schedprofile_display(void)
{
    altanal_warning("ALTANAL_Runtime_schedprofile_display(parsec)", "Scheduler profiling is not available with PaRSEC\n");
}

void ALTANAL_Runtime_kernelprofile_display(void)
{
    altanal_warning("ALTANAL_Runtime_kernelprofile_display(parsec)", "Kernel profiling is not available with PaRSEC\n");
}

/**
 *  Set iteration numbers for traces
 */
void ALTANAL_Runtime_iteration_push( ALTANAL_context_t *altanal, unsigned long iteration )
{
    (void)altanal; (void)iteration;
    return;
}
void ALTANAL_Runtime_iteration_pop( ALTANAL_context_t *altanal )
{
    (void)altanal;
    return;
}

