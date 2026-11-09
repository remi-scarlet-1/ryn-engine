#include "ryn/Plugin.h"

import ryn;

namespace {
    constexpr auto LOG = "Editor";

    struct EditorRoot {
        static void onLoad() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Hello from the editor plugin"); }
        static void onUnload() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Goodbye from the editor plugin"); }
    };
}

RYN_PLUGIN(EditorRoot, "Editor")
