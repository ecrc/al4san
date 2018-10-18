/**
 *
 * @file al4san_common.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2015 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
  *
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2012-09-15
 *
 */
  /**
   *
   * @brief AL4SAN common headers
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
   **/
#ifndef _AL4SAN_COMMON_H_
#define _AL4SAN_COMMON_H_


#if defined( _WIN32 ) || defined( _WIN64 )
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdarg.h>

/**
 * Implementation headers
 */
#if defined(AL4SAN_USE_CUDA) && !defined(AL4SAN_SIMULATION)
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#if defined(AL4SAN_USE_CUBLAS_V2)
#include <cublas.h>
#include <cublas_v2.h>
#else
#include <cublas.h>
#endif
#endif

#if defined(AL4SAN_USE_OPENCL) && !defined(AL4SAN_SIMULATION)
#include <OpenCL/cl.h>
#endif

#if defined(AL4SAN_USE_MPI)
#include <mpi.h>
#endif


/**
 *  Al4san header files
 */
#include "al4san.h"

#include "control/al4san_auxiliary.h"
#include "control/al4san_context.h"
#include "control/al4san_async.h"

/**
 *  Global shortcuts
 */
#define AL4SAN_RANK        al4san_rank(al4san)
#define AL4SAN_SIZE        al4san->world_size
#define AL4SAN_GRPSIZE     al4san->group_size
#define AL4SAN_SCHEDULING  al4san->scheduling
#define AL4SAN_PARALLEL    al4san->parallel_enabled
#define AL4SAN_PROFILING   al4san->profiling_enabled
#if defined(AL4SAN_USE_MPI)
#define AL4SAN_MPI_RANK    al4san->my_mpi_rank
#define AL4SAN_MPI_SIZE    al4san->mpi_comm_size
#endif

/**
 *  IPT internal define
 */
#define Al4sanIPT_NoDep   0
#define Al4sanIPT_Panel   1

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
