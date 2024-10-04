#include "hooks/hooks.h"
#include "sdk/classes/CCSPlayerPawn.h"
#include <filesystem>
#include <fstream>
#include <shlobj_core.h>

DWORD __stdcall on_attach(void* base)
{
    Sleep(5000);

    g::console = new Console();

    sdk::init_modules();
    sdk::init_interfaces();

    netvars::init();
    hooks::init();

    //MJ: TODO: Implement getting localplayer by sig, probably use dwPlayerController, since every player is controller
    //Also implement team & entity == localplayer check in esp

    while (!globals::can_unhook)
        Sleep(50);

    hooks::detach();
    g::console->detach();

    delete g::console;
    
    FreeLibraryAndExitThread(static_cast<HMODULE>(base), EXIT_SUCCESS);

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, void* reserved)
{
    if (reason != DLL_PROCESS_ATTACH)
        return false;
    
    DisableThreadLibraryCalls(module);

    auto thread = LI_FN(CreateThread)(nullptr, 0, on_attach, module, 0, nullptr);
    if (thread)
        LI_FN(CloseHandle)(thread);

    return TRUE;
}

