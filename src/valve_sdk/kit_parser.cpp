#include "kit_parser.hpp"
#include "../helpers/utils.h"

#include <algorithm>

std::vector<game_data::paint_kit> game_data::skin_kits;
std::vector<game_data::paint_kit> game_data::glove_kits;
std::vector<game_data::paint_kit> game_data::sticker_kits;

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

			if(paint_kit->id == 9001)
			   continue;

			const auto wide_name = g::localize->Find(paint_kit->item_name.buffer + 1);

			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			if(paint_kit->id < 10000)
				game_data::skin_kits.push_back({ paint_kit->id, name });
			else
				game_data::glove_kits.push_back({ paint_kit->id, name });
		}

		std::sort(game_data::skin_kits.begin(), game_data::skin_kits.end());
		std::sort(game_data::glove_kits.begin(), game_data::glove_kits.end());
	}

	// Dump sticker kits
	/*{
		const auto sticker_sig = utils::pattern_scan("client.dll", "\x53\x8D\x48\x04\xE8\x00\x00\x00\x00\x8B\x4D\x10") + 4;

		const auto get_sticker_kit_definition_offset = *reinterpret_cast<std::intptr_t*>(sticker_sig + 1);

		const auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		const auto start_element_offset = *reinterpret_cast<intptr_t*>(std::uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		const auto head_offset = start_element_offset - 12;

		const auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(std::uintptr_t(item_schema) + head_offset);

		char name[256];
		for(auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;
			
			//if it is SprayKit - ignore, it can`t be installed same as StickerKit
			if (sticker_name_ptr[1] == 'p')
				continue;

			if(strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const auto wide_name = g::localize->Find(sticker_name_ptr);

			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			game_data::sticker_kits.push_back({ sticker_kit->id, name });
		}

		std::sort(game_data::sticker_kits.begin(), game_data::sticker_kits.end());

		game_data::sticker_kits.insert(game_data::sticker_kits.begin(), { 0, "None" });
	}*/
}