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
#  @version 1.1.0
#  @author Eduardo Gonzalez
#  @author Rabab Alomairy
#
#  @date 2019-02-06
#
###

set( AL4SAN_EXAMPLE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/potrf.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/codelets/codelet_dgemm.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/codelets/codelet_dpotrf.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/codelets/codelet_dsyrk.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/codelets/codelet_dtrsm.c )
include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

string(TOLOWER ${AL4SAN_SCHED_name} _runtime)
if( NOT AL4SAN_SCHED_STARPU )
    configure_file(${CMAKE_SOURCE_DIR}/runtime/${_runtime}/include/al4san_runtime.h ${CMAKE_CURRENT_BINARY_DIR}/runtime/al4san_runtime.h COPYONLY)
else()
    configure_file( ${CMAKE_BINARY_DIR}/runtime/starpu/include/al4san_runtime.h ${CMAKE_CURRENT_BINARY_DIR}/runtime/al4san_runtime.h COPYONLY )
endif()
configure_file(${CMAKE_SOURCE_DIR}/runtime/${_runtime}/include/runtime_task.h ${CMAKE_CURRENT_BINARY_DIR}/runtime/runtime_task.h COPYONLY)
include_directories( ${CMAKE_BINARY_DIR}/${_runtime} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/runtime/${_runtime}/include )

link_directories( ${CHAMELEON_LIBRARY_DIRS} )

add_executable( potrf ${AL4SAN_EXAMPLE_SRC} )
target_link_libraries( potrf al4san ${AL4SAN_DEP} ${BLAS_LIBRARIES} )
add_test( potrf-${_runtime}-success ./potrf --n=10240 --nrhs=10240 --nb=128 --threads=${NUMBER_OF_CPU} )
#add_test( potrf-${_runtime}-nocheck ./potrf 4000 101 0 0 )
#add_test( potrf-${_runtime}-unsuccess ./potrf 4000 123 1 0 )
#set_tests_properties( potrf-${_runtime}-unsuccess PROPERTIES WILL_FAIL TRUE)

# Put doxygen input to parent scope
set(DOXYGEN_INPUT ${DOXYGEN_INPUT} ${AL4SAN_EXAMPLE_SRC} PARENT_SCOPE )



