##################################################
#                epoc daemon                   #
##################################################

add_executable (eegr read.c)
ADD_DEPENDENCIES(eegr emokit)
target_link_libraries (eegr emokit ${LIBS} pthread)

SET_TARGET_PROPERTIES(eegr PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS eegr
	RUNTIME DESTINATION bin
)

