#include <mayak/logger-lite.hpp>
#include <string>
#include <memory>

int main() {
    using namespace mayak::lite::logger;

    // --- 1. Basic logging ---
    MAYAK_LOG(DEBUG)   << "Debug message";
    MAYAK_LOG(INFO)    << "Info message";
    MAYAK_LOG(WARNING) << "Warning message";
    MAYAK_LOG(ERROR)   << "Error message";
    MAYAK_LOG(FATAL)   << "Fatal message";

    // Using mayak::log() directly is equivalent, but with no file/line info
    mayak::log(DEBUG)   << "Debug message";
    mayak::log(INFO)    << "Info message";
    mayak::log(WARNING) << "Warning message";
    mayak::log(ERROR)   << "Error message";
    mayak::log(FATAL)   << "Fatal message";

    // --- 2. Level filtering ---
    minLevelPriority(40); // Only warning and above
    MAYAK_LOG(DEBUG)   << "This DEBUG should NOT appear";
    MAYAK_LOG(INFO)    << "This INFO should NOT appear";
    MAYAK_LOG(WARNING) << "This WARNING should appear";
    MAYAK_LOG(ERROR)   << "This ERROR should appear";

    // Let's reset the priority
    minLevelPriority(0);

    // --- 3. Turn off coloring ---
    coloring(false);
    MAYAK_LOG(INFO) << "Coloring is now OFF";

    // Let's turn coloring back on
    coloring(true);

    // --- 4. Custom level ---
    Level VERBOSE{"VERBOSE", {128, 128, 128}, 10};
    MAYAK_LOG(VERBOSE) << "This is a custom VERBOSE message";

    // --- 5. Long message (>256 chars) ---
    std::string long_msg(300, 'X');
    MAYAK_LOG(INFO) << "Long message: " << long_msg;

    // --- 6. Numbers and different types ---
    MAYAK_LOG(INFO) << "Integer: " << 12345;
    MAYAK_LOG(INFO) << "Negative: " << -9876;
    MAYAK_LOG(INFO) << "Double: " << 3.14159;
    MAYAK_LOG(INFO) << "String: " << "Hello Logger!";

    // --- 7. Using mayak::log() directly with file/line ---
    mayak::log(DEBUG, __FILE__, __LINE__)   << "Direct call with file/line";
    mayak::log(INFO, __FILE__, __LINE__)    << "Direct call with file/line";
    mayak::log(WARNING, __FILE__, __LINE__) << "Direct call with file/line";
    mayak::log(ERROR, __FILE__, __LINE__)   << "Direct call with file/line";
    mayak::log(FATAL, __FILE__, __LINE__)   << "Direct call with file/line";

    // --- 8. Non-supported types ---
    MAYAK_LOG(INFO) << "Created Level: " << VERBOSE;
    MAYAK_LOG(INFO) << "Created unique_ptr: " << std::make_unique<int>(42);
    MAYAK_LOG(INFO) << "Created shared_ptr: " << std::make_shared<int>(42);
    int* raw_ptr = new int(42);
    MAYAK_LOG(INFO) << "Created raw_ptr: " << raw_ptr;
    delete raw_ptr;

    return 0;
}
