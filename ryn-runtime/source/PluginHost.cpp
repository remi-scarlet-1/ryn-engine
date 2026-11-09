#include "PluginHost.h"

#include <new>
#include <string>

#include "ryn/Log.h"

namespace ryn {
    namespace {
        constexpr std::string_view LOG = "PluginHost";
    }

    PluginHost::~PluginHost() { unload(); }

    bool PluginHost::load(const std::filesystem::path& path) {
        unload();

        std::string error;
        if (!m_Library.load(path, error)) {
            log(LogLevel::Error, LOG, "Failed to load '{}': {}", path.string(), error);
            return false;
        }

        auto entry = reinterpret_cast<PluginEntryFn>(m_Library.findSymbol(PLUGIN_ENTRY_SYMBOL));
        if (!entry) {
            log(LogLevel::Error, LOG, "'{}' is not a Ryn plugin (missing '{}')", path.string(), PLUGIN_ENTRY_SYMBOL);
            m_Library.unload();
            return false;
        }

        const PluginInfo* info = entry();
        if (!info || info->ApiVersion != PLUGIN_API_VERSION) {
            log(LogLevel::Error, LOG, "'{}' was built for plugin API {}, runtime expects {}", path.string(),
                info ? info->ApiVersion : 0u, PLUGIN_API_VERSION);
            m_Library.unload();
            return false;
        }

        // The runtime owns the plugin's memory; the plugin only constructs itself into it
        m_Info   = info;
        m_Memory = ::operator new(info->Size, std::align_val_t{info->Align});
        m_Plugin = info->Construct(m_Memory);

        log(LogLevel::Info, LOG, "Loaded plugin '{}' from '{}'", info->Name, path.string());
        m_Plugin->onLoad();
        return true;
    }

    void PluginHost::unload() {
        if (m_Plugin) {
            log(LogLevel::Info, LOG, "Unloading plugin '{}'", m_Info->Name);
            m_Plugin->onUnload();
            // Destructor code lives in the plugin library, so it must run before the library is unloaded
            m_Plugin->~IPlugin();
            ::operator delete(m_Memory, std::align_val_t{m_Info->Align});
        }

        m_Plugin = nullptr;
        m_Memory = nullptr;
        m_Info   = nullptr;
        m_Library.unload();
    }
}
