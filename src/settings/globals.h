#pragma once

#include <map>
#include <vector>
#include <string>
#include "options.hpp"

namespace globals
{
	extern bool post_processing;
	extern bool configs_initialized;
	extern bool esp_menu_opened;

	extern float window_alpha;
	
	extern int teamkills;

	extern ImDrawList* draw_list;

	extern std::string settings;

	extern std::map<int, int> team_damage;
	extern std::map<int, bool> is_visible;
	extern std::map<int, bool> in_smoke;
	extern std::map<std::string, int> class_ids;

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

		extern int esp;
		extern int trigger;
		
		extern bool notify_when_loaded;
		extern std::map<std::string, int> configs;

		void handle(const int& key);
	}
}