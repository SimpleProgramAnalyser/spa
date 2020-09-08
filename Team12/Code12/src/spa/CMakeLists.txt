file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE pkb "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.h")

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

    # pkb
    ${pkb}

    # pql
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/AqTypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/AqTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Evaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Evaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/FormattedQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/FormattedQueryResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Projector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/Projector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/RawQueryResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/RawQueryResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/QueryResultFormatType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.cpp
)

# flags for debug
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")
endif(CMAKE_COMPILER_IS_GNUCXX)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


