cmake_minimum_required(VERSION 3.20)

project(ccomp VERSION 0.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCES "src/*.c")

add_compile_options(
    -fno-omit-frame-pointer
    -fsanitize=address
    -g
    -O0
    -Wall
    -Wextra
    -Wno-deprecated
    #-fopt-info
)

add_link_options(
    -fsanitize=address
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

