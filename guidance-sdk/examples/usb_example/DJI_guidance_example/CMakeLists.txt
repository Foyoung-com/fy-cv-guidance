add_executable( dji_guidance_usb main.cpp DJI_utility.cpp )
target_link_libraries(dji_guidance_usb ${GUIDANCE_EXAMPLES_REQUIRED_LIBS})
if(UNIX)
	if(NOT TARGET libusb)
		message(FATAL_ERROR "libusb library is required for unix")
	endif()
	target_link_libraries(dji_guidance_usb libusb "-Wl,-rpath,\$ORIGIN")
endif(UNIX)

add_custom_command(TARGET dji_guidance_usb POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		-DBINARY=$<TARGET_FILE:dji_guidance_usb>
		-DDIRS=${GUIDANCE_DYLIB_PATH}
		-DDESTINATION=$<TARGET_FILE_DIR:dji_guidance_usb>
		-P "${GUIDANCE_PLATFORMS_PATH}/common/CopyDeps.cmake")