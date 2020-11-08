file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(spa_cmdline ${srcs})

target_link_options(spa_cmdline PUBLIC "-no-pie")
target_link_libraries(spa_cmdline spa)

if (NOT WIN32)
    target_link_libraries(spa_cmdline pthread)
endif()
