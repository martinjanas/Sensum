#include "config_system.h"

namespace config_system
{
	void load()
	{

	}

	void save()
	{
		nlohmann::json config;

		config["Misc"]["Bhop"]["Enabled"] = true;
		config["Misc"]["Fov Changer"]["Enabled"] = false;
		config["Legit"]["AK47"]["Fov"] = 45;
		config["Legit"]["Deagle"]["Fov"] = 12;

		/*std::ofstream file("legit.json");
		if (file.is_open())
		{
			file << config.dump(4);
			file.close();
		}*/
	}
}