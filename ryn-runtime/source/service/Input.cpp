module;
#include "Prelude.h"

module Ryntime.Service;

RYN_SCOPE_FILE("Service|Input");

namespace ryn::service {
    Input::Input()  = default;
    Input::~Input() = default;

    void Input::onEndFrame([[maybe_unused]] const Time& time) {
        // Clear the "just" states
        m_KeysJustDown.reset();
        m_KeysJustUp.reset();
        m_MouseButtonsJustDown.reset();
        m_MouseButtonsJustUp.reset();

        // Clear deltas
        m_MousePositionDelta = Vector2::zero();
        m_MouseScrollDelta   = Vector2::zero();
    }

    void Input::onEvent(EventListener& listener) {
        listener.match<event::WindowKeypress>([this](event::WindowKeypress& e) {
            const auto index = static_cast<size_t>(e.Keycode);
            if (index < MAX_KEYCODES) {
                m_Keys.set(index, e.IsDown);
                if (!e.IsRepeat) (e.IsDown ? m_KeysJustDown : m_KeysJustUp).set(index, true);
            }
            e.consume();
        });

        listener.match<event::WindowMouseButton>([this](event::WindowMouseButton& e) {
            const auto index = static_cast<size_t>(e.Button);
            m_MouseButtons.set(index, e.IsDown);
            (e.IsDown ? m_MouseButtonsJustDown : m_MouseButtonsJustUp).set(index, true);
            e.consume();
        });

        listener.match<event::WindowMouseMove>([this](event::WindowMouseMove& e) {
            m_MousePosition = e.Position;
            m_MousePositionDelta += e.Delta;
            e.consume();
        });

        listener.match<event::WindowMouseScroll>([this](event::WindowMouseScroll& e) {
            m_MouseScrollDelta += e.Delta;
            e.consume();
        });
    }

    bool Input::isKeyHeld(Keycode key) const {
        const auto index = static_cast<size_t>(key);
        RYN_ASSERT(index < MAX_KEYCODES, "Keycode '{0}' is out of m_Keys's bounds!", index);
        return m_Keys.test(index);
    }

    bool Input::isKeyDown(Keycode key) const {
        const auto index = static_cast<size_t>(key);
        RYN_ASSERT(index < MAX_KEYCODES, "Keycode '{0}' is out of m_KeysJustDown's bounds!", index);
        return m_KeysJustDown.test(index);
    }

    bool Input::isKeyUp(Keycode key) const {
        const auto index = static_cast<size_t>(key);
        RYN_ASSERT(index < MAX_KEYCODES, "Keycode '{0}' is out of m_KeysJustUp's bounds!", index);
        return m_KeysJustUp.test(index);
    }

    bool Input::isMouseButtonHeld(MouseButton button) const {
        const auto index = static_cast<size_t>(button);
        RYN_ASSERT(index < MAX_MOUSE_BUTTONS, "MouseButton '{0}' is out of m_MouseButtons's bounds!", index);
        return m_MouseButtons.test(index);
    }

    bool Input::isMouseButtonDown(MouseButton button) const {
        const auto index = static_cast<size_t>(button);
        RYN_ASSERT(index < MAX_MOUSE_BUTTONS, "MouseButton '{0}' is out of m_MouseButtonDown's bounds!", index);
        return m_MouseButtonsJustDown.test(index);
    }

    bool Input::isMouseButtonUp(MouseButton button) const {
        const auto index = static_cast<size_t>(button);
        RYN_ASSERT(index < MAX_MOUSE_BUTTONS, "MouseButton '{0}' is out of m_MouseButtonUp's bounds!", index);
        return m_MouseButtonsJustUp.test(index);
    }

    Vector2 Input::getMousePosition() const { return m_MousePosition; }
    Vector2 Input::getMouseDelta() const { return m_MousePositionDelta; }
    Vector2 Input::getScrollDelta() const { return m_MouseScrollDelta; }
}