file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(spa

    # current directory
    ${srcs}
    ${headers}

    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/Lexer.cpp
        src/pql/PqlManager.h src/pql/PqlManager.cpp src/pql/Evaluator.h src/pql/Evaluator.cpp src/pql/AqTypes.h src/pql/AqTypes.cpp)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


