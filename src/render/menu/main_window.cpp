#include "main_window.h"
#include "../../sdk/helpers/globals.h"
#include <vector>

namespace main_window
{
    bool is_open = false;

    void ShowSmoothModePopup(const char* label, int& current, const std::vector<std::string>& items) 
    {
        char buf[64];
        sprintf_s(buf, "%s: %s", label, items[current].c_str());

        if (ImGui::Button(buf)) {
            ImGui::OpenPopup("SmoothModePopup");
        }

        // Create the popup for the ListBox
        if (ImGui::BeginPopup("SmoothModePopup")) {
            for (int i = 0; i < items.size(); i++) {
                if (ImGui::Selectable(items[i].c_str(), current == i)) {
                    current = i; // Update current selection
                }
            }
            ImGui::EndPopup();
        }
    }

    void ShowMultiSelectPopup(const char* label, int& current, const std::vector<std::string>& items) {
        // Create a buffer for the button label
        char buf[256];
        snprintf(buf, sizeof(buf), "%s: ", label);

        // Prepare the list of selected items
        bool isFirst = true;
        for (size_t i = 0; i < items.size(); i++) {
            if (current & (1 << i)) { // Check if the i-th bit in current is set
                if (!isFirst) {
                    strncat_s(buf, ", ", sizeof(buf) - strlen(buf) - 1); // Add a comma separator
                }
                strncat_s(buf, items[i].c_str(), sizeof(buf) - strlen(buf) - 1); // Append the selected item
                isFirst = false;
            }
        }

        // Create the button to open the popup
        if (ImGui::Button(buf)) {
            ImGui::OpenPopup("MultiSelectPopup");
        }

        // Create the popup for the ListBox
        if (ImGui::BeginPopup("MultiSelectPopup")) {
            for (size_t i = 0; i < items.size(); i++) {
                // Check if the i-th bit in current is set for the checkbox state
                bool isSelected = (current & (1 << i)) != 0;
                if (ImGui::Checkbox(items[i].c_str(), &isSelected)) {
                    // Toggle the bit based on selection state
                    if (isSelected) {
                        current |= (1 << i);  // Set the i-th bit
                    }
                    else {
                        current &= ~(1 << i); // Clear the i-th bit
                    }
                }
            }
            ImGui::EndPopup();
        }
    }

    void InitStyle()
    {
        auto& style = ImGui::GetStyle();
        style.Alpha = 1.f;
        style.WindowRounding = 0.f;
        style.WindowBorderSize = 0.f;
        style.FrameRounding = 0.f;
        style.ScrollbarRounding = 0.f;
        style.ScrollbarSize = 12.f;

        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.FramePadding = ImVec2(4.f, 2.f);
        style.ItemSpacing = ImVec2(5, 5);

        //restyle
        style.FrameRounding = 2.f;
        style.WindowBorderSize = 0.f;
        style.PopupBorderSize = 0.5f;
        style.ItemSpacing = ImVec2(4.f, 6.f);
        style.ItemInnerSpacing = ImVec2(4.f, 4.f);
        style.GrabMinSize = 16.f;
        style.FramePadding = ImVec2(4.f, 1.f);
        style.GrabMinSize = 1.f;

        auto& colors = style.Colors;

        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        //colors[ImGuiCol_Triangle] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
        //colors[ImGuiCol_ButtonText] = ImVec4(0.f, 0.f, 0.f, 1.00f);

        colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.1f, 0.1f, 0.2f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.2f);
        //colors[ImGuiCol_ButtonOutline] = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);

        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
        //colors[ImGuiCol_MenuBarText] = ImVec4(1.f, 1.f, 1.f, 1.f);
        //colors[ImGuiCol_MenuItemHovered] = ImVec4(0.8f, 0.1f, 0.1f, 1.f);
        //colors[ImGuiCol_MenuItemActive] = ImVec4(0.8f, 0.1f, 0.1f, 1.f);
        colors[ImGuiCol_Header] = ImVec4(0.8f, 0.1f, 0.1f, 0.50f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.8f, 0.1f, 0.1f, 0.40f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.8f, 0.1f, 0.1f, 0.67f);

        style.Alpha = 0.95f;
        colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f);

        colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

        //colors[ImGuiCol_ButtonText] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);

        colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
        //colors[ImGuiCol_ButtonOutline] = ImVec4(0.8f, 0.8f, 0.8f, 0.8f);

        colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.1f, 0.1f, 1.f);
        /*colors[ImGuiCol_CheckBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
        colors[ImGuiCol_CheckBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
        colors[ImGuiCol_CheckBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);*/

        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.67f);

        /*colors[ImGuiCol_SliderBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
        colors[ImGuiCol_SliderBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
        colors[ImGuiCol_SliderBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);*/
        colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.26f, 0.26f, 0.8f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);

        /*colors[ImGuiCol_ListBoxItem] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
        colors[ImGuiCol_ListBoxItemActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
        colors[ImGuiCol_ListBoxItemHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);*/

        colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.98f);
            
    }

    void Draw()
    {
        static std::vector<std::string> items = { "Linear", "Constant" };
        static std::vector<std::string> hitbox_items = { "Head", "Chest", "Arms", "Legs", "Nearest In Air" };
  
        ImGui::Begin("Sensum", 0, ImGuiWindowFlags_NoCollapse);

        ImGui::Checkbox("Box", &settings::visuals::m_bBoxEsp); ImGui::SameLine();
        ImGui::ColorEdit3("###boxcolor", (float*)&settings::visuals::m_fBoxColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("Name", &settings::visuals::m_bNameEsp); ImGui::SameLine();
        ImGui::ColorEdit3("###namecolor", (float*)&settings::visuals::m_fNameColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("Health", &settings::visuals::m_bHealthEsp); ImGui::SameLine();
        ImGui::ColorEdit3("###healthcolor", (float*)&settings::visuals::m_fHealthColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox("Bone esp", &settings::visuals::m_bBoneEsp); ImGui::SameLine();
        ImGui::ColorEdit3("###bonecolor", (float*)&settings::visuals::m_fBoneColor, ImGuiColorEditFlags_NoInputs);

        ImGui::SliderInt("Aimbot FOV", &settings::visuals::aimbot_fov, 0, 360);

        ShowMultiSelectPopup("Hitboxes", settings::visuals::aimbot_hitbox, hitbox_items);
        ImGui::SliderFloat("###AimbotSmooth", &settings::visuals::smooth, 1.f, 10.f, "Smooth: %.1f");
        ShowSmoothModePopup("Smooth Mode", settings::visuals::smooth_mode, items);

        ImGui::SliderFloat("Recoil Pitch", &settings::visuals::pitch, 0.f, 2.f, "Recoil Pitch: %.1f");
        ImGui::SliderFloat("Recoil Yaw", &settings::visuals::yaw, 0.f, 2.f, "Recoil Yaw: %.1f");
        
        ImGui::Checkbox("Fov changer", &settings::visuals::m_bFovChanger);

        if (settings::visuals::m_bFovChanger)
            ImGui::SliderInt("Fov slider", &settings::visuals::m_iFov, 10, 200);

        ImGui::SliderFloat("##boxrounding", &settings::visuals::box_rounding, 0.f, 2.f, "Box Rounding: %.1f");
        ImGui::SliderFloat("##boxthickness", &settings::visuals::box_thickness, 0.f, 2.f, "Box Thickness: %.1f");

        ImGui::Checkbox("Unhook", &globals::can_unhook);

        ImGui::End();
    }
}
