#include "tabs.h"
#include "../imgui_custom.h"
#include "../menu.h"

namespace tabs::aimbot_tab
{
    void display()
    {
        static auto table_flags = ImGuiTableFlags_NoBordersInBody | ImGuiTableColumnFlags_NoResize;
    	if (ImGui::BeginTable("table1", 3, table_flags))
    	{
    		static bool button0 = false;
    		static bool button1 = false;
    		static bool button2 = false;
    		
    		for (int column = 0; column < 3; column++)
    		{
    			ImGui::TableNextColumn();
    			ImGui::AlignTextToFramePadding();
     
    			if (column == 0)
    			{
    				imc::separator("General [WIP]");
    				imc::checkbox("Enabled", &button0);
    			}
     
    			if (column == 1)
    			{
    				imc::separator("Configuration [WIP]");
    				imc::checkbox("Nearest Hitbox in Air", &button1);
    			}
     
    			if (column == 2)
    			{
    				imc::separator("Assistance [WIP]");
    				imc::checkbox("Recoil Control System", &button2);
    			}
    		}
     
    		ImGui::EndTable();
    	}
    }
}
