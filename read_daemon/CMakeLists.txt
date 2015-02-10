##################################################
#                epoc daemon                   #
##################################################

add_executable (eegd read_daemon.c)
ADD_DEPENDENCIES(eegd emokit)
target_link_libraries (eegd emokit ${LIBS} pthread)

SET_TARGET_PROPERTIES(eegd PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS eegd
	RUNTIME DESTINATION bin
)

