#ifndef _AL4SAN_RUNTIME_HEADER_
#define _AL4SAN_RUNTIME_HEADER_

BEGIN_C_DECLS


#if defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#include "starpu/include/al4san_starpu.h"
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#include "starpu/include/al4san_starpu.h"
#include "parsec/include/al4san_parsec.h"
#include "quark/include/al4san_quark.h"
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#include "starpu/include/al4san_starpu.h"
#include "quark/include/al4san_quark.h"
#endif

#if !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#include "parsec/include/al4san_parsec.h"
#endif

#if !defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && !defined(AL4SAN_SCHED_OPENMP)
#include "quark/include/al4san_quark.h"
#endif

#if !defined(AL4SAN_SCHED_STARPU) && !defined(AL4SAN_SCHED_QUARK) && !defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#include "openmp/include/al4san_openmp.h"
#endif

#if defined(AL4SAN_SCHED_STARPU) && defined(AL4SAN_SCHED_QUARK) && defined(AL4SAN_SCHED_PARSEC) && defined(AL4SAN_SCHED_OPENMP)
#include "starpu/include/al4san_starpu.h"
#include "parsec/include/al4san_parsec.h"
#include "quark/include/al4san_quark.h"
#include "openmp/include/al4san_openmp.h"
#endif

END_C_DECLS

#endif /*_AL4SAN_RUNTIME_HEADER_*/

