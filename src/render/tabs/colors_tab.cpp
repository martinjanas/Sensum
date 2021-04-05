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

					ColorEdit4("Local                 ", &settings::chams::localplayer::color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Planted C4      ##chams", &settings::chams::misc::color_planted_bomb_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Grenades##chams", &settings::chams::misc::color_nade_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Dropped Weapons##chams", &settings::chams::misc::color_dropped_weapons_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Dropped Defuse Kits     ##chams", &settings::chams::misc::color_dropped_defusekit_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Enemy Backtrack ##chams", &settings::chams::enemy::color_backtrack, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Team Backtrack ##chams", &settings::chams::teammates::color_backtrack, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					separator("ESP");
					ColorEdit4("ESP Visible ", &settings::esp::visibleColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("ESP Invisible", &settings::esp::occludedColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Bone ESP", &settings::esp::bone_esp_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("RCS Cross  ", &settings::visuals::recoilcolor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Spread Cross", &settings::visuals::spread_cross_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Aimbot Fov", &settings::visuals::drawfov_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Bullet Tracer (?)", &settings::visuals::colorBulletTracer, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					tooltip("Disabled saving of Bullet Tracer - Causes bug that will make beams not render :(. Default color is purple.");
					ColorEdit4("AA Indicator", &settings::esp::aa_indicator_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Sound Beams", &settings::esp::colorSoundEsp, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					separator("Glow");

					ColorEdit4("Enemy Visible##glow", &settings::glow::enemy::visible_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Enemy XQZ##glow", &settings::glow::enemy::invisible_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Planted C4      ##glow", &settings::glow::misc::bomb::color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Weapons##glow", &settings::glow::misc::weapons::color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					tooltip("Dropped Weapons");
					ColorEdit4("Team Visible  ##glow", &settings::glow::teammates::visible_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Team XQZ##glow", &settings::glow::teammates::invisible_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Grenades       ##glow", &settings::glow::misc::nades::color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Kits##glow", &settings::glow::misc::kits::color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			});
		}
	}
}