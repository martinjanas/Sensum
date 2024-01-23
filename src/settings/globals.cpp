#include "globals.h"
#include "config.h"
#include "../render/render.h"

#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../features/features.h"

#include <thread>

namespace globals
{
	bool post_processing = true;
	bool configs_initialized = false;
	bool esp_menu_opened = false;

	float window_alpha = 1.f;
	
	int teamkills = 0;

	std::string settings;

	ImDrawList* draw_list = nullptr;

	std::map<int, int> team_damage = { };
	std::map<int, bool> is_visible = { };
	std::map<int, bool> in_smoke = { };

	std::map<std::string, int> class_ids = { };

	namespace view_matrix
	{
		bool has_offset = false;
		DWORD offset;
	}

	namespace clantag
	{
		float delay = 0.6;
		std::string value;
		bool animation = false;
	}

	namespace binds
	{
		namespace thirdperson
		{
			bool enabled = false;
			int key = 0;
		}

		int esp = 0;
		int trigger = 18; // 18 = L-ALT key, 16 = L-SHIFT, 0 = NOTHING
		
		bool notify_when_loaded = true;
		std::map<std::string, int> configs;

		bool is_pressed(const int& key)
		{
			return key > 0 && input_system::was_key_pressed(key);
		}

		void handle(const int& key)
		{
			if (key <= 0)
				return;

			if (key == esp)
				settings::esp::enabled = !settings::esp::enabled;

			if (key == thirdperson::key && !g::engine_client->IsConsoleVisible())
				thirdperson::enabled = !thirdperson::enabled;

			for (auto& bind : configs)
			{
				if (bind.second > 0 && bind.second == key)
				{
					globals::settings = bind.first;
					configs_initialized = false;
					settings::load(bind.first);

					if (notify_when_loaded)
					{
						static char buf[256];
						sprintf_s(buf, "\"%s\" %s", bind.first.c_str(), "loaded");

						notifies::push(buf);
					}

					return;
				}
			}
		}
	}

	void load()
	{
		config::load("config.json", "", true, [](Json::Value root)
			{
				settings = root["settings"].asString();
				if (!settings.empty())
					settings::load(settings);

				Option::Load(root["post_processing"], post_processing, true);
				Option::Load(root["esp_while_in_menu"], esp_menu_opened, false);
			});
	}

	void save()
	{
		config::save("config.json", "", false, []()
			{
				Json::Value root;

				root["post_processing"] = post_processing;
				root["esp_while_in_menu"] = esp_menu_opened;

				root["configs"] = settings;
				root["clantag"] = clantag::value;

				return root;
			});
	}
}