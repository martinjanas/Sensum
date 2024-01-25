#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <mutex>
#include "../../thirdparty/ImGui/imgui.h"
#include "../../sdk/math/Viewmatrix.h"
#include "../../sdk/math/Vector.h"

namespace globals
{
	extern ImDrawList* draw_list;

	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern ID3D11RenderTargetView* render_target_view;
	extern ID3D11Texture2D* back_buffer;

	extern HWND hwnd;

	extern VMatrix* viewmatrix;

	extern int width;
	extern int height;

	extern bool can_unhook;

	void setup_imgui_and_dx11(IDXGISwapChain* swap_chain);
	bool world_to_screen(const Vector& pos, Vector& out);
	bool world2screen(const Vector& in, Vector& out);
}

