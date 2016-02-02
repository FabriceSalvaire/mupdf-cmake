# -*- CMake -*-

####################################################################################################
#
# Generated CMAP, FONT and JAVASCRIPT files
#

set(cmapdump "${CMAKE_BINARY_DIR}/scripts/cmapdump")
set(fontdump "${CMAKE_BINARY_DIR}/scripts/fontdump")
set(cquote "${CMAKE_BINARY_DIR}/scripts/cquote")
set(bin2hex "${CMAKE_BINARY_DIR}/scripts/bin2hex")

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
include_directories(${GENERATED_DIR})

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_cmap_cns.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${cmapdump}
  ${GENERATED_DIR}/gen_cmap_cns.h
  ${RESOURCES_DIR}/cmaps/cns/*
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_cmap_gb.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${cmapdump}
  ${GENERATED_DIR}/gen_cmap_gb.h
  ${RESOURCES_DIR}/cmaps/gb/*
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_cmap_japan.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${cmapdump}
  ${GENERATED_DIR}/gen_cmap_japan.h
  ${RESOURCES_DIR}/cmaps/japan/*
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_cmap_korea.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${cmapdump}
  ${GENERATED_DIR}/gen_cmap_korea.h
  ${RESOURCES_DIR}/cmaps/korea/*
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_font_base14.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${fontdump}
  ${GENERATED_DIR}/gen_font_base14.h
  ${RESOURCES_DIR}/fonts/urw/*.cff
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_font_cjk.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${fontdump}
  ${GENERATED_DIR}/gen_font_cjk.h
  ${RESOURCES_DIR}/fonts/droid/DroidSansFallback.ttc
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_font_cjk_full.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${fontdump}
  ${GENERATED_DIR}/gen_font_cjk_full.h
  ${RESOURCES_DIR}/fonts/droid/DroidSansFallbackFull.ttc
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_js_util.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${cquote}
  ${GENERATED_DIR}/gen_js_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/pdf/js/pdf-util.js
  )

add_custom_command(OUTPUT ${GENERATED_DIR}/gen_adobe_ca.h
  COMMAND mkdir -p ${GENERATED_DIR}
  COMMAND ${bin2hex}
  ${GENERATED_DIR}/gen_adobe_ca.h
  ${RESOURCES_DIR}/certs/AdobeCA.p7c
  )

add_custom_target(generated_cmap DEPENDS
  ${GENERATED_DIR}/gen_cmap_cns.h
  ${GENERATED_DIR}/gen_cmap_gb.h
  ${GENERATED_DIR}/gen_cmap_japan.h
  ${GENERATED_DIR}/gen_cmap_korea.h
  )

add_custom_target(generated_font DEPENDS
  ${GENERATED_DIR}/gen_font_base14.h
  ${GENERATED_DIR}/gen_font_cjk.h
  ${GENERATED_DIR}/gen_font_cjk_full.h
  )

add_custom_target(generated_js_util DEPENDS
  ${GENERATED_DIR}/gen_js_util.h
  )

# pdf/pdf-cmap-table.o : $(CMAP_GEN)
# pdf/pdf-fontfile.o : $(FONT_GEN)
# pdf/pdf-pkcs7.o : $(ADOBECA_GEN)
# pdf/js/pdf-js.o : $(JAVASCRIPT_GEN)
# cmapdump.o : source/pdf/pdf-cmap.c source/pdf/pdf-cmap-parse.c

#################################################################################
#
# MuPDF Library
#

file(GLOB CBZ_SRC cbz/*.c)
file(GLOB FITZ_SRC fitz/*.c)
file(GLOB GPRF_SRC gprf/*.c)
file(GLOB HTML_SRC html/*.c)
file(GLOB IMG_SRC img/*.c)
file(GLOB PDF_SRC pdf/*.c) # pdf-name-table.h
file(GLOB XPS_SRC xps/*.c)

set(JS_NONE_SRC pdf/js/pdf-js-none.c)
set(JS_SRC pdf/js/pdf-js.c)

set(MUPDF_SRC
  ${CBZ_SRC}
  ${FITZ_SRC}
  ${GPRF_SRC}
  ${HTML_SRC}
  ${IMG_SRC}
  ${PDF_SRC}
  ${XPS_SRC}
  )

set(MUPDF_LINK_LIBRARIES
  ${FREETYPE_LIBRARY}
  ${JPEG_LIBRARY}
  ${OPENJPEG2_LIBRARY}
  ${JBIG2DEC_LIBRARY}
  ${LibZ}
  )

# Use local jpeg library
include_directories(${CMAKE_SOURCE_DIR}/scripts/jpeg ${CMAKE_SOURCE_DIR}/thirdparty/jpeg)
include_directories(${CMAKE_SOURCE_DIR}/scripts/openjpeg ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg)
# else
# add_definitions(-DSHARE_JPEG)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/mujs)

add_library(mupdf SHARED ${MUPDF_SRC} ${JS_NONE_SRC})
add_dependencies(mupdf generated_cmap generated_font)
target_link_libraries(mupdf
  ${MUPDF_LINK_LIBRARIES}
  ${MUPDF_LINK_LOCAL_LIBRARIES}
  )
install(TARGETS mupdf LIBRARY DESTINATION lib)

if(V8_FOUND)
add_library(mupdf-js-v8 SHARED ${MUPDF_SRC} ${JS_SRC})
add_dependencies(mupdf-js-v8 generated_cmap generated_font generated_js_util)
target_link_libraries(mupdf-js-v8
  ${MUPDF_LINK_LIBRARIES}
  ${MUPDF_LINK_LOCAL_LIBRARIES}
  )
install(TARGETS mupdf-js-v8 LIBRARY DESTINATION lib)
endif(V8_FOUND)

####################################################################################################
#
# Sub directories
#

add_subdirectory(tools)

####################################################################################################
#
# End
#
####################################################################################################
