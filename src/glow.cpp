#include "features/features.h"

namespace visuals
{
	void glow()
	{
		for (int i = 0; i < g::glow_manager->size; i++)
		{
			auto& objects = g::glow_manager->objects[i];

			if (objects.unused())
				continue;

			auto entity = reinterpret_cast<c_base_player*>(objects.get_entity());
			auto grenade = reinterpret_cast<c_base_combat_weapon*>(objects.get_entity());

			if (!entity)
				continue;

			if (entity->IsDormant())
				continue;

			auto client_class = entity->GetClientClass();

			if (!client_class)
				continue;

			auto class_id = client_class->m_ClassID;

			if (!class_id)
				continue;

			if (!g::local_player)
				continue;

			if (entity->IsPlayer() && class_id == EClassId::CCSPlayer)
			{
				if (settings::glow::glowEnemyEnabled && entity->m_iTeamNum() != g::local_player->m_iTeamNum() && entity->IsAlive())
				{
					objects.color(settings::glow::glowEnemyColor);
					objects.bloom_amount(1.0f);
					//objects.full_bloom_render(true);
					objects.render_when_occluded(true);
					objects.render_when_unoccluded(false);
					objects.glow_style(settings::glow::style_enemy);
				}
				else if (settings::glow::glowTeamEnabled && entity->m_iTeamNum() == g::local_player->m_iTeamNum() && entity->IsAlive())
				{
					objects.color(settings::glow::glowTeamColor);
					objects.bloom_amount(1.0f);
					//objects.full_bloom_render(true);
					objects.render_when_occluded(true);
					objects.render_when_unoccluded(false);
					objects.glow_style(settings::glow::style_teammate);
				}
				else if (entity->EntIndex() == g::local_player->EntIndex() && entity->IsAlive())
				{
					objects.color(settings::glow::glowEnemyColor);
					objects.bloom_amount(1.0f);
					//objects.full_bloom_render(true);
					objects.render_when_occluded(true);
					objects.render_when_unoccluded(false);
					objects.glow_style(settings::glow::style_teammate);
				}
			}
			else if (settings::glow::glowC4PlantedEnabled && class_id == EClassId::CPlantedC4 || class_id == EClassId::CBaseAnimating)
			{
				objects.color(settings::glow::glowC4PlantedColor);
				objects.bloom_amount(1.0f);
				//objects.full_bloom_render(true);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (settings::glow::glowNadesEnabled && class_id == EClassId::CBaseCSGrenade || class_id == EClassId::CBaseCSGrenadeProjectile ||
				class_id == EClassId::CDecoyGrenade || class_id == EClassId::CHEGrenade || class_id == EClassId::CIncendiaryGrenade || class_id == EClassId::CMolotovProjectile || class_id == EClassId::CMolotovGrenade ||
				class_id == EClassId::CSensorGrenade || class_id == EClassId::CSensorGrenadeProjectile || class_id == EClassId::CSmokeGrenade || class_id == EClassId::CSmokeGrenadeProjectile ||
				class_id == EClassId::ParticleSmokeGrenade || class_id == EClassId::CBaseGrenade && grenade && grenade->m_nExplodeEffectTickBegin() < 1) {

				objects.color(settings::glow::glowNadesColor);
				objects.bloom_amount(1.0f);
				//objects.full_bloom_render(true);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (entity->IsWeapon() && settings::glow::glowDroppedWeaponsEnabled && class_id == EClassId::CAK47 || class_id == EClassId::CDEagle || class_id == EClassId::CC4 ||
				class_id >= EClassId::CWeaponAug && class_id <= EClassId::CWeaponXM1014) {

				objects.color(settings::glow::glowDroppedWeaponsColor);
				objects.bloom_amount(1.0f);
				//objects.full_bloom_render(true);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
			else if (settings::glow::glowDroppedKitsEnabled && class_id == EClassId::CEconEntity)
			{
				objects.color(settings::glow::glowDroppedKitsColor);
				objects.bloom_amount(1.0f);
				//objects.full_bloom_render(true);
				objects.render_when_occluded(true);
				objects.render_when_unoccluded(false);
				objects.glow_style(0);
			}
		}
	}
}