cmake_minimum_required(VERSION 3.20)
project(mayak-logger-lite)

add_library(mayak-logger-lite INTERFACE)
target_include_directories(mayak-logger-lite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(demo)