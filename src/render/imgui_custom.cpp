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

        float line_y = rect_start.y - 2.0f;
        draw_list->AddLine(ImVec2(rect_start.x, line_y), ImVec2(rect_end.x, line_y), IM_COL32(255, 255, 255, 255), 2.0f);

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

    void selector(const char* label, const char** items, int items_count, int* v)
    {
        static ImVec2 widget_size(195.f, 26.f); // Widget size

        // Get the current position set externally before calling this function
        ImVec2 original_pos = ImGui::GetCursorScreenPos();

        // You can choose to modify the cursor position here if needed, but avoid it outside of this scope
        ImVec2 pos = original_pos;  // Start with the original cursor position
        ImGui::SetCursorScreenPos(pos);  // Set the cursor to the position you want to start drawing

        auto* draw_list = ImGui::GetWindowDrawList();

        // Define the start and end of the rectangle using the current position
        ImVec2 rect_start = pos;
        ImVec2 rect_end = ImVec2(pos.x + widget_size.x, pos.y + widget_size.y);

        // Draw the main rectangle outline
        draw_list->AddRect(rect_start, rect_end, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);

        // Center the text inside the rectangle
        const char* current_item = items[*v];
        ImVec2 pos_centered = (rect_start + rect_end) * 0.5f;
        ImVec2 text_size = ImGui::CalcTextSize(current_item);
        ImVec2 text_pos = ImVec2(pos_centered.x - text_size.x * 0.25f, pos_centered.y - text_size.y * 0.25f);

        ImGui::PushFont(render::fonts::selector);
        draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), current_item);
        ImGui::PopFont();

        // Lambda to draw arrow buttons for selection
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

                // Invisible button area for interaction
                ImVec2 button_offset = arrow_center - ImVec2{ 11.f, 13.f };
                ImGui::SetCursorScreenPos(button_offset); // This sets the cursor for the invisible button area
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

        // Offset for arrow buttons
        float arrow_offset_x = 10.0f;
        ImVec2 left_arrow_center = ImVec2(rect_start.x + arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);
        ImVec2 right_arrow_center = ImVec2(rect_end.x - arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        // Draw the arrow buttons
        draw_arrow_button("invisbutton#left", left_arrow_center, true);
        draw_arrow_button("invisbutton#right", right_arrow_center, false);

        // Restore the cursor position after the widget is drawn
        ImGui::SetCursorScreenPos(original_pos);  // Restore original cursor position
    }
}
