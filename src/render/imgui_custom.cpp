#include "imgui_custom.h"
#include "../sdk/helpers/console.h"

namespace imgui_custom
{
    bool Selector(const char* label, const char* unique_id)
    {
        // Combine label with a unique ID for ImGui
        std::string full_label = std::string(label) + "##" + unique_id;

        // Define the size of the selector widget
        ImVec2 widget_size(195.f, 26.f);
        bool clicked = false;

        // Save current cursor position
        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Get the draw list to draw custom shapes
        auto* draw_list = ImGui::GetWindowDrawList();

        // Define the start and end positions for the rectangle
        ImVec2 rect_start = pos;
        ImVec2 rect_end = ImVec2(pos.x + widget_size.x, pos.y + widget_size.y);

        // Draw the main rectangle outline
        draw_list->AddRect(rect_start, rect_end, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);

        // Center the label text in the rectangle
        ImVec2 pos_centered = (rect_start + rect_end) * 0.5f;
        ImVec2 text_size = ImGui::CalcTextSize(label);
        ImVec2 text_pos = ImVec2(pos_centered.x - text_size.x * 0.25f, pos_centered.y - text_size.y * 0.25f);

        ImGui::PushFont(render::fonts::selector);
        draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), label);
        ImGui::PopFont();

        // Draw the left and right arrow buttons with hover effects
        auto draw_arrow_button = [&](const char* btn_id, ImVec2 arrow_center, bool is_left_arrow) -> bool {
            // Draw the triangle arrow
            ImVec2 p1, p2, p3;
            if (is_left_arrow) {
                p1 = ImVec2(arrow_center.x - 4.0f, arrow_center.y);         // Tip of the arrow
                p2 = ImVec2(arrow_center.x + 4.0f, arrow_center.y - 7.0f);   // Top corner of the triangle
                p3 = ImVec2(arrow_center.x + 4.0f, arrow_center.y + 7.0f);   // Bottom corner of the triangle
            }
            else {
                p1 = ImVec2(arrow_center.x + 4.0f, arrow_center.y);         // Tip of the arrow
                p2 = ImVec2(arrow_center.x - 4.0f, arrow_center.y + 7.0f);   // Top corner of the triangle
                p3 = ImVec2(arrow_center.x - 4.0f, arrow_center.y - 7.0f);   // Bottom corner of the triangle
            }
            draw_list->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));

            // Position and size for the invisible button
            ImVec2 button_offset = arrow_center - ImVec2{ 11.f, 13.f };
            ImGui::SetCursorScreenPos(button_offset);
            ImGui::InvisibleButton(btn_id, { 22.f, 26.f });

            // Check hover and click states
            bool button_clicked = false;
            if (ImGui::IsItemHovered()) {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
                draw_list->AddRectFilled(min, max, hover_color);
            }

            if (ImGui::IsItemClicked()) {
                g_Console->println(is_left_arrow ? "Hello from left" : "Hello from right");
                button_clicked = true;
            }

            return button_clicked;
            };

        // Draw left and right arrows and check for clicks
        float arrow_offset_x = 10.0f;
        ImVec2 left_arrow_center = ImVec2(rect_start.x + arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);
        ImVec2 right_arrow_center = ImVec2(rect_end.x - arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        // Update clicked state based on arrow button presses
        clicked |= draw_arrow_button("invisbutton#left", left_arrow_center, true);
        clicked |= draw_arrow_button("invisbutton#right", right_arrow_center, false);

        return clicked;
    }
}
