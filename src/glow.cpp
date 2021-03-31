#include "features/features.h"

namespace visuals
{
	void glow_players()
	{
		for (int i = 0; i < g::glow_manager->size; i++)
		{
			auto& objects = g::glow_manager->objects[i];

			if (!g::local_player)
				continue;

			c_base_player* player = reinterpret_cast<c_base_player*>(objects.entity);

			if (!player)
				continue;

			if (!player->IsPlayer())
				continue;

			if (!player->IsAlive() || player->IsDormant())
				continue;

			auto client_class = player->GetClientClass();

			if (!client_class)
				continue;

			auto class_id = client_class->m_ClassID;

			if (!class_id)
				continue;

			if (class_id != EClassId::CCSPlayer)
				continue;

			if (settings::glow::glowEnemyEnabled && player->m_iTeamNum() != g::local_player->m_iTeamNum())
			{
				bool is_vis = settings::glow::visible_only && !(g::local_player->CanSeePlayer(player, player->get_hitbox_position(player, HITBOX_CHEST)) || g::local_player->CanSeePlayer(player, player->get_hitbox_position(player, HITBOX_HEAD)));

				objects.color(settings::glow::glowEnemyColor);
				objects.render_when_occluded(!is_vis);
				objects.render_when_unoccluded(is_vis);
				objects.glow_style(settings::glow::style_enemy);
			}

			if (settings::glow::glowTeamEnabled && player->m_iTeamNum() == g::local_player->m_iTeamNum())
			{
				bool is_vis = settings::glow::teammates_visible_only && (!g::local_player->CanSeePlayer(player, player->get_hitbox_position(player, HITBOX_CHEST)) || !g::local_player->CanSeePlayer(player, player->get_hitbox_position(player, HITBOX_HEAD)));

				objects.color(settings::glow::glowTeamColor);
				objects.render_when_occluded(!is_vis);
				objects.render_when_unoccluded(is_vis);
				objects.glow_style(settings::glow::style_teammate);
			}

			if (player->EntIndex() == g::local_player->EntIndex())
			{
				objects.color(settings::glow::glowEnemyColor);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(settings::glow::style_teammate);
			}
		}
	}

	void glow_misc()
	{
		for (int i = 0; i < g::glow_manager->size; i++)
		{
			auto& objects = g::glow_manager->objects[i];

			auto entity = reinterpret_cast<c_base_combat_weapon*>(objects.entity);

			if (!entity)
				continue;

			auto client_class = entity->GetClientClass();

			if (!client_class)
				continue;

			auto class_id = client_class->m_ClassID;

			if (!class_id)
				continue;

			if (settings::glow::glowC4PlantedEnabled && class_id == EClassId::CPlantedC4 || class_id == EClassId::CBaseAnimating)
			{
				objects.color(settings::glow::glowC4PlantedColor);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (settings::glow::glowNadesEnabled && class_id == EClassId::CBaseCSGrenade || class_id == EClassId::CBaseCSGrenadeProjectile ||
				class_id == EClassId::CDecoyGrenade || class_id == EClassId::CHEGrenade || class_id == EClassId::CIncendiaryGrenade || class_id == EClassId::CMolotovProjectile || class_id == EClassId::CMolotovGrenade ||
				class_id == EClassId::CSensorGrenade || class_id == EClassId::CSensorGrenadeProjectile || class_id == EClassId::CSmokeGrenade || class_id == EClassId::CSmokeGrenadeProjectile ||
				class_id == EClassId::ParticleSmokeGrenade || class_id == EClassId::CBaseGrenade && entity && entity->m_nExplodeEffectTickBegin() < 1) {

				objects.color(settings::glow::glowNadesColor);
				objects.bloom_amount(1.0f);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (entity->IsWeapon() && settings::glow::glowDroppedWeaponsEnabled && class_id == EClassId::CAK47 || class_id == EClassId::CDEagle || class_id == EClassId::CC4 ||
				class_id >= EClassId::CWeaponAug && class_id <= EClassId::CWeaponXM1014) {

				objects.color(settings::glow::glowDroppedWeaponsColor);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (settings::glow::glowDroppedKitsEnabled && class_id == EClassId::CEconEntity)
			{
				objects.color(settings::glow::glowDroppedKitsColor);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}

		}
	}
}