#include <d3d11.h>
#include <dxgi.h>
#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../render/menu.h"
#include "../../features/features.h"
#include "../../sdk/helpers/IconFetcher.h"
#include "../../thirdparty/ImGui/backends/imgui_impl_dx11.h"
#include "../../thirdparty/ImGui/backends/imgui_impl_win32.h"
#include <algorithm>
#include "../../sdk/helpers/Timer.h"

namespace hooks
{
	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

	std::once_flag fetch_icon_flag;
	bool imgui_initialized = false;
	bool device_reset_required = false;
	
	long __stdcall directx::create_swapchain::hooked(IDXGIFactory* factory, IUnknown* device, DXGI_SWAP_CHAIN_DESC* swap_desc, IDXGISwapChain** swap_chain)
	{
		HWND hwnd = FindWindow(nullptr, L"Counter-Strike 2");
		hooks::CreateDeviceD3D11(hwnd, hooks::g_pRealDevice, hooks::g_pSwapChain);
		
		auto vtable = *reinterpret_cast<void***>(hooks::g_pSwapChain);
		auto addr = vtable[8];
		directx::present::safetyhook.reset();
		directx::present::safetyhook = safetyhook::create_inline((void*)addr, reinterpret_cast<void*>(directx::present::hooked));

		if (g_pRenderTargetView)
		{
			g_pRenderTargetView->Release();
			g_pRenderTargetView = nullptr;
		}
		
		device_reset_required = true;
		
		return original_fn(factory, device, swap_desc, swap_chain);
	}

	long __stdcall directx::resize_buffers::hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags)
	{
		return original_fn(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
	}
	
	long __stdcall directx::present::hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags)
	{
		ID3D11Device* device;
		swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
		
		ID3D11DeviceContext* device_context;
		device->GetImmediateContext(&device_context);

		DXGI_SWAP_CHAIN_DESC swap_desc{};
		swap_chain->GetDesc(&swap_desc);
		globals::hwnd = swap_desc.OutputWindow;
		globals::width = swap_desc.BufferDesc.Width;
		globals::height = swap_desc.BufferDesc.Height;

		if (!g_pRenderTargetView) 
		{
			ID3D11Texture2D* back_buffer;
			if (SUCCEEDED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)))
			{
				D3D11_RENDER_TARGET_VIEW_DESC rtv_desc{};
				rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				
				device->CreateRenderTargetView(back_buffer, &rtv_desc, &g_pRenderTargetView);

				back_buffer->Release();
			}
		}
		
		if (!imgui_initialized)
		{
			hooks::wndproc::original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(globals::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc::hooked)));
			
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(globals::hwnd);
			ImGui_ImplDX11_Init(device, device_context);

			render::init_fonts();
			render::init_style();
			
			imgui_initialized = true;;
		}

		if (device_reset_required)
		{
			ImGui_ImplDX11_InvalidateDeviceObjects();
			ImGui_ImplDX11_CreateDeviceObjects();
			
			device_reset_required = false;
		}

		{
			static bool done = false;
			if (!done && device)
			{
				icon_fetcher::fetch_icon_data(device); //is causing crashes (in fullscreen?)
				
				done = true;
			}
		}
		
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		
		ImGui::NewFrame();
		{
			globals::draw_list = ImGui::GetBackgroundDrawList();
			
			menu::modulate_window_alpha();
			menu::draw();
			
			features::esp::render();
			features::esp::render_entities();
		}
		ImGui::EndFrame();
		ImGui::Render();
		
		device_context->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
		return safetyhook.stdcall<long>(swap_chain, sync_interval, flags);
	}
}