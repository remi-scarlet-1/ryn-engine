module;
#include <cstdio>
#include <new>

module ryn.internal;

namespace ryn {
    namespace {
        constexpr const char* LOG = "PluginHost";

        // TODO(M1): replace with the compile-time log formatter. Temporary runtime formatting for M0 only.
        template<typename... Args>
        void logFormatted(LogLevel level, const char* format, Args... args) {
            char message[1024];
            std::snprintf(message, sizeof(message), format, args...);
            logMessage(level, LOG, message);
        }
    }

    PluginHost::~PluginHost() { unload(); }

    bool PluginHost::load(const char* path) {
        unload();

        char error[512];
        if (!m_Library.load(path, error, sizeof(error))) {
            logFormatted(LogLevel::Error, "Failed to load '%s': %s", path, error);
            return false;
        }

        auto getDesc = reinterpret_cast<FnPluginGetDesc>(m_Library.findSymbol(PLUGIN_GET_DESC_SYMBOL));
        if (!getDesc) {
            logFormatted(LogLevel::Error, "'%s' is not a Ryn plugin (missing '%s')", path, PLUGIN_GET_DESC_SYMBOL);
            m_Library.unload();
            return false;
        }

        const PluginDesc* desc = getDesc();
        if (!desc || desc->AbiVersion != PLUGIN_ABI_VERSION) {
            logFormatted(LogLevel::Error,
                         "'%s' was built for plugin ABI version %u, runtime expects %u",
                         path,
                         desc ? static_cast<unsigned>(desc->AbiVersion) : 0u,
                         static_cast<unsigned>(PLUGIN_ABI_VERSION));
            m_Library.unload();
            return false;
        }

        m_Desc      = desc;
        m_RootAlign = desc->RootAlign;
        std::snprintf(m_Name, sizeof(m_Name), "%s", desc->Name ? desc->Name : "<unnamed>");

        // The runtime owns the root's memory; the plugin only constructs the root into it.
        // TODO(M1): allocate from the plugin's persistent/transient roots once Ryn allocators exist (decision 9).
        m_Root = ::operator new(desc->RootSize, std::align_val_t{desc->RootAlign});
        m_Desc->FnConstruct(m_Root);

        logFormatted(LogLevel::Info, "Loaded plugin '%s' from '%s'", m_Name, path);
        m_Desc->OnLoad(m_Root);
        return true;
    }

    void PluginHost::unload() {
        if (m_Root) {
            logFormatted(LogLevel::Info, "Unloading plugin '%s'", m_Name);
            // Both run code inside the plugin, so they must happen before the binary is unloaded
            m_Desc->OnUnload(m_Root);
            m_Desc->FnDestruct(m_Root);
            ::operator delete(m_Root, std::align_val_t{m_RootAlign});
        }

        m_Root      = nullptr;
        m_Desc      = nullptr;
        m_RootAlign = 0;
        m_Library.unload();
    }
}
