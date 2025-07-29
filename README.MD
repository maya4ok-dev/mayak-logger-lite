# mayak-logger-lite

A minimalistic, header-only, and high-efficiency logging library for C++17.  
Built for speed, simplicity, and zero-dependency usage.

---

## ✨ Features
- **Header-only** – just include one file.
- **Blazing fast** – up to **5.6M logs/sec** with no output.
- **Atomic state** – thread-safe runtime enable/disable and minimum log level control.
- **Color support** – RGB log coloring (with Windows VT auto-init).
- **Custom levels** – add your own log levels at runtime.
- **Zero dependencies** – works with just the standard library.

---

## 🚀 Quick Start

### 1. Add submodule
```bash
git submodule add https://github.com/maya4ok-dev/mayak-logger-lite.git external/mayak-logger-lite
```

### 2. CMake integration
```cmake
add_subdirectory(external/mayak-logger-lite)
target_link_libraries(&{PROJECT_NAME} PRIVATE mayak-logger-lite)
```

### 3. Usage
```cpp
#include <mayak/logger-lite.hpp>

int main() {
    MAYAK_LOG(DEBUG) << "Hello, world!";
    MAYAK_LOG(INFO) << "Logger initialized successfully.";
    MAYAK_LOG(WARNING) << "Low disk space warning!";
    MAYAK_LOG(ERROR) << "Failed to open file.";
    MAYAK_LOG(FATAL) << "Critical error!";

    mayak::logger_lite::Level VERBOSE{"VERBOSE", {255, 255, 255}, 10};
    MAYAK_LOG(VERBOSE) << "Custom verbose message";

    return 0;
}
```

### 4. Output
```log
[DEBUG] This is a debug message.
[INFO] Logger initialized successfully.
[WARNING] Low disk space warning!
[ERROR] Failed to open file.
[FATAL] Critical error! System shutting down.
[ERROR] But this error will be logged.
[INFO] Coloring is disabled now.
[VERBOSE] This is a custom verbose message.
```

---


| Mode           | 1M messages | Logs/sec      |
| -------------- | ----------- | ------------- |
| With output    | \~7.4 s     | \~135k logs/s |
| Without output | \~176 ms    | \~5.6M logs/s |

---

## 🔧 Configuration

Macros:

* `MAYAK_LOGGER_LITE_DISABLE_DEFAULT_LEVELS` – disable built-in log levels.
* `MAYAK_LOGGER_LITE_DISABLE_WINDOWS_VT` – disable VT sequence initialization on Windows.

---

## 📜 License

MIT – do whatever you want, just give credit.
Author: [Maya4ok](https://github.com/maya4ok-dev)