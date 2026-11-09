#include <print>
#include <span>
#include <string_view>

#include "DynamicLibrary.h"
#include "PluginHost.h"
#include "ryn/Log.h"

namespace {
    constexpr std::string_view LOG          = "Runtime";
    constexpr std::string_view GAME_PLUGIN   = "RynGame";
    constexpr std::string_view EDITOR_PLUGIN = "RynEditor";

    struct LaunchOptions {
        bool Editor   = false;
        bool ShowHelp = false;
    };

    bool parseArguments(std::span<char*> args, LaunchOptions& options) {
        for (std::string_view arg : args.subspan(1)) {
            if (arg == "--editor") options.Editor = true;
            else if (arg == "--help" || arg == "-h") options.ShowHelp = true;
            else {
                ryn::log(ryn::LogLevel::Error, LOG, "Unknown argument '{}'", arg);
                return false;
            }
        }
        return true;
    }

    void printHelp() {
        std::println("Usage: RynRuntime [--editor]\n"
                     "  (default)   Load the game plugin ({})\n"
                     "  --editor    Load the editor plugin ({})",
                     ryn::DynamicLibrary::fileName(GAME_PLUGIN), ryn::DynamicLibrary::fileName(EDITOR_PLUGIN));
    }
}

int main(int argc, char** argv) {
    LaunchOptions options;
    if (!parseArguments(std::span(argv, static_cast<std::size_t>(argc)), options)) {
        printHelp();
        return 1;
    }
    if (options.ShowHelp) {
        printHelp();
        return 0;
    }

    const auto pluginName = options.Editor ? EDITOR_PLUGIN : GAME_PLUGIN;
    const auto pluginPath = ryn::executableDirectory() / ryn::DynamicLibrary::fileName(pluginName);

    ryn::PluginHost plugin;
    if (!plugin.load(pluginPath)) return 1;

    // TODO: main loop once the runtime has a window and services
    plugin.unload();

    ryn::log(ryn::LogLevel::Info, LOG, "Shutdown complete");
    return 0;
}
