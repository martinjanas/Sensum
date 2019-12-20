#include "render/render.h"
#include "globals.h"
#include "settings.h"
#include "helpers/imdraw.h"
#include "helpers/console.h"

extern void bind_button(const char* eng, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		void colors_tab()
		{
			separator(strings::colors_chams.c_str());
			ColorEdit4(strings::colors_enemyvisible.c_str(), &settings::chams::EnemyColor_vis, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_enemyxqz.c_str(), &settings::chams::EnemyColor_XQZ, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ColorEdit4(strings::colors_teamvisible.c_str(), &settings::chams::TeamColor_vis, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_teamyxqz.c_str(), &settings::chams::TeamColor_XQZ, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ColorEdit4(strings::colors_localvisible.c_str(), &settings::chams::LocalColor_vis, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_localxqz.c_str(), &settings::chams::LocalColor_XQZ, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ColorEdit4((strings::colors_planted + "##chams").c_str(), &settings::chams::colorPlantedC4Chams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();

			ColorEdit4((strings::colors_droppedweapons + "##chams").c_str(), &settings::chams::ColorWeaponDroppedChams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ColorEdit4((strings::colors_grenades + "##chams").c_str(), &settings::chams::colorNadeChams, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4((strings::colors_realangle + "##chams").c_str(), &settings::chams::desync_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			separator(strings::colors_esp.c_str());
			ColorEdit4(strings::colors_espvisible.c_str(), &settings::esp::visibleColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_espinvisible.c_str(), &settings::esp::occludedColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ColorEdit4(strings::colors_rcscross.c_str(), &settings::visuals::recoilcolor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_spreadcross.c_str(), &settings::visuals::spread_cross_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ColorEdit4(strings::colors_aimbotfov.c_str(), &settings::visuals::drawfov_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4(strings::colors_bullettracer.c_str(), &settings::visuals::colorBulletTracer, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			tooltip(strings::colors_bullettracer_tt.c_str());
			ColorEdit4(strings::colors_aaindicator.c_str(), &settings::esp::aa_indicator_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			separator(strings::colors_glow.c_str());

			ColorEdit4(strings::colors_glowenemy.c_str(), &settings::glow::glowEnemyColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4((strings::colors_glowplanted + "##glow").c_str(), &settings::glow::glowC4PlantedColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4((strings::colors_glowdroppedweapons + "##glow").c_str(), &settings::glow::glowDroppedWeaponsColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ColorEdit4(strings::colors_glowteam.c_str(), &settings::glow::glowTeamColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			ImGui::SameLine();
			ColorEdit4((strings::colors_glowgrenades + "##glow").c_str(), &settings::glow::glowNadesColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

			ImGui::NextColumn();

			separator(strings::colors_glowoverride.c_str());

			checkbox(strings::colors_glowoverrideenable.c_str(), &settings::glow::glowOverride);
			if (settings::glow::glowOverride)
			{
				ColorEdit4((strings::colors_glowoverridesmoke + "##glow").c_str(), &settings::glow::glowSmoke, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
				ColorEdit4((strings::colors_glowoverridemolotov + "##glow").c_str(), &settings::glow::glowMolotovIncendiary, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
				ColorEdit4((strings::colors_glowoverridehe + "##glow").c_str(), &settings::glow::glowHE, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
				ColorEdit4((strings::colors_glowoverridedroppedc4 + "##glow").c_str(), &settings::glow::glowDroppedC4Color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
			}
		}
	}
}
