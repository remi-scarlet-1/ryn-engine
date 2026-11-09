#include "ryn/Log.h"
#include "ryn/Plugin.h"

namespace {
    constexpr std::string_view LOG = "Game";

    class GamePlugin final : public ryn::IPlugin {
    public:
        void onLoad() override { ryn::log(ryn::LogLevel::Info, LOG, "Hello from the game plugin"); }
        void onUnload() override { ryn::log(ryn::LogLevel::Info, LOG, "Goodbye from the game plugin"); }
    };
}

RYN_PLUGIN(GamePlugin, "Game")
