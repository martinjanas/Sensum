#include "kit_parser.hpp"
#include "../helpers/utils.h"

#include <algorithm>

std::vector<game_data::paint_kit> game_data::skin_kits;
std::vector<game_data::paint_kit> game_data::glove_kits;

auto game_data::initialize_kits() -> void
{
	static const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(utils::get_export("vstdlib.dll", "V_UCS2ToUTF8"));

	const auto sig_address = utils::pattern_scan("client.dll", "E8 ? ? ? ? FF 76 0C 8D 48 04");

	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);

	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);

	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);

		const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		const auto head_offset = start_element_offset - 12;

		const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);
		
		char name[256];
		for(auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
			   continue;

			const auto wide_name = g::localize->Find(paint_kit->item_name.buffer + 1);
			
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			static auto rename_skin = [&](const char* new_name, const int& skin_id)
			{
				if (paint_kit->id == skin_id)
					sprintf(name, "%s", new_name);
			};

			rename_skin("Default", 0);
			rename_skin("Dragon King", 400);
			rename_skin("Redline (AK47)", 282);
			rename_skin("Elite Build (AK47)", 422);
			rename_skin("Hyper Beast (AWP)", 475);
			rename_skin("Asiimov (AWP)", 279);
			rename_skin("Elite Build (AWP)", 525);
			rename_skin("Redline (AWP)", 259);
			rename_skin("Fever Dream (AWP)", 640);
			rename_skin("Capillary (AWP)", 943);
			rename_skin("Blaze (Deagle)", 37);
			rename_skin("Asiimov (AK47)", 801);
			rename_skin("Asiimov (P90)", 359);
			rename_skin("Asiimov (M4A4)", 255);
			rename_skin("Asiimov (P250)", 551);
			
			if (paint_kit->id < 10000)
				game_data::skin_kits.emplace_back(paint_kit->id, name, paint_kit->rarity);
			else
				game_data::glove_kits.emplace_back(paint_kit->id, name);
		}

		std::sort(game_data::skin_kits.begin()+1, game_data::skin_kits.end());
		std::sort(game_data::glove_kits.begin(), game_data::glove_kits.end());
	}
}