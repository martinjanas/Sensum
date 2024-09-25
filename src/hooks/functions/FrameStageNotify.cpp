#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/helpers/entity_data.h"

void skinchanger()
{
	printf("hello suka\n");

	if (!g::engine_client->IsInGame())
		return;

	auto local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
	if (!local_controller)
	{
		printf("no controller\n");
		return;
	}
	else printf("got controller\n");
		
	auto pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPlayerPawn()); //local_controller->m_hPlayerPawn().Get<CCSPlayerPawn*>();
	if (!pawn)
		return;

	const auto& weapon_services = pawn->m_pWeaponServices();
	if (!weapon_services)
		return;

	auto& my_weapons = weapon_services->m_hMyWeapons();
	
	static int paintkit = 522; //fade

	printf("size: %d\n", my_weapons.Count());

	for (int i = 0; i < my_weapons.Count(); ++i)
	{
		const auto& handle = my_weapons[i];
		if (!handle.IsValid())
			continue;
			
		auto weapon = g::entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(handle); //handle.Get<CBasePlayerWeapon*>();
		if (!weapon)
			continue;

		auto attrib_manager = weapon->m_AttributeManager();
		if (!attrib_manager)
			continue;

		auto item = attrib_manager->m_Item();
		if (!item)
			continue;

		item->m_iItemIDHigh() = -1;
		item->m_iItemIDLow() = -1;

		weapon->m_nFallbackPaintKit() = 38;
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
		/*printf("Hello from postdataupdate_start\n");
		skinchanger();*/
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
