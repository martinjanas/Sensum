#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <thread>

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

char buf[256];

void wait_for_modules()
{
	auto modules = std::vector<std::string>
	{
		xorstr_("engine.dll"),
		xorstr_("shaderapidx9.dll"),
		xorstr_("serverbrowser.dll"),
		xorstr_("materialsystem.dll"),
		xorstr_("client.dll"),
	};

	for (auto& module : modules)
		while (!utils::get_module(module))
			   Sleep(10);
}

void setup_hotkeys(LPVOID base)
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

DWORD __stdcall on_attach(LPVOID base)
{
	wait_for_modules();

#ifdef _DEBUG
	console::attach();
#endif
	g::initialize();
	input_system::initialize();
	render::initialize();
	hooks::init();
	//netvar_manager::get(); //Uncomment this to dump netvars.
	skins::load_statrack();
	skins::initialize_kits();
	skins::load();
	globals::load();

	config::cache("configs");

	static const auto name = g::steam_friends->GetPersonaName();
	sprintf_s(buf, "Injected, hello %s!", name);


	#ifdef _DEBUG
	static const HWND hwnd = reinterpret_cast<HWND>(g::input_system->get_window());
	if (hwnd != NULL)
		SetWindowTextA(hwnd, "Sensum | Debug Mode - Expect problems!");

	g::cvar->ConsoleColorPrintf(Color::Red, "\n\n\n\nSensum detected that it is running in DEBUG MODE, please recompile the cheat in RELEASE MODE to minimize lags and other problems!\n");
	g::cvar->ConsoleColorPrintf(Color::Red, "Sensum detected that it is running in DEBUG MODE, please recompile the cheat in RELEASE MODE to minimize lags and other problems!\n");
	g::cvar->ConsoleColorPrintf(Color::Red, "Sensum detected that it is running in DEBUG MODE, please recompile the cheat in RELEASE MODE to minimize lags and other problems!\n\n");
	#endif

	notifies::push(buf);

	/*g::cvar->ConsoleColorPrintf(Color::Green, "This is ConsoleColorPrintf\n\n"); //TODO: Do console log system.
	g::cvar->ConsolePrintf("This is ConsolePrintf\n\n");
	g::cvar->ConsoleDPrintf("This is ConsolDPrintf\n\n");*/

	setup_hotkeys(base);

	return TRUE;
}

void on_detach()
{
#ifdef _DEBUG
	console::detach();

	static const HWND hwnd = reinterpret_cast<HWND>(g::input_system->get_window());
	if (hwnd != NULL)
		SetWindowTextA(hwnd, "Counter-Strike: Global Offensive");
#endif
	render::destroy();
	hooks::destroy();
	input_system::destroy();
}

BOOL __stdcall DllMain(_In_ HINSTANCE instance, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (instance)
			LI_FN(DisableThreadLibraryCalls)(instance);

		LI_FN(CreateThread)(nullptr, 0, on_attach, instance, 0, nullptr);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
		on_detach();

	return TRUE;
}