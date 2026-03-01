module;
#include "Prelude.h"
#include <bitset>
#include <cstdint>

export module Ryntime.Platform:Input;
export import Ryntime.Core;
export import Ryntime.Core.Reflection;

using namespace ryn::reflection;

export namespace ryn::platform {
    class RYN_API Input final : public DynamicType<IService, "ryn::platform::Input"> {
    public:
        explicit Input();
        ~Input() override;

        void onEndFrame(const FrameTime& time) override;
        void onEvent(EventListener& listener) override;

        [[nodiscard]] bool isKeyHeld(Keycode key) const;
        [[nodiscard]] bool isKeyDown(Keycode key) const;
        [[nodiscard]] bool isKeyUp(Keycode key) const;
        [[nodiscard]] bool isMouseButtonHeld(MouseButton button) const;
        [[nodiscard]] bool isMouseButtonDown(MouseButton button) const;
        [[nodiscard]] bool isMouseButtonUp(MouseButton button) const;

        [[nodiscard]] Vector2 getMousePosition() const;
        [[nodiscard]] Vector2 getMouseDelta() const;
        [[nodiscard]] Vector2 getScrollDelta() const;

    private:
        static constexpr size_t MAX_KEYCODES      = static_cast<size_t>(Keycode::Count);
        static constexpr size_t MAX_MOUSE_BUTTONS = static_cast<size_t>(MouseButton::Count);

        std::bitset<MAX_KEYCODES>      m_Keys;
        std::bitset<MAX_KEYCODES>      m_KeysJustDown;
        std::bitset<MAX_KEYCODES>      m_KeysJustUp;
        std::bitset<MAX_MOUSE_BUTTONS> m_MouseButtons;
        std::bitset<MAX_MOUSE_BUTTONS> m_MouseButtonsJustDown;
        std::bitset<MAX_MOUSE_BUTTONS> m_MouseButtonsJustUp;

        Vector2 m_MousePosition;
        Vector2 m_MousePositionDelta;
        Vector2 m_MouseScrollDelta;
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::platform::Input);