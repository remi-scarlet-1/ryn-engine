module;
#include "Defines.h"
#include <format>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

module Ryntime.Logger;

namespace ryn {
    static std::shared_ptr<spdlog::logger> s_Logger;

    static spdlog::logger* getLogger() {
        if (s_Logger) [[likely]]
            return s_Logger.get();

        // Fallback: If s_Logger isn't initialized, use the default spdlog logger.
        return spdlog::default_logger().get();
    }

    static constexpr spdlog::level::level_enum toLevel(LogLevel level) {
        switch (level) {
            case LogLevel::Trace: return spdlog::level::trace;
            case LogLevel::Info: return spdlog::level::info;
            case LogLevel::Warn: return spdlog::level::warn;
            case LogLevel::Error: return spdlog::level::err;
            case LogLevel::Critical: return spdlog::level::critical;
        }
        return spdlog::level::info;
    }

    void Logger::init() {
        auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        color_sink->set_pattern("%^[%T] %v%$");
        s_Logger = std::make_shared<spdlog::logger>("RYN", color_sink);
        s_Logger->flush_on(spdlog::level::err);

#if defined(RYN_CONFIG_DEBUG)
        s_Logger->set_level(spdlog::level::trace);
#else
        s_Logger->set_level(spdlog::level::info);
#endif
    }

    void Logger::shutdown() {
        s_Logger.reset();
        spdlog::shutdown();
    }

    void Logger::dispatch(LogLevel level, const char* primary, std::string_view message) {
        spdlog::logger* logger   = getLogger();
        auto            spdLevel = toLevel(level);
        logger->log(spdLevel, "[{}] {}", primary, message);
    }

    void Logger::dispatch(LogLevel level, const char* primary, const char* secondary, std::string_view message) {
        spdlog::logger* logger   = getLogger();
        auto            spdLevel = toLevel(level);
        logger->log(spdLevel, "[{}::{}] {}", primary, secondary, message);
    }
}