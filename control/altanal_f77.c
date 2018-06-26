/**
 *
 * @file altanal_f77.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon Fortran77 interface
 *
 * @version 1.0.0
 * @author Bilel Hadri
 * @author Cedric Castagnede
 * @author Florent Pruvost
 * @date 2010-11-15
 *
 */
#include <stdlib.h>
#include "control/common.h"
#include "altanal.h"
#include "altanal_f77.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     *  FORTRAN API - auxiliary function prototypes
     */
    void ALTANAL_INIT(int *CORES, int *NGPUS, int *INFO)
    {   *INFO = ALTANAL_Init(*CORES, *NGPUS); }

    void ALTANAL_FINALIZE(int *INFO)
    {   *INFO = ALTANAL_Finalize(); }

    void ALTANAL_ENABLE(ALTANAL_enum *lever, int *INFO)
    {   *INFO = ALTANAL_Enable(*lever); }

    void ALTANAL_DISABLE(ALTANAL_enum *lever, int *INFO)
    {   *INFO = ALTANAL_Disable(*lever); }

    void ALTANAL_SET(ALTANAL_enum *param, int *value, int *INFO)
    {   *INFO = ALTANAL_Set(*param, *value); }

    void ALTANAL_GET(ALTANAL_enum *param, int *value, int *INFO)
    {   *INFO = ALTANAL_Get(*param, value); }

    void ALTANAL_DEALLOC_HANDLE(size_t *sp, int *INFO)
    {   free((void *)(*sp));
        *INFO = ALTANAL_SUCCESS; }

    void ALTANAL_VERSION(int *VER_MAJOR, int *VER_MINOR, int *VER_MICRO, int *INFO)
    {
        *VER_MAJOR = ALTANAL_VERSION_MAJOR;
        *VER_MINOR = ALTANAL_VERSION_MINOR;
        *VER_MICRO = ALTANAL_VERSION_MICRO;
        *INFO = ALTANAL_SUCCESS;
    }

#ifdef __cplusplus
}
#endif
