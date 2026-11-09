module;
#include "Prelude.h"
#include <concepts>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

export module Ryntime.Core:Service;
export import Ryntime.Reflect;
export import Ryntime.Primitive;
export import Ryntime.Container;
export import :ServiceEvent;
export import :Time;
import Ryntime.Logger;

export namespace ryn {
    class IServiceLocator;

    class RYN_API IService : public DynamicBase<TypeCategory::Service> {
    public:
        ~IService() override;

    protected:
        virtual inline void onAttach([[maybe_unused]] IServiceLocator& locator) {}
        virtual inline void onDetach() {}

        virtual inline void onBeginFrame([[maybe_unused]] const Time& time) {}
        virtual inline void onUpdate([[maybe_unused]] const Time& time) {}
        virtual inline void onEndFrame([[maybe_unused]] const Time& time) {}

        virtual inline void onEvent([[maybe_unused]] EventListener& listener) {}

        inline void dispatchEvent(IEvent& event) const { m_EventDispatcher->dispatchEvent(event); }
        inline void dispatchEvent(IEvent&& event) const { m_EventDispatcher->dispatchEvent(event); }

    private:
        friend class ServiceStack;
        IEventDispatcher* m_EventDispatcher = nullptr;
    };

    template<typename T>
    concept IsService = std::derived_from<T, IService>;

    class RYN_API IServiceStack {
    public:
        virtual ~IServiceStack();

        template<typename T, typename... Args>
        requires IsService<T> && IsType<T>
        inline T* push(Args&&... args) {
            Box<IService> serviceBox = Box<T>::template make<T>(std::forward<Args>(args)...);
            IService*     service    = serviceBox.get();
            pushService(T::TYPE_ID, std::move(serviceBox));
            return static_cast<T*>(service);
        }

        inline void pop() { popService(); }

    private:
        virtual void pushService(TypeID typeId, Box<IService>&& serviceBox) = 0;
        virtual void popService()                                           = 0;
    };

    class RYN_API IServiceLocator {
    public:
        virtual ~IServiceLocator();

        template<typename T>
        requires IsService<T> && IsType<T>
        inline void require(T*& member) {
            member = static_cast<T*>(requestService(T::TYPE_ID, true));
        }

        template<typename T>
        requires IsService<T> && IsType<T>
        [[nodiscard]] inline T* require() {
            return static_cast<T*>(requestService(T::TYPE_ID, true));
        }

        template<typename T>
        requires IsService<T> && IsType<T>
        inline void optional(T*& member) {
            member = static_cast<T*>(requestService(T::TYPE_ID, false));
        }

        template<typename T>
        requires IsService<T> && IsType<T>
        [[nodiscard]] inline T* optional() {
            return static_cast<T*>(requestService(T::TYPE_ID, false));
        }

    protected:
        static inline IService* requestService(IServiceLocator& source, TypeID typeId, bool required) {
            return source.requestService(typeId, required);
        }

    private:
        virtual IService* requestService(TypeID typeId, bool required) = 0;
        virtual void      notifyMissingService(TypeID typeId)          = 0;
    };

    class RYN_API ServiceDiscovery : public IServiceLocator {
    public:
        explicit ServiceDiscovery(IServiceLocator& locator);
        [[nodiscard]] inline bool wasSuccess() const noexcept { return m_Success; }

    private:
        IServiceLocator& m_ServiceLocator;
        bool             m_Success = true;

        IService* requestService(TypeID typeId, bool required) override;
        void      notifyMissingService(TypeID typeId) override;
    };
}

namespace ryn {
    class RYN_API ServiceStack : public IServiceStack, public IServiceLocator, public IEventDispatcher {
    public:
        using EventInterceptor = std::function<void(EventListener&)>;

        ServiceStack();
        ~ServiceStack() override;

        void update();
        void dispatchEvent(IEvent& event) override;
        void setEventInterceptor(EventInterceptor interceptor);

    private:
        EventInterceptor                      m_EventInterceptor;
        std::vector<Box<IService>>            m_Services;
        std::unordered_map<TypeID, IService*> m_ServiceLUT;
        Timer                                 m_Clock;
        Time                             m_FrameTime;

        void      pushService(TypeID typeId, Box<IService>&& serviceBox) override;
        void      popService() override;
        IService* requestService(TypeID typeId, bool required) override;
        void      notifyMissingService(TypeID typeId) override;
    };
}