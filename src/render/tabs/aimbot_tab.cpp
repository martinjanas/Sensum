#include "../render.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../helpers/notifies.h"

extern void bind_button(const char* eng, int& key);
extern bool hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		extern std::map<int, weapon_type_t> get_weapons(bool need_knife);
		extern std::map<int, const char*> get_groups(bool need_knife = false, bool need_groups = false);
		extern bool selectable_weapons(
			int& selected_item,
			bool only_groups,
			std::string& weaponName,
			std::map<int, const char*> groups,
			std::map<int, weapon_type_t> k_item_names,
			std::vector<int> selected_weapons = { }
		);

		extern bool listbox_group_weapons(
			int& selected_item,
			std::map<int, const char*> groups,
			std::map<int, weapon_type_t> items,
			ImVec2 listbox_size,
			bool show_only_selected = false,
			std::vector<int> selected_weapons = { }
		);

		void aimbot_tab()
		{
			static int definition_index = 7;
			auto settings = &settings::aimbot::m_items[definition_index];

			child(strings::aimbot_main.c_str(), [&settings]()
			{
				auto k_item_names = get_weapons(false);

				const char* setting_types[] =
				{
					strings::aimbot_main_setting_types_seperately.c_str(),
					strings::aimbot_main_setting_types_subgroups.c_str(),
					strings::aimbot_main_setting_types_for_all.c_str(),
					strings::aimbot_main_setting_types_groups.c_str()
				};

				static bool is_settings_visible = false;

				if (ImGui::Combo("##setting_type", &settings::aimbot::setting_type, setting_types, IM_ARRAYSIZE(setting_types)))
					is_settings_visible = settings::aimbot::setting_type == settings_type_t::groups ? false : true;

				if (settings::aimbot::setting_type != settings_type_t::groups)
					is_settings_visible = true;

				if (settings::aimbot::setting_type == settings_type_t::for_all)
				{
					definition_index = 0;
				}
				else if (settings::aimbot::setting_type == settings_type_t::separately || settings::aimbot::setting_type == settings_type_t::subgroups)
				{
					auto groups = get_groups(false, true);

					std::string weaponName;
					if (settings::aimbot::setting_type == settings_type_t::subgroups)
					{
						if (groups.count(definition_index) == 0)
							definition_index = WEAPONTYPE_PISTOL;

						weaponName = groups[definition_index];
					}
					else
					{
						if (k_item_names.count(definition_index) == 0)
							definition_index = WEAPON_AK47;

						weaponName = k_item_names[definition_index].name;
					}

					selectable_weapons(definition_index, settings::aimbot::setting_type == settings_type_t::subgroups, weaponName, groups, k_item_names);
				}
				else if (settings::aimbot::setting_type == settings_type_t::groups)
				{
					if (definition_index < 0 || definition_index >= settings::aimbot::m_groups.size())
					{
						definition_index = 0;
					}

					if (settings::aimbot::m_groups.size() > 0)
					{
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 70.f);
						ImGui::Combo("##aimbot.groups", &definition_index, [](void* data, int idx, const char** out_text)
						{
							*out_text = settings::aimbot::m_groups[idx].name.c_str();
							return true;
						}, nullptr, settings::aimbot::m_groups.size(), 10);
						ImGui::PopItemWidth();

						ImGui::SameLine();

						if (is_settings_visible)
						{
							if (ImGui::Button(strings::edit.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
								is_settings_visible = false;
						}
						else
						{
							if (ImGui::Button(strings::hide.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0.f)))
								is_settings_visible = true;
						}
					}

					if (!is_settings_visible)
					{
						separator(strings::aimbot_main_newgroup.c_str());

						ImGui::Text(strings::name.c_str());

						static char group_name[32];
						ImGui::InputText("##aimbot.group_name", group_name, sizeof(group_name));

						if (ImGui::Button(strings::create.c_str()))
						{
							if (strlen(group_name) == 0)
								notifies::push(strings::aimbot_main_entername.c_str(), notify_state_s::danger_state);
							else
							{
								settings::aimbot::m_groups.emplace_back(aimbot_group{ std::string(group_name), { } });

								memset(group_name, 0, sizeof(group_name));
								notifies::push(strings::aimbot_main_groupcreated.c_str());

								definition_index = settings::aimbot::m_groups.size() - 1;
							}
						}

						if (settings::aimbot::m_groups.empty())
							return;

						separator(strings::aimbot_main_currgroup.c_str());

						auto& current_group = settings::aimbot::m_groups[definition_index];

						static auto weapon_to_select = -1;
						std::string placeholder = strings::aimbot_main_selweap.c_str();

						const auto groups = get_groups(false, false);

						if (selectable_weapons(weapon_to_select, false, placeholder, groups, k_item_names, current_group.weapons))
						{
							if (std::find(current_group.weapons.begin(), current_group.weapons.end(), weapon_to_select) == current_group.weapons.end())
								current_group.weapons.emplace_back(weapon_to_select);

							weapon_to_select = -1;
						}

						//ImGui::Text(___("Press for remove weapon", u8"Нажмите для удаления"));

						static int weapon_to_remove = -1;
						//ImVec2(ImGui::GetContentRegionAvailWidth(), 150.f)
						if (listbox_group_weapons(weapon_to_remove, groups, k_item_names, ImVec2(0, 150.f), true, current_group.weapons))
						{
							current_group.weapons.erase(std::find(current_group.weapons.begin(), current_group.weapons.end(), weapon_to_remove));

							weapon_to_remove = -1;
						}

						if (ImGui::Button(strings::delete_st.c_str()))
						{
							notifies::push(strings::aimbot_main_delgroup.c_str());

							settings::aimbot::m_groups.erase(settings::aimbot::m_groups.begin() + definition_index);

							definition_index = 0;
						}

						return;
					}
				}

				checkbox(strings::enabled.c_str(), &settings->enabled);
				checkbox(strings::aimbot_main_randomise_rcs.c_str(), &settings->recoil.humanize);
				tooltip(strings::aimbot_main_randomise_rcs_tooltip.c_str());

				if (settings::aimbot::setting_type == settings_type_t::separately)
				{
					switch (definition_index)
					{
					case WEAPON_P250:
					case WEAPON_USP_SILENCER:
					case WEAPON_GLOCK:
					case WEAPON_FIVESEVEN:
					case WEAPON_TEC9:
					case WEAPON_DEAGLE:
					case WEAPON_ELITE:
					case WEAPON_HKP2000:
					case 201:
						checkbox(strings::aimbot_main_autopistol.c_str(), &settings->autopistol);
					default:
						break;
					}
				}
				else if (settings::aimbot::setting_type == settings_type_t::subgroups && (definition_index == WEAPONTYPE_PISTOL || definition_index == 201))
					checkbox(strings::aimbot_main_autopistol.c_str(), &settings->autopistol);
				else
					checkbox(strings::aimbot_main_autopistol.c_str(), &settings->autopistol);

				checkbox(strings::aimbot_main_aircheck.c_str(), &settings->check_air);
				checkbox(strings::aimbot_main_flashcheck.c_str(), &settings->check_flash);
				checkbox(strings::aimbot_main_smokecheck.c_str(), &settings->check_smoke);

				if (settings::aimbot::setting_type == settings_type_t::separately)
				{
					if (utils::is_sniper(definition_index))
						checkbox(strings::aimbot_main_zoomcheck.c_str(), &settings->check_zoom);
				}
				else if (settings::aimbot::setting_type == settings_type_t::subgroups)
				{
					if (definition_index == 240 || definition_index == 209 || definition_index == WEAPONTYPE_SNIPER_RIFLE)
						checkbox(strings::aimbot_main_zoomcheck.c_str(), &settings->check_zoom);
				}
				else if (settings::aimbot::setting_type == settings_type_t::groups)
					checkbox(strings::aimbot_main_zoomcheck.c_str(), &settings->check_zoom);

				columns(2);
				{
					checkbox(strings::aimbot_main_bydamage.c_str(), &settings->by_damage);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::SliderIntLeftAligned((strings::min + "##min_damage").c_str(), &settings->min_damage, 1, 100, "%.0f HP");
					ImGui::PopItemWidth();
				}
				columns(1);

				columns(2);
				{
					checkbox(strings::aimbot_main_autowall.c_str(), &settings->autowall.enabled);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					ImGui::SliderIntLeftAligned((strings::min + "##min_damage").c_str(), &settings->autowall.min_damage, 1, 100, "%.0f HP");
					ImGui::PopItemWidth();
				}
				columns(1);

				separator(strings::aimbot_main_delays.c_str());

				if (!settings->silent.enabled)
				{
					checkbox(strings::aimbot_main_autodelay.c_str(), &settings->autodelay);
					ImGui::SliderIntLeftAligned(strings::aimbot_main_shotdelay.c_str(), &settings->shot_delay, 0, 250, "%.0f ms");
				}
				ImGui::SliderIntLeftAligned(strings::aimbot_main_targetswitchdelay.c_str(), &settings->kill_delay, 0, 1000, "%.0f ms");

				if (settings::aimbot::setting_type != 0)
					return;

				if (!utils::is_connected())
					return;

				auto weapon = interfaces::local_player->m_hActiveWeapon();
				if (!weapon || !weapon->IsWeapon())
					return;

				const auto item_definition_index = weapon->m_iItemDefinitionIndex();
				if (k_item_names.count(item_definition_index) == 0)
					return;

				ImGui::Separator();

				if (ImGui::Button(strings::aimbot_main_current.c_str()))
					definition_index = item_definition_index;

				if (weapon)
					definition_index = item_definition_index;
			});

			ImGui::NextColumn();

			child(strings::aimbot_silent.c_str(), [&settings]()
			{
				checkbox((strings::enabled + "##silent").c_str(), &settings->silent.enabled);
				checkbox(strings::aimbot_silent_always.c_str(), &settings->silent.always);
				tooltip(strings::aimbot_silent_always_tt.c_str());
				checkbox(strings::aimbot_silent_withsmooth.c_str(), &settings->silent.with_smooth);

				if (settings->silent.with_smooth && settings->fov < settings->silent.fov)
					settings->silent.fov = settings->fov;

				ImGui::SliderFloatLeftAligned(strings::aimbot_fov.c_str(), &settings->silent.fov, 0, settings->silent.with_smooth ? settings->fov : 20.f, "%.1f %"); //20f
				//ImGui::SliderInt("##silent.chanse", &settings->silent.chanse, 1, 10, ___("Chanse: %.0f", u8"Шанс срабатывания: %.0f"));

				separator(strings::aimbot_misc.c_str());

				ImGui::PushID("aimbot.other");
				{
					checkbox(strings::aimbot_misc_dynfov.c_str(), &settings->dynamic_fov);
					checkbox(strings::aimbot_misc_legitbacktrack.c_str(), &settings->backtrack.legit);
					tooltip(strings::aimbot_misc_legitbacktrack_tt.c_str());

					ImGui::SliderFloatLeftAligned(strings::aimbot_fov.c_str(), &settings->fov, 0, 15.f, "%.1f %");

					ImGui::SliderFloatLeftAligned(strings::aimbot_misc_smooth.c_str(), &settings->smooth, 1, 15.f, "%.1f %");

					//ImGui::SliderFloatLeftAligned(___("BT time:", u8"Лаг компенсация:"), &settings->backtrack.time, 0.f, 0.2f, ___("%.2f ms", u8"%.0f тиков"));
					ImGui::SliderIntLeftAligned(strings::aimbot_misc_lagcomp.c_str(), &settings->backtrack.ticks, 0, 12, "%.0f ms");

					ImGui::SliderIntLeftAligned(strings::aimbot_misc_hitchance.c_str(), &settings->min_hitchanse, 0, 100, "%.0f%%");
				}
				ImGui::PopID();

				columns(3);
				{
					checkbox(strings::aimbot_misc_head.c_str(), &settings->hitboxes.head);
					checkbox(strings::aimbot_misc_hands.c_str(), &settings->hitboxes.hands);

					ImGui::NextColumn();

					checkbox(strings::aimbot_misc_neck.c_str(), &settings->hitboxes.neck);
					checkbox(strings::aimbot_misc_legs.c_str(), &settings->hitboxes.legs);

					ImGui::NextColumn();

					checkbox(strings::aimbot_misc_body.c_str(), &settings->hitboxes.body);
				}
				columns(1);
			});

			ImGui::NextColumn();

			child(strings::aimbot_rcs.c_str(), [&settings]()
			{
				checkbox(strings::enabled.c_str(), &settings->recoil.enabled);
				checkbox(strings::aimbot_rcs_firstbullet.c_str(), &settings->recoil.first_bullet);
				checkbox(strings::aimbot_rcs_hitboxoverride.c_str(), &settings->rcs_override_hitbox);
				tooltip(strings::aimbot_rcs_hitboxoverride_tt.c_str());

				ImGui::SliderFloatLeftAligned(strings::aimbot_rcs_pitch.c_str(), &settings->recoil.pitch, 0, 2, "%.1f %");
				ImGui::SliderFloatLeftAligned(strings::aimbot_rcs_yaw.c_str(), &settings->recoil.yaw, 0, 2, "%.1f %");

				separator(strings::aimbot_trigger.c_str());

				columns(2);
				{
					checkbox((strings::enabled + "##trigger").c_str(), &settings->trigger.enabled);

					ImGui::NextColumn();

					ImGui::PushItemWidth(-1);
					hotkey("##binds.trigger", &globals::binds::trigger);
					ImGui::PopItemWidth();
				}
				columns(1);

				bind_button(strings::aimbot_trigger_backshot.c_str(), globals::binds::back_shot);

				ImGui::SliderIntLeftAligned(strings::aimbot_trigger_reactiontime.c_str(), &settings->trigger.delay, 0, 250, "%.0f ms");
				ImGui::SliderIntLeftAligned(strings::aimbot_trigger_dbs.c_str(), &settings->trigger.delay_btw_shots, 0, 250, "%.0f ms");
				ImGui::SliderIntLeftAligned(strings::aimbot_misc_hitchance.c_str(), &settings->trigger.hitchance, 1, 100, "%.0f%%");
			});
		}
	}
}
