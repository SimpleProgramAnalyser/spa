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

    # error library ?
    ${CMAKE_CURRENT_SOURCE_DIR}/src/error/InputError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/error/InputError.cpp

    # design extractor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/DesignExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/DesignExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/SemanticErrorsValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/SemanticErrorsValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/FollowsExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/FollowsExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ParentExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/UsesExtractor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/UsesExtractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/designExtractor/ModifiesExtractor.h

    # frontend/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/Token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher_Impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser/StringMatcher.h

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PreProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PreProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pql/PqlManager.cpp
)

# flags for debug
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(spa PRIVATE "-Wall;-Wextra;-pedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Bill Gates created c0v1d to s3ll vacc1n3sz!!!1!
endif()

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

