###
#
# @copyright (c) 2009-2014 The University of Tennessee and The University
#                          of Tennessee Research Foundation.
#                          All rights reserved.
# @copyright (c) 2012-2016 Inria. All rights reserved.
# @copyright (c) 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria, Univ. Bordeaux. All rights reserved.
# @copyright 2018 King Abdullah University of Science and Technology (KAUST).
#                     All rights reserved.
#
###
#
#  @file CMakeLists.txt
#
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
#  version 1.1.0
#  author Cedric Castagnede
#  author Emmanuel Agullo
#  author Mathieu Faverge
#  author Florent Pruvost
#  date 13-07-2012
#
###

cmake_minimum_required(VERSION 2.8)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/include )


# Define the list of headers
# --------------------------
set(AL4SAN_RUNTIME_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/al4san_openmp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/openmp_task.h
    )


# Force generation of headers
# ---------------------------
add_custom_target(runtime_openmp_include ALL SOURCES ${AL4SAN_RUNTIME_HDRS})

# installation
# ------------
install(FILES ${AL4SAN_RUNTIME_HDRS}
        DESTINATION include/runtime/openmp/include)


set(AL4SAN_RUNTIME_COMMON
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_async.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_options.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_workspace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control/openmp_descriptor.c
    ${AL4SAN_runtime_COMMON_GENERATED}
    )

# Generate the AL4san sources for all possible precisions
# ------------------------------------------------------

set(AL4SAN_RUNTIME_SRCS
  ${AL4SAN_RUNTIME_COMMON}
#  ${AL4SAN_runtime_SRCS_GENERATED}
  )

# Force generation of sources
# ---------------------------
add_custom_target(openmp_sources ALL SOURCES ${AL4SAN_RUNTIME_SRCS})
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};runtime_openmp_include;openmp_sources" CACHE INTERNAL "List of targets of sources")

# Add library
# -----------
add_library(al4san_openmp ${AL4SAN_RUNTIME_SRCS})
set_property(TARGET al4san_openmp PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET al4san_openmp PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")


add_dependencies(al4san_openmp
  al4san_include
  control_include
  runtime_openmp_include
  openmp_sources
)

# installation
# ------------
install(TARGETS al4san_openmp
        DESTINATION lib)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_RUNTIME_SRCS} ${AL4SAN_RUNTIME_HDRS} PARENT_SCOPE )

###
### END CMakeLists.txt
###
