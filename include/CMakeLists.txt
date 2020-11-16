###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2018 King Abdullah University of Science and Technology (KAUST).
#                     All rights reserved.
#
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
#  @date 2018-10-18
#
###
# Generate the al4san headers for all possible precisions
# ------------------------------------------------------

configure_file("al4san/config.h.in"
               "al4san/config.h"
               @ONLY)

# Define the list of headers
# --------------------------
set(AL4SAN_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/al4san/config.h
  ${CMAKE_CURRENT_BINARY_DIR}/al4san/al4san_mangling.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/constants.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/runtime.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/struct.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/task.h
  ${CMAKE_CURRENT_SOURCE_DIR}/al4san/timer.h
  )

# Force generation of headers
# ---------------------------
add_custom_target(al4san_include
  ALL SOURCES al4san.h ${AL4SAN_HDRS} )
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};al4san_include" CACHE INTERNAL "List of targets of sources")

# Installation
# ------------
install( FILES al4san.h
         DESTINATION include )

install( FILES ${AL4SAN_HDRS}
         DESTINATION include/al4san )

# Put doxygen input to parent scope
set(INCLUDE_FILES ${CMAKE_SOURCE_DIR} ${AL4SAN_HDRS}
                  ${CMAKE_CURRENT_SOURCE_DIR}/al4san.h )
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${INCLUDE_FILES} PARENT_SCOPE)
###
### END CMakeLists.txt
###
