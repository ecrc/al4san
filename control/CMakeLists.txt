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

# Define the list of headers
# --------------------------
set(ALTANAL_HDRS
    async.h
    auxiliary.h
    common.h
    context.h
    altanal_f77.h
#    altanalwinthread.h
    )

set(HDR_INSTALL ${ALTANAL_HDRS})

# Force generation of headers
# ---------------------------
add_custom_target(control_include ALL SOURCES ${ALTANAL_HDRS})
set(ALTANAL_SOURCES_TARGETS "${ALTANAL_SOURCES_TARGETS};control_include" CACHE INTERNAL "List of targets of sources")

# installation
# ------------
# install(FILES ${HDR_INSTALL}
#         DESTINATION include/altanal/control)

#compute cmake
# Define the list of sources
# --------------------------
set(ALTANAL_CONTROL
    async.c
    auxiliary.c
    context.c
    control.c
    altanal_f77.c
    task.c
#   altanalwinthread.c
   )

set(flags_to_add "")
foreach(_prec ${ALTANAL_PRECISION})
    set(flags_to_add "${flags_to_add} -DPRECISION_${_prec}")
endforeach()
#set_source_files_properties(../control/tile.c PROPERTIES COMPILE_FLAGS "${flags_to_add}")


set(ALTANAL_SRCS
    ${ALTANAL_CONTROL}
   )

# Generate the altanal fortran sources for all possible precisions
# --------------------------------------------------------------
if(HAVE_ISO_C_BINDING)
#    set(ALTANAL_SRCS_F_GENERATED "")
#    set(ZSRCF
#        altanal_zcf90.F90
#        altanal_zf90.F90
#        altanal_zf90_wrappers.F90
#       )
#    precisions_rules_py(ALTANAL_SRCS_F_GENERATED "${ZSRCF}"
#                        PRECISIONS "${ALTANAL_PRECISION}"
#                        TARGETDIR "control" )
#
    set(ALTANAL_SRCSF
        altanal_f90.f90
#        ${ALTANAL_SRCS_F_GENERATED}
       )
endif(HAVE_ISO_C_BINDING)

# Force generation of sources
# ---------------------------
add_custom_target(altanal_sources ALL SOURCES "${ALTANAL_SRCS};${ALTANAL_SRCSF}")
set(ALTANAL_SOURCES_TARGETS "${ALTANAL_SOURCES_TARGETS};altanal_sources" CACHE INTERNAL "List of targets of sources")

# Compile step
# ------------
add_library(altanal ${ALTANAL_SRCS} ${ALTANAL_SRCSF})
if(ALTANAL_SCHED_STARPU)
  target_link_libraries(altanal altanal_starpu)
elseif(ALTANAL_SCHED_PARSEC)
  target_link_libraries(altanal altanal_parsec)
elseif(ALTANAL_SCHED_QUARK)
  target_link_libraries(altanal altanal_quark)
endif()


add_dependencies(altanal
  altanal_include
  control_include
  altanal_sources
)

set_property(TARGET altanal PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET altanal PROPERTY Fortran_MODULE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set_property(TARGET altanal PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# installation
# ------------
install(TARGETS altanal
        DESTINATION lib)


###
### END CMakeLists.txt
###
