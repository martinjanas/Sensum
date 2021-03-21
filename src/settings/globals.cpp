#include "globals.h"
#include "config.h"
#include "../render/render.h"

#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../features/features.h"

#include <thread>

extern float side;

namespace clantag_features = clantag;

namespace globals
{
	bool post_processing = true;
	bool configs_initialized = false;
	bool sound;
	bool esp_menu_opened = false;

	int yaw_offset = 90;
	float last_yaw_offset = 90.f;

	float window_alpha = 1.f;

	int teamkills = 0;

	std::string settings;
	std::string playername;

	QAngle angles = QAngle();
	ImDrawList* draw_list = nullptr;

	std::map<int, int> team_damage = { };

	std::map<int, int> team_kill = { };

	QAngle aim_punch = QAngle();
	QAngle viewangles = QAngle();

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

		int desync = 0;
		int esp = 0;
		int trigger = 18; // 18 = L-ALT key, 16 = L-SHIFT, 0 = NOTHING
		int zeus_bot = 0;
		int slow_walk = 0;
		int edge_jump = 0;
		
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

			if (!settings::desync::yaw_flip && key == desync)
				desync::flip_yaw = !desync::flip_yaw;

			if (key == desync)
				side = -side;

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
						char buf[256];
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

				clantag::value = root["clantag"].asString();
				clantag_features::restore();

				Option::Load(root["post_processing"], post_processing, true);
				Option::Load(root["esp_while_in_menu"], esp_menu_opened, false);

				Option::Load(root["binds.esp"], binds::esp, 0);
				Option::Load(root["binds.trigger"], binds::trigger, 0);
				Option::Load(root["binds.zeus_bot"], binds::zeus_bot, 0);
				Option::Load(root["binds.thirdperson"], binds::thirdperson::key, 86);
				Option::Load(root["binds.desync"], binds::desync, VK_XBUTTON1);
				Option::Load(root["binds.slow_walk"], binds::slow_walk);
				Option::Load(root["binds.edge_jump"], binds::edge_jump);

				Json::Value config_binds = root["binds.configs"];
				if (!config_binds.empty())
				{
					for (Json::Value::iterator it = config_binds.begin(); it != config_binds.end(); ++it)
					{
						Json::Value settings = *it;

						if (settings["key"].asInt() > 0)
							binds::configs[settings["name"].asString()] = settings["key"].asInt();
					}
				}
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

				root["binds.esp"] = binds::esp;
				root["binds.trigger"] = binds::trigger;
				root["binds.zeus_bot"] = binds::zeus_bot;
				root["binds.thirdperson"] = binds::thirdperson::key;
				root["binds.desync"] = binds::desync;
				root["binds.slow_walk"] = binds::slow_walk;
				root["binds.edge_jump"] = binds::edge_jump;

				Json::Value config_binds;
				for (auto& bind : binds::configs)
				{
					Json::Value bind_data;
					bind_data["key"] = bind.second;
					bind_data["name"] = bind.first;

					config_binds.append(bind_data);
				}

				root["binds.configs"] = config_binds;

				return root;
			});
	}
}