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
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 * @version 1.1.0
 * @author Vijay Joshi
 * @author Cedric Castagnede
 * @date 2019-02-06
 * @version 1.1.0
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 *
 */
#include <stdlib.h>
#include "al4san_openmp.h"

int
al4san_openmp_tag_init( int user_tag_width,
                           int user_tag_sep )
{
   (void)user_tag_width;
   (void)user_tag_sep;
}

void AL4SAN_Openmp_comm_set_tag_sizes( int user_tag_width,
                                 int user_tag_sep )
{
    (void)user_tag_width;
    (void)user_tag_sep;
}

void AL4SAN_Openmp_malloc_ptr(void**A, size_t size )
{ 
   *A = malloc( size );
}

void *AL4SAN_Openmp_malloc( size_t size )
{ 
   return malloc(size);
}

void AL4SAN_Openmp_free( void  *ptr,
                   size_t size )
{
    (void)size;
    free( ptr );
    return;
}

void AL4SAN_Openmp_desc_create( AL4SAN_desc_t *desc )
{
    (void)desc;
    return;
}

void AL4SAN_Openmp_desc_destroy( AL4SAN_desc_t *desc )
{
    (void)desc;
    return;
}

int AL4SAN_Openmp_desc_acquire( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Openmp_desc_release( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

void
AL4SAN_Openmp_desc_flush( const AL4SAN_desc_t     *desc,
                    const AL4SAN_sequence_t *sequence )
{
    (void)desc;
    (void)sequence;
    return;
}


void
AL4SAN_Openmp_flush( )
{
    return;
}

void
AL4SAN_Openmp_data_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    (void)sequence;
    (void)A;
    (void)Am;
    (void)An;
    return;
}

void
AL4SAN_Openmp_matrix_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    (void)sequence;
    (void)A;
    (void)Am;
    (void)An;
    return;
}

void
AL4SAN_Openmp_vector_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am)
{
    (void)sequence;
    (void)A;
    (void)Am;
    return;
}

void
AL4SAN_Openmp_scaler_flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A)
{
    (void)sequence;
    (void)A;
    return;
}

#if defined(AL4SAN_USE_MIGRATE)
void AL4SAN_Openmp_data_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}

void AL4SAN_Openmp_matrix_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}

void AL4SAN_Openmp_vector_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}

void AL4SAN_Openmp_scaler_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int new_rank )
{
    (void)sequence; (void)A; (void)new_rank;
}

#endif

/*void *AL4SAN_Openmp_data_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
    address=desc->get_blkaddr( desc, m, n );
    return  &address;
}*/


void *AL4SAN_Openmp_data_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
  return desc->get_blkaddr( desc, m, n );
}

void *AL4SAN_Openmp_matrix_getaddr( const AL4SAN_desc_t *desc, int m, int n)
{
  return desc->get_blkaddr( desc, m, n );
}

void *AL4SAN_Openmp_vector_getaddr( const AL4SAN_desc_t *desc, int m)
{
  return desc->get_blkaddr( desc, m, 0 );
}
void *AL4SAN_Openmp_scaler_getaddr( const AL4SAN_desc_t *desc)
{
  return desc->get_blkaddr( desc, 0, 0 );
}

void AL4SAN_Openmp_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n)
{
    *ptr = desc->get_blkaddr( desc, m, n );
}

int AL4SAN_Openmp_data_getrank( const AL4SAN_desc_t *A, int m, int n )
{

  return A->get_rankof( A, m, n );
}


int AL4SAN_Openmp_matrix_getrank( const AL4SAN_desc_t *A, int m, int n )
{

  return A->get_rankof( A, m, n );
}

int AL4SAN_Openmp_vector_getrank( const AL4SAN_desc_t *A, int m)
{

  return A->get_rankof( A, m, 0);
}

int AL4SAN_Openmp_scaler_getrank( const AL4SAN_desc_t *A)
{

  return A->get_rankof( A, 0, 0);
}

void AL4SAN_Openmp_broadcast(AL4SAN_sequence_t *sequence, int myrank,
                                int root, void* tile_root, int arena_index,
                                int *dest_ranks, int dest_rank_idx){

  //fprintf(stderr, "It is not available in OpenMP since it is shared-memory only\n");
  return;
}

void AL4SAN_Openmp_broadcast_id(int32_t bcast_id, AL4SAN_sequence_t *sequence, int myrank,
                                int root, void* tile_root, int arena_index,
                                int *dest_ranks, int dest_rank_idx){

  //fprintf(stderr, "It is not available in OpenMP since it is shared-memory only\n");
  return;
}
