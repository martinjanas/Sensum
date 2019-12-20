#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <thread>

#include "config.h"
#include "globals.h"
#include "options.hpp"

#include "helpers/input.h"
#include "helpers/utils.h"
#include "helpers/console.h"
#include "helpers/notifies.h"

#include "hooks/hooks.h"
#include "render/render.h"
#include "valve_sdk/sdk.hpp"
#include "features/features.h"
#include "helpers/imdraw.h"

bool is_unhookable = true;

void wait_for_modules()
{
	auto modules = std::vector<std::string>
	{
		xorstr_("engine.dll"),
		xorstr_("shaderapidx9.dll"),
		xorstr_("serverbrowser.dll"),
		xorstr_("materialsystem.dll"),
		xorstr_("client_panorama.dll"),
	};

	for (auto& module : modules)
		while (!utils::get_module(module))
			LI_FN(Sleep).cached()(10);
}

void setup_hotkeys(LPVOID base)
{
	input_system::register_hotkey(VK_INSERT, []()
	{
		render::menu::toggle();

		render::switch_hwnd();
	});

	if (is_unhookable)
	{
		bool is_active = true;
		input_system::register_hotkey(VK_DELETE, [&is_active]()
		{
			if (render::menu::is_visible())
				//render::switch_hwnd();

				hooks::destroy();

			is_active = false;
		});

		while (is_active)
			Sleep(500);

		LI_FN(FreeLibraryAndExitThread)(static_cast<HMODULE>(base), 1);
	}
}

DWORD __stdcall on_attach(LPVOID base)
{
	wait_for_modules();

#ifdef _DEBUG
	console::attach();
#endif

	interfaces::initialize();
	input_system::initialize();
	render::initialize();
	hooks::initialize();
	skins::initialize_kits();
	skins::LoadStatrack();
	skins::load();

	config::cache("settings");

	globals::load();

	// remove blur in scope
	//auto ptr = *reinterpret_cast<bool**>(utils::pattern_scan(OVERRIDE_BLUR) + 2);
	//*ptr = true;

#ifdef _DEBUG
	const auto version = interfaces::engine_client->GetProductVersionString();
	console::print("CS:GO Version - %s", version);
#endif // _DEBUG
	//if (strcmp(version, "1.36.8.3") != 0)
		//std::abort();

	notifies::push(strings::init.c_str(), notify_state_s::debug_state);

	setup_hotkeys(base);

	return TRUE;
}

void on_detach()
{
#ifdef _DEBUG
	console::detach();
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

		if (is_unhookable = (strstr(GetCommandLineA(), "-insecure") || !utils::get_module(xorstr_("serverbrowser.dll"))))
			LI_FN(CreateThread)(nullptr, 0, on_attach, instance, 0, nullptr);
		else
			on_attach(instance);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
		on_detach();

	return TRUE;
}
