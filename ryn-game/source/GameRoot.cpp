#include "ryn/Plugin.h"

import ryn;

namespace {
    constexpr const char* LOG = "Game";

    struct GameRoot {
        void onLoad() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Hello from the game plugin"); }
        void onUnload() { ryn::logMessage(ryn::LogLevel::Info, LOG, "Goodbye from the game plugin"); }
    };
}

RYN_PLUGIN(GameRoot, "Game")
