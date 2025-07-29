#include <mayak/logger-lite.hpp>

int main() {
    using namespace mayak::logger_lite;

    // Basic usage
    MAYAK_LOG(DEBUG) << "This is a debug message.";
    MAYAK_LOG(INFO) << "Logger initialized successfully.";
    MAYAK_LOG(WARNING) << "Low disk space warning!";
    MAYAK_LOG(ERROR) << "Failed to open file.";
    MAYAK_LOG(FATAL) << "Critical error! System shutting down.";

    // Change logger state
    setMinLogLevel(40); // Only WARNING and above
    MAYAK_LOG(DEBUG) << "This debug message will NOT appear.";
    MAYAK_LOG(ERROR) << "But this error will be logged.";


    // Disable coloring
    setMinLogLevel(0);
    setColoring(false);
    MAYAK_LOG(INFO) << "Coloring is disabled now.";

    // Add a custom level
    Level VERBOSE{"VERBOSE", {128, 128, 128}, 10};
    MAYAK_LOG(VERBOSE) << "This is a custom verbose message.";

    return 0;
}
