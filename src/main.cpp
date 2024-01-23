#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <thread>
#include <functional>
#include <fstream>

#include "settings/config.h"
#include "settings/globals.h"
#include "settings/options.hpp"

#include "helpers/input.h"
#include "helpers/utils.h"
#include "helpers/console.h"
#include "helpers/notifies.h"

#include "hooks/hooks.h"
#include "render/render.h"
#include "valve_sdk/sdk.hpp"
#include "features/features.h"
#include "helpers/imdraw.h"
#include "valve_sdk/netvars.hpp"

#include "valve_sdk/interfaces/ICvar.h"

#include "valve_sdk/kit_parser.hpp"

void setup_hotkeys(void* base)
{
	input_system::register_hotkey(VK_INSERT, []()
	{
			render::menu::toggle();

			render::switch_hwnd();
	});
	
	bool is_active = true;
	input_system::register_hotkey(VK_DELETE, [&is_active]()
	{
		hooks::destroy();
		if (render::menu::is_visible())
		{
			render::menu::toggle();
			render::switch_hwnd();
		}
		is_active = false;
	});

	while (is_active)
		Sleep(500);

	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}

DWORD __stdcall on_attach(void* base)
{
	while (!utils::get_module("serverbrowser.dll"))
		Sleep(10);

//#ifdef _DEBUG
	console::attach();
//#endif

	patterns::initialize();
	g::initialize();
	input_system::initialize();
	render::initialize();

	if (MH_Initialize() != MH_OK)
	{
		MessageBoxA(NULL, xorstr_("Unable to initialize Minhook."), MB_OK, MB_ICONERROR);
		Sleep(2000);

		return 0;
	}

	if (!hooks::init())
	{
		MessageBoxA(NULL, xorstr_("One or more hooks failed to initialize."), MB_OK, MB_ICONERROR); 
		Sleep(2000);

		return 0;
	}

	skins::load();
	globals::load();
	game_data::initialize_kits();
	
	config::cache("configs");

	notifies::push("Hello!");

	setup_hotkeys(base);

	return TRUE;
}

void on_detach()
{
//#ifdef _DEBUG
	console::detach();
//#endif

	render::destroy();
	hooks::destroy();
	input_system::destroy();
}

BOOL __stdcall DllMain(HINSTANCE instance, DWORD fdwReason, void* lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (instance)
			LI_FN(DisableThreadLibraryCalls)(instance);

		auto handle = LI_FN(CreateThread)(nullptr, 0, on_attach, instance, 0, nullptr);

		LI_FN(CloseHandle)(handle);
		
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
		on_detach();

	return TRUE;
}
