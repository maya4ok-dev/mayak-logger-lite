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

// C++17+
#if __cplusplus < 201703L
    #error "This library requires C++17 or higher!"
#endif

#include <sstream>
#include <iostream>
#include <cstdint>
#include <atomic>

namespace mayak::logger_lite {

    inline struct LoggerState {
        std::atomic<bool> enabled{true};
        std::atomic<bool> colored{true};
        std::atomic<int> minLogLevel{0};
    } state;

    [[nodiscard]] inline bool isEnabled() { return state.enabled.load(std::memory_order_relaxed); }
    inline void setEnabled(bool value) { state.enabled.store(value, std::memory_order_relaxed); }

    [[nodiscard]] inline bool isColoring() { return state.colored.load(std::memory_order_relaxed); }
    inline void setColoring(bool value) { state.colored.store(value, std::memory_order_relaxed); }

    [[nodiscard]] inline int getMinLogLevel() { return state.minLogLevel.load(std::memory_order_relaxed); }
    inline void setMinLogLevel(int value) { state.minLogLevel.store(value, std::memory_order_relaxed); }

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
        std::ostringstream oss;
        Level level;
        bool active, coloring;

    public:
        Logger(const Level& lvl): level(lvl), 
            active(state.enabled.load(std::memory_order_relaxed) && level.priority >= state.minLogLevel.load(std::memory_order_relaxed)), 
            coloring(state.colored.load(std::memory_order_relaxed)) {}

        ~Logger() {
            if (!active) return;

            if (coloring) {
                std::cout << "\x1b[38;2;" 
                        << int(level.color.r) << ";" 
                        << int(level.color.g) << ";" 
                        << int(level.color.b) << "m";
            }

            std::cout << "[" << level.name << "] " << oss.str();

            if (coloring)
                std::cout << "\x1b[0m";

            std::cout << "\n";
        }

        template<typename T>
        Logger& operator<<(const T& t) {
            oss << t;
            return *this;
        }
    };
}

#define MAYAK_LOG(level) mayak::logger_lite::Logger(level)

#ifndef MAYAK_LOGGER_LITE_DISABLE_DEFAULT_LEVELS
    static constexpr mayak::logger_lite::Level DEBUG{"DEBUG", {0, 200, 0}, 20};
    static constexpr mayak::logger_lite::Level INFO{"INFO", {0, 120, 255}, 30};
    static constexpr mayak::logger_lite::Level WARNING{"WARNING", {255, 215, 0}, 40};
    static constexpr mayak::logger_lite::Level ERROR{"ERROR", {220, 20, 60}, 50};
    static constexpr mayak::logger_lite::Level FATAL{"FATAL", {255, 0, 0}, 60};
#endif