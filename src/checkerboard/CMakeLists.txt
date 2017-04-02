get_filename_component(CHECKER_BOARD_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

include_directories(${CHECKER_BOARD_PATH})

add_executable( checkerboard ${CHECKER_BOARD_PATH}/checker_board.cpp ${CHECKER_BOARD_PATH}/conf_serializer.cpp )
target_link_libraries(checkerboard ${GUIDANCE_REQUIRED_LIBS} fybase)
if(UNIX)
    if(NOT TARGET libusb)
        message(FATAL_ERROR "libusb library is required for unix")
    endif()
    target_link_libraries(checkerboard libusb "-Wl,-rpath,\$ORIGIN")
endif(UNIX)

add_custom_command(TARGET checkerboard POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -DBINARY=$<TARGET_FILE:checkerboard>
                -DDIRS=${GUIDANCE_DYLIB_PATH}
                -DDESTINATION=$<TARGET_FILE_DIR:checkerboard>
                -P "${GUIDANCE_PLATFORMS_PATH}/common/CopyDeps.cmake")
                                                                                                                                                                      
                                                                           
