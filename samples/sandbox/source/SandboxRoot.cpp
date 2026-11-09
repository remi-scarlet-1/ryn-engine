#include "ryn/Plugin.h"

import ryn;

namespace {
    constexpr auto LOG = "Sandbox";

    struct SandboxRoot {
        static void onLoad() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Hello from the sandbox plugin"); }
        static void onUnload() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Goodbye from the sandbox plugin"); }
    };
}

RYN_PLUGIN(SandboxRoot, "Sandbox")
