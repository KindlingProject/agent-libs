set(INITSCRIPT_PACKAGES "agent;agent-slim;agentone;agentino")

foreach(INITSCRIPT_PACKAGE ${INITSCRIPT_PACKAGES})
	set(PACKAGE_ROOT "_CPack_Packages/${CPACK_TOPLEVEL_TAG}/${CPACK_GENERATOR}/${CPACK_PACKAGE_FILE_NAME}/${INITSCRIPT_PACKAGE}")
	if(CPACK_GENERATOR MATCHES "DEB")
		list(APPEND CPACK_INSTALL_COMMANDS "mkdir -p ${PACKAGE_ROOT}/etc/init.d/")
		list(APPEND CPACK_INSTALL_COMMANDS "cp scripts/debian/dragent ${PACKAGE_ROOT}/etc/init.d")
	endif()

	if(CPACK_GENERATOR MATCHES "RPM")
		list(APPEND CPACK_INSTALL_COMMANDS "mkdir -p ${PACKAGE_ROOT}/etc/rc.d/init.d/")
		list(APPEND CPACK_INSTALL_COMMANDS "cp scripts/rpm/dragent ${PACKAGE_ROOT}/etc/rc.d/init.d")

		list(APPEND CPACK_INSTALL_COMMANDS "mkdir -p ${PACKAGE_ROOT}/${CPACK_DRAIOS_INSTALL_PREFIX}/bin")
		list(APPEND CPACK_INSTALL_COMMANDS "cp scripts/rpm/dragent.in ${PACKAGE_ROOT}/${CPACK_DRAIOS_INSTALL_PREFIX}/bin")
	endif()
endforeach()

if(CPACK_GENERATOR MATCHES "TGZ")
	set(CPACK_SET_DESTDIR "ON")
	set(CPACK_STRIP_FILES "OFF")
endif()
