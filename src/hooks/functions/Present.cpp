#include <d3d11.h>
#include <dxgi.h>
#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../render/menu/main_window.h"
#include "../../features/features.h"

namespace hooks
{
	ID3D11Device* device{};
	ID3D11DeviceContext* context{};
	ID3D11Texture2D* back_buffer{};
	ID3D11RenderTargetView* rtv{};
	DXGI_SWAP_CHAIN_DESC swap_desc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc{};

	static bool init_imgui_done = false;

	bool setup_dx11_and_imgui(IDXGISwapChain* swap_chain)
	{
		if (!swap_chain)
			return false;

		if (FAILED(swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device))))
			return false;
		
		if (!device)
			return false;
		
		device->GetImmediateContext(&context);

		swap_chain->GetDesc(&swap_desc);
		globals::hwnd = swap_desc.OutputWindow;
		globals::width = swap_desc.BufferDesc.Width;
		globals::height = swap_desc.BufferDesc.Height;

		if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer))))
			return false;

		rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		if (FAILED(device->CreateRenderTargetView(back_buffer, &rtv_desc, &rtv)))
			return false;

		back_buffer->Release();

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(globals::hwnd);
		ImGui_ImplDX11_Init(device, context);
		ImGui_ImplDX11_CreateDeviceObjects();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui::GetStyle().WindowMinSize = ImVec2(500, 350); //MJ: Gonna remove this soon probably, because I will be working on menu basics

		//ImGui::GetIO().ImeWindowHandle = globals::hwnd; //Obsolete?
		ImGui::GetMainViewport()->PlatformHandleRaw = globals::hwnd;

		hooks::wndproc::original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(globals::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc::hooked)));

		//device->Release();

		init_imgui_done = true;
	}

	long __stdcall directx::resize_buffers::hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags)
	{
		/*ImGui::CreateContext();

		if (FAILED(swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device))))
			return S_FALSE;

		device->GetImmediateContext(&context);

		swap_chain->GetDesc(&swap_desc);

		globals::hwnd = swap_desc.OutputWindow;
		globals::width = swap_desc.BufferDesc.Width;
		globals::height = swap_desc.BufferDesc.Height;

		ImGui_ImplWin32_Init(globals::hwnd);
		ImGui_ImplDX11_Init(device, context);
		ImGui_ImplDX11_CreateDeviceObjects();

		ID3D11Texture2D* back_buffer{};

		if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer))))
			return S_FALSE;

		rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		if (FAILED(device->CreateRenderTargetView(back_buffer, &rtv_desc, &rtv)))
			return S_FALSE;
				
		back_buffer->Release();

		context->OMSetRenderTargets(1, &rtv, nullptr);

		context->Release();
		device->Release();*/

		return original_fn(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
	}

	void handle(IDXGISwapChain* swap_chain)
	{
		if (!swap_chain)
			return;

		swap_chain->GetDesc(&swap_desc);

		globals::hwnd = swap_desc.OutputWindow;
		globals::width = swap_desc.BufferDesc.Width;
		globals::height = swap_desc.BufferDesc.Height;

		if (!init_imgui_done)
			setup_dx11_and_imgui(swap_chain);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		{
			globals::draw_list = ImGui::GetBackgroundDrawList();
			
			if (main_window::is_open)
				main_window::Draw();
			
			features::esp::render();
		}
		ImGui::EndFrame();
		ImGui::Render();

		context->OMSetRenderTargets(1, &rtv, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	long __stdcall directx::present::hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags)
	{
		handle(swap_chain);

		return original_fn(swap_chain, sync_interval, flags);
	}
}