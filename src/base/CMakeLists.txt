set(FY_BASE_HEADERS ${GUIDANCE_INC_PATH}/fy_log.hpp)

set(FY_BASE_SRCS ${GUIDANCE_SRC_PATH}/base/fy_log.cpp)

add_library(fybase STATIC ${FY_BASE_HEADERS} ${FY_BASE_SRCS})