/**
 *
 * @file parsec/runtime_descriptor.c
 *
 * @copyright 2012-2017 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
* @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * @brief AL4SAN PaRSEC descriptor routines
 *
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 *
 * @version 1.0.1
 * @author Reazul Hoque
 * @author Mathieu Faverge
 * @date 2019-02-06
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 */
#include <stdlib.h>
#include "al4san_parsec.h"
#include <parsec/data.h>
#include <parsec/datatype.h>
#include <parsec/arena.h>

#if defined(AL4SAN_USE_MPI)

/* Variable parsec_dtd_no_of_arenas is private and cannot be changed */
#define AL4SAN_PARSEC_DTD_NO_OF_ARENA 16 /**< Number of arenas available per DTD */

typedef struct al4san_parsec_arena_s {
    /* int mb; */
    /* int nb; */
    /* cham_flttype_t dtype; */
    size_t size;
} al4san_parsec_arena_t;

static int al4san_parsec_nb_arenas = 0;
static al4san_parsec_arena_t al4san_parsec_registered_arenas[AL4SAN_PARSEC_DTD_NO_OF_ARENA] = { { 0 } };

#endif

void AL4SAN_Parsec_comm_set_tag_sizes( int user_tag_width,
                                 int user_tag_sep )
{
    (void)user_tag_width;
    (void)user_tag_sep;
}

void *AL4SAN_Parsec_malloc( size_t size )
{
    return malloc(size);
}

void AL4SAN_Parsec_malloc_ptr( void **ptr, size_t size )
{   
    *ptr = malloc(size);
}


void AL4SAN_Parsec_free( void *ptr, size_t size )
{
    (void)size;
    free(ptr);
    return;
}

static inline void
al4san_parsec_key_to_coordinates(parsec_data_collection_t *data_collection, parsec_data_key_t key,
                                int *m, int *n)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    int _m, _n;

    _m = key % mdesc->lmt;
    _n = key / mdesc->lmt;
    *m = _m - mdesc->i / mdesc->mb;
    *n = _n - mdesc->j / mdesc->nb;
}

static inline parsec_data_key_t
al4san_parsec_data_key(parsec_data_collection_t *data_collection, ...)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    va_list ap;
    int m, n;

    /* Get coordinates */
    va_start(ap, data_collection);
    m = va_arg(ap, unsigned int);
    n = va_arg(ap, unsigned int);
    va_end(ap);

    /* Offset by (i,j) to translate (m,n) in the global matrix */
    m += mdesc->i / mdesc->mb;
    n += mdesc->j / mdesc->nb;

    return ((n * mdesc->lmt) + m);
}

static inline uint32_t
al4san_parsec_rank_of(parsec_data_collection_t *data_collection, ...)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    va_list ap;
    int m, n;

    /* Get coordinates */
    va_start(ap, data_collection);
    m = va_arg(ap, unsigned int);
    n = va_arg(ap, unsigned int);
    va_end(ap);

    /* Offset by (i,j) to translate (m,n) in the global matrix */
    m += mdesc->i / mdesc->mb;
    n += mdesc->j / mdesc->nb;

    return mdesc->get_rankof( mdesc, m, n );
}

static inline uint32_t
al4san_parsec_rank_of_key(parsec_data_collection_t *data_collection, parsec_data_key_t key)
{
    int m, n;
    al4san_parsec_key_to_coordinates(data_collection, key, &m, &n);
    return al4san_parsec_rank_of(data_collection, m, n);
}

static inline int32_t
al4san_parsec_vpid_of(parsec_data_collection_t *data_collection, ... )
{
    (void)data_collection;
    return 0;
}

static inline int32_t
al4san_parsec_vpid_of_key(parsec_data_collection_t *data_collection, parsec_data_key_t key)
{
    int m, n;
    al4san_parsec_key_to_coordinates(data_collection, key, &m, &n);
    return al4san_parsec_vpid_of(data_collection, m, n);
}

static inline parsec_data_t*
al4san_parsec_data_of(parsec_data_collection_t *data_collection, ...)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    va_list ap;
    int m, n;

    /* Get coordinates */
    va_start(ap, data_collection);
    m = va_arg(ap, unsigned int);
    n = va_arg(ap, unsigned int);
    va_end(ap);

    /* Offset by (i,j) to translate (m,n) in the global matrix */
    m += mdesc->i / mdesc->mb;
    n += mdesc->j / mdesc->nb;

#if defined(AL4SAN_USE_MPI)
    /* TODO: change displacement in data_map when in distributed */
    //assert( data_collection->nodes == 1 );
#endif
    return parsec_data_create( pdesc->data_map + n * mdesc->lmt + m, data_collection,
                               al4san_parsec_data_key( data_collection, m, n ),
                               mdesc->get_blkaddr( mdesc, m, n ),
                               mdesc->bsiz * AL4SAN_Element_Size(mdesc->dtyp) );
}

static inline parsec_data_t*
al4san_parsec_data_of_key(parsec_data_collection_t *data_collection, parsec_data_key_t key)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    int m, n;
    al4san_parsec_key_to_coordinates(data_collection, key, &m, &n);

#if defined(AL4SAN_USE_MPI)
    /* TODO: change displacement in data_map when in distributed */
    //assert( data_collection->nodes == 1 );
#endif
    return parsec_data_create( pdesc->data_map + key, data_collection, key,
                               mdesc->get_blkaddr( mdesc, m, n ),
                               mdesc->bsiz * AL4SAN_Element_Size(mdesc->dtyp) );
}

#ifdef parsec_PROF_TRACE
static inline int
al4san_parsec_key_to_string(parsec_data_collection_t *data_collection, parsec_data_key_t key, char * buffer, uint32_t buffer_size)
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)data_collection;
    AL4SAN_desc_t *mdesc = pdesc->desc;
    int m, n, res;
    al4san_parsec_key_to_coordinates(data_collection, key, &m, &n);
    res = snprintf(buffer, buffer_size, "(%d, %d)", m, n);
    if (res < 0)
    {
        printf("error in key_to_string for tile (%u, %u) key: %u\n",
               (unsigned int)m, (unsigned int)n, datakey);
    }
    return res;
}
#endif

/**
 *  Create data descriptor
 */
void AL4SAN_Parsec_desc_create( AL4SAN_desc_t *mdesc )
{
    parsec_data_collection_t *data_collection;
    al4san_parsec_desc_t *pdesc;
    int comm_size;

    pdesc = malloc( sizeof(al4san_parsec_desc_t) );
    data_collection = (parsec_data_collection_t*)pdesc;

    /* Super setup */
    comm_size = AL4SAN_Parsec_comm_size( NULL );
    data_collection->nodes  = comm_size;
    data_collection->myrank = mdesc->myrank;

    data_collection->data_key    = al4san_parsec_data_key;
    data_collection->rank_of     = al4san_parsec_rank_of;
    data_collection->rank_of_key = al4san_parsec_rank_of_key;
    data_collection->data_of     = al4san_parsec_data_of;
    data_collection->data_of_key = al4san_parsec_data_of_key;
    data_collection->vpid_of     = al4san_parsec_vpid_of;
    data_collection->vpid_of_key = al4san_parsec_vpid_of_key;
#if defined(parsec_PROF_TRACE)
    {
        int rc;
        data_collection->key_to_string = al4san_parsec_key_to_string;
        data_collection->key           = NULL;
        rc = asprintf(&(data_collection->key_dim), "(%d, %d)", mdesc->lmt, mdesc->lnt);
        (void)rc;
    }
#endif
    data_collection->memory_registration_status = MEMORY_STATUS_UNREGISTERED;

    pdesc->data_map = calloc( mdesc->lmt * mdesc->lnt, sizeof(parsec_data_t*) );

    /* Double linking */
    pdesc->desc     = mdesc;
    mdesc->schedopt = pdesc;

    parsec_dtd_data_collection_init(data_collection);

    /* arena init */
    pdesc->arena_index = 0;

    /* taskpool init to bypass a requirement of PaRSEC  */
#if defined(AL4SAN_USE_MPI)
    /* Look if an arena already exists for this descriptor */
    {
        al4san_parsec_arena_t *arena = al4san_parsec_registered_arenas;
        size_t size = mdesc->mb * mdesc->nb * AL4SAN_Element_Size(mdesc->dtyp);
        int i;

        for(i=0; i<al4san_parsec_nb_arenas; i++, arena++) {
            if ( size == arena->size) {
                pdesc->arena_index = i;
                break;
            }
        }

        if (i == al4san_parsec_nb_arenas) {
            parsec_datatype_t datatype;

            /* Create a taskpool to make sur the system is initialized */
            if ( i == 0 ) {
                parsec_taskpool_t *tp = parsec_dtd_taskpool_new();
                parsec_taskpool_free( tp );
            }

            /* Internal limitation of PaRSEC */
            assert(al4san_parsec_nb_arenas < AL4SAN_PARSEC_DTD_NO_OF_ARENA);

            switch(mdesc->dtyp) {
            case Al4sanInteger:       datatype = parsec_datatype_int32_t; break;
            case Al4sanRealFloat:     datatype = parsec_datatype_float_t; break;
            case Al4sanRealDouble:    datatype = parsec_datatype_double_t; break;
            case Al4sanComplexFloat:  datatype = parsec_datatype_complex_t; break;
            case Al4sanComplexDouble: datatype = parsec_datatype_double_complex_t; break;
            default: al4san_fatal_error("AL4SAN_Element_Size", "undefined type"); break;
            }

            /* Register the new arena */
            parsec_matrix_add2arena( parsec_dtd_arenas[i], datatype, matrix_UpperLower, 1,
                                     mdesc->mb, mdesc->nb, mdesc->mb, PARSEC_ARENA_ALIGNMENT_SSE, -1 );
            arena->size = size;
            pdesc->arena_index = i;
            al4san_parsec_nb_arenas++;
        }
    }
#endif
    /* /\* Overwrite the leading dimensions to store the padding *\/ */
    /* mdesc->llm = mdesc->mb * mdesc->lmt; */
    /* mdesc->lln = mdesc->nb * mdesc->lnt; */
    return;
}

/**
 *  Destroy data descriptor
 */
void AL4SAN_Parsec_desc_destroy( AL4SAN_desc_t *mdesc )
{
    al4san_parsec_desc_t *pdesc = (al4san_parsec_desc_t*)(mdesc->schedopt);
    if ( pdesc == NULL ) {
        return;
    }

    if ( pdesc->data_map != NULL ) {
        parsec_data_t **data = pdesc->data_map;
        int nb_local_tiles = mdesc->lmt * mdesc->lnt;
        int i;

        for(i=0; i<nb_local_tiles; i++, data++)
        {
            if (*data) {
                parsec_data_destroy( *data );
            }
        }

        free( pdesc->data_map );
        pdesc->data_map = NULL;
    }

    parsec_dtd_data_collection_fini( (parsec_data_collection_t *)pdesc );

    free(pdesc);
    mdesc->schedopt = NULL;
    return;
}

/**
 *  Acquire data
 */
int AL4SAN_Parsec_desc_acquire( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

/**
 *  Release data
 */
int AL4SAN_Parsec_desc_release( const AL4SAN_desc_t *desc )
{
    (void)desc;
    return AL4SAN_SUCCESS;
}

/**
 *  Flush cached data
 */
void AL4SAN_Parsec_flush()
{
}

void AL4SAN_Parsec_desc_flush( const AL4SAN_desc_t     *desc,
                         const AL4SAN_sequence_t *sequence )
{
    parsec_taskpool_t* PARSEC_dtd_taskpool = (parsec_taskpool_t *)(sequence->schedopt);

    parsec_dtd_data_flush_all( PARSEC_dtd_taskpool, (parsec_data_collection_t*)(desc->schedopt) );
}

void AL4SAN_Parsec_data_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int Am, int An )
{
    /*
     * For now, we do nothing in this function as in PaRSEC, once the data is
     * flushed it cannot be reused in the same sequence, when this issue will be
     * fixed, we will uncomment this function
     */
    /* parsec_taskpool_t* PARSEC_dtd_taskpool = (parsec_taskpool_t *)(sequence->schedopt); */
    /* parsec_dtd_data_flush( PARSEC_dtd_taskpool, RTBLKADDR( A, AL4SAN_Complex64_t, Am, An ) ); */

    (void)sequence; (void)A; (void)Am; (void)An;
    return;
}

void AL4SAN_Parsec_matrix_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int Am, int An )
{
    /*
     * For now, we do nothing in this function as in PaRSEC, once the data is
     * flushed it cannot be reused in the same sequence, when this issue will be
     * fixed, we will uncomment this function
     */
    /* parsec_taskpool_t* PARSEC_dtd_taskpool = (parsec_taskpool_t *)(sequence->schedopt); */
    /* parsec_dtd_data_flush( PARSEC_dtd_taskpool, RTBLKADDR( A, AL4SAN_Complex64_t, Am, An ) ); */

    (void)sequence; (void)A; (void)Am; (void)An;
    return;
}

void AL4SAN_Parsec_vector_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int Am)
{
    /*
     * For now, we do nothing in this function as in PaRSEC, once the data is
     * flushed it cannot be reused in the same sequence, when this issue will be
     * fixed, we will uncomment this function
     */
    /* parsec_taskpool_t* PARSEC_dtd_taskpool = (parsec_taskpool_t *)(sequence->schedopt); */
    /* parsec_dtd_data_flush( PARSEC_dtd_taskpool, RTBLKADDR( A, AL4SAN_Complex64_t, Am, An ) ); */

    (void)sequence; (void)A; (void)Am;
    return;
}

void AL4SAN_Parsec_scaler_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A)
{
    /*
     * For now, we do nothing in this function as in PaRSEC, once the data is
     * flushed it cannot be reused in the same sequence, when this issue will be
     * fixed, we will uncomment this function
     */
    /* parsec_taskpool_t* PARSEC_dtd_taskpool = (parsec_taskpool_t *)(sequence->schedopt); */
    /* parsec_dtd_data_flush( PARSEC_dtd_taskpool, RTBLKADDR( A, AL4SAN_Complex64_t, Am, An ) ); */

    (void)sequence; (void)A; 
    return;
}

#if defined(AL4SAN_USE_MIGRATE)
void AL4SAN_Parsec_data_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
void AL4SAN_Parsec_matrix_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
}
void AL4SAN_Parsec_vector_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int new_rank )
{
    (void)sequence; (void)A; (void)Am; (void)new_rank;
}
void AL4SAN_Parsec_scaler_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int new_rank )
{
    (void)sequence; (void)A; (void)new_rank;
}
#endif

/**
 *  Get data addr
 */
void *AL4SAN_Parsec_data_getaddr( const AL4SAN_desc_t *desc, int m, int n )
{

return parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt),
                                                            ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, m, n) );
}

void AL4SAN_Parsec_data_getaddr_ptr( const AL4SAN_desc_t *desc, void **ptr, int m, int n )
{

//return parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt), ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, m, n) );
*ptr = parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt), ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, m, n) );

}


void *AL4SAN_Parsec_matrix_getaddr( const AL4SAN_desc_t *desc, int m, int n )
{

return parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt),
                                                            ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, m, n) );
}

void *AL4SAN_Parsec_vector_getaddr( const AL4SAN_desc_t *desc, int m)
{

return parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt),
                                                            ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, m, 0) );
}

void *AL4SAN_Parsec_scaler_getaddr( const AL4SAN_desc_t *desc)
{

return parsec_dtd_tile_of( (parsec_data_collection_t *) ((desc)->schedopt),
                                                            ((parsec_data_collection_t *) (desc)->schedopt)->data_key((desc)->schedopt, 0, 0) );
}
