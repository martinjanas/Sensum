#include <memory>
#include "sdk/sdk.h"
#include "hooks/hooks.h"
#include "sdk/helpers/netvars.h"
#include "sdk/helpers/console.h"

//Notice: This project is WORK IN PROGRESS!
DWORD __stdcall on_attach(void* base)
{
   /* while (!modules::nav_system.base)
       Sleep(50);*/
    
    Sleep(5000);

    console::attach();

    sdk::init_modules();
    sdk::init_interfaces();

    netvars::init();
    hooks::init();

    //MJ: TODO: Implement getting localplayer by sig, probably use dwPlayerController, since every player is controller
    //Also implement team & entity == localplayer check in esp

    if (globals::can_unhook)
    {
        hooks::detach();
        console::detach();

        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(base), EXIT_SUCCESS);
    }

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

