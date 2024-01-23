#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../minhook/minhook.h"
#include "../valve_sdk/interfaces/IStudioRender.h"

#include <d3dx9.h>
#include <type_traits>

#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	bool init();
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
		using fn = bool(__thiscall*)(void*, float, CUserCmd*);
		static bool __fastcall hooked(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd);

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

	struct override_view
	{
		static const int index = 18;
		using fn = void(__thiscall*)(IClientMode*, CViewSetup*);
		static void __stdcall hooked(CViewSetup*);

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

	struct frame_stage_notify
	{
		static const int index = 37;
		using fn = void(__thiscall*)(IBaseClientDLL*, EClientFrameStage);
		static void __stdcall hooked(EClientFrameStage stage);

		inline static fn original;
		inline static void* setup;
	};

	struct sequence
	{
		static recv_prop_hook* hook;
		using fn = void(__thiscall*)(const CRecvProxyData* data, void* entity, void* output);
		static void hooked(const CRecvProxyData* data, void* entity, void* output);
	};

	struct get_color_modulation
	{
		using fn = void(__thiscall*)(void*, float*, float*, float*);
		static void __fastcall hooked(IMaterial* ecx, void* edx, float* r, float* g, float* b);

		inline static fn original;
		inline static void* setup;
	};

	struct is_using_static_prop_debug_modes
	{
		using fn = bool(__thiscall*)();
		static bool __stdcall hooked();

		inline static fn original;
		inline static void* setup;
	};
};