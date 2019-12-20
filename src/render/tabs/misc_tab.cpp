#include "../render.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../features/features.h"
#include "../../helpers/console.h"

extern void bind_button(const char* eng, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		char name[64];
		char localtag[16];
		bool is_clantag_copied = false;

		void misc_tab()
		{
			child(strings::misc_name.c_str(), []()
			{
				ImGui::InputText("##misc_name", name, sizeof(name));
				columns(2);
				{
					if (ImGui::Button((strings::misc_apply + "##name").c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						utils::set_player_name(name);

					ImGui::NextColumn();

					if (ImGui::Button(strings::misc_hide.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						utils::set_player_name("\n\xAD\xAD\xAD");
				}
				columns(1);

				separator(strings::misc_clantag.c_str());

				if (!is_clantag_copied && std::string(localtag) != globals::clantag::value)
				{
					is_clantag_copied = true;
					strcpy(localtag, globals::clantag::value.c_str());
				}

				ImGui::InputText("##misc_clantag", localtag, sizeof(localtag));
				//if (globals::clantag::animation)
					//ImGui::SliderFloatLeftAligned(___("Delay:", u8"Задержка:"), &globals::clantag::delay, 0, 1);

				auto player_resource = *interfaces::player_resource;
				if (player_resource)
				{
					if (ImGui::BeginCombo("##clantags.list", strings::misc_playertags.c_str()))
					{
						std::vector<std::string> tags;

						for (int i = 1; i < interfaces::engine_client->GetMaxClients(); ++i)
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
					if (ImGui::Button((strings::misc_apply + "##clan").c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
					{
						globals::clantag::value = localtag;
						clantag::set(localtag);

						globals::save();
					}

					ImGui::NextColumn();

					if (ImGui::Button(!globals::clantag::animation ? strings::misc_anim.c_str() : strings::misc_dontanim.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 2.f, 0.f)))
						globals::clantag::animation = !globals::clantag::animation;
				}
				columns(1);

				separator(strings::misc_fakelags.c_str());

				ImGui::PushID("fakelags");
				{
					columns(2);
					{
						checkbox(strings::enabled.c_str(), &settings::fake_lags::enabled);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						hotkey("##binds.fake_lags", &globals::binds::fake_lag);
						ImGui::PopItemWidth();
					}
					columns(1);

					const char* lag_types[] =
					{
						strings::misc_fakelags_always.c_str(),
						strings::misc_fakelags_inair.c_str(),
						strings::misc_fakelags_picking.c_str(),
						strings::misc_fakelags_button.c_str(),
						strings::misc_fakelags_adaptive.c_str()
					};

					ImGui::Combo("##lag_type", &settings::fake_lags::type, lag_types, IM_ARRAYSIZE(lag_types));
					ImGui::SliderIntLeftAligned(strings::misc_fakelags_factor.c_str(), &settings::fake_lags::factor, 1, 6, settings::fake_lags::factor == 1 ? "%.0f tick" : "%.0f ticks");
				}
				ImGui::PopID();

				const char* desyncModes[] = {
					strings::misc_desync_off.c_str(),
					strings::misc_desync_static.c_str(),
					strings::misc_desync_balance.c_str()
				};

				separator(strings::misc_desync.c_str());
				ImGui::PushID("desync");
				{
					columns(2);
					{
						checkbox(strings::enabled.c_str(), &settings::desync::enabled2);

						ImGui::NextColumn();
						ImGui::Combo(strings::misc_desync_mode.c_str(), &settings::desync::desync_mode, desyncModes, IM_ARRAYSIZE(desyncModes));
					}
					columns(1);

					bind_button(strings::misc_leftright.c_str(), globals::binds::desync);
				}
				ImGui::PopID();

				separator(strings::misc_fov.c_str());

				ImGui::SliderFloatLeftAligned(strings::misc_viewmodel.c_str(), &settings::misc::viewmodel_fov, 54, 120, "%.0f *");
				ImGui::SliderIntLeftAligned(strings::misc_debug.c_str(), &settings::misc::debug_fov, 80, 120, "%.0f *");

				static const char* skyList[] = { "Baggage", "Tibet", "Embassy", "Italy", "Daylight 1", "Daylight 2", "Daylight 3", "Daylight 4", "Cloudy", "Night 1", "Night 2", "Night Flat", "Day HD", "Day", "Rural", "Vertigo HD", "Vertigo", "Dusty Sky", "Jungle", "Nuke", "Office" };

				separator(strings::misc_skybox.c_str());

				columns(2);
				{
					checkbox(strings::enabled.c_str(), &settings::visuals::skychanger);

					ImGui::NextColumn();
					ImGui::Combo(strings::misc_list.c_str(), &settings::visuals::skychanger_mode, skyList, IM_ARRAYSIZE(skyList));
				}
				columns(1);

				if (settings::visuals::skychanger)
				{
					color_modulation::SkyChanger();
				}

				static const char* viewList[] = {
					"X",
					"Y",
					"Z"
				};

				separator(strings::misc_viewmodel_ov.c_str());

				columns(2);
				{
					checkbox((strings::enabled + "##viewmodel_changer").c_str(), &settings::misc::override_viewmodel);

					ImGui::NextColumn();
					ImGui::Combo((strings::misc_list + "##viewmodel_changer").c_str(), &settings::visuals::viewmodel_mode, viewList, IM_ARRAYSIZE(viewList));
				}
				columns(1);

				switch (settings::visuals::viewmodel_mode)
				{
				case 0:
					ImGui::SliderFloatLeftAligned(strings::misc_viewmodel_x.c_str(), &settings::misc::viewmodel_offset_x, -21.f, 21.f, "%.0f");
					break;
				case 1:
					ImGui::SliderFloatLeftAligned(strings::misc_viewmodel_y.c_str(), &settings::misc::viewmodel_offset_y, 0.f, 50.f, "%.0f");
					break;
				case 2:
					ImGui::SliderFloatLeftAligned(strings::misc_viewmodel_z.c_str(), &settings::misc::viewmodel_offset_z, -30.f, 30.f, "%.0f");
					break;
				}

				/*if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
				  {
						color_modulation::SkyChanger();
				  } */
			});

			ImGui::NextColumn();

			child(strings::misc_extra.c_str(), []()
			{
				//checkbox("Disable Animations (?)", u8"Отключить анимации (?)", &globals::no_animations);
				//tooltip("Disables the cheat menu animations.", u8"Отключает анимации меню чита.");

				const char* fastStopModes[] = {
					strings::misc_extra_faststop_off.c_str(),
					strings::misc_extra_faststop_leftright.c_str(),
					strings::misc_extra_faststop_forwardback.c_str(),
					strings::misc_extra_faststop_both.c_str()
				};

				checkbox(strings::misc_extra_engpred.c_str(), &settings::movement::engine_prediction);
				checkbox(strings::misc_extra_radar.c_str(), &settings::misc::radar);
				checkbox(strings::misc_extra_noflash.c_str(), &settings::misc::no_flash);
				checkbox(strings::misc_extra_nosmoke.c_str(), &settings::misc::no_smoke);
				checkbox(strings::misc_extra_bhop.c_str(), &settings::misc::bhop);
				checkbox(strings::misc_extra_autostrafe.c_str(), &settings::misc::auto_strafe);
				checkbox(strings::misc_extra_knifebot.c_str(), &settings::misc::knife_bot);
				checkbox(strings::misc_extra_moonwalk.c_str(), &settings::misc::moon_walk);
				checkbox(strings::misc_extra_dm.c_str(), &settings::misc::deathmatch);
				checkbox(strings::misc_extra_postpr.c_str(), &globals::post_processing);
				checkbox(strings::misc_extra_resolver.c_str(), &settings::desync::resolver);
				checkbox(strings::misc_extra_hbhop.c_str(), &settings::misc::human_bhop);
				checkbox(strings::misc_extra_noscope.c_str(), &settings::misc::noscope);
				checkbox(strings::misc_extra_no3person.c_str(), &settings::misc::disable_on_weapon);
				checkbox(strings::misc_extra_antiobs.c_str(), &settings::misc::anti_obs);
				checkbox(strings::misc_extra_leftknife.c_str(), &settings::misc::lefthandknife);
				checkbox(strings::misc_extra_98nade.c_str(), &settings::misc::selfnade);
				tooltip(strings::misc_extra_98nade_tt.c_str());
				columns(2);
				{
					checkbox(strings::misc_extra_faststop.c_str(), &settings::misc::fast_stop);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::Combo(strings::misc_desync_mode.c_str(), &settings::misc::fast_stop_mode, fastStopModes, IM_ARRAYSIZE(fastStopModes));
					ImGui::PopItemWidth();
				}
				columns(1);

				checkbox(strings::misc_extra_forceinv.c_str(), &settings::misc::force_inventory_open);

				columns(2);
				{
					checkbox(strings::misc_edgejump.c_str(),&settings::misc::edge_jump);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					if (settings::misc::edge_jump) {
						ImGui::Checkbox(strings::misc_extra_edgejump_duck.c_str(), &settings::misc::edge_jump_duck_in_air);
					}
					ImGui::PopItemWidth();
				}
				columns(1);
				//checkbox("Block Bot", &settings::misc::block_bot); //WIP blockbot, not fully working
			});

			ImGui::NextColumn();

			child(strings::misc_binds.c_str(), []()
			{
				bind_button(strings::misc_zeusbot.c_str(), globals::binds::zeus_bot);
				bind_button(strings::misc_slowwalk.c_str(), globals::binds::slow_walk);
				bind_button(strings::misc_fakecrouch.c_str(), globals::binds::fake_duck);
				bind_button(strings::misc_thirdperson.c_str(), globals::binds::thirdperson::key);
				bind_button(strings::misc_lightningshot.c_str(), globals::binds::lightning_shot);
				bind_button(strings::misc_edgejump.c_str(), globals::binds::edge_jump);

				separator(strings::misc_unfinished.c_str());

				bind_button(strings::misc_blockbot.c_str(), globals::binds::block_bot);

				separator(strings::language.c_str());

				static const char* languages[] = {
					"English",
					"Russian"
				};
				ImGui::Combo("Language", &globals::language, languages, IM_ARRAYSIZE(languages));
				strings::updatelang();

				if (settings::misc::human_bhop)
				{
					separator(strings::misc_hbhop_settings.c_str());

					ImGui::SliderIntLeftAligned(strings::misc_hbhop_settings_hitch.c_str(), &settings::misc::bhop_hit_chance, 0, 100, "%.0f %%");
					tooltip(strings::misc_hbhop_settings_hitch_tt.c_str());
					ImGui::SliderIntLeftAligned(strings::misc_hbhop_settings_limit.c_str(), &settings::misc::hops_restricted_limit, 0, 12);
					tooltip(strings::misc_hbhop_settings_limit_tt.c_str());
					ImGui::SliderIntLeftAligned(strings::misc_hbhop_settings_maxlimit.c_str(), &settings::misc::max_hops_hit, 0, 12);
					tooltip(strings::misc_hbhop_settings_limit_tt.c_str());
				}

				/*if (!interfaces::local_player)
				{
					ImGui::Separator();

					char btn_label[256];
					sprintf_s(btn_label, "%s (%d)", ___("Invite nearby players", u8"Пригласить ближ. игроков"), lobby_inviter::max_count); //Not working anymore.

					if (ImGui::Button(btn_label, ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
						lobby_inviter::inviteAll();
				} */
			});
		}
	}
}
