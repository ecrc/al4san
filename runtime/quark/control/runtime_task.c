/**
 *
 * @file runtime_task.c
 *
 * @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief ALTANAL Quark Task routines
 *  ALTANAL Task
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
#include "runtime_task.h"
#include <stdarg.h>
#include <quark.c>

void altanal_task_info(ALTANAL_task_t *altanal, va_list varg_list){
  
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
//          quark_direction_t arg_direction = (quark_direction_t) (arg_type & QUARK_DIRECTION_BITMASK);
    switch ( arg_type ) {
      case VALUE:
      mask=( arg_type & QUARK_VALUE_FLAGS_BITMASK );
      if(mask==0){
       altanal->arg_depenency[narg]=arg_type;
       altanal->arg_size[narg]=arg_size;
       narg++;
     }  
     break;
     case INPUT:
     case OUTPUT:
     case INOUT:
     case SCRATCH:
     case NODEP:
     altanal->arg_depenency[narg]=arg_type;
     altanal->arg_size[narg]=sizeof(char *);
     narg++;
     break; 
   }
 }
 altanal->num_arg=narg;

 va_end(varg_list_copy);

}
int altanal_task_create(Quark *quark, Quark_Task *task, va_list varg_list, Quark_Task_Flags *task_flags){
  int arg_type;
  int arg_size;
  void *arg_ptr;
  int narg=0;
  int mask;
  int nflags=0;

  ALTANAL_task_t altanal;
  altanal_task_info(&altanal, varg_list); 
/*
    va_list varg_list_copy; 
    ALTANAL_task_t altanal;
     va_copy(varg_list_copy, varg_list);
       
      while ((arg_type = va_arg(varg_list_copy, int))!=ARG_END)
        { 
          arg_ptr = va_arg(varg_list_copy, void *);
          arg_size = va_arg(varg_list_copy, int);
          quark_direction_t arg_direction = (quark_direction_t) (arg_type & QUARK_DIRECTION_BITMASK);
          switch ( arg_direction ) {
            case VALUE:
            mask=( arg_type & QUARK_VALUE_FLAGS_BITMASK );
              if(mask==0){
                 altanal.arg_depenency[narg]=arg_type;
                 altanal.arg_size[narg]=arg_size;
                 narg++;
               }  
            break;
            case INPUT:
            case OUTPUT:
            case INOUT:
            case SCRATCH:
            case NODEP:
                 altanal.arg_depenency[narg]=arg_type;
                 altanal.arg_size[narg]=sizeof(char *);
                 narg++;
            break; 
           }
      }
     va_end(varg_list_copy);

    altanal.num_arg=narg;
*/
  arg_type=ALTANAL_VALUE;
  arg_ptr= (void *)&altanal;
  arg_size=sizeof(ALTANAL_task_t);

  do{
    switch ( arg_type ) {
     case VALUE:
     case INPUT:
     case OUTPUT:
     case INOUT:
     case SCRATCH:
     case NODEP:
     QUARK_Task_Pack_Arg( quark, task, arg_size, arg_ptr, arg_type);
     break;

        //Task flags (add support to task option inside insert task API)
     case ALTANAL_PRIORITY:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_PRIORITY, (int *)arg_ptr);
     nflags++;
     break;
     case ALTANAL_LOCK_TO_THREAD:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_LOCK_TO_THREAD, (int *)arg_ptr);
     nflags++;
     break;
     case ALTANAL_SEQUENCE:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_SEQUENCE, (Quark_Sequence **)arg_ptr);
     nflags++;
     break;
     case ALTANAL_THREAD_COUNT:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_THREAD_COUNT, (int *)arg_ptr);
     nflags++;
     break;
     case ALTANAL_THREAD_SET_TO_MANUAL_SCHEDULING:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_THREAD_SET_TO_MANUAL_SCHEDULING, (int *)arg_ptr);
     nflags++;
     break;
     case ALTANAL_LOCK_TO_THREAD_MASK:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_LOCK_TO_THREAD_MASK, strdup((unsigned char *)arg_ptr));
     nflags++;
     break;
     case ALTANAL_LABEL:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_LABEL, strdup((char *)arg_ptr));
     nflags++;
     break;   
     case ALTANAL_COLOR:
     QUARK_Task_Flag_Set(task_flags, ALTANAL_COLOR, strdup((char *)arg_ptr));
     nflags++;
     break;
     case ALTANAL_undefined:
     break;     
     
     default:
     fprintf(stderr,"Unrecognized argument, did you perhaps forget to end arguments with ARG_END?\n");
     abort();
     break;
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


//void altanal_insert_task( void (*function)(Quark *), const ALTANAL_option_t *options, ...) 
void altanal_insert_task( altanal_codelet* codelet, ALTANAL_option_t *options, ...) 
{
  va_list varg_list;
  int ret;
  unsigned long long taskid;
  Quark_Task *task;

  runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);         
  
  va_start(varg_list, options);

  task = QUARK_Task_Init(opt->quark, codelet, (Quark_Task_Flags*)opt);
  
  ret=altanal_task_create(opt->quark, task, varg_list, (Quark_Task_Flags*)opt);
  int arg_type, arg_size;
  void *arg_ptr;
  va_end(varg_list);


  taskid = QUARK_Insert_Task_Packed(opt->quark, task );

}

int ALTANAL_Runtime_insert_task(ALTANAL_codelet codelet,  ALTANAL_option_t *options, va_list varg_list)
{
  
  unsigned long long taskid;
  Quark_Task *task;
  int ret;

  runtime_internal_t *opt = (runtime_internal_t*)(options->schedopt);

  task = QUARK_Task_Init(opt->quark, (altanal_codelet*)codelet, (Quark_Task_Flags*)opt);

  ret=altanal_task_create(opt->quark, task, varg_list, (Quark_Task_Flags*)opt);

  taskid = QUARK_Insert_Task_Packed(opt->quark, task );
  
  return ALTANAL_SUCCESS;
}

void altanal_unpack_arg(ALTANAL_arg_list *altanal_arg, ...)
{
  va_list varg_list;
  va_start(varg_list, altanal_arg);
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( altanal_arg->quark ); 
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  ALTANAL_task_t altanal;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&altanal, arg, sizeof(ALTANAL_task_t));
  for (int i=0;i<altanal.num_arg;i++)
  {
    if (altanal.arg_depenency[i]==ALTANAL_INPUT || 
     altanal.arg_depenency[i]==ALTANAL_OUTPUT || 
     altanal.arg_depenency[i]==ALTANAL_INOUT  || 
     altanal.arg_depenency[i]==ALTANAL_SCRATCH)
    { 
      arg=QUARK_Args_Pop(args_list, &lastarg);
      arg_ptr=va_arg(varg_list, void*);
      memcpy(arg_ptr, arg,  sizeof(char*));
    }
    else if(altanal.arg_depenency[i]==ALTANAL_VALUE)
    { 
     arg=QUARK_Args_Pop(args_list, &lastarg);
     arg_val=va_arg(varg_list, void*);
     memcpy(arg_val, arg, altanal.arg_size[i]);
   }
 }
 va_end(varg_list);
}

int ALTANAL_Runtime_unpack_arg(ALTANAL_arg args, va_list varg_list)
{
  ALTANAL_arg_list* altanal_arg= (ALTANAL_arg_list*)args;
  void *lastarg=NULL;
  void *args_list = QUARK_Args_List( altanal_arg->quark );
  void *arg;
  void *arg_ptr;
  void *arg_val;
  int unpack_type;
  ALTANAL_task_t altanal;

  arg=QUARK_Args_Pop(args_list, &lastarg);
  memcpy(&altanal, arg, sizeof(ALTANAL_task_t));
  for (int i=0;i<altanal.num_arg;i++)
  {
    if (altanal.arg_depenency[i]==ALTANAL_INPUT ||
     altanal.arg_depenency[i]==ALTANAL_OUTPUT ||
     altanal.arg_depenency[i]==ALTANAL_INOUT  ||
     altanal.arg_depenency[i]==ALTANAL_SCRATCH)
    {
      arg=QUARK_Args_Pop(args_list, &lastarg);
      arg_ptr=va_arg(varg_list, void*);
      memcpy(arg_ptr, arg,  sizeof(char*));
    }
    else if(altanal.arg_depenency[i]==ALTANAL_VALUE)
    {
     arg=QUARK_Args_Pop(args_list, &lastarg);
     arg_val=va_arg(varg_list, void*);
     memcpy(arg_val, arg, altanal.arg_size[i]);
   }
 }
}

