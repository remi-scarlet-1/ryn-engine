module;
#include "Prelude.h"
#include <vector>

module Ryntime.World;

RYN_SCOPE_FILE("Service|World");

namespace ryn::world {
    ServiceDiscovery::ServiceDiscovery(IServiceLocator& locator) : m_Locator(locator) {}

    IService* ServiceDiscovery::requestService(TypeID typeId, bool required) {
        auto* service = IServiceLocator::requestService(m_Locator, typeId, false);

        if (required && !service) {
            m_Success = false;
            this->notifyMissingService(typeId);
        }

        return service;
    }

    void ServiceDiscovery::notifyMissingService(TypeID typeId) {
        RYN_WARN("Processor service discovery failed. Couldn't find: {0}", TypeRegistry::queryType(typeId));
    }

    IProcessor::~IProcessor() = default;

    void World::onAttach(IServiceLocator& locator) { m_ServiceLocator = &locator; }

    void World::onDetach() {
        for (auto& processor : m_EnabledProcessors)
            processor->onDisable();

        m_EnabledProcessors.clear();
        m_DisabledProcessors.clear();
        m_ProcessorLUT.clear();
    }

    void World::onUpdate(const FrameTime& time) {
        for (auto& processor : m_EnabledProcessors)
            processor->onBeginFrame(time);

        for (auto& processor : m_EnabledProcessors)
            processor->onUpdate(time);

        for (auto& processor : m_EnabledProcessors)
            processor->onEndFrame(time);
    }

    bool World::makeProcessor(TypeID id, Box<IProcessor>&& processor) {
        // Check for duplicates
        if (m_ProcessorLUT.contains(id)) {
            RYN_WARN("Processor {0} did already exists in World! Ignoring create request...", TypeRegistry::queryType(id));
            return false;
        }

        // Configure processor
        IProcessor* ptr = processor.get();
        ptr->m_World    = this;

        ServiceDiscovery serviceDiscovery(*m_ServiceLocator);
        processor->onConfigure(serviceDiscovery);
        bool success = serviceDiscovery.wasSuccessful();

        // Register processor based on discovery results
        if (success) {
            ptr->onEnable();
            m_EnabledProcessors.push_back(std::move(processor));
            m_ProcessorLUT[id] = ProcessorDetails{.Instance = ptr, .IsEnabled = true};

            RYN_TRACE("Processor {0} created and configured successfully.", TypeRegistry::queryType(id));
        } else {
            m_DisabledProcessors.push_back(std::move(processor));
            m_ProcessorLUT[id] = ProcessorDetails{.Instance = ptr, .IsEnabled = false};

            RYN_WARN("Processor {0} couldn't be configured successfully. It will be disabled by default!", TypeRegistry::queryType(id));
        }

        return true;
    }

    bool World::destroyProcessor(TypeID id) {
        // Locate processor metadata
        auto it = m_ProcessorLUT.find(id);
        if (it == m_ProcessorLUT.end()) return false;

        ProcessorDetails& meta = it->second;
        IProcessor*       ptr  = meta.Instance;

        // Vector removal lambda
        auto removeFromVector = [ptr](std::vector<Box<IProcessor>>& vec) -> bool {
            auto vecIt = std::ranges::find_if(vec, [ptr](const auto& box) { return box.get() == ptr; });

            if (vecIt != vec.end()) {
                // Swap-and-Pop for O(1) removal (ignoring search time)
                // Since Box is move-only, we swap the pointers.
                if (vec.size() > 1) *vecIt = std::move(vec.back());

                vec.pop_back();
                return true;
            }

            return false;
        };

        // Remove from appropriate vector and shutdown
        if (meta.IsEnabled) {
            ptr->onDisable();
            const bool removed = removeFromVector(m_EnabledProcessors);
            RYN_ASSERT(removed, "Processor LUT state desync: Enabled processor not found in vector.");
        } else {
            const bool removed = removeFromVector(m_DisabledProcessors);
            RYN_ASSERT(removed, "Processor LUT state desync: Disabled processor not found in vector.");
        }

        // Cleanup LUT entry
        m_ProcessorLUT.erase(it);

        RYN_TRACE("Processor {0} destroyed.", TypeRegistry::queryType(id));
        return true;
    }

    bool World::toggleProcessor(TypeID id, bool enable) {
        // Locate processor metadata
        auto it = m_ProcessorLUT.find(id);
        if (it == m_ProcessorLUT.end()) {
            RYN_WARN("Attempted to enable/disable non-existant processor: {0}", TypeRegistry::queryType(id));
            return false;
        }

        ProcessorDetails& meta = it->second;
        if (meta.IsEnabled == enable) return true; // Check if already in desired state
        IProcessor* ptr = meta.Instance;

        // Vector migration lambda
        auto migrate = [ptr](std::vector<Box<IProcessor>>& source, std::vector<Box<IProcessor>>& destination) {
            auto vecIt = std::ranges::find_if(source, [ptr](const auto& box) { return box.get() == ptr; });

            if (vecIt != source.end()) {
                // Move ownership to the destination vector first
                destination.push_back(std::move(*vecIt));

                // Fill the hole in the source vector with the last element
                if (source.size() > 1) *vecIt = std::move(source.back());

                source.pop_back();
                return true;
            }
            return false;
        };

        // State transition
        if (enable) {
            const bool moved = migrate(m_DisabledProcessors, m_EnabledProcessors);
            RYN_ASSERT(moved, "Processor LUT state desync: Processor marked as disabled not found in vector.");

            ptr->onEnable();
            RYN_TRACE("Processor {0} enabled.", TypeRegistry::queryType(id));
        } else {
            ptr->onDisable();
            RYN_TRACE("Processor {0} disabled.", TypeRegistry::queryType(id));

            const bool moved = migrate(m_EnabledProcessors, m_DisabledProcessors);
            RYN_ASSERT(moved, "Processor LUT state desync: Processor marked as enabled not found in vector.");
        }

        meta.IsEnabled = enable;
        return true;
    }
}