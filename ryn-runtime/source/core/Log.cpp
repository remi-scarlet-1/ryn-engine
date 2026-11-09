module;
#include <cstdio>

module ryn.core;

namespace ryn {
    namespace {
        constexpr const char* levelName(const LogLevel level) {
            switch (level) {
                case LogLevel::Trace: return "trace";
                case LogLevel::Info: return "info";
                case LogLevel::Warn: return "warn";
                case LogLevel::Error: return "error";
            }
            return "?";
        }
    }

    void logMessage(const LogLevel level, const char* category, const char* message) {
        std::FILE* stream = level >= LogLevel::Warn ? stderr : stdout;

        // Plain writes, no runtime format parsing
        std::fputs("[", stream);
        std::fputs(levelName(level), stream);
        std::fputs("] [", stream);
        std::fputs(category ? category : "?", stream);
        std::fputs("] ", stream);
        std::fputs(message ? message : "", stream);
        std::fputc('\n', stream);
    }
}
