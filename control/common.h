/**
 *
 * @file common.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2015 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon common header file
 *
 * @version 1.0.0
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2012-09-15
 *
 */
/**
 *  ALTANAL facilities of interest to both ALTANAL core developer
 *  and also of interest to ALTANAL community contributor.
 */
#ifndef _ALTANAL_COMMON_H_
#define _ALTANAL_COMMON_H_


#if defined( _WIN32 ) || defined( _WIN64 )
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdarg.h>

/**
 * Implementation headers
 */
#if defined(ALTANAL_USE_CUDA) && !defined(ALTANAL_SIMULATION)
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#if defined(ALTANAL_USE_CUBLAS_V2)
#include <cublas.h>
#include <cublas_v2.h>
#else
#include <cublas.h>
#endif
#endif

#if defined(ALTANAL_USE_OPENCL) && !defined(ALTANAL_SIMULATION)
#include <OpenCL/cl.h>
#endif

#if defined(ALTANAL_USE_MPI)
#include <mpi.h>
#endif


/**
 *  Altanal header files
 */
#include "altanal.h"

#include "control/auxiliary.h"
#include "control/context.h"
#include "control/async.h"

/**
 *  Global shortcuts
 */
#define ALTANAL_RANK        altanal_rank(altanal)
#define ALTANAL_SIZE        altanal->world_size
#define ALTANAL_GRPSIZE     altanal->group_size
#define ALTANAL_SCHEDULING  altanal->scheduling
#define ALTANAL_PARALLEL    altanal->parallel_enabled
#define ALTANAL_PROFILING   altanal->profiling_enabled
#if defined(ALTANAL_USE_MPI)
#define ALTANAL_MPI_RANK    altanal->my_mpi_rank
#define ALTANAL_MPI_SIZE    altanal->mpi_comm_size
#endif

/**
 *  IPT internal define
 */
#define AltanalIPT_NoDep   0
#define AltanalIPT_Panel   1
#define AltanalIPT_All     2

/**
 *  Global array of LAPACK constants
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
