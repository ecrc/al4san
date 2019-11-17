###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2018 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2018 King Abdullah University of Science and Technology (KAUST). 
#                      All rights reserved.
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
#  version 1.0.0
#  author Cedric Castagnede
#  author Emmanuel Agullo
#  author Mathieu Faverge
#  author Florent Pruvost
#  date 2012-07-13
###
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
# @version 1.0.1
#  @author Rabab Alomairy
#  @date 2019-02-06
#
###
cmake_minimum_required(VERSION 2.8)
project(AL4SAN Fortran C CXX)

# directly make an error if in-source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(FATAL_ERROR "In-source builds are not allowed.\n"
   "Please create a build directory first and execute cmake configuration from "
   "this directory. Example: mkdir build && cd build && cmake ..")
endif()

# set project version number
set(AL4SAN_VERSION_MAJOR 1)
set(AL4SAN_VERSION_MINOR 0)
set(AL4SAN_VERSION_MICRO 0)

set(AL4SAN_CMAKE_DIR "" CACHE PATH "Directory of AL4SAN CMake modules, can be external to the project")

# Add extra cmake module path and initialize al4san cmake modules
# --------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
if(AL4SAN_CMAKE_DIR)
    list(APPEND CMAKE_MODULE_PATH "${AL4SAN_CMAKE_DIR}/cmake_modules/al4san_cmake/modules")
    set(AL4SAN_CMAKE_MODULE_PATH ${AL4SAN_CMAKE_DIR}/cmake_modules/al4san_cmake/modules )
elseif(EXISTS "${CMAKE_SOURCE_DIR}/cmake_modules/al4san_cmake")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/al4san_cmake/modules")
    set(AL4SAN_CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules/al4san_cmake/modules )
else()
  message(FATAL_ERROR "Al4san CMake system relies on al4san_cmake modules developed here: "
    "https://gitlab.inria.fr/solverstack/al4san_cmake. Please set AL4SAN_CMAKE_DIR to this source "
    "directory.")
endif()

include(Al4sanInit)
include(GenPkgConfig)

# Ensure that the fortran compiler and c compiler specified are compatible
# ------------------------------------------------------------------------
include(FortranCInterface)
FortranCInterface_VERIFY()
FortranCInterface_HEADER(${CMAKE_CURRENT_BINARY_DIR}/include/al4san/al4san_mangling.h
                         MACRO_NAMESPACE "AL4SAN_")

#############################################
#                                           #
#        Compilation of AL4SAN           #
#                                           #
#############################################


###############################################################################
# Parameters/Options #
######################

# Set the RPATH config
# --------------------

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH to be used when installing
list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Misc options
# ------------
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Define precision supported by AL4SAN
# -----------------------------------------
set( RP_AL4SAN_DICTIONNARY ${AL4SAN_CMAKE_MODULE_PATH}/precision_generator/subs.py )
set( RP_AL4SAN_PRECISIONS  "s;d;c;z" )
include(RulesPrecisions)

option(AL4SAN_USE_MIGRATE
  "This options enables the data migration in QR algorithms" ON)

# Options to select the runtime
# -----------------------------

# Create a list of possible runtime
set(AL4SAN_SCHED_list "PARSEC" "STARPU" "QUARK"  "OPENMP")

# Initially PaRSEC runtime is enabled
option(AL4SAN_SCHED_PARSEC
    "Enable PaRSEC scheduler as the default runtime
    (Conflict with other AL4SAN_SCHED_* options)" OFF)
option(AL4SAN_SCHED_STARPU
    "Enable StarPU scheduler as the default runtime
    (Conflict with other AL4SAN_SCHED_* options)" OFF)
option(AL4SAN_SCHED_QUARK
    "Enable Quark scheduler as the default runtime
    (Conflict with other AL4SAN_SCHED_* options)" OFF)
option(AL4SAN_SCHED_OPENMP
    "Enable OPENMP scheduler as the default runtime
    (Conflict with other AL4SAN_SCHED_* options)" OFF)

# For now, we are able to compile only one runtime at a time, so we disable combinations of runtimes
#if (AL4SAN_SCHED_QUARK)
#    set(AL4SAN_SCHED_STARPU OFF)
#    set(AL4SAN_SCHED_PARSEC OFF)
#    set(AL4SAN_SCHED_OPENMP OFF)
#elseif (AL4SAN_SCHED_STARPU)
#    set(AL4SAN_SCHED_QUARK OFF)
#    set(AL4SAN_SCHED_PARSEC OFF)
#    set(AL4SAN_SCHED_OPENMP OFF)
#elseif (AL4SAN_SCHED_PARSEC)
#    set(AL4SAN_SCHED_QUARK OFF)
#    set(AL4SAN_SCHED_STARPU OFF)
#    set(AL4SAN_SCHED_OPENMP OFF)
#elseif (AL4SAN_SCHED_OPENMP)
#    set(AL4SAN_SCHED_QUARK OFF)
#    set(AL4SAN_SCHED_STARPU OFF)
#    set(AL4SAN_SCHED_PARSEC OFF)
#endif()

# Set default to StarPU if nothing specific is required by the user
#if ( NOT AL4SAN_SCHED_STARPU AND NOT AL4SAN_SCHED_PARSEC AND NOT AL4SAN_SCHED_QUARK AND NOT AL4SAN_SCHED_OPENMP)
#    set(AL4SAN_SCHED_STARPU ON)
#endif()
if (AL4SAN_SCHED_STARPU)
    message("-- ${BoldGreen}AL4SAN_SCHED_STARPU is set to ON: AL4SAN uses StarPU runtime\n"
            "   To use AL4SAN with Quark  runtime: set AL4SAN_SCHED_QUARK  to ON\n"
            "   To use AL4SAN with PaRSEC runtime: set AL4SAN_SCHED_PARSEC to ON\n"
            "   To use AL4SAN with OPENMP runtime: set AL4SAN_SCHED_OPENMP to ON\n"
            "   (AL4SAN_SCHED_STARPU will be disabled)${ColourReset}")
endif()
if(AL4SAN_SCHED_QUARK)
    message("-- ${BoldGreen}AL4SAN_SCHED_QUARK is set to ON: AL4SAN uses Quark runtime\n"
            "   To use AL4SAN with StarPU runtime: set AL4SAN_SCHED_STARPU to ON\n"
            "   To use AL4SAN with PaRSEC runtime: set AL4SAN_SCHED_PARSEC to ON\n"
            "   To use AL4SAN with OPENMP runtime: set AL4SAN_SCHED_OPENMP to ON\n"
            "   (AL4SAN_SCHED_QUARK will be disabled)${ColourReset}")
endif()
if(AL4SAN_SCHED_PARSEC)
    message("-- ${BoldGreen}AL4SAN_SCHED_PARSEC is set to ON: AL4SAN uses PaRSEC runtime\n"
            "   To use AL4SAN with StarPU runtime: set AL4SAN_SCHED_STARPU to ON\n"
            "   To use AL4SAN with Quark  runtime: set AL4SAN_SCHED_QUARK  to ON\n"
            "   To use AL4SAN with OPENMP runtime: set AL4SAN_SCHED_OPENMP to ON\n"
            "   (AL4SAN_SCHED_PARSEC will be disabled)${ColourReset}")
endif()
if(AL4SAN_SCHED_OPENMP)
    message("-- ${BoldGreen}AL4SAN_SCHED_OPENMP is set to ON: AL4SAN uses OPENMP runtime\n"
            "   To use AL4SAN with StarPU runtime: set AL4SAN_SCHED_STARPU to ON\n"
            "   To use AL4SAN with Quark  runtime: set AL4SAN_SCHED_QUARK  to ON\n"
            "   To use AL4SAN with PARSEC runtime: set AL4SAN_SCHED_PARSEC to ON\n"
            "   (AL4SAN_SCHED_OPENMP will be disabled)${ColourReset}")
endif()

# Check that one, and only one, SCHED option is set to ON
# count number of runtime sets to ON
math(EXPR number_of_active_runtime 0)
foreach (runtime ${AL4SAN_SCHED_list})
    if (AL4SAN_SCHED_${runtime})
        math(EXPR number_of_active_runtime "${number_of_active_runtime}+1")
        set( AL4SAN_SCHED_name "${runtime}" )
    endif()
endforeach()
if (NOT number_of_active_runtime STREQUAL 1)
    message(
            "Number of active runtime is ${number_of_active_runtime}, "
            "")
endif()

# Use intermediate variable since cmake_dependent_option doesn't have OR conditions
set(AL4SAN_ENABLE_MPI OFF CACHE INTERNAL "Tells if MPI might be supported by the runtime")
if ( AL4SAN_SCHED_PARSEC OR AL4SAN_SCHED_STARPU )
     set(AL4SAN_ENABLE_MPI ON FORCE)
endif()

# Use intermediate variable since cmake_dependent_option doesn't have OR conditions
set(AL4SAN_ENABLE_CUDA OFF CACHE INTERNAL "Tells if CUDA might be supported by the runtime")
if ( AL4SAN_SCHED_PARSEC OR AL4SAN_SCHED_STARPU )
     set(AL4SAN_ENABLE_CUDA ON FORCE)
endif()

# Additional options
# ------------------

# Enable the distributed interface (allowed only when StarPU or PaRSEC is enabled)
# TODO: Default should be changed to ON/OFF when it will be ok
cmake_dependent_option(AL4SAN_USE_MPI
                    "Enable distributed memory through MPI" OFF
                    "AL4SAN_ENABLE_MPI" OFF)
if (AL4SAN_ENABLE_MPI AND NOT AL4SAN_USE_MPI)
    message("-- ${BoldGreen}AL4SAN_USE_MPI is set to OFF, turn it ON to use MPI (unsupported by Quark and OpenMP)${ColourReset}")
endif()

# Enable Cuda kernels if StarPU (only if StarPU or PaRSEC runtime is enabled)
# TODO: Default should be changed to ON/OFF when it will be ok
cmake_dependent_option(AL4SAN_USE_CUDA
                    "Enable CUDA kernels" OFF
                    "AL4SAN_ENABLE_CUDA" OFF)
if (AL4SAN_ENABLE_CUDA AND NOT AL4SAN_USE_CUDA)
    message("-- ${BoldGreen}AL4SAN_USE_CUDA is set to OFF, turn it ON to use CUDA (unsupported by Quark and OpenMP)${ColourReset}")
endif()

# Enable FXT if StarPU
option(AL4SAN_ENABLE_TRACING "Enable tracing support" OFF)
if (NOT AL4SAN_ENABLE_TRACING)
    message("-- ${BoldGreen}AL4SAN_ENABLE_TRACING is set to OFF, turn it ON to use FxT (with StarPU)${ColourReset}")
endif()
#option(AL4SAN_USE_EZTRACE "Enable EZTRACE to build modules" OFF)

# Options to enable/disable testings and timings
# ----------------------------------------------
option(AL4SAN_ENABLE_DOC      "Enable documentation build"  OFF)
if (AL4SAN_ENABLE_DOC)
    set( DOXYGEN_INPUT "" )
    message("-- ${BoldGreen}AL4SAN_ENABLE_DOC is set to ON, turn it OFF to avoid building docs${ColourReset}")
endif()

# Option to activate or not simulation mode (use Simgrid through StarPU)
# ----------------------------------------------------------------------
if(AL4SAN_SCHED_STARPU)
    option(AL4SAN_ENABLE_PRUNING_STATS "Enable pruning statistics" OFF)
    if (NOT AL4SAN_ENABLE_PRUNING_STATS)
        message("-- ${BoldGreen}AL4SAN_ENABLE_PRUNING_STATS is set to OFF, turn it ON to build pruning statistics${ColourReset}")
    endif()
    if(AL4SAN_ENABLE_PRUNING_STATS)
        add_definitions(-DAL4SAN_ENABLE_PRUNING_STATS)
    endif(AL4SAN_ENABLE_PRUNING_STATS)
endif()

# Initially we need to generate files for different precisions
# TODO: use this option to avoid generate multiple precisions each time we launch cmake
#option(AL4SAN_GEN_PREC "Generate source files precisions" ON)
#------------------------------------------------------------------------------


###############################################################################
# Look for dependencies #
#########################
set(AL4SAN_DEP "")

# Check for Thread library
# ------------------------
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
if( THREADS_FOUND AND CMAKE_THREAD_LIBS_INIT )
  libraries_absolute_path(CMAKE_THREAD_LIBS_INIT "")
  list(APPEND EXTRA_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
endif ()

# Add math library to the list of extra
# it normally exists on all common systems provided with a C compiler
if(UNIX OR WIN32)
    find_library(
      M_LIBRARY
      NAMES m
      )
    mark_as_advanced(M_LIBRARY)
    if (M_LIBRARY)
      list(APPEND EXTRA_LIBRARIES "${M_LIBRARY}")
    else()
      message(FATAL_ERROR "Could NOT find libm on your system."
        " Are you sure to a have a C compiler installed?")
    endif()
endif()

# Try to find librt (libposix4 - POSIX.1b Realtime Extensions library)
# on Unix systems except Apple ones because it does not exist on it
if(UNIX AND NOT APPLE)
    find_library(
        RT_LIBRARY
        NAMES rt
        )
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        list(APPEND EXTRA_LIBRARIES "${RT_LIBRARY}")
    else()
        message(FATAL_ERROR "Could NOT find librt on your system")
    endif()
endif()

   
# AL4SAN depends on CUDA/CUBLAS
#----------------------------------
if (AL4SAN_USE_CUDA)

    find_package(CUDA REQUIRED)
    if (CUDA_FOUND)
        message("-- ${Blue}Add definition AL4SAN_USE_CUDA"
        " - Activate CUDA in Al4san${ColourReset}")
        libraries_absolute_path(CUDA_LIBRARIES "$ENV{CUDA_ROOT}/lib64")
        set(EXTRA_LIBRARIES_CUDA)
        if(CUDA_VERSION VERSION_LESS "3.0")
            set(CUDA_HOST_COMPILATION_CPP OFF)
        endif(CUDA_VERSION VERSION_LESS "3.0")
        set(CUDA_BUILD_EMULATION OFF)
        if (CUDA_INCLUDE_DIRS)
            include_directories(${CUDA_INCLUDE_DIRS})
        else()
            message(WARNING "AL4SAN_USE_CUDA requires"
            "\n   CUDA_INCLUDE_DIRS to be found. Be sure you have"
            "\n   cuda headers with your distribution of CUDA.")
        endif()
        if (CUDA_LIBRARIES)
            set(saved_include "${CMAKE_REQUIRED_INCLUDES}")
            set(saved_libs "${CMAKE_REQUIRED_LIBRARIES}")
            set(CMAKE_REQUIRED_INCLUDES  "${CMAKE_REQUIRED_INCLUDES};${CUDA_INCLUDE_DIRS}")
            set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};${CUDA_LIBRARIES}")
            if(CUDA_VERSION VERSION_LESS "4.0")
                set(CUDA_HAVE_PEER_DEVICE_MEMORY_ACCESS 0)
            else()
                check_function_exists(cuDeviceCanAccessPeer CUDA_HAVE_PEER_DEVICE_MEMORY_ACCESS)
            endif()
            set(CMAKE_REQUIRED_INCLUDES  "${saved_include}")
            set(CMAKE_REQUIRED_LIBRARIES "${saved_libs}")
            # Add cublas if found
            if (CUDA_CUBLAS_LIBRARIES)
                message("-- ${Blue}Add definition AL4SAN_USE_CUBLAS"
                " - Use GPU kernels from cuBLAS${ColourReset}")
                list(APPEND EXTRA_LIBRARIES_CUDA ${CUDA_CUBLAS_LIBRARIES})
                list(APPEND EXTRA_LIBRARIES_CUDA ${CUDA_LIBRARIES}) # need CUDA libs to link.
            else()
                message(FATAL_ERROR "AL4SAN_USE_CUDA requires"
                "\n   CUDA_CUBLAS_LIBRARIES to be found. Be sure you have"
                "\n   libcublas with your distribution of CUDA.")
            endif()
        else()
            message(FATAL_ERROR "AL4SAN_USE_CUDA requires"
            "\n   CUDA_LIBRARIES to be found. Be sure you have"
            "\n   libcuda with your distribution of CUDA.")
        endif()
    endif (CUDA_FOUND)

    list(APPEND EXTRA_LIBRARIES ${EXTRA_LIBRARIES_CUDA})

endif(AL4SAN_USE_CUDA)

# AL4SAN depends on MPI
#-------------------------
if (AL4SAN_USE_MPI)

    # allows to use an external mpi compilation by setting compilers with
    # -DMPI_C_COMPILER=path/to/mpicc -DMPI_Fortran_COMPILER=path/to/mpif90
    # at cmake configure
    if(NOT MPI_C_COMPILER)
        set(MPI_C_COMPILER mpicc)
    endif()
    find_package(MPI REQUIRED)

    if (MPI_C_FOUND)
        message("-- ${Blue}Add definition AL4SAN_USE_MPI"
        " - Activate MPI in Al4san${ColourReset}")
        list(APPEND EXTRA_LIBRARIES ${MPI_C_LIBRARIES} )
        include_directories( ${MPI_C_INCLUDE_PATH} )
        # tests for intel mpi
        #list(APPEND MPI_C_COMPILE_FLAGS "-mt_mpi")
        #list(APPEND MPI_COMPILE_FLAGS "-mt_mpi")
        if(MPI_C_COMPILE_FLAGS)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_C_COMPILE_FLAGS}")
        endif()
        if(MPI_C_LINK_FLAGS)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_C_LINK_FLAGS}")
        endif()
    endif (MPI_C_FOUND)

endif (AL4SAN_USE_MPI)


# AL4SAN depends on a runtime
# -------------------------------
if( AL4SAN_SCHED_STARPU )

    set(AL4SAN_STARPU_VERSION "1.1" CACHE STRING "oldest STARPU version desired")

    # create list of components in order to make a single call to find_package(starpu...)
    set(STARPU_COMPONENT_LIST "HWLOC")
    if(AL4SAN_USE_CUDA)
        list(APPEND STARPU_COMPONENT_LIST "CUDA")
    endif()
    if(AL4SAN_USE_MPI)
        list(APPEND STARPU_COMPONENT_LIST "MPI")
    endif()
    if(AL4SAN_ENABLE_TRACING)
        list(APPEND STARPU_COMPONENT_LIST "FXT")
    endif()

    find_package(STARPU ${AL4SAN_STARPU_VERSION} REQUIRED
                 COMPONENTS ${STARPU_COMPONENT_LIST})

    # Add definition and include_dir if found
    if ( STARPU_FOUND )
        message("-- ${Blue}Add definition AL4SAN_SCHED_STARPU"
        " - Activate StarPU in Al4san${ColourReset}")
        if (STARPU_INCLUDE_DIRS_DEP)
            include_directories(${STARPU_INCLUDE_DIRS_DEP})
            set(CMAKE_REQUIRED_INCLUDES "${STARPU_INCLUDE_DIRS_DEP}")
        endif()
        if(STARPU_CFLAGS_OTHER_DEP)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${STARPU_CFLAGS_OTHER_DEP}")
            set(CMAKE_REQUIRED_FLAGS "${STARPU_CFLAGS_OTHER_DEP}")
        endif()
        if(STARPU_LDFLAGS_OTHER_DEP)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${STARPU_LDFLAGS_OTHER_DEP}")
            set(CMAKE_REQUIRED_LDFLAGS "${STARPU_LDFLAGS_OTHER_DEP}")
        endif()
        if(STARPU_LIBRARY_DIRS_DEP)
            list(APPEND CMAKE_INSTALL_RPATH "${STARPU_LIBRARY_DIRS_DEP}")
        endif()
        if (STARPU_LIBRARIES_DEP)
            list(INSERT AL4SAN_DEP 0 ${STARPU_LIBRARIES_DEP})
            set(CMAKE_REQUIRED_LIBRARIES "${STARPU_LIBRARIES_DEP}")
        endif()
        string(REPLACE ";" " " CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
        check_function_exists(starpu_data_idle_prefetch_on_node HAVE_STARPU_IDLE_PREFETCH)
        if ( HAVE_STARPU_IDLE_PREFETCH )
            message("-- ${Blue}Add definition HAVE_STARPU_IDLE_PREFETCH${ColourReset}")
        endif()
        check_function_exists(starpu_iteration_push HAVE_STARPU_ITERATION_PUSH)
        if ( HAVE_STARPU_ITERATION_PUSH )
            message("-- ${Blue}Add definition HAVE_STARPU_ITERATION_PUSH${ColourReset}")
        endif()
        check_function_exists(starpu_data_wont_use HAVE_STARPU_DATA_WONT_USE)
        if ( HAVE_STARPU_DATA_WONT_USE )
            message("-- ${Blue}Add definition HAVE_STARPU_DATA_WONT_USE${ColourReset}")
        endif()
        check_function_exists(starpu_data_set_coordinates HAVE_STARPU_DATA_SET_COORDINATES)
        if ( HAVE_STARPU_DATA_SET_COORDINATES )
            message("-- ${Blue}Add definition HAVE_STARPU_DATA_SET_COORDINATES${ColourReset}")
        endif()
        check_function_exists(starpu_malloc_on_node_set_default_flags HAVE_STARPU_MALLOC_ON_NODE_SET_DEFAULT_FLAGS)
        if ( HAVE_STARPU_MALLOC_ON_NODE_SET_DEFAULT_FLAGS )
            message("-- ${Blue}Add definition HAVE_STARPU_MALLOC_ON_NODE_SET_DEFAULT_FLAGS${ColourReset}")
        endif()
        check_function_exists(starpu_mpi_data_migrate HAVE_STARPU_MPI_DATA_MIGRATE)
        if ( HAVE_STARPU_MPI_DATA_MIGRATE )
            message("-- ${Blue}Add definition HAVE_STARPU_MPI_DATA_MIGRATE${ColourReset}")
        elseif(AL4SAN_USE_MIGRATE)
            set(AL4SAN_USE_MIGRATE "OFF")
            message("-- ${Blue}AL4SAN_USE_MIGRATE is turned OFF because starpu_mpi_data_migrate not found${ColourReset}")
        endif()
        if(AL4SAN_ENABLE_TRACING)
            # check if fxt profiling is accessible in starpu and activate it in al4san
            check_function_exists(starpu_fxt_start_profiling HAVE_STARPU_FXT_PROFILING)
            if ( HAVE_STARPU_FXT_PROFILING )
                message("-- ${Blue}Add definition HAVE_STARPU_FXT_PROFILING"
                " - Activate FxT profiling through StarPU${ColourReset}")
            else()
                message("-- ${Red}Looking for starpu with fxt"
                " - starpu_fxt_start_profiling() test fails in StarPU${ColourReset}")
                message("-- ${Red}Check in CMakeFiles/CMakeError.log to figure out why it fails${ColourReset}")
            endif()
        endif()
        if (AL4SAN_USE_MPI)
            # Check if a specific function exist
            check_function_exists(starpu_mpi_data_register_comm HAVE_STARPU_MPI_DATA_REGISTER)
            if ( HAVE_STARPU_MPI_DATA_REGISTER )
                message("-- ${Blue}Add definition HAVE_STARPU_MPI_DATA_REGISTER - Activate"
                " use of starpu_mpi_data_register() in Al4san with StarPU${ColourReset}")
            endif()
            check_function_exists(starpu_mpi_comm_rank HAVE_STARPU_MPI_COMM_RANK)
            if ( HAVE_STARPU_MPI_COMM_RANK )
                message("-- ${Blue}Add definition HAVE_STARPU_MPI_COMM_RANK - Activate"
                " use of starpu_mpi_comm_rank() in Al4san with StarPU${ColourReset}")
            endif()
            check_function_exists(starpu_mpi_cached_receive HAVE_STARPU_MPI_CACHED_RECEIVE)
            if ( HAVE_STARPU_MPI_CACHED_RECEIVE )
                message("-- ${Blue}Add definition HAVE_STARPU_MPI_CACHED_RECEIVE${ColourReset}")
            endif()
            check_function_exists(starpu_mpi_comm_get_attr HAVE_STARPU_MPI_COMM_GET_ATTR)
            if ( HAVE_STARPU_MPI_COMM_GET_ATTR )
                message("-- ${Blue}Add definition HAVE_STARPU_MPI_COMM_GET_ATTR${ColourReset}")
            endif()
        endif()
    else ( STARPU_FOUND )
        if(AL4SAN_VERBOSE_FIND_PACKAGE)
            if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
                if (NOT HWLOC_hwloc.h_DIRS)
                    Print_Find_Header_Status(hwloc hwloc.h)
                endif ()
                if (NOT HWLOC_hwloc_LIBRARY)
                    Print_Find_Library_Status(hwloc libhwloc)
                endif ()
            endif()
            if(AL4SAN_ENABLE_TRACING AND (NOT FXT_FOUND OR NOT FXT_LIBRARIES))
                if (NOT FXT_fxt.h_DIRS)
                    Print_Find_Header_Status(fxt fxt.h)
                endif ()
                if (NOT FXT_fxt_LIBRARY)
                    Print_Find_Library_Status(fxt libfxt)
                endif ()
            endif()
            if( (NOT STARPU_SHM_FOUND) OR (NOT STARPU_SHM_LIBRARIES) OR
                ( STARPU_LOOK_FOR_MPI AND (NOT STARPU_MPI_FOUND OR NOT STARPU_MPI_LIBRARIES) )
              )
                foreach(starpu_hdr ${STARPU_hdrs_to_find})
                    if (NOT STARPU_${starpu_hdr}_INCLUDE_DIRS)
                        Print_Find_Header_Status(starpu ${starpu_hdr})
                    endif ()
                endforeach()
                if(STARPU_VERSION_STRING)
                    foreach(starpu_lib ${STARPU_libs_to_find})
                        if (NOT STARPU_${starpu_lib}_LIBRARY)
                            Print_Find_Library_Status(starpu ${starpu_lib})
                        endif ()
                    endforeach()
                endif ()
            endif ()
        else(AL4SAN_VERBOSE_FIND_PACKAGE)
            message(WARNING "StarPU library has not been found and AL4SAN_VERBOSE_FIND_PACKAGE is set to OFF."
            " Try to activate AL4SAN_VERBOSE_FIND_PACKAGE option (-DAL4SAN_VERBOSE_FIND_PACKAGE=ON) to get some hints for the detection")
        endif(AL4SAN_VERBOSE_FIND_PACKAGE)
        if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
            message(FATAL_ERROR "hwloc library is required but has not been found")
        endif()
        if(AL4SAN_ENABLE_TRACING AND (NOT FXT_FOUND OR NOT FXT_LIBRARIES))
            message(FATAL_ERROR "FxT library is required but has not been found")
        endif()
        if( (NOT STARPU_SHM_FOUND) OR (NOT STARPU_SHM_LIBRARIES) OR
            ( STARPU_LOOK_FOR_MPI AND (NOT STARPU_MPI_FOUND OR NOT STARPU_MPI_LIBRARIES) )
          )
            message(FATAL_ERROR "StarPU library is required but has not been found")
        endif()
    endif ( STARPU_FOUND )

endif( AL4SAN_SCHED_STARPU )

if( AL4SAN_SCHED_PARSEC )

    # create list of components in order to make a single call to find_package(starpu...)
    set(PARSEC_COMPONENT_LIST "HWLOC")
    if(AL4SAN_USE_MPI)
        list(APPEND PARSEC_COMPONENT_LIST "MPI")
    endif()
    if(AL4SAN_USE_CUDA)
        list(APPEND PARSEC_COMPONENT_LIST "CUDA")
    endif()
    # TODO: Add a AL4SAN_WITH_PROFILING option that enables Fxt for StarPU, or PAPI for PaRSEC
    #if(AL4SAN_WITH_PROFILING)
    #    list(APPEND PARSEC_COMPONENT_LIST "PAPI")
    #endif()
    find_package(PARSEC COMPONENTS ${PARSEC_COMPONENT_LIST})
    if(PARSEC_FOUND)
        message("-- ${Blue}Add definition AL4SAN_SCHED_PARSEC"
        " - Activate PaRSEC in Al4san${ColourReset}")
      if (PARSEC_INCLUDE_DIRS_DEP)
          include_directories(${PARSEC_INCLUDE_DIRS_DEP})
      endif()
      if(PARSEC_CFLAGS_OTHER_DEP)
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PARSEC_CFLAGS_OTHER_DEP}")
      endif()
      if(PARSEC_LDFLAGS_OTHER_DEP)
          set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PARSEC_LDFLAGS_OTHER_DEP}")
      endif()
      if(PARSEC_LIBRARY_DIRS_DEP)
          list(APPEND CMAKE_INSTALL_RPATH "${PARSEC_LIBRARY_DIRS_DEP}")
      endif()
      if (PARSEC_LIBRARIES_DEP)
          list(INSERT AL4SAN_DEP 0 ${PARSEC_LIBRARIES_DEP})
      endif()
      string(REPLACE ";" " " CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
      # to test if symbols exist, e.g.
      # set(CMAKE_REQUIRED_INCLUDES "${PARSEC_INCLUDE_DIRS_DEP}")
      # set(CMAKE_REQUIRED_FLAGS "${PARSEC_CFLAGS_OTHER_DEP}")
      # set(CMAKE_REQUIRED_LDFLAGS "${PARSEC_LDFLAGS_OTHER_DEP}")
      # set(CMAKE_REQUIRED_LIBRARIES "${PARSEC_LIBRARIES_DEP}")
      # check_function_exists(parsec_init HAVE_PARSEC_INIT)
    else ( PARSEC_FOUND )
        if(AL4SAN_VERBOSE_FIND_PACKAGE)
            if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
                if (NOT HWLOC_hwloc.h_DIRS)
                    Print_Find_Header_Status(hwloc hwloc.h)
                endif ()
                if (NOT HWLOC_hwloc_LIBRARY)
                    Print_Find_Library_Status(hwloc libhwloc)
                endif ()
            endif()
            if( (NOT PARSEC_FOUND) OR (NOT PARSEC_LIBRARIES_DEP) )
              foreach(parsec_hdr ${PARSEC_hdrs_to_find})
                    if (NOT PARSEC_${parsec_hdr}_INCLUDE_DIRS)
                        Print_Find_Header_Status(parsec ${parsec_hdr})
                    endif ()
                endforeach()
                if(PARSEC_VERSION_STRING)
                    foreach(parsec_lib ${PARSEC_libs_to_find})
                        if (NOT PARSEC_${parsec_lib}_LIBRARY)
                            Print_Find_Library_Status(parsec ${parsec_lib})
                        endif ()
                    endforeach()
                endif()
            endif()
        else(AL4SAN_VERBOSE_FIND_PACKAGE)
            message(WARNING "PaRSEC library has not been found and AL4SAN_VERBOSE_FIND_PACKAGE is set to OFF."
            " Try to activate AL4SAN_VERBOSE_FIND_PACKAGE option (-DAL4SAN_VERBOSE_FIND_PACKAGE=ON) to get some hints for the detection")
        endif(AL4SAN_VERBOSE_FIND_PACKAGE)
        if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
            message(FATAL_ERROR "hwloc library is required but has not been found")
        endif()
        if( (NOT PARSEC_FOUND) OR (NOT PARSEC_LIBRARIES_DEP) )
            message(FATAL_ERROR "PaRSEC library is required but has not been found")
        endif()
    endif ( PARSEC_FOUND )

endif( AL4SAN_SCHED_PARSEC )

      # try to find quark runtime
if( AL4SAN_SCHED_QUARK )

    find_package(QUARK COMPONENTS HWLOC)

    # Add definition and include_dir if found
    if(QUARK_FOUND)
        message("-- ${Blue}Add definition AL4SAN_SCHED_QUARK"
        " - Activate QUARK in Al4san${ColourReset}")
        if (QUARK_INCLUDE_DIRS_DEP)
            include_directories(${QUARK_INCLUDE_DIRS_DEP})
        endif()
        if(QUARK_CFLAGS_OTHER_DEP)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${QUARK_CFLAGS_OTHER_DEP}")
        endif()
        if(QUARK_LDFLAGS_OTHER_DEP)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${QUARK_LDFLAGS_OTHER_DEP}")
        endif()
        if(QUARK_LIBRARY_DIRS_DEP)
            list(APPEND CMAKE_INSTALL_RPATH "${QUARK_LIBRARY_DIRS_DEP}")
        endif()
        if (QUARK_LIBRARIES_DEP)
            list(INSERT AL4SAN_DEP 0 ${QUARK_LIBRARIES_DEP})
        endif()
    else(QUARK_FOUND)
        if(AL4SAN_VERBOSE_FIND_PACKAGE)
            if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
                if (NOT HWLOC_hwloc.h_DIRS)
                    Print_Find_Header_Status(hwloc hwloc.h)
                endif ()
                if (NOT HWLOC_hwloc_LIBRARY)
                    Print_Find_Library_Status(hwloc libhwloc)
                endif ()
            endif()
            if (NOT QUARK_quark.h_DIRS)
                Print_Find_Header_Status(quark quark.h)
            endif ()
            if (NOT QUARK_quark_LIBRARY)
                Print_Find_Library_Status(quark libquark)
            endif ()
        else()
            message(WARNING "QUARK library has not been found and AL4SAN_VERBOSE_FIND_PACKAGE is set to OFF."
            " Try to activate AL4SAN_VERBOSE_FIND_PACKAGE option (-DAL4SAN_VERBOSE_FIND_PACKAGE=ON) to get some hints for the detection")
        endif()
        if(NOT HWLOC_FOUND OR NOT HWLOC_LIBRARIES)
            message(FATAL_ERROR "hwloc library is required but has not been found")
        endif()
        message(FATAL_ERROR "QUARK library is required but has not been found")
    endif(QUARK_FOUND)

endif()

if( AL4SAN_SCHED_OPENMP )
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp=libomp")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
    find_package(OpenMP)

    # Add definition and include_dir if found
    if(OPENMP_FOUND)
        message("-- ${Blue}Add definition AL4SAN_SCHED_OPENMP"
        " - Activate OPENMP in AL4san${ColourReset}")
        set(AL4SAN_SCHED_OPENMP 1)
    set(OPENMP_SCHED 1)
        include_directories(${OPENMP_INCLUDE_DIRS})
        if(OPENMP_LIBRARY_DIRS)
            # the RPATH to be used when installing
            list(APPEND CMAKE_INSTALL_RPATH "${OPENMP_LIBRARY_DIRS}")
        endif()
        if (OPENMP_LIBRARIES)
            if (OPENMP_LIBRARIES_DEP)
                list(INSERT AL4SAN_DEP 0 ${OPENMP_LIBRARIES_DEP})
            else()
                list(INSERT AL4SAN_DEP 0 ${OPENMP_LIBRARIES})
            endif()
        endif()
        if(OpenMP_C_FLAGS)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_FLAGS}")
        endif()
        if(OpenMP_CXX_FLAGS)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
        endif()
        if(OpenMP_Fortran_FLAGS)
            set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fopenmp")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
       endif()
    else(OPENMP_FOUND)
        message(FATAL_ERROR "OPENMP library is required but has not been found")
    endif(OPENMP_FOUND)
endif()

# getopt
check_include_files(getopt.h AL4SAN_HAVE_GETOPT_H)
if (AL4SAN_HAVE_GETOPT_H)
  check_function_exists(getopt_long AL4SAN_HAVE_GETOPT_LONG)
endif()

# Add option to exploit cublas API v2
# -----------------------------------
cmake_dependent_option(AL4SAN_USE_CUBLAS_V2
  "Enable cublas API v2" ON
  "AL4SAN_USE_CUDA;AL4SAN_SCHED_STARPU" OFF)

list(REMOVE_DUPLICATES CMAKE_C_FLAGS)
string(REPLACE ";" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
list(REMOVE_DUPLICATES CMAKE_EXE_LINKER_FLAGS)
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
# Fix a problem on Mac OS X when building shared libraries
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
endif()

# Save extra dependencies (all required links)
list(APPEND AL4SAN_DEP ${EXTRA_LIBRARIES})
# We remove duplicate dependencies ensuring that redundant libraries are placed last
list(REVERSE AL4SAN_DEP)
list(REMOVE_DUPLICATES AL4SAN_DEP)
list(REVERSE AL4SAN_DEP)

# we add al4san's own library
set(AL4SAN_LIBRARIES "al4san")

if (AL4SAN_SCHED_QUARK)
  list(APPEND AL4SAN_LIBRARIES al4san_quark)
endif()
if (AL4SAN_SCHED_PARSEC)
  list(APPEND AL4SAN_LIBRARIES al4san_parsec)
endif()
if (AL4SAN_SCHED_STARPU)
  list(APPEND AL4SAN_LIBRARIES al4san_starpu)
endif()
if (AL4SAN_SCHED_OPENMP)
  list(APPEND AL4SAN_LIBRARIES al4san_openmp)
endif()
#list(APPEND AL4SAN_LIBRARIES hqr)
set(AL4SAN_LIBRARIES_DEP ${AL4SAN_LIBRARIES} ${AL4SAN_DEP})

# Check for the subdirectories
# ----------------------------

set(AL4SAN_SOURCES_TARGETS "" CACHE INTERNAL "List of targets of sources")

# include headers
# ---------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)



###############################################################################
# Main library #
################
add_subdirectory(include)
add_subdirectory(control)
#add_subdirectory(compute)

###############################################################################
# Runtimes libraries #
######################
add_subdirectory(runtime)

###############################################################################
# Executables and Tests #
#########################
enable_testing()
include(CTest)

if( AL4SAN_ENABLE_EXAMPLE )
    find_package(BLASEXT)
    find_package(CHAMELEON COMPONENTS ${AL4SAN_SCHED_name})
    if( CHAMELEON_FOUND )
    include_directories( AFTER ${CHAMELEON_INCLUDE_DIRS_DEP})
    include_directories( AFTER ${CHAMELEON_DIR_FOUND}/include/coreblas)
    if(CHAMELEON_LINKER_FLAGS)
        list(APPEND CMAKE_EXE_LINKER_FLAGS "${CHAMELEON_LINKER_FLAGS}")
    endif()
    if(CHAMELEON_LIBRARY_DIRS)
        # the RPATH to be used when installing
        list(APPEND CMAKE_INSTALL_RPATH "${CHAMELEON_LIBRARY_DIRS}")
    endif()
    if (CHAMELEON_LIBRARIES)
        if (CHAMELEON_LIBRARIES_DEP)
            list(INSERT AL4SAN_DEP 0 ${CHAMELEON_LIBRARIES_DEP})
        else()
            list(INSERT AL4SAN_DEP 0 ${CHAMELEON_LIBRARIES})
        endif()
    endif()
endif()

    add_subdirectory( example )
endif()


# Necessary to compile executables (temporary)
# should use SystemDetection.cmake module?
string(REGEX MATCH ".*ifort$" _match_ifort ${CMAKE_Fortran_COMPILER})
if(_match_ifort)
    message(STATUS "Add -nofor_main to the Fortran linker (Intel compiler)")
    set(CMAKE_Fortran_FLAGS  "${CMAKE_Fortran_FLAGS} -nofor_main")
endif(_match_ifort)

#------------------------------------------------------------------------------

# Define a target which gathers all targets of sources
# useful for generating the doc without compiling the binaries (objects, libraries)
add_custom_target(al4san_all_sources ALL DEPENDS ${AL4SAN_SOURCES_TARGETS})

###############################################################################
# Documentation #
#################


if(AL4SAN_ENABLE_DOC)
    add_subdirectory(doc)
    #if (DOXYGEN_EXECUTABLE AND EMACS_COMPILER AND TEX_COMPILER)
    #    add_custom_target(doc ALL DEPENDS doxygen-out doc-html-users_guide doc-pdf-users_guide)
    #endif()
endif()
#------------------------------------------------------------------------------

###############################################################################
# Config files (pkg_config) #
#############################

# Create .pc file
# ---------------
generate_pkgconfig_file()
#------------------------------------------------------------------------------

###############################################################################
# Release source #
##################
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_PACKAGE_NAME "al4san")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Al4san is a C library providing parallel algorithms to perform BLAS/LAPACK "
                                      " operations exploiting fully modern architectures.")
set(AL4SAN_VERSION_PATCH ${AL4SAN_VERSION_MICRO})
set(CPACK_PACKAGE_VERSION "${AL4SAN_VERSION_MAJOR}.${AL4SAN_VERSION_MINOR}.${AL4SAN_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_VENDOR "Inria")
set(CPACK_PACKAGE_CONTACT "al4san-devel@lists.gforge.inria.fr")
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENCE.txt)
set(CPACK_SOURCE_IGNORE_FILES "plasma-conversion;build;.cproject;.settings;.dir-locals.el;.project;.pydevproject;.svn;.git;.gitmodules;.gitlab-ci.yml")
include(CPack)

###############################################################################
# Print Options #
#################
include(PrintOpts)

if (CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "C:/Program Files")
    message("-- ${Yellow}Your CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX} which is a default system path."
    " You may want to change it: set the CMAKE_INSTALL_PREFIX variable to do so${ColourReset}")
else()
    message("-- ${Yellow}Your CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}${ColourReset}")
endif()

###
### END CMakeLists.txt
###
