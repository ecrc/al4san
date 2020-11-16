###
#
# @file CMakeLists.txt
#
# @copyright 2009-2015 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
#                      All rights reserved.
###
#
#  CHAMELEON is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
#  author Cedric Castagnede
#  author Emmanuel Agullo
#  author Mathieu Faverge
#  author Florent Pruvost
#  date 2012-07-13
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

# Generate headers for all possible precisions
# --------------------------------------------

# Define the list of headers
# --------------------------
set(AL4SAN_RUNTIME_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/al4san_parsec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parsec_task.h
  )


# Force generation of headers
# ---------------------------
add_custom_target(
  runtime_parsec_include
  ALL SOURCES ${AL4SAN_RUNTIME_HDRS})

# Installation
# ------------
install(
  FILES ${AL4SAN_RUNTIME_HDRS}
  DESTINATION include/runtime/parsec/include)

# Generate the Al4san common for all possible precisions
# ---------------------------------------------------------
set(AL4SAN_RUNTIME_COMMON_GENERATED "")

set(AL4SAN_RUNTIME_COMMON
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_async.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_context.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_control.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_descriptor.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_options.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/parsec_task.c
  ${AL4SAN_RUNTIME_COMMON_GENERATED}
  )


set(AL4SAN_RUNTIME_SRCS
  ${AL4SAN_RUNTIME_COMMON}
  )

# Force generation of sources
# ---------------------------
add_custom_target(parsec_sources ALL SOURCES ${AL4SAN_RUNTIME_SRCS})
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};runtime_parsec_include;parsec_sources" CACHE INTERNAL "List of targets of sources")

# Add library
# -----------
add_library(al4san_parsec ${AL4SAN_RUNTIME_SRCS})
set_property(TARGET al4san_parsec PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET al4san_parsec PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

target_link_libraries(al4san_parsec
  ${PARSEC_LIBRARIES_DEP})


add_dependencies(al4san_parsec
  al4san_include
  control_include
  runtime_parsec_include
  parsec_sources
)


# installation
# ------------
install(TARGETS al4san_parsec
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_RUNTIME_SRCS} ${AL4SAN_RUNTIME_HDRS} PARENT_SCOPE )

###
### END CMakeLists.txt
###
