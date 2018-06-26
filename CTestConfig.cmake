## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)
set(CTEST_PROJECT_NAME "Altanal")
set(CTEST_NIGHTLY_START_TIME "00:00:00 GMT")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "cdash.inria.fr")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=altanal")
set(CTEST_DROP_SITE_CDASH TRUE)

#--------------------------------------------------------------------
# BUILDNAME variable construction
# This variable will be used to set the build name which will appear
# on the Altanal dashboard http://cdash.inria.fr/CDash/
#--------------------------------------------------------------------
# Start with the short system name, e.g. "Linux", "FreeBSD" or "Windows"
if(NOT BUILDNAME)

  set(BUILDNAME "${CMAKE_SYSTEM_NAME}")

  # Add i386 or amd64
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BUILDNAME "${BUILDNAME}-amd64")
  else()
    set(BUILDNAME "${BUILDNAME}-i386")
  endif()

  # Add compiler name
  get_filename_component(CMAKE_C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME)
  set(BUILDNAME "${BUILDNAME}-${CMAKE_C_COMPILER_NAME}")

  # Add the build type, e.g. "Debug, Release..."
  if(CMAKE_BUILD_TYPE)
    set(BUILDNAME "${BUILDNAME}-${CMAKE_BUILD_TYPE}")
  endif(CMAKE_BUILD_TYPE)

  # Specific options of Altanal
  if(ALTANAL_SCHED_QUARK)
    set(BUILDNAME "${BUILDNAME}-Quark")
  endif(ALTANAL_SCHED_QUARK)

  if(ALTANAL_SCHED_STARPU)
    set(BUILDNAME "${BUILDNAME}-StarPU")
  endif(ALTANAL_SCHED_STARPU)

   if(ALTANAL_SCHED_PARSEC)
    set(BUILDNAME "${BUILDNAME}-ParSEC")
  endif(ALTANAL_SCHED_PARSEC)

  if(ALTANAL_SIMULATION)
    set(BUILDNAME "${BUILDNAME}-SimGrid")
  endif(ALTANAL_SIMULATION)

  if(ALTANAL_USE_MPI)
    set(BUILDNAME "${BUILDNAME}-MPI")
  endif(ALTANAL_USE_MPI)

  if(ALTANAL_USE_CUDA)
    set(BUILDNAME "${BUILDNAME}-CUDA")
  endif(ALTANAL_USE_CUDA)

endif()
