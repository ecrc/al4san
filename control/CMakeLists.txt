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


# Generate the al4san headers for all possible precisions
# ------------------------------------------------------

# Define the list of headers
# --------------------------
set(AL4SAN_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_async.h
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_auxiliary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_f77.h
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_descriptor.h
    )

set(HDR_INSTALL ${AL4SAN_HDRS})

# Force generation of headers
# ---------------------------
add_custom_target(control_include ALL SOURCES ${AL4SAN_HDRS})
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};control_include" CACHE INTERNAL "List of targets of sources")

# installation
# ------------
 install(FILES ${HDR_INSTALL}
         DESTINATION include/control)

#compute cmake
# Define the list of sources
# --------------------------
set(AL4SAN_CONTROL
    ${CMAKE_CURRENT_SOURCE_DIR}/async.c
    ${CMAKE_CURRENT_SOURCE_DIR}/auxiliary.c
    ${CMAKE_CURRENT_SOURCE_DIR}/context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/al4san_f77.c
    ${CMAKE_CURRENT_SOURCE_DIR}/task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/options.c
   )

set(flags_to_add "")
foreach(_prec ${AL4SAN_PRECISION})
    set(flags_to_add "${flags_to_add} -DPRECISION_${_prec}")
endforeach()
#set_source_files_properties(../control/tile.c PROPERTIES COMPILE_FLAGS "${flags_to_add}")


set(AL4SAN_SRCS
    ${AL4SAN_CONTROL}
   )

# Generate the al4san fortran sources for all possible precisions
# --------------------------------------------------------------
if(HAVE_ISO_C_BINDING)
#    set(AL4SAN_SRCS_F_GENERATED "")
#    set(ZSRCF
#        al4san_zcf90.F90
#        al4san_zf90.F90
#        al4san_zf90_wrappers.F90
#       )
#    precisions_rules_py(AL4SAN_SRCS_F_GENERATED "${ZSRCF}"
#                        PRECISIONS "${AL4SAN_PRECISION}"
#                        TARGETDIR "control" )
#
    set(AL4SAN_SRCSF
        ${CMAKE_CURRENT_SOURCE_DIR}/al4san_f90.f90
#        ${AL4SAN_SRCS_F_GENERATED}
       )
endif(HAVE_ISO_C_BINDING)

# Force generation of sources
# ---------------------------
add_custom_target(al4san_sources ALL SOURCES "${AL4SAN_SRCS};${AL4SAN_SRCSF}")
set(AL4SAN_SOURCES_TARGETS "${AL4SAN_SOURCES_TARGETS};al4san_sources" CACHE INTERNAL "List of targets of sources")

# Compile step
# ------------
add_library(al4san ${AL4SAN_SRCS} ${AL4SAN_SRCSF})
if(AL4SAN_SCHED_STARPU)
  target_link_libraries(al4san al4san_starpu)
elseif(AL4SAN_SCHED_PARSEC)
  target_link_libraries(al4san al4san_parsec)
elseif(AL4SAN_SCHED_QUARK)
  target_link_libraries(al4san al4san_quark)
elseif(AL4SAN_SCHED_OPENMP)
  target_link_libraries(al4san al4san_openmp)
endif()


add_dependencies(al4san
  al4san_include
  control_include
  al4san_sources
)

set_property(TARGET al4san PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET al4san PROPERTY Fortran_MODULE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set_property(TARGET al4san PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# installation
# ------------
install(TARGETS al4san
        DESTINATION lib)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_HDRS} ${AL4SAN_SRCS} ${AL4SAN_SRCSF} PARENT_SCOPE )

###
### END CMakeLists.txt
###
