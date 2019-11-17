/**
 *
 * @file runtime_task.c
 *
* @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 **/

/**
 *
 * @brief AL4SAN PaRSEC Task routines
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.0
 * @author Rabab Alomairy
 * @date 2018-10-18
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include "al4san_parsec.h"
#include <stdarg.h>


/*
 * This function iterates over a va_list, whose end
 * is marked by ARG_END. It expects a set
 * of three arguments, first of which has to be an int.
 * A method is passed that is called for the set with
 * accompanying data that might be needed.
 */
void AL4SAN_Parsec_task_option_init()
{

 AL4SAN_DEP = 101;
 ARG_END = PARSEC_DTD_ARG_END;
 AL4SAN_INPUT=INPUT;
 AL4SAN_OUTPUT=OUTPUT;
 AL4SAN_INOUT=INOUT;
 AL4SAN_VALUE=VALUE;
 AL4SAN_REF=REF;
 AL4SAN_SCRATCH=SCRATCH;
 AL4SAN_AFFINITY=AFFINITY; 
 AL4SAN_DONT_TRACK=DONT_TRACK; 
 AL4SAN_PASSED_BY_REF=PASSED_BY_REF;

 AL4SAN_PRIORITY=1<<30;
 AL4SAN_LABEL=1<<31;

               //Data region
 AL4SAN_REGION_0=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_1=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_2=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_3=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_4=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_5=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_6=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_7=AL4SAN_PARSEC_UNDEFINED;
               //ldu region
 AL4SAN_REGION_L=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_D=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_REGION_U=AL4SAN_PARSEC_UNDEFINED;
               //flags
 AL4SAN_LOCK_TO_THREAD=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_SEQUENCE=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_THREAD_COUNT=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_THREAD_SET_TO_MANUAL_SCHEDULING=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_LOCK_TO_THREAD_MASK=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_COLOR=AL4SAN_PARSEC_UNDEFINED;
               //data dep
 AL4SAN_NODEP=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_NOLOCALITY=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_ACCUMULATOR=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_NOACCUMULATOR=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_GATHERV=AL4SAN_PARSEC_UNDEFINED;
 AL4SAN_NOGATHERV=AL4SAN_PARSEC_UNDEFINED;
          
                //arry
  AL4SAN_DATA_ARRAY=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_DATA_MODE_ARRAY=AL4SAN_PARSEC_UNDEFINED;
               //args
  AL4SAN_CL_ARGS_NFREE=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_CL_ARGS=AL4SAN_PARSEC_UNDEFINED;

  AL4SAN_REDUX=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_COMMUTE=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_SSEND=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_ACCESS_MODE_MAX=AL4SAN_PARSEC_UNDEFINED;
               //callback
  AL4SAN_CALLBACK=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_CALLBACK_WITH_ARG=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_CALLBACK_ARG=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_ARG=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_POP=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_PROLOGUE_CALLBACK_POP_ARG=AL4SAN_PARSEC_UNDEFINED;
               //rank
  AL4SAN_EXECUTE_ON_NODE=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_EXECUTE_ON_DATA=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_EXECUTE_ON_WORKER=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_WORKER_ORDER=AL4SAN_PARSEC_UNDEFINED;

  AL4SAN_SCHED_CTX=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_HYPERVISOR_TAG=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_POSSIBLY_PARALLEL=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_FLOPS=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_TAG=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_TAG_ONLY=AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_NODE_SELECTION_POLICY=AL4SAN_PARSEC_UNDEFINED;
           //CUDA async option
  AL4SAN_CUDA_FLG = AL4SAN_PARSEC_UNDEFINED;
  AL4SAN_CUDA_ASYNC = AL4SAN_PARSEC_UNDEFINED;  
  AL4SAN_LOCALITY = AL4SAN_PARSEC_UNDEFINED;
}
int
al4san_parsec_arg_iterator(va_list args, parsec_dtd_arg_cb *cb, void *cb_data)
{
    /* We always expect three arguments to come a set */
    int arg_type, arg_size;
    void *arg_ptr;
    while(PARSEC_DTD_ARG_END != (arg_type = va_arg(args, int))) {
        arg_ptr = va_arg(args, void *);
        arg_size  = va_arg(args, int);
        
        if(((arg_type & AL4SAN_PARSEC_UNDEFINED_MASK) & GET_OP_TYPE) == AL4SAN_INPUT || 
         ((arg_type & AL4SAN_PARSEC_UNDEFINED_MASK) & GET_OP_TYPE) == AL4SAN_INOUT || 
         ((arg_type & AL4SAN_PARSEC_UNDEFINED_MASK) & GET_OP_TYPE) == AL4SAN_OUTPUT )
          {
             arg_size=PASSED_BY_REF;
             cb(arg_size, arg_ptr, (arg_type & AL4SAN_PARSEC_UNDEFINED_MASK), cb_data);
            
          }
        else if(arg_type==AL4SAN_VALUE   || 
            arg_type==AL4SAN_REF         || 
            arg_type==AL4SAN_SCRATCH     || 
            arg_type==AL4SAN_AFFINITY    ||
            arg_type==AL4SAN_DONT_TRACK  ||
            arg_type==AL4SAN_PASSED_BY_REF)
           {
            cb(arg_size, arg_ptr, arg_type, cb_data);
           }
        else if(arg_type!=AL4SAN_PRIORITY  && 
            arg_type!=AL4SAN_LABEL     && 
            arg_type!=AL4SAN_PARSEC_UNDEFINED)
           {
            parsec_fatal("Unrecognized argument, did you perhaps forget to end arguments with ARG_END?\n");
           }
        
    }
    return 1;
}
void al4san_parsec_flags_decode(AL4SAN_Parsec_Task_Flags *task_flag, va_list args_for_flags)
{
         /* We always expect three arguments to come a set */
    int arg_type, arg_size;
    void *arg_ptr;
    
       /* flag init */
    task_flag->task_priority=0;
    task_flag->task_label=NULL;
    
    while(PARSEC_DTD_ARG_END != (arg_type = va_arg(args_for_flags, int))) {
        arg_ptr = va_arg(args_for_flags, void *);
        arg_size  = va_arg(args_for_flags, int);

        if( arg_type == AL4SAN_PRIORITY)
          {
           task_flag->task_priority=(intptr_t)arg_ptr;
          }
       else if(arg_type == AL4SAN_LABEL)
         {
           task_flag->task_label=(char *)arg_ptr;
         }
   }
   return;
}

/*void  al4san_insert_task( al4san_parsec_codelet *codelet,  AL4SAN_option_t *options, ...) 
{
    //Defines a DAG of tasks
    parsec_taskpool_t *tp = (parsec_taskpool_t *) (options->sequence->schedopt);
    
    parsec_dtd_taskpool_t *dtd_tp = (parsec_dtd_taskpool_t *)tp;

    if( tp->context == NULL ) {
        parsec_fatal( "Sorry! You can not insert task wihtout enqueuing the taskpool to parsec_context"
          " first. Please make sure you call parsec_enqueue(parsec_context, taskpool) before"
          " you try inserting task in PaRSEC\n" );
    }

    if( dtd_tp == NULL ) {
        parsec_fatal( "Wait! You need to pass a correct parsec taskpool in order to insert task. "
          "Please use \"parsec_dtd_taskpool_new()\" to create new taskpool"
          "and then try to insert task. Thank you\n" );
    }
    

    va_list args, args_for_size, args_for_rank, args_for_flags;
    parsec_dtd_common_args_t common_args;
    AL4SAN_Parsec_Task_Flags task_flag;

    common_args.rank = -1; common_args.write_flow_count = 1;
    common_args.flow_count_of_template = 0; common_args.dtd_tp = dtd_tp;
    common_args.count_of_params_sent_by_user = 0;
    common_args.size_of_params = 0; common_args.flow_index = 0;

    va_start(args, options);
#if defined(PARSEC_PROF_TRACE)
    parsec_profiling_trace(dtd_tp->super.context->virtual_processes[0]->execution_streams[0]->es_profile,
     insert_task_trace_keyin, 0, dtd_tp->super.taskpool_id, NULL );
#endif
     //extracting the flags of the task 
    va_copy(args_for_flags, args);
    al4san_flags_decode(&task_flag, args_for_flags);
    va_end(args_for_flags);

     //extracting the rank of the task 
    va_copy(args_for_rank, args);
    al4san_parsec_arg_iterator(args_for_rank, parsec_dtd_iterator_arg_get_rank, (void*)&common_args);
    va_end(args_for_rank);

    uint64_t fkey = (uint64_t)(uintptr_t)codelet+ common_args.flow_count_of_template;
     //Creating master function structures 
     //Hash table lookup to check if the function structure exists or not 
    parsec_task_class_t *tc = (parsec_task_class_t *)
                                       parsec_dtd_find_task_class(dtd_tp, fkey);

    if( NULL == tc ) {
        va_copy(args_for_size, args);
        al4san_parsec_arg_iterator(args_for_size, parsec_dtd_iterator_arg_get_size, (void*)&common_args);
        va_end(args_for_size);

        tc = parsec_dtd_create_task_class(dtd_tp, (parsec_dtd_funcptr_t*) codelet, task_flag.task_label,
          common_args.count_of_params_sent_by_user,
          common_args.size_of_params, common_args.flow_count_of_template);

#if defined(PARSEC_PROF_TRACE)
        parsec_dtd_add_profiling_info((parsec_taskpool_t *)dtd_tp, tc->task_class_id, task_flag.task_label);
#endif // defined(PARSEC_PROF_TRACE) 
    }

#if defined(DISTRIBUTED)
    if( tp->context->nb_nodes > 1 ) {
        if( (-1 == common_args.rank) && (common_args.write_flow_count > 1) ) {
            parsec_fatal( "You inserted a task with out indicating where the task should be executed(using AFFINITY flag)."
              "This will result in executing this task on all nodes and the outcome might be not be what you want."
              "So we are exiting for now. Please see the usage of AFFINITY flag.\n" );
        } else if( common_args.rank == -1 && common_args.write_flow_count == 1 ) {
            // we have tasks with no real data as parameter so we are safe to execute it in each mpi process 
            common_args.rank = tp->context->my_rank;
        }
    } else {
        common_args.rank = 0;
    }
#else
    common_args.rank = 0;
#endif

    parsec_dtd_task_t *this_task = parsec_dtd_create_and_initialize_task(dtd_tp, tc, common_args.rank);
    this_task->super.priority = task_flag.task_priority;
    common_args.task = this_task;

    if( parsec_dtd_task_is_local(this_task) ) {
        parsec_object_t *object = (parsec_object_t *)this_task;
        // retaining the local task as many write flows as
        // it has and one to indicate when we have executed the task 
        (void)parsec_atomic_fetch_add_int32( &object->obj_reference_count, (common_args.write_flow_count) );

        common_args.tmp_param = NULL;

        // Getting the pointer to allocated memory by mempool 
        common_args.head_of_param_list = GET_HEAD_OF_PARAM_LIST(this_task);
        common_args.current_param      = common_args.head_of_param_list;
        common_args.value_block        = GET_VALUE_BLOCK(common_args.head_of_param_list, ((parsec_dtd_task_class_t*)tc)->count_of_params);
        common_args.current_val        = common_args.value_block;

        al4san_parsec_arg_iterator(args, parsec_dtd_iterator_arg_set_param_local, (void*)&common_args);

        if( common_args.tmp_param != NULL )
            common_args.tmp_param->next = NULL;
    } else {
        al4san_parsec_arg_iterator(args, parsec_dtd_iterator_arg_set_param_remote, (void*)&common_args);
    }
    va_end(args);

#if defined(DISTRIBUTED)
    assert(this_task->rank != -1);
#endif

    parsec_insert_dtd_task( (parsec_task_t *)this_task );
}*/

int AL4SAN_Parsec_insert_task(AL4SAN_codelet codelet,  AL4SAN_option_t *options, va_list args)
{

    //Defines a DAG of tasks
    parsec_taskpool_t *tp = (parsec_taskpool_t *) (options->sequence->schedopt);
    
    parsec_dtd_taskpool_t *dtd_tp = (parsec_dtd_taskpool_t *)tp;

    if( tp->context == NULL ) {
        parsec_fatal( "Sorry! You can not insert task wihtout enqueuing the taskpool to parsec_context"
          " first. Please make sure you call parsec_enqueue(parsec_context, taskpool) before"
          " you try inserting task in PaRSEC\n" );
    }

    if( dtd_tp == NULL ) {
        parsec_fatal( "Wait! You need to pass a correct parsec taskpool in order to insert task. "
          "Please use \"parsec_dtd_taskpool_new()\" to create new taskpool"
          "and then try to insert task. Thank you\n" );
    }
    

    va_list args_for_size, args_for_rank, args_for_flags;
    parsec_dtd_common_args_t common_args;
    AL4SAN_Parsec_Task_Flags task_flag;

    common_args.rank = -1; common_args.write_flow_count = 1;
    common_args.flow_count_of_template = 0; common_args.dtd_tp = dtd_tp;
    common_args.count_of_params_sent_by_user = 0;
    common_args.size_of_params = 0; common_args.flow_index = 0;

    
#if defined(PARSEC_PROF_TRACE)
    parsec_profiling_trace(dtd_tp->super.context->virtual_processes[0]->execution_streams[0]->es_profile,
     insert_task_trace_keyin, 0, dtd_tp->super.taskpool_id, NULL );
#endif
     //extracting the flags of the task 
    va_copy(args_for_flags, args);
    al4san_parsec_flags_decode(&task_flag, args_for_flags);
    va_end(args_for_flags);

     //extracting the rank of the task 
    va_copy(args_for_rank, args);
    al4san_parsec_arg_iterator(args_for_rank, parsec_dtd_iterator_arg_get_rank, (void*)&common_args);
    va_end(args_for_rank);

    uint64_t fkey = (uint64_t)(uintptr_t)(al4san_parsec_codelet *)codelet+ common_args.flow_count_of_template;
     //Creating master function structures 
     //Hash table lookup to check if the function structure exists or not 
    parsec_task_class_t *tc = (parsec_task_class_t *)
    parsec_dtd_find_task_class(dtd_tp, fkey);

    if( NULL == tc ) {
        va_copy(args_for_size, args);
        al4san_parsec_arg_iterator(args_for_size, parsec_dtd_iterator_arg_get_size, (void*)&common_args);
        va_end(args_for_size);

        tc = parsec_dtd_create_task_class(dtd_tp, (parsec_dtd_funcptr_t*)codelet, task_flag.task_label,
          common_args.count_of_params_sent_by_user,
          common_args.size_of_params, common_args.flow_count_of_template);

#if defined(PARSEC_PROF_TRACE)
        parsec_dtd_add_profiling_info((parsec_taskpool_t *)dtd_tp, tc->task_class_id, task_flag.task_label);
#endif // defined(PARSEC_PROF_TRACE) 
    }

#if defined(DISTRIBUTED)
    if( tp->context->nb_nodes > 1 ) {
        if( (-1 == common_args.rank) && (common_args.write_flow_count > 1) ) {
            parsec_fatal( "You inserted a task with out indicating where the task should be executed(using AFFINITY flag)."
              "This will result in executing this task on all nodes and the outcome might be not be what you want."
              "So we are exiting for now. Please see the usage of AFFINITY flag.\n" );
        } else if( common_args.rank == -1 && common_args.write_flow_count == 1 ) {
            // we have tasks with no real data as parameter so we are safe to execute it in each mpi process 
            common_args.rank = tp->context->my_rank;
        }
    } else {
        common_args.rank = 0;
    }
#else
    common_args.rank = 0;
#endif

    parsec_dtd_task_t *this_task = parsec_dtd_create_and_initialize_task(dtd_tp, tc, common_args.rank);
    this_task->super.priority = task_flag.task_priority;
    common_args.task = this_task;

    if( parsec_dtd_task_is_local(this_task) ) {
        parsec_object_t *object = (parsec_object_t *)this_task;
        // retaining the local task as many write flows as
        // it has and one to indicate when we have executed the task 
        (void)parsec_atomic_fetch_add_int32( &object->obj_reference_count, (common_args.write_flow_count) );

        common_args.tmp_param = NULL;

        // Getting the pointer to allocated memory by mempool 
        common_args.head_of_param_list = GET_HEAD_OF_PARAM_LIST(this_task);
        common_args.current_param      = common_args.head_of_param_list;
        common_args.value_block        = GET_VALUE_BLOCK(common_args.head_of_param_list, ((parsec_dtd_task_class_t*)tc)->count_of_params);
        common_args.current_val        = common_args.value_block;

        al4san_parsec_arg_iterator(args, parsec_dtd_iterator_arg_set_param_local, (void*)&common_args);

        if( common_args.tmp_param != NULL )
            common_args.tmp_param->next = NULL;
    } else {
        al4san_parsec_arg_iterator(args, parsec_dtd_iterator_arg_set_param_remote, (void*)&common_args);
    }

#if defined(DISTRIBUTED)
    assert(this_task->rank != -1);
#endif

    parsec_insert_dtd_task( (parsec_task_t *)this_task );
    
    return AL4SAN_SUCCESS;
}


/*void al4san_unpack_arg(AL4SAN_Parsec_arg_list *al4san_arg, ...)
{
    parsec_dtd_task_t *current_task = (parsec_dtd_task_t *)al4san_arg->this_task;
    parsec_dtd_task_param_t *current_param = GET_HEAD_OF_PARAM_LIST(current_task);
    int i = 0;
    void *tmp_val; void **tmp_ref;
    va_list varg_list;

    va_start(varg_list, al4san_arg);
    while(current_param != NULL) {
        if((current_param->op_type & GET_OP_TYPE) == VALUE) {
            tmp_val = va_arg(varg_list, void*);
            memcpy(tmp_val, current_param->pointer_to_tile, current_param->arg_size);
        } else if((current_param->op_type & GET_OP_TYPE) == SCRATCH ||
          (current_param->op_type & GET_OP_TYPE) == REF) {
            tmp_ref = va_arg(varg_list, void**);
            *tmp_ref = current_param->pointer_to_tile;
        } else if((current_param->op_type & GET_OP_TYPE) == INPUT ||
          (current_param->op_type & GET_OP_TYPE) == INOUT ||
          (current_param->op_type & GET_OP_TYPE) == OUTPUT) {
            tmp_ref = va_arg(varg_list, void**);
            *tmp_ref = PARSEC_DATA_COPY_GET_PTR(al4san_arg->this_task->data[i].data_in);
            i++;
        } else {
            parsec_warning("/!\\ Flag is not recognized in parsec_dtd_unpack_args /!\\.\n");
            assert(0);
        }
        current_param = current_param->next;
    }
    va_end(varg_list);
}*/


int AL4SAN_Parsec_unpack_arg(AL4SAN_arg_list* al4san_arg, va_list varg_list)
{
//    AL4SAN_Parsec_arg_list *al4san_arg= (AL4SAN_Parsec_arg_list*)args;
   AL4SAN_Parsec_arg_list *parsecarg=al4san_arg->Parsec_arg_list;
    parsec_dtd_task_t *current_task = (parsec_dtd_task_t *)parsecarg->this_task;
    parsec_dtd_task_param_t *current_param = GET_HEAD_OF_PARAM_LIST(current_task);
    int i = 0;
    void *tmp_val; void **tmp_ref;

    while(current_param != NULL) {
        if((current_param->op_type & GET_OP_TYPE) == VALUE) {
            tmp_val = va_arg(varg_list, void*);
            memcpy(tmp_val, current_param->pointer_to_tile, current_param->arg_size);
        } else if((current_param->op_type & GET_OP_TYPE) == SCRATCH ||
          (current_param->op_type & GET_OP_TYPE) == REF) {
            tmp_ref = va_arg(varg_list, void**);
            *tmp_ref = current_param->pointer_to_tile;
        } else if((current_param->op_type & GET_OP_TYPE) == INPUT ||
          (current_param->op_type & GET_OP_TYPE) == INOUT ||
          (current_param->op_type & GET_OP_TYPE) == OUTPUT) {
            tmp_ref = va_arg(varg_list, void**);
            *tmp_ref = PARSEC_DATA_COPY_GET_PTR(parsecarg->this_task->data[i].data_in);
            i++;
        } else {
            parsec_warning("/!\\ Flag is not recognized in parsec_dtd_unpack_args /!\\.\n");
            assert(0);
        }
        current_param = current_param->next;
    }
}
