if(NOT DEFINED GUIDANCE_LIB_PATH)
	message(FATAL_ERROR "The guidance library path is not defined!")
endif()

if(NOT DEFINED GUIDANCE_SDK_LIB_PATH)
	message(FATAL_ERROR "The guidance sdk library path is not defined!")
endif()

add_definitions(-DWINVER=0x0601)
add_definitions(-D_WIN32_WINNT=0x0601)

set(OPENCV_LIBRARYDIR ${GUIDANCE_LIB_PATH}/OpenCV/2.4.9/win64/msvc120/)

find_library(Guidance_Lib DJI_guidance.lib PATHS ${GUIDANCE_SDK_LIB_PATH}/2013/x64)

