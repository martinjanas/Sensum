#include "imgui_custom.h"
#include "../sdk/helpers/console.h"

namespace imgui_custom
{
    void selector(const char* label, ImVec2& pos)
    {
        auto draw_list = ImGui::GetForegroundDrawList();

        ImVec2 rect_start = pos;
        ImVec2 rect_end = ImVec2{ pos.x + 195.f, pos.y + 26.f };

        // Draw the rectangle outline
        draw_list->AddRect(rect_start, rect_end, IM_COL32_WHITE);

        // Center the label text in the rectangle
        auto pos_centered = (rect_start + rect_end) * 0.5f;
        auto text_size = ImGui::CalcTextSize(label);
        auto text_mid = text_size * 0.25f;

        ImGui::PushFont(render::fonts::selector);
        draw_list->AddText({ pos_centered.x - text_mid.x, pos_centered.y - text_mid.y }, IM_COL32_WHITE, label);
        ImGui::PopFont();

        // Draw a left-pointing arrow triangle 25.f from the left side of the rectangle
        float arrow_offset_x = 10.0f;
        ImVec2 left_arrow_offset = ImVec2(rect_start.x + arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        g_Console->println("rs: %.2f, %.2f", pos_centered.x, pos_centered.y);
        ImGui::SetCursorScreenPos(pos_centered); //this doesnt work, doesnt position the button properly

        bool invis_left_arrow_btn = ImGui::Button("leftarrowbtn");
        if (ImGui::IsItemClicked())
        {
            g_Console->println("Left Arrow Clicked");
        }

        // Define the points for the triangle
        ImVec2 left_arrow_p1 = ImVec2(left_arrow_offset.x - 4.0f, left_arrow_offset.y);         // Tip of the arrow
        ImVec2 left_arrow_p2 = ImVec2(left_arrow_offset.x + 4.0f, left_arrow_offset.y - 7.0f);   // Top corner of the triangle
        ImVec2 left_arrow_p3 = ImVec2(left_arrow_offset.x + 4.0f, left_arrow_offset.y + 7.0f);   // Bottom corner of the triangle

        // Draw the filled triangle for the arrow
        draw_list->AddTriangleFilled(left_arrow_p1, left_arrow_p2, left_arrow_p3, IM_COL32_WHITE);

        ImVec2 right_arrow_offset = ImVec2(rect_end.x - arrow_offset_x, (rect_start.y + rect_end.y) * 0.5f);

        ImVec2 right_arrow_p1 = ImVec2(right_arrow_offset.x + 4.0f, right_arrow_offset.y);         // Tip of the arrow
        ImVec2 right_arrow_p2 = ImVec2(right_arrow_offset.x - 4.0f, right_arrow_offset.y + 7.0f);   // Top corner of the triangle
        ImVec2 right_arrow_p3 = ImVec2(right_arrow_offset.x - 4.0f, right_arrow_offset.y - 7.0f);   // Bottom corner of the triangle

        draw_list->AddTriangleFilled(right_arrow_p1, right_arrow_p2, right_arrow_p3, IM_COL32_WHITE);

    }
}
