/**
 *
 * @file descriptor.c
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 ***
 *
 * @brief Al4san descriptors routines
 *
 * @version 1.0.1
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2019-02-06
 * @version 1.0.1
 * @author Rabab Alomairy
 * @date 2019-02-06
 ***
 *
 * @defgroup Descriptor
 * @brief Group descriptor routines exposed to users
 *
 */
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "control/al4san_descriptor.h"
#include "control/al4san_common.h"
#include "al4san/runtime.h"
#include "al4san/constants.h"
static int nbdesc = 0;

/**
 *
 */
int al4san_desc_mat_alloc( AL4SAN_desc_t *desc )
{
    size_t size = (size_t)(desc->llm) * (size_t)(desc->lln)
        * (size_t)AL4SAN_Element_Size(desc->dtyp);

    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_rank()", "AL4SAN not initialized");
        return -1;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     desc->mat = AL4SAN_Quark_malloc(size);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         desc->mat = AL4SAN_Starpu_malloc(size);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         desc->mat = AL4SAN_Parsec_malloc(size);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    desc->mat = AL4SAN_Openmp_malloc(size);
#endif

    if (desc->mat == NULL) {
        al4san_error("al4san_desc_mat_alloc", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    /* The matrix has already been registered by the Runtime alloc */
    desc->register_mat = 0;

    return AL4SAN_SUCCESS;
}

/**
 *
 */
int al4san_desc_mat_free( AL4SAN_desc_t *desc )
{
    if ( (desc->mat       != NULL) &&
         (desc->use_mat   == 1   ) &&
         (desc->alloc_mat == 1   ) )
    {
 
     AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Comm_rank()", "AL4SAN not initialized");
        return -1;
    }
    
    size_t size = (size_t)(desc->llm) * (size_t)(desc->lln)
            * (size_t)AL4SAN_Element_Size(desc->dtyp);

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_free(desc->mat, size);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_free(desc->mat, size);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_free(desc->mat, size);
#endif
#ifdef AL4SAN_SCHED_OPENMP
    if(al4san->scheduler==3)
     AL4SAN_Openmp_free(desc->mat, size);
#endif
        desc->mat = NULL;
    }

    return AL4SAN_SUCCESS;
}

/**
 ******************************************************************************
 *
 * @ingroup Descriptor
 *
 * @brief Internal function to create tiled matrix descriptor
 * with generic function for data distribution and storage format.
 *
 ******************************************************************************
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] mb
 *          Number of rows in a tile.
 *
 * @param[in] nb
 *          Number of columns in a tile.
 *
 * @param[in] bsiz
 *          Size in bytes including padding.
 *
 * @param[in] lm
 *          Number of rows of the entire matrix.
 *
 * @param[in] ln
 *          Number of columns of the entire matrix.
 *
 * @param[in] i
 *          Row index to the beginning of the submatrix.
 *
 * @param[in] j
 *          Column indes to the beginning of the submatrix.
 *
 * @param[in] m
 *          Number of rows of the submatrix.
 *
 * @param[in] n
 *          Number of columns of the submatrix.
 *
 * @param[in] p
 *          2D-block cyclic distribution in rows.
 *
 * @param[in] q
 *          2D-block cyclic distribution in columns.
 *
 * @param[in] (*get_blkaddr)( const AL4SAN_desc_t *A, int m, int n)
 *          A function which return the address of the data corresponding to
 *          the tile A(m,n).
 *
 * @param[in] (*get_blkldd)( const AL4SAN_desc_t *A, int m )
 *          A function that return the leading dimension of the tile A(m,*).
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return  The descriptor with the matrix description parameters set.
 *
 */
int al4san_desc_init( AL4SAN_desc_t *desc, void *mat,
                         al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                         int lm, int ln, int i, int j,
                         int m,  int n,  int p, int q,
                         void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                         int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                         int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) )
{
    AL4SAN_context_t *al4san;
    int rc = AL4SAN_SUCCESS;

    memset( desc, 0, sizeof(AL4SAN_desc_t) );

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Create", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    // If one of the function get_* is NULL, we switch back to the default, like in al4san_desc_init()
    desc->get_blkaddr = get_blkaddr ? get_blkaddr : al4san_getaddr_ccrb;
    desc->get_blkldd  = get_blkldd  ? get_blkldd  : al4san_getblkldd_ccrb;
    desc->get_rankof  = get_rankof  ? get_rankof  : al4san_getrankof_2d;
    // Matrix properties
    desc->dtyp = dtyp;
    // Should be given as parameter to follow get_blkaddr (unused)
    desc->styp = Al4sanCCRB;
    desc->mb   = mb;
    desc->nb   = nb;
    desc->bsiz = bsiz;
    // Large matrix parameters
    desc->lm = lm;
    desc->ln = ln;
    // Large matrix derived parameters
    desc->lmt = (lm%mb==0) ? (lm/mb) : (lm/mb+1);
    desc->lnt = (ln%nb==0) ? (ln/nb) : (ln/nb+1);
    // Submatrix parameters
    desc->i = i;
    desc->j = j;
    desc->m = m;
    desc->n = n;
    // Submatrix derived parameters
    desc->mt = (m == 0) ? 0 : (i+m-1)/mb - i/mb + 1;
    desc->nt = (n == 0) ? 0 : (j+n-1)/nb - j/nb + 1;

    desc->id = nbdesc;
    nbdesc++;
    desc->occurences = 0;

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     desc->myrank = AL4SAN_Quark_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         desc->myrank = AL4SAN_Starpu_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         desc->myrank = AL4SAN_Parsec_comm_rank( al4san );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    desc->myrank = AL4SAN_Openmp_comm_rank( al4san );
#endif
//    desc->myrank = AL4SAN_Runtime_comm_rank( al4san );

    // Grid size
    desc->p = p;
    desc->q = q;

    int rank, size;
#if defined(AL4SAN_USE_MPI)
    MPI_Comm comm;
#endif
    int dim[2], period[2], reorder;
    int coord[2], id;


    /*dim[0]=p; dim[1]=q;
    period[0]=1; period[1]=0;
    reorder=1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
    chamctxt->newcomm=comm;*/
#if defined(AL4SAN_USE_MPI)
    MPI_Cart_coords(al4san->newcomm, desc->myrank, 2, coord);
#endif
    //printf("\n Me:%d, i:%d, j:%d\n", desc->myrank, coord[0], coord[1]);

if ( desc->myrank < (p*q) ) {
     int i0=0;
     //Figure process distance from source process
     int mydist_rows=(p+coord[0]-i0)%p;
     int mydist_cols=(q+coord[1]-i0)%q;
     
     //Figure the total number of whole complete bloaks
     int MBLOCKS=desc->lm/mb;
     int NBLOCKS=desc->ln/nb;
     
    //Figure min number of rows/cols a process can have
     int numroc_row=(MBLOCKS/p)*mb;
     int numroc_cols=(NBLOCKS/q)*nb;

    //See if there are an extra balocks
    int extra_rows=MBLOCKS%p;
    int extra_cols=NBLOCKS%q;

   //Rows
   if(mydist_rows<extra_rows) numroc_row=numroc_row+mb;
   else if(mydist_rows==extra_rows) numroc_row=numroc_row+(desc->lm%mb);

   //cols
   if(mydist_cols<extra_cols) numroc_cols=numroc_cols+nb;
   else if(mydist_cols==extra_cols) numroc_cols=numroc_cols+(desc->ln%nb);

   desc->llm=numroc_row;
   desc->lln=numroc_cols;
   desc->llm1 = (desc->llm/mb);
   desc->lln1 = (desc->lln/nb);
   desc->llmt = (desc->lmt + p - 1) / p;
   desc->llnt = (desc->lnt + q - 1) / q;
}
else
{
        desc->llmt = 0;
        desc->llnt = 0;
        desc->llm  = 0;
        desc->lln  = 0;
        desc->llm1 = 0;
        desc->lln1 = 0;
}
    // Local dimensions in tiles
    /*if ( desc->myrank < (p*q) ) {
        desc->llmt = (desc->lmt + p - 1) / p;
        desc->llnt = (desc->lnt + q - 1) / q;

        // Local dimensions
        if ( ((desc->lmt-1) % p) == (desc->myrank / q) ) {
            desc->llm  = ( desc->llmt - 1 ) * mb + ((lm%mb==0) ? mb : (lm%mb));
        } else {
            desc->llm  =  desc->llmt * mb;
        }

        if ( ((desc->lnt-1) % q) == (desc->myrank % q) ) {
            desc->lln  = ( desc->llnt - 1 ) * nb + ((ln%nb==0) ? nb : (ln%nb));
        } else {
            desc->lln  =  desc->llnt * nb;
        }

        desc->llm1 = (desc->llm/mb);
        desc->lln1 = (desc->lln/nb);
    } else {
        desc->llmt = 0;
        desc->llnt = 0;
        desc->llm  = 0;
        desc->lln  = 0;
        desc->llm1 = 0;
        desc->lln1 = 0;
    }*/

    /* memory of the matrix is handled by the user */
    desc->alloc_mat    = 0;
    /* if the user gives a pointer to the overall data (tiles) we can use it */
    desc->use_mat      = 0;
    /* users data can have multiple forms: let him register tiles */
    desc->register_mat = 0;
    /* The matrix is alocated tile by tile with out of core */
    desc->ooc = 0;

    // Matrix address
    if ( mat == AL4SAN_MAT_ALLOC_GLOBAL ) {
        rc = al4san_desc_mat_alloc( desc );

        desc->alloc_mat = 1;
        desc->use_mat   = 1;
    }
    else if ( mat == AL4SAN_MAT_ALLOC_TILE ) {
        //al4san_error( "al4san_desc_init", "AL4SAN_MAT_ALLOC_TILE is not available yet" );
        //desc->mat = NULL;
        rc = al4san_desc_mat_alloc( desc );
        desc->use_mat   = 1;

        desc->alloc_mat = 1;
    }
    else if ( mat == AL4SAN_MAT_OOC ) {
        desc->mat = NULL;
        desc->ooc = 1;
    }
    else {
        /* memory of the matrix is handled by users */
        desc->mat     = mat;
        desc->use_mat = 1;
    }

    desc->A21 = (size_t)(desc->llm - desc->llm%mb)*(size_t)(desc->lln - desc->lln%nb);
    desc->A12 = (size_t)(            desc->llm%mb)*(size_t)(desc->lln - desc->lln%nb) + desc->A21;
    desc->A22 = (size_t)(desc->llm - desc->llm%mb)*(size_t)(            desc->lln%nb) + desc->A12;


#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_desc_create( desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_desc_create( desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_desc_create( desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
        AL4SAN_Openmp_desc_create( desc );
#endif

    /* Create runtime specific structure like registering data */
 //   AL4SAN_Runtime_desc_create( desc );

    return rc;
}

/**
 *  Internal static descriptor initializer for submatrices
 */
AL4SAN_desc_t* al4san_desc_submatrix(AL4SAN_desc_t *descA, int i, int j, int m, int n)
{
    AL4SAN_desc_t *descB = malloc(sizeof(AL4SAN_desc_t));
    int mb, nb;

    if ( (descA->i + i + m) > descA->lm ) {
        al4san_error("al4san_desc_submatrix", "The number of rows (i+m) of the submatrix doesn't fit in the parent matrix");
        assert((descA->i + i + m) > descA->lm);
    }
    if ( (descA->j + j + n) > descA->ln ) {
        al4san_error("al4san_desc_submatrix", "The number of rows (j+n) of the submatrix doesn't fit in the parent matrix");
        assert((descA->j + j + n) > descA->ln);
    }

    memcpy( descB, descA, sizeof(AL4SAN_desc_t) );
    mb = descA->mb;
    nb = descA->nb;
    // Submatrix parameters
    descB->i = descA->i + i;
    descB->j = descA->j + j;
    descB->m = m;
    descB->n = n;
    // Submatrix derived parameters
    descB->mt = (m == 0) ? 0 : (descB->i+m-1)/mb - descB->i/mb + 1;
    descB->nt = (n == 0) ? 0 : (descB->j+n-1)/nb - descB->j/nb + 1;

    // Increase the number of occurences to avoid multiple free of runtime specific data structures.
    descB->occurences++;

    return descB;
}

void al4san_desc_destroy( AL4SAN_desc_t *desc )
{
    AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("al4san_desc_destroy", "AL4SAN not initialized");
        //return -1;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_desc_destroy( desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_desc_destroy( desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_desc_destroy( desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
        AL4SAN_Openmp_desc_destroy( desc );
#endif

    //AL4SAN_Runtime_desc_destroy( desc );
    al4san_desc_mat_free( desc );
}

/**
 *  Check for descriptor correctness
 */
int al4san_desc_check(const AL4SAN_desc_t *desc)
{
    if (desc == NULL) {
        al4san_error("al4san_desc_check", "NULL descriptor");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }
    if (desc->mat == NULL && desc->use_mat == 1) {
        al4san_error("al4san_desc_check", "NULL matrix pointer");
        return AL4SAN_ERR_UNALLOCATED;
    }
    if (desc->dtyp != Al4sanRealFloat &&
        desc->dtyp != Al4sanRealDouble &&
        desc->dtyp != Al4sanComplexFloat &&
        desc->dtyp != Al4sanComplexDouble  ) {
        al4san_error("al4san_desc_check", "invalid matrix type");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if (desc->mb <= 0 || desc->nb <= 0) {
        al4san_error("al4san_desc_check", "negative tile dimension");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if (desc->bsiz < desc->mb*desc->nb) {
        al4san_error("al4san_desc_check", "tile memory size smaller than the product of dimensions");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if (desc->lm <= 0 || desc->ln <= 0) {
        al4san_error("al4san_desc_check", "negative matrix dimension");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if ((desc->lm < desc->m) || (desc->ln < desc->n)) {
        al4san_error("al4san_desc_check", "matrix dimensions larger than leading dimensions");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if ((desc->i > 0 && desc->i >= desc->lm) || (desc->j > 0 && desc->j >= desc->ln)) {
        al4san_error("al4san_desc_check", "beginning of the matrix out of scope");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    if (desc->i+desc->m > desc->lm || desc->j+desc->n > desc->ln) {
        al4san_error("al4san_desc_check", "submatrix out of scope");
        return AL4SAN_ERR_ILLEGAL_VALUE;
    }
    return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Create - Create tiled matrix descriptor.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
	 *
	 * @param[in] mat
 *          Memory location of the matrix. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] mb
 *          Number of rows in a tile.
 *
 * @param[in] nb
 *          Number of columns in a tile.
 *
 * @param[in] bsiz
 *          Size in bytes including padding.
 *
 * @param[in] lm
 *          Number of rows of the entire matrix.
 *
 * @param[in] ln
 *          Number of columns of the entire matrix.
 *
 * @param[in] i
 *          Row index to the beginning of the submatrix.
 *
 * @param[in] j
 *          Column indes to the beginning of the submatrix.
 *
 * @param[in] m
 *          Number of rows of the submatrix.
 *
 * @param[in] n
 *          Number of columns of the submatrix.
 *
 * @param[in] p
 *          2D-block cyclic distribution in rows.
 *
 * @param[in] q
 *          2D-block cyclic distribution in columns.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Create( AL4SAN_desc_t **descptr, void *mat, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                           int lm, int ln, int i, int j, int m, int n, int p, int q )
{
    return AL4SAN_Desc_Create_User( descptr, mat, dtyp, mb, nb, bsiz,
                                       lm, ln, i, j, m, n, p, q,
                                       NULL, NULL, NULL );
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Matrix_Create - Create tiled matrix descriptor.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
 *
 * @param[in] mat
 *          Memory location of the matrix. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] mb
 *          Number of rows in a tile.
 *
 * @param[in] nb
 *          Number of columns in a tile.
 *
 * @param[in] bsiz
 *          Size in bytes including padding.
 *
 * @param[in] lm
 *          Number of rows of the entire matrix.
 *
 * @param[in] ln
 *          Number of columns of the entire matrix.
 *
 * @param[in] i
 *          Row index to the beginning of the submatrix.
 *
 * @param[in] j
 *          Column indes to the beginning of the submatrix.
 *
 * @param[in] m
 *          Number of rows of the submatrix.
 *
 * @param[in] n
 *          Number of columns of the submatrix.
 *
 * @param[in] p
 *          2D-block cyclic distribution in rows.
 *
 * @param[in] q
 *          2D-block cyclic distribution in columns.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Matrix_Create( AL4SAN_desc_t **descptr, void *mat, al4san_flttype_t dtyp, int order, int mb, int nb, 
                           int bld, int lm, int ln, int ld) 
{
      AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Matrix_Create", "AL4SAN not initialized");
        //return -1;
    }
#if defined(AL4SAN_USE_MPI)
    return AL4SAN_Desc_Create_User( descptr, mat, dtyp, mb, nb, mb*nb,
                                       lm, ln, 0, 0, lm, ln, al4san->prows, al4san->pcols,
                                        NULL, NULL, NULL );
#else
    return AL4SAN_Desc_Create_User( descptr, mat, dtyp, mb, nb, mb*nb,
                                       lm, ln, 0, 0, lm, ln, 1, 1,
                                        NULL, NULL, NULL );
#endif
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Vector_Create - Create Vector descriptor.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the Vector.
 *
 * @param[in] vec
 *          Memory location of the Vector. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the Vector:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] mb
 *          Number of rows in a tile.
 *
 * @param[in] bsiz
 *          Size in bytes including padding.
 *
 * @param[in] lm
 *          Number of rows of the entire Vector.
 *
 * @param[in] i
 *          Row index to the beginning of the subvector.
 *
 * @param[in] m
 *          Number of rows of the subvector.
 *
 * @param[in] p
 *          2D-block cyclic distribution in rows.
 *
 * @param[in] q
 *          2D-block cyclic distribution in columns.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */

int AL4SAN_Vector_Create( AL4SAN_desc_t **descptr, void *vec, al4san_flttype_t dtyp, int mb, 
                           int lm)
{
      AL4SAN_context_t *al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Matrix_Create", "AL4SAN not initialized");
        //return -1;
    }
#if defined(AL4SAN_USE_MPI)
    return AL4SAN_Desc_Create_User( descptr, vec, dtyp, mb, 1, mb,
                                       lm, 1, 0, 0, lm, 1, al4san->prows, al4san->pcols,
                                       NULL, NULL, NULL );
#else
    return AL4SAN_Desc_Create_User( descptr, vec, dtyp, mb, 1, mb,
                                       lm, 1, 0, 0, lm, 1, 1, 1,
                                       NULL, NULL, NULL );
#endif
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Scaler_Create - Create Scaler Descriptor.
 *
 ******************************************************************************
 *
 * @param[out] descptr
 *          On exit, descriptor of the Scaler.
 *
 * @param[in] scaler
 *          Memory location of the Scaler. If scaler is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] i
 *          Row index to the beginning of the Scaler.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */


int AL4SAN_Scaler_Create( AL4SAN_desc_t **descptr, void *scaler, al4san_flttype_t dtyp)
{
    return AL4SAN_Desc_Create_User( descptr, scaler, dtyp, 1, 1, 1,
                                       1, 1, 0, 0, 1, 1, 1, 1,
                                       NULL, NULL, NULL );
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Create_User - Create generic tiled matrix descriptor for general
 *  applications.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
 *
 * @param[in] mat
 *          Memory location of the matrix. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] nb
 *          Number of rows and columns in a tile.
 *
 * @param[in] m
 *          Number of rows of the entire matrix.
 *
 * @param[in] n
 *          Number of columns of the entire matrix.
 *
 * @param[in] p
 *          2d-block cyclic partitioning, number of tiles in rows.
 *
 * @param[in] q
 *          2d-block cyclic partitioning, number of tiles in columns.
 *
 * @param[in] (*get_blkaddr)( const AL4SAN_desc_t *A, int m, int n)
 *          A function which return the address of the data corresponding to
 *          the tile A(m,n).
 *
 * @param[in] (*get_blkldd)( const AL4SAN_desc_t *A, int m)
 *          A function that return the leading dimension of the tile A(m,*).
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Create_User( AL4SAN_desc_t **descptr, void *mat, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                                int lm, int ln, int i, int j, int m, int n, int p, int q,
                                void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                                int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                                int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) )
{
    AL4SAN_context_t *al4san;
    AL4SAN_desc_t *desc;
    int status;

    *descptr = NULL;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    /* Allocate memory and initialize the descriptor */
    desc = (AL4SAN_desc_t*)malloc(sizeof(AL4SAN_desc_t));
    if (desc == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    al4san_desc_init( desc, mat, dtyp, mb, nb, bsiz,
                         lm, ln, i, j, m, n, p, q,
                         get_blkaddr, get_blkldd, get_rankof );

    status = al4san_desc_check( desc );
    if (status != AL4SAN_SUCCESS) {
        al4san_error("AL4SAN_Desc_Create_User", "invalid descriptor");
        AL4SAN_Desc_Destroy( &desc );
        return status;
    }

    *descptr = desc;
    return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Create_User - Create generic tiled matrix descriptor for general
 *  applications.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
 *
 * @param[in] mat
 *          Memory location of the matrix. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] nb
 *          Number of rows and columns in a tile.
 *
 * @param[in] m
 *          Number of rows of the entire matrix.
 *
 * @param[in] n
 *          Number of columns of the entire matrix.
 *
 * @param[in] p
 *          2d-block cyclic partitioning, number of tiles in rows.
 *
 * @param[in] q
 *          2d-block cyclic partitioning, number of tiles in columns.
 *
 * @param[in] (*get_blkaddr)( const AL4SAN_desc_t *A, int m, int n)
 *          A function which return the address of the data corresponding to
 *          the tile A(m,n).
 *
 * @param[in] (*get_blkldd)( const AL4SAN_desc_t *A, int m)
 *          A function that return the leading dimension of the tile A(m,*).
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Matrix_Create_User( AL4SAN_desc_t **descptr, void *mat, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                                int lm, int ln, int i, int j, int m, int n, int p, int q,
                                void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                                int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                                int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) )
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_desc_t *desc;
    int status;

    *descptr = NULL;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    /* Allocate memory and initialize the descriptor */
    desc = (AL4SAN_desc_t*)malloc(sizeof(AL4SAN_desc_t));
    if (desc == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    al4san_desc_init( desc, mat, dtyp, mb, nb, bsiz,
                         lm, ln, i, j, m, n, p, q,
                         get_blkaddr, get_blkldd, get_rankof );

    status = al4san_desc_check( desc );
    if (status != AL4SAN_SUCCESS) {
        al4san_error("AL4SAN_Desc_Create_User", "invalid descriptor");
        AL4SAN_Desc_Destroy( &desc );
        return status;
    }

    *descptr = desc;
    return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Vector_Create_User - Create generic blocked vector descriptor for general
 *  applications.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the vector.
 *
 * @param[in] mat
 *          Memory location of the vector. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the vector:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] m
 *          Number of rows of the entire vector.
 *
 * @param[in] p
 *          2d-block cyclic partitioning, number of tiles in rows.
 *
 * @param[in] q
 *          2d-block cyclic partitioning, number of tiles in columns.
 *
 * @param[in] (*get_blkaddr)( const AL4SAN_desc_t *A, int m, int n)
 *          A function which return the address of the data corresponding to
 *          the tile A(m,n).
 *
 * @param[in] (*get_blkldd)( const AL4SAN_desc_t *A, int m)
 *          A function that return the leading dimension of the tile A(m,*).
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Vector_Create_User( AL4SAN_desc_t **descptr, void *vec, al4san_flttype_t dtyp, int mb, int bsiz,
                                int lm, int i, int m, int p, int q,
                                void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                                int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                                int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) )
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_desc_t *desc;
    int status;

    *descptr = NULL;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    /* Allocate memory and initialize the descriptor */
    desc = (AL4SAN_desc_t*)malloc(sizeof(AL4SAN_desc_t));
    if (desc == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    al4san_desc_init( desc, vec, dtyp, mb, 1, bsiz,
                         lm, 1, i, 0, m, 1, p, q,
                         get_blkaddr, get_blkldd, get_rankof );

    status = al4san_desc_check( desc );
    if (status != AL4SAN_SUCCESS) {
        al4san_error("AL4SAN_Desc_Create_User", "invalid descriptor");
        AL4SAN_Desc_Destroy( &desc );
        return status;
    }

    *descptr = desc;
    return AL4SAN_SUCCESS;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Scaler_Create_User - Create generic scaler descriptor for general
 *  applications.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the vector.
 *
 * @param[in] scaler
 *          Memory location of the scaler. If mat is NULL, the space to store
 *          the data is automatically allocated by the call to the function.
 *
 * @param[in] dtyp
 *          Data type of the vector:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 *
 * @param[in] (*get_blkaddr)( const AL4SAN_desc_t *A, int m, int n)
 *          A function which return the address of the data corresponding to
 *          the tile A(m,n).
 *
 * @param[in] (*get_blkldd)( const AL4SAN_desc_t *A, int m)
 *          A function that return the leading dimension of the tile A(m,*).
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Scaler_Create_User( AL4SAN_desc_t **descptr, void *scaler, al4san_flttype_t dtyp, int i, 
                                void* (*get_blkaddr)( const AL4SAN_desc_t*, int, int ),
                                int   (*get_blkldd) ( const AL4SAN_desc_t*, int      ),
                                int   (*get_rankof) ( const AL4SAN_desc_t*, int, int ) )
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_desc_t *desc;
    int status;

    *descptr = NULL;

    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    /* Allocate memory and initialize the descriptor */
    desc = (AL4SAN_desc_t*)malloc(sizeof(AL4SAN_desc_t));
    if (desc == NULL) {
        al4san_error("AL4SAN_Desc_Create_User", "malloc() failed");
        return AL4SAN_ERR_OUT_OF_RESOURCES;
    }

    al4san_desc_init( desc, scaler, dtyp, 1, 1, 1,
                         1, 1, i, 0, 1, 1, 1, 1,
                         get_blkaddr, get_blkldd, get_rankof );

    status = al4san_desc_check( desc );
    if (status != AL4SAN_SUCCESS) {
        al4san_error("AL4SAN_Desc_Create_User", "invalid descriptor");
        AL4SAN_Desc_Destroy( &desc );
        return status;
    }

    *descptr = desc;
    return AL4SAN_SUCCESS;
}


/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Create_OOC_User - Create matrix descriptor for tiled matrix which
 *  may not fit memory.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] nb
 *          Number of rows and columns in a tile.
 *
 * @param[in] m
 *          Number of rows of the entire matrix.
 *
 * @param[in] n
 *          Number of columns of the entire matrix.
 *
 * @param[in] p
 *          2d-block cyclic partitioning, number of tiles in rows.
 *
 * @param[in] q
 *          2d-block cyclic partitioning, number of tiles in columns.
 *
 * @param[in] (*get_rankof)( const AL4SAN_desc_t *A, int m, int n)
 *          A function that return the MPI rank of the tile A(m,n).
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Create_OOC_User(AL4SAN_desc_t **descptr, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                                   int lm, int ln, int i, int j, int m, int n, int p, int q,
                                   int (*get_rankof)( const AL4SAN_desc_t*, int, int ))
{
#if !defined (AL4SAN_SCHED_STARPU)
    (void)descptr; (void)dtyp; (void)mb; (void)nb; (void)bsiz;
    (void)lm; (void)ln; (void)i; (void)j; (void)m; (void)n; (void)p; (void)q;
    (void)get_rankof;

    al4san_error("AL4SAN_Desc_Create_OOC_User", "Only StarPU supports on-demand tile allocation");
    return AL4SAN_ERR_NOT_SUPPORTED;
#else
    int rc;
    rc = AL4SAN_Desc_Create_User( descptr, AL4SAN_MAT_OOC, dtyp, mb, nb, bsiz,
                                     lm, ln, i, j, m, n, p, q,
                                     al4san_getaddr_null, NULL, get_rankof );
    return rc;
#endif
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Create_OOC - Create matrix descriptor for tiled matrix which may
 *  not fit memory.
 *
 ******************************************************************************
 *
 * @param[out] desc
 *          On exit, descriptor of the matrix.
 *
 * @param[in] dtyp
 *          Data type of the matrix:
 *          @arg Al4sanRealFloat:     single precision real (S),
 *          @arg Al4sanRealDouble:    double precision real (D),
 *          @arg Al4sanComplexFloat:  single precision complex (C),
 *          @arg Al4sanComplexDouble: double precision complex (Z).
 *
 * @param[in] nb
 *          Number of rows and columns in a tile.
 *
 * @param[in] m
 *          Number of rows of the entire matrix.
 *
 * @param[in] n
 *          Number of columns of the entire matrix.
 *
 * @param[in] p
 *          2d-block cyclic partitioning, number of tiles in rows.
 *
 * @param[in] q
 *          2d-block cyclic partitioning, number of tiles in columns.
 *
 ******************************************************************************
 *
 * @return
 *          \retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Create_OOC(AL4SAN_desc_t **descptr, al4san_flttype_t dtyp, int mb, int nb, int bsiz,
                              int lm, int ln, int i, int j, int m, int n, int p, int q)
{
    return AL4SAN_Desc_Create_User( descptr, AL4SAN_MAT_OOC, dtyp, mb, nb, bsiz,
                                       lm, ln, i, j, m, n, p, q,
                                       al4san_getaddr_null, NULL, NULL );
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Destroy - Destroys matrix descriptor.
 *
 ******************************************************************************
 *
 * @param[in] desc
 *          Matrix descriptor.
 *
 ******************************************************************************
 *
 * @retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Destroy(AL4SAN_desc_t **desc)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    if (*desc == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "attempting to destroy a NULL descriptor");
        return AL4SAN_ERR_UNALLOCATED;
    }

    al4san_desc_destroy( *desc );
    free(*desc);
    *desc = NULL;
    return AL4SAN_SUCCESS;
}

int AL4SAN_Desc_Discharge(AL4SAN_desc_t **desc)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    if (*desc == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "attempting to destroy a NULL descriptor");
        return AL4SAN_ERR_UNALLOCATED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_desc_destroy( *desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_desc_destroy( *desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_desc_destroy( *desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     AL4SAN_Openmp_desc_destroy( *desc );
#endif

    return AL4SAN_SUCCESS;
}

/**
 ***
 * @brief Allocate size bytes through the runtime memory management system if any, or with malloc otherwise.
 *
 * This function allows to allocate pinned memory if needed, or eventually not
 * perform the allocation in simulation mode.
 * @param[in, out] A
 *            Pointer to allocated memory
 * @param[in] size
 *            The number of bytes to allocate.
 *
 * @return The pointer to allocated area of size bytes on success, NULL otherwise.
 */

void AL4SAN_Malloc_ptr(void**A, size_t size)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     *A = AL4SAN_Quark_malloc(size);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         *A = AL4SAN_Starpu_malloc(size);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         *A = AL4SAN_Parsec_malloc(size);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     *A = AL4SAN_Openmp_malloc(size);
#endif

}

/**
 * @brief Free allocated memory through AL4SAN_Free() function call
 *
 * @param[in,out] ptr
 *            The ptr to free
 *
 * @param[in] size
 *            The size in bytes of the allocated area associated to ptr.
 */
void AL4SAN_Free( void  *ptr, size_t size )
{
     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_free(ptr, size);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_free(ptr, size);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_free(ptr, size);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     AL4SAN_Openmp_free(ptr, size);
#endif
return;
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Acquire - Ensures that all data of the descriptor are
 *  up-to-date.
 *
 ******************************************************************************
 *
 * @param[in] desc
 *          Matrix descriptor.
 *
 ******************************************************************************
 *
 * @retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Acquire (AL4SAN_desc_t  *desc) {

     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     return AL4SAN_Quark_desc_acquire( desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         return AL4SAN_Starpu_desc_acquire( desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_desc_acquire( desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
     return AL4SAN_Openmp_desc_acquire( desc );
#endif

}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Release - Release the data of the descriptor acquired by the
 *  application. Should be called if AL4SAN_Desc_Acquire has been called on the
 *  descriptor and if you do not need to access to its data anymore.
 *
 ******************************************************************************
 *
 * @param[in] desc
 *          Matrix descriptor.
 *
 ******************************************************************************
 *
 * @retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Release (AL4SAN_desc_t  *desc) {
     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     return AL4SAN_Quark_desc_release( desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         return AL4SAN_Starpu_desc_release( desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_desc_release( desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    return AL4SAN_Openmp_desc_release( desc );
#endif

    //return AL4SAN_Runtime_desc_release( desc );
}

/**
 * @brief Flush all data submitted to the runtime systems from the distributed
 * cache, and/or mark for eviction from the GPU memory.
 *
 * This function flushes all data from the distributed cache of the runtime system.
 */

void AL4SAN_Flush()
{
     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_flush();
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_flush();
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_flush();
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_flush();
#endif

    return;    
}

/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_Desc_Flush - Flushes the data in the sequence when they won't be
 *  reused. This calls cleans up the distributed communication caches, and
 *  transfer the data back to the CPU.
 *
 ******************************************************************************
 *
 * @param[in] desc
 *          Matrix descriptor.
 *
 ******************************************************************************
 *
 * @retval AL4SAN_SUCCESS successful exit
 *
 */
int AL4SAN_Desc_Flush( AL4SAN_desc_t     *desc,
                          AL4SAN_sequence_t *sequence )
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_desc_flush( desc, sequence );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_desc_flush( desc, sequence );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_desc_flush( desc, sequence );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_desc_flush( desc, sequence );
#endif

    return AL4SAN_SUCCESS;
}

/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 */
void
AL4SAN_Data_Flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_data_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_data_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_data_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_data_flush(sequence, A, Am, An);
#endif

    //AL4SAN_Runtime_data_flush(sequence, A, Am, An);
}

/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 */
void
AL4SAN_Matrix_Flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am, int An )
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
     AL4SAN_Quark_matrix_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_matrix_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_matrix_flush(sequence, A, Am, An);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_matrix_flush(sequence, A, Am, An);
#endif

    //AL4SAN_Runtime_matrix_flush(sequence, A, Am, An);
}

/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the vector
 *
 */
void
AL4SAN_Vector_Flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A, int Am)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
         AL4SAN_Quark_vector_flush(sequence, A, Am);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_vector_flush(sequence, A, Am);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_vector_flush(sequence, A, Am);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_vector_flush(sequence, A, Am);
#endif
}

/**
 * @brief Flush a single piece of data.
 *
 * This function marks a piece of data as unused in the future to:
 *   - cleanup the data from the distributed cache if present.
 *   - mark for eviction from the GPU if space is needed
 *   - move the data back to the main memory
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 *
 */
void
AL4SAN_Scaler_Flush( const AL4SAN_sequence_t *sequence,
                    const AL4SAN_desc_t *A)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
         AL4SAN_Quark_scaler_flush(sequence, A);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_scaler_flush(sequence, A);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_scaler_flush(sequence, A);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_scaler_flush(sequence, A);
#endif

}

/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Data_Migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
        AL4SAN_Quark_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif

}

/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Matrix_Migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int An, int new_rank )
{
        AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
        AL4SAN_Quark_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_data_migrate( sequence, A,  Am,  An,  new_rank );
#endif
}

/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the vector
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the vector
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Vector_Migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank )
{
     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
        AL4SAN_Quark_vector_migrate( sequence, A,  Am,  new_rank );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_vector_migrate( sequence, A,  Am, new_rank );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_vector_migrate( sequence, A,  Am,  new_rank );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_vector_migrate( sequence, A,  Am,  new_rank );
#endif

    //AL4SAN_Runtime_vector_migrate( sequence, A,  Am, new_rank );
}


/**
 * @brief Migrate a single piece of data.
 *
 * This function migrate a piece of data from its original rank to the new_rank
 * and changes its ownership.
 *
 * @param[in] sequence
 *            The sequence in which the data is used.
 *
 * @param[in] A
 *            The descriptor to which the piece of data belongs.
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the scaler
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the scaler
 *
 * @param[in] new_rank
 *            The new_rank on which to migrate the data
 */
void
AL4SAN_Scaler_Migrate( const AL4SAN_sequence_t *sequence,
                      const AL4SAN_desc_t *A, int Am, int new_rank )
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
        AL4SAN_Quark_scaler_migrate( sequence, A, new_rank );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_scaler_migrate( sequence, A, new_rank );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_scaler_migrate( sequence, A, new_rank );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_scaler_migrate( sequence, A,  new_rank );
#endif
//    AL4SAN_Runtime_scaler_migrate( sequence, A, new_rank );
}

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void *AL4SAN_Data_getaddr( const AL4SAN_desc_t *desc,  int m, int n)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Data_getaddr", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
      return AL4SAN_Quark_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
      return AL4SAN_Starpu_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    return AL4SAN_Openmp_data_getaddr( desc, m, n);
#endif
    //*ptr = AL4SAN_Runtime_data_getaddr( desc, m, n);
}


/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void *AL4SAN_Matrix_getaddr( const AL4SAN_desc_t *desc,  int m, int n)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Matrix_getaddr", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
      return AL4SAN_Quark_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
      return AL4SAN_Starpu_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    return AL4SAN_Openmp_matrix_getaddr( desc, m, n);
#endif

    //*ptr = AL4SAN_Runtime_data_getaddr( desc, m, n);
}

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void *AL4SAN_Vector_getaddr( const AL4SAN_desc_t *desc,  int m)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Vector_getaddr", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
      return AL4SAN_Quark_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
      return AL4SAN_Starpu_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    return AL4SAN_Openmp_vector_getaddr( desc, m);
#endif
}

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void *AL4SAN_Scaler_getaddr( const AL4SAN_desc_t *desc)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Data_getaddr", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
      return AL4SAN_Quark_scaler_getaddr( desc);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
      return AL4SAN_Starpu_scaler_getaddr( desc);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         return AL4SAN_Parsec_scaler_getaddr( desc);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    return AL4SAN_Openmp_scaler_getaddr( desc);
#endif
}
/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in out] ptr
 *        Pointer to retrieved data
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void AL4SAN_Data_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m, int n)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    *ptr = AL4SAN_Quark_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         *ptr = AL4SAN_Starpu_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         *ptr = AL4SAN_Parsec_data_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    *ptr = AL4SAN_Openmp_data_getaddr( desc, m, n);
#endif
}

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in out] ptr
 *        Pointer to retrieved data
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the matrix
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the matrix
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void AL4SAN_Matrix_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m, int n)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    *ptr = AL4SAN_Quark_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         *ptr = AL4SAN_Starpu_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         *ptr = AL4SAN_Parsec_matrix_getaddr( desc, m, n);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    *ptr = AL4SAN_Openmp_matrix_getaddr( desc, m, n);
#endif
}
/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in out] ptr
 *        Pointer to retrieved data
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the vector
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the vector
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void AL4SAN_Vector_Getaddr( const AL4SAN_desc_t *desc, void **ptr, int m)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    *ptr = AL4SAN_Quark_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         *ptr = AL4SAN_Starpu_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         *ptr = AL4SAN_Parsec_vector_getaddr( desc, m);
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    *ptr = AL4SAN_Openmp_vector_getaddr( desc, m);
#endif
}

/**
 * @brief Get the pointer to the data or the runtime handler associated to the
 * piece of data (m, n) in desc.
 *
 *
 * @param[in] A
 *            The descriptor to which belongs the piece of data.
 *
 * @param[in out] ptr
 *        Pointer to retrieved data
 *
 * @param[in] Am
 *            The row coordinate of the piece of data in the vector
 *
 * @param[in] An
 *            The column coordinate of the piece of data in the vector
 *
 *
 * @retval The runtime handler address of the piece of data.
 */

void AL4SAN_Scaler_Getaddr( const AL4SAN_desc_t *desc, void **ptr)
{
    AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }  
#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    *ptr = AL4SAN_Quark_scaler_getaddr( desc );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         *ptr = AL4SAN_Starpu_scaler_getaddr( desc );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         *ptr = AL4SAN_Parsec_scaler_getaddr( desc );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    *ptr = AL4SAN_Openmp_scaler_getaddr( desc );
#endif
}


/**
 *****************************************************************************
 *
 * @ingroup Descriptor
 *
 *  AL4SAN_user_tag_size - Set the sizes for the MPI tags
 *  Default value: tag_width=31, tag_sep=24, meaning that the MPI tag is stored
 *  in 31 bits, with 24 bits for the tile tag and 7 for the descriptor.  This
 *  function must be called before any descriptor creation.
 *
 ******************************************************************************
 *
 * @param[in] user_tag_width
 *          The new value for tag_width.
 *
 * @param[in] user_tag_sep
 *          The new value for tag_sep.
 *
 */
void AL4SAN_User_Tag_Size(int user_tag_width, int user_tag_sep) {
     AL4SAN_context_t *al4san;

    al4san = al4san_context_self();
    if (al4san == NULL) {
        al4san_error("AL4SAN_Desc_Destroy", "AL4SAN not initialized");
        //return AL4SAN_ERR_NOT_INITIALIZED;
    }

#ifdef AL4SAN_SCHED_QUARK
    if(al4san->scheduler==0)
    AL4SAN_Quark_comm_set_tag_sizes( user_tag_width, user_tag_sep );
#endif
#ifdef AL4SAN_SCHED_STARPU
    if(al4san->scheduler==1)
         AL4SAN_Starpu_comm_set_tag_sizes( user_tag_width, user_tag_sep );
#endif
#ifdef AL4SAN_SCHED_PARSEC
    if(al4san->scheduler==2)
         AL4SAN_Parsec_comm_set_tag_sizes( user_tag_width, user_tag_sep );
#endif
#ifdef AL4SAN_SCHED_OPENMP 
    if(al4san->scheduler==3)
    AL4SAN_Openmp_comm_set_tag_sizes( user_tag_width, user_tag_sep );
#endif
    return;
}

