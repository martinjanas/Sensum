#pragma once
#include "../sdk/sdk.h"
#include "../sdk/helpers/globals.h"
#include "../sdk/classes/CHandle.h"
#include "../sdk/hooking/minhook/MinHook.h"
#include "../sdk/classes/CEntityInstance.h"
#include "../sdk/helpers/console.h"
#include "../thirdparty/ImGui/imgui.h"
#include "../thirdparty/ImGui/imgui_internal.h"
#include "../thirdparty/ImGui/backends/imgui_impl_dx11.h"
#include "../thirdparty/ImGui/backends/imgui_impl_win32.h"

namespace hooks
{
	bool init();
	bool detach();

	namespace wndproc
	{
		inline WNDPROC original;
		LRESULT __stdcall hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	struct create_move
	{
		static const int index = 14;
		using fn = void(__fastcall*)(CSGOInput*, unsigned int, void*, unsigned __int8);
		static void __fastcall hooked(CSGOInput* input, unsigned int a2, void* a3, unsigned __int8 unk);

		inline static fn original_fn;
	};

	struct on_add_entity
	{
		static const int index = 14;
		using fn = CEntityInstance*(__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle handle);

		inline static fn original_fn;
	};

	struct on_remove_entity
	{
		static const int index = 15;
		using fn = CEntityInstance * (__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle handle);

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
	}

	struct frame_stage_notify
	{
		using fn = void(__fastcall*)(void*, int);
		static void __fastcall hooked(void* a1, int stage);

		inline static fn original_fn;
	};

	struct get_fov
	{
		using fn = float(__fastcall*)(void*);
		static float __fastcall hooked(void* camera);

		inline static fn original_fn;
	};
	
	struct get_matrices_for_view
	{
		using fn = void(__fastcall*)(void*, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);
		static void __fastcall hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);

		inline static fn original_fn;
	};
}
