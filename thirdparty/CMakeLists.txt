# -*- CMake -*-

#################################################################################
#
# MuJS Library
#

file(GLOB MUJS_SRC mujs/js*.c mujs/utf*.c mujs/regexp.c)
add_library(mujs SHARED ${MUJS_SRC})
install(TARGETS mujs LIBRARY DESTINATION lib)

#################################################################################
#
# JPEG library from IJG
#

set(JPEG_IGJ_SRC
  jaricom.c
  jcomapi.c
  jdapimin.c
  jdapistd.c
  jdarith.c
  jdatadst.c
  jdatasrc.c
  jdcoefct.c
  jdcolor.c
  jddctmgr.c
  jdhuff.c
  jdinput.c
  jdmainct.c
  jdmarker.c
  jdmaster.c
  jdmerge.c
  jdpostct.c
  jdsample.c
  jdtrans.c
  jerror.c
  jfdctflt.c
  jfdctfst.c
  jfdctint.c
  jidctflt.c
  jidctfst.c
  jidctint.c
  jmemmgr.c
  jquant1.c
  jquant2.c
  jutils.c
  )
prepend_path(JPEG_IGJ_SRC jpeg)

add_library(jpeg-ijg SHARED ${JPEG_IGJ_SRC})
target_compile_definitions(jpeg-ijg PRIVATE main=xxxmain)
install(TARGETS jpeg-ijg LIBRARY DESTINATION lib)

#################################################################################
#
# OPENJPEG library
#

# Fixme: libopenjpeg has already CMakeLists
set(OPENJPEG_SRC
  bio.c
  cidx_manager.c
  cio.c
  dwt.c
  event.c
  function_list.c
  image.c
  invert.c
  j2k.c
  jp2.c
  mct.c
  mqc.c
  openjpeg.c
  opj_clock.c
  phix_manager.c
  pi.c
  ppix_manager.c
  raw.c
  t1.c
  t2.c
  tcd.c
  tgt.c
  thix_manager.c
  tpix_manager.c
  )
prepend_path(OPENJPEG_SRC openjpeg/libopenjpeg)

add_library(openjpeg SHARED ${OPENJPEG_SRC})
target_compile_definitions(openjpeg PRIVATE OPJ_SHARED OPJ_HAVE_STDINT_H)
install(TARGETS openjpeg LIBRARY DESTINATION lib)

####################################################################################################
#
# End
#
####################################################################################################
