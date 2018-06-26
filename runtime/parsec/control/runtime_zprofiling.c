/**
 *
 * @file runtime_zprofiling.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC ALTANAL_Complex64_t kernel progiling
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
 *
 */
#include "altanal_runtime.h"

void ALTANAL_Runtime_zdisplay_allprofile()
{
    altanal_warning("ALTANAL_Runtime_zdisplay_allprofile(PaRSEC)", "Profiling is not available with PaRSEC");
}

void ALTANAL_Runtime_zdisplay_oneprofile( ALTANAL_kernel_t kernel )
{
    (void)kernel;
    altanal_warning("ALTANAL_Runtime_zdisplay_oneprofile(PaRSEC)", "Profiling is not available with PaRSEC\n");
}

