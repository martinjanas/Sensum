#include "menu.h"
#include "../sdk/helpers/globals.h"
#include "../sdk/helpers/console.h"
#include "../render/imgui_custom.h"

#include "../settings/settings.h"


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

void draw_menu_debug()
{
	static std::vector<std::string> hitbox_items = { "Head", "Chest", "Arms", "Legs", "Nearest In Air" };

	ImGui::PushFont(render::fonts::selector);
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
	
	ImGui::SliderFloat("Recoil Pitch", &settings::visuals::pitch, 0.f, 2.f, "Recoil Pitch: %.1f");
	ImGui::SliderFloat("Recoil Yaw", &settings::visuals::yaw, 0.f, 2.f, "Recoil Yaw: %.1f");

	ImGui::Checkbox("Fov changer", &settings::visuals::m_bFovChanger);

	if (settings::visuals::m_bFovChanger)
		ImGui::SliderInt("Fov slider", &settings::visuals::m_iFov, 10, 200);

	ImGui::SliderFloat("##boxrounding", &settings::visuals::box_rounding, 0.f, 2.f, "Box Rounding: %.1f");
	ImGui::SliderFloat("##boxthickness", &settings::visuals::box_thickness, 0.f, 2.f, "Box Thickness: %.1f");
	ImGui::Checkbox("BHop", &settings::misc::m_bBHop);
	ImGui::Checkbox("Unhook", &globals::can_unhook);
	ImGui::PopFont();
}

namespace menu
{
	bool is_open = false;
	float window_alpha = 0.f;
	int current_tab = 0;

	bool was_logo_pressed = false;

	static const ImVec2 window_size(800.f, 500.f);

	static ImVec4 from_hsb(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f; // Normalize hue
		float f = h - static_cast<int>(h); // Fractional part of hue
		float p = brightness * (1.0f - saturation); // Chroma calculation
		float q = brightness * (1.0f - saturation * f); // Intermediate value
		float t = brightness * (1.0f - (saturation * (1.0f - f))); // Intermediate value

		int hh = static_cast<int>(floor(h)); // Integer part of hue
		switch (hh) {
		case 0:
			return ImVec4(brightness, t, p, 1.0f); // Red to Yellow
		case 1:
			return ImVec4(q, brightness, p, 1.0f); // Yellow to Green
		case 2:
			return ImVec4(p, brightness, t, 1.0f); // Green to Cyan
		case 3:
			return ImVec4(p, q, brightness, 1.0f); // Cyan to Blue
		case 4:
			return ImVec4(t, p, brightness, 1.0f); // Blue to Magenta
		default:
			return ImVec4(brightness, p, q, 1.0f); // Magenta to Red
		}
	}

	void draw()
	{
		if (window_alpha <= 0.01f)
			return;

		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		static auto window_center = ImVec2(globals::width * 0.5f, globals::height * 0.5f);

		draw_background();

		auto* style = &ImGui::GetStyle();

		const auto old_alpha = style->Alpha;
		
		ImGui::GetStyle().Alpha = window_alpha;

		static auto window_pos_center = ImVec2(window_center.x - (window_size.x / 2.f), window_center.y - (window_size.y / 2.f));

		ImGui::SetNextWindowPos(window_pos_center, ImGuiCond_Once);
		ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

		const auto old_window_padding = style->WindowPadding;
		style->WindowPadding = ImVec2(0, 0);

		ImGui::Begin("Sensum", nullptr, flags);
		{
			render_header();

			style->WindowPadding = old_window_padding;

			imgui::BeginChild("##tabscontent", { 0, 0 }, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
			{
				imgui::Columns(3, nullptr, false);
				{
					static const char* items[] = { "Linear", "Constant" };

					if (was_logo_pressed)
					{
						draw_menu_debug();
					}

					if (current_tab == 0)
					{
						imc::selector("Smoothing Mode", items, 2, &settings::visuals::smooth_mode);
					}
					else if (current_tab == 1)
						imgui::GetForegroundDrawList()->AddText(window_center, IM_COL32_WHITE, "Visuals Tab");
					else if (current_tab == 2)
						imgui::GetForegroundDrawList()->AddText(window_center, IM_COL32_WHITE, "Misc Tab");
					else if (current_tab == 3)
						imgui::GetForegroundDrawList()->AddText(window_center, IM_COL32_WHITE, "Skins Tab");
					else if (current_tab == 4)
						imgui::GetForegroundDrawList()->AddText(window_center, IM_COL32_WHITE, "Players Tab");
					else if (current_tab == 5)
						imgui::GetForegroundDrawList()->AddText(window_center, IM_COL32_WHITE, "Config Tab");
				}
				imgui::Columns(1);
			}
			imgui::EndChild();
		}
		ImGui::End();

		ImGui::GetStyle().Alpha = old_alpha;
	}

	void draw_background()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.09f, 0.09f, 0.40f / 1.f * window_alpha));
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
		ImGui::Begin("##background", nullptr, flags);
		ImGui::End();
		ImGui::PopStyleColor();
	}

	void render_header()
	{
		auto win_pos = ImGui::GetWindowPos();
		auto draw_list = ImGui::GetWindowDrawList();

		if (!render::fonts::header_title || !render::fonts::header_buttons)
			return;

		ImVec2 title_size;
		ImGui::PushFont(render::fonts::header_title);
		{
			static const auto white = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 1.f));

			title_size = ImGui::CalcTextSize("Sensum");
			const auto rect_start = ImVec2(win_pos.x, win_pos.y);
			const auto rect_end = ImVec2(win_pos.x + window_size.x, win_pos.y + title_size.y + 16.f);
			const auto rect_height = rect_end.y - rect_start.y;

			draw_list->AddRectFilled(rect_start, rect_end, ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f)));

			ImGui::SetCursorPosX(10.f);
			ImGui::SetCursorPosY(rect_start.y + rect_height / 2 - win_pos.y - title_size.y / 2);

			static float _rainbow = 0.f;
			if (_rainbow > 1.f)
				_rainbow = 0.f;

			auto cursor = ImGui::GetCursorPos();

			ImGui::Text("");

			ImGui::SetCursorScreenPos({ win_pos.x + cursor.x, win_pos.y + cursor.y }); // Set cursor to starting point of text
			ImGui::InvisibleButton("header_button", title_size); // Define invisible button over text

			if (ImGui::IsItemClicked())
				was_logo_pressed = !was_logo_pressed;

			auto rainbow = _rainbow;

			float delta_time = ImGui::GetIO().DeltaTime;

			float rainbow_offset = 0.5f * delta_time;
			float rainbow_offset2 = 0.17 * delta_time;
			
			for (auto& letter : "Sensum")
			{
				const auto rgb_color = from_hsb(rainbow, 1.f, 1.f);

				auto color = rgb_color;

				char buf[4];
				sprintf_s(buf, "%c", letter);

				ImVec2 text_pos = { win_pos.x + cursor.x, win_pos.y + cursor.y };
				text_pos.y += 1.f;

				draw_list->AddText(text_pos, ImGui::GetColorU32(color), buf);

				cursor.x += 0.001f + ImGui::CalcTextSize(buf).x;
				rainbow += rainbow_offset;

				if (rainbow > 1.f)
					rainbow = 0.f;
			}

			_rainbow += rainbow_offset2;
		}
		ImGui::PopFont();

		ImGui::SameLine();

		ImGui::PushFont(render::fonts::header_buttons);
		{
			static const auto menu_items = std::vector<std::string>
			{
				"Aimbot",
				"Visuals",
				"Misc",
				"Skins",
				"Players",
				"Configs"
			};

			const auto menu_items_count = menu_items.size();

			const auto button_width = 72.f;
			const auto button_height = 24.f;
			const auto total_button_space = button_width * menu_items_count + 40.f * menu_items_count;
			ImGui::SetCursorPosX(window_size.x - total_button_space);
	
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.1f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.f));
			
			auto outline_color = ImVec4(0.8f, 0.1f, 0.1f, 1.f);
			float outline_thickness = 0.5f;

			auto& button_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

			for (size_t k = 0; k < menu_items_count; k++)
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);

				ImVec2 button_pos = ImGui::GetCursorScreenPos();
				ImVec2 button_size = ImVec2(button_width, button_height);

				draw_list->AddRect(button_pos, { button_pos.x + button_size.x, button_pos.y + button_size.y }, ImGui::ColorConvertFloat4ToU32(outline_color), 0.0f, 0, outline_thickness); // Rounded corners

				const auto selected = current_tab == k;
				if (selected)
					ImGui::PushStyleColor(ImGuiCol_Button, button_color);

				if (ImGui::Button(menu_items[k].c_str(), ImVec2(button_width, button_height)))
					current_tab = k;

				if (selected)
					ImGui::PopStyleColor();

				if (k != menu_items_count - 1)
					ImGui::SameLine();
			}

			ImGui::PopStyleColor(3);
		}
		ImGui::PopFont();
	}

	void render_bottom_header()
	{

	}

	void modulate_window_alpha()
	{
		float alpha_offset = 12.f * ImGui::GetIO().DeltaTime;

		if (is_open) 
		{
			if (window_alpha < 1.f) 
				window_alpha += alpha_offset;
		}
		else 
		{
			if (window_alpha > 0.01f) 
				window_alpha -= alpha_offset;
		}

		window_alpha = std::clamp(window_alpha, 0.01f, 1.f);
	}
}
