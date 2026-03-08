module;
#include "Prelude.h"
#include <concepts>

export module Ryntime.Core:Event;
export import Ryntime.Core.Reflection;

export namespace ryn {
    class RYN_API IEvent : public DynamicBase<TypeCategory::Event> {
    public:
        ~IEvent() override;
        inline void consume() { m_Consumed = true; }
        inline bool isConsumed() const { return m_Consumed; }

    private:
        bool m_Consumed = false;
    };

    template<typename T>
    concept IsEvent = std::derived_from<T, IEvent>;

    class RYN_API IEventDispatcher {
    public:
        virtual ~IEventDispatcher();
        virtual void dispatchEvent(IEvent& event) = 0;
        inline void  dispatchEvent(IEvent&& event) { dispatchEvent(event); }
    };

    class EventListener {
    public:
        inline explicit EventListener(IEvent& event) : m_Event(event) {}

        template<typename T, typename Func>
        requires IsEvent<T> && IsType<T>
        inline bool match(const Func& func) {
            if (m_Event.getTypeID() == T::TYPE_ID) {
                func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

        [[nodiscard]] inline IEvent& getEvent() const { return m_Event; }

    private:
        IEvent& m_Event;
    };
}