if(NOT DEFINED GUIDANCE_LIB_PATH)
    message(FATAL_ERROR "The guidance library path is not defined!")
endif()

if(NOT DEFINED GUIDANCE_SDK_SO_PATH)
    message(FATAL_ERROR "The guidance sdk shared library path is not defined!")
endif()


set(OPENCV_LIBRARYDIR ${GUIDANCE_LIB_PATH}/OpenCV/2.4.9/linux64/)
set(LIBUSB_LIBRARYDIR ${GUIDANCE_LIB_PATH}/libusb/1.0.9/linux64/)

add_library(libusb STATIC IMPORTED)
set_property(TARGET libusb PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LIBUSB_LIBRARYDIR}/include/libusb-1.0")
set_target_properties(libusb PROPERTIES IMPORTED_LOCATION "${LIBUSB_LIBRARYDIR}/lib/libusb-1.0.a")

find_library(Guidance_Lib DJI_guidance PATHS "${GUIDANCE_SDK_SO_PATH}/x64")

