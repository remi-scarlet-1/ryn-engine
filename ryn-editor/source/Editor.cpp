module;
#include "Prelude.h"
#include "other/FontEmbeddings.inl"
#include <algorithm>
#include <cmath>
#include <imgui.h>
#include <memory>

module Ryndition;
import Ryndition.Panel;

RYN_SCOPE_FILE("Service|Editor");

namespace {
    constexpr ImGuiKey mapKeycodeToImGuiKey(ryn::Keycode keycode);
}

namespace rynd {
    void Editor::onAttach(ryn::IServiceLocator& locator) {
        locator.require(m_WindowService);
        locator.require(m_GraphicsService);

        // Create ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io    = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Upload font
        io.Fonts->Clear();

        ImFontConfig fontConfig;
        fontConfig.PixelSnapH  = true;
        fontConfig.OversampleH = 2;
        fontConfig.OversampleV = 1;

        m_Font = io.Fonts->AddFontFromMemoryCompressedTTF(other::JETBRAIN_MONO_COMPRESSED_DATA,
                                                                 other::JETBRAIN_MONO_COMPRESSED_SIZE,
                                                                 24,
                                                                 &fontConfig);
        RYN_VERIFY(m_Font != nullptr, "Failed to upload default font!");

        // Create ImGui backend
        m_Backend = std::make_unique<other::ImGuiBackend>(*m_GraphicsService);

        // Create panels
        m_Panels.emplace_back(std::make_unique<panel::Registry>());
    }

    void Editor::onDetach() {
        m_Backend.reset();
        ImGui::DestroyContext();
    }

    void Editor::onBeginFrame(const ryn::FrameTime& time) {
        const auto framebufferSize = m_WindowService->getFramebufferSize();

        // Update ImGui context
        ImGuiIO& io                = ImGui::GetIO();
        io.DeltaTime               = static_cast<float>(time.Delta);
        io.DisplaySize             = ImVec2(static_cast<float>(framebufferSize.x), static_cast<float>(framebufferSize.y));
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        if (io.WantTextInput) m_WindowService->maintainTextInput();

        // Prepare ImGui backend new frame
        m_Backend->newFrame();
        ImGui::NewFrame();
    }

    void Editor::onEndFrame(const ryn::FrameTime& time) { ImGui::EndFrame(); }

    void Editor::onUpdate(const ryn::FrameTime& time) {
        // Create ImGui dockspace
        static constexpr ImGuiDockNodeFlags DOCK_NODE_FLAGS = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), DOCK_NODE_FLAGS);

        // Update panels
        for (const auto& panel : m_Panels)
            if (panel->isOpen())
                panel->onUpdate(time);

        // Draw panels
        for (const auto& panel : m_Panels)
            if (ImGui::Begin(panel->getName()), panel->isOpen()) {
                panel->onDraw(time);
                ImGui::End();
            }

        // Finalize and render frame
        ImGui::Render();
        m_Backend->render();
    }

    void Editor::onEvent(ryn::EventListener& listener) {
        ImGuiIO& io = ImGui::GetIO();

        listener.match<ryn::event::WindowKeypress>([&](auto& e) {
            io.AddKeyEvent(mapKeycodeToImGuiKey(e.Keycode), e.IsDown);
            if (io.WantCaptureKeyboard) e.consume();
        });

        listener.match<ryn::event::WindowMouseButton>([&](auto& e) {
            int button = 0;
            if (e.Button == ryn::MouseButton::Right) button = 1;
            if (e.Button == ryn::MouseButton::Middle) button = 2;
            io.AddMouseButtonEvent(button, e.IsDown);
            if (io.WantCaptureMouse) e.consume();
        });

        listener.match<ryn::event::WindowMouseMove>([&](auto& e) {
            io.AddMousePosEvent(e.Position.x, e.Position.y);
            if (io.WantCaptureMouse) e.consume();
        });

        listener.match<ryn::event::WindowMouseScroll>([&](auto& e) {
            io.AddMouseWheelEvent(e.Delta.x, e.Delta.y);
            if (io.WantCaptureMouse) e.consume();
        });

        listener.match<ryn::event::WindowTextInput>([&](auto& e) {
            io.AddInputCharactersUTF8(e.Text);
            if (io.WantCaptureKeyboard) e.consume();
        });
    }
}

namespace {
    using KeycodeLUT = std::array<ImGuiKey, static_cast<size_t>(ryn::Keycode::Count)>;

    constexpr KeycodeLUT s_KeycodeLUT = [] {
        KeycodeLUT lut;
        lut.fill(ImGuiKey_None);
#define MAP(keycode, imGuiKey) lut[static_cast<size_t>(keycode)] = imGuiKey;
        /* Alphabetic */
        MAP(ryn::Keycode::A, ImGuiKey_A)
        MAP(ryn::Keycode::B, ImGuiKey_B)
        MAP(ryn::Keycode::C, ImGuiKey_C)
        MAP(ryn::Keycode::D, ImGuiKey_D)
        MAP(ryn::Keycode::E, ImGuiKey_E)
        MAP(ryn::Keycode::F, ImGuiKey_F)
        MAP(ryn::Keycode::G, ImGuiKey_G)
        MAP(ryn::Keycode::H, ImGuiKey_H)
        MAP(ryn::Keycode::I, ImGuiKey_I)
        MAP(ryn::Keycode::J, ImGuiKey_J)
        MAP(ryn::Keycode::K, ImGuiKey_K)
        MAP(ryn::Keycode::L, ImGuiKey_L)
        MAP(ryn::Keycode::M, ImGuiKey_M)
        MAP(ryn::Keycode::N, ImGuiKey_N)
        MAP(ryn::Keycode::O, ImGuiKey_O)
        MAP(ryn::Keycode::P, ImGuiKey_P)
        MAP(ryn::Keycode::Q, ImGuiKey_Q)
        MAP(ryn::Keycode::R, ImGuiKey_R)
        MAP(ryn::Keycode::S, ImGuiKey_S)
        MAP(ryn::Keycode::T, ImGuiKey_T)
        MAP(ryn::Keycode::U, ImGuiKey_U)
        MAP(ryn::Keycode::V, ImGuiKey_V)
        MAP(ryn::Keycode::W, ImGuiKey_W)
        MAP(ryn::Keycode::X, ImGuiKey_X)
        MAP(ryn::Keycode::Y, ImGuiKey_Y)
        MAP(ryn::Keycode::Z, ImGuiKey_Z)
        /* Numeric (Top Row) */
        MAP(ryn::Keycode::D1, ImGuiKey_1)
        MAP(ryn::Keycode::D2, ImGuiKey_2)
        MAP(ryn::Keycode::D3, ImGuiKey_3)
        MAP(ryn::Keycode::D4, ImGuiKey_4)
        MAP(ryn::Keycode::D5, ImGuiKey_5)
        MAP(ryn::Keycode::D6, ImGuiKey_6)
        MAP(ryn::Keycode::D7, ImGuiKey_7)
        MAP(ryn::Keycode::D8, ImGuiKey_8)
        MAP(ryn::Keycode::D9, ImGuiKey_9)
        MAP(ryn::Keycode::D0, ImGuiKey_0)
        /* Function */
        MAP(ryn::Keycode::F1, ImGuiKey_F1)
        MAP(ryn::Keycode::F2, ImGuiKey_F2)
        MAP(ryn::Keycode::F3, ImGuiKey_F3)
        MAP(ryn::Keycode::F4, ImGuiKey_F4)
        MAP(ryn::Keycode::F5, ImGuiKey_F5)
        MAP(ryn::Keycode::F6, ImGuiKey_F6)
        MAP(ryn::Keycode::F7, ImGuiKey_F7)
        MAP(ryn::Keycode::F8, ImGuiKey_F8)
        MAP(ryn::Keycode::F9, ImGuiKey_F9)
        MAP(ryn::Keycode::F10, ImGuiKey_F10)
        MAP(ryn::Keycode::F11, ImGuiKey_F11)
        MAP(ryn::Keycode::F12, ImGuiKey_F12)
        /* Arrow */
        MAP(ryn::Keycode::ArrowUp, ImGuiKey_UpArrow)
        MAP(ryn::Keycode::ArrowDown, ImGuiKey_DownArrow)
        MAP(ryn::Keycode::ArrowLeft, ImGuiKey_LeftArrow)
        MAP(ryn::Keycode::ArrowRight, ImGuiKey_RightArrow)
        /* Control, Navigation & Special */
        MAP(ryn::Keycode::Escape, ImGuiKey_Escape)
        MAP(ryn::Keycode::Tab, ImGuiKey_Tab)
        MAP(ryn::Keycode::CapsLock, ImGuiKey_CapsLock)
        MAP(ryn::Keycode::Space, ImGuiKey_Space)
        MAP(ryn::Keycode::Return, ImGuiKey_Enter)
        MAP(ryn::Keycode::Backslash, ImGuiKey_Backslash)
        MAP(ryn::Keycode::Backspace, ImGuiKey_Backspace)
        MAP(ryn::Keycode::Minus, ImGuiKey_Minus)
        MAP(ryn::Keycode::Equals, ImGuiKey_Equal)
        MAP(ryn::Keycode::LBracket, ImGuiKey_LeftBracket)
        MAP(ryn::Keycode::RBracket, ImGuiKey_RightBracket)
        MAP(ryn::Keycode::Semicolon, ImGuiKey_Semicolon)
        MAP(ryn::Keycode::Apostrophe, ImGuiKey_Apostrophe)
        MAP(ryn::Keycode::Comma, ImGuiKey_Comma)
        MAP(ryn::Keycode::Period, ImGuiKey_Period)
        MAP(ryn::Keycode::Slash, ImGuiKey_Slash)
        MAP(ryn::Keycode::Insert, ImGuiKey_Insert)
        MAP(ryn::Keycode::Delete, ImGuiKey_Delete)
        MAP(ryn::Keycode::PageUp, ImGuiKey_PageUp)
        MAP(ryn::Keycode::PageDown, ImGuiKey_PageDown)
        MAP(ryn::Keycode::Home, ImGuiKey_Home)
        MAP(ryn::Keycode::Pause, ImGuiKey_Pause)
        MAP(ryn::Keycode::End, ImGuiKey_End)
        MAP(ryn::Keycode::ScrollLock, ImGuiKey_ScrollLock)
        MAP(ryn::Keycode::PrintScreen, ImGuiKey_PrintScreen)
        MAP(ryn::Keycode::NumLock, ImGuiKey_NumLock)
        /* Keypad */
        MAP(ryn::Keycode::KP_Period, ImGuiKey_KeypadDecimal)
        MAP(ryn::Keycode::KP_Divide, ImGuiKey_KeypadDivide)
        MAP(ryn::Keycode::KP_Multiply, ImGuiKey_KeypadMultiply)
        MAP(ryn::Keycode::KP_Minus, ImGuiKey_KeypadSubtract)
        MAP(ryn::Keycode::KP_Plus, ImGuiKey_KeypadAdd)
        MAP(ryn::Keycode::KP_Enter, ImGuiKey_KeypadEnter)
        MAP(ryn::Keycode::KP_1, ImGuiKey_Keypad1)
        MAP(ryn::Keycode::KP_2, ImGuiKey_Keypad2)
        MAP(ryn::Keycode::KP_3, ImGuiKey_Keypad3)
        MAP(ryn::Keycode::KP_4, ImGuiKey_Keypad4)
        MAP(ryn::Keycode::KP_5, ImGuiKey_Keypad5)
        MAP(ryn::Keycode::KP_6, ImGuiKey_Keypad6)
        MAP(ryn::Keycode::KP_7, ImGuiKey_Keypad7)
        MAP(ryn::Keycode::KP_8, ImGuiKey_Keypad8)
        MAP(ryn::Keycode::KP_9, ImGuiKey_Keypad9)
        MAP(ryn::Keycode::KP_0, ImGuiKey_Keypad0)
        /* Modifiers */
        MAP(ryn::Keycode::LCtrl, ImGuiMod_Ctrl)
        MAP(ryn::Keycode::LShift, ImGuiMod_Shift)
        MAP(ryn::Keycode::LAlt, ImGuiMod_Alt)
        MAP(ryn::Keycode::LSuper, ImGuiMod_Super)
        MAP(ryn::Keycode::RCtrl, ImGuiMod_Ctrl)
        MAP(ryn::Keycode::RShift, ImGuiMod_Shift)
        MAP(ryn::Keycode::RAlt, ImGuiMod_Alt)
        MAP(ryn::Keycode::RSuper, ImGuiMod_Super)
#undef MAP
        return lut;
    }();

    constexpr ImGuiKey mapKeycodeToImGuiKey(ryn::Keycode keycode) {
        const auto value = static_cast<size_t>(keycode);
        if (value < s_KeycodeLUT.size()) return s_KeycodeLUT[value];
        return ImGuiKey_None;
    }
}