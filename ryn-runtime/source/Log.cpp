module;
#include <cstdio>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

module Ryntime.Log;

// TODO: logger fix scuffed fallback logic & add error handler spdlog::set_error_handler
//   since we disabled exceptions in the build settings

namespace ryn {
    static std::shared_ptr<spdlog::logger> s_Logger;

    static void fallback(const char* scope, const char* message) { printf("[FALLBACK] [%s] %s\n", scope, message); }

    void Log::init() {
        if (s_Logger) return;

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%^[%T] %v%$");

        s_Logger = std::make_shared<spdlog::logger>("RynLogger", consoleSink);
#if defined(RYN_CONFIG_DEBUG)
        s_Logger->set_level(spdlog::level::trace);
#else
        s_Logger->set_level(spdlog::level::info);
#endif
        spdlog::set_default_logger(s_Logger);
    }

    void Log::shutdown() {
        s_Logger.reset();
        spdlog::shutdown();
    }

    void Log::trace(const char* scope, const char* message) {
        if (!s_Logger) return fallback(scope, message), void();
        s_Logger->log(spdlog::level::trace, "[{}] {}", scope, message);
    }

    void Log::info(const char* scope, const char* message) {
        if (!s_Logger) return fallback(scope, message), void();
        s_Logger->log(spdlog::level::info, "[{}] {}", scope, message);
    }

    void Log::warn(const char* scope, const char* message) {
        if (!s_Logger) return fallback(scope, message), void();
        s_Logger->log(spdlog::level::warn, "[{}] {}", scope, message);
    }

    void Log::error(const char* scope, const char* message) {
        if (!s_Logger) return fallback(scope, message), void();
        s_Logger->log(spdlog::level::err, "[{}] {}", scope, message);
    }

    void Log::critical(const char* scope, const char* message) {
        if (!s_Logger) return fallback(scope, message), void();
        s_Logger->log(spdlog::level::critical, "[{}] {}", scope, message);
    }
}