#include "Console.h"

namespace console
{
    void attach()
    {
        AllocConsole();
        AttachConsole(ATTACH_PARENT_PROCESS);

        FILE* file;
        freopen_s(&file, "CONIN$", "r", stdin);
        freopen_s(&file, "CONOUT$", "w", stdout);
        freopen_s(&file, "CONOUT$", "w", stderr);
    }

    void detach()
    {
        HWND hwnd = GetConsoleWindow();

        FreeConsole();
        PostMessageA(hwnd, WM_QUIT, 0, 0);
    }
}