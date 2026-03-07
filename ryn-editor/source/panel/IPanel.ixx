module;
#include "Prelude.h"
#include <concepts>

export module Ryndition.Panel:IPanel;
export import Ryntime;

// TODO: expand IPanel:
//  - setup a structure for panels to cache local and persistent state
//  - allow saving of panel layout (with the help of the ImGui .ini thing)

// TODO: improve panel API
//  - move IPanels to reflection system
//  - have a titlebar which automatically discovers all available panels and allows to make/close(destry) them on the fly

export namespace rynd::panel {
    class IPanel {
    public:
        virtual ~IPanel() = default;

        [[nodiscard]] virtual const char* getName() const = 0;

        virtual void onConfigure(ryn::ServiceDiscovery& discovery) = 0;
        virtual void onEnable() {}
        virtual void onDisable() {}
        virtual void onDraw(const ryn::FrameTime& time) = 0;
    };

    template<typename T>
    concept IsPanel = std::derived_from<T, IPanel>;
}