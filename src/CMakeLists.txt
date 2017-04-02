cmake_minimum_required(VERSION 3.2)

project( fy_guidance )

set(CMAKE_CXX_STANDARD 11)

#==================================================================
#project scope PATHs, setup first to allow other files to reference
get_filename_component(GUIDANCE_SRC_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(GUIDANCE_ROOT_PATH "${GUIDANCE_SRC_PATH}" DIRECTORY)
get_filename_component(GUIDANCE_INC_PATH "${GUIDANCE_ROOT_PATH}/include" ABSOLUTE)
get_filename_component(GUIDANCE_LIB_PATH "${GUIDANCE_ROOT_PATH}/lib" ABSOLUTE)
get_filename_component(GUIDANCE_TOOLS_PATH "${GUIDANCE_ROOT_PATH}/tools" ABSOLUTE)
get_filename_component(GUIDANCE_PLATFORMS_PATH "${GUIDANCE_ROOT_PATH}/platforms" ABSOLUTE)
get_filename_component(GUIDANCE_SDK_PATH "${GUIDANCE_ROOT_PATH}/guidance-sdk" ABSOLUTE)
get_filename_component(GUIDANCE_SDK_INC_PATH "${GUIDANCE_SDK_PATH}/include" ABSOLUTE)
get_filename_component(GUIDANCE_SDK_LIB_PATH "${GUIDANCE_SDK_PATH}/lib" ABSOLUTE)
get_filename_component(GUIDANCE_SDK_SO_PATH "${GUIDANCE_SDK_PATH}/so" ABSOLUTE)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all executables")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/int CACHE PATH "Single Directory for all static libraries")

#-------------------------------------------------------------------------------
# Load platform dependent parameters and settings
include("${GUIDANCE_PLATFORMS_PATH}/common/ConfigLib.cmake")

set(GUIDANCE_REQUIRED_LIBS ${GUIDANCE_REQUIRED_LIBS} ${Guidance_Lib})

get_filename_component(GUIDANCE_DYLIB_PATH "${Guidance_Lib}" DIRECTORY)

list(APPEND CMAKE_PREFIX_PATH "${OPENCV_LIBRARYDIR}")

include_directories(${GUIDANCE_INC_PATH})
include_directories(${GUIDANCE_SDK_INC_PATH})

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)
list(APPEND GUIDANCE_REQUIRED_LIBS  ${OpenCV_LIBS})

add_subdirectory(base)
add_subdirectory(checkerboard)                              
