#pragma once

#include "patterns.h"
#include "../security/importer.h"

#define NOMINMAX
#include <Windows.h>
#include <map>

#include "misc/vfunc.hpp"

#include "misc/Enums.hpp"

#include "math/VMatrix.hpp"
#include "math/QAngle.hpp"
#include "math/Vectors.hpp"
#include "misc/Studio.hpp"

#include "interfaces/IRefCounted.h"
#include "interfaces/IAppSystem.h"
#include "interfaces/IClientEntity.hpp"
#include "interfaces/IClientEntityList.hpp"
#include "interfaces/IClientMode.hpp"
#include "interfaces/IConVar.hpp"
#include "misc/ClientClass.hpp"
#include "interfaces/IEngineTrace.hpp"
#include "interfaces/IVEngineClient.hpp"
#include "interfaces/CInput.hpp"
#include "interfaces/IGameEvent.hpp"
#include "interfaces/IMaterialSystem.hpp"
#include "interfaces/IMoveHelper.hpp"
#include "interfaces/IMDLCache.hpp"
#include "interfaces/IPrediction.hpp"
#include "interfaces/IPanel.hpp"
#include "interfaces/IViewRender.hpp"
#include "interfaces/IPhysics.hpp"
#include "interfaces/IMaterial.h"
#include "interfaces/IVModelRender.h"
#include "interfaces/IBaseClientDLL.h"
#include "interfaces/IVRenderView.h"
#include "interfaces/ISurface.h"
#include "interfaces/IVModelInfo.h"
#include "interfaces/CGlobalVarsBase.h"
#include "interfaces/CClientState.h"
#include "interfaces/ILocalize.h"
#include "interfaces/IMemAlloc.h"
#include "interfaces/IFileSystem.h"
#include "interfaces/IViewRenderBeams.h"
#include "interfaces/IStudioRender.h"
#include "interfaces/IEngineSound.hpp"
#include "interfaces/IMaterialVar.h"
#include "interfaces/IInputSystem.h"
#include "interfaces/CHud.h"
#include "interfaces/CHudChat.h"
#include "interfaces/ServerClass.h"
#include "interfaces/IVDebugOverlay.h"
#include "interfaces/CFireBullets.h"

#include "misc/Convar.hpp"
#include "interfaces/ICvar.h"
#include "misc/CUserCmd.hpp"

#include "netvars.hpp"
#include "steam.h"

struct IDirect3DDevice9;

class ActiveChannels 
{
public:
	int count;
	short list[128];
};

class Channel 
{
public:
	std::byte pad001[244];
	int sound_source;
	std::byte pad002[56];
	Vector origin;
	Vector direction;
	std::byte pad003[80];
};

class CLocalPlayer
{
public:
	CLocalPlayer() : m_local(nullptr) {}
	
	operator bool() const 
	{ 
		return *m_local != nullptr; 
	}

	operator c_base_player* () const 
	{ 
		return *m_local; 
	}

	c_base_player* operator->() 
	{ 
		return *m_local; 
	}
private:
	c_base_player** m_local;
};

class CSPlayerResource;

class CSGameRulesProxy;

namespace g
{
	extern ActiveChannels* active_channel;
	extern Channel* channels;
	extern CInput* input;
	extern IDirect3DDevice9* d3_device;
	extern IMDLCache* mdl_cache;
	extern IPrediction* prediction;
	extern IClientMode* client_mode;
	extern CGlobalVarsBase* global_vars;
	extern IMoveHelper* move_helper;
	extern IBaseClientDLL* base_client;
	extern IClientEntityList* entity_list;
	extern CClientState* client_state;
	extern CGameMovement* game_movement;
	extern IVEngineClient* engine_client;
	extern IVModelInfo* mdl_info;
	extern IVModelRender* mdl_render;
	extern IVRenderView* render_view;
	extern IEngineTrace* engine_trace;
	extern IVDebugOverlay* debug_overlay;
	extern IGameEventManager2* game_events;
	extern IViewRender* view_render;
	extern IMaterialSystem* mat_system;
	extern ICvar* cvar;
	extern IPanel* vgui_panel;
	extern ISurface* surface;
	extern IPhysicsSurfaceProps* physics_surface;
	extern CLocalPlayer local_player;
	extern IEngineSound* engine_sound;
	extern IInputSystem* input_system;
	extern CSPlayerResource** player_resource;
	extern CHud* hud_system;
	extern ILocalize* localize;
	extern IMemAlloc* mem_alloc;
	extern IFileSystem* file_system;
	extern IViewRenderBeams* view_render_beams;
	extern CHudChat* hud_chat;
	extern CFireBullets* fire_bullets;
	extern CSGameRulesProxy* game_rules_proxy;
	extern ServerClassDLL* server_class;

	extern ISteamUser* steam_user;
	extern ISteamHTTP* steam_http;
	extern ISteamClient* steam_client;
	extern ISteamFriends* steam_friends;
	extern ISteamGameCoordinator* game_coordinator;
	extern ISteamUtils* steam_utils;
	extern ISteamUserStats* steam_user_stats;

	void initialize();
	void get_class_ids();
}

#include "misc/EHandle.hpp"
