#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../minhook/minhook.h"
#include "../valve_sdk/interfaces/IStudioRender.h"

#include <d3dx9.h>
#include <type_traits>

#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	void init();
	void destroy();
	
	struct end_scene
	{
		static const int index = 42;
		using fn = long(__stdcall*)(IDirect3DDevice9*);
		static long __stdcall hooked(IDirect3DDevice9*);

		inline static fn original;
		inline static void* setup;
	};

	struct create_move
	{
		static const int index = 24;
		using fn = bool(__thiscall*)(IClientMode*, float, CUserCmd*);
		static bool __stdcall hooked(float input_sample_frametime, CUserCmd* cmd);

		inline static fn original;
		inline static void* setup;
	};

	struct reset
	{
		static const int index = 16;
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		static long __stdcall hooked(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

		inline static fn original;
		inline static void* setup;
	};

	struct paint_traverse
	{
		static const int index = 41;
		using fn = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
		static void __stdcall hooked(vgui::VPANEL, bool forceRepaint, bool allowForce);

		inline static fn original;
		inline static void* setup;
	};

	struct scene_end
	{
		static const int index = 9;
		using fn = void(__thiscall*)(IVRenderView*);
		static void __fastcall hooked(IVRenderView*&);

		inline static fn original;
		inline static void* setup;
	};

	struct override_view
	{
		static const int index = 18;
		using fn = void(__thiscall*)(IClientMode*, CViewSetup*);
		static void __stdcall hooked(CViewSetup*);

		inline static fn original;
		inline static void* setup;
	};

	struct emit_sound
	{
		static const int index = 5;
		using fn = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);
		static void __stdcall hooked(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);

		inline static fn original;
		inline static void* setup;
	};

	struct draw_model_execute
	{
		static const int index = 21;
		using fn = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t*, const ModelRenderInfo_t*, matrix3x4_t*);
		static void __stdcall hooked(IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone);

		inline static fn original;
		inline static void* setup;
	};

	struct post_screen_effects
	{
		static const int index = 44;
		using fn = int(__thiscall*)(void*, int);
		static int __stdcall hooked(int value);

		inline static fn original;
		inline static void* setup;
	};

	struct is_connected
	{
		static const int index = 27;
		using fn = bool(__thiscall*)(IVEngineClient*);
		static bool __stdcall hooked();

		inline static fn original;
		inline static void* setup;
	};

	struct frame_stage_notify
	{
		static const int index = 37;
		using fn = void(__thiscall*)(IBaseClientDLL*, EClientFrameStage);
		static void __stdcall hooked(EClientFrameStage stage);

		inline static fn original;
		inline static void* setup;
	};

	struct post_data_update
	{
		static const int index = 7;
		using fn = void(__thiscall*)(IClientNetworkable* thisptr, int update_type);
		static void call();

		inline static fn original;
		inline static void* setup;
	};

	struct find_mdl
	{
		static const int index = 10;
		using fn = MDLHandle_t(__thiscall*)(void*, char* FilePath);
		static MDLHandle_t __fastcall hooked(void* ecx, void* edx, char* FilePath);

		inline static fn original;
		inline static void* setup;
	};

	struct retrieve_message
	{
		static const int index = 2;
		using fn = EGCResults(__thiscall*)(void*, uint32_t*, void*, uint32_t, uint32_t*);
		static EGCResults __stdcall hooked(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);

		inline static fn original;
		inline static void* setup;
	};

	struct send_net_message
	{
		static const int index = 40;
		using fn = bool(__thiscall*)(void* thisptr, INetMessage& msg, bool bForceReliable, bool bVoice);
		static bool __stdcall hooked(INetMessage& msg, bool bForceReliable, bool bVoice);

		inline static fn original;
		inline static void* setup;
	};

	struct send_datagram
	{
		static const int index = 46;
		using fn = int(__thiscall*)(INetChannel* netchan, bf_write* datagram);
		static int __stdcall hooked(bf_write* datagram);

		inline static fn original;
		inline static void* setup;
	};

	struct sequence
	{
		static recv_prop_hook* hook;
		using fn = void(__thiscall*)(const CRecvProxyData* data, void* entity, void* output);
		static void hooked(const CRecvProxyData* data, void* entity, void* output);
	};

	struct fire_bullets
	{
		static const int index = 7;
		using fn = void(__thiscall*)(void*, int);
		static void __stdcall hooked(int);
	
		inline static fn original;
		inline static void* setup;
	};

	struct check_file_crc_server
	{
		using fn = void(__thiscall*)(void*, void*);
		static void __fastcall hooked(void*, void*);

		inline static fn original;
		inline static void* setup;
	};

	struct loose_file_allowed
	{
		static const int index = 128;
		using fn = bool(__thiscall*)(void*, void*);
		static bool __fastcall hooked(void*, void*);

		inline static fn original;
		inline static void* setup;
	};

	struct read_packet
	{
		static const int index = 17;
		using fn = netpacket_t * (__thiscall*)(IDemoPlayer*);
		static netpacket_t* __fastcall hooked(IDemoPlayer* player);

		inline static fn original;
		inline static void* setup;
	};

	struct hud_update
	{
		static const int index = 11;
		using fn = void(__thiscall*)(IBaseClientDLL*, bool);
		static void __fastcall hooked(IBaseClientDLL* _this, void* edx, bool active);

		inline static fn original;
		inline static void* setup;
	};

	struct is_playing_demo
	{
		static const int index = 82;
		using fn = bool(__thiscall*)();
		static bool __stdcall hooked();

		inline static fn original;
		inline static void* setup;
	};

	/*struct dispatch_user_message
	{
		static const int index = 38;
		using fn = bool(__thiscall*)(void*, int type, int a3, int length, void* msg_data);
		static bool __stdcall hooked(void*&, int& type, int& a3, int& length, void*& msg_data);
	};*/
};