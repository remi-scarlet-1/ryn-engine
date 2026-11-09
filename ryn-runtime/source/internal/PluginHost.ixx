export module ryn.internal:plugin_host;

import ryn.core;
import :dynamic_library;

export namespace ryn {
    /// Loads one plugin binary, owns the memory of its root and tears both down in the right order.
    class PluginHost {
    public:
        PluginHost() = default;
        ~PluginHost();

        PluginHost(const PluginHost&)            = delete;
        PluginHost& operator=(const PluginHost&) = delete;

        /// Loads the plugin at the UTF-8 `path`, constructs its root and calls its OnLoad. Logs and returns false on failure.
        bool load(const char* path);

        /// Calls the root's OnUnload, destroys it, frees its memory and unloads the plugin binary.
        void unload();

        [[nodiscard]] bool        isLoaded() const { return m_Root != nullptr; }
        [[nodiscard]] const char* name() const { return m_Name; }

    private:
        DynamicLibrary    m_Library;
        const PluginDesc* m_Desc      = nullptr; // points into the plugin binary: only valid while it is loaded
        void*             m_Root      = nullptr;
        usize             m_RootAlign = 0;
        char              m_Name[64]  = {};      // copied, so it stays valid after the plugin is unloaded
    };
}
