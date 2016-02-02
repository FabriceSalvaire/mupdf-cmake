# -*- CMake -*-

####################################################################################################
#
# Main CMakeLists.txt to build the MuPDF project using CMake
# Written by Fabrice Salvaire on nov 2013 and updated on jan 216
#
####################################################################################################

# Todo: check install, v8 and generated ...

####################################################################################################
#
# Customizable settings
#

# -D <var>:<type>=<value>: Create a cmake cache entry.

# Install path prefix, prepended onto install directories.
# -D CMAKE_INSTALL_PREFIX:PATH=/usr/local/

# Choose the type of build, options are: None(CMAKE_CXX_FLAGS or
# CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.
# -D CMAKE_BUILD_TYPE:STRING=Debug

# If this value is on, makefiles will be generated without the
# .SILENT directive, and all commands will be echoed to the console
# during the make.  This is useful for debugging only.
# -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON

####################################################################################################

project(mupdf C)

set(MUPDF_VERSION_MAJOR 1)
set(MUPDF_VERSION_MINOR 8)
set(MUPDF_VERSION_PATCH 0) # right name?
set(MUPDF_VERSION ${MUPDF_VERSION_MAJOR}.${MUPDF_VERSION_MINOR}.${MUPDF_VERSION_PATCH})

####################################################################################################
#
# CMake Configuration
#

# check cmake version
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(POLICY CMP0990)
  cmake_policy(SET CMP0990 NEW)
endif()

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

####################################################################################################
#
# Macros
#

MACRO(prepend_path _sources _path)
  unset(_tmp)
  foreach(src ${${_sources}})
    list(APPEND _tmp ${_path}/${src})
  endforeach(src)
  set(${_sources} ${_tmp})
ENDMACRO(prepend_path)

####################################################################################################
#
# Options
#

SET(CMAKE_BUILD_TYPE RELEASE)
#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

####################################################################################################
#
# Compiler Options
#

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_CXX_FLAGS_COMMON "-DSystemLinux -Wall -pipe")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_COMMON} -g -DDEBUG")

  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_COMMON} -O2 -DNDEBUG -pg")
#? set(CMAKE_LD_FLAGS_PROFILE "-pg")

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_COMMON} -O2 -DNDEBUG -fomit-frame-pointer")

  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COMMON} -g -DDEBUG -pg -fprofile-arcs -ftest-coverage")
#? set(CMAKE_LD_FLAGS_COVERAGE "-lgcov")

  set(CMAKE_CXX_FLAGS_NATIVE "${CMAKE_CXX_FLAGS_COMMON} -O2 -DNDEBUG -fomit-frame-pointer -march=native -mfpmath=sse")

  set(CMAKE_CXX_FLAGS_MEMENTO "${CMAKE_CXX_FLAGS_COMMON} -g -DMEMENTO -DDEBUG")
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

####################################################################################################
#
# Find Libraries
#

find_package(Freetype)
find_package(JBIG2Dec)
find_package(X11)
find_package(CURL)
find_package(OpenSSL)
find_package(V8)
find_library(LibZ z)

# We build against third party
# find_package(JPEG)
# find_package(OpenJPEG2)

# libjpeg.so.62 => /lib64/libjpeg.so.62 (0x00007fd65f5c1000)
# libopenjp2.so.7 => /lib64/libopenjp2.so.7 (0x00007fd65f38c000)

####################################################################################################
#
# Include Directories
#

include_directories(
  ${FREETYPE_INCLUDE_DIRS}
  ${JBIG2DEC_INCLUDE_DIR}
  ${X11_X11_INCLUDE_PATH}
  ${CURL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

include_directories(
  thirdparty/jpeg
  thirdparty/openjpeg/libopenjpeg
  )

include_directories(
  thirdparty/mujs
  )

####################################################################################################
#
# Third party Libraries
#

add_subdirectory(thirdparty)

set(MUPDF_LINK_LOCAL_LIBRARIES jpeg-ijg openjpeg mujs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

####################################################################################################
#
# Sub directories
#

add_subdirectory(scripts)
add_subdirectory(source)
add_subdirectory(platform/x11)

####################################################################################################
#
# Install
#

install(DIRECTORY include/mupdf
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  )

install(FILES
  README
  COPYING
  CHANGES
  DESTINATION share/doc/mupdf
  )

install(DIRECTORY docs
  DESTINATION share/doc/mupdf
  FILES_MATCHING PATTERN "*.txt"
  )

####################################################################################################
#
# End
#
####################################################################################################
