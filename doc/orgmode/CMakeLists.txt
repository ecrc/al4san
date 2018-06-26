###
#
# @file CMakeLists.txt
#
# @copyright (c) 2017 Inria. All rights reserved.
#
###
#
#  @project ALTANAL
#  ALTANAL is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver. 
#
# @version 1.0.0
#  @author Florent Pruvost
#  @date 2017-08-25
#
###

cmake_minimum_required(VERSION 2.8)

# Create file version.org
# -----------------------
configure_file("version.org.in"
               "version.org"
               @ONLY)
configure_file("users_guide.org.in"
               "users_guide.org"
               @ONLY)

set(FIGURES 
    tile_lu.pdf
    tile_lu.jpg
    tile_layout.pdf
    tile_layout.jpg
    trace_qr.pdf
    trace_qr.jpg
    potri_async.png
    altanal_header.png
    )
set(FIGURES_HTML 
    tile_lu.jpg
    tile_layout.jpg
    trace_qr.jpg
    potri_async.png
    altanal_header.png
    )

foreach(_fig ${FIGURES})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/chapters/${_fig}
                   ${CMAKE_CURRENT_BINARY_DIR}/${_fig}
                   COPYONLY)
endforeach()

# Looking for emacs
# -----------------
FIND_PROGRAM(EMACS_COMPILER emacs)
FIND_PROGRAM(TEX_COMPILER tex)

if(EMACS_COMPILER)
    # Add target
    # ----------
    add_custom_command(OUTPUT  users_guide.html
                       COMMAND ${EMACS_COMPILER}
                       ARGS    ${CMAKE_CURRENT_BINARY_DIR}/users_guide.org 
                               --batch
                               -f
                               org-html-export-to-html
                               --kill
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/users_guide.org
                               ${CMAKE_CURRENT_BINARY_DIR}/version.org
                      )
    add_custom_command(OUTPUT  users_guide.pdf
                       COMMAND ${EMACS_COMPILER}
                       ARGS    ${CMAKE_CURRENT_BINARY_DIR}/users_guide.org 
                               --batch
                               -f
                               org-latex-export-to-pdf
                               --kill
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/users_guide.org
                               ${CMAKE_CURRENT_BINARY_DIR}/version.org
                      )
    add_custom_target(doc-html-users_guide ALL DEPENDS users_guide.html)
    add_custom_target(doc-pdf-users_guide ALL DEPENDS users_guide.pdf)

    # Installation
    # ------------
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/users_guide.html
            DESTINATION share/altanal/html)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/users_guide.pdf
            DESTINATION share/altanal/pdf)
    foreach(_fig ${FIGURES_HTML})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_fig}
                DESTINATION share/altanal/html)
    endforeach()

else(EMACS_COMPILER)
    message(STATUS "Looking for emacs - not found")
endif(EMACS_COMPILER)

###
### END CMakeLists.txt
###
