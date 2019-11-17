/**
 *
 * @file starpu_workspace.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * @brief Al4san StarPU workspace header
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @date 2018-10-18
 *
 */
/**
 *
 * @brief AL4SAN StarPU workspace header
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
**/
#ifndef _AL4SAN_STARPU_WORKSPACE_H_
#define _AL4SAN_STARPU_WORKSPACE_H_
#include "starpu_task.h"
/*
 * Allocate workspace in host memory: CPU for any worker
 * or allocate workspace in worker's memory: main memory for cpu workers,
 * and embedded memory for CUDA devices.
 */
BEGIN_C_DECLS
#define AL4SAN_HOST_MEM    0
#define AL4SAN_WORKER_MEM  1

struct al4san_starpu_ws_s {
    size_t size;
    int    memory_location;
    int    which_workers;
    void  *workspaces[STARPU_NMAXWORKERS];
};

typedef struct al4san_starpu_ws_s AL4SAN_starpu_ws_t;

/*
 * This function creates a workspace on each type of worker in "which_workers"
 * (eg. AL4SAN_CUDA|AL4SAN_CPU for all CPU and GPU workers).  The
 * memory_location argument indicates whether this should be a buffer in host
 * memory or in worker's memory (AL4SAN_HOST_MEM or AL4SAN_WORKER_MEM). This function
 * returns 0 upon successful completion.
 */
int   AL4SAN_Starpu_starpu_ws_alloc   ( AL4SAN_starpu_ws_t **workspace, size_t size, int which_workers, int memory_location);
int   AL4SAN_Starpu_starpu_ws_free    ( AL4SAN_starpu_ws_t  *workspace);
void *AL4SAN_Starpu_starpu_ws_getlocal( AL4SAN_starpu_ws_t  *workspace);
END_C_DECLS
#endif /* _AL4SAN_STARPU_WORKSPACE_H_ */
