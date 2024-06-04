#include "../hooks.h"
#include "../../render/menu/main_window.h"
#include "../../thirdparty/ImGui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks
{
    LRESULT __stdcall hooks::wndproc::hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_KEYDOWN && wparam == VK_INSERT)
            main_window::is_open = !main_window::is_open;

        // Set flag to unhook
        if (msg == WM_KEYDOWN && (wparam == VK_END || wparam == VK_DELETE))
            globals::can_unhook = true;

        //main_window::is_open && 
        // Process ImGui messages if the main window is open
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
            return true;

        return CallWindowProcA(original, hwnd, msg, wparam, lparam);
    }
}