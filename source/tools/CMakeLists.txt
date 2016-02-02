# -*- CMake -*-

####################################################################################################
#
# Tools
#

# add_executable(mudraw mudraw.c)
# target_link_libraries(mudraw mupdf-js-v8)

add_executable(mutool
  mutool.c
  mudraw.c
  pdfclean.c
  pdfextract.c
  pdfinfo.c
  pdfpages.c
  pdfposter.c
  pdfshow.c
  )
target_link_libraries(mutool mupdf-js-v8)

# add_executable(mjsgen mjsgen.c)
# target_link_libraries(mjsgen mupdf)

install(TARGETS
  mutool
  # mudraw
  # mjsgen
  RUNTIME DESTINATION bin
  )

####################################################################################################
#
# End
#
####################################################################################################
