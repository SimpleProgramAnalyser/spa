file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

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

    # frontend/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher.h
)

# flags for debug
target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


