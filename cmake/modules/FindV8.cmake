####################################################################################################
#
# - Find V8
# Find the native V8 includes and library
# This module defines
#  V8_INCLUDE_DIR, where to find v8.h.
#  V8_LIBRARIES, the libraries needed to use V8.
#  V8_FOUND, If false, do not try to use V8.
# also defined, but not for general use are
#  V8_LIBRARY, where to find the V8 library.
#
# Written by Fabrice Salvaire
#
####################################################################################################

find_path(V8_INCLUDE_DIR v8.h)

set(V8_NAMES ${V8_NAMES} v8)
find_library(V8_LIBRARY NAMES ${V8_NAMES})

# handle the QUIETLY and REQUIRED arguments and set V8_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(V8 DEFAULT_MSG V8_LIBRARY V8_INCLUDE_DIR)

if(V8_FOUND)
  set(V8_LIBRARIES ${V8_LIBRARY})
endif()

mark_as_advanced(V8_LIBRARY V8_INCLUDE_DIR)

####################################################################################################
#
# End
#
####################################################################################################

