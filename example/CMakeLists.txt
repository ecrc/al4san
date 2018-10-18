#####
 #
 #
 #
 # @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 #                     All rights reserved.
 #####
###
#
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
#  @version 1.0.0
#  @author Eduardo Gonzalez
#  @author Rabab Alomairy
#
#  @date 2018-10-18
#
###

find_package(BLASEXT)
if( BLA_VENDOR MATCHES "Intel" )
    set( AL4SAN_EXAMPLE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/cholesky.c )
    include_directories( ${CMAKE_SOURCE_DIR}/include )
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

    if ( AL4SAN_SCHED_QUARK )
        set( _runtime "quark" )
    elseif( AL4SAN_SCHED_OPENMP )
        set( _runtime "openmp" )
    endif()
    configure_file(${CMAKE_SOURCE_DIR}/runtime/${_runtime}/include/al4san_runtime.h ${CMAKE_CURRENT_BINARY_DIR}/runtime/al4san_runtime.h COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/runtime/${_runtime}/include/runtime_task.h ${CMAKE_CURRENT_BINARY_DIR}/runtime/runtime_task.h COPYONLY)
    include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

    add_executable( cholesky ${AL4SAN_EXAMPLE_SRC} )
    target_link_libraries( cholesky al4san ${BLAS_LIBRARIES} )
    add_test( cholesky-${_runtime}-success ./cholesky 4000 100 1 0 )
    add_test( cholesky-${_runtime}-nocheck ./cholesky 4000 101 0 0 )
    add_test( cholesky-${_runtime}-unsuccess ./cholesky 4000 123 1 0 )
    set_tests_properties( cholesky-${_runtime}-unsuccess PROPERTIES WILL_FAIL TRUE)

    # Put doxygen input to parent scope
    set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_EXAMPLE_SRC} PARENT_SCOPE )
else()
    message(WARNING "AL4SAN examples require MKL library")
endif()


