module;
#include "Prelude.h"
#include <unordered_map>
#include <vector>

module Ryntime.Core;
import Ryntime.Logger;

RYN_SCOPE_FILE("ServiceStack");

namespace ryn {
    IService::~IService()               = default;
    IServiceStack::~IServiceStack()     = default;
    IServiceLocator::~IServiceLocator() = default;

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
        RYN_WARN("Service discovery failed. Couldn't find: {0}", TypeRegistry::queryType(typeId));
    }

    ServiceStack::ServiceStack() = default;
    ServiceStack::~ServiceStack() {
        RYN_INFO("Destroy Services...");
        while (!m_Services.empty())
            ServiceStack::popService();
        m_Services.clear();
        m_ServiceLUT.clear();
    }

    void ServiceStack::update() {
        m_FrameTime.Delta = m_Clock.elapsed();
        m_FrameTime.Total += m_FrameTime.Delta;
        m_FrameTime.FrameCount++;
        m_Clock.reset();

        for (const auto& serviceBox : m_Services)
            serviceBox->onBeginFrame(m_FrameTime);

        for (const auto& serviceBox : m_Services)
            serviceBox->onUpdate(m_FrameTime);

        // Reversed iteration for onEndFrame()
        for (auto it = m_Services.rbegin(); it != m_Services.rend(); ++it)
            (*it)->onEndFrame(m_FrameTime);
    }

    void ServiceStack::dispatchEvent(IEvent& event) {
        EventListener listener(event);

        // Allow event interception before propagation
        if (m_EventInterceptor) {
            m_EventInterceptor(listener);
            if (event.isConsumed()) return;
        }

        // Propagate event top to bottom through services
        for (auto it = m_Services.rbegin(); it != m_Services.rend(); ++it) {
            if (event.isConsumed()) break;
            (*it)->onEvent(listener);
        }

        // Be chatty about events that weren't consumed
        if (!event.isConsumed()) RYN_TRACE("Event '{0}' was not consumed by any layer", event.getTypeName());
    }

    void ServiceStack::setEventInterceptor(EventInterceptor interceptor) { m_EventInterceptor = std::move(interceptor); }

    void ServiceStack::pushService(TypeID typeId, Box<IService>&& serviceBox) {
        IService* service = serviceBox.get();

        if (m_ServiceLUT.contains(typeId)) {
            RYN_ERROR("Failed to push service '{0}' because an instance of it's type already exists!", service->getTypeName());
            return;
        }

        RYN_INFO("Push service '{0}'", service->getTypeName());
        m_ServiceLUT[typeId] = service;
        m_Services.push_back(std::move(serviceBox));

        service->m_EventDispatcher = this;
        service->onAttach(*this);
    }

    void ServiceStack::popService() {
        if (m_Services.empty()) return;

        auto& box     = m_Services.back();
        auto* service = box.get();

        RYN_INFO("Pop service '{0}'", service->getTypeName());
        service->onDetach();

        m_ServiceLUT.erase(service->getTypeID());
        m_Services.pop_back();
    }

    IService* ServiceStack::requestService(TypeID typeId, bool required) {
        const auto it      = m_ServiceLUT.find(typeId);
        IService*  service = it != m_ServiceLUT.end() ? it->second : nullptr;

        if (!service && required) notifyMissingService(typeId);

        return service;
    }

    void ServiceStack::notifyMissingService(TypeID typeId) {
        RYN_VERIFY(false, "Required service {0} couldn't be found!", TypeRegistry::queryType(typeId));
    }
}