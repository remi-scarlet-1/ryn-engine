#include "ryn/Plugin.h"

import ryn;

namespace {
    constexpr const char* LOG = "Editor";

    struct EditorRoot {
        void onLoad() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Hello from the editor plugin"); }
        void onUnload() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Goodbye from the editor plugin"); }
    };
}

RYN_PLUGIN(EditorRoot, "Editor")
