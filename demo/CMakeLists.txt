cmake_minimum_required(VERSION 3.20)

project(mayak-logger-lite-demo LANGUAGES CXX)

add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE mayak-logger-lite)
set_target_properties(demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE mayak-logger-lite)
set_target_properties(benchmark PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
