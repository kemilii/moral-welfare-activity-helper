cmake_minimum_required(VERSION 3.12)
project(sudoku CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)


file(GLOB SOURCES
        src/*.cpp)

add_executable(sudoku ${SOURCES})
