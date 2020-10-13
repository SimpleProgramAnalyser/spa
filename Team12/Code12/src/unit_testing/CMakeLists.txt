set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ast_utils/AstUtils.cpp")
file(GLOB headers
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ast_utils/AstUtils.h")
file(GLOB_RECURSE frontend
        "${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/*.hpp")
file(GLOB_RECURSE pql
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/parser*.cpp")
file(GLOB_RECURSE pkb
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/relationships/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/tables/*.cpp")
file(GLOB_RECURSE cfg
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cfg/*.h")

add_executable(unit_testing ${srcs} ${headers} ${pql} ${frontend} ${pkb} ${cfg})

target_link_libraries(unit_testing spa)
