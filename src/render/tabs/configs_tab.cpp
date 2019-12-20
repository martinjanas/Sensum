#include "../render.h"
#include "../../config.h"
#include "../../globals.h"
#include "../../settings.h"
#include "../../helpers/notifies.h"

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
				config::cache("settings");

				is_loaded = true;
			}

			child(strings::config_newconfig.c_str(), []()
			{
				ImGui::InputText("##filename", filename, 32);

				if (ImGui::Button(strings::create.c_str(), button_size))
				{
					if (strlen(filename) == 0)
					{
						notifies::push(strings::config_configname.c_str(), notify_state_s::warning_state);
					}
					else
					{
						settings::save(std::string(filename));
						memset(filename, 0, 32);
						is_loaded = false;

						notifies::push(strings::config_created.c_str());

						globals::settings = currentName;
						globals::save();
					}
				}

				separator(strings::config_actions.c_str());

				if (!currentName.empty())
				{
					ImGui::InputText("##currentname", current_loaded_config, 32);

					if (ImGui::Button(strings::config_rename.c_str(), button_size))
					{
						if (strlen(current_loaded_config) == 0)
						{
							notifies::push(strings::config_newname.c_str(), notify_state_s::warning_state);
						}
						else
						{
							settings::load(currentName);
							config::remove(currentName, "settings");

							currentName = std::string(current_loaded_config);
							settings::save(currentName);
							is_loaded = false;

							notifies::push(strings::config_renamed.c_str());

							globals::settings = currentName;
							globals::save();
						}
					}

					if (ImGui::Button(strings::config_load.c_str(), button_size))
					{
						settings::load(currentName);

						globals::settings = currentName;
						globals::save();

						notifies::push(strings::config_loaded.c_str());
					}

					if (ImGui::Button(strings::config_save.c_str(), button_size))
					{
						settings::save(currentName);

						globals::settings = currentName;
						globals::save();

						notifies::push(strings::config_saved.c_str(), notify_state_s::success_state);
					}

					if (ImGui::Button(strings::delete_st.c_str(), button_size))
					{
						if (config::remove(currentName, "settings"))
						{
							currentName.clear();
							is_loaded = false;

							notifies::push(strings::config_removed.c_str(), notify_state_s::success_state);
						}
						else
						{
							notifies::push(strings::config_cantremove.c_str(), notify_state_s::warning_state);
						}
					}
				}
			});

			ImGui::NextColumn();

			child(strings::config_list.c_str(), []()
			{
				if (!render::fonts::configs_list)
					return;

				ImGui::PushFont(render::fonts::configs_list);
				{
					ImGui::ListBoxHeader("##configs", get_listbox_size(0.f, 26.f));
					{
						for (auto& config : config::cached["settings"])
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

				if (ImGui::Button(strings::config_refresh.c_str(), button_size))
					is_loaded = false;
			});

			ImGui::NextColumn();

			child(strings::config_binds.c_str(), []()
			{
				checkbox(strings::config_notify.c_str(), &globals::binds::notify_when_loaded);

				for (auto& config : config::cached["settings"])
					bind_button(config.first.c_str(), globals::binds::configs[config.first]);
			});
		}
	}
}
