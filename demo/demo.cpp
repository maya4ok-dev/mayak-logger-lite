#include <mayak/logger-lite.hpp>

int main() {
    

    // API is actually simple. I mean, there's no need to do anything special.

    // Using default levels
    MAYAK_LOG(DEBUG) << "Hello, world!";
    MAYAK_LOG(INFO) << "Hello, world!";
    MAYAK_LOG(WARNING) << "Hello, world!";
    MAYAK_LOG(ERROR) << "Hello, world!";
    MAYAK_LOG(FATAL) << "Hello, world!";

    // Adding a new level
    mayak::logger_lite::Level verbose = {"VERBOSE", {255, 255, 255}, 5};
    MAYAK_LOG(verbose) << "Hello, world!";

    // Temporary Level object won't work because it gets destroyed immediately,
    // leaving a dangling reference inside Logger. NEVER DO THIS:
    // MAYAK_LOG({ "VERBOSE", {255, 255, 255}, 5 }) << "Goodbye, world!";
}