#################################################################################
#
# Executables rules
#

set(MUPDF_X11_SRC x11_main.c x11_image.c pdfapp.c)
set(MUPDF_X11_LIBRARIES mupdf-js-v8 ${X11_X11_LIB} ${X11_Xext_LIB})

if(X11_FOUND)
add_executable(mupdf-x11 ${MUPDF_X11_SRC})
target_link_libraries(mupdf-x11 ${MUPDF_X11_LIBRARIES})
install(TARGETS mupdf-x11 RUNTIME DESTINATION bin)

if(CURL_FOUND)
add_executable(mupdf-x11-curl ${MUPDF_X11_SRC})
target_link_libraries(mupdf-x11-curl ${MUPDF_X11_LIBRARIES} ${CURL_LIBRARY})
install(TARGETS mupdf-x11-curl RUNTIME DESTINATION bin)
endif(CURL_FOUND)

if(V8_FOUND)
add_executable(mupdf-x11-v8 ${MUPDF_X11_SRC})
target_link_libraries(mupdf-x11-v8 ${MUPDF_X11_LIBRARIES} ${V8_LIBRARY})
install(TARGETS mupdf-x11-v8 RUNTIME DESTINATION bin)
endif(V8_FOUND)
endif(X11_FOUND)

if(V8_FOUND)
add_executable(mujstest-v8 jstest_main.c pdfapp.c)
target_link_libraries(mujstest-v8 mupdf-js-v8 ${V8_LIBRARY})
install(TARGETS mujstest-v8 RUNTIME DESTINATION bin)
endif(V8_FOUND)

#################################################################################
#
# End
#
#################################################################################
