module;
#include "Defines.h"

export module Ryntime.Log;

export namespace ryn {
    class RYN_API Log {
    public:
        static void init();
        static void shutdown();

        static void trace(const char* scope, const char* message);
        static void info(const char* scope, const char* message);
        static void warn(const char* scope, const char* message);
        static void error(const char* scope, const char* message);
        static void critical(const char* scope, const char* message);
    };
}
