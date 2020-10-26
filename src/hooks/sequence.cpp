#include "hooks.h"
#include "../settings/globals.h"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../features/features.h"
#include "../valve_sdk/misc/Recv.hpp"

namespace hooks
{
	recv_prop_hook* sequence::hook;

	int get_new_animation(const fnv::hash model, const int sequence)
	{
		switch (model)
		{
		case FNV("models/weapons/v_pist_deagle.mdl"):
		{
			switch (sequence)
			{
			case 7:
				return 8;
			default:
				return sequence;
			}
		}
		case FNV("models/weapons/v_knife_butterfly.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
				return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
			default:
				return sequence + 1;
			}
		}
		case FNV("models/weapons/v_knife_falchion_advanced.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_FALCHION_IDLE1;
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				return utils::random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			default:
				return sequence - 1;
			}
		}
		case FNV("models/weapons/v_knife_push.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_DAGGERS_IDLE1;
			case SEQUENCE_DEFAULT_LIGHT_MISS1:
			case SEQUENCE_DEFAULT_LIGHT_MISS2:
				return utils::random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				return utils::random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
			case SEQUENCE_DEFAULT_HEAVY_HIT1:
			case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
			case SEQUENCE_DEFAULT_LOOKAT01:
				return sequence + 3;
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			default:
				return sequence + 2;
			}
		}
		case FNV("models/weapons/v_knife_survival_bowie.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_BOWIE_IDLE1;
			default:
				return sequence - 1;
			}
		}
		case FNV("models/weapons/v_knife_ursus.mdl"):
		case FNV("models/weapons/v_knife_cord.mdl"):
		case FNV("models/weapons/v_knife_canis.mdl"):
		case FNV("models/weapons/v_knife_outdoor.mdl"):
		case FNV("models/weapons/v_knife_skeleton.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
				return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
			default:
				return sequence + 1;
			}
		}
		case FNV("models/weapons/v_knife_stiletto.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(12, 13);
			}
		}
		case FNV("models/weapons/v_knife_widowmaker.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(14, 15);
			case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				return sequence - 1;
			}
		}
		case FNV("models/weapons/v_knife_css.mdl"):
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_LOOKAT01:
				return utils::random(SEQUENCE_CSS_LOOKAT02, SEQUENCE_CSS_LOOKAT02);
			default:
				return sequence;
			}
		}
		default:
			return sequence;
		}
	}

	void remapping(CRecvProxyData* data, c_base_view_model* entity)
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return;

		const auto owner = (c_base_player*)g::entity_list->GetClientEntityFromHandle(entity->m_hOwner());
		if (owner != g::local_player)
			return;

		const auto view_model_weapon = (c_base_attributable_item*)g::entity_list->GetClientEntityFromHandle(entity->m_hWeapon());
		if (!view_model_weapon)
			return;

		if (skins::get_weapon_info(view_model_weapon->m_iItemDefinitionIndex()) == 0)
			return;

		const auto weapon_info = skins::get_weapon_info(view_model_weapon->m_iItemDefinitionIndex());
		auto& sequence = data->m_Value.m_Int;
		sequence = get_new_animation(fnv::hash_runtime(weapon_info->model), sequence);
	}

	void sequence::hooked(const CRecvProxyData* data, void* entity, void* output)
	{
		post_data_update::call();

		static auto original = hook->get_original_function();

		const auto proxy_data = const_cast<CRecvProxyData*>(data);
		const auto view_model = static_cast<c_base_view_model*>(entity);

		remapping(proxy_data, view_model);

		original(data, entity, output);
	}
}