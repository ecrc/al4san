/**
 *
 * @file altanal_types.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon basic datatypes header
 *
 * @version 1.0.0
 * @author Cedric Augonnet
 * @author Mathieu Faverge
 * @author Cedric Castagnede
 * @date 2011-06-01
 *
 */
#ifndef _ALTANAL_TYPES_H_
#define _ALTANAL_TYPES_H_

#include "altanal/altanal_config.h"

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
 *  ALTANAL types
 */
typedef int  ALTANAL_enum;
typedef int  ALTANAL_bool;
typedef long ALTANAL_index;
typedef long ALTANAL_size;


/**
 * ALTANAL Complex numbers
 */
#define ALTANAL_HAS_COMPLEX_H 1

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
    #undef  ALTANAL_COMPLEX_CPP
    #define ALTANAL_COMPLEX_CPP
# else
    #error "Supported compilers on WIN32 are MSVC and Intel Compiler."
# endif /* __INTEL_COMPILER */

# define isnan _isnan
# define isinf !_finite
#endif /* _WIN32 */

/* Sun doesn't ship the complex.h header. Sun Studio doesn't have it and older GCC compilers don't have it either. */
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC) || defined(sun) || defined(__sun)
#undef ALTANAL_HAS_COMPLEX_H
#endif /* __SUNPRO_C */

#ifndef __cplusplus
    #undef ALTANAL_COMPLEX_CPP
#endif

#if defined(ALTANAL_COMPLEX_CPP)
    #ifndef LAPACK_COMPLEX_CPP
    # define LAPACK_COMPLEX_CPP
    # warning "ALTANAL_COMPLEX_CPP was defined, but not LAPACK_COMPLEX_CPP. Maybe you want to set both."
    #endif
    #include <complex> // needed for std::complex declaration
    #define ALTANAL_Complex32_t std::complex<float>
    #define ALTANAL_Complex64_t std::complex<double>
#else /* ALTANAL_COMPLEX_CPP */
      /* not using cplusplus complex type: */

    #if defined(__STDC_NO_COMPLEX__)
    # error "Compiler support for complex number is required."
    #endif

    #define ALTANAL_Complex32_t float  _Complex
    #define ALTANAL_Complex64_t double _Complex

    #if ALTANAL_HAS_COMPLEX_H
    # include <complex.h>
    #endif
#endif /* ALTANAL_COMPLEX_CPP */

/**
 *  ALTANAL Deprecated attribute
 */
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define ALTANAL_DEPRECATED  __attribute__((__deprecated__))
#else
#define ALTANAL_DEPRECATED
#endif /* __GNUC__ */

BEGIN_C_DECLS

/**
 *  Global utilities
 */
static inline int altanal_max( int a, int b ) {
    if ( a > b ) return a; else return b;
}

static inline int altanal_min( int a, int b ) {
    if ( a < b ) return a; else return b;
}

END_C_DECLS

#endif /* __ALTANAL_H__ */
