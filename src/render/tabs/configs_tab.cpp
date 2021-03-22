#include "../render.h"
#include "../../settings/config.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../helpers/notifies.h"
#include "../../features/features.h"

extern void bind_button(const char* label, int& key);

namespace render
{
	namespace menu
	{
		char filename[32];
		bool is_loaded = true;
		std::string currentName;
		char current_loaded_config[32];

		ImVec2 button_size = ImVec2();

		void initialize()
		{
			if (globals::configs_initialized)
				return;

			globals::configs_initialized = true;
			currentName = globals::settings;
			strcpy(current_loaded_config, globals::settings.c_str());

			button_size = ImVec2(ImGui::GetContentRegionAvailWidth(), 24.f);
		}

		void configs_tab()
		{
			initialize();

			if (!is_loaded)
			{
				config::cache("configs");

				is_loaded = true;
			}

			child("New Config", []()
				{
					ImGui::InputText("##filename", filename, 32);

					if (ImGui::Button("Create", button_size))
					{
						if (strlen(filename) == 0)
						{
							notifies::push("Enter config name", notify_state_s::warning_state);
						}
						else
						{
							settings::save(std::string(filename));
							memset(filename, 0, 32);
							is_loaded = false;

							notifies::push("Config created");

							globals::settings = currentName;
							globals::save();
						}
					}

					separator("Some Actions");

					if (!currentName.empty())
					{
						ImGui::InputText("##currentname", current_loaded_config, 32);

						if (ImGui::Button("Rename", button_size))
						{
							if (strlen(current_loaded_config) == 0)
							{
								notifies::push("Enter new config name", notify_state_s::warning_state);
							}
							else
							{
								settings::load(currentName);
								config::remove(currentName, "configs");

								currentName = std::string(current_loaded_config);
								settings::save(currentName);
								is_loaded = false;

								notifies::push("Config renamed");

								globals::settings = currentName;
								globals::save();
							}
						}

						if (ImGui::Button("Load", button_size))
						{
							settings::load(currentName);

							globals::load();

							skins::load();

							notifies::push("Config loaded");
						}

						ImGui::NewLine();
						
						if (ImGui::Button("Save", button_size))
						{
							settings::save(currentName);

							globals::settings = currentName;
							globals::save();

							skins::save();

							notifies::push("Config saved", notify_state_s::success_state);
						}

						ImGui::NewLine();

						if (ImGui::Button("Remove", button_size))
						{
							if (config::remove(currentName, "configs"))
							{
								currentName.clear();
								is_loaded = false;

								notifies::push("Config removed", notify_state_s::success_state);
							}
							else
							{
								notifies::push("Something went wrong", notify_state_s::warning_state);
							}
						}
					}
				});

			ImGui::NextColumn();

			child("Configs", []()
				{
					if (!render::fonts::configs_list)
						return;

					ImGui::PushFont(render::fonts::configs_list);
					{
						ImGui::ListBoxHeader("##configs", get_listbox_size(0.f, 26.f));
						{
							for (auto& config : config::cached["configs"])
							{
								if (selectable(config.first.c_str(), config.first == currentName))
								{
									strcpy(current_loaded_config, config.first.c_str());
									currentName = config.first;
								}
							}
						}
						ImGui::ListBoxFooter();
					}
					ImGui::PopFont();

					if (ImGui::Button("Refresh", button_size))
						is_loaded = false;
				});

			ImGui::NextColumn();

			child("Binds & Misc", []()
				{
					checkbox("Show notify when loaded", &globals::binds::notify_when_loaded);
					checkbox("ESP while in menu", &globals::esp_menu_opened);
					
					for (auto& config : config::cached["configs"])
						bind_button(config.first.c_str(), globals::binds::configs[config.first]);
				});
		}
	}
}