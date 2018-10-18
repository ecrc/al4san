/**
 *
 * @file al4san_f77.h
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
 * author Florent Pruvost
 * date 2017-05-03
 *
 */
#ifndef _AL4SAN_F77_H_
#define _AL4SAN_F77_H_

#include "al4san/al4san_mangling.h"

/**
 *  Determine FORTRAN names
 */
#define AL4SAN_FNAME(lcname, UCNAME) AL4SAN_GLOBAL(al4san_##lcname, AL4SAN_##UCNAME)
#define AL4SAN_TILE_FNAME(lcname, UCNAME) AL4SAN_GLOBAL(al4san_##lcname##_tile, AL4SAN_##UCNAME##_TILE)
#define AL4SAN_ASYNC_FNAME(lcname, UCNAME) AL4SAN_GLOBAL(al4san_##lcname##_tile_async, AL4SAN_##UCNAME##_TILE_ASYNC)
#define AL4SAN_WS_FNAME(lcname, UCNAME) AL4SAN_GLOBAL(al4san_alloc_workspace_##lcname, AL4SAN_ALLOC_WORKSPACE_##UCNAME)
#define AL4SAN_WST_FNAME(lcname, UCNAME) AL4SAN_GLOBAL(al4san_alloc_workspace_##lcname##_tile, AL4SAN_ALLOC_WORKSPACE_##UCNAME##_TILE)

#define AL4SAN_INIT AL4SAN_GLOBAL(al4san_init, AL4SAN_INIT)
#define AL4SAN_FINALIZE AL4SAN_GLOBAL(al4san_finalize, AL4SAN_FINALIZE)
#define AL4SAN_ENABLE AL4SAN_GLOBAL(al4san_enable, AL4SAN_ENABLE)
#define AL4SAN_DISABLE AL4SAN_GLOBAL(al4san_disable, AL4SAN_DISABLE)
#define AL4SAN_SET AL4SAN_GLOBAL(al4san_set, AL4SAN_SET)
#define AL4SAN_GET AL4SAN_GLOBAL(al4san_get, AL4SAN_GET)
#define AL4SAN_DEALLOC_HANDLE AL4SAN_GLOBAL(al4san_dealloc_handle, AL4SAN_DEALLOC_HANDLE)
#define AL4SAN_VERSION AL4SAN_GLOBAL(al4san_version, AL4SAN_VERSION)
#endif
