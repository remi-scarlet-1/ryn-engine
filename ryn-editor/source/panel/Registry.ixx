module;
#include "Prelude.h"
#include <array>
#include <format>
#include <imgui.h>
#include <string>

export module Ryndition.Panel:Registry;
export import Ryndition;
export import Ryntime.Core.Reflection;

using namespace ryn::reflection;

export namespace rynd::panel {
    class Registry : public IEditorPanel {
    public:
        Registry() { m_Filters.fill(true); }

        void onDraw(const ryn::FrameTime& time) override {
            drawFilters();
            ImGui::Separator();

            for (uint32_t i = 0; i < static_cast<uint32_t>(TypeCategory::Count); ++i) {
                const auto category = static_cast<TypeCategory>(i);
                drawCategoryGroup(typeCategoryToString(category), category, m_Filters[i]);
            }
        }

        const char* getName() const override { return "Registry"; }

    private:
        std::array<bool, static_cast<size_t>(TypeCategory::Count)> m_Filters;

        void drawFilters() {
            int activeCount = 0;
            for (bool filter : m_Filters)
                if (filter) activeCount++;

            auto preview = activeCount == static_cast<int>(TypeCategory::Count) ? "All Categories" : "Filtered Selection...";

            if (ImGui::BeginCombo("##CategoryFilter", preview)) {
                for (uint16_t i = 0; i < static_cast<uint16_t>(TypeCategory::Count); ++i)
                    ImGui::Selectable(typeCategoryToString(static_cast<TypeCategory>(i)), &m_Filters[i], ImGuiSelectableFlags_DontClosePopups);
                ImGui::EndCombo();
            }
        }

        void drawCategoryGroup(const char* label, TypeCategory category, bool isVisible) {
            if (!isVisible) return;

            auto types = TypeRegistry::queryTypesByCategory(category);
            if (types.isEmpty()) return;

            if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
                static constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                                              ImGuiTableFlags_Hideable;

                if (ImGui::BeginTable(label, 3, tableFlags)) {
                    const ImVec4& idColor       = ImGui::GetStyle().Colors[ImGuiCol_TextDisabled];
                    const float   idColumnWidth = ImGui::CalcTextSize("0x0000000000000000").x + ImGui::GetStyle().ItemSpacing.x * 2.0f;
                    const float   em            = ImGui::GetFontSize();
                    ImGui::TableSetupColumn("Type ID", ImGuiTableColumnFlags_WidthFixed, idColumnWidth);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Dynamic", ImGuiTableColumnFlags_WidthFixed, em * 3.25f);
                    ImGui::TableHeadersRow();

                    for (const auto* info : types) {
                        ImGui::TableNextRow();

                        // Column 0 ID
                        char idBuffer[20];
                        auto [out, _] = std::format_to_n(idBuffer, sizeof(idBuffer) - 1, "{}", info->ID);
                        *out          = '\0';

                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextColored(idColor, "%s", idBuffer);

                        // Column 1 Name
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(info->Name);

                        // Column 2 Dynamic Flag
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