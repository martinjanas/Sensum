#include "hooks/hooks.h"
#include "sdk/classes/CCSPlayerPawn.h"
#include <filesystem>
#include <fstream>
#include <shlobj_core.h>

DWORD __stdcall on_attach(void* base)
{
    g_Console = std::make_unique<Console>();
    
#if defined(_DEBUG) || defined(RELEASE_CONSOLE)
    g_Console->attach();
#endif

    sdk::init_modules();
    sdk::init_interfaces();

    if (!modules::nav_system.base)
        Sleep(50);
    
    netvars::init();
    hooks::init();
    
    while (!globals::can_unhook)
        Sleep(50);

    hooks::detach();

#if defined(_DEBUG) || defined(RELEASE_CONSOLE)
    g_Console->detach();
#endif

    FreeLibraryAndExitThread(static_cast<HMODULE>(base), EXIT_SUCCESS);

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, void* reserved)
{
    
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(module);
        
        auto thread = LI_FN(CreateThread)(nullptr, 0, on_attach, module, 0, nullptr);
        if (thread)
            LI_FN(CloseHandle)(thread);
    }
      
    return TRUE;
}

