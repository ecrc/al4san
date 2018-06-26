/**
 *
 * @file runtime_task.c
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL StarPU Task routines
 *
 *  ALTANAL is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 0.1.0
 * @author Rabab Alomairy
 * @date 2018-05-19
 *
 **/


#include <stdio.h>
#include <stdlib.h>
#include "altanal_runtime.h"
#include <stdarg.h>

typedef void (*_starpu_callback_func_t)(void *);

void altanal_task_info(ALTANAL_task_t* altanal_task, int *codelet_buffers, va_list varg_list)
{
  int arg_type, ptr_size;
  void *arg_ptr;
  int num_arg=0;
  va_list varg_list_copy;  

  va_copy(varg_list_copy, varg_list);
  while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
  {
    arg_ptr = va_arg(varg_list_copy, void *);
    ptr_size = va_arg(varg_list_copy, int);
    switch(arg_type){
      case ALTANAL_VALUE:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      break;
      case ALTANAL_INPUT:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=1;
      num_arg++;
      (*codelet_buffers)++;
      break;
      case ALTANAL_OUTPUT:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=1;
      num_arg++;
      (*codelet_buffers)++;
      break;
      case ALTANAL_INOUT:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=1;
      num_arg++;
      (*codelet_buffers)++;
      break;
      case ALTANAL_SCRATCH:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=1;
      num_arg++;
      (*codelet_buffers)++;
      break;
      case ALTANAL_REDUX:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      (*codelet_buffers)++;
      break;
      case ALTANAL_DATA_ARRAY:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      break;
      case ALTANAL_DATA_MODE_ARRAY:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      break;
      case ALTANAL_CL_ARGS:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      break;
      case ALTANAL_CL_ARGS_NFREE:
      altanal_task->arg_depenency[num_arg]=arg_type;
      altanal_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      break;
    }
  }
  altanal_task->num_arg=num_arg;
  va_end(varg_list_copy);

}

//Low level function, it is similar to _starpu_task_insert_create in starpu package
int altanal_task_create(struct starpu_codelet *cl, struct starpu_task *task, va_list varg_list)
{
  int arg_type, ptr_size;
  void *arg_ptr;
  int current_buffer;
  int allocated_buffers = 0;
  unsigned ndeps = 0;
  struct starpu_task **task_deps_array = NULL;
  int num_arg=0;
  int codelet_buffers=0;

  _STARPU_TRACE_TASK_BUILD_START();

  task->cl = cl;
  current_buffer = 0;

  struct starpu_codelet_pack_arg_data state;
  starpu_codelet_pack_arg_init(&state);
  
  ALTANAL_task_t altanal_task;
  altanal_task_info(&altanal_task, &codelet_buffers, varg_list);
/*    
  va_list varg_list_copy; 
  ALTANAL_task_t altanal_task;
  va_copy(varg_list_copy, varg_list);
  while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
    {
      arg_ptr = va_arg(varg_list_copy, void *);
      ptr_size = va_arg(varg_list_copy, int);
      switch(arg_type){
        case ALTANAL_VALUE:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
        break;
        case ALTANAL_INPUT:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=1;
              num_arg++;
              codelet_buffers++;
        break;
        case ALTANAL_OUTPUT:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=1;
              num_arg++;
              codelet_buffers++;
        break;
        case ALTANAL_INOUT:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=1;
              num_arg++;
              codelet_buffers++;
        break;
        case ALTANAL_SCRATCH:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=1;
              num_arg++;
              codelet_buffers++;
        break;
        case ALTANAL_REDUX:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
              codelet_buffers++;
        break;
        case ALTANAL_DATA_ARRAY:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
        break;
        case ALTANAL_DATA_MODE_ARRAY:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
        break;
        case ALTANAL_CL_ARGS:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
        break;
        case ALTANAL_CL_ARGS_NFREE:
              altanal_task.arg_depenency[num_arg]=arg_type;
              altanal_task.arg_size[num_arg]=ptr_size;
              num_arg++;
        break;
      }
    }
    va_end(varg_list_copy);
    
    altanal_task.num_arg=num_arg;
  */ 
  cl->nbuffers=codelet_buffers;

  arg_type=ALTANAL_VALUE;
  arg_ptr= (void *)&altanal_task;
  ptr_size=sizeof(ALTANAL_task_t);
  starpu_codelet_pack_arg(&state, arg_ptr, ptr_size);

  while((arg_type = va_arg(varg_list, int)) != ARG_END){
    if (arg_type & STARPU_R || arg_type & STARPU_W || arg_type & STARPU_SCRATCH || arg_type & STARPU_REDUX)
    {
      /* We have an access mode : we expect to find a handle */
      starpu_data_handle_t handle = va_arg(varg_list, starpu_data_handle_t);
      starpu_task_insert_data_process_arg(cl, task, &allocated_buffers, &current_buffer, arg_type, handle);
      ptr_size=va_arg(varg_list, int);
    }
    else if (arg_type == STARPU_DATA_ARRAY)
    {
      // Expect to find a array of handles and its size
      starpu_data_handle_t *handles = va_arg(varg_list, starpu_data_handle_t *);
      int nb_handles = va_arg(varg_list, int);
      starpu_task_insert_data_process_array_arg(cl, task, &allocated_buffers, &current_buffer, nb_handles, handles);
    }
    else if (arg_type==STARPU_DATA_MODE_ARRAY)
    {
      // Expect to find a array of descr and its size
      struct starpu_data_descr *descrs = va_arg(varg_list, struct starpu_data_descr *);
      int nb_descrs = va_arg(varg_list, int);
      starpu_task_insert_data_process_mode_array_arg(cl, task, &allocated_buffers, &current_buffer, nb_descrs, descrs);
    }
    else if (arg_type==STARPU_VALUE)
    {
      void *ptr = va_arg(varg_list, void *);
      size_t arg_size = va_arg(varg_list, size_t);
      starpu_codelet_pack_arg(&state, ptr, arg_size);
    }
    else if (arg_type==STARPU_CL_ARGS)
    {
      task->cl_arg = va_arg(varg_list, void *);
      task->cl_arg_size = va_arg(varg_list, size_t);
      task->cl_arg_free = 1;
    }
    else if (arg_type==STARPU_CL_ARGS_NFREE)
    {
      task->cl_arg = va_arg(varg_list, void *);
      task->cl_arg_size = va_arg(varg_list, size_t);
      task->cl_arg_free = 0;
    }
    else if (arg_type==STARPU_TASK_DEPS_ARRAY)
    {
      STARPU_ASSERT_MSG(task_deps_array == NULL, "Parameter 'STARPU_TASK_DEPS_ARRAY' cannot be set twice");
      ndeps = va_arg(varg_list, unsigned);
      task_deps_array = va_arg(varg_list, struct starpu_task **);
    }
    else if (arg_type==STARPU_CALLBACK)
    {
      task->callback_func = va_arg(varg_list, _starpu_callback_func_t);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_CALLBACK_WITH_ARG)
    {
      task->callback_func = va_arg(varg_list, _starpu_callback_func_t);
      task->callback_arg = va_arg(varg_list, void *);
    }
    else if (arg_type==STARPU_CALLBACK_ARG)
    {
      task->callback_arg = va_arg(varg_list, void *);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_PROLOGUE_CALLBACK)
    {
      task->prologue_callback_func = va_arg(varg_list, _starpu_callback_func_t);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_PROLOGUE_CALLBACK_ARG)
    {
      task->prologue_callback_arg = va_arg(varg_list, void *);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_PROLOGUE_CALLBACK_POP)
    {
      task->prologue_callback_pop_func = va_arg(varg_list, _starpu_callback_func_t);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_PROLOGUE_CALLBACK_POP_ARG)
    {
      task->prologue_callback_pop_arg = va_arg(varg_list, void *);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_PRIORITY)
    {
      /* Followed by a priority level */
      int prio = va_arg(varg_list, int);
      task->priority = prio;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_EXECUTE_ON_NODE)
    {
      (void)va_arg(varg_list, int);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_EXECUTE_ON_DATA)
    {
      (void)va_arg(varg_list, starpu_data_handle_t);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_EXECUTE_WHERE)
    {
      task->where = va_arg(varg_list, unsigned long long);
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_EXECUTE_ON_WORKER)
    {
      int worker = va_arg(varg_list, int);
      if (worker != -1)
      {
        task->workerid = worker;
        task->execute_on_a_specific_worker = 1;
      }
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_WORKER_ORDER)
    {
      unsigned order = va_arg(varg_list, unsigned);
      if (order != 0)
      {
        STARPU_ASSERT_MSG(task->execute_on_a_specific_worker, "worker order only makes sense if a workerid is provided");
        task->workerorder = order;
      }
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_SCHED_CTX)
    {
      unsigned sched_ctx = va_arg(varg_list, unsigned);
      task->sched_ctx = sched_ctx;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_HYPERVISOR_TAG)
    {
      int hypervisor_tag = va_arg(varg_list, int);
      task->hypervisor_tag = hypervisor_tag;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_POSSIBLY_PARALLEL)
    {
      unsigned possibly_parallel = va_arg(varg_list, unsigned);
      task->possibly_parallel = possibly_parallel;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_FLOPS)
    {
      double flops = va_arg(varg_list, double);
      task->flops = flops;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_TAG)
    {
      starpu_tag_t tag = va_arg(varg_list, starpu_tag_t);
      task->tag_id = tag;
      task->use_tag = 1;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_TAG_ONLY)
    {
      starpu_tag_t tag = va_arg(varg_list, starpu_tag_t);
      task->tag_id = tag;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_NAME)
    {
      const char *name = va_arg(varg_list, const char *);
      task->name = name;
      (void)va_arg(varg_list, int);
    }
    else if (arg_type==STARPU_NODE_SELECTION_POLICY)
    {
      (void)va_arg(varg_list, int);
      (void)va_arg(varg_list, int);
    }
    else if(arg_type==ALTANAL_CUDA_FLG)
    {
#ifdef ALTANAL_USE_CUDA
     if ( va_arg(varg_list, int)== ON){
      cl->cuda_flags[0]=ALTANAL_CUDA_ASYNC;
      (void)va_arg(varg_list, int);
    }
    else
      (void)va_arg(varg_list, int);
#else
    (void)va_arg(varg_list, int);
    (void)va_arg(varg_list, int);          
#endif
  }
  else if (arg_type!=ALTANAL_undefined)
  {
    STARPU_ABORT_MSG("Unrecognized argument %d, did you perhaps forget to end arguments with 0?\n", arg_type);
  }
}
if (cl)
{
  if (cl->nbuffers == STARPU_VARIABLE_NBUFFERS)
  {
    task->nbuffers = current_buffer;
  }
  else
  {
    STARPU_ASSERT_MSG(current_buffer == cl->nbuffers, "Incoherent number of buffers between cl (%d) and number of parameters (%d)", cl->nbuffers, current_buffer);
  }
}

if (state.nargs) {
  if (task->cl_arg != NULL) {
    _STARPU_DISP("Parameters STARPU_CL_ARGS and STARPU_VALUE cannot be used in the same call\n");
    free(state.arg_buffer);
    return -EINVAL;
  }
  starpu_codelet_pack_arg_fini(&state, &task->cl_arg, &task->cl_arg_size);
}

if (task_deps_array)
{
  starpu_task_declare_deps_array(task, ndeps, task_deps_array);
}
_STARPU_TRACE_TASK_BUILD_END();
return 0;
}

//Low level function, it is _starpu_task_build_v in starpu package
static struct starpu_task *altanal_task_build(struct starpu_codelet *cl, const char* task_name, int cl_arg_free, va_list varg_list)
{
  va_list varg_list_copy;
  int ret;

  struct starpu_task *task=starpu_task_create();
  task->name=task_name;
  task->cl_arg_free= cl_arg_free;

  va_copy(varg_list_copy, varg_list);
  ret=altanal_task_create(cl, task, varg_list_copy);
  va_end(varg_list_copy);

  if(ret != 0)
  {
    task->destroy=0;
    starpu_task_destroy(task);
  }
  return (ret==0) ? task: NULL;
}

#if defined(ALTANAL_USE_MPI)
int altanal_mpi_task_decode(struct starpu_codelet *codelet, int me, int nb_nodes, int *xrank, int *do_execute, struct starpu_data_descr **descrs_p, int *nb_data_p, int *prio_p, va_list varg_list)
{
    /* XXX: _fstarpu_mpi_task_decode_v needs to be updated at the same time */
  va_list varg_list_copy;
  int inconsistent_execute = 0;
  int arg_type;
  int node_selected = 0;
  int nb_allocated_data = 16;
  struct starpu_data_descr *descrs;
  int nb_data;
  int prio = 0;
  int select_node_policy = STARPU_MPI_NODE_SELECTION_CURRENT_POLICY;

  _STARPU_TRACE_TASK_MPI_DECODE_START();

  _STARPU_MPI_MALLOC(descrs, nb_allocated_data * sizeof(struct starpu_data_descr));
  nb_data = 0;
  *do_execute = -1;
  *xrank = -1;

  va_copy(varg_list_copy, varg_list);
  while ((arg_type = va_arg(varg_list_copy, int)) != ARG_END)
  {
    int arg_type_nocommute = arg_type & ~STARPU_COMMUTE;
    if (arg_type==STARPU_EXECUTE_ON_NODE)
    {
      *xrank = va_arg(varg_list_copy, int);
      if (node_selected == 0)
      {
        _STARPU_MPI_DEBUG(100, "Executing on node %d\n", *xrank);
        *do_execute = 1;
        node_selected = 1;
        inconsistent_execute = 0;
      }
      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type==STARPU_EXECUTE_ON_DATA)
    {
      starpu_data_handle_t data = va_arg(varg_list_copy, starpu_data_handle_t);
      if (node_selected == 0)
      {
        *xrank = starpu_mpi_data_get_rank(data);
        STARPU_ASSERT_MSG(*xrank != -1, "Rank of the data must be set using starpu_mpi_data_register() or starpu_data_set_rank()");
        _STARPU_MPI_DEBUG(100, "Executing on data node %d\n", *xrank);
        STARPU_ASSERT_MSG(*xrank <= nb_nodes, "Node %d to execute codelet is not a valid node (%d)", *xrank, nb_nodes);
        *do_execute = 1;
        node_selected = 1;
        inconsistent_execute = 0;
      }
      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type_nocommute & STARPU_R || arg_type_nocommute & STARPU_W || arg_type_nocommute & STARPU_RW || arg_type & STARPU_SCRATCH || arg_type & STARPU_REDUX)
    {
      starpu_data_handle_t data = va_arg(varg_list_copy, starpu_data_handle_t);
      enum starpu_data_access_mode mode = (enum starpu_data_access_mode) arg_type;
      if (node_selected == 0)
      {
        int ret = _starpu_mpi_find_executee_node(data, mode, me, do_execute, &inconsistent_execute, xrank);
        if (ret == -EINVAL)
        {
          free(descrs);
          va_end(varg_list_copy);
          _STARPU_TRACE_TASK_MPI_DECODE_END();
          return ret;
        }
      }
      if (nb_data >= nb_allocated_data)
      {
        nb_allocated_data *= 2;
        _STARPU_MPI_REALLOC(descrs, nb_allocated_data * sizeof(struct starpu_data_descr));
      }
      descrs[nb_data].handle = data;
      descrs[nb_data].mode = mode;
      nb_data ++;

      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type == STARPU_DATA_ARRAY)
    {
      starpu_data_handle_t *datas = va_arg(varg_list_copy, starpu_data_handle_t *);
      int nb_handles = va_arg(varg_list_copy, int);
      int i;

      for(i=0 ; i<nb_handles ; i++)
      {
        STARPU_ASSERT_MSG(codelet->nbuffers == STARPU_VARIABLE_NBUFFERS || nb_data < codelet->nbuffers, "Too many data passed to starpu_mpi_task_insert");
        enum starpu_data_access_mode mode = STARPU_CODELET_GET_MODE(codelet, nb_data);
        if (node_selected == 0)
        {
          int ret = _starpu_mpi_find_executee_node(datas[i], mode, me, do_execute, &inconsistent_execute, xrank);
          if (ret == -EINVAL)
          {
            free(descrs);
            va_end(varg_list_copy);
            _STARPU_TRACE_TASK_MPI_DECODE_END();
            return ret;
          }
        }
        if (nb_data >= nb_allocated_data)
        {
          nb_allocated_data *= 2;
          _STARPU_MPI_REALLOC(descrs, nb_allocated_data * sizeof(struct starpu_data_descr));
        }
        descrs[nb_data].handle = datas[i];
        descrs[nb_data].mode = mode;
        nb_data ++;
      }

      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type == STARPU_DATA_MODE_ARRAY)
    {
      struct starpu_data_descr *_descrs = va_arg(varg_list_copy, struct starpu_data_descr*);
      int nb_handles = va_arg(varg_list_copy, int);
      int i;

      for(i=0 ; i<nb_handles ; i++)
      {
        enum starpu_data_access_mode mode = _descrs[i].mode;
        if (node_selected == 0)
        {
          int ret = _starpu_mpi_find_executee_node(_descrs[i].handle, mode, me, do_execute, &inconsistent_execute, xrank);
          if (ret == -EINVAL)
          {
            free(descrs);
            va_end(varg_list_copy);
            _STARPU_TRACE_TASK_MPI_DECODE_END();
            return ret;
          }
        }
        if (nb_data >= nb_allocated_data)
        {
          nb_allocated_data *= 2;
          _STARPU_MPI_REALLOC(descrs, nb_allocated_data * sizeof(struct starpu_data_descr));
        }
        descrs[nb_data].handle = _descrs[i].handle;
        descrs[nb_data].mode = mode;
        nb_data ++;
      }
      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type==STARPU_VALUE)
    {
      (void)va_arg(varg_list_copy, void *);
      (void)va_arg(varg_list_copy, size_t);
    }
    else if (arg_type==STARPU_CL_ARGS)
    {
      (void)va_arg(varg_list_copy, void *);
      (void)va_arg(varg_list_copy, size_t);
    }
    else if (arg_type==STARPU_CL_ARGS_NFREE)
    {
      (void)va_arg(varg_list_copy, void *);
      (void)va_arg(varg_list_copy, size_t);
    }
    else if (arg_type==STARPU_TASK_DEPS_ARRAY)
    {
      (void)va_arg(varg_list_copy, unsigned);
      (void)va_arg(varg_list_copy, struct starpu_task **);
    }
    else if (arg_type==STARPU_CALLBACK)
    {
      (void)va_arg(varg_list_copy, _starpu_callback_func_t);
      (void)va_arg(varg_list_copy, int);  
    }
    else if (arg_type==STARPU_CALLBACK_WITH_ARG)
    {
      (void)va_arg(varg_list_copy, _starpu_callback_func_t);
      (void)va_arg(varg_list_copy, void *);
    }
    else if (arg_type==STARPU_CALLBACK_ARG)
    {
      (void)va_arg(varg_list_copy, void *);
      (void)va_arg(varg_list_copy, int);      }
      else if (arg_type==STARPU_PRIORITY)
      {
        prio = va_arg(varg_list_copy, int);
        (void)va_arg(varg_list_copy, int);  
      }
    /* STARPU_EXECUTE_ON_NODE handled above */
    /* STARPU_EXECUTE_ON_DATA handled above */
    /* STARPU_DATA_ARRAY handled above */
    /* STARPU_DATA_MODE_ARRAY handled above */
      else if (arg_type==STARPU_TAG)
      {
        (void)va_arg(varg_list_copy, starpu_tag_t);
        (void)va_arg(varg_list_copy, int);  

      }
      else if (arg_type==STARPU_HYPERVISOR_TAG)
      {
        (void)va_arg(varg_list_copy, int);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_FLOPS)
      {
        (void)va_arg(varg_list_copy, double);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_SCHED_CTX)
      {
        (void)va_arg(varg_list_copy, unsigned);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_PROLOGUE_CALLBACK)
      {
        (void)va_arg(varg_list_copy, _starpu_callback_func_t);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_PROLOGUE_CALLBACK_ARG)
      {
        (void)va_arg(varg_list_copy, void *);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_PROLOGUE_CALLBACK_POP)
      {
        (void)va_arg(varg_list_copy, _starpu_callback_func_t);
        (void)va_arg(varg_list_copy, int);      
      }
      else if (arg_type==STARPU_PROLOGUE_CALLBACK_POP_ARG)
      {
        (void)va_arg(varg_list_copy, void *);
        (void)va_arg(varg_list_copy, int);      
      }
      else if (arg_type==STARPU_EXECUTE_WHERE)
      {
      // the flag is decoded and set later when
      // calling function _starpu_task_insert_create()
        (void)va_arg(varg_list_copy, unsigned long long);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_EXECUTE_ON_WORKER)
      {
      // the flag is decoded and set later when
      // calling function _starpu_task_insert_create()
        (void)va_arg(varg_list_copy, int);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_TAG_ONLY)
      {
        (void)va_arg(varg_list_copy, starpu_tag_t);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_NAME)
      {
        (void)va_arg(varg_list_copy, const char *);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_POSSIBLY_PARALLEL)
      {
        (void)va_arg(varg_list_copy, unsigned);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_WORKER_ORDER)
      {
      // the flag is decoded and set later when
      // calling function _starpu_task_insert_create()
        (void)va_arg(varg_list_copy, unsigned);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==STARPU_NODE_SELECTION_POLICY)
      {
        select_node_policy = va_arg(varg_list_copy, int);
        (void)va_arg(varg_list_copy, int);  
      }
      else if (arg_type==ALTANAL_CUDA_FLG)
      {
        (void)va_arg(varg_list_copy, unsigned);
        (void)va_arg(varg_list_copy, int);
      }
      else if (arg_type!=ALTANAL_undefined)
      {
        STARPU_ABORT_MSG("Unrecognized argument %d, did you perhaps forget to end arguments with 0?\n", arg_type);
      }

    }
    va_end(varg_list_copy);

    if (inconsistent_execute == 1 || *xrank == -1)
    {
    // We need to find out which node is going to execute the codelet.
      _STARPU_MPI_DEBUG(100, "Different nodes are owning W data. The node to execute the codelet is going to be selected with the current selection node policy. See starpu_mpi_node_selection_set_current_policy() to change the policy, or use STARPU_EXECUTE_ON_NODE or STARPU_EXECUTE_ON_DATA to specify the node\n");
      *xrank = _starpu_mpi_select_node(me, nb_nodes, descrs, nb_data, select_node_policy);
      *do_execute = *xrank == STARPU_MPI_PER_NODE || (me == *xrank);
    }
    else
    {
      _STARPU_MPI_DEBUG(100, "Inconsistent=%d - xrank=%d\n", inconsistent_execute, *xrank);
      *do_execute = *xrank == STARPU_MPI_PER_NODE || (me == *xrank);
    }
    _STARPU_MPI_DEBUG(100, "do_execute=%d\n", *do_execute);

    *descrs_p = descrs;
    *nb_data_p = nb_data;
    *prio_p = prio;

    _STARPU_TRACE_TASK_MPI_DECODE_END();
    return 0;
  }

  int altanal_task_build_mpi(MPI_Comm comm, struct starpu_codelet *codelet, struct starpu_task **task, int *xrank_p, struct starpu_data_descr **descrs_p, int *nb_data_p, int *prio_p, va_list varg_list)
  {
    int me, do_execute, xrank, nb_nodes;
    int ret;
    int i;
    struct starpu_data_descr *descrs;
    int nb_data;
    int prio;

    _STARPU_MPI_LOG_IN();

    starpu_mpi_comm_rank(comm, &me);
    starpu_mpi_comm_size(comm, &nb_nodes);

  /* Find out whether we are to execute the data because we own the data to be written to. */
    ret = altanal_mpi_task_decode(codelet, me, nb_nodes, &xrank, &do_execute, &descrs, &nb_data, &prio, varg_list);
    if (ret < 0)
      return ret;

    _STARPU_TRACE_TASK_MPI_PRE_START();
  /* Send and receive data as requested */
    for(i=0 ; i<nb_data ; i++)
    {
      _starpu_mpi_exchange_data_before_execution(descrs[i].handle, descrs[i].mode, me, xrank, do_execute, prio, comm);
    }

    if (xrank_p)
      *xrank_p = xrank;
    if (nb_data_p)
      *nb_data_p = nb_data;
    if (prio_p)
      *prio_p = prio;

    if (descrs_p)
      *descrs_p = descrs;
    else
      free(descrs);


    _STARPU_TRACE_TASK_MPI_PRE_END();

    if (do_execute == 0)
    {

      return 1;
    }
    else
    {

      va_list varg_list_copy;
      _STARPU_MPI_DEBUG(100, "Execution of the codelet %p (%s)\n", codelet, codelet?codelet->name:NULL);

      *task = starpu_task_create();
      (*task)->cl_arg_free = 1;

      va_copy(varg_list_copy, varg_list);
      altanal_task_create(codelet, *task, varg_list_copy);
      va_end(varg_list_copy);

      return 0;
    }
  }

  void altanal_insert_task(altanal_codelet *codelet,  ALTANAL_option_t *options, ...) 
  {
    va_list varg_list;
    int ret;
    int xrank;
    int do_execute = 0;
    struct starpu_data_descr *descrs;
    int nb_data;
    int prio;
    struct starpu_task *task;
    int arg_type, ptr_size;
    void *arg_ptr;
    ALTANAL_task_t altanal_task;

    va_start(varg_list, options);
    

    ret=altanal_task_build_mpi(starpu_mpi_codelet(codelet), &task, &xrank, &descrs, &nb_data, &prio, varg_list);

    
    if (ret < 0)
      return ret;

    if (ret == 0)
    {
      do_execute = 1;
      ret = starpu_task_submit(task);

      if (STARPU_UNLIKELY(ret == -ENODEV))
      {

        _STARPU_MSG("submission of task %p wih codelet %p failed (symbol `%s') (err: ENODEV)\n",
          task, task->cl,
          (codelet == NULL) ? "none" :
          task->cl->name ? task->cl->name :
          (task->cl->model && task->cl->model->symbol)?task->cl->model->symbol:"none");

        task->destroy = 0;
        starpu_task_destroy(task);
      }
    }

    int val = _starpu_mpi_task_postbuild_v(MPI_COMM_WORLD, xrank, do_execute, descrs, nb_data, prio);

    if (ret == 0 && pre_submit_hook)
      pre_submit_hook(task);

    va_end(varg_list);
    return val;
  }

  int ALTANAL_Runtime_insert_task(ALTANAL_codelet codelet,  ALTANAL_option_t *options, va_list varg_list)
  {
    int ret;
    int xrank;
    int do_execute = 0;
    struct starpu_data_descr *descrs;
    int nb_data;
    int prio;
    struct starpu_task *task;
    int arg_type, ptr_size;
    void *arg_ptr;
    ALTANAL_task_t altanal_task; 

    ret=altanal_task_build_mpi(starpu_mpi_codelet((altanal_codelet *)codelet), &task, &xrank, &descrs, &nb_data, &prio, varg_list);

    
    if (ret < 0)
      return ret;

    if (ret == 0)
    {
      do_execute = 1;
      ret = starpu_task_submit(task);

      if (STARPU_UNLIKELY(ret == -ENODEV))
      {

        _STARPU_MSG("submission of task %p wih codelet %p failed (symbol `%s') (err: ENODEV)\n",
          task, task->cl,
          ((altanal_codelet *)codelet == NULL) ? "none" :
          task->cl->name ? task->cl->name :
          (task->cl->model && task->cl->model->symbol)?task->cl->model->symbol:"none");

        task->destroy = 0;
        starpu_task_destroy(task);
      }
    }

    int val = _starpu_mpi_task_postbuild_v(MPI_COMM_WORLD, xrank, do_execute, descrs, nb_data, prio);

    if (ret == 0 && pre_submit_hook)
      pre_submit_hook(task);

    return val;
  }

#else
  void altanal_insert_task(altanal_codelet *codelet,  ALTANAL_option_t *options, ...) 
  {
    va_list varg_list;
    int ret;
    struct starpu_task *task;
    int arg_type, ptr_size;
    void *arg_ptr;
    ALTANAL_task_t altanal_task;

    va_start(varg_list, options);

    task=altanal_task_build(starpu_mpi_codelet(codelet), NULL, 1, varg_list);
    ret=starpu_task_submit(task);
    
    if (STARPU_UNLIKELY(ret == -ENODEV))
    {
      _STARPU_MSG("submission of task %p wih codelet %p failed (symbol `%s') (err: ENODEV)\n",
        task, task->cl,
        (codelet == NULL) ? "none" :
        task->cl->name ? task->cl->name :
        (task->cl->model && task->cl->model->symbol)?task->cl->model->symbol:"none");
      task->destroy = 0;
      starpu_task_destroy(task);
    }


    va_end(varg_list);
    return ret;
  }

  int ALTANAL_Runtime_insert_task(ALTANAL_codelet codelet,  ALTANAL_option_t *options, va_list varg_list)
  {
    int ret;
    struct starpu_task *task;
    int arg_type, ptr_size;
    void *arg_ptr; 
    ALTANAL_task_t altanal_task;
    
    
    task=altanal_task_build(starpu_mpi_codelet((altanal_codelet *)codelet), NULL, 1, varg_list);
    ret=starpu_task_submit(task);
    
    if (STARPU_UNLIKELY(ret == -ENODEV))
    { 
      _STARPU_MSG("submission of task %p wih codelet %p failed (symbol `%s') (err: ENODEV)\n",
        task, task->cl,
        ((altanal_codelet *)codelet == NULL) ? "none" :
        task->cl->name ? task->cl->name :
        (task->cl->model && task->cl->model->symbol)?task->cl->model->symbol:"none");
      task->destroy = 0;
      starpu_task_destroy(task);
    }  
    return ret;
  }
#endif
  void altanal_task_init(ALTANAL_task_t **altanal_task)
  {

   if (*altanal_task==NULL){
    *altanal_task = (ALTANAL_task_t*)malloc(sizeof(ALTANAL_task_t));
  }
}


void altanal_unpack_arg(ALTANAL_arg_list *altanal_arg, ...)
{

  char *cl_arg = (char *) altanal_arg->cl_arg;
  ALTANAL_task_t altanal_task;
  va_list varg_list;
  size_t current_arg_offset = 0;
  int nargs, arg;
  void *_buffer=NULL;
  size_t buffer_size=0;
  int ptr_index=0;
  STARPU_ASSERT(cl_arg);
  void *argptr;
  void **tmp_ref;
  size_t arg_size;
  
      /* We fill the different pointers with the appropriate arguments */
  memcpy(&nargs, cl_arg, sizeof(nargs));
  current_arg_offset += sizeof(nargs);

  memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
  current_arg_offset += sizeof(arg_size);

  memcpy(&altanal_task, cl_arg+current_arg_offset, arg_size);
  current_arg_offset += arg_size;

  va_start(varg_list, altanal_arg);

  for (int i=0;i<altanal_task.num_arg;i++)
  {
   if (altanal_task.arg_depenency[i]==ALTANAL_INPUT  || 
     altanal_task.arg_depenency[i]==ALTANAL_OUTPUT || 
     altanal_task.arg_depenency[i]==ALTANAL_INOUT  || 
     altanal_task.arg_depenency[i]==ALTANAL_SCRATCH)

   {
     tmp_ref = va_arg(varg_list, void**);
     *tmp_ref = STARPU_MATRIX_GET_PTR(altanal_arg->descr[ptr_index]); 
     ptr_index++;

   }
   else
   {
     
    argptr = va_arg(varg_list, void *);

    /* If not reading all cl_args */
    // NULL was the initial end marker, we now use 0
    // 0 and NULL should be the same value, but we
    // keep both equalities for systems on which they could be different
    // cppcheck-suppress duplicateExpression
    if(argptr == 0 || argptr == NULL)
      break;

    memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
    current_arg_offset += sizeof(arg_size);

    memcpy(argptr, cl_arg+current_arg_offset, arg_size);
    current_arg_offset += arg_size;
  }
}
va_end(varg_list);
}


int ALTANAL_Runtime_unpack_arg(ALTANAL_arg args, va_list varg_list)
{
  ALTANAL_arg_list *altanal_arg= (ALTANAL_arg_list*)args;
  char *cl_arg = (char *) altanal_arg->cl_arg;
  ALTANAL_task_t altanal_task;
  size_t current_arg_offset = 0;
  int nargs, arg;
  void *_buffer=NULL;
  size_t buffer_size=0;
  int ptr_index=0;
  STARPU_ASSERT(cl_arg);
  void *argptr;
  void **tmp_ref;
  size_t arg_size;
  
      /* We fill the different pointers with the appropriate arguments */
  memcpy(&nargs, cl_arg, sizeof(nargs));
  current_arg_offset += sizeof(nargs);

  memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
  current_arg_offset += sizeof(arg_size);

  memcpy(&altanal_task, cl_arg+current_arg_offset, arg_size);
  current_arg_offset += arg_size;
  
  for (int i=0;i<altanal_task.num_arg;i++)
  {
   if (altanal_task.arg_depenency[i]==ALTANAL_INPUT  || 
     altanal_task.arg_depenency[i]==ALTANAL_OUTPUT || 
     altanal_task.arg_depenency[i]==ALTANAL_INOUT  || 
     altanal_task.arg_depenency[i]==ALTANAL_SCRATCH)

   {
     tmp_ref = va_arg(varg_list, void**);
     *tmp_ref = STARPU_MATRIX_GET_PTR(altanal_arg->descr[ptr_index]); 
     ptr_index++;

   }
   else
   {
     
    argptr = va_arg(varg_list, void *);

    /* If not reading all cl_args */
    // NULL was the initial end marker, we now use 0
    // 0 and NULL should be the same value, but we
    // keep both equalities for systems on which they could be different
    // cppcheck-suppress duplicateExpression
    if(argptr == 0 || argptr == NULL)
      break;

    memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
    current_arg_offset += sizeof(arg_size);

    memcpy(argptr, cl_arg+current_arg_offset, arg_size);
    current_arg_offset += arg_size;
  }
}
}
