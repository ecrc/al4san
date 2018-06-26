/**
 *
 * @file runtime_zlocality.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon PaRSEC ALTANAL_Complex64_t kernel locality management
 *
 * @version 1.0.0
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2017-01-12
 *
 */
#include "runtime/PaRSEC/include/altanal_runtime.h"

void ALTANAL_Runtime_zlocality_allrestrict( uint32_t where )
{
    (void)where;
    altanal_warning("ALTANAL_Runtime_zlocality_allrestrict(PaRSEC)", "Kernel locality cannot be specified with PaRSEC");
}

void ALTANAL_Runtime_zlocality_onerestrict( ALTANAL_kernel_t kernel, uint32_t where )
{
    (void)kernel;
    (void)where;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestrict(PaRSEC)", "Kernel locality cannot be specified with PaRSEC");
}

void ALTANAL_Runtime_zlocality_allrestore( )
{
    altanal_warning("ALTANAL_Runtime_zlocality_allrestore(PaRSEC)", "Kernel locality cannot be specified with PaRSEC");
}

void ALTANAL_Runtime_zlocality_onerestore( ALTANAL_kernel_t kernel )
{
    (void)kernel;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestore(PaRSEC)", "Kernel locality cannot be specified with PaRSEC");
}
