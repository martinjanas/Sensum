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

	//client_mode.Apply(override_view::index, reinterpret_cast<uintptr_t*>(&override_view::hooked), reinterpret_cast<void**>(&override_view::original_fn));

	get_fov::safetyhook = safetyhook::create_inline(modules::client.scan("E8 ? ? ? ? F3 0F 11 45 ? 48 8B 5C 24 ?", "get_fov_hook").add(0x1).abs().as(), reinterpret_cast<void*>(get_fov::hooked));
	get_matrices_for_view::safetyhook = safetyhook::create_inline(modules::client.scan("40 53 48 81 EC ? ? ? ? 49 8B C1", "get_matrices_for_view_hook").as(), reinterpret_cast<void*>(get_matrices_for_view::hooked));
	clientmodeshared_createmove::safetyhook = safetyhook::create_inline(modules::client.scan("40 53 48 83 EC 20 48 8B CA 48 8B DA", "createmove_shared_hook").as(), reinterpret_cast<void*>(clientmodeshared_createmove::hooked));
	//emit_sound::safetyhook = safetyhook::create_inline(modules::client.scan("48 8B C4 4C 89 40 18 55 53 41 54 41 56 41 57 48 8D 68 A9 48 81", "emit_sound_hook").as(), reinterpret_cast<void*>(emit_sound::hooked));

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
	//csgo_input.Remove(csgoinput_createmove::index);

	entity_system.RestoreTable();
	csgo_input.RestoreTable();
	client.RestoreTable();
	swap_chain.RestoreTable();
	client_mode.RestoreTable();
	dxgi.RestoreTable();

	return true;
}

class C_PointCamera // C_BaseEntity 
{
public:
	NETVAR(float, m_FOV, "C_PointCamera", "m_FOV");
	NETVAR(float, m_flZFar, "C_PointCamera", "m_flZFar");
	NETVAR(float, m_flZNear, "C_PointCamera", "m_flZNear");

	//constexpr std::ptrdiff_t m_FOV = 0x540; // float
	//constexpr std::ptrdiff_t m_Resolution = 0x544; // float
	//constexpr std::ptrdiff_t m_bFogEnable = 0x548; // bool
	//constexpr std::ptrdiff_t m_FogColor = 0x549; // Color
	//constexpr std::ptrdiff_t m_flFogStart = 0x550; // float
	//constexpr std::ptrdiff_t m_flFogEnd = 0x554; // float
	//constexpr std::ptrdiff_t m_flFogMaxDensity = 0x558; // float
	//constexpr std::ptrdiff_t m_bActive = 0x55C; // bool
	//constexpr std::ptrdiff_t m_bUseScreenAspectRatio = 0x55D; // bool
	//constexpr std::ptrdiff_t m_flAspectRatio = 0x560; // float
	//constexpr std::ptrdiff_t m_bNoSky = 0x564; // bool
	//constexpr std::ptrdiff_t m_fBrightness = 0x568; // float
	//constexpr std::ptrdiff_t m_flZFar = 0x56C; // float
	//constexpr std::ptrdiff_t m_flZNear = 0x570; // float
	//constexpr std::ptrdiff_t m_bCanHLTVUse = 0x574; // bool
	//constexpr std::ptrdiff_t m_bDofEnabled = 0x575; // bool
	//constexpr std::ptrdiff_t m_flDofNearBlurry = 0x578; // float
	//constexpr std::ptrdiff_t m_flDofNearCrisp = 0x57C; // float
	//constexpr std::ptrdiff_t m_flDofFarCrisp = 0x580; // float
	//constexpr std::ptrdiff_t m_flDofFarBlurry = 0x584; // float
	//constexpr std::ptrdiff_t m_flDofTiltToGround = 0x588; // float
	//constexpr std::ptrdiff_t m_TargetFOV = 0x58C; // float
	//constexpr std::ptrdiff_t m_DegreesPerSecond = 0x590; // float
	//constexpr std::ptrdiff_t m_bIsOn = 0x594; // bool
	//constexpr std::ptrdiff_t m_pNext = 0x598; // C_PointCamera*
};

int __fastcall hooks::emit_sound::hooked(void* rcx, EmitSound_t* params, int16_t a3, uint32_t ent_index)
{
	printf("Hello from emitsound hooked, index: %u\n", ent_index);

	return safetyhook.fastcall<int>(rcx, params, a3, ent_index);
}
