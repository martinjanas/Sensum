#include "../render.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../helpers/imdraw.h"
#include "../../helpers/console.h"
#include "../..//features/features.h"

extern void bind_button(const char* eng, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		void visuals_tab()
		{
			child(strings::colors_esp.c_str(), []()
			{
				columns(2);
				{
					checkbox(strings::enabled.c_str(), &settings::esp::enabled);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					hotkey("##binds.esp", &globals::binds::esp);
					ImGui::PopItemWidth();
				}
				columns(1);

				checkbox(strings::visuals_esp_visible.c_str(), &settings::esp::visible_only);

				checkbox(strings::visuals_esp_name.c_str(), &settings::esp::names);

				columns(2);
				{
					checkbox(strings::visuals_esp_weapon.c_str(), &settings::esp::weapons);

					ImGui::NextColumn();

					const char* weapon_modes[] = {
					strings::visuals_esp_mode_text.c_str(),
					strings::visuals_esp_mode_icon.c_str()
					};

					ImGui::PushItemWidth(-1);
					{
						ImGui::Combo(strings::visuals_esp_mode.c_str(), &settings::esp::weapon_mode, weapon_modes, IM_ARRAYSIZE(weapon_modes));
					}
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_playerinfo.c_str(), &settings::visuals::player_info_box);

					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					{
						ImGui::SliderFloatLeftAligned((strings::visuals_esp_alpha + "##infobox").c_str(), &settings::visuals::player_info_box_alpha, 0.0f, 1.0f, "%0.1f");
					}
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_grief.c_str(), &settings::visuals::grief_box);

					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					{
						ImGui::SliderFloatLeftAligned((strings::visuals_esp_alpha + "##griefbox").c_str(), &settings::visuals::grief_box_alpha, 0.0f, 1.0f, "%0.1f");
					}
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_box.c_str(), &settings::esp::boxes);

					ImGui::NextColumn();

					const char* box_types[] = {
						strings::visuals_esp_box_normal.c_str(), strings::visuals_esp_box_corner.c_str()
					};

					ImGui::PushItemWidth(-1);
					{
						ImGui::Combo("##esp.box_type", &settings::esp::box_type, box_types, IM_ARRAYSIZE(box_types));
					}
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				const char* positions[] =
				{
					strings::visuals_esp_pos_left.c_str(),
					strings::visuals_esp_pos_right.c_str(),
					strings::visuals_esp_pos_bottom.c_str()
				};

				const char* HealthPositions[] =
				{
					strings::visuals_esp_pos_left.c_str(),
					strings::visuals_esp_pos_right.c_str(),
					strings::visuals_esp_pos_bottom.c_str(),
					strings::visuals_esp_pos_num.c_str()
				};

				columns(2);
				{
					checkbox(strings::visuals_esp_hp.c_str(), &settings::esp::health);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("##health.position", &settings::esp::health_position, HealthPositions, IM_ARRAYSIZE(HealthPositions));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_armor.c_str(), &settings::esp::armour);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo("##armor.position", &settings::esp::armour_position, positions, IM_ARRAYSIZE(positions));
					ImGui::PopItemWidth();
				}
				columns(1);

				//checkbox("Dormant", &Settings::ESP::dormant);
				checkbox(strings::visuals_esp_isscoped.c_str(), &settings::esp::is_scoped);
				checkbox(strings::visuals_esp_isflashed.c_str(), &settings::esp::is_flashed);
				checkbox(strings::visuals_esp_isdefusing.c_str(), &settings::esp::is_defusing);
				checkbox(strings::visuals_esp_isdesync.c_str(), &settings::esp::is_desyncing);
				checkbox(strings::visuals_esp_haskit.c_str(), &settings::esp::haskit);
				checkbox(strings::visuals_esp_ammoesp.c_str(), &settings::esp::ammo);
				checkbox(strings::visuals_esp_moneyesp.c_str(), &settings::esp::money);
				checkbox(strings::visuals_esp_chokeesp.c_str(), &settings::visuals::choke);
				checkbox(strings::visuals_esp_soundesp.c_str(), &settings::esp::soundesp);

				//checkbox("Beams", u8"Лучи света", &settings::esp::beams); //Doesnt work.
				//checkbox("Sound Direction (?)", &settings::esp::sound); //Doesnt work.
				//tooltip("Sound ESP", u8"Показывает стрелками направление звука, откуда слышно игрока.");

				checkbox(strings::visuals_esp_bonbdmesp.c_str(), &settings::esp::bomb_esp);
				checkbox(strings::visuals_esp_offscreenesp.c_str(), &settings::esp::offscreen);
			});

			ImGui::NextColumn();

			child(strings::colors_chams.c_str(), []()
			{
				const char* ChamsTypes[] = {
				strings::visuals_esp_chams_visnorm.c_str(),
				strings::visuals_esp_chams_visflat.c_str(),
				strings::visuals_esp_chams_viswire.c_str(),
				strings::visuals_esp_chams_visgrass.c_str(),
				strings::visuals_esp_chams_vismetal.c_str(),
				strings::visuals_esp_chams_xqz.c_str(),
				strings::visuals_esp_chams_metalxqz.c_str(),
				strings::visuals_esp_chams_flatxqz.c_str()
				};

				const char* bttype[] = {
				strings::visuals_esp_chams_off.c_str(),
				strings::visuals_esp_chams_lasttick.c_str(),
				strings::visuals_esp_chams_allticks.c_str()
				};

				const char* chamsMaterials[] = {
				strings::visuals_esp_chams_normal.c_str(),
				strings::visuals_esp_chams_dogtag.c_str(),
				strings::visuals_esp_chams_flat.c_str(),
				strings::visuals_esp_chams_metal.c_str(),
				strings::visuals_esp_chams_plat.c_str(),
				strings::visuals_esp_chams_glass.c_str(),
				strings::visuals_esp_chams_cryst.c_str(),
				strings::visuals_esp_chams_gold.c_str(),
				strings::visuals_esp_chams_darkcr.c_str(),
				strings::visuals_esp_chams_plast.c_str(),
				strings::visuals_esp_chams_glow.c_str()
				};

				columns(2);
				{
					checkbox(strings::visuals_esp_chams_enemy.c_str(), &settings::chams::enemynew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo(strings::visuals_esp_chams_enemy_mode.c_str(), &settings::chams::enemymodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_chams_team.c_str(), &settings::chams::teamnew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo(strings::visuals_esp_chams_team_mode.c_str(), &settings::chams::teammodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox(strings::visuals_esp_chams_local.c_str(), &settings::chams::localnew);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo(strings::visuals_esp_chams_local_mode.c_str(), &settings::chams::localmodenew, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox(strings::colors_realangle.c_str(), &settings::chams::desync);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo(strings::visuals_esp_chams_material.c_str(), &settings::chams::desyncChamsMode, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
					ImGui::PopItemWidth();
				}
				columns(1);

				//checkbox("Viewmodel Weapons", &settings::chams::wepchams);
				ImGui::SameLine();
				checkbox(strings::colors_glowplanted.c_str(), &settings::chams::plantedc4_chams);
				checkbox(strings::colors_glowdroppedweapons.c_str(), &settings::chams::wep_droppedchams);
				ImGui::SameLine();
				checkbox(strings::colors_glowgrenades.c_str(), &settings::chams::nade_chams);
				checkbox(strings::visuals_esp_chams_health.c_str(), &settings::chams::health_chams);

				//separator("BT Chams - Mode");
				//ImGui::Combo("BT Chams Mode", &settings::chams::bttype, bttype, IM_ARRAYSIZE(bttype));
				//checkbox("BT Chams - Flat", &settings::chams::btflat);
				//ColorEdit4("BT Color", &settings::chams::btColorChams);

				/*separator("Arms", u8"Руки");

				checkbox("Enabled##arms", u8"Включено##arms", &settings::chams::arms::enabled);
				checkbox("Wireframe##arms", u8"Сетка##arms", &settings::chams::arms::wireframe);

				ImGui::Separator();

				ColorEdit4(___("Visible", u8"Видимый"), &settings::chams::visible_color);
				ColorEdit4(___("Occluded", u8"За преградой"), &settings::chams::occluded_color);

				ColorEdit4(___("Arms", u8"Руки"), &settings::chams::arms::color); */

				child(strings::visuals_esp_glow.c_str(), []()
				{
					checkbox(strings::visuals_esp_chams_enemy.c_str(), &settings::glow::glowEnemyEnabled);
					ImGui::SameLine();
					checkbox(strings::colors_glowplanted.c_str(), &settings::glow::glowC4PlantedEnabled);
					ImGui::SameLine();
					checkbox(strings::colors_glowgrenades.c_str(), &settings::glow::glowNadesEnabled);
					checkbox(strings::visuals_esp_chams_team.c_str(), &settings::glow::glowTeamEnabled);
					ImGui::SameLine();
					checkbox(strings::colors_glowdroppedweapons.c_str(), &settings::glow::glowDroppedWeaponsEnabled);
				});
			});

			ImGui::NextColumn();

			child(strings::visuals_extra.c_str(), []()
			{
				const char* cross_types[] = {
					strings::visuals_extra_crosstypes_cross.c_str(),
					strings::visuals_extra_crosstypes_circle.c_str(),
				};

				const char* hitmarkersounds[] = {
					strings::visuals_extra_sound_cod.c_str(),
					strings::visuals_extra_sound_skeet.c_str(),
					strings::visuals_extra_sound_punch.c_str(),
					strings::visuals_extra_sound_metal.c_str(),
					strings::visuals_extra_sound_boom.c_str()
				};

				checkbox(strings::visuals_extra_buylog.c_str(), &settings::esp::buylog);
				checkbox(strings::colors_glowplanted.c_str(), &settings::visuals::planted_c4);
				checkbox(strings::visuals_extra_defuse.c_str(), &settings::visuals::defuse_kit);
				checkbox(strings::visuals_extra_worldweap.c_str(), &settings::visuals::dropped_weapons);
				checkbox(strings::visuals_extra_worldnade.c_str(), &settings::visuals::world_grenades);
				checkbox(strings::visuals_extra_snipercross.c_str(), &settings::visuals::sniper_crosshair);
				checkbox(strings::visuals_extra_snaplines.c_str(), &settings::esp::snaplines);
				checkbox(strings::visuals_extra_armorstatus.c_str(), &settings::esp::kevlarinfo);
				tooltip(strings::visuals_extra_armorstatus_tt.c_str());
				checkbox(strings::visuals_extra_nadepred.c_str(), &settings::visuals::grenade_prediction);
				checkbox(strings::visuals_extra_dmgind.c_str(), &settings::misc::damage_indicator);
				checkbox(strings::colors_aimbotfov.c_str(), &settings::esp::drawFov);
				checkbox(strings::colors_spreadcross.c_str(), &settings::visuals::spread_cross);
				checkbox(strings::visuals_extra_tracer.c_str(), &settings::visuals::bullet_tracer);
				checkbox(strings::visuals_extra_hitmarker.c_str(), &settings::visuals::hitmarker);
				ImGui::Combo(strings::visuals_extra_hitsound.c_str(), &settings::visuals::hitsound, hitmarkersounds, IM_ARRAYSIZE(hitmarkersounds));
				checkbox(strings::visuals_extra_rcs.c_str(), &settings::visuals::rcs_cross);
				ImGui::Combo(strings::visuals_extra_rcstype.c_str(), &settings::visuals::rcs_cross_mode, cross_types, IM_ARRAYSIZE(cross_types));
				if (settings::visuals::rcs_cross_mode == 1)
					ImGui::SliderFloatLeftAligned(strings::visuals_extra_radius.c_str(), &settings::visuals::radius, 8.f, 18.f, "%.1f");

				const auto old_night_state = settings::visuals::night_mode;
				const auto old_style_state = settings::visuals::newstyle;
				checkbox(strings::visuals_extra_nightmode.c_str(), &settings::visuals::night_mode);

				if (settings::visuals::night_mode)
				{
					ImGui::SliderFloatLeftAligned(strings::visuals_extra_nightinten.c_str(), &settings::esp::mfts, 0.0f, 1.0f, "%.1f %");

					if (ImGui::Button(strings::visuals_extra_apply.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
					{
						color_modulation::SetMatForce();
					}
				}

				checkbox(strings::visuals_extra_espchance.c_str(), &settings::misc::esp_random);
				tooltip(strings::visuals_extra_espchance_tt.c_str());

				if (settings::misc::esp_random)
				{
					ImGui::SliderIntLeftAligned(strings::visuals_extra_espchance.c_str(), &settings::esp::esp_chance, 1, 100, "%.0f %%");
					tooltip(strings::visuals_extra_espchance_tt.c_str());
				}

				checkbox(strings::visuals_extra_darkmenu.c_str(), &settings::visuals::newstyle);
				if (old_style_state != settings::visuals::newstyle) //settings::visuals::night_mode
					imdraw::apply_style(settings::visuals::newstyle);
			});
		}
	}
}
