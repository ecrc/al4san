###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
#
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

# Generate the altanal headers for all possible precisions
# ------------------------------------------------------

configure_file("altanal/altanal_config.h.in"
               "altanal/altanal_config.h"
               @ONLY)

# Define the list of headers
# --------------------------
set(ALTANAL_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/altanal/altanal_config.h
  ${CMAKE_CURRENT_BINARY_DIR}/altanal/altanal_mangling.h
  altanal/altanal_constants.h
  altanal/altanal_kernels.h
  altanal/altanal_runtime.h
  altanal/altanal_simulate.h
  altanal/altanal_struct.h
  altanal/altanal_types.h
  )

# Force generation of headers
# ---------------------------
add_custom_target(altanal_include
  ALL SOURCES altanal.h ${ALTANAL_HDRS} )
set(ALTANAL_SOURCES_TARGETS "${ALTANAL_SOURCES_TARGETS};altanal_include" CACHE INTERNAL "List of targets of sources")

# Installation
# ------------
install( FILES altanal.h
         DESTINATION include )

install( FILES ${ALTANAL_HDRS}
         DESTINATION include/altanal )

###
### END CMakeLists.txt
###
