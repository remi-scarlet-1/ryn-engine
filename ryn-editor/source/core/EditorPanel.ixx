module;
#include "Prelude.h"
#include <concepts>
#include <unordered_map>
#include <vector>

export module Ryndition.Core:EditorPanel;
export import Ryntime;
import :EditorTypeCategory;

// TODO: expand IEditorPanel:
//  - allow saving of panel layout (with the help of the ImGui .ini thing)
//  - have a titlebar which automatically discovers all available panels and allows to make/close(destry) them on the fly

// TODO: rewrite Hierarchy/Registry panels to improve performance and tie them in with EditorSettings/EditorContext

using ryn::service::Entity;

export namespace rynd {
    struct EditorSettings {
        struct {
            const float SashHeight = 6.0f; // px
            const float SashPadding = 100.0f; // px
            float SashRatio = 0.5f; // TODO: this sash thing might be abstractable to re-use
        } Inspector;

        struct {
            std::unordered_map<TypeCategory, bool> Filters{};
        } Registry;
    };

    struct EditorContext {
        struct {
            Entity              SelectionPrimary = Entity::Null;
            std::vector<Entity> SelectionAll{};
        } Inspector;
    };

    class IEditorPanel : public ryn::DynamicBase<EditorTypeCategory::Panel> {
    public:
        ~IEditorPanel() override = default;

        virtual const char*     getDisplayName() const = 0;
        virtual ryn::Vector2Int getWindowMinSize() { return ryn::Vector2Int(600, 400); }

        virtual void onConfigure(ryn::ServiceDiscovery& discovery)                               = 0;
        virtual void onDraw(const ryn::Time& time, EditorSettings& settings, EditorContext& ctx) = 0;
    };

    template<typename T>
    concept IsPanel = std::derived_from<T, IEditorPanel>;
}