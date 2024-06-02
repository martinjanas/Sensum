#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

bool hooks::init()
{
	entity_system = ShadowVMT(g::entity_system);
	csgo_input = ShadowVMT(g::csgo_input);
	client = ShadowVMT(g::client);
	swap_chain = ShadowVMT(g::swap_chain);
	client_mode = ShadowVMT(g::client_mode);

	entity_system.Apply(on_add_entity::index, reinterpret_cast<uintptr_t*>(&on_add_entity::hooked), reinterpret_cast<void**>(&on_add_entity::original_fn));
	entity_system.Apply(on_remove_entity::index, reinterpret_cast<uintptr_t*>(&on_remove_entity::hooked), reinterpret_cast<void**>(&on_remove_entity::original_fn));

	csgo_input.Apply(create_move::index, reinterpret_cast<uintptr_t*>(&create_move::hooked), reinterpret_cast<void**>(&create_move::original_fn));
	client.Apply(frame_stage_notify::index, reinterpret_cast<uintptr_t*>(&frame_stage_notify::hooked), reinterpret_cast<void**>(&frame_stage_notify::original_fn));

	swap_chain.Apply(directx::present::index, reinterpret_cast<uintptr_t*>(&directx::present::hooked), reinterpret_cast<void**>(&directx::present::original_fn));
	swap_chain.Apply(directx::resize_buffers::index, reinterpret_cast<uintptr_t*>(&directx::resize_buffers::hooked), reinterpret_cast<void**>(&directx::resize_buffers::original_fn));

	client_mode.Apply(clientmode_createmove::index, reinterpret_cast<uintptr_t*>(&clientmode_createmove::hooked), reinterpret_cast<void**>(&clientmode_createmove::original_fn));
	//csgo_input.Apply(override_view::index, reinterpret_cast<uintptr_t*>(&override_view::hooked), reinterpret_cast<void**>(&override_view::original_fn));

	get_fov::safetyhook = safetyhook::create_inline(modules::client.pattern_scanner.scan("E8 ? ? ? ? F3 0F 11 45 ? 48 8B 5C 24 ?").add(0x1).abs().as(), reinterpret_cast<void*>(get_fov::hooked));
	get_matrices_for_view::safetyhook = safetyhook::create_inline(modules::client.pattern_scanner.scan("40 53 48 81 EC ? ? ? ? 49 8B C1").as(), reinterpret_cast<void*>(get_matrices_for_view::hooked));
	
	return true;
}

bool hooks::detach()
{
	SetWindowLongPtrA(globals::hwnd, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

	entity_system.Remove(on_add_entity::index);
	entity_system.Remove(on_remove_entity::index);
	csgo_input.Remove(create_move::index);
	client.Remove(frame_stage_notify::index);
	swap_chain.Remove(directx::present::index);
	swap_chain.Remove(directx::resize_buffers::index);

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

//void __fastcall hooks::override_view::hooked(void* rcx, void* rdx, CViewSetup* setup)
//{
//	if (g::engine_client->IsInGame())
//		setup->flFov = 120.f;
//
//	original_fn(rcx, rdx, setup);
//}

