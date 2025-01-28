cmake_minimum_required(VERSION 3.20)

project(ccomp VERSION 0.0.0 LANGUAGES C)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -Wall
    -Wextra
    -Wwrite-strings
    -Wstrict-prototypes
    -Wformat-nonliteral
    -Wmissing-declarations
    -fno-common
    -fsanitize=address
    -fno-omit-frame-pointer
    #
    -pipe
    -march=native
    -O0
    -g
)

target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
