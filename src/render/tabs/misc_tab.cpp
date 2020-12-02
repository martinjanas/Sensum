#include "../render.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../features/features.h"
#include "../../helpers/console.h"
#include <string.h>

extern void bind_button(const char* label, int& key);
extern bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		char name[64];
		char localtag[16];
		bool is_clantag_copied = false;

		void misc_tab()
		{
			child("Clan Tag", []()
				{
					if (!is_clantag_copied && std::string(localtag) != globals::clantag::value)
					{
						is_clantag_copied = true;
						strcpy(localtag, globals::clantag::value.c_str());
					}

					ImGui::InputText("##misc_clantag", localtag, sizeof(localtag));

					auto player_resource = *g::player_resource;
					if (player_resource)
					{
						if (ImGui::BeginCombo("##clantags.list", "Player tags"))
						{
							std::vector<std::string> tags;

							for (int i = 1; i < g::engine_client->GetMaxClients(); ++i)
							{
								auto* player = c_base_player::GetPlayerByIndex(i);
								if (!player)
									continue;

								const auto info = player->GetPlayerInfo();
								if (info.fakeplayer)
									continue;

								auto usertag = std::string(player_resource->GetClanTag()[player->GetIndex()]);
								if (usertag.empty() || std::find(tags.begin(), tags.end(), usertag) != tags.end())
									continue;

								tags.push_back(usertag);

								if (ImGui::Selectable(usertag.c_str()))
								{
									strcpy(localtag, usertag.c_str());
									globals::clantag::value = usertag;
									clantag::set(localtag);
								}
							}

							ImGui::EndCombo();
						}
					}

					columns(2);
					{
						if (ImGui::Button("Apply##clan", ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						{
							globals::clantag::value = localtag;
							clantag::set(localtag);
						}

						ImGui::NextColumn();

						if (ImGui::Button(!globals::clantag::animation ? "Animate" : "Dont Animate", ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
							globals::clantag::animation = !globals::clantag::animation;
					}
					columns(1);

					static const char* skyList[] = {
						"Off", "Baggage", "Tibet", "Embassy", "Italy",
						"Daylight 1", "Daylight 2", "Daylight 3", "Daylight 4",
						"Cloudy", "Night 1", "Night 2", "Night Flat", "Day HD",
						"Day", "Rural", "Vertigo HD", "Vertigo Blue HD", "Vertigo", 
						"Vietnam", "Dusty Sky", "Jungle", "Nuke", "Office"
					};

					separator("Desync");
					checkbox("Enabled", &settings::desync::enabled);

					separator("FOV");

					ImGui::SliderFloatLeftAligned("View Model:", &settings::misc::viewmodel_fov, 54, 120, "%.0f *");
					ImGui::SliderIntLeftAligned("Debug:", &settings::misc::debug_fov, 80, 120, "%.0f *");

					separator("Skybox Changer");

					columns(2);
					{
						ImGui::Combo("", &settings::visuals::skychanger_mode, skyList, IM_ARRAYSIZE(skyList));
					}
					columns(1);

					static const char* viewList[] = {
						"X",
						"Y",
						"Z"
					};

					separator("Viewmodel Override");

					columns(2);
					{
						checkbox("Enabled##viewmodel_changer", &settings::misc::override_viewmodel);

						ImGui::NextColumn();
						ImGui::Combo("List##viewmodel_changer", &settings::visuals::viewmodel_mode, viewList, IM_ARRAYSIZE(viewList));
					}
					columns(1);

					switch (settings::visuals::viewmodel_mode)
					{
					case 0:
						ImGui::SliderFloatLeftAligned("Offset X:", &settings::misc::viewmodel_offset_x, -21.f, 21.f, "%.0f");
						break;
					case 1:
						ImGui::SliderFloatLeftAligned("Offset Y:", &settings::misc::viewmodel_offset_y, 0.f, 50.f, "%.0f");
						break;
					case 2:
						ImGui::SliderFloatLeftAligned("Offset Z:", &settings::misc::viewmodel_offset_z, -30.f, 30.f, "%.0f");
						break;
					}

					/*if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
					  {
							color_modulation::SkyChanger();
					  } */
				});

			ImGui::NextColumn();

			child("Extra", []()
				{
					const char* fastStopModes[] = {
						"Off",
						"Left & Right",
						"Forward & Backward",
						"Both"
					};

					checkbox("Engine Prediction", &settings::movement::engine_prediction);
					checkbox("Radar", &settings::misc::radar);
					checkbox("No Flash", &settings::misc::no_flash);
					checkbox("No Smoke", &settings::misc::no_smoke);
					checkbox("Bunny Hop", &settings::misc::bhop);
					checkbox("Auto Strafe", &settings::misc::auto_strafe);
					checkbox("Post Processing", &globals::post_processing);
					checkbox("Noscope Overlay", &settings::misc::noscope);
					checkbox("No 3rd Person on Equiped Weapons.", &settings::misc::disable_on_weapon);
					checkbox("Anti OBS", &settings::misc::anti_obs);
					checkbox("Left Hand Knife", &settings::misc::lefthandknife);
					checkbox("Ow revealer", &settings::misc::ow_reveal);
					checkbox("Smoke Helper (?)", &settings::misc::smoke_helper);
					tooltip("Jump = jump throw, LClick/RClick = left/right mouse buttons.");
					checkbox("Flash Helper - WIP", &settings::misc::flash_helper);
					columns(2);
					{
						checkbox("Fast Stop", &settings::misc::fast_stop);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::Combo("Mode", &settings::misc::fast_stop_mode, fastStopModes, IM_ARRAYSIZE(fastStopModes));
						ImGui::PopItemWidth();
					}
					columns(1);

					checkbox("Force Inventory Open", &settings::misc::force_inventory_open);

					columns(2);
					{
						checkbox("Edge Jump", &settings::misc::edge_jump);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						if (settings::misc::edge_jump) {
							ImGui::Checkbox("Duck in Air", &settings::misc::edge_jump_duck_in_air);
						}
						ImGui::PopItemWidth();
					}
					columns(1);
				});

			ImGui::NextColumn();

			child("Binds", []()
				{
					bind_button("Zeus Bot", globals::binds::zeus_bot);
					bind_button("Slow Walk", globals::binds::slow_walk);
					bind_button("Third Person", globals::binds::thirdperson::key);
					bind_button("Edge Jump", globals::binds::edge_jump);
				});
		}
	}
}