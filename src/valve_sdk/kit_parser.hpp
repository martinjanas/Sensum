#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace game_data
{
	struct paint_kit
	{
		int id;
		std::string name;
		int rarity;

		paint_kit(int id, const std::string& name)
		{
			this->id = id;
			this->name = name;
		}

		paint_kit(int id, const std::string& name, int rarity)
		{
			this->id = id;
			this->name = name;
			this->rarity = rarity;
		}

		auto operator < (const paint_kit& other) const -> bool
		{
			return name < other.name;
		}
	};

	extern std::vector<paint_kit> skin_kits;
	extern std::vector<paint_kit> glove_kits;
	extern std::vector<paint_kit> sticker_kits;

	extern auto initialize_kits() -> void;
}

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
	int previous_id;		//0x0000
	int next_id;			//0x0004
	void* _unknown_ptr;		//0x0008
	int _unknown;			//0x000C
	Key key;				//0x0010
	Value value;			//0x0014
};

template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory;		//0x0000
	int allocation_count;			//0x0004
	int grow_size;					//0x0008
	int start_element;				//0x000C
	int next_available;				//0x0010
	int _unknown;					//0x0014
	int last_element;				//0x0018
}; //Size=0x001C

// could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
	char* buffer;	//0x0000
	int capacity;	//0x0004
	int grow_size;	//0x0008
	int length;		//0x000C
}; //Size=0x0010

struct CPaintKit
{
	int id;

	String_t name;
	String_t description_string;
	String_t item_name;
	String_t sameNameFamilyAggregate;
	String_t pattern;
	String_t normal;
	String_t logo_material;

	bool base_diffuse;
	int rarity;
};

struct CStickerKit
{
	int id;

	int item_rarity;

	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	String_t image_inventory2;
	String_t image_inventory_large;

	std::uint32_t pad0[4];
};

