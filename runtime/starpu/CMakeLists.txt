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
# @version 1.0.1
#  @author Rabab Alomairy
#  @date 2019-02-06
#
###
cmake_minimum_required(VERSION 2.8)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/include )

configure_file("include/al4san_starpu.h.in"
               "include/al4san_starpu.h"
               @ONLY)

# Generate headers for all possible precisions


# Define the list of headers
# --------------------------
set(AL4SAN_RUNTIME_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/include/al4san_starpu.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/starpu_codelet.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/starpu_workspace.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/starpu_task.h
  )

# Force generation of headers
# ---------------------------
add_custom_target(
  runtime_starpu_include
  ALL SOURCES ${AL4SAN_RUNTIME_HDRS})

# Installation
# ------------
install(
  FILES ${AL4SAN_RUNTIME_HDRS}
  DESTINATION include/runtime/starpu/include )

# Generate the Al4san common for all possible precisions
# ---------------------------------------------------------
set(AL4SAN_RUNTIME_COMMON_GENERATED "")

set(AL4SAN_RUNTIME_COMMON
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_async.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_context.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_control.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_options.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_workspace.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_descriptor.c
  ${CMAKE_CURRENT_SOURCE_DIR}/control/starpu_task.c
  ${AL4SAN_RUNTIME_COMMON_GENERATED}
  )

set(flags_to_add "")
foreach(_prec ${AL4SAN_PRECISION})
  set(flags_to_add "${flags_to_add} -DPRECISION_${_prec}")
endforeach()
set_source_files_properties(control/runtime_profiling.c PROPERTIES COMPILE_FLAGS "${flags_to_add}")


set(AL4SAN_RUNTIME_SRCS
  ${AL4SAN_RUNTIME_COMMON}
  )

# Force generation of sources
# ---------------------------
add_custom_target(starpu_sources ALL SOURCES ${AL4SAN_RUNTIME_SRCS})
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};runtime_starpu_include;starpu_sources" CACHE INTERNAL "List of targets of sources")

# Add library
# -----------
add_library(al4san_starpu ${AL4SAN_RUNTIME_SRCS})
set_property(TARGET al4san_starpu PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET al4san_starpu PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

target_link_libraries(al4san_starpu
  ${STARPU_LIBRARIES_DEP})

add_dependencies(al4san_starpu
  al4san_include
  control_include
  runtime_starpu_include
  starpu_sources
  )


# installation
# ------------
install(TARGETS al4san_starpu
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_RUNTIME_SRCS} ${AL4SAN_RUNTIME_HDRS} PARENT_SCOPE )

###
### END CMakeLists.txt
###
