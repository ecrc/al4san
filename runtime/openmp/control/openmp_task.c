  /**
   *
   * @file runtime_task.c
   *
   * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
   *                     All rights reserved.
   *
   * @brief AL4SAN StarPU Task routines
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.1.0
   * @author Rabab Alomairy
   * @date 2018-10-18
**/


#include <stdio.h>
#include <stdlib.h>
#include "al4san_openmp.h"
#include <stdarg.h>
#include <string.h>
#include <assert.h>

int ARG_END;
int AL4SAN_DEP;
int AL4SAN_CUDA_ASYNC;
int AL4SAN_CUDA_FLG;
int AL4SAN_INPUT;
int AL4SAN_OUTPUT;
int AL4SAN_INOUT;
int AL4SAN_SCRATCH;
int AL4SAN_REDUX;
int AL4SAN_COMMUTE;
int AL4SAN_SSEND;
int AL4SAN_ACCESS_MODE_MAX;
int AL4SAN_VALUE;
int AL4SAN_DATA_ARRAY;
int AL4SAN_DATA_MODE_ARRAY;
int AL4SAN_CL_ARGS_NFREE;
int AL4SAN_CL_ARGS;
int AL4SAN_CALLBACK;
int AL4SAN_CALLBACK_WITH_ARG;
int AL4SAN_CALLBACK_ARG;
int AL4SAN_PROLOGUE_CALLBACK;
int AL4SAN_PROLOGUE_CALLBACK_ARG;
int AL4SAN_PROLOGUE_CALLBACK_POP;
int AL4SAN_PROLOGUE_CALLBACK_POP_ARG;
int AL4SAN_EXECUTE_ON_NODE;
int AL4SAN_EXECUTE_ON_DATA;
int AL4SAN_EXECUTE_ON_WORKER;
int AL4SAN_WORKER_ORDER;
int AL4SAN_SCHED_CTX;
int AL4SAN_HYPERVISOR_TAG;
int AL4SAN_POSSIBLY_PARALLEL;
int AL4SAN_FLOPS;
int AL4SAN_TAG;
int AL4SAN_TAG_ONLY;
int AL4SAN_NODE_SELECTION_POLICY;
int AL4SAN_TASK_DEPS_ARRAY;
int AL4SAN_EXECUTE_WHERE;
int AL4SAN_COLOR;
int AL4SAN_LOCALITY;
int AL4SAN_PRIORITY;
int AL4SAN_LABEL;
int AL4SAN_REGION_0;
int AL4SAN_REGION_1;
int AL4SAN_REGION_2;
int AL4SAN_REGION_3;
int AL4SAN_REGION_4;
int AL4SAN_REGION_5;
int AL4SAN_REGION_6;
int AL4SAN_REGION_7;
               //ldu region
int AL4SAN_REGION_L;
int AL4SAN_REGION_D;
int AL4SAN_REGION_U;
               //flags
int AL4SAN_LOCK_TO_THREAD;
int AL4SAN_SEQUENCE;
int AL4SAN_THREAD_COUNT;
int AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING;
int AL4SAN_LOCK_TO_THREAD_MASK;
//int AL4SAN_COLOR = INIT_VALUE;
               //data dep
int AL4SAN_NODEP;
int AL4SAN_NOLOCALITY;
int AL4SAN_ACCUMULATOR;
int AL4SAN_NOACCUMULATOR;
int AL4SAN_GATHERV;
int AL4SAN_NOGATHERV;
               //parsec
int AL4SAN_REF;
int AL4SAN_AFFINITY;
int AL4SAN_DONT_TRACK;
int AL4SAN_PASSED_BY_REF;

int AL4SAN_FULL_TILE;

void *OPENMP_func, *STARPU_func, *PARSEC_func, *QUARK_func;

void AL4SAN_Openmp_task_option_init()
{
  AL4SAN_DEP = 101;
  ARG_END = 0;
  AL4SAN_INPUT=(1<<0);
  AL4SAN_OUTPUT=(1<<1);
  AL4SAN_INOUT=(AL4SAN_INPUT|AL4SAN_OUTPUT);
  AL4SAN_VALUE=(1<<2);
  AL4SAN_SCRATCH=(1<<3);

  AL4SAN_PRIORITY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_LABEL=AL4SAN_OPENMP_UNDEFINED;

  AL4SAN_REGION_0=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_1=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_2=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_3=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_4=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_5=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_6=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_7=AL4SAN_OPENMP_UNDEFINED;
               //ldu region
  AL4SAN_REGION_L=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_D=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_REGION_U=AL4SAN_OPENMP_UNDEFINED;
               //flags
  AL4SAN_LOCK_TO_THREAD=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_SEQUENCE=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_THREAD_COUNT=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_LOCK_TO_THREAD_MASK=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_COLOR=AL4SAN_OPENMP_UNDEFINED;
               //data dep
  AL4SAN_NODEP=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_NOLOCALITY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_ACCUMULATOR=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_NOACCUMULATOR=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_GATHERV=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_NOGATHERV=AL4SAN_OPENMP_UNDEFINED;
     
  AL4SAN_REF=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_AFFINITY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_DONT_TRACK=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_PASSED_BY_REF=AL4SAN_OPENMP_UNDEFINED;   
                //arry
  AL4SAN_DATA_ARRAY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_DATA_MODE_ARRAY=AL4SAN_OPENMP_UNDEFINED;
               //args
  AL4SAN_CL_ARGS_NFREE=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_CL_ARGS=AL4SAN_OPENMP_UNDEFINED;

  AL4SAN_REDUX=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_COMMUTE=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_SSEND=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_ACCESS_MODE_MAX=AL4SAN_OPENMP_UNDEFINED;
               //callback
  AL4SAN_CALLBACK=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_CALLBACK_WITH_ARG=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_CALLBACK_ARG=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_OPENMP_UNDEFINED;
               //rank
  AL4SAN_EXECUTE_ON_NODE=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_EXECUTE_ON_DATA=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_EXECUTE_ON_WORKER=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_WORKER_ORDER=AL4SAN_OPENMP_UNDEFINED;

  AL4SAN_SCHED_CTX=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_HYPERVISOR_TAG=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_POSSIBLY_PARALLEL=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_FLOPS=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_TAG=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_TAG_ONLY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_NODE_SELECTION_POLICY=AL4SAN_OPENMP_UNDEFINED;
           //CUDA async option
  AL4SAN_CUDA_FLG = AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_CUDA_ASYNC = AL4SAN_OPENMP_UNDEFINED;

  AL4SAN_LOCALITY=AL4SAN_OPENMP_UNDEFINED;
  AL4SAN_FULL_TILE = AL4SAN_OPENMP_UNDEFINED;
}

void al4san_openmp_task_info(AL4SAN_Openmp_task_t* al4san_task, int *codelet_buffers, va_list varg_list)
{
  int arg_type, ptr_size;
  void *arg_ptr;
  int num_arg=0;
  va_list varg_list_copy;

  va_copy(varg_list_copy, varg_list);
  while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
  {

    if (((arg_type & AL4SAN_OPENMP_UNDEFINED_MASK)== AL4SAN_INPUT) ||
     ((arg_type & AL4SAN_OPENMP_UNDEFINED_MASK)==AL4SAN_OUTPUT) ||
     ((arg_type & AL4SAN_OPENMP_UNDEFINED_MASK)==AL4SAN_INOUT)){

      arg_ptr = va_arg(varg_list_copy, void *);
      (void*)va_arg(varg_list_copy, void *);
      ptr_size = va_arg(varg_list_copy, int);
      al4san_task->arg_depenency[num_arg]=(arg_type & AL4SAN_OPENMP_UNDEFINED_MASK);
      al4san_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      (*codelet_buffers)++;
   }
     else if ((arg_type  & AL4SAN_OPENMP_UNDEFINED_MASK) ==AL4SAN_SCRATCH){
      arg_ptr = va_arg(varg_list_copy, void *);
      ptr_size = va_arg(varg_list_copy, int);
      al4san_task->arg_depenency[num_arg]=(arg_type & AL4SAN_OPENMP_UNDEFINED_MASK);
      al4san_task->arg_size[num_arg]=ptr_size;
      num_arg++;
      (*codelet_buffers)++;
   }
  else if (arg_type==AL4SAN_VALUE)
   { 
    arg_ptr = va_arg(varg_list_copy, void *);
    ptr_size = va_arg(varg_list_copy, int);
    al4san_task->arg_depenency[num_arg]=arg_type;
    al4san_task->arg_size[num_arg]=ptr_size;
    num_arg++;
  }else{
    arg_ptr = va_arg(varg_list_copy, void *);
    ptr_size = va_arg(varg_list_copy, int);
  }
}
al4san_task->num_arg=num_arg;
va_end(varg_list_copy);

}

void al4san_openmp_pack_arg_init(struct al4san_openmp_pack_arg_data *state)
{
        state->arg_buffer = NULL;
        state->arg_buffer_size = 0;
        state->current_offset = sizeof(int);
        state->nargs = 0;
}

void al4san_openmp_pack_arg(struct al4san_openmp_pack_arg_data *state, const void *ptr, size_t ptr_size)
{
        AL4SAN_OPENMP_ASSERT_MSG(state->current_offset >= sizeof(int), "struct al4san_openmp_codelet_pack_arg has to be initialized with al4san_openmp_pack_arg_init");
        if (state->current_offset + sizeof(ptr_size) + ptr_size > state->arg_buffer_size)
        {
                if (state->arg_buffer_size == 0)
                        state->arg_buffer_size = 128 + sizeof(ptr_size) + ptr_size;
                else
                        state->arg_buffer_size = 2 * state->arg_buffer_size + sizeof(ptr_size) + ptr_size;
                AL4SAN_OPENMP_REALLOC(state->arg_buffer, state->arg_buffer_size);
        }
        memcpy(state->arg_buffer+state->current_offset, (void *)&ptr_size, sizeof(ptr_size));
        state->current_offset += sizeof(ptr_size);
        memcpy(state->arg_buffer+state->current_offset, ptr, ptr_size);
        state->current_offset += ptr_size;
        assert(state->current_offset <= state->arg_buffer_size);
        state->nargs++;
}

void al4san_openmp_pack_arg_fini(struct al4san_openmp_pack_arg_data *state, void **cl_arg, size_t *cl_arg_size)
{
        if (state->nargs)
        {
                memcpy(state->arg_buffer, &state->nargs, sizeof(state->nargs));
        }
        else
        {
                free(state->arg_buffer);
                state->arg_buffer = NULL;
        }

        *cl_arg = state->arg_buffer;
        *cl_arg_size = state->arg_buffer_size;
}

int al4san_openmp_task_create(al4san_openmp_codelet* codelet,  kmp_task_t *task, kmp_int32 gtid, va_list varg_list)
{

kmp_depend_info_t dep_info[NUM_SHAREDS];
   int arg_dep;
   void * ptr;
   int arg_size;
  int codelet_buffers=0;
   struct al4san_openmp_pack_arg_data state;
   al4san_openmp_pack_arg_init(&state);
   int dep_count=0;
  task->shareds->pcounter=0;
  task->shareds->wkcounter=0;

  AL4SAN_Openmp_task_t al4san_task;
  al4san_openmp_task_info(&al4san_task, &codelet_buffers, varg_list);
  arg_dep=AL4SAN_VALUE;
  ptr= (void *)&al4san_task;
  arg_size=sizeof(AL4SAN_Openmp_task_t);
  al4san_openmp_pack_arg(&state, ptr, arg_size);
   

while ((arg_dep=va_arg(varg_list, int))!=0)
{ 
      if((arg_dep & AL4SAN_OPENMP_UNDEFINED_MASK)==AL4SAN_INPUT )
       {
         ptr=va_arg(varg_list, void *);
         (void*)va_arg(varg_list, void *);
         arg_size=va_arg(varg_list, int);
        dep_info[dep_count].base_addr=ptr;
        dep_info[dep_count].len=arg_size;
        dep_info[dep_count].flags.in = 1;
        dep_info[dep_count].flags.out = 0;
        dep_count++;

        task->shareds->ptr[task->shareds->pcounter]=ptr;
        task->shareds->pcounter++;
       }
      else if((arg_dep & AL4SAN_OPENMP_UNDEFINED_MASK)==AL4SAN_OUTPUT)
       {
        ptr=va_arg(varg_list, void *);
        (void*)va_arg(varg_list, void *);
        arg_size=va_arg(varg_list, int);
        dep_info[dep_count].base_addr=ptr;
        dep_info[dep_count].len=arg_size;
        dep_info[dep_count].flags.in = 0;
        dep_info[dep_count].flags.out = 1;
        dep_count++;

        task->shareds->ptr[task->shareds->pcounter]=ptr;
       task->shareds->pcounter++;
       }
     else if((arg_dep& AL4SAN_OPENMP_UNDEFINED_MASK) ==AL4SAN_INOUT)
       {
        ptr=va_arg(varg_list, void *);
        (void*)va_arg(varg_list, void *);
        arg_size=va_arg(varg_list, int);
        dep_info[dep_count].base_addr=ptr;
        dep_info[dep_count].len=arg_size;
        dep_info[dep_count].flags.in = 1;
        dep_info[dep_count].flags.out = 1;
        dep_count++;

        task->shareds->ptr[task->shareds->pcounter]=ptr;
       task->shareds->pcounter++;
       }
     else if((arg_dep& AL4SAN_OPENMP_UNDEFINED_MASK) ==AL4SAN_SCRATCH)
       {
           ptr=va_arg(varg_list, void *);
           arg_size=va_arg(varg_list, int);
      if (ptr==NULL)
          {
          ptr=malloc(arg_size);
          task->shareds->workspace[task->shareds->wkcounter]=ptr;
          task->shareds->workflage[task->shareds->wkcounter]=1;
          task->shareds->wkcounter++;
          }
      else{
      task->shareds->workspace[task->shareds->wkcounter]=ptr;
      task->shareds->workflage[task->shareds->wkcounter]=0;
       task->shareds->wkcounter++;
          }
       }
     else if(arg_dep==AL4SAN_VALUE){
       ptr=va_arg(varg_list, void *);
       arg_size=va_arg(varg_list, int);
       al4san_openmp_pack_arg(&state, ptr, arg_size);
   }
   else if(arg_dep==AL4SAN_PRIORITY  ||
           arg_dep==AL4SAN_LABEL     ||
           arg_dep==AL4SAN_OPENMP_UNDEFINED)
    {
             ptr = va_arg(varg_list, void *);
             arg_size  = va_arg(varg_list, int);
   }
  else if(arg_dep!=AL4SAN_PRIORITY  &&
          arg_dep!=AL4SAN_LABEL     &&
          arg_dep!=AL4SAN_OPENMP_UNDEFINED)
           {
   fprintf(stderr,"Unrecognized argument, did you perhaps forget to end arguments with ARG_END?\n");
   abort();
     }
  else{
       ptr=va_arg(varg_list, void *);
       arg_size=va_arg(varg_list, int);
  }
}
if (state.nargs>0)
{
 al4san_openmp_pack_arg_fini(&state, &task->cl_arg, &task->cl_arg_size);
}
 task->routine=codelet;
 __kmpc_omp_task_with_deps(NULL,gtid,task, dep_count,&dep_info[0],0,NULL);

  return AL4SAN_SUCCESS;

}

int AL4SAN_Openmp_insert_task(AL4SAN_codelet codelet, AL4SAN_option_t *options, va_list varg_list)
{
    int ret;
    kmp_int32 gtid = __kmpc_global_thread_num(NULL);
    kmp_task_t *task = __kmpc_omp_task_alloc(NULL, gtid, 1, sizeof(kmp_task_t), sizeof(struct shar), codelet);
    
    ret=al4san_openmp_task_create((al4san_openmp_codelet*)codelet, task, gtid, varg_list);

  return AL4SAN_SUCCESS;
}

int AL4SAN_Openmp_insert_task_v2(AL4SAN_sequence_t *sequence, AL4SAN_codelet codelet, va_list varg_list)
{
    int ret;
    kmp_int32 gtid = __kmpc_global_thread_num(NULL);
    kmp_task_t *task = __kmpc_omp_task_alloc(NULL, gtid, 1, sizeof(kmp_task_t), sizeof(struct shar), codelet);
    
    ret=al4san_openmp_task_create((al4san_openmp_codelet*)codelet, task, gtid, varg_list);

  return AL4SAN_SUCCESS;
}

int AL4SAN_Openmp_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list)
{
    AL4SAN_Openmp_arg_list* openmparg= al4san_arg->Openmp_arg_list;

      char *cl_arg = (char *) openmparg->task->cl_arg;
      pshareds * shar=openmparg->task->shareds;
      AL4SAN_Openmp_task_t al4san_task;
      int ptr_index=0;
      int wk_index=0;

        size_t current_arg_offset = 0;
        int nargs, arg;
        void *argptr;
        void **tmp_ref;
        size_t arg_size;
        /* We fill the different pointers with the appropriate arguments */
        memcpy(&nargs, cl_arg, sizeof(nargs));
        current_arg_offset += sizeof(nargs);


        memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
        current_arg_offset += sizeof(arg_size);

       memcpy(&al4san_task, cl_arg+current_arg_offset, arg_size);
       current_arg_offset += arg_size;

  for (int i=0;i<al4san_task.num_arg;i++)
  {
   if (al4san_task.arg_depenency[i]==AL4SAN_INPUT  ||
     al4san_task.arg_depenency[i]==AL4SAN_OUTPUT ||
     al4san_task.arg_depenency[i]==AL4SAN_INOUT)
   {
     tmp_ref = va_arg(varg_list, void**);
     *tmp_ref = (void*)shar->ptr[ptr_index];
     ptr_index++;
   }
   else if(al4san_task.arg_depenency[i]==AL4SAN_SCRATCH)
   {
     tmp_ref = va_arg(varg_list, void**);
     *tmp_ref = (void*)shar->workspace[wk_index];
     wk_index++;
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
  return AL4SAN_SUCCESS;
}

int al4san_openmp_workspace_destroy(AL4SAN_arg_list *al4san_arg)
{
      AL4SAN_Openmp_arg_list* openmparg= al4san_arg->Openmp_arg_list;

      char *cl_arg = (char *) openmparg->task->cl_arg;
      pshareds * shar=openmparg->task->shareds;
      AL4SAN_Openmp_task_t al4san_task;
      int wk_index=0;

        size_t current_arg_offset = 0;
        int nargs, arg;
        void *argptr;
        void **tmp_ref;
        size_t arg_size;
        /* We fill the different pointers with the appropriate arguments */
        memcpy(&nargs, cl_arg, sizeof(nargs));
        current_arg_offset += sizeof(nargs);


        memcpy(&arg_size, cl_arg+current_arg_offset, sizeof(arg_size));
        current_arg_offset += sizeof(arg_size);

       memcpy(&al4san_task, cl_arg+current_arg_offset, arg_size);
       current_arg_offset += arg_size;

  for (int i=0;i<al4san_task.num_arg;i++)
  {
   if (al4san_task.arg_depenency[i]==AL4SAN_SCRATCH && shar->workflage[wk_index])
   {
    void * work=shar->workspace[wk_index];
     free(work);
     wk_index++;
   }
}
  return AL4SAN_SUCCESS;
}
