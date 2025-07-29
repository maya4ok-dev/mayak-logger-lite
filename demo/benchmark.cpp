#include <mayak/logger-lite.hpp>
#include <chrono>
#include <iostream>

void bench_with_output(int count) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        MAYAK_LOG(INFO) << "Log message number " << i;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "With output: Logged " << count << " messages in " << duration.count() << " ms\n";
    std::cout << "≈ " << count / (duration.count() / 1000.0) << " logs/sec\n";
}

void bench_without_output(int count) {
    // Временно отключаем вывод, просто делаем "пустой" логгер
    bool old_enabled = mayak::logger_lite::isEnabled();
    mayak::logger_lite::setEnabled(false);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        MAYAK_LOG(INFO) << "Log message number " << i;
    }
    auto end = std::chrono::high_resolution_clock::now();

    mayak::logger_lite::setEnabled(old_enabled);

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Without output: Logged " << count << " messages in " << duration.count() << " ms\n";
    std::cout << "≈ " << count / (duration.count() / 1000.0) << " logs/sec\n";
}

int main() {
    constexpr int count = 1000000;

    bench_with_output(count);
    bench_without_output(count);

    return 0;
}
