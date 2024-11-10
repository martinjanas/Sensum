#include "imgui_custom.h"
#include "../sdk/helpers/console.h"

namespace imgui_custom
{
    /*void selector(const char* label, const char** items, int items_count, int* v)
    {
        static ImVec2 widget_size(195.f, 26.f);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        auto* draw_list = ImGui::GetWindowDrawList();

        ImVec2 rect_start = pos;
        ImVec2 rect_end = ImVec2(pos.x + widget_size.x, pos.y + widget_size.y);

        draw_list->AddRect(rect_start, rect_end, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);

        const char* current_item = items[*v];

        ImVec2 pos_centered = (rect_start + rect_end) * 0.5f;
        ImVec2 text_size = ImGui::CalcTextSize(current_item);
        ImVec2 text_pos = ImVec2(pos_centered.x - text_size.x * 0.25f, pos_centered.y - text_size.y * 0.25f);

        ImGui::PushFont(render::fonts::selector);
        draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), current_item);
        ImGui::PopFont();

        auto draw_arrow_button = [&](const char* btn_id, ImVec2& arrow_center, bool is_left_arrow)
        {
            ImVec2 p1, p2, p3;

            if (is_left_arrow)
            {
                p1 = ImVec2(arrow_center.x - 4.0f, arrow_center.y);
                p2 = ImVec2(arrow_center.x + 4.0f, arrow_center.y - 7.0f);
                p3 = ImVec2(arrow_center.x + 4.0f, arrow_center.y + 7.0f);
            }
            else
            {
                p1 = ImVec2(arrow_center.x + 4.0f, arrow_center.y);
                p2 = ImVec2(arrow_center.x - 4.0f, arrow_center.y + 7.0f);
                p3 = ImVec2(arrow_center.x - 4.0f, arrow_center.y - 7.0f);
            }

            draw_list->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));

            ImVec2 button_offset = arrow_center - ImVec2{ 11.f, 13.f };
            ImGui::SetCursorScreenPos(button_offset);
            ImGui::InvisibleButton(btn_id, { 22.f, 26.f });

            if (ImGui::IsItemHovered())
            {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);

                draw_list->AddRectFilled(min, max, hover_color);
            }

            if (ImGui::IsItemClicked())
            {
                if (is_left_arrow && *v > 0)
                    (*v)--;
                else if (!is_left_arrow && *v < items_count - 1)
                    (*v)++;
            }
        };

        float arrow_offset_x = 10.0f;
        ImVec2 left_arrow_center = ImVec2(rect_start.x + arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);
        ImVec2 right_arrow_center = ImVec2(rect_end.x - arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        draw_arrow_button("invisbutton#left", left_arrow_center, true);
        draw_arrow_button("invisbutton#right", right_arrow_center, false);
    }*/

    void selector(const std::vector<std::string>& items, int* v)
    {
        // Push a unique ID for the widget to avoid interference with other widgets
        ImGui::PushID("CustomSelectableWidget");

        // Define the size for the widget
        const ImVec2 widgetSize(196, 26);

        // Create a horizontal layout for the arrows and the current selection
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));  // Adjust spacing between elements

        // Begin a group to align everything horizontally
        ImGui::BeginGroup();

        // Left Arrow Button (to go backward)
        if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) {
            if (*v > 0) {
                --(*v);  // Move the selection backward
            }
        }

        // SameLine to keep the elements in the same row
        ImGui::SameLine();

        // Add the selectable item showing the current selection
        ImGui::ItemSize(widgetSize);  // Define the widget size for ImGui
        ImGui::ItemAdd(ImRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + widgetSize), ImGui::GetID("##Selectable"));

        // Set the active ID (interactive state) for the selectable
        if (ImGui::IsItemHovered()) {
            ImGui::SetActiveID(ImGui::GetID("##Selectable"), ImGui::GetCurrentWindow());
        }

        if (ImGui::Selectable(items[*v].c_str(), false, ImGuiSelectableFlags_None, widgetSize)) {
            // If the selectable is clicked, it will update the value of *v
            // This part can be handled if you want more complex behavior here
        }

        // SameLine to keep the elements in the same row
        ImGui::SameLine();

        // Right Arrow Button (to go forward)
        if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {
            if (*v < (items.size() - 1)) {
                ++(*v);  // Move the selection forward
            }
        }

        // End the group
        ImGui::EndGroup();

        // Pop style variable
        ImGui::PopStyleVar();

        // Pop the widget ID
        ImGui::PopID();
    }
}
