###
#
# @file PrintOpts.cmake
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
#
###
#
#  @project MORSE
#  MORSE is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver.
#
# @version 1.0.0
#  @author Florent Pruvost
#  @date 2014-11-10
#
###

set(dep_message "\nConfiguration of Al4san:\n"
        "       BUILDNAME ...........: ${BUILDNAME}\n"
        "       SITE ................: ${SITE}\n"
        "\n"
        "       Compiler: C .........: ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_ID})\n"
        "       Compiler: Fortran ...: ${CMAKE_Fortran_COMPILER} (${CMAKE_Fortran_COMPILER_ID})\n")
if(AL4SAN_USE_MPI)
  set(dep_message "${dep_message}"
  "       Compiler: MPI .......: ${MPI_C_COMPILER}\n"
  "       compiler flags ......: ${MPI_C_COMPILE_FLAGS}\n")
endif()
set(dep_message "${dep_message}"
"       Linker: .............: ${CMAKE_LINKER}\n"
"\n"
"       Build type ..........: ${CMAKE_BUILD_TYPE}\n"
"       Build shared ........: ${BUILD_SHARED_LIBS}\n"
"       CFlags ..............: ${CMAKE_C_FLAGS}\n"
"       LDFlags .............: ${CMAKE_C_LINK_FLAGS}\n"
"       EXE LDFlags .........: ${CMAKE_EXE_LINKER_FLAGS}\n"
"\n"
"       Implementation paradigm\n"
"       CUDA ................: ${AL4SAN_USE_CUDA}\n"
"       MPI .................: ${AL4SAN_USE_MPI}\n"
"\n"
"       Runtime specific\n"
"       PARSEC ..............: ${AL4SAN_SCHED_PARSEC}\n"
"       QUARK ...............: ${AL4SAN_SCHED_QUARK}\n"
"       STARPU ..............: ${AL4SAN_SCHED_STARPU}\n"
"\n"
"       Kernels specific\n"
"       BLAS ................: ${BLAS_VENDOR_FOUND}\n"
"       LAPACK...............: ${LAPACK_VENDOR_FOUND}\n"
"\n"
"       Trace ...............: ${AL4SAN_ENABLE_TRACING}\n"
"       Simulation mode .....: ${AL4SAN_SIMULATION}\n"
"\n"
"       Binaries to build\n"
"       documentation ........: ${AL4SAN_ENABLE_DOC}\n"
"       example ..............: ${AL4SAN_ENABLE_EXAMPLE}\n"
"       testing ..............: ${AL4SAN_ENABLE_TESTING}\n"
"       timing ...............: ${AL4SAN_ENABLE_TIMING}\n"
"\n"
"       AL4SAN dependencies :\n")
foreach (_dep ${AL4SAN_LIBRARIES_DEP})
    set(dep_message "${dep_message}"
    "                                 ${_dep}\n")
endforeach ()
set(dep_message "${dep_message}"
"\n"
"       INSTALL_PREFIX ......: ${CMAKE_INSTALL_PREFIX}\n\n")

string(REPLACE ";" " " dep_message_wsc "${dep_message}")
message(${dep_message})
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/config.log "${dep_message_wsc}")
message(STATUS "Configuration is done - A summary of the current configuration"
"\n   has been written in ${CMAKE_CURRENT_BINARY_DIR}/config.log")
# installation
# ------------
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.log DESTINATION share/al4san)
