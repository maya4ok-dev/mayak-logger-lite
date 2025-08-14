# Header-only library
add_library(mayak-logger-lite INTERFACE)

target_include_directories(mayak-logger-lite INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(mayak-logger-lite INTERFACE cxx_std_17)

# Demo
option(MAYAK_LOGGER_LITE_BUILD_DEMO "Build demo and benchmark executables" ON)
if(MAYAK_LOGGER_LITE_BUILD_DEMO)
    add_subdirectory(demo)
endif()

# Install rules
install(TARGETS mayak-logger-lite EXPORT mayak-logger-liteTargets)
install(DIRECTORY include/ DESTINATION include)

# Install export
install(EXPORT mayak-logger-liteTargets
    DESTINATION cmake
    FILE mayak-logger-lite-config.cmake
)
