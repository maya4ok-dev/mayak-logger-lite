/// @file logger-lite.hpp
/// @brief A header-only lite version of maya4ok's logging library
///
/// @details This is a minimum version of mayak::logger, 
///
/// @note If you don't need all the features of maya4ok's logging library or pay for them, 
/// you can use free and open source lite version!
///
/// @author Maya4ok (https://github.com/maya4ok-dev)

#pragma once

// Check for C++17 or higher
#if __cplusplus < 201703L
    #error "This library requires C++17 or higher!"
#endif

// On Windows you need to enable virtual terminal processing for colored output
#if !defined(MAYAK_LOGGER_LITE_DISABLE_WINDOWS_VT) && defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#include <iostream>
#include <cstdint>
#include <atomic>
#include <string_view>
#include <type_traits>
#include <cstdio>
#include <cstring>
#include <mutex>

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
    #include <memory>

    [[nodiscard]] inline std::string demangle(const char* name) {
        int status = 0;
        std::unique_ptr<char[], void(*)(void*)> res{
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free
        };
        return (status == 0) ? res.get() : name;
    }
#endif

namespace mayak::lite::logger {

inline struct LoggerState {
    std::atomic<bool> enabled{true};
    std::atomic<bool> colored{true};
    std::atomic<int> minLevelPriority{0};
    std::mutex mtx;

    LoggerState() {
        #if !defined(MAYAK_LOGGER_LITE_DISABLE_WINDOWS_VT) && defined(_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // Get the handle for the console output
            if (hOut != INVALID_HANDLE_VALUE) { // Ensure the handle is valid
                DWORD dwMode = 0;
                if (GetConsoleMode(hOut, &dwMode)) { // Get the current console mode
                    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // Replace the current mode
                    SetConsoleMode(hOut, dwMode); // Set the new console mode
                }
            }
        #endif
    }
} state;

[[nodiscard]] inline bool enabled() { return state.enabled.load(std::memory_order_relaxed); }
inline void enabled(bool value) { state.enabled.store(value, std::memory_order_relaxed); }

[[nodiscard]] inline bool coloring() { return state.colored.load(std::memory_order_relaxed); }
inline void coloring(bool value) { state.colored.store(value, std::memory_order_relaxed); }

[[nodiscard]] inline int minLevelPriority() { return state.minLevelPriority.load(std::memory_order_relaxed); }
inline void minLevelPriority(int value) { state.minLevelPriority.store(value, std::memory_order_relaxed); }

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Level {
    const char* name;
    RGB color;
    int priority;
    constexpr Level(const char* n, RGB c, int p): name(n), color(c), priority(p) {}
};

class Logger {
    static constexpr size_t BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    size_t pos = 0;
    Level level;
    bool active, coloring;
    const char* file;
    int line;

    void append(std::string_view str) {
        constexpr char TRUNC[] = "[TRUNCATED]";
        constexpr size_t TRUNC_LEN = sizeof(TRUNC) - 1;

        size_t available = BUFF_SIZE - pos - 1;

        if (str.size() > available) {
            size_t copy_len = available > TRUNC_LEN ? available - TRUNC_LEN : 0;
            if (copy_len > 0) {
                memcpy(buff + pos, str.data(), copy_len);
                pos += copy_len;
            }
            if (pos + TRUNC_LEN < BUFF_SIZE) {
                memcpy(buff + pos, TRUNC, TRUNC_LEN);
                pos += TRUNC_LEN;
            } else if (pos < BUFF_SIZE - 1) {
                size_t part = BUFF_SIZE - 1 - pos;
                memcpy(buff + pos, TRUNC, part);
                pos += part;
            }
        } else {
            memcpy(buff + pos, str.data(), str.size());
            pos += str.size();
        }
        buff[pos] = '\0';
    }

    template<typename T>
    void appendValue(const T& value) {
        char tmp[64];
        if constexpr(std::is_integral_v<T>) {
            snprintf(tmp, sizeof(tmp), "%lld", static_cast<long long>(value));
        } else if constexpr(std::is_floating_point_v<T>) {
            snprintf(tmp, sizeof(tmp), "%f", static_cast<double>(value));
        }
        append(tmp);
    }

public:

    template<typename T>
    Logger& operator<<(T&& t) {
        using U = std::remove_reference_t<T>;

        if constexpr (std::is_arithmetic_v<U>) {
            appendValue(t);
        } else if constexpr (std::is_convertible_v<U, std::string_view>) {
            append(std::string_view(t));
        } else if constexpr (std::is_pointer_v<U>) {
            append("[");
            #if __has_include(<cxxabi.h>)
                append(demangle(typeid(*t).name()));
            #else
                append(typeid(*t).name());
            #endif
            append(" @ ");
            char tmp[32];
            snprintf(tmp, sizeof(tmp), "%p", t);
            append(tmp);
            append("]");
        } else {
            append("[");
            #if __has_include(<cxxabi.h>)
                append(demangle(typeid(t).name()));
            #else
                append(typeid(t).name());
            #endif
            append(" @ ");
            char tmp[32];
            snprintf(tmp, sizeof(tmp), "%p", (const void*)&t);
            append(tmp);
            append("]");
        }

        return *this;
    }

    Logger(const Level& lvl): level(lvl), file(nullptr), line(0),
        active(state.enabled.load(std::memory_order_relaxed) && level.priority >= state.minLevelPriority.load(std::memory_order_relaxed)), 
        coloring(state.colored.load(std::memory_order_relaxed)) {}

    Logger(const Level& lvl, const char* f, int l): file(f), line(l), level(lvl), 
        active(state.enabled.load(std::memory_order_relaxed) && level.priority >= state.minLevelPriority.load(std::memory_order_relaxed)), 
        coloring(state.colored.load(std::memory_order_relaxed)) {}

    ~Logger() {
        if (!active) return;

        std::lock_guard<std::mutex> lock(state.mtx);

        std::string_view filename;
        if (file) {
            filename = file;
            auto pos_sep = filename.find_last_of("/\\");
            if (pos_sep != std::string_view::npos) filename.remove_prefix(pos_sep + 1);
        }

        if (coloring) {
            std::cout << "\x1b[38;2;"
                    << int(level.color.r) << ";"
                    << int(level.color.g) << ";"
                    << int(level.color.b) << "m";
        }

        std::cout << "[" << level.name << "] ";
        if (file) std::cout << "[" << filename << ":" << line << "] ";
        std::cout << buff;
        if (coloring) std::cout << "\x1b[0m";
        std::cout << "\n";
    }
};

} // namespace mayak::lite::logger

namespace mayak {

/// @brief Logs a message at the specified level to the console
/// @param level The logging level, e.g., DEBUG, INFO, WARNING, ERROR 
/// @details This function returns a Logger instance that can be used to log messages.
///
/// Usage example:
/// @code
/// mayak::log(INFO) << "This is an info message";
/// @endcode
inline lite::logger::Logger log(lite::logger::Level level) { return lite::logger::Logger(level); }

/// @brief Logs a message at the specified level to the console using the specified file and line number
/// @param level The logging level, e.g., DEBUG, INFO, WARNING, ERROR 
/// @details This function returns a Logger instance that can be used to log messages.
///
/// Usage example:
/// @code
/// mayak::log(INFO, __FILE__, __LINE__) << "This is an info message";
/// @endcode
inline lite::logger::Logger log(lite::logger::Level level, const char* file, int line) { return lite::logger::Logger(level, file, line); }

}

/// @def MAYAK_LOG
/// @brief Logs a message at the specified level to the console with file and line number
/// @param level The logging level, e.g., DEBUG, INFO, WARNING, ERROR
/// @details This macro expands to a call to the log function with the current file and line number.
///
/// @code
/// MAYAK_LOG(INFO) << "This is an info message";
/// @endcode
#define MAYAK_LOG(level) mayak::log(level, __FILE__, __LINE__)

#ifndef MAYAK_LOGGER_LITE_DISABLE_DEFAULT_LEVELS
    static constexpr mayak::lite::logger::Level DEBUG{"DEBUG", {0, 200, 0}, 20};
    static constexpr mayak::lite::logger::Level INFO{"INFO", {0, 120, 255}, 30};
    static constexpr mayak::lite::logger::Level WARNING{"WARNING", {255, 215, 0}, 40};
    static constexpr mayak::lite::logger::Level ERROR{"ERROR", {220, 20, 60}, 50};
    static constexpr mayak::lite::logger::Level FATAL{"FATAL", {255, 0, 0}, 60};
#endif
