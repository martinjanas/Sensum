#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

bool hooks::init()
{
	entity_system = ShadowVMT(g::entity_system);
	csgo_input = ShadowVMT(g::csgo_input);
	client = ShadowVMT(g::client);
	swap_chain = ShadowVMT(g::render_system->swap_chain);
	client_mode = ShadowVMT(g::client_mode_csnormal);
	
	ID3D11Device* d3d11_device = nullptr;
	g::render_system->swap_chain->GetDevice(IID_PPV_ARGS(&d3d11_device));

	IDXGIDevice* dxgi_device = nullptr;
	d3d11_device->QueryInterface(IID_PPV_ARGS(&dxgi_device));

	IDXGIAdapter* dxgi_adapter = nullptr;
	dxgi_device->GetAdapter(&dxgi_adapter);

	IDXGIFactory* dxgi_factory = nullptr;
	dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));

	dxgi = ShadowVMT(dxgi_factory);

	entity_system.Apply(on_add_entity::index, reinterpret_cast<uintptr_t*>(&on_add_entity::hooked), reinterpret_cast<void**>(&on_add_entity::original_fn));
	entity_system.Apply(on_remove_entity::index, reinterpret_cast<uintptr_t*>(&on_remove_entity::hooked), reinterpret_cast<void**>(&on_remove_entity::original_fn));

	client.Apply(frame_stage_notify::index, reinterpret_cast<uintptr_t*>(&frame_stage_notify::hooked), reinterpret_cast<void**>(&frame_stage_notify::original_fn));

	swap_chain.Apply(directx::present::index, reinterpret_cast<uintptr_t*>(&directx::present::hooked), reinterpret_cast<void**>(&directx::present::original_fn));
	//swap_chain.Apply(directx::resize_buffers::index, reinterpret_cast<uintptr_t*>(&directx::resize_buffers::hooked), reinterpret_cast<void**>(&directx::resize_buffers::original_fn));

	//dxgi.Apply(directx::create_swapchain::index, reinterpret_cast<uintptr_t*>(&directx::create_swapchain::hooked), reinterpret_cast<void**>(&directx::create_swapchain::original_fn));

	csgo_input.Apply(createmove_csgoinput::index, reinterpret_cast<uintptr_t*>(&createmove_csgoinput::hooked), reinterpret_cast<void**>(&createmove_csgoinput::original_fn));

	client_mode.Apply(level_init::index, reinterpret_cast<uintptr_t*>(&level_init::hooked), reinterpret_cast<void**>(&level_init::original_fn));
	client_mode.Apply(level_shutdown::index, reinterpret_cast<uintptr_t*>(&level_shutdown::hooked), reinterpret_cast<void**>(&level_shutdown::original_fn));

	//Todo: get_fov: ClientModeCSNormal at index 27?
	get_fov::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::GetFov"), __FUNCTION__).as(), reinterpret_cast<void*>(get_fov::hooked));
	get_matrices_for_view::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::GetMatricesForView"), __FUNCTION__).as(), reinterpret_cast<void*>(get_matrices_for_view::hooked));

	return true;
}

bool hooks::detach()
{
	SetWindowLongPtrA(globals::hwnd, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

	entity_system.Remove(on_add_entity::index);
	entity_system.Remove(on_remove_entity::index);
	client.Remove(frame_stage_notify::index);
	swap_chain.Remove(directx::present::index);
	swap_chain.Remove(directx::resize_buffers::index);
	dxgi.Remove(directx::create_swapchain::index);
	csgo_input.Remove(createmove_csgoinput::index);

	entity_system.RestoreTable();
	csgo_input.RestoreTable();
	client.RestoreTable();
	swap_chain.RestoreTable();
	client_mode.RestoreTable();
	dxgi.RestoreTable();

	return true;
}

int64_t* __fastcall hooks::level_init::hooked(void* rcx, const char* map)
{
	g::global_vars = *modules::client.get_sig_addr(FNV("g::global_vars")).as<CGlobalVarsBase**>();

	return original_fn(rcx, map);
}

int64_t* __fastcall hooks::level_shutdown::hooked(void* rcx)
{
	return original_fn(rcx);
}

