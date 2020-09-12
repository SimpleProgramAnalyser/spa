set(CMAKE_BUILD_TYPE Debug)

file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB pql
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.hpp")
file(GLOB_RECURSE pkb
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.h")

add_executable(unit_testing ${srcs} ${pql} ${pkb})

target_link_libraries(unit_testing spa)
