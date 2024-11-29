#include "imgui_custom.h"
#include "../sdk/helpers/console.h"

namespace imgui_custom
{
    void Selector(const char* label, const char** items, int items_count, int* v)
    {
        // Push a unique ID for this widget instance to avoid conflicts
        ImGui::PushID(label);

        // Set the widget size and position
        static ImVec2 base_widget_size(165.f, 26.f);
        ImVec2 label_text_size = ImGui::CalcTextSize(label);
        float label_width = label_text_size.x + 5.f; // Adding padding to the label width

        // The total widget width is the label width plus the base widget size
        ImVec2 widget_size = ImVec2(base_widget_size.x + label_width, base_widget_size.y);

        // Positioning and drawing the main widget
        ImVec2 pos = ImGui::GetCursorScreenPos();
        auto* draw_list = ImGui::GetWindowDrawList();

        // Draw the label text on the left side of the widget
        ImVec2 label_pos = ImVec2(pos.x, pos.y + (widget_size.y - label_text_size.y) * 0.5f);
        draw_list->AddText(label_pos, IM_COL32(255, 255, 255, 255), label);

        // Set the position for the main widget rect, offsetting for the label
        ImVec2 rect_start = ImVec2(pos.x + label_width, pos.y); // Offset by the label width
        ImVec2 rect_end = ImVec2(rect_start.x + base_widget_size.x, pos.y + base_widget_size.y);

        // Draw the custom widget border
        draw_list->AddRect(rect_start, rect_end, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);

        // Display the current item inside the widget
        const char* current_item = items[*v];
        ImVec2 pos_centered = (rect_start + rect_end) * 0.5f;
        ImVec2 text_size = ImGui::CalcTextSize(current_item);
        ImVec2 text_pos = ImVec2(pos_centered.x - text_size.x * 0.5f, pos_centered.y - text_size.y * 0.5f);

        // Set a custom font if needed and draw the text
        ImGui::PushFont(render::fonts::selector);
        draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), current_item);
        ImGui::PopFont();

        // Function to draw an arrow button and handle interaction
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

                // Draw the arrow triangle
                draw_list->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));

                // Set position for the invisible button
                ImVec2 button_offset = arrow_center - ImVec2{ 11.f, 13.f };
                ImGui::SetCursorScreenPos(button_offset);
                ImGui::InvisibleButton(btn_id, { 22.f, 26.f });

                // Hover effect
                if (ImGui::IsItemHovered())
                {
                    ImVec2 min = ImGui::GetItemRectMin();
                    ImVec2 max = ImGui::GetItemRectMax();
                    ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
                    draw_list->AddRectFilled(min, max, hover_color);
                }

                // Click event handling
                if (ImGui::IsItemClicked())
                {
                    if (is_left_arrow && *v > 0)
                        (*v)--; // Move to the previous item
                    else if (!is_left_arrow && *v < items_count - 1)
                        (*v)++; // Move to the next item
                }
            };

        // Calculate positions for the arrow buttons
        float arrow_offset_x = 10.0f;
        ImVec2 left_arrow_center = ImVec2(rect_start.x + arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);
        ImVec2 right_arrow_center = ImVec2(rect_end.x - arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        // Draw arrow buttons
        draw_arrow_button("invisbutton#left", left_arrow_center, true);
        draw_arrow_button("invisbutton#right", right_arrow_center, false);

        // Pop the unique ID to restore the original state
        ImGui::PopID();
    }
}
