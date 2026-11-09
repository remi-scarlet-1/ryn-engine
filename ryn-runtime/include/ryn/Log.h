#pragma once

#include <format>
#include <string_view>

#include "ryn/Core.h"

namespace ryn {
    enum class LogLevel { Trace, Info, Warn, Error };

    // Writes a finished message. Lives in the runtime so all output goes through one place.
    RYN_API void logMessage(LogLevel level, std::string_view category, std::string_view message);

    // Formatting happens on the caller's side; only the finished string crosses into the runtime.
    template<typename... Args>
    void log(LogLevel level, std::string_view category, std::format_string<Args...> fmt, Args&&... args) {
        logMessage(level, category, std::format(fmt, std::forward<Args>(args)...));
    }
}
