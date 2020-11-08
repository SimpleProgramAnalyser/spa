file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB unit_testing_utils
    "${CMAKE_CURRENT_SOURCE_DIR}/../unit_testing/src/cfg_utils/CfgUtils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../unit_testing/src/cfg_utils/CfgUtils.cpp")

add_executable(integration_testing ${srcs} ${headers} ${unit_testing_utils})

target_link_libraries(integration_testing spa)
