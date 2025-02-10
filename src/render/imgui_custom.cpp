#include "imgui_custom.h"
#include "../sdk/helpers/console.h"

namespace imgui_custom
{
    void selector(const char* label, const char** items, int items_count, int* v)
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

    void separator(const char* label)
    {
        ImGui::PushID(label);
        {
            auto size = ImGui::CalcTextSize(label);
            auto draw_list = ImGui::GetWindowDrawList();
            const auto width = ImGui::GetContentRegionAvail().x;
            auto style = imgui::GetStyle();
            
            const auto win_pos = ImGui::GetWindowPos();
            const auto cursor = ImVec2(ImGui::GetCursorPosX() + win_pos.x, ImGui::GetCursorPosY() + win_pos.y);
            const auto pos = ImVec2(cursor.x + width / 2 - size.x / 2, cursor.y);
            const auto col = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text));
            draw_list->AddText(pos, col, label);
            
            const ImVec2 line_start(cursor.x, cursor.y + size.y / 2);
            const ImVec2 line_end(cursor.x + width, cursor.y + size.y / 2);
            
            const float line_padding = 5.f; // Padding for the line
            draw_list->AddLine(ImVec2(cursor.x - line_padding, cursor.y + size.y / 2), 
                               ImVec2(pos.x - 5.f, cursor.y + size.y / 2), col);
            draw_list->AddLine(ImVec2(pos.x + size.x + 5.f, cursor.y + size.y / 2), 
                               ImVec2(line_end.x + line_padding, cursor.y + size.y / 2), col);
            
            ImGui::NewLine();
            
        }
        ImGui::PopID();
    }

    bool checkbox(const char* label, bool* v)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        const float square_sz = ImGui::GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
        ImGui::ItemSize(total_bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
            return false;
        }

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
        {
            *v = !(*v);
            ImGui::MarkItemEdited(id);
        }

        const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
        ImGui::RenderNavHighlight(total_bb, id);
        ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
        ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);
        bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
        if (*v)
        {
            window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, check_col, style.FrameRounding);
        }

        ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
        if (g.LogEnabled)
            ImGui::LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
        if (label_size.x > 0.0f)
            ImGui::RenderText(label_pos, label);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return pressed;
    }
}
