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

		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		globals::device->CreateRenderTargetView(globals::back_buffer, &rtv_desc, &globals::render_target_view);

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

	bool screen_transform(const Vector& in, Vector& out)
	{
		auto result = viewmatrix;
		if (!result)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		const auto& world_matrix = result;

		const auto w = world_matrix->m[3][0] * in.x + world_matrix->m[3][1] * in.y + world_matrix->m[3][2] * in.z + world_matrix->m[3][3];
		if (w < 0.001f)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x = world_matrix->m[0][0] * in.x + world_matrix->m[0][1] * in.y + world_matrix->m[0][2] * in.z + world_matrix->m[0][3];
		out.y = world_matrix->m[1][0] * in.x + world_matrix->m[1][1] * in.y + world_matrix->m[1][2] * in.z + world_matrix->m[1][3];
		out.z = 0.0f;

		out.x /= w;
		out.y /= w;

		return true;
	}

	bool world2screen(const Vector& in, Vector& out)
	{
		if (!viewmatrix)
			return false;

		if (!screen_transform(in, out))
			return false;

		int w, h;
		g::engine_client->GetScreenSize(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
}
