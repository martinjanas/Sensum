#include "features.h"
#include "../helpers/console.h"

#include <format>

namespace features
{
	static std::map<const char*, const char*> weapon_names =
	{
		{ "weapon_ak47", "AK47" },
		{ "weapon_aug", "AUG" },
		{ "weapon_famas", "FAMAS" },
		{ "weapon_galilar", "Galil AR" },
		{ "weapon_m4a1", "M4A4" },
		{ "weapon_m4a1_silencer", "M4A1-S" },
		{ "weapon_sg556", "SG 556" },

		{ "weapon_awp", "AWP" },
		{ "weapon_g3sg1", "G3SG1" },
		{ "weapon_scar20", "SCAR20" },
		{ "weapon_ssg08", "SSG 08" },

		{ "weapon_mac10", "MAC10" },
		{ "weapon_mp7", "MP7" },
		{ "weapon_mp9", "MP9" },
		{ "weapon_bizon", "PP-Bizon" },
		{ "weapon_p90", "P90" },
		{ "weapon_ump45", "UMP-45" },
		{ "weapon_mp5sd", "MP5-SD" },

		{ "weapon_m249", "M249" },
		{ "weapon_negev", "Negev" },

		{ "weapon_hkp2000", "P2000" },
		{ "weapon_usp_silencer", "USP-S" },
		{ "weapon_p250", "P250" },
		{ "weapon_elite", "Dual Berettas" },
		{ "weapon_fiveseven", "Five-Seven" },
		{ "weapon_glock", "Glock" },
		{ "weapon_tec9", "Tec-9" },
		{ "weapon_deagle", "Deagle" },
		{ "weapon_cz75a", "CZ75-A" },
		{ "weapon_revolver", "Revolver" },

		{ "weapon_mag7", "MAG-7" },
		{ "weapon_nova", "Nova" },
		{ "weapon_sawedoff", "Sawed Off" },
		{ "weapon_xm1014", "XM1014" },

		{ "item_defuser", "Defuse Kit" },
		{ "item_kevlar", "Kevlar" },
		{ "item_assaultsuit", "Kevlar + Helmet" },

		{ "weapon_hegrenade", "HE" },
		{ "weapon_flashbang", "Flashbang" },
		{ "weapon_molotov", "Molotov" },
		{ "weapon_smokegrenade", "Smoke" },
		{ "weapon_incgrenade", "Molotov" },
		{ "weapon_decoy", "Decoy" },
		{ "weapon_taser", "Zeus x27" }
	};

	void format_names(const char* weapon, c_base_player* player)
	{
		static std::string output;

		static auto green = " \x04";
		static auto yellow = " \x10";
		static auto white = " \x01";

		for (auto& it : weapon_names) //TODO: Convert weapon_names to hash for better performance.
		{
			/*for (auto& hash : weapon_names_hashes)
			{
				auto weapon_hash = fnv::hash_runtime(weapon);
				if (hash == weapon_hash)
				{
					if (hash == item_defuser_blacklisted_hash) //TODO: Probably loop thru array of blacklisted hashes.
						continue;
					output = std::format("{:s}[Buy] {:s}Player {:s}{:.8s} {:s}bought {:s}{:s}", yellow, green, white, player->GetPlayerInfo().szName, green, white, it.second);
					g::hud_chat->ChatPrintf(0, 0, output.c_str());
				}
			}*/

			if (strstr(it.first, weapon))
			{
				if (!strcmp(it.first, "item_defuser") || !strcmp(it.first, "item_kevlar") //TODO: Convert to hash.
					|| !strcmp(it.first, "item_assaultsuit") || !strcmp(it.first, "weapon_taser")
					|| !strcmp(it.first, "weapon_decoy") || !strcmp(it.first, "weapon_incgrenade")
					|| !strcmp(it.first, "weapon_molotov") || !strcmp(it.first, "weapon_hegrenade")
					|| !strcmp(it.first, "weapon_smokegrenade") || !strcmp(it.first, "weapon_flashbang"))
					continue;

				output = std::format("{:s}[Buy] {:s}{:.8s} {:s}bought {:s}{:s}", yellow, white, player->GetPlayerInfo().szName, green, white, it.second);
				g::hud_chat->ChatPrintf(0, 0, output.c_str());
			}
		}
	}

	void buy_logger(IGameEvent* event)
	{
		if (!settings::esp::buylog)
			return;

		if (!utils::IsPlayingMM())
			return;

		c_base_player* enemy = c_base_player::GetPlayerByUserId(event->GetInt("userid"));

		if (!enemy || !g::local_player || enemy->m_iTeamNum() == g::local_player->m_iTeamNum())
			return;

		format_names(event->GetString("weapon"), enemy);
	}
}