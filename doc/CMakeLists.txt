#
# @copyright 2018 King Abdullah University of Science and Technology (KAUST).
#                     All rights reserved.
find_package( Doxygen )
if( DOXYGEN_FOUND )
    # Collect Doxygen enabled sections
    #set(DOXYGEN_ENABLED_SECTIONS OPENMP MPI STARPU GSL)

    # Make DOXYGEN_INPUT appropriate for config file
    string( REPLACE ";" "\"\\\n    \"" DOXYGEN_INPUT "${DOXYGEN_INPUT}" )

    configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/config" )
    add_custom_target( docs COMMAND
                       "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/config" )
endif()
