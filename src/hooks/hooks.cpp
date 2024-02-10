#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

bool hooks::init()
{
	if (MH_Initialize() != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::entity_system, on_add_entity::index, &on_add_entity::hooked, reinterpret_cast<void**>(&on_add_entity::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::entity_system, on_remove_entity::index, &on_remove_entity::hooked, reinterpret_cast<void**>(&on_remove_entity::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::csgo_input, create_move::index, &create_move::hooked, reinterpret_cast<void**>(&create_move::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A").as(), &frame_stage_notify::hooked, reinterpret_cast<void**>(&frame_stage_notify::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("E8 ? ? ? ? F3 0F 11 45 ? 48 8B 5C 24 ?").add(0x1).abs().as(), &get_fov::hooked, reinterpret_cast<void**>(&get_fov::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::swap_chain, directx::present::index, &directx::present::hooked, reinterpret_cast<void**>(&directx::present::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::swap_chain, directx::resize_buffers::index, &directx::resize_buffers::hooked, reinterpret_cast<void**>(&directx::resize_buffers::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("40 53 48 81 EC ? ? ? ? 49 8B C1").as(), get_matrices_for_view::hooked, reinterpret_cast<void**>(&get_matrices_for_view::original_fn)) != MH_STATUS::MH_OK)
		return false;

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

bool hooks::detach()
{
	SetWindowLongPtrA(globals::hwnd, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

	if (MH_Uninitialize() != MH_STATUS::MH_OK)
		return false;

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

float __fastcall hooks::get_fov::hooked(void* camera)
{
	if (g::engine_client->IsInGame() && settings::visuals::m_bFovChanger)
		return static_cast<float>(settings::visuals::m_iFov);

    return original_fn(camera);
}

void get_viewmatrix(VMatrix* world_to_projection)
{
	if (!entity_data::view_matrix::matrix)
		entity_data::view_matrix::matrix = world_to_projection;
}

//Temp placement, move later
void __fastcall hooks::get_matrices_for_view::hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels)
{
	if (g::engine_client->IsInGame() && world_to_projection)
		get_viewmatrix(world_to_projection);

	original_fn(rcx, rdx, world_to_view, view_to_projection, world_to_projection, world_to_pixels);
}


