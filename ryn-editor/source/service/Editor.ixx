module;
#include "Macros.h"
#include "Prelude.h"
#include <imgui.h>
#include <memory>
#include <vector>

export module Ryndition.Service:Editor;
export import Ryndition.Detail.ImGuiBackend;
export import Ryndition.Panel;
export import Ryntime;

using rynd::detail::ImGuiBackend;

export namespace rynd::service {
    class RYN_API Editor final : public ryn::reflection::DynamicType<ryn::IService, "rynd::service::Editor"> {
    public:
        Editor()           = default;
        ~Editor() override = default;

    private:
        struct PanelEntry {
            std::unique_ptr<panel::IPanel> Instance;
            bool                           IsConfigured;
        };

        ryn::IServiceLocator*         m_ServiceLocator = nullptr;
        ryn::service::Window*         m_Window         = nullptr;
        ryn::service::Graphics*       m_Graphics       = nullptr;
        std::unique_ptr<ImGuiBackend> m_ImGuiBackend   = nullptr;
        std::vector<PanelEntry>       m_Panels;

        void onAttach(ryn::IServiceLocator& locator) override;
        void onDetach() override;
        void onUpdate(const ryn::FrameTime& time) override;
        void onEvent(ryn::EventListener& listener) override;

        void imGuiInit();
        void imGuiCleanup();
        void imGuiBeginFrame(const ryn::FrameTime& time);
        void imGuiEndFrame();

        // TODO: improve panel API
        //  - move IPanels to reflection system
        //  - have a titlebar which automatically discovers all available panels and allows to make/close(destry) them on the fly

        template<panel::IsPanel T, typename... Args>
        void makePanel(Args&&... args) {
            auto panel = std::make_unique<T>(std::forward<Args>(args)...);
            m_Panels.emplace_back(setupPanel(std::move(panel)));
        }

        PanelEntry setupPanel(std::unique_ptr<panel::IPanel> panel);
    };
}

RYN_TYPE_REFLECTION_KEEP(rynd::service::Editor);