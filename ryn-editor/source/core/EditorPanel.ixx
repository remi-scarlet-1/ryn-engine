module;
#include "Prelude.h"
#include <concepts>
#include <unordered_map>
#include <vector>

export module Ryndition.Core:EditorPanel;
export import Ryntime;
import :TypeCategory;

// TODO: expand IEditorPanel:
//  - allow saving of panel layout (with the help of the ImGui .ini thing)
//  - have a titlebar which automatically discovers all available panels and allows to make/close(destry) them on the fly

using namespace ryn::reflection;
using ryn::service::Entity;

export namespace rynd::core {
    struct EditorSettings {
        struct {
            float SashRatio = 0.75f; // TODO: this sash thing might be abstractable to re-use
        } Inspector;

        struct {
            std::unordered_map<TypeCategory, bool> Filters{};
        } Registry;
    };

    struct EditorContext {
        struct {
            Entity              Primary = Entity::Null;
            std::vector<Entity> Multi{};
        } Inspector;
    };

    class IEditorPanel : public DynamicBase<EditorTypeCategory::Panel> {
    public:
        ~IEditorPanel() override = default;

        virtual const char* getDisplayName() const = 0;

        virtual void onConfigure(ryn::ServiceDiscovery& discovery) = 0;
        virtual void onDraw(const ryn::FrameTime& time, EditorSettings& settings, EditorContext& ctx) = 0;
    };

    template<typename T>
    concept IsPanel = std::derived_from<T, IEditorPanel>;
}