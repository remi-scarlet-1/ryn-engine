module;
#include "Prelude.h"
#include <SDL3/SDL.h>
#include <cstdint>

export module Ryntime.Service:Window;
export import Ryntime.Core;
export import Ryntime.Core.Reflection;

export namespace ryn::service {
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

    class RYN_API Window final : public DynamicType<IService, "ryn::service::Window"> {
    public:
        explicit Window(const WindowConfig& config);
        ~Window() override;

        void onBeginFrame(const Time& time) override;

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
}

export namespace ryn::event {
    struct RYN_API WindowClose final : DynamicType<IEvent, "ryn::event::WindowClose"> {
        inline explicit WindowClose() {}
    };

    struct RYN_API WindowFramebufferResize final : DynamicType<IEvent, "ryn::event::WindowFramebufferResize"> {
        Vector2Int Resolution;
        inline explicit WindowFramebufferResize(Vector2Int resolution) : Resolution(resolution) {}
    };

    struct RYN_API WindowTextInput final : DynamicType<IEvent, "ryn::event::WindowTextInput"> {
        const char* Text;
        inline explicit WindowTextInput(const char* text) : Text(text) {}
    };

    struct RYN_API WindowKeypress final : DynamicType<IEvent, "ryn::event::WindowKeypress"> {
        Keycode Keycode;
        bool    IsDown;
        bool    IsRepeat;
        inline explicit WindowKeypress(ryn::Keycode keycode, bool isDown, bool isRepeat)
            : Keycode(keycode),
              IsDown(isDown),
              IsRepeat(isRepeat) {}
    };

    struct RYN_API WindowMouseButton final : DynamicType<IEvent, "ryn::event::WindowMouseButton"> {
        MouseButton Button;
        bool        IsDown;
        uint8_t     ClickCount;
        Vector2     ClickPosition;
        inline explicit WindowMouseButton(MouseButton button, bool isDown, uint8_t clickCount, Vector2 clickPosition)
            : Button(button),
              IsDown(isDown),
              ClickCount(clickCount),
              ClickPosition(clickPosition) {}
    };

    struct RYN_API WindowMouseMove final : DynamicType<IEvent, "ryn::event::WindowMouseMove"> {
        Vector2 Position = Vector2::zero();
        Vector2 Delta    = Vector2::zero();
        inline explicit WindowMouseMove(Vector2 position, Vector2 delta) : Position(position), Delta(delta) {}
    };

    struct RYN_API WindowMouseScroll final : DynamicType<IEvent, "ryn::event::WindowMouseScroll"> {
        Vector2 Delta = Vector2::zero();
        inline explicit WindowMouseScroll(Vector2 delta) : Delta(delta) {}
    };
}

RYN_TYPE_REFLECTION_KEEP(ryn::service::Window);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowClose);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowFramebufferResize);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowTextInput);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowKeypress);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowMouseButton);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowMouseMove);
RYN_TYPE_REFLECTION_KEEP(ryn::event::WindowMouseScroll);