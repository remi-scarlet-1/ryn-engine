#pragma once

#include <filesystem>

#include "DynamicLibrary.h"
#include "ryn/Plugin.h"

namespace ryn {
    // Loads one plugin library, owns the memory of its IPlugin instance and tears both down in the right order.
    class PluginHost {
    public:
        PluginHost() = default;
        ~PluginHost();

        PluginHost(const PluginHost&)            = delete;
        PluginHost& operator=(const PluginHost&) = delete;

        bool load(const std::filesystem::path& path);
        void unload();

        [[nodiscard]] IPlugin*          get() const { return m_Plugin; }
        [[nodiscard]] const PluginInfo* info() const { return m_Info; }

    private:
        DynamicLibrary    m_Library;
        const PluginInfo* m_Info    = nullptr;
        void*             m_Memory  = nullptr;
        IPlugin*          m_Plugin  = nullptr;
    };
}
