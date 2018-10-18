/**
 *
 * @file types.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 * @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 *                     All rights reserved.
 *
 ***
 *
 *
 * version 1.0.0
 * author Cedric Augonnet
 * author Mathieu Faverge
 * author Cedric Castagnede
 * date 2011-06-01
 *
 */

/**
   *
   * @brief AL4SAN basic datatypes header
   *
   *  AL4SAN is a software package provided by King Abdullah University of Science and Technology (KAUST)
   *
   * @version 1.0.0
   * @author Rabab Alomairy
   * @date 2018-10-18
   *
  **/
#ifndef _AL4SAN_TYPES_H_
#define _AL4SAN_TYPES_H_

#include "al4san/config.h"

/**
 *  System requirements
 */
#include <stddef.h>
#if defined( _WIN32 )
  /* This must be included before INPUT is defined below, otherwise we
     have a name clash/problem  */
  #include <windows.h>
  #include <limits.h>
#else /* _WIN32 */
  #include <inttypes.h>
#endif /* _WIN32 */

#include <stdarg.h>

/**
 *  AL4SAN types
 */
typedef int  AL4SAN_enum;
typedef int  AL4SAN_bool;
typedef long AL4SAN_index;
typedef long AL4SAN_size;


/**
 * AL4SAN Complex numbers
 */
#define AL4SAN_HAS_COMPLEX_H 1

#if defined(_WIN32)
# include <float.h>
# if defined(__INTEL_COMPILER)
    /* Fix name conflict within the cabs prototype (_Complex) that    */
    /* conflicts with a C99 keyword.                                  */
    #define _Complex __ConflictingComplex
    #include <math.h>
    #undef _Complex
    #undef complex
# elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    #undef  AL4SAN_COMPLEX_CPP
    #define AL4SAN_COMPLEX_CPP
# else
    #error "Supported compilers on WIN32 are MSVC and Intel Compiler."
# endif /* __INTEL_COMPILER */

# define isnan _isnan
# define isinf !_finite
#endif /* _WIN32 */

/* Sun doesn't ship the complex.h header. Sun Studio doesn't have it and older GCC compilers don't have it either. */
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC) || defined(sun) || defined(__sun)
#undef AL4SAN_HAS_COMPLEX_H
#endif /* __SUNPRO_C */

#ifndef __cplusplus
    #undef AL4SAN_COMPLEX_CPP
#endif

#if defined(AL4SAN_COMPLEX_CPP)
    #ifndef LAPACK_COMPLEX_CPP
    # define LAPACK_COMPLEX_CPP
    # warning "AL4SAN_COMPLEX_CPP was defined, but not LAPACK_COMPLEX_CPP. Maybe you want to set both."
    #endif
    #include <complex> // needed for std::complex declaration
    #define AL4SAN_Complex32_t std::complex<float>
    #define AL4SAN_Complex64_t std::complex<double>
#else /* AL4SAN_COMPLEX_CPP */
      /* not using cplusplus complex type: */

    #if defined(__STDC_NO_COMPLEX__)
    # error "Compiler support for complex number is required."
    #endif

    #define AL4SAN_Complex32_t float  _Complex
    #define AL4SAN_Complex64_t double _Complex

    #if AL4SAN_HAS_COMPLEX_H
    # include <complex.h>
    #endif
#endif /* AL4SAN_COMPLEX_CPP */

/**
 *  AL4SAN Deprecated attribute
 */
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define AL4SAN_DEPRECATED  __attribute__((__deprecated__))
#else
#define AL4SAN_DEPRECATED
#endif /* __GNUC__ */

BEGIN_C_DECLS

/**
 *  Global utilities
 */
static inline int al4san_max( int a, int b ) {
    if ( a > b ) return a; else return b;
}

static inline int al4san_min( int a, int b ) {
    if ( a < b ) return a; else return b;
}

END_C_DECLS

#endif /* __AL4SAN_H__ */
