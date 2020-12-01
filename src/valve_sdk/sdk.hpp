#pragma once

#include "patterns.h"
#include "../security/importer.h"

#define NOMINMAX
#include <Windows.h>

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
#include "interfaces/CGlowObjectManager.h"
#include "interfaces/IStudioRender.h"
#include "interfaces/IEngineSound.hpp"
#include "interfaces/IMaterialVar.h"
#include "interfaces/IDemoPlayer.h"

#include "misc/Convar.hpp"
#include "interfaces/ICvar.h"
#include "misc/CUserCmd.hpp"

#include "netvars.hpp"
#include "steam.h"

struct IDirect3DDevice9;

class CLocalPlayer
{
	friend bool operator==(const CLocalPlayer& lhs, void* rhs);
public:
	CLocalPlayer() : m_local(nullptr) {}

	operator bool() const { return *m_local != nullptr; }
	operator c_base_player* () const { return *m_local; }

	c_base_player* operator->() { return *m_local; }

private:
	c_base_player** m_local;
};

class c_cs_player_resource;
class CRender;
class IVEngineVGui;

class IVDebugOverlay
{
public:
	/*bool ScreenPosition(const Vector& vIn, Vector& vOut)
	{
		typedef bool(__thiscall * oScreenPosition)(void*, const Vector&, Vector&);
		return CallVFunction<oScreenPosition>(this, 13)(this, vIn, vOut);
	}*/

	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const Vector& origin, float duration, const char* format, ...) FMTFUNCTION(4, 5) = 0;
	virtual void AddTextOverlay(const Vector& origin, int line_offset, float duration, const char* format, ...) FMTFUNCTION(5, 6) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const Vector& origin) = 0;
	virtual void AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int  ScreenPosition(const Vector& point, Vector& screen) = 0;
	virtual int  ScreenPosition(float flXPos, float flYPos, Vector& screen) = 0;
	virtual void* GetFirst() = 0;
	virtual void* GetNext(void* current) = 0;
	virtual void ClearDeadOverlays() = 0;
	virtual void ClearAllOverlays() = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;
	virtual void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, int a, float thickness, float duration) = 0;
	virtual void PurgeTextOverlays() = 0;
	virtual void AddCapsuleOverlay(const Vector& vStart, const Vector& vEnd, const float& flRadius, int r, int g, int b, int a, float flDuration) = 0;
};

class IRecipientFilter;
class IInputSystem
{
public:
	void EnableInput(bool enable)
	{
		return CallVFunction<void(__thiscall*)(void*, bool)>(this, 11)(this, enable);
	}

	void*& get_window()
	{
		static uint32_t offset = 0;
		if (!offset)
			offset = *reinterpret_cast<uint32_t*>((*reinterpret_cast<char***>(this))[10] + 5);

		return *reinterpret_cast<void**>(reinterpret_cast<char*>(this) + offset);
	}
};

class INetMessage
{
public:
	virtual	~INetMessage() {};
	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;	// set to true if it's a reliable message
	virtual bool Process(void) = 0; // calles the recently set handler to process this message
	virtual	bool ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual	bool WriteToBuffer(bf_write& buffer) = 0;	// returns true if writing was OK
	virtual bool IsReliable(void) const = 0;  // true, if message needs reliable handling
	virtual int GetType(void) const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;	// returns net message group of this message
};

class CHud
{
public:
	template <typename T>
	T* FindHudElement(const char* name)
	{
		static auto ptr = utils::pattern_scan(FIND_HUD);
		static auto fn = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(ptr);

		return (T*)fn(this, name);
	}
};

class CCSGO_HudChat : public CHud
{
private:
	char pad_0x0000[0x4C]; //0x0000
public:
	int m_timesOpened; //0x004C is 0 when m_isOpen = false
private:
	char pad_0x0050[0x8]; //0x0050
public:
	bool m_isOpen; //0x0058
private:
	char pad_0x0059[0x427]; //0x0059
};

typedef struct netpacket_s
{
	void* from;		// sender IP
	int				source;		// received source 
	double			received;	// received time
	unsigned char*  data;		// pointer to raw packet data
	bf_read*		msg;	// easy bitbuf data access
	int				size;		// size in bytes
	int				wiresize;   // size in bytes before decompression
	bool			stream;		// was send as stream
	struct netpacket_s* pNext;	// for internal use, should be NULL in public
} netpacket_t;

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
		//=============================================================================
		// HPE_BEGIN:
		// [tj]Added a filter for achievement announce
		//=============================================================================

		CHAT_FILTER_ACHIEVEMENT = 0x000020,

		//=============================================================================
		// HPE_END
		//=============================================================================
	};

	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		char msg[1024];

		va_list args;
		va_start(args, fmt);
		vsprintf(msg, fmt, args);
		CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, iPlayerIndex, iFilter, fmt);
		va_end(args);
	}
};

class C_TEFireBullets
{
public:
	char pad_0000[8]; //0x0000
	int32_t m_iPlayer1; //0x000C
	int32_t m_iPlayer; //0x000C
	int32_t m_iItemDefinitionIndex; //0x0010
	Vector m_vecOrigin; //0x0014
	QAngle m_absAngles; //0x0020
};

class c_cs_game_rules_proxy;

namespace g
{
	extern CInput* input;
	extern IDirect3DDevice9* d3_device;
	extern IMDLCache* mdl_cache;
	extern IPrediction* prediction;
	extern IClientMode* client_mode;
	extern CGlobalVarsBase* global_vars;
	extern IMoveHelper* move_helper;
	extern IViewRender* view_render;
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
	extern IMaterialSystem* mat_system;
	extern ICvar* cvar;
	extern IPanel* vgui_panel;
	extern ISurface* surface;
	extern IPhysicsSurfaceProps* physics_surface;
	extern CLocalPlayer local_player;
	extern IEngineSound* engine_sound;
	extern IInputSystem* input_system;
	extern IVEngineVGui* engine_vgui;
	extern CRender* render;
	extern c_cs_player_resource** player_resource;
	extern CHud* hud_system;
	extern ILocalize* localize;
	extern IMemAlloc* mem_alloc;
	extern IWeaponSystem* weapon_system;
	extern IFileSystem* file_system;
	extern IViewRenderBeams* view_render_beams;
	extern CHudChat* hud_chat;
	extern C_TEFireBullets* fire_bullets;
	extern c_cs_game_rules_proxy* game_rules_proxy;
	extern glow_manager_t* glow_manager;
	extern IStudioRender* g_studiorender;
	extern IDemoPlayer* demo_player;

	extern ISteamUser* steam_user;
	extern ISteamHTTP* steam_http;
	extern ISteamClient* steam_client;
	extern ISteamFriends* steam_friends;
	extern ISteamGameCoordinator* game_coordinator;

	void initialize();
}

#pragma pack(push, 1)
struct mat_vide_mode_t
{
	int m_Width;
	int m_Height;
	int m_Format;
	int m_RefreshRate;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct matsystem_config_t
{
	mat_vide_mode_t m_VideoMode;
	float m_fMonitorGamma;
	float m_fGammaTVRangeMin;
	float m_fGammaTVRangeMax;
	float m_fGammaTVExponent;
	bool m_bGammaTVEnabled;
	bool m_bTripleBuffered;
	int m_nAASamples;
	int m_nForceAnisotropicLevel;
	int m_nSkipMipLevels;
	int m_nDxSupportLevel;
	int m_nFlags;
	bool m_bEditMode;
	char m_nProxiesTestMode;
	bool m_bCompressedTextures;
	bool m_bFilterLightmaps;
	bool m_bFilterTextures;
	bool m_bReverseDepth;
	bool m_bBufferPrimitives;
	bool m_bDrawFlat;
	bool m_bMeasureFillRate;
	bool m_bVisualizeFillRate;
	bool m_bNoTransparency;
	bool m_bSoftwareLighting;
	bool m_bAllowCheats;
	char m_nShowMipLevels;
	bool m_bShowLowResImage;
	bool m_bShowNormalMap;
	bool m_bMipMapTextures;
	char m_nFullbright;
	bool m_bFastNoBump;
	bool m_bSuppressRendering;
	bool m_bDrawGray;
	bool m_bShowSpecular;
	bool m_bShowDiffuse;
	int m_nWindowedSizeLimitWidth;
	int m_nWindowedSizeLimitHeight;
	int m_nAAQuality;
	bool m_bShadowDepthTexture;
	bool m_bMotionBlur;
	bool m_bSupportFlashlight;
	bool m_bPaintEnabled;
	char pad[0xC];
};
#pragma pack(pop)

#include "misc/EHandle.hpp"
