set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp")
file(GLOB_RECURSE headers
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.hpp")

add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)
