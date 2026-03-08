module;
#include "Defines.h"
#include <spdlog/spdlog.h>

export module Ryntime.Logger;

export namespace ryn {
    enum class LogLevel {
        Trace,
        Info,
        Warn,
        Error,
        Critical
    };

    class RYN_API Logger {
    public:
        static void init();
        static void shutdown();

        template<typename... Args>
        static void message(LogLevel level, const char* primary, std::format_string<Args...> fmt, Args&&... args) {
            dispatch(level, primary, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void messageLocal(LogLevel level, const char* primary, const char* secondary, std::format_string<Args...> fmt, Args&&... args) {
            dispatch(level, primary, secondary, std::format(fmt, std::forward<Args>(args)...));
        }

    private:
        static void dispatch(LogLevel level, const char* primary, std::string_view message);
        static void dispatch(LogLevel level, const char* primary, const char* secondary, std::string_view message);
    };
}