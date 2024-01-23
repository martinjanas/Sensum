#include "../render.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../helpers/imdraw.h"
#include "../../helpers/console.h"
#include "../..//features/features.h"
#include "../menu_strings.h"

extern void bind_button(const char* label, int& key);
extern bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));


namespace render
{
	namespace menu
	{
		void visuals_tab()
		{
			child("ESP", []()
				{
					columns(2);
					{
						checkbox("Enabled", &settings::esp::enabled);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						Hotkey("##binds.esp", &globals::binds::esp);
						ImGui::PopItemWidth();
					}
					columns(1);

					checkbox("Visible Only", &settings::esp::visible_only);

					if (settings::esp::visible_only)
						checkbox("Sound Visible", &settings::esp::visible_on_sound);
					
					checkbox("Name", &settings::esp::names);

					columns(2);
					{
						checkbox("Weapon", &settings::esp::weapons);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						{
							ImGui::Combo("Mode", &settings::esp::weapon_mode, render::visual_tab::weapon_modes, IM_ARRAYSIZE(render::visual_tab::weapon_modes));
						}
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);

					columns(2);
					{
						checkbox("Player Info Box", &settings::visuals::player_info_box);

						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						{
							ImGui::SliderFloatLeftAligned("Alpha##infobox", &settings::visuals::player_info_box_alpha, 0.0f, 1.0f, "%0.1f");
						}
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);

					columns(2);
					{
						checkbox("Boxes", &settings::esp::boxes);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						{
							ImGui::Combo("##esp.box_type", &settings::esp::box_type, render::visual_tab::box_types, IM_ARRAYSIZE(render::visual_tab::box_types));
						}
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);

					
					columns(2);
					{
						checkbox("Health", &settings::esp::health);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("##health.position", &settings::esp::health_position, render::visual_tab::positions, IM_ARRAYSIZE(render::visual_tab::positions));
						ImGui::PopItemWidth();
					}
					columns(1);

					columns(2);
					{
						checkbox("Armor", &settings::esp::armour);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("##armor.position", &settings::esp::armour_position, render::visual_tab::positions, IM_ARRAYSIZE(render::visual_tab::positions));
						ImGui::PopItemWidth();
					}
					columns(1);

					checkbox("Bone ESP", &settings::esp::bone_esp);
					checkbox("Is Scoped", &settings::esp::is_scoped);
					checkbox("Is Flashed", &settings::esp::is_flashed);
					checkbox("Is Defusing", &settings::esp::is_defusing);
					checkbox("Ammo ESP", &settings::esp::ammo);
					checkbox("Bomb Carrier ESP", &settings::esp::bomb_esp);
					checkbox("Offscreen ESP", &settings::esp::offscreen);
				});

			ImGui::NextColumn();

			child("Chams", []()
				{
					ImGui::Combo("List##chams", &settings::chams::chams_list_mode, render::visual_tab::chams_list, IM_ARRAYSIZE(render::visual_tab::chams_list));

					bool b_wip = false;

					switch (settings::chams::chams_list_mode)
					{
					case 0:
						columns(2);
						{
							checkbox("Enabled", &settings::chams::enemy::enabled);

							checkbox("Health", &settings::chams::enemy::health_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Visible only", &settings::chams::enemy::visible_only);
							ImGui::PopItemWidth();
						}
						columns(1);
						break;
					case 1:
						columns(2);
						{
							checkbox("Enabled", &settings::chams::teammates::enabled);

							checkbox("Health", &settings::chams::teammates::health_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Visible only", &settings::chams::teammates::visible_only);
							ImGui::PopItemWidth();
						}
						columns(1);
						break;
					}
				});

			ImGui::NextColumn();

			child("Extra", []()
				{
					checkbox("Buy Log", &settings::esp::buylog);
					checkbox("Planted C4", &settings::visuals::planted_c4);
					checkbox("World Weapons", &settings::visuals::dropped_weapons);
					checkbox("World Grenades", &settings::visuals::world_grenades);
					checkbox("Sniper Crosshair", &settings::visuals::sniper_crosshair);
					checkbox("Grenade Prediction", &settings::visuals::grenade_prediction);
	
					columns(2);
					{
						checkbox("RCS Crosshair", &settings::visuals::rcs_cross);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("RCS Crosshair Type", &settings::visuals::rcs_cross_mode, render::visual_tab::cross_types, IM_ARRAYSIZE(render::visual_tab::cross_types));
						ImGui::PopItemWidth();
					}
					columns(1);

					if (settings::visuals::rcs_cross_mode == 1)
						ImGui::SliderFloatLeftAligned("Radius", &settings::visuals::radius, 8.f, 18.f, "%.1f");

					checkbox("Night Mode", &settings::visuals::night_mode);
				});
		}
	}
}