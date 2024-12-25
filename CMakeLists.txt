cmake_minimum_required(VERSION 3.20)

project(ccomp VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -Wall
    -Wwrite-strings
    -Wstrict-prototypes
    -Wsign-compare
    -fno-common
    -Wtype-limits
    -Wformat-nonliteral
    -Wmissing-declarations
    #    -fsanitize=address
    -fno-omit-frame-pointer
    -pipe
    #-mavx2
    #-O3
)

#target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
