#include "../render.h"
#include "../../globals.h"
#include "../../features/features.h"

namespace render
{
	namespace menu
	{
		extern std::map<int, weapon_type_t> get_weapons(bool need_knife);
		extern std::map<int, const char*> get_groups(bool need_knife = false, bool need_groups = false);

		extern bool listbox_group_weapons(
			int& selected_item,
			std::map<int, const char*> groups,
			std::map<int, weapon_type_t> items,
			ImVec2 listbox_size,
			bool show_only_selected = false,
			std::vector<int> selected_weapons = { }
		);

		void skins_tab()
		{
			static int definition_index = 1;
			auto& entries = skins::m_items;
			auto& selected_entry = entries[definition_index];
			selected_entry.definition_index = definition_index;

			static int index = 1;
			auto& entry2 = skins::statrack_items;
			auto& sel_entry2 = entry2[index];
			sel_entry2.definition_index = index;

			child(strings::skins_items.c_str(), [&selected_entry]()
			{
				auto weapon_index = 0;
				const auto weapons = get_weapons(true);
				auto can_change_index = [weapons, &weapon_index]()
				{
					if (!interfaces::local_player || !interfaces::local_player->IsAlive())
						return false;

					if (!interfaces::local_player->m_hActiveWeapon())
						return false;

					weapon_index = interfaces::local_player->m_hActiveWeapon()->m_iItemDefinitionIndex();
					return weapons.count(weapon_index) > 0;
				};

				const auto state = can_change_index();

				listbox_group_weapons(definition_index, get_groups(true, false), weapons, get_listbox_size(0.f, state ? 26.f : 0.f));

				if (!state)
					return;

				if (ImGui::Button(strings::skins_currweapon.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 22.f)))
					definition_index = weapon_index;
			});

			ImGui::NextColumn();

			child(strings::skins_paints.c_str(), [&selected_entry]()
			{
				static auto show_all_kits = false;
				//checkbox("All skins", u8"Полный список", &show_all_kits);

				static char query_kit[64];
				ImGui::InputText("##skins.query", query_kit, sizeof(query_kit));

				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.f, 0.f, 0.f, 0.1f));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.f, 0.f, 0.f, 0.1f));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.f, 0.f, 0.f, 0.1f));

				ImGui::ListBoxHeader("##items", get_listbox_size());
				{
					const std::string query(query_kit);
					const auto query_length = query.length();
					const auto has_query = query_length > 0;

					const auto is_glove = selected_entry.definition_index == GLOVE_CT_SIDE || selected_entry.definition_index == GLOVE_T_SIDE;
					for (size_t k = 0; k < skins::skin_kits.size(); k++) //for (size_t k = 0; k < skins::skin_kits.size(); k++)
					{
						weapon_kit_t weapon_kit;
						if (!show_all_kits)
						{
							for (auto& definition : skins::skin_kits[k].weapons)
							{
								if (definition.index == (is_glove ? selected_entry.definition_override_index : selected_entry.definition_index))
									weapon_kit = definition;
							}

							if (weapon_kit.rarity.empty())
								continue;
						}

						const auto name = skins::skin_kits[k].english;
						if (has_query)
						{
							if (skins::skin_kits[k].english.length() < query_length)
								continue;

							if (skins::skin_kits[k].english.find(query) == -1)
								continue;
						}

						ImVec4 color;
						/*if (weapon_kit.rarity == "common")
							color = ImVec4(0.69f, 0.76f, 0.85f, 1.f);
						else if (weapon_kit.rarity == "rare")
							color = ImVec4(0.29f, 0.41f, 1.f, 1.f);
						else if (weapon_kit.rarity == "uncommon")
							color = ImVec4(0.36f, 0.60f, 0.85f, 1.f);
						else if (weapon_kit.rarity == "mythical")
							color = ImVec4(0.53f, 0.27f, 1.f, 1.f);
						else if (weapon_kit.rarity == "legendary")
							color = ImVec4(0.82f, 0.17f, 0.90f, 1.f);
						else if (weapon_kit.rarity == "ancient")
							color = ImVec4(0.92f, 0.29f, 0.29f, 1.f);
						else if (weapon_kit.rarity == "immortal")
							color = ImVec4(0.89f, 0.68f, 0.22f, 1.f);
						else
							continue; */

						color = ImVec4(0.29f, 0.41f, 1.f, 1.f);
						ImGui::PushStyleColor(ImGuiCol_Text, color);

						char buf_name[256];
						sprintf_s(buf_name, sizeof(buf_name), "%s##%d", name.c_str(), skins::skin_kits[k].id);
						if (selectable(buf_name, skins::skin_kits[k].id == selected_entry.paint_kit_index))
							selected_entry.paint_kit_index = skins::skin_kits[k].id;

						ImGui::PopStyleColor();
					}
				}
				ImGui::ListBoxFooter();

				ImGui::PopStyleColor(3);
			});

			ImGui::NextColumn();

			child(strings::skins_customizing.c_str(), [&selected_entry, &sel_entry2]()
			{
				//checkbox("Enabled", u8"Включено", &selected_entry.enabled);

				if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T)
				{
					ImGui::Text(strings::skins_model.c_str());

					std::string knife_name;
					for (const auto& item : skins::knife_names)
					{
						if (item.definition_index == selected_entry.definition_override_index)
							knife_name = item.name;
					}

					if (ImGui::BeginCombo("##skins.knives", knife_name.c_str(), ImGuiComboFlags_HeightLarge))
					{
						for (auto& item : skins::knife_names)
						{
							if (ImGui::Selectable(item.name, selected_entry.definition_override_index == item.definition_index))
								selected_entry.definition_override_index = item.definition_index;
						}

						ImGui::EndCombo();
					}
				}
				else if (selected_entry.definition_index == GLOVE_CT_SIDE || selected_entry.definition_index == GLOVE_T_SIDE)
				{
					ImGui::Text(strings::skins_model.c_str());

					std::string glove_name;
					for (const auto& item : skins::glove_names)
					{
						if (item.definition_index == selected_entry.definition_override_index)
							glove_name = item.name;
					}

					if (ImGui::BeginCombo("##skins.knives", glove_name.c_str(), ImGuiComboFlags_HeightLarge))
					{
						for (auto& item : skins::glove_names)
						{
							if (ImGui::Selectable(item.name, selected_entry.definition_override_index == item.definition_index))
								selected_entry.definition_override_index = item.definition_index;
						}

						ImGui::EndCombo();
					}
				}
				else
					selected_entry.definition_override_index = 0;

				if (selected_entry.definition_index != GLOVE_CT_SIDE && selected_entry.definition_index != GLOVE_T_SIDE)
					checkbox(strings::enabled.c_str(), &selected_entry.enabled);

				/*	int index;

					if (selected_entry.wear >= 0 || selected_entry.wear <= 0.07) //Factory New
						index = 0;

					if (selected_entry.wear >= 0.08 || selected_entry.wear <= 0.15) //Minimal Wear
						index = 1;

					if (selected_entry.wear >= 0.16 || selected_entry.wear <= 0.38) //Field Tested
						index = 2;

					if (selected_entry.wear >= 0.39 || selected_entry.wear <= 0.45) //Well Worn
						index = 3;

					if (selected_entry.wear >= 0.46) //Well Worn
						index = 4;

				static int wearIndex = index;

				const char* types[] =
				{
					"Factory New",
					"Minimal Wear",
					"Field Tested",
					"Well Worn",
					"Battle Scarred",
				};
				types[index];
				ImGui::Text("Wear Presets:");
				ImGui::Combo("Wear Type:", &wearIndex, types, IM_ARRAYSIZE(types));
				*/

				//ImGui::SliderFloatLeftAligned(___("Wear:", u8"Износ:"), &selected_entry.wear, FLT_MIN, 1.f, "%.10f");
				ImGui::Text(strings::skins_wear.c_str());
				ImGui::InputFloat("##skins.wear", &selected_entry.wear);
				tooltip("FN: 0 - 0.07, MW: 0.08 - 0.14, FT: 0.15 - 0.38, WW: 0.39 - 0.45, BS: 0.46");

				ImGui::Text(strings::skins_seed.c_str());
				ImGui::InputInt("##skins.seed", &selected_entry.seed);

				if (selected_entry.definition_index != GLOVE_CT_SIDE && selected_entry.definition_index != GLOVE_T_SIDE)
				{
					ImGui::Text(strings::skins_nametag.c_str());
					ImGui::InputText("##skins.nametag", selected_entry.custom_name, 32);

					columns(2);
					{
						checkbox(strings::skins_stattrack.c_str(), &selected_entry.stat_track.enabled);

						ImGui::NextColumn();

						ImGui::PushItemWidth(-1);
						ImGui::InputInt("##skins.stat_track.counter", &sel_entry2.statrack_new.counter);
						ImGui::PopItemWidth();
					}
					columns(1);
				}

				if (ImGui::Button(strings::skins_save.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 18.f)))
				{
					utils::force_full_update();
					skins::save();

					/*if (wearIndex == 0) //Factory New
						selected_entry.wear = 0.01;

					if (wearIndex == 1) //Minimal Wear
						selected_entry.wear = 0.08;

					if (wearIndex == 2) //Field Tested
						selected_entry.wear = 0.16;

					if (wearIndex == 3) //Well Worn
						selected_entry.wear = 0.39;

					if (wearIndex == 4) //Battle Scarred
						selected_entry.wear = 0.46; */
				}
			});
		}
	}
}
