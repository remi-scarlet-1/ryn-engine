module;
#include "Prelude.h"
#include <SDL3/SDL.h>
#include <cstdint>

export module Ryntime.Platform:Window;
export import Ryntime.Core;
export import Ryntime.Core.Reflection;

using namespace ryn::reflection;

export namespace ryn::platform {
    enum class WindowNativeType : uint64_t {
        Undefined = 0,
        Wayland,
        X11,
        Win32
    };

    struct WindowHandles {
        WindowNativeType NativeType          = WindowNativeType::Undefined;
        void*            NativeHandle        = nullptr;
        void*            NativeDisplayHandle = nullptr;
    };

    struct WindowConfig {
        const char* Title        = "RynWindow";
        Vector2Int  Resolution   = {1920, 1080};
        bool        IsFullscreen = false;
        bool        IsResizable  = true;
        bool        IsVisible    = true;
    };

    class RYN_API Window final : public DynamicType<IService, "ryn::platform::Window"> {
    public:
        explicit Window(const WindowConfig& config);
        ~Window() override;

        void onBeginFrame(const FrameTime& time) override;

        void center();
        void maintainTextInput();

        [[nodiscard]] WindowHandles getHandles() const;
        [[nodiscard]] Vector2Int    getFramebufferSize() const;
        [[nodiscard]] Vector2Int    getClientSize() const;
        [[nodiscard]] Vector2Int    getClientPosition() const;
        [[nodiscard]] Vector2Int    getDisplaySize() const;
        [[nodiscard]] const char*   getTitle() const;
        [[nodiscard]] bool          isFullscreen() const;
        [[nodiscard]] bool          isResizable() const;
        [[nodiscard]] bool          isVisible() const;
        [[nodiscard]] bool          isClosing() const;

        void setClientSize(Vector2Int size);
        void setClientPosition(Vector2Int position);
        void setTitle(const char* title);
        void setFullscreen(bool enabled);
        void setResizable(bool enabled);
        void setVisible(bool enabled);

    private:
        SDL_Window*   m_Window = nullptr;
        WindowHandles m_CachedHandles;
        bool          m_IsClosing      = false;
        bool          m_WantsTextInput = false;

        void resolveHandles(const char* debugTitle);
    };

    struct RYN_API WindowCloseEvent final : DynamicType<IEvent, "ryn::platform::WindowCloseEvent"> {
        inline explicit WindowCloseEvent() {}
    };

    struct RYN_API WindowFramebufferResizeEvent final : DynamicType<IEvent, "ryn::platform::WindowFramebufferResizeEvent"> {
        Vector2Int Resolution;
        inline explicit WindowFramebufferResizeEvent(Vector2Int resolution) : Resolution(resolution) {}
    };

    struct RYN_API WindowTextInputEvent final : DynamicType<IEvent, "ryn::platform::WindowTextInputEvent"> {
        const char* Text;
        inline explicit WindowTextInputEvent(const char* text) : Text(text) {}
    };

    struct RYN_API WindowKeypressEvent final : DynamicType<IEvent, "ryn::platform::WindowKeypressEvent"> {
        Keycode Keycode;
        bool    IsDown;
        bool    IsRepeat;
        inline explicit WindowKeypressEvent(ryn::Keycode keycode, bool isDown, bool isRepeat)
            : Keycode(keycode),
              IsDown(isDown),
              IsRepeat(isRepeat) {}
    };

    struct RYN_API WindowMouseButtonEvent final : DynamicType<IEvent, "ryn::platform::WindowMouseButtonEvent"> {
        MouseButton Button;
        bool        IsDown;
        uint8_t     ClickCount;
        Vector2     ClickPosition;
        inline explicit WindowMouseButtonEvent(MouseButton button, bool isDown, uint8_t clickCount, Vector2 clickPosition)
            : Button(button),
              IsDown(isDown),
              ClickCount(clickCount),
              ClickPosition(clickPosition) {}
    };

    struct RYN_API WindowMouseMoveEvent final : DynamicType<IEvent, "ryn::platform::WindowMouseMoveEvent"> {
        Vector2 Position = Vector2::zero();
        Vector2 Delta    = Vector2::zero();
        inline explicit WindowMouseMoveEvent(Vector2 position, Vector2 delta) : Position(position), Delta(delta) {}
    };

    struct RYN_API WindowMouseScrollEvent final : DynamicType<IEvent, "ryn::platform::WindowMouseScrollEvent"> {
        Vector2 Delta = Vector2::zero();
        inline explicit WindowMouseScrollEvent(Vector2 delta) : Delta(delta) {}
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::platform::Window);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowCloseEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowFramebufferResizeEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowTextInputEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowKeypressEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowMouseButtonEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowMouseMoveEvent);
RYN_TYPE_REFLECTION_KEEP(ryn::platform::WindowMouseScrollEvent);