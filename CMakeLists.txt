cmake_minimum_required(VERSION 3.20)
project(mayak-logger-lite VERSION 1.0 LANGUAGES CXX)

# Library
add_library(mayak-logger-lite INTERFACE)
target_include_directories(mayak-logger-lite
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(mayak-logger-lite INTERFACE cxx_std_17)

# Demo
option(MAYAK_LOGGER_LITE_BUILD_DEMO "Build the demo executable" ON)

if (MAYAK_LOGGER_LITE_BUILD_DEMO)
    add_subdirectory(demo)
endif()

# Install rules
install(TARGETS mayak-logger-lite EXPORT mayak-logger-lite-config)
install(DIRECTORY include/ DESTINATION include)
export(EXPORT mayak-logger-lite-config NAMESPACE mayak:: FILE mayak-logger-lite-config.cmake)
