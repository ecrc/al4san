/**
 *
 * @file runtime_profiling.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU profiling and kernel locality header
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 */
#ifndef _PROFILING_H_
#define _PROFILING_H_

#ifdef ALTANAL_ENABLE_PRUNING_STATS
extern unsigned long ALTANAL_Runtime_total_tasks;
extern unsigned long ALTANAL_Runtime_exec_tasks;
extern unsigned long ALTANAL_Runtime_comm_tasks;
extern unsigned long ALTANAL_Runtime_changed_tasks;
#endif

typedef struct measure_s {
    double sum;
    double sum2;
    long   n;
} measure_t;

void ALTANAL_Runtime_profiling_display_info(const char *kernel_name, measure_t perf[STARPU_NMAXWORKERS]);
void ALTANAL_Runtime_profiling_display_efficiency(void);

void ALTANAL_Runtime_profiling_zdisplay_all(void);
void ALTANAL_Runtime_profiling_cdisplay_all(void);
void ALTANAL_Runtime_profiling_ddisplay_all(void);
void ALTANAL_Runtime_profiling_sdisplay_all(void);

void ALTANAL_zload_FakeModel();
void ALTANAL_cload_FakeModel();
void ALTANAL_dload_FakeModel();
void ALTANAL_sload_FakeModel();

void ALTANAL_zrestore_Model();
void ALTANAL_crestore_Model();
void ALTANAL_drestore_Model();
void ALTANAL_srestore_Model();

#endif
