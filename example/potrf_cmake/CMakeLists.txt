cmake_minimum_required(VERSION 3.13)
# Defines the source files of the tests. Each file generates 1 test
include_directories( ${CMAKE_SOURCE_DIR} )
include_directories(../../build-parsec/install/include)
include_directories(/project/k1205/omairyrm/dplasma-parsec/parsec/build/install_dir/include/)
set(SRC
   ${CMAKE_SOURCE_DIR}/codelets/codelet_dtrsm.c
   ${CMAKE_SOURCE_DIR}/codelets/codelet_dpotrf.c
   ${CMAKE_SOURCE_DIR}/codelets/codelet_dgemm.c
   ${CMAKE_SOURCE_DIR}/codelets/codelet_dsyrk.c
   ${CMAKE_SOURCE_DIR}/codelets/codelet_dplgsy.c
   ${CMAKE_SOURCE_DIR}/coreblas/core_dplgsy.c
   ${CMAKE_SOURCE_DIR}/compute/dplgsy.c
   ${CMAKE_SOURCE_DIR}/compute/pdplgsy.c
   ${CMAKE_SOURCE_DIR}/potrf.c
)
add_library( potrf_lib ${SRC})
#file(GLOB tests_files "src/*.cpp")

add_definitions(-DENABLE_FINE_TIMING)
#add_definitions(-DENABLE_DEBUGGING)
# Uses RUNPATH instead of RPATH
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--enable-new-dtags")
list( APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-new-dtags")
# Transform the list into a space separate string
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set( TIMINGS_SRC 
   ${CMAKE_SOURCE_DIR}/potrf.c
   )
foreach( test_src ${TIMINGS_SRC} )
    get_filename_component( test_exe ${test_src}  NAME_WE )
    if( "${test_exe}" STREQUAL "timing" )
        # skip timing.c
        continue()
    endif()

    add_executable( ${test_exe}  ${test_src} )
    if( _build_netlib-scalapack )
        add_dependencies( ${test_exe} NETLIB-SCALAPACK )
    endif()

    target_link_libraries( ${test_exe} potrf_lib
                                       ${MPI_C_LIBRARIES}
                                       ${PARSEC_LIBRARIES}
                                       ${LAPACKE_LIBRARIES}
                                       ${LAPACK_LIBRARIES}
                                       ${BLAS_LIBRARIES}
                                       ${EXTRA_LIBS}
                                       ${AL4SAN_LIBS} 
					)
    install( TARGETS ${test_exe} DESTINATION lib/timing )
endforeach()
