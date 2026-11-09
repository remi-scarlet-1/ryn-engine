#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

#include "ryn/Core.h"

namespace ryn {
    // Bump whenever IPlugin or PluginInfo change shape. The runtime refuses plugins built against another version.
    inline constexpr std::uint32_t PLUGIN_API_VERSION = 1;

    class IPlugin {
    public:
        virtual ~IPlugin() = default;

        virtual void onLoad()   = 0;
        virtual void onUnload() = 0;
    };

    // Describes a plugin to the runtime. The runtime allocates `Size` bytes aligned to `Align`
    // and asks the plugin to construct itself there, so the runtime owns the memory.
    struct PluginInfo {
        std::uint32_t ApiVersion;
        const char*   Name;
        std::size_t   Size;
        std::size_t   Align;
        IPlugin* (*Construct)(void* memory);
    };

    // Name of the symbol every plugin exports (see RYN_PLUGIN)
    inline constexpr const char* PLUGIN_ENTRY_SYMBOL = "rynPluginInfo";
    using PluginEntryFn                              = const PluginInfo* (*)();
}

// Place once in a plugin's .cpp to export it, e.g. RYN_PLUGIN(EditorPlugin, "Editor")
#define RYN_PLUGIN(Type, DisplayName)                                                                                  \
    static_assert(std::is_base_of_v<::ryn::IPlugin, Type>, #Type " must derive from ryn::IPlugin");                    \
    RYN_PLUGIN_API const ::ryn::PluginInfo* rynPluginInfo() {                                                          \
        static constexpr ::ryn::PluginInfo info{                                                                       \
            .ApiVersion = ::ryn::PLUGIN_API_VERSION,                                                                   \
            .Name       = DisplayName,                                                                                 \
            .Size       = sizeof(Type),                                                                                \
            .Align      = alignof(Type),                                                                               \
            .Construct  = [](void* memory) -> ::ryn::IPlugin* { return ::new (memory) Type(); },                       \
        };                                                                                                             \
        return &info;                                                                                                  \
    }
