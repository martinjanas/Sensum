#include "../render.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../helpers/imdraw.h"
#include "../../helpers/console.h"
#include "../..//features/features.h"

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

					checkbox("Name", &settings::esp::names);

					columns(2);
					{
						checkbox("Weapon", &settings::esp::weapons);

						ImGui::NextColumn();

						const char* weapon_modes[] = {
						"Text",
						"Icons"
						};

						ImGui::PushItemWidth(-1);
						{
							ImGui::Combo("Mode", &settings::esp::weapon_mode, weapon_modes, IM_ARRAYSIZE(weapon_modes));
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
						checkbox("Grief Box", &settings::visuals::grief_box);

						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						{
							ImGui::SliderFloatLeftAligned("Alpha##griefbox", &settings::visuals::grief_box_alpha, 0.0f, 1.0f, "%0.1f");
						}
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);

					columns(2);
					{
						checkbox("Boxes", &settings::esp::boxes);

						ImGui::NextColumn();

						const char* box_types[] = {
							"Normal",
							"Corner"
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
						"Left",
						"Right",
						"Bottom"
					};

					const char* HealthPositions[] =
					{
						"Left",
						"Right",
						"Bottom",
						"Number"
					};

					columns(2);
					{
						checkbox("Health", &settings::esp::health);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("##health.position", &settings::esp::health_position, HealthPositions, IM_ARRAYSIZE(HealthPositions));
						ImGui::PopItemWidth();
					}
					columns(1);

					columns(2);
					{
						checkbox("Armor", &settings::esp::armour);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("##armor.position", &settings::esp::armour_position, positions, IM_ARRAYSIZE(positions));
						ImGui::PopItemWidth();
					}
					columns(1);

					checkbox("Bone ESP", &settings::esp::bone_esp);
					checkbox("Is Scoped", &settings::esp::is_scoped);
					checkbox("Is Flashed", &settings::esp::is_flashed);
					checkbox("Is Defusing", &settings::esp::is_defusing);
					checkbox("Is Desyncing", &settings::esp::is_desyncing);
					checkbox("Has Kit", &settings::esp::haskit);
					checkbox("Ammo ESP", &settings::esp::ammo);
					checkbox("Money ESP", &settings::esp::money);
					checkbox("Sound ESP", &settings::esp::soundesp);

					//checkbox("Beams", &settings::esp::beams);
					//checkbox("Sound Direction (?)", &settings::esp::sound);
				
					checkbox("Bomb Damage ESP", &settings::esp::bomb_esp);
					checkbox("Offscreen ESP", &settings::esp::offscreen);
				});

			ImGui::NextColumn();

			child("Chams", []()
				{
					static const char* glow_modes[] = {
						"Exterior",
						"Interior",
						"Outline"
					};

					static const char* chams_list[] = {
						"Enemies",
						"Teammates",
						"Localplayer",
						"Misc",
						"Styles"
					};

					static const char* glow_list[] = {
						"Enemies",
						"Teammates",
						"Misc"
					};

					ImGui::Combo("List##chams", &settings::chams::chams_list_mode, chams_list, IM_ARRAYSIZE(chams_list));

					bool b_wip = false;

					switch (settings::chams::chams_list_mode)
					{
					case 0:
						columns(2);
						{
							checkbox("Enabled", &settings::chams::enemy::enabled);

							checkbox("Wireframe", &settings::chams::enemy::wireframe);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Visible only", &settings::chams::enemy::visible_only);
							ImGui::PopItemWidth();

							checkbox("Flat", &settings::chams::enemy::flat);
						}
						columns(1);
						break;
					case 1:
						columns(2);
						{
							checkbox("Enabled", &settings::chams::teammates::enabled);

							checkbox("Wireframe", &settings::chams::localplayer::wireframe);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Visible only", &settings::chams::teammates::visible_only);
							ImGui::PopItemWidth();

							checkbox("Flat", &settings::chams::teammates::flat);
						}
						columns(1);
						break;
					case 2:
						columns(2);
						{
							checkbox("Enabled", &settings::chams::localplayer::enabled);

							checkbox("Wireframe", &settings::chams::localplayer::wireframe);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Desync chams", &settings::chams::localplayer::desync_chams);
							ImGui::PopItemWidth();

							checkbox("Flat", &settings::chams::localplayer::flat);
						}
						columns(1);
						break;
					case 3:
						columns(2);
						{
							checkbox("Guns", &settings::chams::misc::weapon_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Dropped Guns", &settings::chams::misc::dropped_weapons);
							ImGui::PopItemWidth();
						}
						columns(1);

						columns(2);
						{
							checkbox("Planted C4", &settings::chams::misc::planted_bomb_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Dropped C4", &settings::chams::misc::dropped_bomb_chams);
							ImGui::PopItemWidth();
						}
						columns(1);

						columns(2);
						{
							checkbox("Nades", &settings::chams::misc::nade_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Accesories", &settings::chams::misc::accesories_chams);
							ImGui::PopItemWidth();
						}
						columns(1);

						columns(2);
						{
							checkbox("Arms", &settings::chams::misc::arms_chams);

							ImGui::NextColumn();

							ImGui::PushItemWidth(-1);
							checkbox("Dropped Kits", &settings::chams::misc::dropped_defusekit_chams);
							ImGui::PopItemWidth();
						}
						columns(1);
						break;
					case 4:
						checkbox("TODO: Work In Progress!", &b_wip);
						checkbox("In this section, nothing here works", &b_wip);
						checkbox("not a bug btw.", &b_wip);
						checkbox("Test: Arms - style", &b_wip);
						checkbox("Test: Arms - Wireframe, etc...", &b_wip);
						break;
					}

					child("Glow", []()
						{
							ImGui::Combo("List##glow", &settings::glow::glow_list_mode, glow_list, IM_ARRAYSIZE(glow_list));

							switch (settings::glow::glow_list_mode)
							{
							case 0:
								columns(2);
								{
									checkbox("Enabled", &settings::glow::glowEnemyEnabled);

									ImGui::NextColumn();

									ImGui::PushItemWidth(-1);
									ImGui::Combo("Mode", &settings::glow::style_enemy, glow_modes, IM_ARRAYSIZE(glow_modes));
									ImGui::PopItemWidth();
								}
								columns(1);
								break;
							case 1:
								columns(2);
								{
									checkbox("Enabled", &settings::glow::glowTeamEnabled);

									ImGui::NextColumn();

									ImGui::PushItemWidth(-1);
									ImGui::Combo("Mode", &settings::glow::style_teammate, glow_modes, IM_ARRAYSIZE(glow_modes));
									ImGui::PopItemWidth();
								}
								columns(1);
								break;
							case 2:
								columns(2);
								{
									checkbox("Planted C4", &settings::glow::glowC4PlantedEnabled);

									ImGui::NextColumn();

									ImGui::PushItemWidth(-1);
									checkbox("Nades", &settings::glow::glowNadesEnabled);
									ImGui::PopItemWidth();
								}
								columns(1);

								columns(2);
								{
									checkbox("Dropped Guns", &settings::glow::glowDroppedWeaponsEnabled);

									ImGui::NextColumn();

									ImGui::PushItemWidth(-1);
									checkbox("Dropped Kits", &settings::glow::glowDroppedKitsEnabled);
									ImGui::PopItemWidth();
								}
								columns(1);
								break;
							}
						});
				});

			ImGui::NextColumn();

			child("Extra", []()
				{
					static const char* cross_types[] = {
						"Crosshair",
						"Circle"
					};

					static const char* hitmarkersounds[] = {
						"Cod",
						"Skeet",
						"Punch",
						"Metal",
						"Boom"
					};

					checkbox("Buy Log", &settings::esp::buylog);
					checkbox("Planted C4", &settings::visuals::planted_c4);
					checkbox("World Weapons", &settings::visuals::dropped_weapons);
					checkbox("World Grenades", &settings::visuals::world_grenades);
					checkbox("Sniper Crosshair", &settings::visuals::sniper_crosshair);
					checkbox("Snap Lines", &settings::esp::snaplines);
					checkbox("Armor Status (?)", &settings::esp::kevlarinfo);
					tooltip("Will display HK if enemy has kevlar + helmer or K if enemy has kevlar only.");
					checkbox("Grenade Prediction", &settings::visuals::grenade_prediction);
					checkbox("Damage Indicator", &settings::misc::damage_indicator);
					checkbox("Aimbot Fov", &settings::esp::drawFov);
					checkbox("Spread Crosshair", &settings::visuals::spread_cross);
					checkbox("Bullet Tracer", &settings::visuals::bullet_tracer);


					columns(2);
					{
						checkbox("Hitmarker", &settings::visuals::hitmarker);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("Hitmarker Sound", &settings::visuals::hitsound, hitmarkersounds, IM_ARRAYSIZE(hitmarkersounds));
						ImGui::PopItemWidth();
					}
					columns(1);

					columns(2);
					{
						checkbox("RCS Crosshair", &settings::visuals::rcs_cross);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("RCS Crosshair Type", &settings::visuals::rcs_cross_mode, cross_types, IM_ARRAYSIZE(cross_types));
						ImGui::PopItemWidth();
					}
					columns(1);

					if (settings::visuals::rcs_cross_mode == 1)
						ImGui::SliderFloatLeftAligned("Radius", &settings::visuals::radius, 8.f, 18.f, "%.1f");

					const auto old_night_state = settings::visuals::night_mode;
					const auto old_style_state = settings::visuals::newstyle;
					checkbox("Night Mode", &settings::visuals::night_mode);

					if (settings::visuals::night_mode)
					{
						ImGui::SliderFloatLeftAligned("Night Mode Intensity:", &settings::esp::mfts, 0.0f, 1.0f, "%.1f %");

						if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
						{
							color_modulation::set_material_tone();
						}
					}

					checkbox("Dark Menu", &settings::visuals::newstyle);
					if (old_style_state != settings::visuals::newstyle)
						imdraw::apply_style(settings::visuals::newstyle);
				});
		}
	}
}