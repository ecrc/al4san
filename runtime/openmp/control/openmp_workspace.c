/**
 *
 * @file   runtime_workspace.c
 *
 *  PLASMA is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
   *
   * @brief AL4SAN Workspace routines
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
**/

#include "al4san_openmp.h"
#include <omp.h>

/******************************************************************************/

int AL4SAN_Openmp_Workspace_Create(AL4SAN_workspace_t *workspace, size_t lworkspace,
                            size_t dtyp)
{

    #pragma omp parallel
    #pragma omp master
    {
       workspace->nthread = omp_get_num_threads();
 
    }
    workspace->lworkspace = lworkspace;
    workspace->dtyp  = dtyp;
    if ((workspace->spaces = (void**)calloc(workspace->nthread,
                                            sizeof(void*))) == NULL) {
        free(workspace->spaces);
        al4san_error("RUNTIME_workspace_create", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    // Each thread allocates its workspace.
    size_t size = (size_t)lworkspace * workspace->dtyp; 
    int info = AL4SAN_SUCCESS;
    #pragma omp parallel
    {        int tid = omp_get_thread_num();
      if ((workspace->spaces[tid] = (void*)malloc(size)) == NULL) {
            info = AL4SAN_ERR_OUT_OF_RESOURCES;
        }
    }
    if (info != AL4SAN_SUCCESS) {
        AL4SAN_Workspace_Destroy(workspace);
    }
    return info;
}

/******************************************************************************/
int AL4SAN_Openmp_Workspace_Destroy(AL4SAN_workspace_t *workspace)
{
    if (workspace->spaces != NULL) {
        for (int i = 0; i < workspace->nthread; ++i) {
            free(workspace->spaces[i]);
            workspace->spaces[i] = NULL;
        }
        free(workspace->spaces);
        workspace->spaces  = NULL;
        workspace->nthread = 0;
        workspace->lworkspace   = 0;
    }
    return AL4SAN_SUCCESS;
}
