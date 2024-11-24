#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <cstddef>

#include "../sdk/interfaces/IVEngineClient.h"
#include "../sdk/interfaces/SchemaSystem.h"
#include "../sdk/interfaces/IMemAlloc.h"
#include "../sdk/interfaces/CGameResourceService.h"
#include "../sdk/interfaces/CSGOInput.h"
#include "../sdk/interfaces/CSource2Client.h"
#include "../sdk/interfaces/CInputSystem.h"
#include "../sdk/interfaces/CGameTrace.h"
#include "../sdk/interfaces/GameEntitySystem.h"
#include "../sdk/math/Viewmatrix.h"

class CGlobalVarsBase
{
public:
	float m_realtime; //0x0000
	int32_t m_framecount; //0x0004
	float m_frametime; //0x0008 //returns 0
	float m_abs_frametime; //0x000C //returns 0
	int32_t m_maxclients; //0x0010
	char pad_0014[28]; //0x0014
	float m_frametime2; //0x0030
	float m_curtime; //0x0034 //outputs valid data
	float m_curtime2; //0x0038 //outputs valid data, same data as m_curtime
	char pad_003C[20]; //0x003C
	int32_t m_tickcount; //0x0050 //outputs garbage data
	char pad_0054[292]; //0x0054
	uint64_t m_current_map; //0x0178
	uint64_t m_current_mapname; //0x0180
}; //Size: 0x0188

class CClientModeCSNormal
{
public:

};

class CNetworkGameClient
{
public:
	//CGlobalVarsBase* GetGlobalVars()
	//{
	//	//xref: Curtime( %f )\nRealtime( %f )\n module: engine2
	//	return GetVirtual<CGlobalVarsBase * (__thiscall*)(void*)>(this, 4)(this);
	//}

	void FullUpdate()
	{
		//   @ida: #STR: "Requesting full game update (%s)...\n"
		VTable::GetThiscall<void>(this, 28, "unk");
	}
};

class CNetworkGameService
{
public:
	CNetworkGameClient* GetNetworkGameClient()
	{
		//return *reinterpret_cast<CNetworkGameClient**>(reinterpret_cast<uintptr_t>(this) + 0xB8);

		return VTable::GetThiscall<CNetworkGameClient*>(this, 23);
	}
};

class CGameType
{
public:
	bool IsDeathmatch()
	{
		return GetGameType() == 1 && GetGameMode() == 2;
	}

	bool IsCasual()
	{
		return GetGameType() == 0 && GetGameMode() == 0;
	}

	bool IsCompetitive()
	{
		return GetGameType() == 0 && GetGameMode() == 1;
	}

	bool IsWingman()
	{
		return GetGameType() == 0 && GetGameMode() == 2;
	}

	bool IsArmsrace()
	{
		return GetGameType() == 1 && GetGameMode() == 0;
	}

	bool IsMatchmaking()
	{
		return IsCasual() || IsCompetitive();
	}
private:
	int GetGameType()
	{
		return VTable::GetThiscall<int>(this, 19);
	}

	int GetGameMode()
	{
		return VTable::GetThiscall<int>(this, 20);
	}
};

class CHudChat
{
public:
	enum ChatFilters
	{
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		CHAT_FILTER_ACHIEVEMENT = 0x000020,
	};

	void ChatPrintf(const char* fmt, ...) //crashing
	{
		using fn = void(__cdecl*)(void*, uint32_t, const char*, ...);
		static auto addr = modules::client.scan("E8 ?? ?? ?? ?? 49 8B 4E 20 BA ?? ?? ?? ??", "ChatPrintf").add(0x1).abs().as();
		if (!addr)
			return;

		auto chat_printf = reinterpret_cast<fn>(addr);
		if (chat_printf)
			chat_printf(this, -1, fmt);
	}
};

namespace sdk
{
	void init_modules();
	void init_interfaces();
	void scan_and_cache_sigs();
}

class CRenderSystem //TODO: Move this into its own file
{
public:
	std::byte pad001[0x170];
	IDXGISwapChain* swap_chain;
};

namespace interfaces //move to interfaces.h ?
{
	extern CCSPlayerController* localplayer;
	extern IVEngineClient* engine_client;
	extern CSource2Client* client;
	extern CSchemaSystem* schema_system;
	extern IMemAlloc* mem_alloc;
	extern CGameResourceService* game_resource_service;
	extern CEntitySystem* entity_system;
	extern CSGOInput* csgo_input;
	extern CRenderSystem* render_system;
	extern CInputSystem* input_system;
	extern CClientModeCSNormal* client_mode_csnormal;
    extern CGlobalVarsBase* global_vars;
	extern CGameTrace* engine_trace;
	extern CNetworkGameService* network_game_service;
	extern CGameType* game_type;
	extern CHudChat* hud_chat;
}

namespace g = interfaces;
