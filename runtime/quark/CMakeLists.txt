###
#
# @file CMakeLists.txt
#
# @copyright 2009-2015 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). 
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
# version 1.1.0
# author Cedric Castagnede
# author Emmanuel Agullo
# author Mathieu Faverge
# author Florent Pruvost
# date 2012-07-13
#
###
###
#
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
# @version 1.1.0
#  @author Rabab Alomairy
#  @date 2019-02-06
#
###
cmake_minimum_required(VERSION 2.8)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/include )

# # Generate headers for all possible precisions
# # --------------------------------------------

# Define the list of headers
# --------------------------
set(AL4SAN_RUNTIME_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/al4san_quark.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/quark_task.h
  )


# Force generation of headers
# ---------------------------
add_custom_target(
  runtime_quark_include
  ALL SOURCES ${AL4SAN_RUNTIME_HDRS})

# Installation
# ------------
install(
  FILES ${AL4SAN_RUNTIME_HDRS}
  DESTINATION include/runtime/quark/include)

# Generate the Al4san common for all possible precisions
# ---------------------------------------------------------
set(AL4SAN_RUNTIME_COMMON_GENERATED "")

set(AL4SAN_RUNTIME_COMMON
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_async.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_context.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_control.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_options.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_task.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/quark_descriptor.c
  ${AL4SAN_RUNTIME_COMMON_GENERATED}
  )

set(AL4SAN_RUNTIME_SRCS
  ${AL4SAN_RUNTIME_COMMON}
)
# Force generation of sources
# ---------------------------
add_custom_target(quark_sources ALL SOURCES ${AL4SAN_RUNTIME_SRCS})
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};runtime_quark_include;quark_sources" CACHE INTERNAL "List of targets of sources")

# Add library
# -----------
add_library(al4san_quark ${AL4SAN_RUNTIME_SRCS})
set_property(TARGET al4san_quark PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET al4san_quark PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")


target_link_libraries(al4san_quark ${QUARK_LIBRARIES_DEP})

add_dependencies(al4san_quark
  al4san_include
  control_include
  runtime_quark_include
  quark_sources
)

# installation
# ------------
install(TARGETS al4san_quark
  DESTINATION lib)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_RUNTIME_SRCS} ${AL4SAN_RUNTIME_HDRS} PARENT_SCOPE )

###
### END CMakeLists.txt
###
