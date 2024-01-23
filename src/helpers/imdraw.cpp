#include "imdraw.h"
#include "../settings/globals.h"

namespace imdraw
{
	void apply_style()
	{
		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleText] = ImVec4(1.f, 1.f, 1.f, 1.f);
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
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_Triangle] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
		colors[ImGuiCol_ButtonText] = ImVec4(0.f, 0.f, 0.f, 1.00f);

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
		colors[ImGuiCol_MenuBarText] = ImVec4(1.f, 1.f, 1.f, 1.f);
		colors[ImGuiCol_MenuItemHovered] = ImVec4(0.8f, 0.1f, 0.1f, 1.f);
		colors[ImGuiCol_MenuItemActive] = ImVec4(0.8f, 0.1f, 0.1f, 1.f);
		colors[ImGuiCol_Header] = ImVec4(0.8f, 0.1f, 0.1f, 0.50f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.8f, 0.1f, 0.1f, 0.40f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.8f, 0.1f, 0.1f, 0.67f);

		style.Alpha = 0.95f;
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.98f);

		colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

		colors[ImGuiCol_ButtonText] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);

		colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
		colors[ImGuiCol_ButtonOutline] = ImVec4(0.8f, 0.8f, 0.8f, 0.8f);

		colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.1f, 0.1f, 1.f);
		colors[ImGuiCol_CheckBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		colors[ImGuiCol_CheckBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		colors[ImGuiCol_CheckBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.67f);

		colors[ImGuiCol_SliderBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		colors[ImGuiCol_SliderBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
		colors[ImGuiCol_SliderBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.26f, 0.26f, 0.8f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);

		colors[ImGuiCol_ListBoxItem] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		colors[ImGuiCol_ListBoxItemActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
		colors[ImGuiCol_ListBoxItemHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);

		colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.98f);
	}

	void outlined_text(const char* text, const ImVec2& pos, const ImU32& color)
	{
		outlined_text(text, pos, color, ImGui::GetColorU32(ImVec4::Black));
	}

	void outlined_text(const char* text, const ImVec2& pos, const ImU32& color, const ImU32& outlined_color)
	{
		globals::draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) + 1.f), outlined_color, text);
		//globals::draw_list->AddText(ImVec2((pos.x) - 1.f, (pos.y) - 1.f), outlined_color, text);
		globals::draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) - 1.f), outlined_color, text);
		//globals::draw_list->AddText(ImVec2((pos.x) - 1.f, (pos.y) + 1.f), outlined_color, text);

		globals::draw_list->AddText(pos, color, text);
	}
}