add_executable( dji_guidance_usb main.cpp DJI_utility.cpp )
target_link_libraries(dji_guidance_usb ${GUIDANCE_EXAMPLES_REQUIRED_LIBS})