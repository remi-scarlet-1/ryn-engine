#include "ryn/Log.h"

#include <cstdio>
#include <print>

namespace ryn {
    namespace {
        constexpr std::string_view levelName(LogLevel level) {
            switch (level) {
                case LogLevel::Trace: return "trace";
                case LogLevel::Info: return "info";
                case LogLevel::Warn: return "warn";
                case LogLevel::Error: return "error";
            }
            return "?";
        }
    }

    void logMessage(LogLevel level, std::string_view category, std::string_view message) {
        std::FILE* stream = level >= LogLevel::Warn ? stderr : stdout;
        std::println(stream, "[{}] [{}] {}", levelName(level), category, message);
    }
}
