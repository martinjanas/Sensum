#include "config_system.h"
#include "../../settings/settings.h"
#include <fstream>

namespace config_system
{
	void load_weapon_configs(nlohmann::ordered_json& config)
	{
		if (!config.contains("Legitbot"))
			return;

		settings::aimbot::weapon_config_t weapon_config;
		for (const auto& data : config["Legitbot"].items())
		{
			int wpn_index = weapon_ids[data.key()];
			const auto& wpn_data = data.value();

			weapon_config.enabled = wpn_data.value("enabled", false);
			weapon_config.fov = wpn_data.value("fov", 0.f);
			weapon_config.smooth = wpn_data.value("smooth", 0.0f);
			weapon_config.smooth_mode = wpn_data.value("smooth_mode", 0);
			weapon_config.hitboxes = wpn_data.value("hitboxes", 0);
			weapon_config.nearest_hitbox_in_air_override = wpn_data.value("nearest_hitbox_in_air_override", false);
			
			if (wpn_data.contains("recoil"))
			{
				weapon_config.recoil.enabled = wpn_data["recoil"].value("enabled", false);
				weapon_config.recoil.pitch = wpn_data["recoil"].value("pitch", 0.0f);
				weapon_config.recoil.yaw = wpn_data["recoil"].value("yaw", 0.0f);
			}

			settings::aimbot::weapon_configs[wpn_index] = weapon_config;
		}
	}

	void save_weapon_configs(nlohmann::ordered_json& config)
	{
		for (const auto& data : settings::aimbot::weapon_configs) //saves null when saving in the menu/saves only m_hActiveWeapon guns
		{
			const auto& weapon_name = weapon_names[data.first];
			if (weapon_name.empty())
				continue;

			const auto aimbot_data = data.second;

			config["Legitbot"][weapon_name.c_str()]["enabled"] = aimbot_data.enabled;
			config["Legitbot"][weapon_name.c_str()]["fov"] = aimbot_data.fov;
			config["Legitbot"][weapon_name.c_str()]["smooth"] = aimbot_data.smooth;
			config["Legitbot"][weapon_name.c_str()]["smooth_mode"] = aimbot_data.smooth_mode;
			config["Legitbot"][weapon_name.c_str()]["hitboxes"] = aimbot_data.hitboxes;
			config["Legitbot"][weapon_name.c_str()]["nearest_hitbox_in_air_override"] = aimbot_data.nearest_hitbox_in_air_override;
			config["Legitbot"][weapon_name.c_str()]["recoil"]["enabled"] = aimbot_data.recoil.enabled;
			config["Legitbot"][weapon_name.c_str()]["recoil"]["pitch"] = aimbot_data.recoil.pitch;
			config["Legitbot"][weapon_name.c_str()]["recoil"]["yaw"] = aimbot_data.recoil.yaw;
		}
	}

	void load()
	{
		nlohmann::ordered_json config;

		std::ifstream file("legit.json");
		if (file.is_open()) 
		{
			try 
			{
				file >> config;

				load_weapon_configs(config);
		
				printf("Configuration loaded successfully!\n");
			}
			catch (const nlohmann::json::parse_error& e) {
				printf("Error parsing JSON: %s", e.what());
			}
			catch (const std::exception& e)
			{
				printf("Error: %s", e.what());
			}
			file.close();
		}
	}

	void save()
	{
		nlohmann::ordered_json config;

		save_weapon_configs(config);

		std::ofstream file("legit.json");
		if (file.is_open())
		{
			file << config.dump(4);
			file.close();
		}
	}
}