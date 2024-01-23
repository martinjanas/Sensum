#pragma once
#include "../security/xor.h"

#define GLOBAL_VARS				 xorstr_("client.dll"), xorstr_("A1 ? ? ? ? 5E 8B 40 10")
#define CLIENT_MODE				 xorstr_("client.dll"), xorstr_("B9 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 53")
#define CINPUT					 xorstr_("client.dll"), xorstr_("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10")
#define MOVE_HELPER				 xorstr_("client.dll"), xorstr_("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01")
#define VIEW_RENDER				 xorstr_("client.dll"), xorstr_("A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10")
#define D3D_DEVICE				 xorstr_("shaderapidx9.dll"), xorstr_("A1 ? ? ? ? 50 8B 08 FF 51 0C")
#define CLIENT_STATE 			 xorstr_("engine.dll"), xorstr_("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0")
#define LOCAL_PLAYER			 xorstr_("client.dll"), xorstr_("8B 0D ? ? ? ? 83 FF FF 74 07")
#define CRENDER					 xorstr_("engine.dll"), xorstr_("56 57 0F 57 C0 C7 05 ? ? ? ? ? ? ? ? 51")
#define CHUD					 xorstr_("client.dll"), xorstr_("B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24")
#define PLAYER_RESOURCE			 xorstr_("client.dll"), xorstr_("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7")
#define GLOW_MANAGER			 xorstr_("client.dll"), xorstr_("0F 11 05 ? ? ? ? 83 C8 01")
#define WEAPON_SYSTEM			 xorstr_("client.dll"), xorstr_("8B 35 ? ? ? ? FF 10 0F B7 C0")
#define FIRE_BULLETS			 xorstr_("client.dll"), xorstr_("55 8B EC 51 53 56 8B F1 BB ? ? ? ? B8")
#define GAME_RULES_PROXY 	     xorstr_("client.dll"), xorstr_("A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A")
#define VIEW_RENDER_BEAMS		 xorstr_("client.dll"), xorstr_("B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9")

#define IS_LINE_GOES_THROUGH_SMOKE xorstr_("client.dll"), xorstr_("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0")
#define IS_LINE_GOES_THROUGH_SMOKE_FN xorstr_("client.dll"), xorstr_("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F")
#define SET_SKY					xorstr_("engine.dll"), xorstr_("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")
#define CLEAR_HUD_WEAPON_ICON	xorstr_("client.dll"), xorstr_("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC")
#define FORCE_FULL_UPDATE		xorstr_("engine.dll"), xorstr_("A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85")
#define DRAW_POINTS				xorstr_("studiorender.dll"), xorstr_("8B 7D FC 03 F8")
#define SET_MATCH_ACCEPTED		xorstr_("client.dll"), xorstr_("55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")
#define PRESENT_OVERLAY			xorstr_("gameoverlayrenderer.dll"), xorstr_("FF 15 ? ? ? ? 8B F8 85 DB")
#define RESET_OVERLAY			xorstr_("gameoverlayrenderer.dll"), xorstr_("FF 15 ? ? ? ? 8B F8 85 FF 78 18")
#define CURRENT_COMMAND			xorstr_("client.dll"), xorstr_("89 BE ? ? ? ? E8 ? ? ? ? 85 FF")
#define HAS_C4					xorstr_("client.dll"), xorstr_("56 8B F1 85 F6 74 31")
#define SET_ABS_ORIGIN			xorstr_("client.dll"), xorstr_("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8")
#define INRELOAD				xorstr_("client.dll"), xorstr_("C6 87 ? ? ? ? ? 8B 06 8B CE FF 90")
#define OVERRIDE_BLUR			xorstr_("client.dll"), xorstr_("80 3D ? ? ? ? ? 53 56 57 0F 85")
#define FIND_HUD				xorstr_("client.dll"), xorstr_("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39")
#define UPDATE_VISIBILITY_ENTITIES xorstr_("client.dll"), xorstr_("53 56 66 8B ? ? ? ? ? ? FF FF ? ? 57 90")
#define UPDATE_ANIM_STATE		xorstr_("client.dll"), xorstr_("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24")
#define SET_ABS_ANGLE			xorstr_("client.dll"), xorstr_("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8")
#define INVALIDATE_BONE_CACHE	xorstr_("client.dll"), xorstr_("80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81")
#define SET_CLAN_TAG			xorstr_("engine.dll"), xorstr_("53 56 57 8B DA 8B F9 FF 15")
#define PREDICTION_RANDOM_SEED	xorstr_("client.dll"), xorstr_("8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04")
#define IS_GAME_STARTED			xorstr_("materialsystem.dll"), xorstr_("80 B9 ? ? ? ? ? 74 0F")
#define LOBBIES_COLLECTION		xorstr_("client.dll"), xorstr_("8B 35 ? ? ? ? 66 3B D0 74 07")
#define GET_SEQUENCE_ACTIVITY	xorstr_("client.dll"), xorstr_("55 8B EC 53 8B 5D 08 56 8B F1 83")
#define REVEAL_RANKS            xorstr_("client.dll"), xorstr_("55 8B EC 51 A1 ? ? ? ? 85 C0 75 37")

namespace patterns
{
	void initialize();

	inline extern uint8_t* set_sky_addr = 0;
	inline extern uint8_t* find_hud_addr = 0;
	inline extern uint8_t* set_clan_tag_addr = 0;
	inline extern uint8_t* update_visibility_entities_addr = 0;
	inline extern uint8_t* set_abs_origin_addr = 0;
	inline extern uint8_t* set_abs_angle_addr = 0;
	inline extern uint8_t* invalidate_bonecache_addr = 0;
	inline extern uint8_t* in_reload_addr = 0;
	inline extern uint8_t* current_command_addr = 0;
	inline extern uint8_t* has_c4_addr = 0;
	inline extern uint8_t* get_sequence_activity_addr = 0;
	inline extern uint8_t* is_line_goes_through_smoke_addr = 0;
	inline extern uint8_t* is_line_goes_through_smoke_fn_addr = 0;
	inline extern uint8_t* reveal_ranks_addr = 0;
}

