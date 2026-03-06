module;
#include "Prelude.h"

#include <cstdint>
#include <imgui.h>
#include <string>
#include <unordered_set>

export module Ryndition.Panel:Hierarchy;
export import :IPanel;
import Ryntime;
import Ryntime.System;

using namespace ryn;
using namespace ryn::service;

RYN_SCOPE_FILE("Test");

export namespace rynd::panel {
    class Hierarchy : public IPanel {
    public:
        Hierarchy()           = default;
        ~Hierarchy() override = default;

        const char* getName() const override { return "Hierarchy"; }

        void onConfigure(ServiceDiscovery& discovery) override { discovery.require(m_World); }

        void onDraw(const FrameTime& time) override {
            drawToolbar();
            ImGui::Separator();

            float availableHeight = ImGui::GetContentRegionAvail().y;

            drawHierarchy();
            splitter(availableHeight);
            drawOrphans();

            if (!m_PendingDeletes.empty()) {
                for (Entity e : m_PendingDeletes)
                    m_World->Entity.destroy(e);
                m_PendingDeletes.clear();
            }
        }

    private:
        World*                     m_World             = nullptr;
        static constexpr float     SPLITTER_HEIGHT     = 6.0; // 6px
        static constexpr float     SPLITTER_MIN_HEIGHT = 100;
        float                      m_SplitterHeight    = 300.0f;
        Entity                     m_SelectionLatest   = Entity::Null;
        std::unordered_set<Entity> m_Selection;
        std::vector<Entity>        m_PendingDeletes;

        void drawToolbar() {
            if (ImGui::Button("Create Entity")) createEntity();
            ImGui::SameLine();
            if (ImGui::Button("Create Orphan")) createOrphan();
        }

        void beginRegion(const char* name, ImVec2 size = ImVec2(0, 0)) {
            ImGui::TextDisabled("%s", name);
            ImGui::Separator();
            ImGui::BeginChild(name, size, ImGuiChildFlags_None, ImGuiWindowFlags_None);
        }

        void endRegion() { ImGui::EndChild(); }

        void splitter(float totalHeight) {
            ImGui::Button("##splitter", ImVec2(-1, SPLITTER_HEIGHT));
            if (ImGui::IsItemActive()) m_SplitterHeight += ImGui::GetIO().MouseDelta.y;

            const float maxHeight = totalHeight - SPLITTER_MIN_HEIGHT - SPLITTER_HEIGHT;
            if (m_SplitterHeight < SPLITTER_MIN_HEIGHT) m_SplitterHeight = SPLITTER_MIN_HEIGHT;
            if (m_SplitterHeight > maxHeight) m_SplitterHeight = maxHeight;

            if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        }

        void drawHierarchy() {
            beginRegion("Hierarchy", ImVec2(0, m_SplitterHeight));

            ImGuiTreeNodeFlags baseFlags{};
            baseFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            baseFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
            baseFlags |= ImGuiTreeNodeFlags_DefaultOpen;

            auto recursiveDraw = [&](auto&& self, Entity e) -> void {
                auto& h = *m_World->Component.get<component::Hierarchy>(e);

                bool isLeaf = (h.getFirstChild() == Entity::Null);
                if (!drawTreeNode(e, isLeaf, baseFlags)) return;

                Entity child = h.getFirstChild();
                while (child != Entity::Null) {
                    self(self, child); // recursion here

                    auto* sibling = m_World->Component.get<component::HierarchySibling>(child);
                    child         = sibling ? sibling->getNext() : Entity::Null;
                }

                ImGui::TreePop();
            };

            m_World->Query.foreach<World::QueryAPI::Incl<component::Hierarchy>>([&](Entity e, component::Hierarchy& h) {
                if (h.getParent() != Entity::Null) return;
                recursiveDraw(recursiveDraw, e);
            });

            endRegion();

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_GROUP")) {
                    size_t entityCount = payload->DataSize / sizeof(Entity);
                    auto*  entities    = static_cast<Entity*>(payload->Data);
                    for (size_t i = 0; i < entityCount; i++)
                        system::Hierarchy::setParent(*m_World, entities[i], Entity::Null);
                }
                ImGui::EndDragDropTarget();
            }
        }

        void drawOrphans() {
            beginRegion("Orphans");

            ImGuiTreeNodeFlags baseFlags{};
            baseFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            baseFlags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;

            m_World->Query.foreach<World::QueryAPI::Excl<component::Hierarchy>>([&](Entity e) { drawTreeNode(e, true, baseFlags); });

            endRegion();
        }

        bool drawTreeNode(Entity e, bool isLeaf, ImGuiTreeNodeFlags baseFlags) {
            ImGui::PushID(e);

            // Flags
            ImGuiTreeNodeFlags flags = baseFlags;
            flags |= isLeaf ? ImGuiTreeNodeFlags_Leaf : 0;
            flags |= isSelected(e) ? ImGuiTreeNodeFlags_Selected : 0;

            // Draw tree node
            bool open = ImGui::TreeNodeEx("node", flags, "Entity %u", static_cast<uint32_t>(e));

            // Selection
            if (ImGui::IsItemHovered()) { handleSelection(e); }

            // Drag source
            if (ImGui::BeginDragDropSource()) {
                std::vector<Entity> entities;

                if (isSelected(e)) entities.assign(m_Selection.begin(), m_Selection.end());
                else entities.push_back(e);

                ImGui::SetDragDropPayload("ENTITY_GROUP", entities.data(), sizeof(Entity) * entities.size());
                ImGui::Text("Moving %zu entity(s)", entities.size());
                ImGui::EndDragDropSource();
            }

            // Drop target
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_GROUP")) {

                    size_t entityCount = payload->DataSize / sizeof(Entity);
                    auto*  entities    = static_cast<Entity*>(payload->Data);

                    for (size_t i = 0; i < entityCount; i++) {
                        Entity dragged = entities[i];
                        if (dragged != e) system::Hierarchy::setParent(*m_World, dragged, e);
                    }
                }

                ImGui::EndDragDropTarget();
            }

            // Right click context menu
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete")) {
                    for (Entity selected : m_Selection)
                        m_PendingDeletes.push_back(selected);
                    m_Selection.clear();
                    m_SelectionLatest = Entity::Null;
                }
                ImGui::EndPopup();
            }

            ImGui::PopID();
            return open;
        }

        Entity createEntity(Entity parent = Entity::Null) {
            auto entity = createOrphan();
            m_World->Component.make<component::Hierarchy>(entity);
            if (parent != Entity::Null) system::Hierarchy::setParent(*m_World, entity, parent);
            return entity;
        }

        Entity createOrphan() {
            auto entity = m_World->Entity.create();
            return entity;
        }

        void handleSelection(Entity entity) {
            ImGuiIO& io = ImGui::GetIO();

            bool isPressed      = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            bool isPressedRight = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
            bool isReleased     = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
            bool isDragging     = ImGui::IsMouseDragging(ImGuiMouseButton_Left);

            if (isPressed) {
                // Range Selection (Shift)
                if (io.KeyShift && m_SelectionLatest != Entity::Null && entity != m_SelectionLatest)
                    if (tryRangeSelect(entity, m_SelectionLatest)) return;

                // Toggle Selection (Ctrl)
                if (io.KeyCtrl) {
                    if (isSelected(entity)) m_Selection.erase(entity);
                    else m_Selection.insert(entity);
                    m_SelectionLatest = entity;
                    return;
                }

                // Standard Selection (Press Phase)
                // If the item is NOT already selected, we select it immediately.
                // This is crucial so that BeginDragDropSource has a selection to work with.
                if (!isSelected(entity)) {
                    m_Selection.clear();
                    m_Selection.insert(entity);
                    m_SelectionLatest = entity;
                }
            } else if (isReleased && !isDragging) {
                // Standard Selection (Release Phase)
                // If we release the mouse on an item that was part of a multi-selection
                // and we DIDN'T drag it, the user's intent was to select ONLY this item.
                if (!io.KeyCtrl && !io.KeyShift && isSelected(entity)) {
                    m_Selection.clear();
                    m_Selection.insert(entity);
                    m_SelectionLatest = entity;
                }
            }

            // Standard Selection (Right Click)
            // If we right click on an item that isn't selected, replace current selection for it
            if (isPressedRight && !isSelected(entity)) {
                m_Selection.clear();
                m_Selection.insert(entity);
                m_SelectionLatest = entity;
            }
        }

        bool tryRangeSelect(Entity e1, Entity e2) {
            auto* h1 = m_World->Component.tryGet<component::Hierarchy>(e1);
            auto* h2 = m_World->Component.tryGet<component::Hierarchy>(e2);

            bool bothHierarchy = h1 && h2;
            bool bothOrphans   = !h1 && !h2;
            if (!bothHierarchy && !bothOrphans) return false;

            m_Selection.clear();

            if (bothHierarchy)
                if (h1->getParent() == Entity::Null) { // Case: Range selection over roots
                    bool capturing = false;
                    m_World->Query.foreach<World::QueryAPI::Incl<component::Hierarchy>>([&](Entity walker, component::Hierarchy& h) {
                        if (h.getParent() != Entity::Null) return;
                        bool isBoundary = (walker == e1 || walker == e2);
                        if (isBoundary) capturing = !capturing;
                        if (capturing || isBoundary) m_Selection.insert(walker);
                    });
                } else { // Case: Range selection over siblings/children
                    bool   capturing = false;
                    Entity child     = m_World->Component.get<component::Hierarchy>(h1->getParent())->getFirstChild();
                    while (child != Entity::Null) {
                        bool isBoundary = (child == e1 || child == e2);
                        if (isBoundary) capturing = !capturing;
                        if (capturing || isBoundary) m_Selection.insert(child);

                        auto* sibling = m_World->Component.get<component::HierarchySibling>(child);
                        child         = sibling ? sibling->getNext() : Entity::Null;
                    }
                }
            else if (bothOrphans) { // Case: Range selection over orphans (entities not belonging to the hierarchy)
                bool capturing = false;
                m_World->Query.foreach<World::QueryAPI::Excl<component::Hierarchy>>([&](Entity walker) {
                    bool isBoundary = (walker == e1 || walker == e2);
                    if (isBoundary) capturing = !capturing;
                    if (capturing || isBoundary) m_Selection.insert(walker);
                });
            }

            m_Selection.insert(e1);
            m_SelectionLatest = e1;
            return true;
        }

        bool isSelected(Entity e) const { return m_Selection.contains(e); }
    };
}