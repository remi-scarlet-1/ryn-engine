module;
#include "Prelude.h"
#include <SDL3/SDL.h>
#include <array>
#include <string_view>

module Ryntime.Service;

RYN_SCOPE_FILE("Service|Window");

namespace {
    constexpr ryn::Keycode mapSDLScancodeToKeycode(SDL_Scancode scancode);
}

namespace ryn::service {
    Window::Window(const WindowConfig& config) {
        if (!SDL_WasInit(SDL_INIT_VIDEO)) {
            RYN_TRACE("Initialize SDL video layer...");
            RYN_VERIFY(SDL_InitSubSystem(SDL_INIT_VIDEO), "Failed to initialize SDL video subsystem: {0}", SDL_GetError());
        }

        SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
        if (config.IsFullscreen) flags |= SDL_WINDOW_FULLSCREEN;
        if (config.IsResizable) flags |= SDL_WINDOW_RESIZABLE;
        if (!config.IsVisible) flags |= SDL_WINDOW_HIDDEN;

        RYN_TRACE("Create window '{0}'...", config.Title);
        m_Window = SDL_CreateWindow(config.Title, config.Resolution.x, config.Resolution.y, flags);

        RYN_VERIFY(m_Window, "Failed to create window '{0}': {1}", config.Title, SDL_GetError());
        RYN_INFO("Successfully created window '{0}' ({1}x{2})", config.Title, config.Resolution.x, config.Resolution.y);

        resolveHandles(config.Title);
    }

    Window::~Window() {
        // ReSharper disable once CppDFAConstantConditions
        // ReSharper disable once CppDFAUnreachableCode
        if (m_Window != nullptr) {
            RYN_TRACE("Destroy window '{0}'...", getTitle());
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
    }
    void Window::onBeginFrame([[maybe_unused]] const FrameTime& time) {
        const Vector2Int clientSize      = getClientSize();
        const Vector2Int framebufferSize = getFramebufferSize();

        // Calculate DPI factor
        Vector2 dpiFactor = Vector2::one();
        if (clientSize.x > 0 && clientSize.y > 0) {
            dpiFactor.x = static_cast<float>(framebufferSize.x) / static_cast<float>(clientSize.x);
            dpiFactor.y = static_cast<float>(framebufferSize.y) / static_cast<float>(clientSize.y);
        }

        // Toggle OS text input
        const bool isPollingText = SDL_TextInputActive(m_Window);
        if (m_WantsTextInput && !isPollingText) SDL_StartTextInput(m_Window);
        else if (!m_WantsTextInput && isPollingText) SDL_StopTextInput(m_Window);
        m_WantsTextInput = false;

        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                    // clang-format off
                case SDL_EVENT_QUIT: {
                    RYN_TRACE("Close requested by operating system");
                    m_IsClosing = true;
                    dispatchEvent(event::WindowClose());
                    break;
                }
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                    if (event.window.windowID == SDL_GetWindowID(m_Window)) {
                        RYN_TRACE("Close requested by user");
                        m_IsClosing = true;
                        dispatchEvent(event::WindowClose());
                    }
                    break;
                }
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                    RYN_TRACE("Resize detected {0}x{1}", event.window.data1, event.window.data2);
                    dispatchEvent(event::WindowFramebufferResize(
                        Vector2Int(event.window.data1, event.window.data2)
                    ));
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    dispatchEvent(event::WindowTextInput(event.text.text));
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP: {
                    dispatchEvent(event::WindowKeypress(
                        mapSDLScancodeToKeycode(event.key.scancode),
                        event.type == SDL_EVENT_KEY_DOWN,
                        event.key.repeat
                    ));
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    dispatchEvent(event::WindowMouseButton(
                        static_cast<MouseButton>(event.button.button),
                        event.type == SDL_EVENT_MOUSE_BUTTON_DOWN,
                        event.button.clicks,
                        Vector2(event.button.x * dpiFactor.x, event.button.y * dpiFactor.y)
                    ));
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION: {
                    dispatchEvent(event::WindowMouseMove(
                        Vector2(event.motion.x * dpiFactor.x, event.motion.y * dpiFactor.y),
                        Vector2(event.motion.xrel * dpiFactor.x, event.motion.yrel * dpiFactor.y)
                    ));
                    break;
                }
                case SDL_EVENT_MOUSE_WHEEL: {
                    Vector2 delta(event.wheel.x, event.wheel.y);
                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                        delta.x = -delta.x;
                    dispatchEvent(event::WindowMouseScroll(delta));
                    break;
                }
                default: break;
                    // clang-format on
            }
        }
    }

    void Window::center() { SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); }

    void Window::maintainTextInput() { m_WantsTextInput = true; }

    WindowHandles Window::getHandles() const { return m_CachedHandles; }

    Vector2Int Window::getFramebufferSize() const {
        Vector2Int result;
        SDL_GetWindowSizeInPixels(m_Window, &result.x, &result.y);
        return result;
    }

    Vector2Int Window::getClientSize() const {
        Vector2Int result;
        SDL_GetWindowSize(m_Window, &result.x, &result.y);
        return result;
    }

    Vector2Int Window::getClientPosition() const {
        Vector2Int result;
        SDL_GetWindowPosition(m_Window, &result.x, &result.y);
        return result;
    }

    Vector2Int Window::getDisplaySize() const {
        const SDL_DisplayID    displayID   = SDL_GetDisplayForWindow(m_Window);
        const SDL_DisplayMode* displayMode = displayID != 0 ? SDL_GetCurrentDisplayMode(displayID) : nullptr;

        Vector2Int result = Vector2Int::zero();
        if (displayMode) {
            result.x = displayMode->w;
            result.y = displayMode->h;
        }
        return result;
    }

    const char* Window::getTitle() const { return SDL_GetWindowTitle(m_Window); }

    bool Window::isFullscreen() const { return (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN) != 0; }

    bool Window::isResizable() const { return (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_RESIZABLE) != 0; }

    bool Window::isVisible() const { return (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_HIDDEN) == 0; }

    // ReSharper disable once CppDFAConstantFunctionResult
    bool Window::isClosing() const { return m_IsClosing; }

    void Window::setClientSize(Vector2Int size) { SDL_SetWindowSize(m_Window, size.x, size.y); }

    void Window::setClientPosition(Vector2Int position) { SDL_SetWindowPosition(m_Window, position.x, position.y); }

    void Window::setTitle(const char* title) { SDL_SetWindowTitle(m_Window, title); }

    void Window::setFullscreen(bool enabled) { SDL_SetWindowFullscreen(m_Window, enabled); }

    void Window::setResizable(bool enabled) { SDL_SetWindowResizable(m_Window, enabled); }

    void Window::setVisible(bool enabled) { (enabled ? SDL_ShowWindow : SDL_HideWindow)(m_Window); }

    void Window::resolveHandles(const char* debugTitle) {
        // Get cstyle string driver name
        const char* driver = SDL_GetCurrentVideoDriver();

        // IIFE (subroutine for discovering native handles)
        m_CachedHandles = [this, driver] {
            WindowHandles handles{.NativeType = WindowNativeType::Undefined, .NativeHandle = nullptr, .NativeDisplayHandle = nullptr};

            if (!m_Window || !driver) return handles;

            const SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
            const std::string_view driverName{driver};

#if defined(RYN_PLATFORM_LINUX)
            if (driverName == "wayland") {
                void* surface = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
                void* display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
                if (surface && display) {
                    handles.NativeType          = WindowNativeType::Wayland;
                    handles.NativeHandle        = surface;
                    handles.NativeDisplayHandle = display;
                }
            } else if (driverName == "x11") {
                const Sint64 windowID = SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
                void*        display  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
                if (windowID != 0 && display) {
                    handles.NativeType          = WindowNativeType::X11;
                    handles.NativeHandle        = reinterpret_cast<void*>(static_cast<uintptr_t>(windowID));
                    handles.NativeDisplayHandle = display;
                }
            }

#elif defined(RYN_PLATFORM_WIN)
            if (driverName == "windows") {
                void* hwnd = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                if (hwnd) {
                    handles.NativeType   = WindowNativeType::Win32;
                    handles.NativeHandle = hwnd;
                }
            }
#endif
            return handles;
        }();

        // Warn in case native handle discovery failed
        if (m_CachedHandles.NativeType == WindowNativeType::Undefined)
            RYN_WARN("Couldn't determine native handles for '{0}' (Driver: '{1}')", debugTitle, driver ? driver : "NULL");
    }
}

namespace {
    using ScancodeLUT = std::array<ryn::Keycode, static_cast<size_t>(SDL_SCANCODE_COUNT)>;

    constexpr ScancodeLUT s_ScancodeLUT = [] {
        ScancodeLUT lut;
        lut.fill(static_cast<ScancodeLUT::value_type>(SDL_SCANCODE_UNKNOWN));
#define MAP(sdlKey, keycode) lut[sdlKey] = keycode;
        /* Alphabetic */
        MAP(SDL_SCANCODE_A, ryn::Keycode::A)
        MAP(SDL_SCANCODE_B, ryn::Keycode::B)
        MAP(SDL_SCANCODE_C, ryn::Keycode::C)
        MAP(SDL_SCANCODE_D, ryn::Keycode::D)
        MAP(SDL_SCANCODE_E, ryn::Keycode::E)
        MAP(SDL_SCANCODE_F, ryn::Keycode::F)
        MAP(SDL_SCANCODE_G, ryn::Keycode::G)
        MAP(SDL_SCANCODE_H, ryn::Keycode::H)
        MAP(SDL_SCANCODE_I, ryn::Keycode::I)
        MAP(SDL_SCANCODE_J, ryn::Keycode::J)
        MAP(SDL_SCANCODE_K, ryn::Keycode::K)
        MAP(SDL_SCANCODE_L, ryn::Keycode::L)
        MAP(SDL_SCANCODE_M, ryn::Keycode::M)
        MAP(SDL_SCANCODE_N, ryn::Keycode::N)
        MAP(SDL_SCANCODE_O, ryn::Keycode::O)
        MAP(SDL_SCANCODE_P, ryn::Keycode::P)
        MAP(SDL_SCANCODE_Q, ryn::Keycode::Q)
        MAP(SDL_SCANCODE_R, ryn::Keycode::R)
        MAP(SDL_SCANCODE_S, ryn::Keycode::S)
        MAP(SDL_SCANCODE_T, ryn::Keycode::T)
        MAP(SDL_SCANCODE_U, ryn::Keycode::U)
        MAP(SDL_SCANCODE_V, ryn::Keycode::V)
        MAP(SDL_SCANCODE_W, ryn::Keycode::W)
        MAP(SDL_SCANCODE_X, ryn::Keycode::X)
        MAP(SDL_SCANCODE_Y, ryn::Keycode::Y)
        MAP(SDL_SCANCODE_Z, ryn::Keycode::Z)
        /* Numeric (Top Row) */
        MAP(SDL_SCANCODE_1, ryn::Keycode::D1)
        MAP(SDL_SCANCODE_2, ryn::Keycode::D2)
        MAP(SDL_SCANCODE_3, ryn::Keycode::D3)
        MAP(SDL_SCANCODE_4, ryn::Keycode::D4)
        MAP(SDL_SCANCODE_5, ryn::Keycode::D5)
        MAP(SDL_SCANCODE_6, ryn::Keycode::D6)
        MAP(SDL_SCANCODE_7, ryn::Keycode::D7)
        MAP(SDL_SCANCODE_8, ryn::Keycode::D8)
        MAP(SDL_SCANCODE_9, ryn::Keycode::D9)
        MAP(SDL_SCANCODE_0, ryn::Keycode::D0)
        /* Function */
        MAP(SDL_SCANCODE_F1, ryn::Keycode::F1)
        MAP(SDL_SCANCODE_F2, ryn::Keycode::F2)
        MAP(SDL_SCANCODE_F3, ryn::Keycode::F3)
        MAP(SDL_SCANCODE_F4, ryn::Keycode::F4)
        MAP(SDL_SCANCODE_F5, ryn::Keycode::F5)
        MAP(SDL_SCANCODE_F6, ryn::Keycode::F6)
        MAP(SDL_SCANCODE_F7, ryn::Keycode::F7)
        MAP(SDL_SCANCODE_F8, ryn::Keycode::F8)
        MAP(SDL_SCANCODE_F9, ryn::Keycode::F9)
        MAP(SDL_SCANCODE_F10, ryn::Keycode::F10)
        MAP(SDL_SCANCODE_F11, ryn::Keycode::F11)
        MAP(SDL_SCANCODE_F12, ryn::Keycode::F12)
        /* Arrow */
        MAP(SDL_SCANCODE_UP, ryn::Keycode::ArrowUp)
        MAP(SDL_SCANCODE_DOWN, ryn::Keycode::ArrowDown)
        MAP(SDL_SCANCODE_LEFT, ryn::Keycode::ArrowLeft)
        MAP(SDL_SCANCODE_RIGHT, ryn::Keycode::ArrowRight)
        /* Control, Navigation & Special */
        MAP(SDL_SCANCODE_ESCAPE, ryn::Keycode::Escape)
        MAP(SDL_SCANCODE_TAB, ryn::Keycode::Tab)
        MAP(SDL_SCANCODE_CAPSLOCK, ryn::Keycode::CapsLock)
        MAP(SDL_SCANCODE_SPACE, ryn::Keycode::Space)
        MAP(SDL_SCANCODE_RETURN, ryn::Keycode::Return)
        MAP(SDL_SCANCODE_BACKSLASH, ryn::Keycode::Backslash)
        MAP(SDL_SCANCODE_BACKSPACE, ryn::Keycode::Backspace)
        MAP(SDL_SCANCODE_MINUS, ryn::Keycode::Minus)
        MAP(SDL_SCANCODE_EQUALS, ryn::Keycode::Equals)
        MAP(SDL_SCANCODE_LEFTBRACKET, ryn::Keycode::LBracket)
        MAP(SDL_SCANCODE_RIGHTBRACKET, ryn::Keycode::RBracket)
        MAP(SDL_SCANCODE_SEMICOLON, ryn::Keycode::Semicolon)
        MAP(SDL_SCANCODE_APOSTROPHE, ryn::Keycode::Apostrophe)
        MAP(SDL_SCANCODE_COMMA, ryn::Keycode::Comma)
        MAP(SDL_SCANCODE_PERIOD, ryn::Keycode::Period)
        MAP(SDL_SCANCODE_SLASH, ryn::Keycode::Slash)
        MAP(SDL_SCANCODE_INSERT, ryn::Keycode::Insert)
        MAP(SDL_SCANCODE_DELETE, ryn::Keycode::Delete)
        MAP(SDL_SCANCODE_PAGEUP, ryn::Keycode::PageUp)
        MAP(SDL_SCANCODE_PAGEDOWN, ryn::Keycode::PageDown)
        MAP(SDL_SCANCODE_HOME, ryn::Keycode::Home)
        MAP(SDL_SCANCODE_PAUSE, ryn::Keycode::Pause)
        MAP(SDL_SCANCODE_END, ryn::Keycode::End)
        MAP(SDL_SCANCODE_SCROLLLOCK, ryn::Keycode::ScrollLock)
        MAP(SDL_SCANCODE_PRINTSCREEN, ryn::Keycode::PrintScreen)
        MAP(SDL_SCANCODE_NUMLOCKCLEAR, ryn::Keycode::NumLock)
        /* Keypad */
        MAP(SDL_SCANCODE_KP_PERIOD, ryn::Keycode::KP_Period)
        MAP(SDL_SCANCODE_KP_DIVIDE, ryn::Keycode::KP_Divide)
        MAP(SDL_SCANCODE_KP_MULTIPLY, ryn::Keycode::KP_Multiply)
        MAP(SDL_SCANCODE_KP_MINUS, ryn::Keycode::KP_Minus)
        MAP(SDL_SCANCODE_KP_PLUS, ryn::Keycode::KP_Plus)
        MAP(SDL_SCANCODE_KP_ENTER, ryn::Keycode::KP_Enter)
        MAP(SDL_SCANCODE_KP_1, ryn::Keycode::KP_1)
        MAP(SDL_SCANCODE_KP_2, ryn::Keycode::KP_2)
        MAP(SDL_SCANCODE_KP_3, ryn::Keycode::KP_3)
        MAP(SDL_SCANCODE_KP_4, ryn::Keycode::KP_4)
        MAP(SDL_SCANCODE_KP_5, ryn::Keycode::KP_5)
        MAP(SDL_SCANCODE_KP_6, ryn::Keycode::KP_6)
        MAP(SDL_SCANCODE_KP_7, ryn::Keycode::KP_7)
        MAP(SDL_SCANCODE_KP_8, ryn::Keycode::KP_8)
        MAP(SDL_SCANCODE_KP_9, ryn::Keycode::KP_9)
        MAP(SDL_SCANCODE_KP_0, ryn::Keycode::KP_0)
        /* Modifiers */
        MAP(SDL_SCANCODE_LCTRL, ryn::Keycode::LCtrl)
        MAP(SDL_SCANCODE_LSHIFT, ryn::Keycode::LShift)
        MAP(SDL_SCANCODE_LALT, ryn::Keycode::LAlt)
        MAP(SDL_SCANCODE_LGUI, ryn::Keycode::LSuper)
        MAP(SDL_SCANCODE_RCTRL, ryn::Keycode::RCtrl)
        MAP(SDL_SCANCODE_RSHIFT, ryn::Keycode::RShift)
        MAP(SDL_SCANCODE_RALT, ryn::Keycode::RAlt)
        MAP(SDL_SCANCODE_RGUI, ryn::Keycode::RSuper)
#undef MAP
        return lut;
    }();

    constexpr ryn::Keycode mapSDLScancodeToKeycode(const SDL_Scancode scancode) {
        const auto value = static_cast<size_t>(scancode);
        if (value < s_ScancodeLUT.size()) return s_ScancodeLUT[value];
        return ryn::Keycode::Unknown;
    }
}