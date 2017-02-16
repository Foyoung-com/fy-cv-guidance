add_executable( sdk_tracking_camshift camshiftDepth.cpp DJI_utility.cpp )
target_link_libraries(sdk_tracking_camshift ${GUIDANCE_EXAMPLES_REQUIRED_LIBS})
if(UNIX)
	if(NOT TARGET libusb)
		message(FATAL_ERROR "libusb library is required for unix")
	endif()
	target_link_libraries(sdk_tracking_camshift libusb "-Wl,-rpath,\$ORIGIN")
endif(UNIX)

add_custom_command(TARGET sdk_tracking_camshift POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		-DBINARY=$<TARGET_FILE:sdk_tracking_camshift>
		-DDIRS=${GUIDANCE_DYLIB_PATH}
		-DDESTINATION=$<TARGET_FILE_DIR:sdk_tracking_camshift>
		-P "${GUIDANCE_PLATFORMS_PATH}/common/CopyDeps.cmake")