# - Try to find LibHQR
# Once done this will define
#  LIBHQR_FOUND - System has LibHQR
#  LIBHQR_INCLUDE_DIRS - The LibHQR include directories
#  LIBHQR_LIBRARIES - The libraries needed to use LibHQR
#  LIBHQR_DEFINITIONS - Compiler switches required for using LIBHQR

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_LIBHQR QUIET libhqr)
endif()

find_path(
  LIBHQR_INCLUDE_DIR
  libhqr.h
  HINTS  ${LIBHQR_DIR} ${PC_LIBHQR_INCLUDEDIR} ${PC_LIBHQR_INCLUDE_DIRS}
  PATH_SUFFIXES include include/libhqr
  )

find_library(
  LIBHQR_LIBRARY
  NAMES hqr
  HINTS ${LIBHQR_DIR} ${PC_LIBHQR_LIBDIR} ${PC_LIBHQR_LIBRARY_DIRS}
  PATH_SUFFIXES lib lib32 lib64 lib/libhqr lib32/libhqr lib64/libhqr
  )

set(LIBHQR_DIR "" CACHE PATH "Path where LIBHQR was installed")


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments
# and set LIBHQR_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  LIBHQR DEFAULT_MSG LIBHQR_LIBRARY LIBHQR_INCLUDE_DIR)

mark_as_advanced(LIBHQR_INCLUDE_DIR LIBHQR_LIBRARY )

set(LIBHQR_LIBRARIES ${LIBHQR_LIBRARY} )
set(LIBHQR_INCLUDE_DIRS ${LIBHQR_INCLUDE_DIR} )
set(LIBHQR_DEFINITIONS ${PC_LIBHQR_CFLAGS_OTHER} )
