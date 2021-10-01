#include "../features/features.h"

#include "../settings/globals.h"

namespace visuals
{
	void glow_players()
	{
		c_base_player* player = nullptr;
		for (int i = 0; i < g::glow_manager->size; i++)
		{
			auto& objects = g::glow_manager->objects[i];

			if (objects.unused())
				continue;

			if (!g::local_player)
				continue;

			player = reinterpret_cast<c_base_player*>(objects.m_pEntity);

			if (!player)
				continue;

			if (!player->IsPlayer())
				continue;

			if (!player->IsAlive() || player->IsDormant())
				continue;

			if (settings::glow::enemy::enabled && player->m_iTeamNum() != g::local_player->m_iTeamNum())
			{
				bool is_vis = (!globals::is_visible[player->GetPlayerInfo().userId] || globals::in_smoke[player->GetPlayerInfo().userId]);

				Color color = !is_vis ? settings::glow::enemy::visible_color : settings::glow::enemy::invisible_color;

				bool visible = (settings::glow::enemy::visible_only && is_vis);

				//objects.set(color, visible ? false : true, !visible ? true : false, settings::glow::enemy::style);
				objects.set(color, visible ? false : true, false, settings::glow::enemy::style);
				objects.full_bloom_render(false);
			}

			if (settings::glow::teammates::enabled && player->m_iTeamNum() == g::local_player->m_iTeamNum())
			{
				bool is_vis = (!globals::is_visible[player->GetPlayerInfo().userId] || globals::in_smoke[player->GetPlayerInfo().userId]);

				Color color = !is_vis ? settings::glow::teammates::visible_color : settings::glow::teammates::invisible_color;

				bool visible = (settings::glow::teammates::visible_only && is_vis);

				//objects.set(color, visible ? false : true, !visible ? true : false, settings::glow::teammates::style);
				objects.set(color, visible ? false : true, false, settings::glow::teammates::style);
				objects.full_bloom_render(false);
			}
		}
	}

	void glow_misc()
	{
		for (int i = 0; i < g::glow_manager->size; i++)
		{
			auto& objects = g::glow_manager->objects[i];

			auto entity = reinterpret_cast<c_base_combat_weapon*>(objects.m_pEntity);

			if (!entity)
				continue;

			auto client_class = entity->GetClientClass();

			if (!client_class)
				continue;

			auto class_id = client_class->m_ClassID;

			if (!class_id)
				continue;

			if (settings::glow::misc::bomb::enabled && class_id == EClassId::CPlantedC4 || class_id == EClassId::CBaseAnimating)
			{
				objects.color(settings::glow::misc::bomb::color);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.full_bloom_render(false);
				objects.glow_style(0);
			}
			else if (settings::glow::misc::nades::enabled && class_id == EClassId::CBaseCSGrenade || class_id == EClassId::CBaseCSGrenadeProjectile ||
				class_id == EClassId::CDecoyGrenade || class_id == EClassId::CHEGrenade || class_id == EClassId::CIncendiaryGrenade || class_id == EClassId::CMolotovProjectile || class_id == EClassId::CMolotovGrenade ||
				class_id == EClassId::CSensorGrenade || class_id == EClassId::CSensorGrenadeProjectile || class_id == EClassId::CSmokeGrenade || class_id == EClassId::CSmokeGrenadeProjectile ||
				class_id == EClassId::ParticleSmokeGrenade || class_id == EClassId::CBaseGrenade && entity && entity->m_nExplodeEffectTickBegin() < 1) {

				objects.color(settings::glow::misc::nades::color);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.full_bloom_render(false);
				objects.glow_style(0);
			}
			else if (entity->IsWeapon() && settings::glow::misc::weapons::enabled && class_id == EClassId::CAK47 || class_id == EClassId::CDEagle || class_id == EClassId::CC4 ||
				class_id >= EClassId::CWeaponAug && class_id <= EClassId::CWeaponXM1014) {

				objects.color(settings::glow::misc::weapons::color);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.full_bloom_render(false);
				objects.glow_style(0);
			}
			else if (settings::glow::misc::kits::enabled && class_id == EClassId::CEconEntity)
			{
				objects.color(settings::glow::misc::kits::color);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.full_bloom_render(false);
				objects.glow_style(0);
			}

		}
	}
}