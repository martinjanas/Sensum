#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../render/menu/main_window.h"
#include "../../features/features.h"

namespace hooks
{
	std::once_flag init_flag;
	void handle(IDXGISwapChain* swap_chain)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		swap_chain->GetDesc(&desc);

		globals::hwnd = desc.OutputWindow;
		globals::width = desc.BufferDesc.Width;
		globals::height = desc.BufferDesc.Height;

		std::call_once(init_flag, globals::setup_imgui_and_dx11, swap_chain);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		{
			globals::draw_list = ImGui::GetBackgroundDrawList();

			if (main_window::is_open)
				main_window::Draw();
			
			features::esp::render();
		}
		ImGui::Render();

		globals::context->OMSetRenderTargets(0, nullptr, nullptr);
		globals::context->OMSetRenderTargets(1, &globals::render_target_view, nullptr);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	long __stdcall present::hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags)
	{
		//Todo: cleanup imgui and the various dx11 pointers on exit/unhook, etc...
		handle(swap_chain);

		return original_fn(swap_chain, sync_interval, flags);
	}
}