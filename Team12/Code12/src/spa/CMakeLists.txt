file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

set(CMAKE_BUILD_TYPE Debug)

add_library(spa

    # current directory
    ${srcs}
    ${headers}

    # lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.cpp

    # ast library
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstLibrary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/AstLibrary.cpp

    # pql
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Evaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Evaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/AqTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/AqTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/RawQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/RawQueryResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/FormattedQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/FormattedQueryResult.cpp
)

# flags for debug
target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


