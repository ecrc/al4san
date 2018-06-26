###
#
# @file CMakeLists.txt
#
# @copyright 2009-2014 The University of Tennessee and The University of
#                      Tennessee Research Foundation. All rights reserved.
# @copyright 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
#                      Univ. Bordeaux. All rights reserved.
# @copyright 2017-2018 King Abdullah University of Science and Technology (KAUST). 
#                      All rights reserved.
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
#  @date 2012-07-13
#
###
###
#
#  @project ALTANAL
#  ALTANAL is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
# @version 0.1.0
#  @author Rabab Alomairy
#  @date 2018-05-19
#
###

# Check for the subdirectories
# ----------------------------
if( ALTANAL_SCHED_QUARK )
    add_subdirectory(quark)
elseif( ALTANAL_SCHED_PARSEC )
    add_subdirectory(parsec)
elseif( ALTANAL_SCHED_STARPU )
    add_subdirectory(starpu)
endif()

###
### END CMakeLists.txt
###
