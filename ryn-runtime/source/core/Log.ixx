module;
#include "ryn/Defines.h"

export module ryn.core:log;

import :types;

export namespace ryn {
    enum class LogLevel : u8 {
        Trace,
        Info,
        Warn,
        Error
    };

    /**
     * @brief   Writes one finished message to the console.
     * @param   level    Severity of the message.
     * @param   category Null-terminated name of the subsystem that logs, e.g. "PluginHost".
     * @param   message  Null-terminated, already formatted text.
     * @warning Not safe to call from several threads at once: lines may interleave.
     * @todo    M1: replace by the compile-time formatter + log macros (decision 8); this stays as their sink.
     */
    RYN_API void logMessage(LogLevel level, const char* category, const char* message);
}
