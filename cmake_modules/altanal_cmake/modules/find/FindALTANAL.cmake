###
#
# @copyright (c) 2009-2014 The University of Tennessee and The University
#                          of Tennessee Research Foundation.
#                          All rights reserved.
# @copyright (c) 2012-2016 Inria. All rights reserved.
# @copyright (c) 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria, Univ. Bordeaux. All rights reserved.
#
# @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). All rights reserved.
###
#
# - Find ALTANAL include dirs and libraries
# Use this module by invoking find_package with the form:
#  find_package(ALTANAL
#               [REQUIRED]             # Fail with error if altanal is not found
#               [COMPONENTS <comp1> <comp2> ...] # dependencies
#              )
#
#  ALTANAL depends on the following libraries:
#   - Threads, m, rt
#   - HWLOC
#   - CBLAS
#   - LAPACKE
#   - TMG
#   - At least one runtime, default is StarPU
#     (For QUARK, use COMPONENTS QUARK)
#
#  COMPONENTS are optional libraries ALTANAL could be linked with,
#  Use it to drive detection of a specific compilation chain
#  COMPONENTS can be some of the following:
#   - STARPU (default): to activate detection of ALtanal linked with StarPU
#   - QUARK (STARPU will be deactivated): to activate detection of ALtanal linked with QUARK
#   - CUDA (comes with cuBLAS): to activate detection of ALtanal linked with CUDA
#   - MAGMA: to activate detection of ALtanal linked with MAGMA
#   - MPI: to activate detection of ALtanal linked with MPI
#   - FXT: to activate detection of ALtanal linked with StarPU+FXT
#
# This module finds headers and altanal library.
# Results are reported in variables:
#  ALTANAL_FOUND            - True if headers and requested libraries were found
#  ALTANAL_C_FLAGS          - list of required compilation flags (excluding -I)
#  ALTANAL_LINKER_FLAGS     - list of required linker flags (excluding -l and -L)
#  ALTANAL_INCLUDE_DIRS     - altanal include directories
#  ALTANAL_LIBRARY_DIRS     - Link directories for altanal libraries
#  ALTANAL_INCLUDE_DIRS_DEP - altanal + dependencies include directories
#  ALTANAL_LIBRARY_DIRS_DEP - altanal + dependencies link directories
#  ALTANAL_LIBRARIES_DEP    - altanal libraries + dependencies
# The user can give specific paths where to find the libraries adding cmake
# options at configure (ex: cmake path/to/project -DALTANAL_DIR=path/to/altanal):
#  ALTANAL_DIR              - Where to find the base directory of altanal
#  ALTANAL_INCDIR           - Where to find the header files
#  ALTANAL_LIBDIR           - Where to find the library files
# The module can also look for the following environment variables if paths
# are not given as cmake variable: ALTANAL_DIR, ALTANAL_INCDIR, ALTANAL_LIBDIR

#=============================================================================
# Copyright 2012-2013 Inria
# Copyright 2012-2013 Emmanuel Agullo
# Copyright 2012-2013 Mathieu Faverge
# Copyright 2012      Cedric Castagnede
# Copyright 2013-2016 Florent Pruvost
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file ALTANAL-Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of ALtanal, substitute the full
#  License text for the above reference.)


if (NOT ALTANAL_FOUND)
  set(ALTANAL_DIR "" CACHE PATH "Installation directory of ALTANAL library")
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "A cache variable, namely ALTANAL_DIR, has been set to specify the install directory of ALTANAL")
  endif()
endif()

# Try to find ALTANAL dependencies if specified as COMPONENTS during the call
set(ALTANAL_LOOK_FOR_STARPU ON)
set(ALTANAL_LOOK_FOR_QUARK OFF)
set(ALTANAL_LOOK_FOR_PARSEC OFF)
set(ALTANAL_LOOK_FOR_CUDA OFF)
set(ALTANAL_LOOK_FOR_MAGMA OFF)
set(ALTANAL_LOOK_FOR_MPI OFF)
set(ALTANAL_LOOK_FOR_FXT OFF)

if( ALTANAL_FIND_COMPONENTS )
  foreach( component ${ALTANAL_FIND_COMPONENTS} )
    if (${component} STREQUAL "STARPU")
      # means we look for ALtanal with StarPU
      set(ALTANAL_LOOK_FOR_STARPU ON)
      set(ALTANAL_LOOK_FOR_QUARK OFF)
      set(ALTANAL_LOOK_FOR_PARSEC OFF)
    endif()
    if (${component} STREQUAL "QUARK")
      # means we look for ALtanal with QUARK
      set(ALTANAL_LOOK_FOR_QUARK ON)
      set(ALTANAL_LOOK_FOR_STARPU OFF)
      set(ALTANAL_LOOK_FOR_PARSEC OFF)
    endif()
     if (${component} STREQUAL "PARSEC")
      # means we look for ALtanal with PARSEC
       set(ALTANAL_LOOK_FOR_PARSEC ON)
       set(ALTANAL_LOOK_FOR_STARPU OFF)
       set(ALTANAL_LOOK_FOR_QUARK OFF)
     endif()
    if (${component} STREQUAL "CUDA")
      # means we look for ALtanal with CUDA
      set(ALTANAL_LOOK_FOR_CUDA ON)
    endif()
    if (${component} STREQUAL "MAGMA")
      # means we look for ALtanal with MAGMA
      set(ALTANAL_LOOK_FOR_MAGMA ON)
    endif()
    if (${component} STREQUAL "MPI")
      # means we look for ALtanal with MPI
      set(ALTANAL_LOOK_FOR_MPI ON)
    endif()
    if (${component} STREQUAL "FXT")
      # means we look for ALtanal with FXT
      set(ALTANAL_LOOK_FOR_FXT ON)
    endif()
  endforeach()
endif()

set(ENV_ALTANAL_DIR "$ENV{ALTANAL_DIR}")
set(ENV_ALTANAL_INCDIR "$ENV{ALTANAL_INCDIR}")
set(ENV_ALTANAL_LIBDIR "$ENV{ALTANAL_LIBDIR}")
set(ALTANAL_GIVEN_BY_USER "FALSE")
if ( ALTANAL_DIR OR ( ALTANAL_INCDIR AND ALTANAL_LIBDIR) OR ENV_ALTANAL_DIR OR (ENV_ALTANAL_INCDIR AND ENV_ALTANAL_LIBDIR) )
  set(ALTANAL_GIVEN_BY_USER "TRUE")
endif()

# Optionally use pkg-config to detect include/library dirs (if pkg-config is available)
# -------------------------------------------------------------------------------------
include(FindPkgConfig)
find_package(PkgConfig QUIET)
if(PKG_CONFIG_EXECUTABLE AND NOT ALTANAL_GIVEN_BY_USER)

  pkg_search_module(ALTANAL altanal)
  if (NOT ALTANAL_FIND_QUIETLY)
    if (ALTANAL_FOUND AND ALTANAL_LIBRARIES)
      message(STATUS "Looking for ALTANAL - found using PkgConfig")
      #if(NOT ALTANAL_INCLUDE_DIRS)
      #    message("${Magenta}ALTANAL_INCLUDE_DIRS is empty using PkgConfig."
      #        "Perhaps the path to altanal headers is already present in your"
      #        "C(PLUS)_INCLUDE_PATH environment variable.${ColourReset}")
      #endif()
    else()
      message(STATUS "${Magenta}Looking for ALTANAL - not found using PkgConfig."
	"\n   Perhaps you should add the directory containing altanal.pc"
	"\n   to the PKG_CONFIG_PATH environment variable.${ColourReset}")
    endif()
  endif()

  if (ALTANAL_FIND_VERSION_EXACT)
    if( NOT (ALTANAL_FIND_VERSION_MAJOR STREQUAL ALTANAL_VERSION_MAJOR) OR
	NOT (ALTANAL_FIND_VERSION_MINOR STREQUAL ALTANAL_VERSION_MINOR) )
      if(NOT ALTANAL_FIND_QUIETLY)
	message(FATAL_ERROR
	  "ALTANAL version found is ${ALTANAL_VERSION_STRING}"
	  "when required is ${ALTANAL_FIND_VERSION}")
      endif()
    endif()
  else()
    # if the version found is older than the required then error
    if( (ALTANAL_FIND_VERSION_MAJOR STRGREATER ALTANAL_VERSION_MAJOR) OR
	(ALTANAL_FIND_VERSION_MINOR STRGREATER ALTANAL_VERSION_MINOR) )
      if(NOT ALTANAL_FIND_QUIETLY)
	message(FATAL_ERROR
	  "ALTANAL version found is ${ALTANAL_VERSION_STRING}"
	  "when required is ${ALTANAL_FIND_VERSION} or newer")
      endif()
    endif()
  endif()

  set(ALTANAL_C_FLAGS "${ALTANAL_CFLAGS_OTHER}")
  set(ALTANAL_INCLUDE_DIRS_DEP "${ALTANAL_STATIC_INCLUDE_DIRS}")
  set(ALTANAL_LIBRARY_DIRS_DEP "${ALTANAL_STATIC_LIBRARY_DIRS}")
  set(ALTANAL_LIBRARIES_DEP "${ALTANAL_STATIC_LIBRARIES}")

endif(PKG_CONFIG_EXECUTABLE AND NOT ALTANAL_GIVEN_BY_USER)

if( (NOT PKG_CONFIG_EXECUTABLE) OR (PKG_CONFIG_EXECUTABLE AND NOT ALTANAL_FOUND) OR (ALTANAL_GIVEN_BY_USER) )

  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - PkgConfig not used")
  endif()

  # Dependencies detection
  # ----------------------

  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect pthread")
  endif()
  if (ALTANAL_FIND_REQUIRED)
    find_package(Threads REQUIRED)
  else()
    find_package(Threads)
  endif()
  set(ALTANAL_EXTRA_LIBRARIES "")
  if( THREADS_FOUND )
    list(APPEND ALTANAL_EXTRA_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
  endif ()

  # Add math library to the list of extra
  # it normally exists on all common systems provided with a C compiler
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect libm")
  endif()
  set(ALTANAL_M_LIBRARIES "")
  if(UNIX OR WIN32)
    find_library(
      ALTANAL_M_m_LIBRARY
      NAMES m
      )
    mark_as_advanced(ALTANAL_M_m_LIBRARY)
    if (ALTANAL_M_m_LIBRARY)
      list(APPEND ALTANAL_M_LIBRARIES "${ALTANAL_M_m_LIBRARY}")
      list(APPEND ALTANAL_EXTRA_LIBRARIES "${ALTANAL_M_m_LIBRARY}")
    else()
      if (ALTANAL_FIND_REQUIRED)
	message(FATAL_ERROR "Could NOT find libm on your system."
	  "Are you sure to a have a C compiler installed?")
      endif()
    endif()
  endif()

  # Try to find librt (libposix4 - POSIX.1b Realtime Extensions library)
  # on Unix systems except Apple ones because it does not exist on it
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect librt")
  endif()
  set(ALTANAL_RT_LIBRARIES "")
  if(UNIX AND NOT APPLE)
    find_library(
      ALTANAL_RT_rt_LIBRARY
      NAMES rt
      )
    mark_as_advanced(ALTANAL_RT_rt_LIBRARY)
    if (ALTANAL_RT_rt_LIBRARY)
      list(APPEND ALTANAL_RT_LIBRARIES "${ALTANAL_RT_rt_LIBRARY}")
      list(APPEND ALTANAL_EXTRA_LIBRARIES "${ALTANAL_RT_rt_LIBRARY}")
    else()
      if (ALTANAL_FIND_REQUIRED)
	message(FATAL_ERROR "Could NOT find librt on your system")
      endif()
    endif()
  endif()

  # ALTANAL depends on CBLAS
  #---------------------------
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect CBLAS (depends on BLAS)")
  endif()
  if (ALTANAL_FIND_REQUIRED)
    find_package(CBLAS REQUIRED)
  else()
    find_package(CBLAS)
  endif()

  # ALTANAL depends on LAPACKE
  #-----------------------------

  # standalone version of lapacke seems useless for now
  # let the comment in case we meet some problems of non existing lapacke
  # functions in lapack library such as mkl, acml, ...
  #set(LAPACKE_STANDALONE TRUE)
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect LAPACKE (depends on LAPACK)")
  endif()
  if (ALTANAL_FIND_REQUIRED)
    find_package(LAPACKE REQUIRED)
  else()
    find_package(LAPACKE)
  endif()

  # ALTANAL depends on TMG
  #-------------------------
  if (NOT ALTANAL_FIND_QUIETLY)
    message(STATUS "Looking for ALTANAL - Try to detect TMG (depends on LAPACK)")
  endif()
  if (ALTANAL_FIND_REQUIRED)
    find_package(TMG REQUIRED)
  else()
    find_package(TMG)
  endif()

  # ALTANAL may depend on CUDA/CUBLAS
  #------------------------------------
  if (NOT CUDA_FOUND AND ALTANAL_LOOK_FOR_CUDA)
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_CUDA)
      find_package(CUDA REQUIRED)
    else()
      find_package(CUDA)
    endif()
    if (CUDA_FOUND)
      mark_as_advanced(CUDA_BUILD_CUBIN)
      mark_as_advanced(CUDA_BUILD_EMULATION)
      mark_as_advanced(CUDA_SDK_ROOT_DIR)
      mark_as_advanced(CUDA_TOOLKIT_ROOT_DIR)
      mark_as_advanced(CUDA_VERBOSE_BUILD)
    endif()
  endif()

  # ALTANAL may depend on MAGMA gpu kernels
  # call our cmake module to test (in cmake_modules)
  # change this call position if not appropriated
  #-------------------------------------------------
  if( CUDA_FOUND AND ALTANAL_LOOK_FOR_MAGMA )
    set(ALTANAL_MAGMA_VERSION "1.4" CACHE STRING "oldest MAGMA version desired")
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_MAGMA)
      find_package(MAGMA ${ALTANAL_MAGMA_VERSION} REQUIRED)
    else()
      find_package(MAGMA ${ALTANAL_MAGMA_VERSION})
    endif()
  endif()

  # ALTANAL depends on MPI
  #-------------------------
  if( NOT MPI_FOUND AND ALTANAL_LOOK_FOR_MPI )

    # allows to use an external mpi compilation by setting compilers with
    # -DMPI_C_COMPILER=path/to/mpicc -DMPI_Fortran_COMPILER=path/to/mpif90
    # at cmake configure
    if(NOT MPI_C_COMPILER)
      set(MPI_C_COMPILER mpicc)
    endif()
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_MPI)
      find_package(MPI REQUIRED)
    else()
      find_package(MPI)
    endif()
    if (MPI_FOUND)
      mark_as_advanced(MPI_LIBRARY)
      mark_as_advanced(MPI_EXTRA_LIBRARY)
    endif()

  endif()

  if( NOT STARPU_FOUND AND ALTANAL_LOOK_FOR_STARPU )

    set(ALTANAL_STARPU_VERSION "1.1" CACHE STRING "oldest STARPU version desired")

    # create list of components in order to make a single call to find_package(starpu...)
    # we explicitly need a StarPU version built with hwloc
    set(STARPU_COMPONENT_LIST "HWLOC")

    # StarPU may depend on MPI
    # allows to use an external mpi compilation by setting compilers with
    # -DMPI_C_COMPILER=path/to/mpicc -DMPI_Fortran_COMPILER=path/to/mpif90
    # at cmake configure
    if (ALTANAL_LOOK_FOR_MPI)
      if(NOT MPI_C_COMPILER)
	set(MPI_C_COMPILER mpicc)
      endif()
      list(APPEND STARPU_COMPONENT_LIST "MPI")
    endif()
    if (ALTANAL_LOOK_FOR_CUDA)
      list(APPEND STARPU_COMPONENT_LIST "CUDA")
    endif()
    if (ALTANAL_LOOK_FOR_FXT)
      list(APPEND STARPU_COMPONENT_LIST "FXT")
    endif()
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_STARPU)
      find_package(STARPU ${ALTANAL_STARPU_VERSION} REQUIRED
	COMPONENTS ${STARPU_COMPONENT_LIST})
    else()
      find_package(STARPU ${ALTANAL_STARPU_VERSION}
	COMPONENTS ${STARPU_COMPONENT_LIST})
    endif()

  endif()

  if( NOT QUARK_FOUND AND ALTANAL_LOOK_FOR_QUARK )

    # try to find quark runtime
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_QUARK)
      find_package(QUARK REQUIRED COMPONENTS HWLOC)
    else()
      find_package(QUARK COMPONENTS HWLOC)
    endif()

  if( NOT PARSEC_FOUND AND ALTANAL_LOOK_FOR_PARSEC)


    # create list of components in order to make a single call to find_package(parsec...)
    # we explicitly need a StarPU version built with hwloc
    set(PARSEC_COMPONENT_LIST "HWLOC")

    # PARSEC may depend on MPI
    # allows to use an external mpi compilation by setting compilers with
    # -DMPI_C_COMPILER=path/to/mpicc -DMPI_Fortran_COMPILER=path/to/mpif90
    # at cmake configure
    if (ALTANAL_LOOK_FOR_MPI)
      if(NOT MPI_C_COMPILER)
        set(MPI_C_COMPILER mpicc)
      endif()
      list(APPEND PARSEC_COMPONENT_LIST "MPI")
    endif()
    if (ALTANAL_LOOK_FOR_CUDA)
      list(APPEND PARSEC_COMPONENT_LIST "CUDA")
    endif()
    if (ALTANAL_FIND_REQUIRED AND ALTANAL_FIND_REQUIRED_PARSEC)
      find_package(PARSEC REQUIRED ${PARSEC_COMPONENT_LIST})
    else()
      find_package(PARSEC COMPONENTS ${PARSEC_COMPONENT_LIST})
    endif()
  endif()

  endif()

  # Looking for include
  # -------------------

  # Add system include paths to search include
  # ------------------------------------------
  unset(_inc_env)
  set(ENV_ALTANAL_DIR "$ENV{ALTANAL_DIR}")
  set(ENV_ALTANAL_INCDIR "$ENV{ALTANAL_INCDIR}")
  if(ENV_ALTANAL_INCDIR)
    list(APPEND _inc_env "${ENV_ALTANAL_INCDIR}")
  elseif(ENV_ALTANAL_DIR)
    list(APPEND _inc_env "${ENV_ALTANAL_DIR}")
    list(APPEND _inc_env "${ENV_ALTANAL_DIR}/include")
    list(APPEND _inc_env "${ENV_ALTANAL_DIR}/include/altanal")
  else()
    if(WIN32)
      string(REPLACE ":" ";" _inc_env "$ENV{INCLUDE}")
    else()
      string(REPLACE ":" ";" _path_env "$ENV{INCLUDE}")
      list(APPEND _inc_env "${_path_env}")
      string(REPLACE ":" ";" _path_env "$ENV{C_INCLUDE_PATH}")
      list(APPEND _inc_env "${_path_env}")
      string(REPLACE ":" ";" _path_env "$ENV{CPATH}")
      list(APPEND _inc_env "${_path_env}")
      string(REPLACE ":" ";" _path_env "$ENV{INCLUDE_PATH}")
      list(APPEND _inc_env "${_path_env}")
    endif()
  endif()
  list(APPEND _inc_env "${CMAKE_PLATFORM_IMPLICIT_INCLUDE_DIRECTORIES}")
  list(APPEND _inc_env "${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}")
  list(REMOVE_DUPLICATES _inc_env)


  # Try to find the altanal header in the given paths
  # ---------------------------------------------------
  # call cmake macro to find the header path
  if(ALTANAL_INCDIR)
    set(ALTANAL_altanal.h_DIRS "ALTANAL_altanal.h_DIRS-NOTFOUND")
    find_path(ALTANAL_altanal.h_DIRS
      NAMES altanal.h
      HINTS ${ALTANAL_INCDIR})
  else()
    if(ALTANAL_DIR)
      set(ALTANAL_altanal.h_DIRS "ALTANAL_altanal.h_DIRS-NOTFOUND")
      find_path(ALTANAL_altanal.h_DIRS
	NAMES altanal.h
	HINTS ${ALTANAL_DIR}
	PATH_SUFFIXES "include" "include/altanal")
    else()
      set(ALTANAL_altanal.h_DIRS "ALTANAL_altanal.h_DIRS-NOTFOUND")
      find_path(ALTANAL_altanal.h_DIRS
	NAMES altanal.h
	HINTS ${_inc_env}
	PATH_SUFFIXES "altanal")
    endif()
  endif()
  mark_as_advanced(ALTANAL_altanal.h_DIRS)

  # If found, add path to cmake variable
  # ------------------------------------
  if (ALTANAL_altanal.h_DIRS)
    set(ALTANAL_INCLUDE_DIRS "${ALTANAL_altanal.h_DIRS}")
  else ()
    set(ALTANAL_INCLUDE_DIRS "ALTANAL_INCLUDE_DIRS-NOTFOUND")
    if(NOT ALTANAL_FIND_QUIETLY)
      message(STATUS "Looking for altanal -- altanal.h not found")
    endif()
  endif()


  # Looking for lib
  # ---------------

  # Add system library paths to search lib
  # --------------------------------------
  unset(_lib_env)
  set(ENV_ALTANAL_LIBDIR "$ENV{ALTANAL_LIBDIR}")
  if(ENV_ALTANAL_LIBDIR)
    list(APPEND _lib_env "${ENV_ALTANAL_LIBDIR}")
  elseif(ENV_ALTANAL_DIR)
    list(APPEND _lib_env "${ENV_ALTANAL_DIR}")
    list(APPEND _lib_env "${ENV_ALTANAL_DIR}/lib")
  else()
    if(WIN32)
      string(REPLACE ":" ";" _lib_env "$ENV{LIB}")
    else()
      if(APPLE)
	string(REPLACE ":" ";" _lib_env "$ENV{DYLD_LIBRARY_PATH}")
      else()
	string(REPLACE ":" ";" _lib_env "$ENV{LD_LIBRARY_PATH}")
      endif()
      list(APPEND _lib_env "${CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES}")
      list(APPEND _lib_env "${CMAKE_C_IMPLICIT_LINK_DIRECTORIES}")
    endif()
  endif()
  list(REMOVE_DUPLICATES _lib_env)

  # Try to find the altanal lib in the given paths
  # ------------------------------------------------

  # create list of libs to find
  set(ALTANAL_libs_to_find "altanal")
  if (STARPU_FOUND)
    list(APPEND ALTANAL_libs_to_find "altanal_starpu")
  elseif (QUARK_FOUND)
    list(APPEND ALTANAL_libs_to_find "altanal_quark")
      elseif (PARSEC_FOUND)
    list(APPEND ALTANAL_libs_to_find "altanal_parsec")
  endif()
  list(APPEND ALTANAL_libs_to_find "coreblas")

  # call cmake macro to find the lib path
  if(ALTANAL_LIBDIR)
    foreach(altanal_lib ${ALTANAL_libs_to_find})
      set(ALTANAL_${altanal_lib}_LIBRARY "ALTANAL_${altanal_lib}_LIBRARY-NOTFOUND")
      find_library(ALTANAL_${altanal_lib}_LIBRARY
	NAMES ${altanal_lib}
	HINTS ${ALTANAL_LIBDIR})
    endforeach()
  else()
    if(ALTANAL_DIR)
      foreach(altanal_lib ${ALTANAL_libs_to_find})
	set(ALTANAL_${altanal_lib}_LIBRARY "ALTANAL_${altanal_lib}_LIBRARY-NOTFOUND")
	find_library(ALTANAL_${altanal_lib}_LIBRARY
	  NAMES ${altanal_lib}
	  HINTS ${ALTANAL_DIR}
	  PATH_SUFFIXES lib lib32 lib64)
      endforeach()
    else()
      foreach(altanal_lib ${ALTANAL_libs_to_find})
	set(ALTANAL_${altanal_lib}_LIBRARY "ALTANAL_${altanal_lib}_LIBRARY-NOTFOUND")
	find_library(ALTANAL_${altanal_lib}_LIBRARY
	  NAMES ${altanal_lib}
	  HINTS ${_lib_env})
      endforeach()
    endif()
  endif()

  # If found, add path to cmake variable
  # ------------------------------------
  foreach(altanal_lib ${ALTANAL_libs_to_find})

    get_filename_component(${altanal_lib}_lib_path ${ALTANAL_${altanal_lib}_LIBRARY} PATH)
    # set cmake variables (respects naming convention)
    if (ALTANAL_LIBRARIES)
      list(APPEND ALTANAL_LIBRARIES "${ALTANAL_${altanal_lib}_LIBRARY}")
    else()
      set(ALTANAL_LIBRARIES "${ALTANAL_${altanal_lib}_LIBRARY}")
    endif()
    if (ALTANAL_LIBRARY_DIRS)
      list(APPEND ALTANAL_LIBRARY_DIRS "${${altanal_lib}_lib_path}")
    else()
      set(ALTANAL_LIBRARY_DIRS "${${altanal_lib}_lib_path}")
    endif()
    mark_as_advanced(ALTANAL_${altanal_lib}_LIBRARY)

  endforeach(altanal_lib ${ALTANAL_libs_to_find})

  # check a function to validate the find
  if(ALTANAL_LIBRARIES)

    set(REQUIRED_LDFLAGS)
    set(REQUIRED_INCDIRS)
    set(REQUIRED_LIBDIRS)
    set(REQUIRED_LIBS)

    # ALTANAL
    if (ALTANAL_INCLUDE_DIRS)
      set(REQUIRED_INCDIRS "${ALTANAL_INCLUDE_DIRS}")
    endif()
    foreach(libdir ${ALTANAL_LIBRARY_DIRS})
      if (libdir)
	list(APPEND REQUIRED_LIBDIRS "${libdir}")
      endif()
    endforeach()
    set(REQUIRED_LIBS "${ALTANAL_LIBRARIES}")
    # STARPU
    if (STARPU_FOUND AND ALTANAL_LOOK_FOR_STARPU)
      if (STARPU_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${STARPU_INCLUDE_DIRS_DEP}")
      elseif (STARPU_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${STARPU_INCLUDE_DIRS}")
      endif()
      if(STARPU_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${STARPU_LIBRARY_DIRS_DEP}")
      elseif(STARPU_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${STARPU_LIBRARY_DIRS}")
      endif()
      if (STARPU_LIBRARIES_DEP)
	list(APPEND REQUIRED_LIBS "${STARPU_LIBRARIES_DEP}")
      elseif (STARPU_LIBRARIES)
	foreach(lib ${STARPU_LIBRARIES})
	  if (EXISTS ${lib} OR ${lib} MATCHES "^-")
	    list(APPEND REQUIRED_LIBS "${lib}")
	  else()
	    list(APPEND REQUIRED_LIBS "-l${lib}")
	  endif()
	endforeach()
      endif()
    endif()
    # QUARK
    if (QUARK_FOUND AND ALTANAL_LOOK_FOR_QUARK)
      if (QUARK_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${QUARK_INCLUDE_DIRS_DEP}")
      elseif(QUARK_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${QUARK_INCLUDE_DIRS}")
      endif()
      if(QUARK_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${QUARK_LIBRARY_DIRS_DEP}")
      elseif(QUARK_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${QUARK_LIBRARY_DIRS}")
      endif()
      if (QUARK_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBS "${QUARK_LIBRARIES_DEP}")
      elseif (QUARK_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBS "${QUARK_LIBRARIES}")
      endif()
    endif()
    # PARSEC
    if (PARSEC_FOUND AND ALTANAL_LOOK_FOR_PARSEC)
      if (PARSEC_INCLUDE_DIRS_DEP)
        list(APPEND REQUIRED_INCDIRS "${PARSEC_INCLUDE_DIRS_DEP}")
      elseif (PARSEC_INCLUDE_DIRS)
        list(APPEND REQUIRED_INCDIRS "${PARSEC_INCLUDE_DIRS}")
      endif()
      if(PARSEC_LIBRARY_DIRS_DEP)
        list(APPEND REQUIRED_LIBDIRS "${PARSEC_LIBRARY_DIRS_DEP}")
      elseif(PARSEC_LIBRARY_DIRS)
        list(APPEND REQUIRED_LIBDIRS "${PARSEC_LIBRARY_DIRS}")
      endif()
      if (PARSEC_LIBRARIES_DEP)
        list(APPEND REQUIRED_LIBS "${PARSEC_LIBRARIES_DEP}")
      elseif (PARSEC_LIBRARIES)
        foreach(lib ${PARSEC_LIBRARIES})
          if (EXISTS ${lib} OR ${lib} MATCHES "^-")
            list(APPEND REQUIRED_LIBS "${lib}")
          else()
            list(APPEND REQUIRED_LIBS "-l${lib}")
          endif()
        endforeach()
      endif()
    endif()
    # CUDA
    if (CUDA_FOUND AND ALTANAL_LOOK_FOR_CUDA)
      if (CUDA_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${CUDA_INCLUDE_DIRS}")
      endif()
      foreach(libdir ${CUDA_LIBRARY_DIRS})
	if (libdir)
	  list(APPEND REQUIRED_LIBDIRS "${libdir}")
	endif()
      endforeach()
      list(APPEND REQUIRED_LIBS "${CUDA_CUBLAS_LIBRARIES};${CUDA_LIBRARIES}")
    endif()
    # MAGMA
    if (MAGMA_FOUND AND ALTANAL_LOOK_FOR_MAGMA)
      if (MAGMA_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${MAGMA_INCLUDE_DIRS_DEP}")
      elseif(MAGMA_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${MAGMA_INCLUDE_DIRS}")
      endif()
      if (MAGMA_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${MAGMA_LIBRARY_DIRS_DEP}")
      elseif(MAGMA_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${MAGMA_LIBRARY_DIRS}")
      endif()
      if (MAGMA_LIBRARIES_DEP)
	list(APPEND REQUIRED_LIBS "${MAGMA_LIBRARIES_DEP}")
      elseif(MAGMA_LIBRARIES)
	foreach(lib ${MAGMA_LIBRARIES})
	  if (EXISTS ${lib} OR ${lib} MATCHES "^-")
	    list(APPEND REQUIRED_LIBS "${lib}")
	  else()
	    list(APPEND REQUIRED_LIBS "-l${lib}")
	  endif()
	endforeach()
      endif()
    endif()
    # MPI
    if (MPI_FOUND AND ALTANAL_LOOK_FOR_MPI)
      if (MPI_C_INCLUDE_PATH)
	list(APPEND REQUIRED_INCDIRS "${MPI_C_INCLUDE_PATH}")
      endif()
      if (MPI_C_LINK_FLAGS)
	if (${MPI_C_LINK_FLAGS} MATCHES "  -")
	  string(REGEX REPLACE " -" "-" MPI_C_LINK_FLAGS ${MPI_C_LINK_FLAGS})
	endif()
	list(APPEND REQUIRED_LDFLAGS "${MPI_C_LINK_FLAGS}")
      endif()
      list(APPEND REQUIRED_LIBS "${MPI_C_LIBRARIES}")
    endif()
    # HWLOC
    if (HWLOC_FOUND)
      if (HWLOC_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${HWLOC_INCLUDE_DIRS}")
      endif()
      foreach(libdir ${HWLOC_LIBRARY_DIRS})
	if (libdir)
	  list(APPEND REQUIRED_LIBDIRS "${libdir}")
	endif()
      endforeach()
      foreach(lib ${HWLOC_LIBRARIES})
	if (EXISTS ${lib} OR ${lib} MATCHES "^-")
	  list(APPEND REQUIRED_LIBS "${lib}")
	else()
	  list(APPEND REQUIRED_LIBS "-l${lib}")
	endif()
      endforeach()
    endif()
    # TMG
    if (TMG_FOUND)
      if (TMG_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${TMG_INCLUDE_DIRS_DEP}")
      elseif (TMG_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${TMG_INCLUDE_DIRS}")
      endif()
      if(TMG_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${TMG_LIBRARY_DIRS_DEP}")
      elseif(TMG_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${TMG_LIBRARY_DIRS}")
      endif()
      if (TMG_LIBRARIES_DEP)
	list(APPEND REQUIRED_LIBS "${TMG_LIBRARIES_DEP}")
      elseif(TMG_LIBRARIES)
	list(APPEND REQUIRED_LIBS "${TMG_LIBRARIES}")
      endif()
      if (TMG_LINKER_FLAGS)
	list(APPEND REQUIRED_LDFLAGS "${TMG_LINKER_FLAGS}")
      endif()
    endif()
    # LAPACKE
    if (LAPACKE_FOUND)
      if (LAPACKE_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${LAPACKE_INCLUDE_DIRS_DEP}")
      elseif (LAPACKE_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${LAPACKE_INCLUDE_DIRS}")
      endif()
      if(LAPACKE_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${LAPACKE_LIBRARY_DIRS_DEP}")
      elseif(LAPACKE_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${LAPACKE_LIBRARY_DIRS}")
      endif()
      if (LAPACKE_LIBRARIES_DEP)
	list(APPEND REQUIRED_LIBS "${LAPACKE_LIBRARIES_DEP}")
      elseif(LAPACKE_LIBRARIES)
	list(APPEND REQUIRED_LIBS "${LAPACKE_LIBRARIES}")
      endif()
      if (LAPACK_LINKER_FLAGS)
	list(APPEND REQUIRED_LDFLAGS "${LAPACK_LINKER_FLAGS}")
      endif()
    endif()
    # CBLAS
    if (CBLAS_FOUND)
      if (CBLAS_INCLUDE_DIRS_DEP)
	list(APPEND REQUIRED_INCDIRS "${CBLAS_INCLUDE_DIRS_DEP}")
      elseif (CBLAS_INCLUDE_DIRS)
	list(APPEND REQUIRED_INCDIRS "${CBLAS_INCLUDE_DIRS}")
      endif()
      if(CBLAS_LIBRARY_DIRS_DEP)
	list(APPEND REQUIRED_LIBDIRS "${CBLAS_LIBRARY_DIRS_DEP}")
      elseif(CBLAS_LIBRARY_DIRS)
	list(APPEND REQUIRED_LIBDIRS "${CBLAS_LIBRARY_DIRS}")
      endif()
      if (CBLAS_LIBRARIES_DEP)
	list(APPEND REQUIRED_LIBS "${CBLAS_LIBRARIES_DEP}")
      elseif(CBLAS_LIBRARIES)
	list(APPEND REQUIRED_LIBS "${CBLAS_LIBRARIES}")
      endif()
      if (BLAS_LINKER_FLAGS)
	list(APPEND REQUIRED_LDFLAGS "${BLAS_LINKER_FLAGS}")
      endif()
    endif()
    # EXTRA LIBS such that pthread, m, rt
    list(APPEND REQUIRED_LIBS ${ALTANAL_EXTRA_LIBRARIES})

    # set required libraries for link
    set(CMAKE_REQUIRED_INCLUDES "${REQUIRED_INCDIRS}")
    set(CMAKE_REQUIRED_LIBRARIES)
    list(APPEND CMAKE_REQUIRED_LIBRARIES "${REQUIRED_LDFLAGS}")
    foreach(lib_dir ${REQUIRED_LIBDIRS})
      list(APPEND CMAKE_REQUIRED_LIBRARIES "-L${lib_dir}")
    endforeach()
    list(APPEND CMAKE_REQUIRED_LIBRARIES "${REQUIRED_LIBS}")
    string(REGEX REPLACE "^ -" "-" CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")

    # test link
    unset(ALTANAL_WORKS CACHE)
    include(CheckFunctionExists)
    check_function_exists(ALTANAL_Init ALTANAL_WORKS)
    mark_as_advanced(ALTANAL_WORKS)

    if(ALTANAL_WORKS)
      # save link with dependencies
      set(ALTANAL_LIBRARIES_DEP "${REQUIRED_LIBS}")
      set(ALTANAL_LIBRARY_DIRS_DEP "${REQUIRED_LIBDIRS}")
      set(ALTANAL_INCLUDE_DIRS_DEP "${REQUIRED_INCDIRS}")
      set(ALTANAL_LINKER_FLAGS "${REQUIRED_LDFLAGS}")
      list(REMOVE_DUPLICATES ALTANAL_LIBRARY_DIRS_DEP)
      list(REMOVE_DUPLICATES ALTANAL_INCLUDE_DIRS_DEP)
      list(REMOVE_DUPLICATES ALTANAL_LINKER_FLAGS)
    else()
      if(NOT ALTANAL_FIND_QUIETLY)
	message(STATUS "Looking for altanal : test of ALTANAL_Init fails")
	message(STATUS "CMAKE_REQUIRED_LIBRARIES: ${CMAKE_REQUIRED_LIBRARIES}")
	message(STATUS "CMAKE_REQUIRED_INCLUDES: ${CMAKE_REQUIRED_INCLUDES}")
	message(STATUS "Check in CMakeFiles/CMakeError.log to figure out why it fails")
	message(STATUS "Maybe ALTANAL is linked with specific libraries. "
	  "Have you tried with COMPONENTS (STARPU/QUARK, CUDA, MAGMA, MPI, FXT)? "
	  "See the explanation in FindALTANAL.cmake.")
      endif()
    endif()
    set(CMAKE_REQUIRED_INCLUDES)
    set(CMAKE_REQUIRED_FLAGS)
    set(CMAKE_REQUIRED_LIBRARIES)
  endif(ALTANAL_LIBRARIES)

endif( (NOT PKG_CONFIG_EXECUTABLE) OR (PKG_CONFIG_EXECUTABLE AND NOT ALTANAL_FOUND) OR (ALTANAL_GIVEN_BY_USER) )

if (ALTANAL_LIBRARIES)
  if (ALTANAL_LIBRARY_DIRS)
    foreach(dir ${ALTANAL_LIBRARY_DIRS})
      if ("${dir}" MATCHES "altanal")
	set(first_lib_path "${dir}")
      endif()
    endforeach()
  else()
    list(GET ALTANAL_LIBRARIES 0 first_lib)
    get_filename_component(first_lib_path "${first_lib}" PATH)
  endif()
  if (${first_lib_path} MATCHES "/lib(32|64)?$")
    string(REGEX REPLACE "/lib(32|64)?$" "" not_cached_dir "${first_lib_path}")
    set(ALTANAL_DIR_FOUND "${not_cached_dir}" CACHE PATH "Installation directory of ALTANAL library" FORCE)
  else()
    set(ALTANAL_DIR_FOUND "${first_lib_path}" CACHE PATH "Installation directory of ALTANAL library" FORCE)
  endif()
endif()
mark_as_advanced(ALTANAL_DIR)
mark_as_advanced(ALTANAL_DIR_FOUND)

# check that ALTANAL has been found
# ---------------------------------
include(FindPackageHandleStandardArgs)
if (PKG_CONFIG_EXECUTABLE AND ALTANAL_FOUND)
  find_package_handle_standard_args(ALTANAL DEFAULT_MSG
    ALTANAL_LIBRARIES)
else()
  find_package_handle_standard_args(ALTANAL DEFAULT_MSG
    ALTANAL_LIBRARIES
    ALTANAL_WORKS)
endif()
