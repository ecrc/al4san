/**
 *
 * @file runtime_task.c
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief AL4SAN Quark Task routines
 *  AL4SAN Task
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include "al4san_runtime.h"
#include "runtime_task.h"
#include <stdarg.h>
//#include <quark.c>

void al4san_task_info(AL4SAN_task_t *al4san, va_list varg_list){

  int arg_type;
  int arg_size;
  void *arg_ptr;
  int mask;
  int narg=0;

  va_list varg_list_copy; 
  va_copy(varg_list_copy, varg_list);
  
  while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
  { 
    arg_ptr = va_arg(varg_list_copy, void *);
    arg_size = va_arg(varg_list_copy, int);
    quark_direction_t arg_direction = (quark_direction_t) ((arg_type & AL4SAN_UNDEFINED_MASK) & QUARK_DIRECTION_BITMASK);
    if( arg_direction==AL4SAN_INPUT || 
      arg_direction==AL4SAN_OUTPUT || 
      arg_direction==AL4SAN_INOUT ) 
    {
     al4san->arg_depenency[narg]=arg_direction;
     al4san->arg_size[narg]=sizeof(char *);
     narg++;
    }
   else if( arg_type==AL4SAN_SCRATCH ||
     arg_type==AL4SAN_NODEP){
     al4san->arg_depenency[narg]=arg_type;
   al4san->arg_size[narg]=sizeof(char *);
   narg++;
   }
 else if( arg_type==AL4SAN_VALUE ) {
  mask=( arg_type & QUARK_VALUE_FLAGS_BITMASK );
  if(mask==0){
   al4san->arg_depenency[narg]=arg_type;
   al4san->arg_size[narg]=arg_size;
   narg++;
      }
   }  
}
al4san->num_arg=narg;

va_end(varg_list_copy);

}

int al4san_task_create(Quark *quark, Quark_Task *task, va_list varg_list, Quark_Task_Flags *task_flags){
  int arg_type;
  int arg_size;
  void *arg_ptr;
  int narg=0;
  int mask;
  int nflags=0;

  AL4SAN_task_t al4san;
  al4san_task_info(&al4san, varg_list); 

  arg_type=AL4SAN_VALUE;
  arg_ptr= (void *)&al4san;
  arg_size=sizeof(AL4SAN_task_t);

  do{
    quark_direction_t arg_direction = (quark_direction_t) ((arg_type & AL4SAN_UNDEFINED_MASK) & QUARK_DIRECTION_BITMASK);
    if( arg_direction==AL4SAN_INPUT || arg_direction==AL4SAN_OUTPUT || arg_direction==AL4SAN_INOUT ) {
   //  QUARK_Task_Pack_Arg( quark, task, arg_size, arg_ptr, (arg_type & AL4SAN_UNDEFINED_MASK));
       QUARK_Task_Pack_Arg( quark, task, AL4SAN_DEP, arg_ptr, (arg_type & AL4SAN_UNDEFINED_MASK));
    }
   else if (arg_type==AL4SAN_VALUE || arg_type==AL4SAN_SCRATCH || arg_type==AL4SAN_NODEP)
   {
    QUARK_Task_Pack_Arg( quark, task, arg_size, arg_ptr, arg_type);
   }
        //Task flags (add support to task option inside insert task API)
  else if (arg_type==AL4SAN_PRIORITY){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_PRIORITY, (intptr_t)arg_ptr);
   nflags++;
   }
  else if (arg_type==AL4SAN_LOCK_TO_THREAD){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_LOCK_TO_THREAD, (intptr_t)arg_ptr);
   nflags++;
   }
  else if (arg_type==AL4SAN_SEQUENCE){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_SEQUENCE, (intptr_t)arg_ptr);
   nflags++;
   }
  else if (arg_type==AL4SAN_THREAD_COUNT){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_THREAD_COUNT, (intptr_t)arg_ptr);
   nflags++;
   }
  else if (arg_type==AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING, (intptr_t)arg_ptr);
   nflags++;
   }
  else if (arg_type==AL4SAN_LOCK_TO_THREAD_MASK){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_LOCK_TO_THREAD_MASK, (intptr_t)strdup((unsigned char *)arg_ptr));
   nflags++;
  }
  else if (arg_type==AL4SAN_LABEL){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_LABEL, (intptr_t)strdup((char *)arg_ptr));
   nflags++;
 }
 else if (arg_type==AL4SAN_COLOR){
   QUARK_Task_Flag_Set(task_flags, AL4SAN_COLOR, (intptr_t)strdup((char *)arg_ptr));
   nflags++;
  }
else if (arg_type!=AL4SAN_UNDEFINED){
   fprintf(stderr,"Unrecognized argument, did you perhaps forget to end arguments with ARG_END?\n");
   abort();
 }

 arg_type = va_arg(varg_list, int); 
 arg_ptr = va_arg(varg_list, void *);
 arg_size = va_arg(varg_list, int);
}while (arg_type != ARG_END);

if (nflags!=0){
  quark_set_task_flags_in_task_structure( quark, task, task_flags );
}

return 0;
}


//void al4san_insert_task( void (*function)(Quark *), const AL4SAN_option_t *options, ...) 
void al4san_insert_task( al4san_codelet* codelet, AL4SAN_option_t *options, ...) 
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

}

int AL4SAN_Runtime_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list varg_list)
{

  unsigned long long taskid;
  Quark_Task *task;
  int ret;

  runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);

  task = QUARK_Task_Init(opt->quark, (al4san_codelet*)codelet, (Quark_Task_Flags*)opt);

  ret=al4san_task_create(opt->quark, task, varg_list, (Quark_Task_Flags*)opt);

  taskid = QUARK_Insert_Task_Packed(opt->quark, task );
  
  return AL4SAN_SUCCESS;
}

void al4san_unpack_arg(AL4SAN_arg_list *al4san_arg, ...)
{
  va_list varg_list;
  va_start(varg_list, al4san_arg);
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( al4san_arg->quark ); 
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  AL4SAN_task_t al4san;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&al4san, arg, sizeof(AL4SAN_task_t));
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
}

int AL4SAN_Runtime_unpack_arg(AL4SAN_arg args, va_list varg_list)
{
  AL4SAN_arg_list* al4san_arg= (AL4SAN_arg_list*)args;
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( al4san_arg->quark );
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  AL4SAN_task_t al4san;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&al4san, arg, sizeof(AL4SAN_task_t));
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
}

