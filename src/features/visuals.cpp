#include "features.h"
#include "../settings/globals.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"
#include "../helpers/console.h"
#include "../features/esp.hpp"
#include "../hooks/hooks.h"
#include "../helpers/runtime_saver.h"
#include "../render/render.h"
#include "../helpers/autowall.h"
#include "../helpers/MaterialManager.h"

#include <mutex>

extern float side;

QAngle viewanglesBackup;

namespace visuals
{
	std::mutex render_mutex;

	struct entity_data_t
	{
		std::string text;
		std::string text2;
		Vector origin;
		Color color;
		bool is_grenade;
	};

	struct grenade_info_t
	{
		std::string name;
		Color color;
	};

	RECT GetBBox(c_base_entity* ent)
	{
		RECT rect{};
		auto collideable = ent->GetCollideable();

		if (!collideable)
			return rect;

		const auto& min = collideable->OBBMins();
		const auto& max = collideable->OBBMaxs();

		const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

		Vector points[] =
		{
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		Vector pointsTransformed[8];

		for (int i = 0; i < 8; i++)
			math::VectorTransform(points[i], trans, pointsTransformed[i]);

		Vector screen_points[8] = {};

		for (int i = 0; i < 8; i++)
		{
			if (!math::world2screen(pointsTransformed[i], screen_points[i]))
				return rect;
		}

		auto left = screen_points[0].x;
		auto top = screen_points[0].y;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++)
		{
			if (left > screen_points[i].x)
				left = screen_points[i].x;

			if (top < screen_points[i].y)
				top = screen_points[i].y;

			if (right < screen_points[i].x)
				right = screen_points[i].x;

			if (bottom > screen_points[i].y)
				bottom = screen_points[i].y;
		}

		return RECT{ (long)left, (long)top, (long)right, (long)bottom };
	}

	std::vector<entity_data_t> entities;
	std::vector<entity_data_t> saved_entities;

	bool is_enabled()
	{
		return g::engine_client->IsConnected() && g::local_player && !render::menu::is_visible();
	}

	void push_entity(c_base_entity* entity, const std::string& text, const std::string& text2, bool is_grenade, const Color& color = Color::White)
	{
		entities.emplace_back(entity_data_t{ text, text2, entity->m_vecOrigin(), color, is_grenade });
	}

	void world_grenades(c_base_player* entity)
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return;

		if (g::local_player->IsFlashed())
			return;

		/*if (!g::local_player->CanSeePlayer(entity, entity->GetRenderOrigin()))
			return; */

		if (utils::is_line_goes_through_smoke(g::local_player->GetEyePos(), entity->GetRenderOrigin()))
			return;

		auto bbox = GetBBox(entity);

		auto class_id = entity->GetClientClass()->m_ClassID;
		std::string name = "";

		auto grenade = reinterpret_cast<c_base_combat_weapon*>(entity);

		if (!grenade)
			return;

		switch (class_id)
		{
		case EClassId::CBaseCSGrenadeProjectile:
		case EClassId::CMolotovProjectile:
		case EClassId::CDecoyProjectile:
		case EClassId::CSmokeGrenadeProjectile:
		case EClassId::CSensorGrenadeProjectile:
			name = entity->m_hOwnerEntity().Get()->GetPlayerInfo().szName; break;
		}

		grenade_info_t info;
		const auto model_name = fnv::hash_runtime(g::mdl_info->GetModelName(entity->GetModel()));
		if (model_name == FNV("models/Weapons/w_eq_smokegrenade_thrown.mdl"))
			info = { "Smoke", Color::White };
		else if (model_name == FNV("models/Weapons/w_eq_flashbang_dropped.mdl"))
			info = { "Flash", Color::Yellow };
		else if (model_name == FNV("models/Weapons/w_eq_fraggrenade_dropped.mdl"))
			info = { "Grenade", Color::Red };
		else if (model_name == FNV("models/Weapons/w_eq_molotov_dropped.mdl") || model_name == FNV("models/Weapons/w_eq_incendiarygrenade_dropped.mdl"))
			info = { "Molly", Color::Orange };
		else if (model_name == FNV("models/Weapons/w_eq_decoy_dropped.mdl"))
			info = { "Decoy", Color::Green };

		if (!info.name.empty() && (grenade->m_nExplodeEffectTickBegin() < 1))
			push_entity(entity, info.name, name, true, info.color);
	}

	void DrawDamageIndicator()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (!g::local_player && !g::local_player->IsAlive())
			return;

		float CurrentTime = g::local_player->m_nTickBase() * g::global_vars->interval_per_tick;

		for (int i = 0; i < indicator.size(); i++)
		{
			if (indicator[i].flEraseTime < CurrentTime)
			{
				indicator.erase(indicator.begin() + i);
				continue;
			}

			if (!indicator[i].bInitialized)
			{
				indicator[i].Position = indicator[i].Player->get_bone_position(8);
				indicator[i].bInitialized = true;
			}

			if (CurrentTime - indicator[i].flLastUpdate > 0.001f)
			{
				indicator[i].Position.z -= (0.5f * (CurrentTime - indicator[i].flEraseTime));
				indicator[i].flLastUpdate = CurrentTime;
			}
			
			if (!indicator[i].Player)
			{
				indicator.erase(indicator.begin() + i);
				continue;
			}

			Vector ScreenPosition;

			Color color = Color::White;

			if (indicator[i].iDamage >= 100)
				color = Color::Red;

			if (indicator[i].iDamage >= 50 && indicator[i].iDamage < 100)
				color = Color::Orange;

			if (indicator[i].iDamage < 50)
				color = Color::White;

			if (math::world2screen(indicator[i].Position, ScreenPosition))
			{
				globals::draw_list->AddText(NULL, 0.0f, ImVec2(ScreenPosition.x, ScreenPosition.y), IM_COL32(color.r(), color.g(), color.b(), color.a()), std::to_string(indicator[i].iDamage).c_str());
			}
		}
	}

	void RenderPunchCross()
	{
		int w, h;

		g::engine_client->GetScreenSize(w, h);

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (!g::local_player && !g::local_player->IsAlive())
			return;

		int x = w / 2;
		int y = h / 2;
		int dy = h / 97;
		int dx = w / 97;

		QAngle punchAngle = g::local_player->m_aimPunchAngle();
		x -= (dx * (punchAngle.yaw));
		y += (dy * (punchAngle.pitch));

		auto active_wep = g::local_player->m_hActiveWeapon();

        if (active_wep)
        {
            auto index = active_wep->m_iItemDefinitionIndex();

            if (index == WEAPON_USP_SILENCER || index == WEAPON_DEAGLE || index == WEAPON_GLOCK || index == WEAPON_REVOLVER || 
                index == WEAPON_HKP2000)
                return;
        }

		float radius = settings::visuals::radius;

		switch (settings::visuals::rcs_cross_mode)
		{
		case 0:
            if (g::local_player->m_iShotsFired() > 1)
            {
                globals::draw_list->AddLine(ImVec2(x - 5, y), ImVec2(x + 5, y), ImGui::GetColorU32(settings::visuals::recoilcolor));
                globals::draw_list->AddLine(ImVec2(x, y - 5), ImVec2(x, y + 5), ImGui::GetColorU32(settings::visuals::recoilcolor));
            }
			break;
		case 1:
            if (g::local_player->m_iShotsFired() > 1)
            {
                globals::draw_list->AddCircle(ImVec2(x, y), radius, ImGui::GetColorU32(settings::visuals::recoilcolor), 255);
            }
			break;
		}
	}

	void KnifeLeft()
	{
		static auto left_knife = g::cvar->find("cl_righthand");

		if (!g::local_player || !g::local_player->IsAlive())
		{
			left_knife->SetValue(1);
			return;
		}

		auto weapon = g::local_player->m_hActiveWeapon();
		if (!weapon) return;

		left_knife->SetValue(!weapon->IsKnife());
	}

	void DrawFov()
	{
		auto pWeapon = g::local_player->m_hActiveWeapon();
		if (!pWeapon)
			return;

		if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
			return;

		const auto& settings = settings::aimbot::m_items[pWeapon->m_iItemDefinitionIndex()];

		bool dynamic_fov = settings.dynamic_fov;

		if (settings.enabled) {
			float fov = static_cast<float>(g::local_player->GetFOV());

			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (!g::local_player && !g::local_player->IsAlive())
				return;

			int w, h;
			g::engine_client->GetScreenSize(w, h);

			Vector2D screenSize = Vector2D(w, h);
			Vector2D center = screenSize * 0.5f;

			float ratio = screenSize.x / screenSize.y;
			float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));

			float radiusFOV;

			if (dynamic_fov)
			{
				Vector src3D, dst3D, forward;
				trace_t tr;
				Ray_t ray;
				CTraceFilter filter;

				QAngle angles = viewanglesBackup;
				math::angle2vectors(angles, forward);
				filter.pSkip = g::local_player;
				src3D = g::local_player->GetEyePos();
				dst3D = src3D + (forward * 8192);

				ray.Init(src3D, dst3D);
				g::engine_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

				QAngle leftViewAngles = QAngle(angles.pitch, angles.yaw - 90.f, 0.f);
				math::AngleNormalize(leftViewAngles);
				math::angle2vectors(leftViewAngles, forward);
				forward *= settings.fov * 7.f;

				Vector maxAimAt = tr.endpos + forward;

				Vector max2D;
				if (g::debug_overlay->ScreenPosition(maxAimAt, max2D))
					return;

				radiusFOV = fabsf(w / 2 - max2D.x);
			}
			else
			{
				radiusFOV = tanf(DEG2RAD(aimbot::get_fov())) / tanf(screenFov) * center.x;
			}

			globals::draw_list->AddCircle(ImVec2(center.x, center.y), radiusFOV, ImGui::GetColorU32(settings::visuals::drawfov_color), 255);
		}
	}

	void runCM(CUserCmd* cmd)
	{
		viewanglesBackup = cmd->viewangles;
	}

	void RenderHitmarker()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		static int cx;
		static int cy;
		static int w, h;

		g::engine_client->GetScreenSize(w, h);
		cx = w / 2;
		cy = h / 2;

		if (g::global_vars->realtime - saver.HitmarkerInfo.HitTime > .5f)
			return;

		float percent = (g::global_vars->realtime - saver.HitmarkerInfo.HitTime) / .5f;
		float percent2 = percent;

		if (percent > 1.f)
		{
			percent = 1.f;
			percent2 = 1.f;
		}

		percent = 1.f - percent;
		float addsize = percent2 * 5.f;

		ImVec4 clr = ImVec4{ 1.0f, 1.0f, 1.0f, percent * 1.0f };

		globals::draw_list->AddLine(ImVec2(cx - 3.f - addsize, cy - 3.f - addsize), ImVec2(cx + 3.f + addsize, cy + 3.f + addsize), ImGui::GetColorU32(clr));
		globals::draw_list->AddLine(ImVec2(cx - 3.f - addsize, cy + 3.f + addsize), ImVec2(cx + 3.f + addsize, cy - 3.f - addsize), ImGui::GetColorU32(clr));
	}

	void RenderNoScopeOverlay()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		static int cx;
		static int cy;
		static int w, h;

		g::engine_client->GetScreenSize(w, h);
		cx = w / 2;
		cy = h / 2;

		auto weapon = g::local_player->m_hActiveWeapon();

		if (weapon)
		{
			if (g::local_player->m_bIsScoped() && weapon->IsSniper())
			{
				globals::draw_list->AddLine(ImVec2(0, cy), ImVec2(w, cy), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
				globals::draw_list->AddLine(ImVec2(cx, 0), ImVec2(cx, h), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
				globals::draw_list->AddCircle(ImVec2(cx, cy), 255, ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }), 255);
			}
		}
	}

	void more_chams() noexcept
	{
		static IMaterial* mat = g::mat_system->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);

		for (int i = 0; i < g::entity_list->GetHighestEntityIndex(); i++) {
			auto entity = reinterpret_cast<c_base_player*>(g::entity_list->GetClientEntity(i));

			auto grenade = reinterpret_cast<c_base_combat_weapon*>(g::entity_list->GetClientEntity(i));

			if (entity && entity != g::local_player) {
				auto client_class = entity->GetClientClass();
				auto model_name = g::mdl_info->GetModelName(entity->GetModel());

				switch (client_class->m_ClassID) {
				case EClassId::CPlantedC4:
				case EClassId::CBaseAnimating:
					if (settings::chams::misc::planted_bomb_chams) {
						g::render_view->SetColorModulation(settings::chams::misc::color_planted_bomb_chams.r() / 255.f, settings::chams::misc::color_planted_bomb_chams.g() / 255.f, settings::chams::misc::color_planted_bomb_chams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g::mdl_render->ForcedMaterialOverride(mat);
						entity->DrawModel(1, 255);
					}
					break;
				case EClassId::CHEGrenade:
				case EClassId::CFlashbang:
				case EClassId::CMolotovGrenade:
				case EClassId::CMolotovProjectile:
				case EClassId::CIncendiaryGrenade:
				case EClassId::CDecoyGrenade:
				case EClassId::CDecoyProjectile:
				case EClassId::CSmokeGrenade:
				case EClassId::CSmokeGrenadeProjectile:
				case EClassId::ParticleSmokeGrenade:
				case EClassId::CBaseCSGrenade:
				case EClassId::CBaseCSGrenadeProjectile:
				case EClassId::CBaseGrenade:
				case EClassId::CBaseParticleEntity:
				case EClassId::CSensorGrenade:
				case EClassId::CSensorGrenadeProjectile:
					if (settings::chams::misc::nade_chams && grenade && grenade->m_nExplodeEffectTickBegin() < 1) {
						g::render_view->SetColorModulation(settings::chams::misc::color_nade_chams.r() / 255.f, settings::chams::misc::color_nade_chams.g() / 255.f, settings::chams::misc::color_nade_chams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g::mdl_render->ForcedMaterialOverride(mat);
						entity->DrawModel(1, 255);
					}
					break;
				}

				if (client_class->m_ClassID == CAK47 || client_class->m_ClassID == CDEagle || client_class->m_ClassID == CC4 ||
					client_class->m_ClassID >= CWeaponAug && client_class->m_ClassID <= CWeaponXM1014) {
					if (settings::chams::misc::dropped_weapons) {
						g::render_view->SetColorModulation(settings::chams::misc::color_dropped_weapons_chams.r() / 255.f, settings::chams::misc::color_dropped_weapons_chams.g() / 255.f, settings::chams::misc::color_dropped_weapons_chams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g::mdl_render->ForcedMaterialOverride(mat);
						entity->DrawModel(1, 255);
					}
				}
				g::mdl_render->ForcedMaterialOverride(nullptr);
				mat->IncrementReferenceCount();


				if (client_class->m_ClassID == CEconEntity) {
					if (settings::chams::misc::dropped_defusekit_chams) {
						g::render_view->SetColorModulation(settings::chams::misc::color_dropped_defusekit_chams.r() / 255.f, settings::chams::misc::color_dropped_defusekit_chams.g() / 255.f, settings::chams::misc::color_dropped_defusekit_chams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						g::mdl_render->ForcedMaterialOverride(mat);
						entity->DrawModel(1, 255);
					}
				}
				//g::mdl_render->ForcedMaterialOverride(nullptr);
				//mat->IncrementReferenceCount();
			}
		}
	}

	void AAIndicator()
	{
		int x, y;

		g::engine_client->GetScreenSize(x, y);

		if (!g::local_player || !g::local_player->IsAlive())
			return;

		if (!utils::IsPlayingMM_AND_IsValveServer())
			return;

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		const auto radius = 80.f;

		int cx = x / 2;
		int cy = y / 2;

		auto draw_arrow = [&](float rot, ImU32 color) -> void
		{
			globals::draw_list->AddTriangleFilled(ImVec2(cx + cosf(rot) * radius, cy + sinf(rot) * radius),
				ImVec2(cx + cosf(rot + DEG2RAD(10)) * (radius - 23.f),
					cy + sinf(rot + DEG2RAD(10)) * (radius - 23.f)),
				ImVec2(cx + cosf(rot - DEG2RAD(10)) * (radius - 23.f),
					cy + sinf(rot - DEG2RAD(10)) * (radius - 23.f)),
				IM_COL32_BLACK);

			globals::draw_list->AddTriangleFilled(ImVec2(cx + cosf(rot) * radius, cy + sinf(rot) * radius),
				ImVec2(cx + cosf(rot + DEG2RAD(10)) * (radius - 22.f),
					cy + sinf(rot + DEG2RAD(10)) * (radius - 22.f)),
				ImVec2(cx + cosf(rot - DEG2RAD(10)) * (radius - 22.f),
					cy + sinf(rot - DEG2RAD(10)) * (radius - 22.f)),
				color);
		};


		draw_arrow(side > 0.0f ? PI : -PI * 4, IM_COL32(settings::esp::aa_indicator_color.r(), settings::esp::aa_indicator_color.g(), settings::esp::aa_indicator_color.b(), settings::esp::aa_indicator_color.a()));
	}

	void DesyncChams()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (g::local_player->m_bGunGameImmunity() || g::local_player->m_fFlags() & FL_FROZEN)
			return;

		if (!utils::IsPlayingMM_AND_IsValveServer())
			return;

		QAngle OrigAng;

		static IMaterial* material = nullptr;

		static IMaterial* Normal = g::mat_system->FindMaterial("sensum_regular", TEXTURE_GROUP_MODEL);
		static IMaterial* Dogtags = g::mat_system->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);
		static IMaterial* Flat = g::mat_system->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
		static IMaterial* Metallic = g::mat_system->FindMaterial("sensum_reflective", TEXTURE_GROUP_MODEL);
		static IMaterial* Platinum = g::mat_system->FindMaterial("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_MODEL);
		static IMaterial* Glass = g::mat_system->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_MODEL);
		static IMaterial* Crystal = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL);
		static IMaterial* Gold = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL);
		static IMaterial* DarkChrome = g::mat_system->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_MODEL);
		static IMaterial* PlasticGloss = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_MODEL);
		static IMaterial* Glow = g::mat_system->FindMaterial("vgui/achievements/glow", TEXTURE_GROUP_MODEL);

		Normal->IncrementReferenceCount();
		Dogtags->IncrementReferenceCount();
		Flat->IncrementReferenceCount();
		Metallic->IncrementReferenceCount();
		Platinum->IncrementReferenceCount();
		Glass->IncrementReferenceCount();
		Crystal->IncrementReferenceCount();
		Gold->IncrementReferenceCount();
		DarkChrome->IncrementReferenceCount();
		PlasticGloss->IncrementReferenceCount();
		Glow->IncrementReferenceCount();

		switch (settings::chams::localplayer::desync_chams_mode)
		{
		case 0: material = Normal;
			break;
		case 1: material = MaterialManager::Get().material_crystal_blue;
			break;
		case 2: material = Flat;
			break;
		case 3: material = Metallic;
			break;
		case 4: material = Platinum;
			break;
		case 5: material = Glass;
			break;
		case 6: material = Crystal;
			break;
		case 7: material = Gold;
			break;
		case 8: material = DarkChrome;
			break;
		case 9: material = PlasticGloss;
			break;
		case 10: material = Glow;
			break;
		}

		OrigAng = g::local_player->GetAbsAngles();
		g::local_player->SetAngle2(QAngle(0, g::local_player->GetPlayerAnimState()->m_flEyeYaw, 0)); //around 90% accurate
		if (g::input->m_fCameraInThirdPerson)
		{
			g::mdl_render->ForcedMaterialOverride(material);
			g::render_view->SetColorModulation(settings::chams::localplayer::desync_color.r(), settings::chams::localplayer::desync_color.g(), settings::chams::localplayer::desync_color.b());
		}
		g::local_player->GetClientRenderable()->DrawModel(0x1, 255);
		g::local_player->SetAngle2(OrigAng);
	}

	float damage_for_armor(float damage, int armor_value) {
		float armor_ratio = 0.5f;
		float armor_bonus = 0.5f;
		if (armor_value > 0) {
			float armor_new = damage * armor_ratio;
			float armor = (damage - armor_new) * armor_bonus;

			if (armor > static_cast<float>(armor_value)) {
				armor = static_cast<float>(armor_value) * (1.f / armor_bonus);
				armor_new = damage - armor;
			}

			damage = armor_new;
		}
		return damage;
	}

	void SpreadCircle()
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return;

		c_base_combat_weapon* weapon = g::local_player->m_hActiveWeapon().Get();

		if (!weapon)
			return;

		float spread = weapon->GetInaccuracy() * 1000;

		if (spread == 0.f)
			return;

		int x, y;
		g::engine_client->GetScreenSize(x, y);
		float cx = x / 2.f;
		float cy = y / 2.f;

		globals::draw_list->AddCircle(ImVec2(cx, cy), spread, ImGui::GetColorU32(settings::visuals::spread_cross_color), 255);
		globals::draw_list->AddCircleFilled(ImVec2(cx, cy), spread - 1, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.2f }), 255);
	}

	void glow() noexcept {
		auto local_player = reinterpret_cast<c_base_player*>(g::entity_list->GetClientEntity(g::engine_client->GetLocalPlayer()));
		if (!local_player)
			return;

		for (size_t i = 0; i < g::glow_manager->size; i++) {
			auto& glow = g::glow_manager->objects[i];
			if (glow.unused())
				continue;

			auto glow_entity = reinterpret_cast<c_base_player*>(glow.entity);
			auto client_class = glow_entity->GetClientClass();
			if (!glow_entity || glow_entity->IsDormant())
				continue;

			auto is_enemy = glow_entity->m_iTeamNum() != local_player->m_iTeamNum();
			auto is_teammate = glow_entity->m_iTeamNum() == local_player->m_iTeamNum();

			switch (client_class->m_ClassID) {
			case EClassId::CCSPlayer:
				if (is_enemy && settings::glow::glowEnemyEnabled) {
					glow.set(settings::glow::glowEnemyColor.r() / 255.f, settings::glow::glowEnemyColor.g() / 255.f, settings::glow::glowEnemyColor.b() / 255.f, settings::glow::glowEnemyColor.a() / 255.f, settings::glow::style_enemy);
				}
				else if (is_teammate && settings::glow::glowTeamEnabled) {
					glow.set(settings::glow::glowTeamColor.r() / 255.f, settings::glow::glowTeamColor.g() / 255.f, settings::glow::glowTeamColor.b() / 255.f, settings::glow::glowTeamColor.a() / 255.f, settings::glow::style_teammate);
				}
				break;
			case EClassId::CPlantedC4:
			case EClassId::CBaseAnimating:
				if (settings::glow::glowC4PlantedEnabled) {
					glow.set(settings::glow::glowC4PlantedColor.r() / 255.f, settings::glow::glowC4PlantedColor.g() / 255.f, settings::glow::glowC4PlantedColor.b() / 255.f, settings::glow::glowC4PlantedColor.a() / 255.f);
				}
				break;
			case EClassId::CHEGrenade:
			case EClassId::CFlashbang:
			case EClassId::CMolotovGrenade:
			case EClassId::CMolotovProjectile:
			case EClassId::CIncendiaryGrenade:
			case EClassId::CDecoyGrenade:
			case EClassId::CDecoyProjectile:
			case EClassId::CSmokeGrenade:
			case EClassId::CSmokeGrenadeProjectile:
			case EClassId::ParticleSmokeGrenade:
			case EClassId::CBaseCSGrenade:
			case EClassId::CBaseCSGrenadeProjectile:
			case EClassId::CBaseGrenade:
			case EClassId::CBaseParticleEntity:
			case EClassId::CSensorGrenade:
			case EClassId::CSensorGrenadeProjectile:
				if (settings::glow::glowNadesEnabled && !settings::glow::glowOverride) {
					glow.set(settings::glow::glowNadesColor.r() / 255.f, settings::glow::glowNadesColor.g() / 255.f, settings::glow::glowNadesColor.b() / 255.f, settings::glow::glowNadesColor.a() / 255.f);
				}
				break;
			}

			if (client_class->m_ClassID == CAK47 || client_class->m_ClassID == CDEagle || client_class->m_ClassID == CC4 ||
				client_class->m_ClassID >= CWeaponAug && client_class->m_ClassID <= CWeaponXM1014) {
				if (settings::glow::glowDroppedWeaponsEnabled) {
					glow.set(settings::glow::glowDroppedWeaponsColor.r() / 255.f, settings::glow::glowDroppedWeaponsColor.g() / 255.f, settings::glow::glowDroppedWeaponsColor.b() / 255.f, settings::glow::glowDroppedWeaponsColor.a() / 255.f);
				}
			}

			if (client_class->m_ClassID == CEconEntity) {
				if (settings::glow::glowDroppedKitsEnabled) {
					glow.set(settings::glow::glowDroppedKitsColor.r() / 255.f, settings::glow::glowDroppedKitsColor.g() / 255.f, settings::glow::glowDroppedKitsColor.b() / 255.f, settings::glow::glowDroppedKitsColor.a() / 255.f);
				}
			}
		}
	}

	void fetch_entities()
	{
		render_mutex.lock();

		entities.clear();

		if (!is_enabled())
		{
			render_mutex.unlock();
			return;
		}

		for (auto i = 1; i <= g::entity_list->GetHighestEntityIndex(); ++i)
		{
			auto* entity = c_base_player::GetPlayerByIndex(i);

			if (!entity || entity->IsPlayer() || entity->is_dormant() || entity == g::local_player)
				continue;

			const auto classid = entity->GetClientClass()->m_ClassID;
			if (settings::visuals::world_grenades && (classid == 9 || classid == 134 || classid == 111 || classid == 113 || classid == 156 || classid == 48)) //9 = HE,113 = molly,156 = smoke,48 = decoy
				world_grenades(entity);
			else if (settings::visuals::planted_c4 && entity->IsPlantedC4())
				push_entity(entity, "Bomb", "", false, Color::Yellow);
			else if (settings::visuals::dropped_weapons && entity->IsWeapon() && !entity->m_hOwnerEntity().IsValid())
				push_entity(entity, utils::get_weapon_name(entity), "", false, Color::White);
		}

		render_mutex.unlock();
	}

	void render(ImDrawList* draw_list)
	{
		if (!is_enabled() || !render::fonts::visuals || g::engine_client->IsConsoleVisible())
			return;

		if (render_mutex.try_lock())
		{
			saved_entities = entities;
			render_mutex.unlock();
		}

		ImGui::PushFont(render::fonts::visuals);

		Vector origin;
		for (const auto& entity : saved_entities)
		{
			if (math::world2screen(entity.origin, origin))
			{
				const auto text_size = ImGui::CalcTextSize(entity.text.c_str());
				imdraw::outlined_text(entity.text.c_str(), ImVec2(origin.x - text_size.x / 2.f, origin.y + 7.f), utils::to_im32(entity.color));

				const auto text_size2 = ImGui::CalcTextSize(entity.text2.c_str());
				imdraw::outlined_text(entity.text2.c_str(), ImVec2(origin.x - text_size2.x / 2.f, origin.y + 15.f), utils::to_im32(Color::White));

				if (entity.is_grenade)
					draw_list->AddRect(ImVec2(origin.x + 7.f, origin.y + 7.f), ImVec2(origin.x - 7.f, origin.y - 7.f), utils::to_im32(entity.color));
			}
		}

		if (settings::visuals::rcs_cross)
			RenderPunchCross();

		if (settings::esp::drawFov)
			DrawFov();

		if (settings::visuals::hitmarker)
			RenderHitmarker();

		if (settings::misc::noscope)
			RenderNoScopeOverlay();

		if (settings::visuals::spread_cross)
			SpreadCircle();

		if (settings::desync::enabled)
			AAIndicator();

		if (settings::misc::damage_indicator)
			DrawDamageIndicator();

		ImGui::PopFont();
	}
}