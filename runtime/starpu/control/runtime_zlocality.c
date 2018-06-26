/**
 *
 * @file runtime_zlocality.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU ALTANAL_Complex64_t kernel locality management
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 * @precisions normal z -> s d c
 *
 */
#include "altanal_runtime.h"
//#include "runtime_codelet_z.h"

#ifdef ALTANAL_USE_CUDA
/* Only codelets with multiple choices are present here */
void ALTANAL_Runtime_zlocality_allrestrict( uint32_t where )
{
    (void)where;
    altanal_warning("ALTANAL_Runtime_zlocality_allrestrict(starpu)", "not yet supported");

}

void ALTANAL_Runtime_zlocality_onerestrict( ALTANAL_kernel_t kernel, uint32_t where )
{   
    (void)kernel;
    (void)where;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestrict(starpu)", "not yet supported");
}

void ALTANAL_Runtime_zlocality_allrestore( )
{
    altanal_warning("ALTANAL_Runtime_zlocality_allrestore(starpu)", "not yet supported");

}

void ALTANAL_Runtime_zlocality_onerestore( ALTANAL_kernel_t kernel )
{
     (void)kernel;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestore(starpu)", "not yet supported");
}
#else

void ALTANAL_Runtime_zlocality_allrestrict( uint32_t where )
{
    (void)where;
        altanal_warning("ALTANAL_Runtime_zlocality_allrestrict(starpu)", "not yet supported");

}

void ALTANAL_Runtime_zlocality_onerestrict( ALTANAL_kernel_t kernel, uint32_t where )
{
    (void)kernel;
    (void)where;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestrict(starpu)", "not yet supported");

}

void ALTANAL_Runtime_zlocality_allrestore( ) {
altanal_warning("ALTANAL_Runtime_zlocality_allrestore(starpu)", "not yet supported");
 }

void ALTANAL_Runtime_zlocality_onerestore( ALTANAL_kernel_t kernel )
{
    (void)kernel;
    altanal_warning("ALTANAL_Runtime_zlocality_onerestore(starpu)", "not yet supported");

}

#endif
