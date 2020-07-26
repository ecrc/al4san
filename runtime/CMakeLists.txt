###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
#                      All rights reserved.
###
#
#  project CHAMELEON
#  CHAMELEON is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
# version 1.0.0
# author Cedric Castagnede
# author Emmanuel Agullo
# author Mathieu Faverge
# date 2012-07-13
#
###
###
#
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
# @version 1.0.0
#  @author Rabab Alomairy
#  @date 2018-10-18
#
###

# Check for the subdirectories
# ----------------------------
if( AL4SAN_SCHED_QUARK )
    add_subdirectory(quark)
endif()
if( AL4SAN_SCHED_PARSEC )
    add_subdirectory(parsec)
endif()
if( AL4SAN_SCHED_STARPU )
    add_subdirectory(starpu)
endif()
if( AL4SAN_SCHED_OPENMP)
    add_subdirectory(openmp)
endif()

# Define the list of headers
# --------------------------
set(AL4SAN_RUNTIME_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san_runtime.h
  )


# Force generation of headers
# ---------------------------
add_custom_target(
  runtime_include
  ALL SOURCES ${AL4SAN_RUNTIME_HDRS})

# Installation
# ------------
install(
  FILES ${AL4SAN_RUNTIME_HDRS}
  DESTINATION include/runtime )

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} PARENT_SCOPE )

###
### END CMakeLists.txt
###
