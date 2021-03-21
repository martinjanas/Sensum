#pragma once

#include <map>
#include <vector>
#include <string>
#include "options.hpp"

namespace globals
{
	extern bool post_processing;
	extern bool configs_initialized;
	extern bool sound;
	extern bool esp_menu_opened;

	extern int yaw_offset;
	extern float last_yaw_offset;

	extern float window_alpha;

	extern int teamkills;

	extern QAngle angles;
	extern ImDrawList* draw_list;

	extern std::string settings;
	extern std::string playername;

	extern std::map<int, int> team_damage;
	extern std::map<int, int> team_kill;

	extern QAngle aim_punch;
	extern QAngle viewangles;

	void save();
	void load();

	namespace view_matrix
	{
		extern bool has_offset;
		extern DWORD offset;
	}

	namespace clantag
	{
		extern float delay;
		extern std::string value;
		extern bool animation;
	}

	namespace binds
	{
		namespace thirdperson
		{
			extern bool enabled;
			extern int key;
		}

		extern int desync;
		extern int esp;
		extern int trigger;
		extern int zeus_bot;
		extern int slow_walk;
		extern int edge_jump;
		
		extern bool notify_when_loaded;
		extern std::map<std::string, int> configs;

		void handle(const int& key);
	}
}