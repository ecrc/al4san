/**
 *
 * @file al4san_f77.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 *
 * author Bilel Hadri
 * author Cedric Castagnede
 * author Florent Pruvost
 * date 2010-11-15
 *
 */
#include <stdlib.h>
#include "control/al4san_common.h"
#include "al4san.h"
#include "al4san_f77.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     *  FORTRAN API - auxiliary function prototypes
     */
    void AL4SAN_INIT(int *CORES, int *NGPUS, int *INFO)
    {   *INFO = AL4SAN_Init(*CORES, *NGPUS); }

    void AL4SAN_FINALIZE(int *INFO)
    {   *INFO = AL4SAN_Finalize(); }

    void AL4SAN_ENABLE(AL4SAN_enum *lever, int *INFO)
    {   *INFO = AL4SAN_Enable(*lever); }

    void AL4SAN_DISABLE(AL4SAN_enum *lever, int *INFO)
    {   *INFO = AL4SAN_Disable(*lever); }

    void AL4SAN_SET(AL4SAN_enum *param, int *value, int *INFO)
    {   *INFO = AL4SAN_Set(*param, *value); }

    void AL4SAN_GET(AL4SAN_enum *param, int *value, int *INFO)
    {   *INFO = AL4SAN_Get(*param, value); }

    void AL4SAN_DEALLOC_HANDLE(size_t *sp, int *INFO)
    {   free((void *)(*sp));
        *INFO = AL4SAN_SUCCESS; }

    void AL4SAN_VERSION(int *VER_MAJOR, int *VER_MINOR, int *VER_MICRO, int *INFO)
    {
        *VER_MAJOR = AL4SAN_VERSION_MAJOR;
        *VER_MINOR = AL4SAN_VERSION_MINOR;
        *VER_MICRO = AL4SAN_VERSION_MICRO;
        *INFO = AL4SAN_SUCCESS;
    }

#ifdef __cplusplus
}
#endif
