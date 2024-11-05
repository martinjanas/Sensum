#include "../hooks.h"
#include "../../render/menu/main_window.h"
#include "../../render/menu.h"
#include "../../thirdparty/ImGui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks
{
    LRESULT __stdcall hooks::wndproc::hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_CLOSE)
        {
            g_Console->save_log();
            Sleep(500);
        }

        if (msg == WM_KEYDOWN && wparam == VK_INSERT)
            main_window::is_open = !main_window::is_open;

        if (msg == WM_KEYDOWN && wparam == VK_DOWN)
            menu::is_open = !menu::is_open;

        if (msg == WM_KEYDOWN && ((wparam == VK_END || wparam == VK_DELETE) && GetKeyState(VK_LCONTROL) & 0x8000))
            globals::can_unhook = true;

        if (main_window::is_open || menu::is_open)
        {
            if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
                return true;

            if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN)
                return 0;
        }

        return CallWindowProcA(original, hwnd, msg, wparam, lparam);
    }
}