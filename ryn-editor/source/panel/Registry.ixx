module;
#include "Prelude.h"
#include <algorithm>
#include <format>
#include <imgui.h>
#include <string_view>
#include <unordered_map>
#include <vector>

export module Ryndition.Panel:Registry;
import Ryntime.Logger;
export import Ryndition.Core;
import Ryntime;

export namespace rynd::panel {
    class Registry final : public ryn::DynamicType<IEditorPanel, "rynd::panel::Registry"> {
    public:
        const char* getDisplayName() const override { return "Registry"; }

        void onConfigure(ryn::ServiceDiscovery& discovery) override {}

        void onDraw(const ryn::Time& time, EditorSettings& settings, EditorContext& ctx) override {
            // Synchronize our UI state with the Registry's dynamic categories
            syncAndSort();

            // Draw global filter controls
            drawFilters();
            ImGui::Separator();

            // Draw the groups in alphabetical order
            // We iterate our sorted list and fetch the types from the Registry
            for (const auto& category : m_SortedCategories) {
                if (m_Filters[category]) {
                    auto types = ryn::TypeRegistry::queryTypesByCategory(category);
                    drawCategoryGroup(category, types);
                }
            }
        }

    private:
        // We store the sorted list and the filter state
        std::vector<ryn::TypeCategory>              m_SortedCategories;
        std::unordered_map<ryn::TypeCategory, bool> m_Filters;

        void syncAndSort() {
            bool needsSort = false;

            // Use the callback to discover categories
            ryn::TypeRegistry::foreachCategory([&](const auto& category, const auto& types) {
                if (!m_Filters.contains(category)) {
                    m_Filters[category] = true; // Default to visible
                    m_SortedCategories.push_back(category);
                    needsSort = true;
                }
            });

            if (needsSort) {
                std::ranges::sort(m_SortedCategories,
                                  [](const auto& a, const auto& b) { return std::string_view(a.Name) < std::string_view(b.Name); });
            }
        }

        void drawFilters() {
            uint64_t activeCount = 0;
            for (const auto& cat : m_SortedCategories)
                if (m_Filters[cat]) activeCount++;

            const char* preview = (activeCount == m_SortedCategories.size()) ? "All Categories" : "Filtered Selection...";

            if (ImGui::BeginCombo("##CategoryFilter", preview)) {
                for (const auto& cat : m_SortedCategories) {
                    ImGui::Selectable(cat.Name, &m_Filters[cat], ImGuiSelectableFlags_DontClosePopups);
                }
                ImGui::EndCombo();
            }
        }

        void drawCategoryGroup(const ryn::TypeCategory& category, ryn::Span<const ryn::TypeInfo* const> types) {
            if (types.isEmpty()) return;

            if (ImGui::CollapsingHeader(category.Name, ImGuiTreeNodeFlags_DefaultOpen)) {
                static constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                                              ImGuiTableFlags_Hideable;

                if (ImGui::BeginTable(category.Name, 3, tableFlags)) {
                    const ImVec4& idColor       = ImGui::GetStyle().Colors[ImGuiCol_TextDisabled];
                    const float   idColumnWidth = ImGui::CalcTextSize("0x0000000000000000").x + ImGui::GetStyle().ItemSpacing.x * 2.0f;
                    const float   em            = ImGui::GetFontSize();

                    ImGui::TableSetupColumn("Type ID", ImGuiTableColumnFlags_WidthFixed, idColumnWidth);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Dynamic", ImGuiTableColumnFlags_WidthFixed, em * 3.25f);
                    ImGui::TableHeadersRow();

                    for (const auto* info : types) {
                        ImGui::TableNextRow();

                        // Column 0: ID (Formatted via your TypeID formatter)
                        char idBuffer[20];
                        auto [out, _] = std::format_to_n(idBuffer, sizeof(idBuffer) - 1, "{}", info->ID);
                        *out          = '\0';

                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextColored(idColor, "%s", idBuffer);

                        // Column 1: Name
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(info->Name);

                        // Column 2: Dynamic Flag
                        ImGui::TableSetColumnIndex(2);
                        if (info->IsDynamic) ImGui::TextColored(idColor, "[x]");
                        else ImGui::TextColored(idColor, "[ ]");
                    }
                    ImGui::EndTable();
                }
                ImGui::Spacing();
            }
        }
    };
}