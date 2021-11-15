/**
 *
 * @file runtime_task.c
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **
 *
 * @brief AL4SAN Quark Task routines
 *  AL4SAN Task
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "al4san_quark.h"
#include "quark_task.h"
#include <stdarg.h>
//#include <quark.c>

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

void AL4SAN_Quark_task_option_init()
{

     AL4SAN_DEP=101;
     ARG_END = 0;
     
     AL4SAN_INPUT=QUARK_INPUT;
     AL4SAN_OUTPUT=QUARK_OUTPUT;
     AL4SAN_INOUT=QUARK_INOUT;
     AL4SAN_VALUE=QUARK_VALUE;
     AL4SAN_NODEP=NODEP;
     AL4SAN_SCRATCH=QUARK_SCRATCH;
     AL4SAN_LOCALITY=LOCALITY;
     AL4SAN_NOLOCALITY=NOLOCALITY; 
     AL4SAN_ACCUMULATOR=ACCUMULATOR; 
     AL4SAN_NOACCUMULATOR=NOACCUMULATOR;
     AL4SAN_GATHERV=GATHERV;
     AL4SAN_NOGATHERV=NOGATHERV;


     AL4SAN_REGION_0=QUARK_REGION_0; 
     AL4SAN_REGION_1=QUARK_REGION_1; 
     AL4SAN_REGION_2=QUARK_REGION_2; 
     AL4SAN_REGION_3=QUARK_REGION_3;
     AL4SAN_REGION_4=QUARK_REGION_4; 
     AL4SAN_REGION_5=QUARK_REGION_5; 
     AL4SAN_REGION_6=QUARK_REGION_6; 
     AL4SAN_REGION_7=QUARK_REGION_7;


     AL4SAN_REGION_L=QUARK_REGION_0|QUARK_REGION_1|QUARK_REGION_2;
     AL4SAN_REGION_D=QUARK_REGION_3|QUARK_REGION_4;
     AL4SAN_REGION_U=QUARK_REGION_5|QUARK_REGION_6|QUARK_REGION_7;

     AL4SAN_PRIORITY=TASK_PRIORITY;
     AL4SAN_LOCK_TO_THREAD=TASK_LOCK_TO_THREAD;
     AL4SAN_SEQUENCE=TASK_SEQUENCE;
     AL4SAN_THREAD_COUNT=TASK_THREAD_COUNT;
     AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=THREAD_SET_TO_MANUAL_SCHEDULING;
     AL4SAN_LOCK_TO_THREAD_MASK=TASK_LOCK_TO_THREAD_MASK;
     AL4SAN_LABEL=TASK_LABEL;
     AL4SAN_COLOR=TASK_COLOR;

     AL4SAN_DATA_ARRAY=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_DATA_MODE_ARRAY=AL4SAN_QUARK_UNDEFINED;
               //args
     AL4SAN_CL_ARGS_NFREE=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_CL_ARGS=AL4SAN_QUARK_UNDEFINED;

     AL4SAN_REDUX=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_COMMUTE=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_SSEND=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_ACCESS_MODE_MAX=AL4SAN_QUARK_UNDEFINED;
               //callback
     AL4SAN_CALLBACK=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_CALLBACK_WITH_ARG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_CALLBACK_ARG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_PROLOGUE_CALLBACK=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_QUARK_UNDEFINED;
               //rank
     AL4SAN_EXECUTE_ON_NODE=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_EXECUTE_ON_DATA=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_EXECUTE_ON_WORKER=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_WORKER_ORDER=AL4SAN_QUARK_UNDEFINED;

     AL4SAN_SCHED_CTX=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_HYPERVISOR_TAG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_POSSIBLY_PARALLEL=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_FLOPS=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_TAG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_TAG_ONLY=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_NODE_SELECTION_POLICY=AL4SAN_QUARK_UNDEFINED;
     
               //parsec
     AL4SAN_REF=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_AFFINITY=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_DONT_TRACK=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_PASSED_BY_REF=AL4SAN_QUARK_UNDEFINED;
     
               //CUDA async option not defined in quark
     AL4SAN_CUDA_FLG=AL4SAN_QUARK_UNDEFINED;
     AL4SAN_CUDA_ASYNC = AL4SAN_QUARK_UNDEFINED;
     AL4SAN_FULL_TILE = AL4SAN_QUARK_UNDEFINED;
}

void al4san_quark_task_info(AL4SAN_Quark_task_t *al4san, va_list varg_list){

  int arg_type;
  int arg_size;
  void *arg_ptr;
  int mask;
  int narg=0;

  va_list varg_list_copy; 
  va_copy(varg_list_copy, varg_list);
  
  while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
  { 
    quark_direction_t arg_direction = (quark_direction_t) ((arg_type & AL4SAN_QUARK_UNDEFINED_MASK) & QUARK_DIRECTION_BITMASK);
    if( arg_direction==AL4SAN_INPUT || 
      arg_direction==AL4SAN_OUTPUT || 
      arg_direction==AL4SAN_INOUT ) 
    {
     arg_ptr = va_arg(varg_list_copy, void *);
     (void *)va_arg(varg_list_copy, void *);
     arg_size = va_arg(varg_list_copy, int);
     al4san->arg_depenency[narg]=arg_direction;
     al4san->arg_size[narg]=sizeof(char *);
     narg++;
    }
   else if( arg_type==AL4SAN_SCRATCH ||
     arg_type==AL4SAN_NODEP){
     arg_ptr = va_arg(varg_list_copy, void *);
     arg_size = va_arg(varg_list_copy, int);
     al4san->arg_depenency[narg]=arg_type;
     al4san->arg_size[narg]=sizeof(char *);
   narg++;
   }
 else if( arg_type==AL4SAN_VALUE ) {
  mask=( arg_type & QUARK_QUARK_VALUE_FLAGS_BITMASK );
  arg_ptr = va_arg(varg_list_copy, void *);
  arg_size = va_arg(varg_list_copy, int);
  if(mask==0){
   al4san->arg_depenency[narg]=arg_type;
   al4san->arg_size[narg]=arg_size;
   narg++;
      }
   }
 else{
   arg_ptr = va_arg(varg_list_copy, void *);
   arg_size = va_arg(varg_list_copy, int);
 }  
}
al4san->num_arg=narg;

va_end(varg_list_copy);

}

int al4san_quark_task_create(Quark *quark, Quark_Task *task, va_list varg_list, Quark_Task_Flags *task_flags){
  int arg_type;
  int arg_size;
  void *arg_ptr;
  int narg=0;
  int mask;
  int nflags=0;

  AL4SAN_Quark_task_t al4san;
  al4san_quark_task_info(&al4san, varg_list); 

  arg_type=AL4SAN_VALUE;
  arg_ptr= (void *)&al4san;
  arg_size=sizeof(AL4SAN_Quark_task_t);
  QUARK_Task_Pack_Arg( quark, task, arg_size, arg_ptr, arg_type);

  while((arg_type = va_arg(varg_list, int)) != ARG_END){
    quark_direction_t arg_direction = (quark_direction_t) ((arg_type & AL4SAN_QUARK_UNDEFINED_MASK) & QUARK_DIRECTION_BITMASK);
    if( arg_direction==AL4SAN_INPUT || arg_direction==AL4SAN_OUTPUT || arg_direction==AL4SAN_INOUT ) {

       arg_ptr = va_arg(varg_list, void *);
       (void*)va_arg(varg_list, void *);
       arg_size = va_arg(varg_list, int);
       QUARK_Task_Pack_Arg( quark, task, AL4SAN_DEP, arg_ptr, (arg_type & AL4SAN_QUARK_UNDEFINED_MASK));
    }
   else if (arg_type==AL4SAN_VALUE || arg_type==AL4SAN_SCRATCH || arg_type==AL4SAN_NODEP)
   {
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Pack_Arg( quark, task, arg_size, arg_ptr, arg_type);
   }
        //Task flags (add support to task option inside insert task API)
  else if (arg_type==AL4SAN_PRIORITY){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_PRIORITY, (intptr_t)arg_ptr);
      nflags++;
   }
  else if (arg_type==AL4SAN_LOCK_TO_THREAD){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_LOCK_TO_THREAD, (intptr_t)arg_ptr);
      nflags++;
   }
  else if (arg_type==AL4SAN_SEQUENCE){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_SEQUENCE, (intptr_t)arg_ptr);
      nflags++;
   }
  else if (arg_type==AL4SAN_THREAD_COUNT){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_THREAD_COUNT, (intptr_t)arg_ptr);
      nflags++;
   }
  else if (arg_type==AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING, (intptr_t)arg_ptr);
      nflags++;
   }
  else if (arg_type==AL4SAN_LOCK_TO_THREAD_MASK){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_LOCK_TO_THREAD_MASK, (intptr_t)strdup((unsigned char *)arg_ptr));
      nflags++;
  }
  else if (arg_type==AL4SAN_LABEL){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_LABEL, (intptr_t)strdup((char *)arg_ptr));
      nflags++;
 }
 else if (arg_type==AL4SAN_COLOR){
      arg_ptr = va_arg(varg_list, void *);
      arg_size = va_arg(varg_list, int);
      QUARK_Task_Flag_Set(task_flags, AL4SAN_COLOR, (intptr_t)strdup((char *)arg_ptr));
      nflags++;
  }
else if (arg_type!=AL4SAN_QUARK_UNDEFINED){
   fprintf(stderr,"Unrecognized argument, did you perhaps forget to end arguments with ARG_END?\n");
   abort();
 }

 }

if (nflags!=0){
  quark_set_task_flags_in_task_structure( quark, task, task_flags );
}

return 0;
}


//void al4san_insert_task( void (*function)(Quark *), const AL4SAN_option_t *options, ...) 
/*void al4san_insert_task( al4san_quark_codelet* codelet, AL4SAN_option_t *options, ...) 
{
  va_list varg_list;
  int ret;
  unsigned long long taskid;
  Quark_Task *task;

  runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);         
  
  va_start(varg_list, options);

  task = QUARK_Task_Init(opt->quark, codelet, (Quark_Task_Flags*)opt);
  
  ret=al4san_task_create(opt->quark, task, varg_list, (Quark_Task_Flags*)opt);
  int arg_type, arg_size;
  void *arg_ptr;
  va_end(varg_list);

  taskid = QUARK_Insert_Task_Packed(opt->quark, task );

}*/

int AL4SAN_Quark_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list)
{

  unsigned long long taskid;
  Quark_Task *task;
  int ret;

  runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);

  task = QUARK_Task_Init(opt->quark, (al4san_quark_codelet*)codelet, (Quark_Task_Flags*)opt);

  ret=al4san_quark_task_create(opt->quark, task, varg_list, (Quark_Task_Flags*)opt);

  taskid = QUARK_Insert_Task_Packed(opt->quark, task );
  
  return AL4SAN_SUCCESS;
}

/*void al4san_unpack_arg(AL4SAN_Quark_arg_list *al4san_arg, ...)
{
  va_list varg_list;
  va_start(varg_list, al4san_arg);
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( al4san_arg->quark ); 
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  AL4SAN_Quark_task_t al4san;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&al4san, arg, sizeof(AL4SAN_Quark_task_t));
  for (int i=0;i<al4san.num_arg;i++)
  {
    if (al4san.arg_depenency[i]==AL4SAN_INPUT || 
     al4san.arg_depenency[i]==AL4SAN_OUTPUT || 
     al4san.arg_depenency[i]==AL4SAN_INOUT  || 
     al4san.arg_depenency[i]==AL4SAN_SCRATCH)
    { 
      arg=QUARK_Args_Pop(args_list, &lastarg);
      arg_ptr=va_arg(varg_list, void*);
      memcpy(arg_ptr, arg,  sizeof(char*));
    }
    else if(al4san.arg_depenency[i]==AL4SAN_VALUE)
    { 
     arg=QUARK_Args_Pop(args_list, &lastarg);
     arg_val=va_arg(varg_list, void*);
     memcpy(arg_val, arg, al4san.arg_size[i]);
   }
 }
 va_end(varg_list);
}*/

int AL4SAN_Quark_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list)
{
  AL4SAN_Quark_arg_list* quarkarg= al4san_arg->Quark_arg_list;
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( quarkarg->quark );
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  AL4SAN_Quark_task_t al4san;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&al4san, arg, sizeof(AL4SAN_Quark_task_t));
  for (int i=0;i<al4san.num_arg;i++)
  {
    if (al4san.arg_depenency[i]==AL4SAN_INPUT ||
     al4san.arg_depenency[i]==AL4SAN_OUTPUT ||
     al4san.arg_depenency[i]==AL4SAN_INOUT  ||
     al4san.arg_depenency[i]==AL4SAN_SCRATCH || 
     al4san.arg_depenency[i]==AL4SAN_NODEP)
    {
      arg=QUARK_Args_Pop(args_list, &lastarg);
      arg_ptr=va_arg(varg_list, void*);
      memcpy(arg_ptr, arg,  sizeof(char*));
    }
    else if(al4san.arg_depenency[i]==AL4SAN_VALUE)
    {
     arg=QUARK_Args_Pop(args_list, &lastarg);
     arg_val=va_arg(varg_list, void*);
     memcpy(arg_val, arg, al4san.arg_size[i]);
   }
 }
}

