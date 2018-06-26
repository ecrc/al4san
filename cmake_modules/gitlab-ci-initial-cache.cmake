set(BUILD_SHARED_LIBS "ON" CACHE BOOL "")
set(CMAKE_INSTALL_PREFIX "$ENV{PWD}/install" CACHE PATH "")
set(CMAKE_VERBOSE_MAKEFILE "ON" CACHE BOOL "")

option(ALTANAL_ENABLE_WARNING       "Enable warning messages" ON)
option(ALTANAL_ENABLE_COVERAGE      "Enable flags for coverage test" ON)
