#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/helpers/entity_data.h"

void skinchanger() //doesnt work
{
	if (!g::engine_client->IsInGame())
		return;

	auto local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
	if (!local_controller)
		return;

	auto pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPlayerPawn()); //local_controller->m_hPlayerPawn().Get<CCSPlayerPawn*>();
	if (!pawn)
		return;

	const auto& weapon_services = pawn->m_pWeaponServices();
	if (!weapon_services)
		return;

	static int paintkit = 522; //fade

	auto my_weapons = weapon_services->m_hMyWeapons();
	for (int i = 0; i < my_weapons.Count(); ++i)
	{
		const auto& handle = my_weapons[i];
		if (!handle.IsValid())
			continue;
			
		auto weapon = g::entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(handle);
		if (!weapon)
			continue;

		weapon->m_AttributeManager().m_Item().m_iItemIDHigh() = -1;
		weapon->m_AttributeManager().m_Item().m_iItemIDLow() = -1;
		
		weapon->m_nFallbackPaintKit() = paintkit;

		auto client = g::network_game_service->GetNetworkGameClient();

		if (client && g::input_system->IsButtonDown(ButtonCode::KeyR))
			client->FullUpdate();
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
		//skinchanger();
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		break;
	case FRAME_NET_UPDATE_END:
		entity_data::fetch_player_data(nullptr);
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

	original_fn(rcx, stage);
}
