#include "tabs.h"
#include "../menu.h"

namespace tabs::aimbot_tab
{
    void display()
    {
        static auto table_flags = ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoBordersInBodyUntilResize | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Resizable;
    	if (ImGui::BeginTable("table1", 3, table_flags))
    	{
    		static bool button0 = false;
    		static bool button2 = false;

    		for (int column = 0; column < 3; column++)
    		{
    			ImGui::TableNextColumn();
    			ImGui::AlignTextToFramePadding();
    			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Header").x) / 2.0f);
    			ImGui::Text("Header %d", column + 1);
    		}

    		for (int column = 0; column < 3; column++)
    		{
    			ImGui::TableNextColumn();
    			ImGui::AlignTextToFramePadding();
    			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Column").x) / 2.0f);
    			ImGui::Text("Column %d", column + 1);

    			if (column == 0)
    			{
    				float availableWidth = ImGui::GetContentRegionAvail().x;
    				float buttonWidth = 100.0f;  // Custom button width
    				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (availableWidth - buttonWidth) / 2.0f);
    				if (ImGui::Button("Button##column0"))
    					button0 = !button0;

    				if (button0)
    				{
    					auto str = std::format("Button in column 1");
    					auto text_size = imgui::CalcTextSize(str.c_str());

    					ImGui::AlignTextToFramePadding();
    					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - text_size.x) / 2.0f);
    					ImGui::Text("Button in column %d", column + 1);  // Optional feedback text
    				}
    			}

    			if (column == 2)
    			{
    				float availableWidth = ImGui::GetContentRegionAvail().x;
    				float buttonWidth = 100.0f;  // Custom button width
    				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (availableWidth - buttonWidth) / 2.0f);
    				if (ImGui::Button("Button##column2"))
    					button2 = !button2;

    				if (button2)
    				{
    					auto str = std::format("Button in column 1");
    					auto text_size = imgui::CalcTextSize(str.c_str());

    					ImGui::AlignTextToFramePadding();
    					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - text_size.x) / 2.0f);
    					ImGui::Text("Button in column %d", column + 1);
    				}
    			}
    		}

    		ImGui::EndTable();
    	}
    }
}
