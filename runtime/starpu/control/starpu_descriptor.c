/**
 *
 * @file runtime_descriptor.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 * @brief Al4san StarPU descriptor routines
 *
 * @version 1.0.1
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2019-02-06
 *
 ***
 *
 * @brief AL4SAN StarPU descriptor routines
 *  
 *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
 *
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 *
 **/
#include <stdlib.h>
#include <unistd.h>
#include "al4san_starpu.h"

/**
 *  Set the tag sizes
 */
#if defined(AL4SAN_USE_MPI)

/* Take 24 bits for the tile id, and 7 bits for descriptor id.
 These values can be changed through the call AL4SAN_User_Tag_Size(int tag_width, int tag_sep) */
#define TAG_WIDTH_MIN 20
static int tag_width = 31;
static int tag_sep   = 24;
static int _tag_mpi_initialized_ = 0;

int
al4san_starpu_tag_init( int user_tag_width,
                           int user_tag_sep )
{
    if (!_tag_mpi_initialized_) {
        int ok = 0;
        uintptr_t tag_ub;

        tag_width = user_tag_width;
        tag_sep   = user_tag_sep;

        void *tag_ub_p = NULL;
#if defined(HAVE_STARPU_MPI_COMM_GET_ATTR)
        starpu_mpi_comm_get_attr(MPI_COMM_WORLD, STARPU_MPI_TAG_UB, &tag_ub_p, &ok);
#else
        MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &tag_ub_p, &ok);
#endif
        tag_ub = (uintptr_t)tag_ub_p;

        if ( !ok ) {
            al4san_error("RUNTIME_desc_create", "MPI_TAG_UB not known by StarPU");
        }

        while ( ((uintptr_t)((1UL<<tag_width) - 1) > tag_ub ) &&
                (tag_width >= TAG_WIDTH_MIN) )
        {
            tag_width--;
            tag_sep--;
        }

        if ( tag_width < TAG_WIDTH_MIN ) {
            al4san_error("RUNTIME_desc_create", "MPI_TAG_UB too small to identify all the data");
            return AL4SAN_ERR_OUT_OF_RESOURCES;
        }

        _tag_mpi_initialized_ = 1;
        return AL4SAN_SUCCESS;
    }
    else {
        return AL4SAN_ERR_REINITIALIZED;
    }
}


#ifndef HAVE_STARPU_MPI_DATA_REGISTER
#define starpu_mpi_data_register( handle_, tag_, owner_ )       \
    do {                                                        \
        starpu_data_set_rank( (handle_), (owner_) );            \
        starpu_data_set_tag( (handle_), (tag_) );               \
    } while(0)
#endif

#endif

void AL4SAN_Starpu_comm_set_tag_sizes( int user_tag_width,
                                 int user_tag_sep )
{
#if defined(AL4SAN_USE_MPI)
    int rc;
    rc = al4san_starpu_tag_init( user_tag_width, user_tag_sep );
    if ( rc != AL4SAN_SUCCESS ) {
        al4san_error("RUNTIME_user_tag_size",
                    "must be called before creating any Cham descriptor with AL4SAN_Desc_create(). The tag sizes will not be modified.");
    }
#endif
    (void)user_tag_width; (void)user_tag_sep;
}

/**
 *  Malloc/Free of the data
 */
#ifdef STARPU_MALLOC_SIMULATION_FOLDED
#define FOLDED STARPU_MALLOC_SIMULATION_FOLDED
#else
#define FOLDED 0
#endif

void *AL4SAN_Starpu_malloc( size_t size )
{
#if defined(CHAMELEON_SIMULATION) && !defined(STARPU_MALLOC_SIMULATION_FOLDED) && !defined(CHAMELEON_USE_MPI)
    return (void*) 1;
#else
    void *ptr;

    if (starpu_malloc_flags(&ptr, size, STARPU_MALLOC_PINNED|FOLDED|STARPU_MALLOC_COUNT) != 0) {
        return NULL;
    }
    return ptr;
#endif
}

void AL4SAN_Starpu_malloc_ptr( void**ptr, size_t size )
{

#if defined(AL4SAN_SIMULATION) && !defined(STARPU_MALLOC_SIMULATION_FOLDED) && !defined(AL4SAN_USE_MPI)
    return (void*) 1;
#else

    if (starpu_malloc_flags(ptr, size, STARPU_MALLOC_PINNED|FOLDED|STARPU_MALLOC_COUNT) != 0) {
        return;
    }
#endif
}


void AL4SAN_Starpu_free( void  *ptr,
                   size_t size )
{
#if defined(AL4SAN_SIMULATION) && !defined(STARPU_MALLOC_SIMULATION_FOLDED) && !defined(AL4SAN_USE_MPI)
    (void)ptr; (void)size;
    return;
#else
    starpu_free_flags(ptr, size, STARPU_MALLOC_PINNED|FOLDED|STARPU_MALLOC_COUNT);
#endif
}

/**
 *  Create data descriptor
 */
void AL4SAN_Starpu_desc_create( AL4SAN_desc_t *desc )
{
    int64_t lmt = desc->lmt;
    int64_t lnt = desc->lnt;

    desc->occurences = 1;

    /*
     * Allocate starpu_handle_t array (handlers are initialized on the fly when
     * discovered by any algorithm to save space)
     */
    desc->schedopt = (void*)calloc(lnt*lmt,sizeof(starpu_data_handle_t));
    assert(desc->schedopt);

#if defined(AL4SAN_USE_CUDA) && !defined(AL4SAN_SIMULATION)
    /*
     * Register allocated memory as CUDA pinned memory
     */
    if ( (desc->use_mat == 1) && (desc->register_mat == 1) )
    {
        int64_t eltsze = AL4SAN_Element_Size(desc->dtyp);
        size_t size = (size_t)(desc->llm) * (size_t)(desc->lln) * eltsze;
        cudaError_t rc;

        /* Register the matrix as pinned memory */
        rc = cudaHostRegister( desc->mat, size, cudaHostRegisterPortable );
        if ( rc != cudaSuccess )
        {
            /* Disable the unregister as register failed */
            desc->register_mat = 0;
            al4san_warning("RUNTIME_desc_create(StarPU): cudaHostRegister - ", cudaGetErrorString( rc ));
        }
    }
#endif

    if (desc->ooc) {
        int     lastmm   = desc->lm - (desc->lmt-1) * desc->mb;
        int     lastnn   = desc->ln - (desc->lnt-1) * desc->nb;
        int64_t eltsze   = AL4SAN_Element_Size(desc->dtyp);
        int     pagesize = getpagesize();

        if ( ((desc->mb * desc->nb * eltsze) % pagesize != 0) ||
             ((lastmm   * desc->nb * eltsze) % pagesize != 0) ||
             ((desc->mb * lastnn   * eltsze) % pagesize != 0) ||
             ((lastmm   * lastnn   * eltsze) % pagesize != 0) )
        {
            al4san_error("RUNTIME_desc_create", "Matrix and tile size not suitable for out-of-core: all tiles have to be multiples of 4096. Tip : choose 'n' and 'nb' as both multiples of 32.");
            return;
        }
    }

#if defined(AL4SAN_USE_MPI)
    /*
     * Check that we are not going over MPI tag limitations
     */
    {
        al4san_starpu_tag_init( tag_width, tag_sep );

        /* Check that we won't create overflow in tags used */
        if ( ((uintptr_t)(lnt*lmt)) > ((uintptr_t)(1UL<<tag_sep)) ) {
            al4san_fatal_error("RUNTIME_desc_create", "Too many tiles in the descriptor for MPI tags");
            return;
        }
        assert(lmt*lmt<=(1<<tag_sep));

        if ( ((uintptr_t)desc->id) >= (uintptr_t)(1UL<<(tag_width-tag_sep)) ) {
            al4san_fatal_error("RUNTIME_desc_create", "Number of descriptor available in MPI mode out of stock");
            return;
        }
        assert( ((uintptr_t)desc->id) < (uintptr_t)(1UL<<(tag_width-tag_sep)) );
    }
#endif
}

/**
 *  Destroy data descriptor
 */
void AL4SAN_Starpu_desc_destroy( AL4SAN_desc_t *desc )
{
    desc->occurences--;

    /*
     * If this is the last descriptor using the matrix, we release the handle
     * and unregister the GPU data
     */
    if ( desc->occurences == 0 ) {
        starpu_data_handle_t *handle = (starpu_data_handle_t*)(desc->schedopt);
        int lmt = desc->lmt;
        int lnt = desc->lnt;
        int m, n;

        for (n = 0; n < lnt; n++) {
            for (m = 0; m < lmt; m++)
            {
                if (*handle != NULL) {
                    starpu_data_unregister(*handle);
                }
                handle++;
            }
        }

#if defined(AL4SAN_USE_CUDA) && !defined(AL4SAN_SIMULATION)
        if ( (desc->use_mat == 1) && (desc->register_mat == 1) )
        {
            /* Unmap the pinned memory associated to the matrix */
            if (cudaHostUnregister(desc->mat) != cudaSuccess)
            {
                al4san_warning("RUNTIME_desc_destroy(StarPU)",
                                  "cudaHostUnregister failed to unregister the "
                                  "pinned memory associated to the matrix");
            }
        }
#endif /* defined(AL4SAN_USE_CUDA) */

        free(desc->schedopt);
    }
}

/**
 *  Acquire data
 */
int AL4SAN_Starpu_desc_acquire( const AL4SAN_desc_t *desc )
{
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(desc->schedopt);
    int lmt = desc->lmt;
    int lnt = desc->lnt;
    int m, n;

    for (n = 0; n < lnt; n++) {
        for (m = 0; m < lmt; m++)
        {
            /*if ( (*handle == NULL) ||
                 !al4san_desc_islocal( desc, m, n ) )
            {
                handle++;
                continue;
            }*/
            starpu_data_acquire(*handle, STARPU_R);
            handle++;
        }
    }
    return AL4SAN_SUCCESS;
}

/**
 *  Release data
 */
int AL4SAN_Starpu_desc_release( const AL4SAN_desc_t *desc )
{
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(desc->schedopt);
    int lmt = desc->lmt;
    int lnt = desc->lnt;
    int m, n;

    for (n = 0; n < lnt; n++) {
        for (m = 0; m < lmt; m++)
        {
            /*if ( (*handle == NULL) ||
                 !al4san_desc_islocal( desc, m, n ) )
            {
                handle++;
                continue;
            }*/
            starpu_data_release(*handle);
            handle++;
        }
    }
    return AL4SAN_SUCCESS;
}

/**
 *  Flush cached data
 */
void AL4SAN_Starpu_flush()
{
#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush_all_data(MPI_COMM_WORLD);
#endif
}

/**
 * Different implementations of the flush call based on StarPU version
 */
#ifdef HAVE_STARPU_DATA_WONT_USE

static inline void
al4san_starpu_data_wont_use( starpu_data_handle_t handle ) {
    starpu_data_wont_use( handle );
}

#elif defined HAVE_STARPU_IDLE_PREFETCH

static inline void
al4san_starpu_data_flush( starpu_data_handle_t handle)
{
    starpu_data_idle_prefetch_on_node(handle, STARPU_MAIN_RAM, 1);
    starpu_data_release_on_node(handle, -1);
}

static inline void
al4san_starpu_data_wont_use( starpu_data_handle_t handle ) {
    starpu_data_acquire_on_node_cb( handle, -1, STARPU_R,
                                    al4san_starpu_data_flush, handle );
}

#else

static inline void
al4san_starpu_data_wont_use( starpu_data_handle_t handle ) {
    starpu_data_acquire_cb( handle, STARPU_R,
                            (void (*)(void*))&starpu_data_release, handle );
}

#endif

void AL4SAN_Starpu_desc_flush( const AL4SAN_desc_t     *desc,
                         const AL4SAN_sequence_t *sequence )
{
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(desc->schedopt);
    int lmt = desc->lmt;
    int lnt = desc->lnt;
    int m, n;

    for (n = 0; n < lnt; n++)
    {
        for (m = 0; m < lmt; m++, handle++)
        {
            if ( *handle == NULL ) {
                continue;
            }

#if defined(AL4SAN_USE_MPI)
            starpu_mpi_cache_flush( MPI_COMM_WORLD, *handle );
#endif
            /*if ( al4san_desc_islocal( desc, m, n ) ) {
                al4san_starpu_data_wont_use( *handle );
            }*/
        }
    }

    (void)sequence;
}

void AL4SAN_Starpu_data_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int m, int n )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = n + (A->j / A->nb);

    starpu_data_handle_t *handle = A->schedopt;
    handle += ((int64_t)A->lmt) * nn + mm;

    if (*handle == NULL) {
        return;
    }

#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush( MPI_COMM_WORLD, *handle );
#endif

    if ( al4san_desc_islocal( A, m, n ) ) {
        al4san_starpu_data_wont_use( *handle );
    }

    (void)sequence;
}

void AL4SAN_Starpu_matrix_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int m, int n )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = n + (A->j / A->nb);

    starpu_data_handle_t *handle = A->schedopt;
    handle += ((int64_t)A->lmt) * nn + mm;

    if (*handle == NULL) {
        return;
    }

#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush( MPI_COMM_WORLD, *handle );
#endif

    if ( al4san_desc_islocal( A, m, n ) ) {
        al4san_starpu_data_wont_use( *handle );
    }

    (void)sequence;
}

void AL4SAN_Starpu_vector_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A, int m)
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = 0 + (A->j / A->nb);

    starpu_data_handle_t *handle = A->schedopt;
    handle += ((int64_t)A->lmt) * nn + mm;

    if (*handle == NULL) {
        return;
    }

#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush( MPI_COMM_WORLD, *handle );
#endif

    if ( al4san_desc_islocal( A, m, 0 ) ) {
        al4san_starpu_data_wont_use( *handle );
    }

    (void)sequence;
}

void AL4SAN_Starpu_scaler_flush( const AL4SAN_sequence_t *sequence,
                         const AL4SAN_desc_t *A)
{
    int64_t mm = 0 + (A->i / A->mb);
    int64_t nn = 0 + (A->j / A->nb);

    starpu_data_handle_t *handle = A->schedopt;
    handle += ((int64_t)A->lmt) * nn + mm;

    if (*handle == NULL) {
        return;
    }

#if defined(AL4SAN_USE_MPI)
    starpu_mpi_cache_flush( MPI_COMM_WORLD, *handle );
#endif

    if ( al4san_desc_islocal( A, 0, 0 ) ) {
        al4san_starpu_data_wont_use( *handle );
    }

    (void)sequence;
}
#if defined(AL4SAN_USE_MIGRATE)
void AL4SAN_Starpu_data_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
#if defined(HAVE_STARPU_MPI_DATA_MIGRATE)
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(A->schedopt);
    starpu_data_handle_t lhandle;
    handle += ((int64_t)(A->lmt) * (int64_t)An + (int64_t)Am);

    lhandle = *handle;
    if ( lhandle == NULL ) {
        /* Register the data */
        lhandle = AL4SAN_Starpu_data_getaddr( A, Am, An );
    }

    starpu_mpi_data_migrate( MPI_COMM_WORLD, lhandle, new_rank );

    (void)sequence;
#else
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
#endif
}

void AL4SAN_Starpu_matrix_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
#if defined(HAVE_STARPU_MPI_DATA_MIGRATE)
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(A->schedopt);
    starpu_data_handle_t lhandle;
    handle += ((int64_t)(A->lmt) * (int64_t)An + (int64_t)Am);

    lhandle = *handle;
    if ( lhandle == NULL ) {
        /* Register the data */
        lhandle = AL4SAN_Starpu_data_getaddr( A, Am, An );
    }

    starpu_mpi_data_migrate( MPI_COMM_WORLD, lhandle, new_rank );

    (void)sequence;
#else
    (void)sequence; (void)A; (void)Am; (void)An; (void)new_rank;
#endif
}

void AL4SAN_Starpu_vector_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int Am, int new_rank )
{
#if defined(HAVE_STARPU_MPI_DATA_MIGRATE)
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(A->schedopt);
    starpu_data_handle_t lhandle;
    handle += ((int64_t)(A->lmt) * (int64_t)0 + (int64_t)Am);

    lhandle = *handle;
    if ( lhandle == NULL ) {
        /* Register the data */
        lhandle = AL4SAN_Starpu_data_getaddr( A, Am, 0 );
    }

    starpu_mpi_data_migrate( MPI_COMM_WORLD, lhandle, new_rank );

    (void)sequence;
#else
    (void)sequence; (void)A; (void)Am; (void)new_rank;
#endif
}


void AL4SAN_Starpu_scaler_migrate( const AL4SAN_sequence_t *sequence,
                           const AL4SAN_desc_t *A, int new_rank )
{
#if defined(HAVE_STARPU_MPI_DATA_MIGRATE)
    starpu_data_handle_t *handle = (starpu_data_handle_t*)(A->schedopt);
    starpu_data_handle_t lhandle;
    handle += ((int64_t)(A->lmt) * (int64_t)0 + (int64_t)0);

    lhandle = *handle;
    if ( lhandle == NULL ) {
        /* Register the data */
        lhandle = AL4SAN_Starpu_data_getaddr( A, 0, 0 );
    }

    starpu_mpi_data_migrate( MPI_COMM_WORLD, lhandle, new_rank );

    (void)sequence;
#else
    (void)sequence; (void)A; (void)new_rank;
#endif
}

#endif

/**
 *  Get data addr
 */
/* For older revision of StarPU, STARPU_MAIN_RAM is not defined */
#ifndef STARPU_MAIN_RAM
#define STARPU_MAIN_RAM 0
#endif

void *AL4SAN_Starpu_data_getaddr( const AL4SAN_desc_t *A, int m, int n )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = n + (A->j / A->nb);

    starpu_data_handle_t *ptrtile = A->schedopt;
    ptrtile += ((int64_t)A->lmt) * nn + mm;

    if (*ptrtile == NULL) {
        int home_node = -1;
        void *user_ptr = NULL;
        int myrank = A->myrank;
        int owner  = A->get_rankof( A, m, n );
        int64_t eltsze = AL4SAN_Element_Size(A->dtyp);
        int tempmm = (mm == A->lmt-1) ? (A->lm - mm * A->mb) : A->mb;
        int tempnn = (nn == A->lnt-1) ? (A->ln - nn * A->nb) : A->nb;

        if ( myrank == owner ) {
            user_ptr = A->get_blkaddr(A, m, n);
            if ( user_ptr != NULL ) {
                home_node = STARPU_MAIN_RAM;
            }
        }

        starpu_matrix_data_register( ptrtile, home_node, (uintptr_t) user_ptr,
                                     A->get_blkldd( A, m ),
                                     tempmm, tempnn, eltsze );

#ifdef HAVE_STARPU_DATA_SET_COORDINATES
        starpu_data_set_coordinates( *ptrtile, 2, m, n );
#endif

#if defined(AL4SAN_USE_MPI)
        {
            int64_t block_ind = A->lmt * nn + mm;
            starpu_mpi_data_register(*ptrtile, (A->id << tag_sep) | (block_ind), owner);
        }
#endif /* defined(AL4SAN_USE_MPI) */
    }

    return *ptrtile;
}


void *AL4SAN_Starpu_matrix_getaddr( const AL4SAN_desc_t *A, int m, int n )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = n + (A->j / A->nb);

    starpu_data_handle_t *ptrtile = A->schedopt;
    ptrtile += ((int64_t)A->lmt) * nn + mm;

    if (*ptrtile == NULL) {
        int home_node = -1;
        void *user_ptr = NULL;
        int myrank = A->myrank;
        int owner  = A->get_rankof( A, m, n );
        int64_t eltsze = AL4SAN_Element_Size(A->dtyp);
        int tempmm = (mm == A->lmt-1) ? (A->lm - mm * A->mb) : A->mb;
        int tempnn = (nn == A->lnt-1) ? (A->ln - nn * A->nb) : A->nb;

        if ( myrank == owner ) {
            user_ptr = A->get_blkaddr(A, m, n);
            if ( user_ptr != NULL ) {
                home_node = STARPU_MAIN_RAM;
            }
        }

        starpu_matrix_data_register( ptrtile, home_node, (uintptr_t) user_ptr,
                                     A->get_blkldd( A, m ),
                                     tempmm, tempnn, eltsze );

#ifdef HAVE_STARPU_DATA_SET_COORDINATES
        starpu_data_set_coordinates( *ptrtile, 2, m, n );
#endif

#if defined(AL4SAN_USE_MPI)
        {
            int64_t block_ind = A->lmt * nn + mm;
            starpu_mpi_data_register(*ptrtile, (A->id << tag_sep) | (block_ind), owner);
        }
#endif /* defined(AL4SAN_USE_MPI) */
    }

    return *ptrtile;
}

void *AL4SAN_Starpu_vector_getaddr( const AL4SAN_desc_t *A, int m )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = 0 + (A->j / A->nb);

    starpu_data_handle_t *ptrtile = A->schedopt;
    ptrtile += ((int64_t)A->lmt) * nn + mm;

    if (*ptrtile == NULL) {
        int home_node = -1;
        void *user_ptr = NULL;
        int myrank = A->myrank;
        int owner  = A->get_rankof( A, m, 0 );
        int64_t eltsze = AL4SAN_Element_Size(A->dtyp);
        int tempmm = (mm == A->lmt-1) ? (A->lm - mm * A->mb) : A->mb;
        int tempnn = (nn == A->lnt-1) ? (A->ln - nn * A->nb) : A->nb;

        if ( myrank == owner ) {
            user_ptr = A->get_blkaddr(A, m, 0);
            if ( user_ptr != NULL ) {
                home_node = STARPU_MAIN_RAM;
            }
        }

        starpu_matrix_data_register( ptrtile, home_node, (uintptr_t) user_ptr,
                                     A->get_blkldd( A, m ),
                                     tempmm, tempnn, eltsze );

#ifdef HAVE_STARPU_DATA_SET_COORDINATES
        starpu_data_set_coordinates( *ptrtile, 2, m, 0 );
#endif

#if defined(AL4SAN_USE_MPI)
        {
            int64_t block_ind = A->lmt * nn + mm;
            starpu_mpi_data_register(*ptrtile, (A->id << tag_sep) | (block_ind), owner);
        }
#endif /* defined(AL4SAN_USE_MPI) */
    }

    return *ptrtile;
}

void *AL4SAN_Starpu_scaler_getaddr( const AL4SAN_desc_t *A)
{
    int64_t mm = 0 + (A->i / A->mb);
    int64_t nn = 0 + (A->j / A->nb);

    starpu_data_handle_t *ptrtile = A->schedopt;
    ptrtile += ((int64_t)A->lmt) * nn + mm;

    if (*ptrtile == NULL) {
        int home_node = -1;
        void *user_ptr = NULL;
        int myrank = A->myrank;
        int owner  = A->get_rankof( A, 0, 0 );
        int64_t eltsze = AL4SAN_Element_Size(A->dtyp);
        int tempmm = (mm == A->lmt-1) ? (A->lm - mm * A->mb) : A->mb;
        int tempnn = (nn == A->lnt-1) ? (A->ln - nn * A->nb) : A->nb;

        if ( myrank == owner ) {
            user_ptr = A->get_blkaddr(A, 0, 0);
            if ( user_ptr != NULL ) {
                home_node = STARPU_MAIN_RAM;
            }
        }

        starpu_matrix_data_register( ptrtile, home_node, (uintptr_t) user_ptr,
                                     A->get_blkldd( A, 0 ),
                                     tempmm, tempnn, eltsze );

#ifdef HAVE_STARPU_DATA_SET_COORDINATES
        starpu_data_set_coordinates( *ptrtile, 2, 0, 0 );
#endif

#if defined(AL4SAN_USE_MPI)
        {
            int64_t block_ind = A->lmt * nn + mm;
            starpu_mpi_data_register(*ptrtile, (A->id << tag_sep) | (block_ind), owner);
        }
#endif /* defined(AL4SAN_USE_MPI) */
    }

    return *ptrtile;
}

void AL4SAN_Starpu_data_getaddr_ptr( const AL4SAN_desc_t *A, void **ptr, int m, int n )
{
    int64_t mm = m + (A->i / A->mb);
    int64_t nn = n + (A->j / A->nb);

    starpu_data_handle_t *ptrtile = A->schedopt;
    ptrtile += ((int64_t)A->lmt) * nn + mm;

    if (*ptrtile == NULL) {
        int home_node = -1;
        void *user_ptr = NULL;
        int myrank = A->myrank;
        int owner  = A->get_rankof( A, m, n );
        int64_t eltsze = AL4SAN_Element_Size(A->dtyp);
        int tempmm = (mm == A->lmt-1) ? (A->lm - mm * A->mb) : A->mb;
        int tempnn = (nn == A->lnt-1) ? (A->ln - nn * A->nb) : A->nb;

        if ( myrank == owner ) {
            user_ptr = A->get_blkaddr(A, m, n);
            if ( user_ptr != NULL ) {
                home_node = STARPU_MAIN_RAM;
            }
        }

        starpu_matrix_data_register( ptrtile, home_node, (uintptr_t) user_ptr,
                                     A->get_blkldd( A, m ),
                                     tempmm, tempnn, eltsze );

#ifdef HAVE_STARPU_DATA_SET_COORDINATES
        starpu_data_set_coordinates( *ptrtile, 2, m, n );
#endif

#if defined(AL4SAN_USE_MPI)
        {
            int64_t block_ind = A->lmt * nn + mm;
            starpu_mpi_data_register(*ptrtile, (A->id << tag_sep) | (block_ind), owner);
        }
#endif /* defined(AL4SAN_USE_MPI) */
    }

    *ptr=*ptrtile;
}
