###
#
# @file GenPkgConfig.cmake
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2017 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
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
# @version 1.0.1
#  @author Cedric Castagnede
#  @author Emmanuel Agullo
#  @author Mathieu Faverge
#  @author Florent Pruvost
#  @date 2019-02-06
#
###

###
#
# CONVERT_LIBSTYLE_TO_PKGCONFIG: convert a libraries list to follow the pkg-config style
#                                used in CLEAN_LIB_LIST
#
###
MACRO(CONVERT_LIBSTYLE_TO_PKGCONFIG _liblist)
    set(${_liblist}_CPY "${${_liblist}}")
    set(${_liblist} "")
    foreach(_dep ${${_liblist}_CPY})
        if (${_dep} MATCHES "^/")
            get_filename_component(dep_libname ${_dep} NAME)
            get_filename_component(dep_libdir  ${_dep} DIRECTORY)
            STRING(REPLACE "lib"    "" dep_libname "${dep_libname}")
            STRING(REPLACE ".so"    "" dep_libname "${dep_libname}")
            STRING(REPLACE ".a"     "" dep_libname "${dep_libname}")
            STRING(REPLACE ".dylib" "" dep_libname "${dep_libname}")
            STRING(REPLACE ".dll"   "" dep_libname "${dep_libname}")
            list(APPEND ${_liblist} -L${dep_libdir} -l${dep_libname})
        elseif(NOT ${_dep} MATCHES "^-")
            list(APPEND ${_liblist} "-l${_dep}")
        else()
            list(APPEND ${_liblist} ${_dep})
        endif()
    endforeach()
ENDMACRO(CONVERT_LIBSTYLE_TO_PKGCONFIG)

###
#
# CLEAN_LIB_LIST: clean libraries lists to follow the pkg-config style
#                 used in GENERATE_PKGCONFIG_FILE
#
###
MACRO(CLEAN_LIB_LIST _package)
    list(REMOVE_DUPLICATES ${_package}_PKGCONFIG_LIBS)
    list(REMOVE_DUPLICATES ${_package}_PKGCONFIG_LIBS_PRIVATE)
#    list(REMOVE_DUPLICATES ${_package}_PKGCONFIG_REQUIRED)
    list(REMOVE_DUPLICATES ${_package}_PKGCONFIG_REQUIRED_PRIVATE)
    CONVERT_LIBSTYLE_TO_PKGCONFIG(${_package}_PKGCONFIG_LIBS)
    CONVERT_LIBSTYLE_TO_PKGCONFIG(${_package}_PKGCONFIG_LIBS_PRIVATE)
    STRING(REPLACE ";" " " ${_package}_PKGCONFIG_LIBS "${${_package}_PKGCONFIG_LIBS}")
    STRING(REPLACE ";" " " ${_package}_PKGCONFIG_LIBS_PRIVATE "${${_package}_PKGCONFIG_LIBS_PRIVATE}")
    STRING(REPLACE ";" " " ${_package}_PKGCONFIG_REQUIRED "${${_package}_PKGCONFIG_REQUIRED}")
    STRING(REPLACE ";" " " ${_package}_PKGCONFIG_REQUIRED_PRIVATE "${${_package}_PKGCONFIG_REQUIRED_PRIVATE}")
ENDMACRO(CLEAN_LIB_LIST)

###
#
# GENERATE_PKGCONFIG_FILE: generate files al4san.pc
#
###
MACRO(GENERATE_PKGCONFIG_FILE)

    # The definitions that should be given to users (change the API)
    set(AL4SAN_PKGCONFIG_DEFINITIONS "")
    set(DIR_INC "")
    if(AL4SAN_SCHED_STARPU)
       list(APPEND DIR_INC "${STARPU_INCLUDE_DIRS_DEP}")
    elseif(AL4SAN_SCHED_QUARK)
       list(APPEND DIR_INC "${QUARK_INCLUDE_DIRS}")
    elseif(AL4SAN_SCHED_PARSEC)
       list(APPEND DIR_INC "${PARSEC_INCLUDE_DIRS_DEP}")
    elseif(AL4SAN_SCHED_OPENMP)
       list(APPEND DIR_INC "${OPENMP_INCLUDE_DIRS}")
    endif()
    if(AL4SAN_USE_MPI)
       list(APPEND DIR_INC "${MPI_C_INCLUDE_PATH}")
    endif()
    if(AL4SAN_USE_CUDA)
       list(APPEND DIR_INC "${${CUDA_INCLUDE_DIRS}}")
    endif()

    #set(DIR_INC "${QUARK_INCLUDE_DIRS}")
    # The link flags specific to this package and any required libraries
    # that don't support PkgConfig
    set(AL4SAN_PKGCONFIG_LIBS "-lal4san")

    # The link flags for private libraries required by this package but not
    # exposed to applications
    set(AL4SAN_PKGCONFIG_LIBS_PRIVATE "")

    # A list of packages required by this package
    #set(AL4SAN_PKGCONFIG_REQUIRED "hqr")

    # A list of private packages required by this package but not exposed to
    # applications
    set(AL4SAN_PKGCONFIG_REQUIRED_PRIVATE "")

    if(AL4SAN_SCHED_STARPU)
        list(APPEND AL4SAN_PKGCONFIG_LIBS -lal4san_starpu)
        if ( AL4SAN_USE_MPI )
            list(APPEND AL4SAN_PKGCONFIG_LIBS "${STARPU_LIBRARY_DIRS_DEP}")
            list(APPEND AL4SAN_PKGCONFIG_LIBS -lstarpumpi-${STARPU_VERSION})
            list(APPEND AL4SAN_PKGCONFIG_REQUIRED starpumpi-${STARPU_VERSION})            
         else()
            list(APPEND AL4SAN_PKGCONFIG_LIBS "${STARPU_LIBRARIES}") 
#           list(APPEND AL4SAN_PKGCONFIG_REQUIRED starpu-${STARPU_VERSION})
        endif()
    elseif(AL4SAN_SCHED_QUARK)
        list(APPEND AL4SAN_PKGCONFIG_LIBS -lal4san_quark)
        list(APPEND AL4SAN_PKGCONFIG_LIBS "${QUARK_LIBRARIES_DEP}")
    elseif(AL4SAN_SCHED_PARSEC)
       list(APPEND AL4SAN_PKGCONFIG_LIBS -lal4san_parsec)
        list(APPEND AL4SAN_PKGCONFIG_LIBS "${PARSEC_LIBRARIES_DEP}")
    elseif(AL4SAN_SCHED_OPENMP)
       list(APPEND AL4SAN_PKGCONFIG_LIBS -lal4san_openmp)
        list(APPEND AL4SAN_PKGCONFIG_LIBS "${OPENMP_LIBRARIES_DEP}")
    endif()


    list(APPEND AL4SAN_PKGCONFIG_LIBS
        ${EXTRA_LIBRARIES}
    )

    # Define required package
    # -----------------------
    CLEAN_LIB_LIST(AL4SAN)

    # Create .pc file
    # ---------------
    SET(_output_al4san_file "${CMAKE_BINARY_DIR}/al4san.pc")

    # TODO: add url of AL4SAN releases in .pc file
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/lib/pkgconfig/al4san.pc.in" "${_output_al4san_file}" @ONLY)

    # installation
    # ------------
    INSTALL(FILES ${_output_al4san_file} DESTINATION lib/pkgconfig)

ENDMACRO(GENERATE_PKGCONFIG_FILE)

##
## @end file GenPkgConfig.cmake
##
