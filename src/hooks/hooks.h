#pragma once
#include "../sdk/sdk.h"
#include "../sdk/helpers/globals.h"
#include "../sdk/classes/CHandle.h"
#include "../sdk/classes/CEntityInstance.h"
#include "../sdk/helpers/console.h"
#include "../thirdparty/ImGui/imgui.h"
#include "../thirdparty/ImGui/imgui_internal.h"
#include "../thirdparty/ImGui/backends/imgui_impl_dx11.h"
#include "../thirdparty/ImGui/backends/imgui_impl_win32.h"

#include "../sdk/hooking/safetyhook/safetyhook.hpp"
#include "../sdk/hooking/shadow_vmt/ShadowVMT.h"

#include "../sdk/interfaces/IVEngineClient.h"

namespace hooks
{
	inline static ShadowVMT entity_system;
	inline static ShadowVMT csgo_input;
	inline static ShadowVMT client;
	inline static ShadowVMT swap_chain;
	inline static ShadowVMT client_mode;
	inline static ShadowVMT dxgi;

	bool init();
	bool detach();

	namespace wndproc
	{
		inline WNDPROC original;
		LRESULT __stdcall hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	/*struct createmove_csgoinput
	{
		static const int index = 21; //rcs not working correctly with this one, why?
		using fn = bool(__fastcall*)(void*, int, CUserCmd*, float a4);
		static bool __fastcall hooked(void* rcx, int slot, CUserCmd* cmd, float a4);

		inline static fn original_fn;
	};*/

	struct createmove_csgoinput
	{
		static const int index = 5;
		using fn = void(__fastcall*)(void*, int, bool);
		static void __fastcall hooked(void* rcx, int slot, bool active);

		inline static fn original_fn;
	};

	struct level_init
	{
		static const int index = 23;
		using fn = int64_t * (__fastcall*)(void*, const char*);
		static int64_t* _fastcall hooked(void* rcx, const char* map);

		inline static fn original_fn;
	};

	struct level_shutdown
	{
		static const int index = 24;
		using fn = int64_t * (__fastcall*)(void*);
		static int64_t* _fastcall hooked(void* rcx);

		inline static fn original_fn;
	};

	struct on_add_entity
	{
		static const int index = 15;
		using fn = CEntityInstance*(__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle handle);

		inline static fn original_fn;
	};

	struct on_remove_entity
	{
		static const int index = 16;
		using fn = CEntityInstance * (__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle  handle);

		inline static fn original_fn;
	};

	namespace directx
	{
		struct present
		{
			static const int index = 8;
			using fn = long(__stdcall*)(void*, uint32_t, uint32_t);
			static long __stdcall hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags);

			inline static fn original_fn;
		};

		struct resize_buffers
		{
			static const int index = 13;
			using fn = long(__stdcall*)(void* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags);
			static long __stdcall hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags);

			inline static fn original_fn;
		};

		struct create_swapchain
		{
			static const int index = 10;
			using fn = long(__stdcall*)(IDXGIFactory* factory, IUnknown* device, DXGI_SWAP_CHAIN_DESC* swap_desc, IDXGISwapChain** swap_chain);
			static long __stdcall hooked(IDXGIFactory* factory, IUnknown* device, DXGI_SWAP_CHAIN_DESC* swap_desc, IDXGISwapChain** swap_chain);

			inline static fn original_fn;
		};
	}

	struct frame_stage_notify
	{
		static const int index = 36;
		using fn = void(__fastcall*)(void*, EClientFrameStage);
		static void __fastcall hooked(void* rcx, EClientFrameStage stage);

		inline static fn original_fn;
	};

	struct get_fov
	{
		using fn = float(__fastcall*)(void*);
		static float __fastcall hooked(void* camera);

		inline static SafetyHookInline safetyhook;
	};
	
	struct get_matrices_for_view
	{
		using fn = void(__fastcall*)(void*, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);
		static void __fastcall hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);

		inline static SafetyHookInline safetyhook;
	};
}
