/**
 *
 * @file runtime_descriptor.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * @brief AL4SAN Quark descriptor routines
 *
 * @version 1.0.1
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2019-02-06
 *
 */
/**
 *
 * @brief AL4SAN Quark descriptor routines
 *  
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#include <stdlib.h>
#include "al4san_runtime.h"

int
al4san_runtime_tag_init( int user_tag_width,
                           int user_tag_sep )
{
   (void)user_tag_width;
   (void)user_tag_sep;
}

void AL4SAN_Runtime_comm_set_tag_sizes( int user_tag_width,
                                 int user_tag_sep )
{
    (void)user_tag_width;
    (void)user_tag_sep;
}

void AL4SAN_RUNTIME_malloc(void**A, size_t size )
{ 
   *A = malloc( size );
}

void *AL4SAN_Runtime_malloc( size_t size )
{ 
   return malloc(size);
}

void AL4SAN_Runtime_free( void  *ptr,
                   size_t size )
{
    (void)size;
    free( ptr );
    return;
}

void AL4SAN_Runtime_desc_create( AL4SAN_desc_t *desc )
{
    (void)desc;
    return;
}

void AL4SAN_Runtime_desc_destroy( AL4SAN_desc_t *desc )
{
    (void)desc;
    return;
}

int AL4SAN_Runtime_desc_acquire( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Runtime_desc_release( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

void
AL4SAN_Runtime_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence )
{
    (void)desc;
    (void)sequence;
    return;
}


void
AL4SAN_Runtime_flush( )
{
    return;
}

void
AL4SAN_Runtime_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    (void)sequence;
    (void)A;
    (void)Am;
    (void)An;
    return;
}

void
AL4SAN_Runtime_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    (void)sequence;
    (void)A;
    (void)Am;
    (void)An;
    return;
}

void
AL4SAN_Runtime_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am)
{
    (void)sequence;
    (void)A;
    (void)Am;
    return;
}

void
AL4SAN_Runtime_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A)
{
    (void)sequence;
    (void)A;
    return;
}

#if defined(AL4SAN_USE_MIGRATE)
void AL4SAN_Runtime_data_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}

void AL4SAN_Runtime_matrix_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}

void AL4SAN_Runtime_vector_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}

void AL4SAN_Runtime_scaler_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int new_rank )
{
    (void)sequence; (void)A; (void)new_rank;
}

#endif

/*void *AL4SAN_Runtime_data_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
    address=desc->get_blkaddr( desc, m, n );
    return  &address;
}*/


void *AL4SAN_Runtime_data_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
  return desc->get_blkaddr( desc, m, n );
}

void *AL4SAN_Runtime_matrix_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
  return desc->get_blkaddr( desc, m, n );
}

void *AL4SAN_Runtime_vector_getaddr( const AL4SAN_desc_t *desc, int m)
{
  return desc->get_blkaddr( desc, m, 0 );
}
void *AL4SAN_Runtime_scaler_getaddr( const AL4SAN_desc_t *desc)
{
  return desc->get_blkaddr( desc, 0, 0 );
}

void AL4SAN_RUNTIME_data_getaddr( const AL4SAN_desc_t *desc, void **ptr, int m, int n)
{
    *ptr = desc->get_blkaddr( desc, m, n );
}
