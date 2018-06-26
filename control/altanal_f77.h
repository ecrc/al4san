/**
 *
 * @file altanal_f77.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon Fortran77 naming macros
 *
 * @version 1.0.0
 * @author Florent Pruvost
 * @date 2017-05-03
 *
 */
#ifndef _ALTANAL_F77_H_
#define _ALTANAL_F77_H_

#include "altanal/altanal_mangling.h"

/**
 *  Determine FORTRAN names
 */
#define ALTANAL_FNAME(lcname, UCNAME) ALTANAL_GLOBAL(altanal_##lcname, ALTANAL_##UCNAME)
#define ALTANAL_TILE_FNAME(lcname, UCNAME) ALTANAL_GLOBAL(altanal_##lcname##_tile, ALTANAL_##UCNAME##_TILE)
#define ALTANAL_ASYNC_FNAME(lcname, UCNAME) ALTANAL_GLOBAL(altanal_##lcname##_tile_async, ALTANAL_##UCNAME##_TILE_ASYNC)
#define ALTANAL_WS_FNAME(lcname, UCNAME) ALTANAL_GLOBAL(altanal_alloc_workspace_##lcname, ALTANAL_ALLOC_WORKSPACE_##UCNAME)
#define ALTANAL_WST_FNAME(lcname, UCNAME) ALTANAL_GLOBAL(altanal_alloc_workspace_##lcname##_tile, ALTANAL_ALLOC_WORKSPACE_##UCNAME##_TILE)

#define ALTANAL_INIT ALTANAL_GLOBAL(altanal_init, ALTANAL_INIT)
#define ALTANAL_FINALIZE ALTANAL_GLOBAL(altanal_finalize, ALTANAL_FINALIZE)
#define ALTANAL_ENABLE ALTANAL_GLOBAL(altanal_enable, ALTANAL_ENABLE)
#define ALTANAL_DISABLE ALTANAL_GLOBAL(altanal_disable, ALTANAL_DISABLE)
#define ALTANAL_SET ALTANAL_GLOBAL(altanal_set, ALTANAL_SET)
#define ALTANAL_GET ALTANAL_GLOBAL(altanal_get, ALTANAL_GET)
#define ALTANAL_DEALLOC_HANDLE ALTANAL_GLOBAL(altanal_dealloc_handle, ALTANAL_DEALLOC_HANDLE)
#define ALTANAL_VERSION ALTANAL_GLOBAL(altanal_version, ALTANAL_VERSION)
#endif
