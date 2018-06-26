###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
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
#  @date 2012-07-13
#
###

cmake_minimum_required(VERSION 2.8)

# Looking for doxygen
# -------------------
find_package(Doxygen)

# Generate documentation
# ----------------------
if(DOXYGEN_EXECUTABLE)

    # Create file altanal.dox
    # ---------------------------
    if(ALTANAL_SCHED_STARPU)

        configure_file("conf-dev-starpu.dox.in"
                       "${CMAKE_CURRENT_BINARY_DIR}/altanal-dev.dox"
                       @ONLY)
#        configure_file("conf-usr-starpu.dox.in"
#                       "altanal-usr.dox"
#                       @ONLY)

    elseif(ALTANAL_SCHED_QUARK)

        configure_file("conf-dev-quark.dox.in"
                       "${CMAKE_CURRENT_BINARY_DIR}/altanal-dev.dox"
                        @ONLY)
#        configure_file("conf-usr-quark.dox.in"
#                       "altanal-usr.dox"
#                       @ONLY)

    endif()

    # Message during configure
    #-------------------------
    message(STATUS "Looking for doxygen - found")

    # User's documentation
    # --------------------
    add_custom_command(OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/out-dev
                       COMMAND ${DOXYGEN_EXECUTABLE}
                       ARGS    ${CMAKE_CURRENT_BINARY_DIR}/altanal-dev.dox
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/altanal-dev.dox
                      )
#    add_custom_command(OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/out-usr
#                       COMMAND ${DOXYGEN_EXECUTABLE}
#                       ARGS    ${CMAKE_CURRENT_BINARY_DIR}/altanal-usr.dox
#                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/altanal-usr.dox
#                      )
    add_custom_target(doxygen-out ALL
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/out-dev
                              #${CMAKE_CURRENT_BINARY_DIR}/out-usr
                              altanal_all_sources
                              )

    # Installation for html version
    # -----------------------------
    install(DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/out-dev/html
            DESTINATION share/altanal/doxygen/dev)
#    install(DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/out-usr/html
#            DESTINATION share/altanal/doxygen/usr)

else()
    message(STATUS "Looking for doxygen - not found")

endif()

###
### END CMakeLists.txt
###
