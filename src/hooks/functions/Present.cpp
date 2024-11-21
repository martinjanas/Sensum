#include <d3d11.h>
#include <dxgi.h>
#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../render/menu.h"
#include "../../features/features.h"

namespace hooks
{
	ID3D11Device* g_pDevice = nullptr;
	ID3D11DeviceContext* g_pContext = nullptr;
	ID3D11RenderTargetView* g_pRenderTargetView;
	DXGI_SWAP_CHAIN_DESC swap_desc;
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;

	bool init_imgui_done = false;

	//bug: imgui not rendering if changing from windowed to fullscreen or when changing resolution

	long __stdcall directx::create_swapchain::hooked(IDXGIFactory* factory, IUnknown* device, DXGI_SWAP_CHAIN_DESC* swap_desc, IDXGISwapChain** swap_chain)
	{
		return original_fn(factory, device, swap_desc, swap_chain);
	}

	long __stdcall directx::resize_buffers::hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags)
	{
		return original_fn(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
	}

	long __stdcall directx::present::hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags)
	{
		if (!g_pDevice || !g_pContext || !g_pRenderTargetView)
		{
			swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pDevice);
			g_pDevice->GetImmediateContext(&g_pContext);

			swap_chain->GetDesc(&swap_desc);
			globals::hwnd = swap_desc.OutputWindow;
			globals::width = swap_desc.BufferDesc.Width;
			globals::height = swap_desc.BufferDesc.Height;

			ID3D11Texture2D* pBackBuffer;
			swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			pBackBuffer->Release();
		}

		if (!init_imgui_done && (g_pDevice && g_pContext))
		{
			hooks::wndproc::original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(globals::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc::hooked)));

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(globals::hwnd);
			ImGui_ImplDX11_Init(g_pDevice, g_pContext);

			render::init_fonts();
			render::init_style();

			init_imgui_done = true;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		{
			globals::draw_list = ImGui::GetBackgroundDrawList();

			menu::modulate_window_alpha();
			menu::draw();

			features::esp::render();
		}
		ImGui::EndFrame();
		ImGui::Render();

		g_pContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return original_fn(swap_chain, sync_interval, flags);
	}
}