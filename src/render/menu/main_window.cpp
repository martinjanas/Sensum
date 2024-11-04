#include "main_window.h"
#include "../../sdk/helpers/globals.h"
#include <vector>

namespace main_window
{
    bool is_open = false;

    ImFont* esp;

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
