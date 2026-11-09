module;
#include "Prelude.h"
#include <imgui.h>
#include <memory>
#include <unordered_map>
#include <vector>

export module Ryndition.Editor;
import Ryntime.Logger;
export import Ryndition.Core;
export import Ryndition.Detail.ImGuiBackend;
export import Ryndition.Panel;
import Ryntime;

using rynd::detail::ImGuiBackend;

export namespace rynd {
    class Editor final : public ryn::DynamicType<ryn::IService, "rynd::Editor"> {
    public:
        Editor()           = default;
        ~Editor() override = default;

        template<IsPanel T>
        void makePanel() {
            ryn::Box<IEditorPanel> panel = ryn::Box<T>::template make<T>();
            internal_makePanel(T::TYPE_ID, std::move(panel));
        }

        template<IsPanel T>
        void togglePanel(bool enable) {
            internal_togglePanel(T::TYPE_ID, enable);
        }

    private:
        ryn::IServiceLocator*   m_ServiceLocator  = nullptr;
        ryn::service::Window*   m_ServiceWindow   = nullptr;
        ryn::service::Graphics* m_ServiceGraphics = nullptr;

        EditorSettings m_EditorSettings;
        EditorContext  m_EditorContext;

        struct PanelEntry {
            ryn::Box<IEditorPanel> Instance;
            bool                   IsConfigured;
            bool                   IsEnabled;
        };

        std::unordered_map<ryn::TypeID, PanelEntry> m_Panels;
        std::unique_ptr<ImGuiBackend>               m_Backend = nullptr;

        void onAttach(ryn::IServiceLocator& locator) override;
        void onDetach() override;
        void onUpdate(const ryn::Time& time) override;
        void onEvent(ryn::EventListener& listener) override;

        void internal_makePanel(ryn::TypeID id, ryn::Box<IEditorPanel>&& panel);
        void internal_togglePanel(ryn::TypeID id, bool enable);

        void imGuiInit();
        void imGuiCleanup();
        void imGuiBeginFrame(const ryn::Time& time);
        void imGuiEndFrame();
    };
}

RYN_KEEP_REFLECTED_TYPE(rynd::Editor);