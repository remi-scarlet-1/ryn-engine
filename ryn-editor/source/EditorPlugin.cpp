#include "ryn/Log.h"
#include "ryn/Plugin.h"

namespace {
    constexpr std::string_view LOG = "Editor";

    class EditorPlugin final : public ryn::IPlugin {
    public:
        void onLoad() override { ryn::log(ryn::LogLevel::Info, LOG, "Hello from the editor plugin"); }
        void onUnload() override { ryn::log(ryn::LogLevel::Info, LOG, "Goodbye from the editor plugin"); }
    };
}

RYN_PLUGIN(EditorPlugin, "Editor")
