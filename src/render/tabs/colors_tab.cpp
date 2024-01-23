#include "../../render/render.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../helpers/imdraw.h"
#include "../../helpers/console.h"

extern void bind_button(const char* label, int& key);
extern bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		void colors_tab()
		{
			child("Colors", []()
			{
					separator("Chams");
					ColorEdit4("Enemy Visible", &settings::chams::enemy::color_visible, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Enemy XQZ", &settings::chams::enemy::color_not_visible, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Team Visible  ", &settings::chams::teammates::color_visible, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Team XQZ", &settings::chams::teammates::color_not_visible, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Enemy Backtrack ##chams", &settings::chams::enemy::color_backtrack, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Team Backtrack ##chams", &settings::chams::teammates::color_backtrack, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					separator("ESP");
					ColorEdit4("ESP Visible ", &settings::esp::visible_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("ESP Invisible", &settings::esp::occluded_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Bone ESP", &settings::esp::bone_esp_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("RCS Cross  ", &settings::visuals::recoil_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
			});
		}
	}
}