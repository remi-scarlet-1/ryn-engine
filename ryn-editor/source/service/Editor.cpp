module;
#include "../detail/FontEmbeddings.inl"
#include "Prelude.h"
#include <algorithm>
#include <cmath>
#include <imgui.h>
#include <memory>

module Ryndition.Service;

RYN_SCOPE_FILE("Service|Editor");

namespace {
    constexpr ImGuiKey mapKeycodeToImGuiKey(ryn::Keycode keycode);
}

namespace rynd::service {
    void Editor::onAttach(ryn::IServiceLocator& locator) {
        m_ServiceLocator = &locator;
        locator.require(m_Window);
        locator.require(m_Graphics);

        imGuiInit();

        makePanel<panel::Registry>();
        makePanel<panel::Hierarchy>();
    }

    void Editor::onDetach() { imGuiCleanup(); }

    void Editor::onUpdate(const ryn::FrameTime& time) {
        imGuiBeginFrame(time);

        // Draw panels
        for (const auto& panel : m_Panels) {
            if (!panel.IsConfigured) continue;

            ImGui::SetNextWindowSizeConstraints(ImVec2(200, 150), ImVec2(FLT_MAX, FLT_MAX));

            static constexpr bool IS_OPEN = true;
            if (ImGui::Begin(panel.Instance->getName()), &IS_OPEN)
                panel.Instance->onDraw(time);

            ImGui::End();
        }

        imGuiEndFrame();
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

    void Editor::imGuiInit() {
        // Create context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io    = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Upload font
        ImFontConfig fontConfig;
        fontConfig.PixelSnapH  = true;
        fontConfig.OversampleH = 2;
        fontConfig.OversampleV = 1;

        io.Fonts->Clear();
        io.Fonts->AddFontFromMemoryCompressedTTF(detail::JETBRAIN_MONO_COMPRESSED_DATA, detail::JETBRAIN_MONO_COMPRESSED_SIZE, 24, &fontConfig);

        // Create backend
        m_ImGuiBackend = std::make_unique<detail::ImGuiBackend>(*m_Graphics);
    }

    void Editor::imGuiCleanup() {
        // Cleanup backend
        m_ImGuiBackend.reset();

        // Cleanup context
        ImGui::DestroyContext();
    }

    void Editor::imGuiBeginFrame(const ryn::FrameTime& time) {
        const auto resolution = m_Window->getFramebufferSize();

        // Update context
        ImGuiIO& io                = ImGui::GetIO();
        io.DeltaTime               = static_cast<float>(time.Delta);
        io.DisplaySize             = ImVec2(static_cast<float>(resolution.x), static_cast<float>(resolution.y));
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        if (io.WantTextInput) m_Window->maintainTextInput();

        // Prepare backend
        m_ImGuiBackend->beginFrame();

        // Prepare context
        ImGui::NewFrame();

        // Draw dockspace
        static constexpr ImGuiDockNodeFlags DOCK_NODE_FLAGS = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), DOCK_NODE_FLAGS);
    }

    void Editor::imGuiEndFrame() {
        // Finalize frame context
        ImGui::Render();

        // Finalize frame backend
        m_ImGuiBackend->render();
        ImGui::EndFrame();
    }

    Editor::PanelEntry Editor::setupPanel(std::unique_ptr<panel::IPanel> panel) {
        ryn::ServiceDiscovery discovery(*m_ServiceLocator);
        panel->onConfigure(discovery);

        if (discovery.wasSuccess())
            panel->onEnable();
        else
            RYN_WARN("Disabled '{0}' panel because it's required services couldn't be discovered!", panel->getName());

        return PanelEntry{.Instance = std::move(panel), .IsConfigured = discovery.wasSuccess()};
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