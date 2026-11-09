#include <cstdio>
#include <cstring>

import ryn.core;
import ryn.internal;

namespace {
    constexpr auto LOG           = "Runtime";
    constexpr auto GAME_PLUGIN   = "RynSandbox"; // TODO(M4): read from the project file instead
    constexpr auto EDITOR_PLUGIN = "RynEditor";

    struct LaunchOptions {
        bool Editor   = false;
        bool ShowHelp = false;
    };

    bool parseArguments(const int argc, char** argv, LaunchOptions& options) {
        for (int i = 1; i < argc; ++i) {
            const char* arg = argv[i];
            if (std::strcmp(arg, "--editor") == 0) options.Editor = true;
            else if (std::strcmp(arg, "--help") == 0 || std::strcmp(arg, "-h") == 0) options.ShowHelp = true;
            else {
                char message[256];
                std::snprintf(message, sizeof(message), "Unknown argument '%s'", arg); // TODO(M1): log formatter
                ryn::logMessage(ryn::LogLevel::Error, LOG, message);
                return false;
            }
        }
        return true;
    }

    void printHelp() {
        char gameFile[ryn::MAX_PATH_LENGTH];
        char editorFile[ryn::MAX_PATH_LENGTH];
        ryn::DynamicLibrary::fileName(GAME_PLUGIN, gameFile, sizeof(gameFile));
        ryn::DynamicLibrary::fileName(EDITOR_PLUGIN, editorFile, sizeof(editorFile));

        std::fputs("Usage: RynRuntime [--editor]\n  (default)   Load the game plugin (", stdout);
        std::fputs(gameFile, stdout);
        std::fputs(")\n  --editor    Load the editor plugin (", stdout);
        std::fputs(editorFile, stdout);
        std::fputs(")\n", stdout);
    }

    // Builds "<executable folder>/<platform file name of plugin>"
    bool pluginPath(const char* pluginName, char* out, ryn::usize capacity) {
        char directory[ryn::MAX_PATH_LENGTH];
        char file[ryn::MAX_PATH_LENGTH];
        if (!ryn::executableDirectory(directory, sizeof(directory))) return false;
        if (!ryn::DynamicLibrary::fileName(pluginName, file, sizeof(file))) return false;

        const int written = std::snprintf(out, capacity, "%s%c%s", directory, ryn::PATH_SEPARATOR, file);
        return written > 0 && static_cast<ryn::usize>(written) < capacity;
    }
}

int main(const int argc, char** argv) {
    LaunchOptions options;
    if (!parseArguments(argc, argv, options)) {
        printHelp();
        return 1;
    }
    if (options.ShowHelp) {
        printHelp();
        return 0;
    }

    char path[ryn::MAX_PATH_LENGTH];
    if (!pluginPath(options.Editor ? EDITOR_PLUGIN : GAME_PLUGIN, path, sizeof(path))) {
        ryn::logMessage(ryn::LogLevel::Error, LOG, "Could not build the plugin path (path too long?)");
        return 1;
    }

    ryn::PluginHost plugin;
    if (!plugin.load(path)) return 1;

    // TODO(M4): main loop once the runtime has an application model
    plugin.unload();

    ryn::logMessage(ryn::LogLevel::Info, LOG, "Shutdown complete");
    return 0;
}
