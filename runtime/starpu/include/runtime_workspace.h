/**
 *
 * @file runtime_workspace.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Altanal StarPU workspace header
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @date 2011-06-01
 *
 */
#ifndef _ALTANAL_STARPU_WORKSPACE_H_
#define _ALTANAL_STARPU_WORKSPACE_H_

/*
 * Allocate workspace in host memory: CPU for any worker
 * or allocate workspace in worker's memory: main memory for cpu workers,
 * and embedded memory for CUDA devices.
 */
#define ALTANAL_HOST_MEM    0
#define ALTANAL_WORKER_MEM  1

struct altanal_runtime_ws_s {
    size_t size;
    int    memory_location;
    int    which_workers;
    void  *workspaces[STARPU_NMAXWORKERS];
};

typedef struct altanal_runtime_ws_s ALTANAL_starpu_ws_t;

/*
 * This function creates a workspace on each type of worker in "which_workers"
 * (eg. ALTANAL_CUDA|ALTANAL_CPU for all CPU and GPU workers).  The
 * memory_location argument indicates whether this should be a buffer in host
 * memory or in worker's memory (ALTANAL_HOST_MEM or ALTANAL_WORKER_MEM). This function
 * returns 0 upon successful completion.
 */
int   ALTANAL_Runtime_starpu_ws_alloc   ( ALTANAL_starpu_ws_t **workspace, size_t size, int which_workers, int memory_location);
int   ALTANAL_Runtime_starpu_ws_free    ( ALTANAL_starpu_ws_t  *workspace);
void *ALTANAL_Runtime_starpu_ws_getlocal( ALTANAL_starpu_ws_t  *workspace);

#endif /* _ALTANAL_STARPU_WORKSPACE_H_ */
