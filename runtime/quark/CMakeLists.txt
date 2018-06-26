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
#  @project CHAMELEON
#  CHAMELEON is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
# @version 1.0.0
#  @author Cedric Castagnede
#  @author Emmanuel Agullo
#  @author Mathieu Faverge
#  @author Florent Pruvost
#  @date 2012-07-13
#
###
###
#
#  @project ALTANAL
#  ALTANAL is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
# @version 0.1.0
#  @author Rabab Alomairy
#  @date 2018-05-19
#
###
cmake_minimum_required(VERSION 2.8)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/include )

# # Generate headers for all possible precisions
# # --------------------------------------------

# Define the list of headers
# --------------------------
set(ALTANAL_Runtime_HDRS
  include/altanal_runtime.h
  include/runtime_task.h
  )


# Force generation of headers
# ---------------------------
add_custom_target(
  runtime_quark_include
  ALL SOURCES ${ALTANAL_Runtime_HDRS})

# Installation
# ------------
install(
  FILES ${ALTANAL_Runtime_HDRS}
  DESTINATION include/runtime )

# Generate the Altanal common for all possible precisions
# ---------------------------------------------------------
set(ALTANAL_Runtime_COMMON_GENERATED "")
set(ZSRC
  control/runtime_zprofiling.c
  control/runtime_zlocality.c
  )

precisions_rules_py(
  ALTANAL_Runtime_COMMON_GENERATED "${ZSRC}"
  PRECISIONS "${ALTANAL_PRECISION}"
  TARGETDIR "control")

set(ALTANAL_Runtime_COMMON
  control/runtime_async.c
  control/runtime_context.c
  control/runtime_control.c
  control/runtime_options.c
  control/runtime_profiling.c
  control/runtime_task.c
  ${ALTANAL_Runtime_COMMON_GENERATED}
  )

set(ALTANAL_Runtime_SRCS
  ${ALTANAL_Runtime_COMMON}
)
# Force generation of sources
# ---------------------------
add_custom_target(quark_sources ALL SOURCES ${ALTANAL_Runtime_SRCS})
set(ALTANAL_SOURCES_TARGETS "${ALTANAL_SOURCES_TARGETS};runtime_quark_include;quark_sources" CACHE INTERNAL "List of targets of sources")

# Add library
# -----------
add_library(altanal_quark ${ALTANAL_Runtime_SRCS})
set_property(TARGET altanal_quark PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET altanal_quark PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")


target_link_libraries(altanal_quark ${QUARK_LIBRARIES_DEP})

add_dependencies(altanal_quark
  altanal_include
  control_include
  runtime_quark_include
  quark_sources
)
#if (NOT ALTANAL_SIMULATION)
#    add_dependencies(altanal_quark coreblas_include)
#endif()

# installation
# ------------
install(TARGETS altanal_quark
  DESTINATION lib)

###
### END CMakeLists.txt
###
