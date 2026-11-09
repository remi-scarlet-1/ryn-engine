#include "ryn/Plugin.h"

import ryn;

namespace {
    constexpr const char* LOG = "Sandbox";

    struct SandboxRoot {
        void onLoad() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Hello from the sandbox plugin"); }
        void onUnload() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Goodbye from the sandbox plugin"); }
    };
}

RYN_PLUGIN(SandboxRoot, "Sandbox")
