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
			auto& style = ImGui::GetStyle();
			auto& colors = style.Colors;

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
					ImGui::SameLine();
					ColorEdit4("Real Angle##chams", &settings::chams::localplayer::desync_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Planted C4      ##chams", &settings::chams::misc::color_planted_bomb_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Grenades##chams", &settings::chams::misc::color_nade_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Dropped Weapons##chams", &settings::chams::misc::color_dropped_weapons_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					ColorEdit4("Dropped Defuse Kits     ##chams", &settings::chams::misc::color_dropped_defusekit_chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

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
					ColorEdit4("Sound ESP", &settings::esp::colorSoundEsp, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

					separator("Glow");

					ColorEdit4("Enemy", &settings::glow::glowEnemyColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Planted C4##glow", &settings::glow::glowC4PlantedColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Weapons##glow", &settings::glow::glowDroppedWeaponsColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					tooltip("Dropped Weapons");
					ColorEdit4("Team  ", &settings::glow::glowTeamColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Grenades  ##glow", &settings::glow::glowNadesColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Kits##glow", &settings::glow::glowDroppedKitsColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			});

			ImGui::NextColumn();

			child("Menu Theme", [&]()
			{
					ColorEdit4("Window BG", &colors[ImGuiCol_WindowBg], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Text", &colors[ImGuiCol_Text], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Text Disabled", &colors[ImGuiCol_TextDisabled], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Button Text", &colors[ImGuiCol_ButtonText], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Button Hovered", &colors[ImGuiCol_ButtonHovered], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Button Active", &colors[ImGuiCol_ButtonActive], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Button Outline", &colors[ImGuiCol_ButtonOutline], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Check Mark", &colors[ImGuiCol_CheckMark], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Check BG", &colors[ImGuiCol_CheckBg], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Check BG Active", &colors[ImGuiCol_CheckBgActive], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Check BG Hovered", &colors[ImGuiCol_CheckBgHovered], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Frame BG", &colors[ImGuiCol_FrameBg], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Frame BG Hovered", &colors[ImGuiCol_FrameBgHovered], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					ColorEdit4("Frame BG Active", &colors[ImGuiCol_FrameBgActive], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					ColorEdit4("Slider BG", &colors[ImGuiCol_SliderBg], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
					
					/*colors[ImGuiCol_SliderBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
					colors[ImGuiCol_SliderBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
					colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.26f, 0.26f, 0.8f);
					colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);

					colors[ImGuiCol_ListBoxItem] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
					colors[ImGuiCol_ListBoxItemActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
					colors[ImGuiCol_ListBoxItemHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.f);

					colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.98f);*/
			});
		}
	}
}