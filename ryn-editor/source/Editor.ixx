module;
#include "Macros.h"
#include "Prelude.h"
#include <imgui.h>
#include <memory>
#include <vector>

export module Ryndition;
export import Ryndition.ImGuiBackend;
export import Ryntime;

// TODO: expand IEditorPanel:
//  - allow panels to locate ServiceStack services by passing a IServiceLocator reference
//  - setup a structure for panels to cache local and persistent state
//  - allow saving of panel layout (with the help of the ImGui .ini thing)

export namespace rynd::panel {
    class IEditorPanel {
    public:
        virtual ~IEditorPanel() = default;

        virtual void onUpdate(const ryn::FrameTime& time) {}
        virtual void onDraw(const ryn::FrameTime& time) = 0;

        [[nodiscard]] virtual const char* getName() const = 0;
        [[nodiscard]] bool                isOpen() { return m_IsOpen; }

    protected:
        bool m_IsOpen = true;
    };
}

export namespace rynd {
    class RYN_API Editor final : public ryn::reflection::DynamicType<ryn::IService, "rynd::Editor"> {
    public:
        Editor()           = default;
        ~Editor() override = default;

        void onAttach(ryn::IServiceLocator& locator) override;
        void onDetach() override;
        void onBeginFrame(const ryn::FrameTime& time) override;
        void onEndFrame(const ryn::FrameTime& time) override;
        void onUpdate(const ryn::FrameTime& time) override;
        void onEvent(ryn::EventListener& listener) override;

    private:
        ryn::platform::Window*   m_WindowService   = nullptr;
        ryn::platform::Graphics* m_GraphicsService = nullptr;

        ImFont*                                           m_Font = nullptr;
        std::unique_ptr<other::ImGuiBackend>              m_Backend;
        std::vector<std::unique_ptr<panel::IEditorPanel>> m_Panels;
    };
}

RYN_TYPE_REFLECTION_KEEP(rynd::Editor);