module;
#include "Prelude.h"

module Ryntime.World;

RYN_SCOPE_FILE("Service|World");

namespace ryn::world {
    ServiceDiscovery::ServiceDiscovery(IServiceLocator& locator) : m_ServiceLocator(locator) {}

    IService* ServiceDiscovery::requestService(TypeID typeId, bool required) {
        auto* service = IServiceLocator::requestService(m_ServiceLocator, typeId, false);

        // Handle required missing services manually
        if (required && !service) {
            m_Success = false;
            this->notifyMissingService(typeId);
        }

        return service;
    }

    void ServiceDiscovery::notifyMissingService(TypeID typeId) {
        RYN_WARN("System configuration failed. Couldn't find: {0}", TypeRegistry::queryType(typeId));
    }

    ISystem::~ISystem() = default;

    void World::onAttach(IServiceLocator& locator) { m_ServiceLocator = &locator; }

    void World::onDetach() {
        // Shutdown all systems
        for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it)
            if (it->Instance && it->IsEnabled)
                it->Instance->onDisable();

        m_Systems.clear();
        m_SystemIndexMap.clear();
    }

    void World::onUpdate(const FrameTime& time) {
        auto execute = [&](auto memberFunc) {
            for (size_t i = 0; i < m_Systems.size(); ++i) {
                auto& entry = m_Systems[i];
                if (entry.Instance && entry.IsEnabled)
                    (entry.Instance.get()->*memberFunc)(time, *this);
            }
        };

        execute(&ISystem::onBeginFrame);
        execute(&ISystem::onUpdate);
        execute(&ISystem::onEndFrame);

        // Remove any systems destroyed during this frame and fix mapping indices
        sweepSystems();
    }

    bool World::internal_systemMake(TypeID id, Box<ISystem>&& system) {
        // Avoid duplicate entries
        if (m_SystemIndexMap.contains(id)) {
            RYN_ERROR("Cannot make duplicate systems! Second instance of system {0} will be destroyed.", TypeRegistry::queryType(id));
            return false;
        }

        // Setup service discovery
        ServiceDiscovery discovery(*m_ServiceLocator);
        system->onConfigure(discovery);
        bool discoveryWasSuccessful = discovery.wasSuccess();

        // Initialize
        if (discoveryWasSuccessful) system->onEnable();

        // Add entry
        m_SystemIndexMap[id] = m_Systems.size();
        m_Systems.push_back({
            .Instance     = std::move(system),
            .CachedTypeID = id,
            .IsEnabled    = discoveryWasSuccessful,
            .IsConfigured = discoveryWasSuccessful,
        });
        return true;
    }

    bool World::internal_systemDestroy(TypeID id) {
        auto it = m_SystemIndexMap.find(id);
        if (it == m_SystemIndexMap.end()) {
            RYN_WARN("Attempt to destroy system {0} failed. No instance of it's type existed!", TypeRegistry::queryType(id));
            return false;
        }

        if (m_Systems[it->second].IsEnabled) m_Systems[it->second].Instance->onDisable();
        m_Systems[it->second].Instance.reset();
        m_SystemIndexMap.erase(it);
        return true;
    }

    bool World::internal_systemToggle(TypeID id, bool enable) {
        auto it = m_SystemIndexMap.find(id);
        if (it == m_SystemIndexMap.end()) {
            RYN_WARN("Attempt to toggle system {0} failed. No instance of it's type existed!", TypeRegistry::queryType(id));
            return false;
        }

        SystemEntry& entry = m_Systems[it->second];
        if (entry.IsConfigured != true) {
            RYN_WARN("Attempt to toggle system {0} failed. It's configuration failed and it's therefore in an invalid state!",
                     TypeRegistry::queryType(id));
            return false;
        }

        if (entry.IsEnabled == enable) return true; // Already in desired state

        entry.IsEnabled = enable;
        if (enable) entry.Instance->onEnable();
        else entry.Instance->onDisable();

        return true;
    }

    void World::sweepSystems() {
        // Erase-remove idiom for the nulls
        auto it = std::remove_if(m_Systems.begin(), m_Systems.end(), [](const auto& s) { return !s.Instance; });

        if (it != m_Systems.end()) {
            m_Systems.erase(it, m_Systems.end());
            // Re-index only if we actually removed something
            for (size_t i = 0; i < m_Systems.size(); ++i)
                m_SystemIndexMap[m_Systems[i].CachedTypeID] = i;
        }
    }
}