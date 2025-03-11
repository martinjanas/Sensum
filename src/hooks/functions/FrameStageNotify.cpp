#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/helpers/entity_data.h"

//temp placement
void SetMeshGroupMask(void* scene_node, uint64_t mask)
{
	using fn = void(__fastcall*)(void*, uint64_t);
	static auto addr = modules::client.scan("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71", "SetMeshGroupMask").as<>();
	if (!addr)
		return;

	auto set_mesh_group_mask = reinterpret_cast<fn>(addr);

	set_mesh_group_mask(scene_node, mask);
}

void skinchanger()
{
	if (!g::engine_client->IsInGame())
		return;

	auto local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
	if (!local_controller)
		return;

	auto pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPawn()); //local_controller->m_hPlayerPawn().Get<CCSPlayerPawn*>();
	if (!pawn)
		return;

	const auto& weapon_services = pawn->m_pWeaponServices();
	if (!weapon_services)
		return;

	const auto& viewmodel_services = pawn->m_pViewModelServices();
	if (!viewmodel_services)
		return;
	
	static int paintkit = 282; //fade

	auto my_weapons = weapon_services->m_hMyWeapons();
	for (int i = 0; i < my_weapons.Count(); ++i)
	{
		const auto& handle = my_weapons[i];
		if (!handle.IsValid())
			continue;
			
		auto weapon = g::entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(handle);
		if (!weapon)
			continue;

		if (weapon->m_iItemDefinitionIndex() != WEAPON_AK47)
			continue;
		
		auto viewmodel_entity = g::entity_system->GetEntityFromHandle<CBaseEntity*>(viewmodel_services->m_hViewModel());
		if (!viewmodel_entity)
			return;
		
		weapon->m_AttributeManager().m_Item().m_iItemIDHigh() = -1;
		weapon->m_AttributeManager().m_Item().m_iAccountID() = weapon->m_OriginalOwnerXuidLow();
		
		weapon->m_nFallbackPaintKit() = paintkit;

		auto scene_node = weapon->m_pGameSceneNode();
		if (!scene_node)
			return;
		
		SetMeshGroupMask(scene_node, 2);
		SetMeshGroupMask(viewmodel_entity->m_pGameSceneNode(), 2);
		
	}
}

void __fastcall hooks::frame_stage_notify::hooked(void* rcx, EClientFrameStage stage)
{
	switch (stage)
	{
	case FRAME_START:
		break;
	case FRAME_NET_UPDATE_START:
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		break;
	case FRAME_NET_UPDATE_END:
		break;
	case FRAME_RENDER_START:
		break;
	case FRAME_RENDER_END:
		break;
	case FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE:
		break;
	default:
		break;
	}
	
	skinchanger();
	
	original_fn(rcx, stage);
}
