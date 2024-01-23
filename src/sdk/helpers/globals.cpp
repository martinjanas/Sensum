#include "globals.h"
#include "../../hooks/hooks.h"

namespace globals
{
	ImDrawList* draw_list = nullptr;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* render_target_view = nullptr;
	ID3D11Texture2D* back_buffer = nullptr;

	HWND hwnd = NULL;

	VMatrix* viewmatrix = nullptr;

	int width = -1;
	int height = -1;

	bool can_unhook = false;

	void setup_imgui_and_dx11(IDXGISwapChain* swap_chain)
	{
		swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&globals::device));
		globals::device->GetImmediateContext(&globals::context);
		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&globals::back_buffer));

		globals::device->CreateRenderTargetView(globals::back_buffer, nullptr, &globals::render_target_view);

		if (globals::back_buffer)
			globals::back_buffer->Release();

		hooks::wndproc::original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(globals::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc::hooked)));

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(globals::hwnd);
		ImGui_ImplDX11_Init(globals::device, globals::context);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui::GetStyle().WindowMinSize = ImVec2(500, 350); //MJ: Gonna remove this soon probably, because I will be working on menu basics
	}

	bool world_to_screen(const Vector& pos, Vector& out)
	{
		if (!viewmatrix)
			return false;

		out.x = viewmatrix->m[0][0] * pos.x + viewmatrix->m[0][1] * pos.y + viewmatrix->m[0][2] * pos.z + viewmatrix->m[0][3];
		out.y = viewmatrix->m[1][0] * pos.x + viewmatrix->m[1][1] * pos.y + viewmatrix->m[1][2] * pos.z + viewmatrix->m[1][3];

		float w = viewmatrix->m[3][0] * pos.x + viewmatrix->m[3][1] * pos.y + viewmatrix->m[3][2] * pos.z + viewmatrix->m[3][3];

		if (w < 0.01f)
			return false;

		float inv_w = 1.f / w;
		out.x *= inv_w;
		out.y *= inv_w;

		int width, height;
		g::engine_client->GetScreenSize(width, height);

		float x = width * .5f;
		float y = height * .5f;

		x += 0.5f * out.x * width + 0.5f;
		y -= 0.5f * out.y * height + 0.5f;

		out.x = x;
		out.y = y;

		return true;
	}
}
