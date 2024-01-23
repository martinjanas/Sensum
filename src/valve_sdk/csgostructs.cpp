#include "csgostructs.hpp"
#include "../helpers/math.h"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../settings/globals.h"
#include "../helpers/entities.h"
#include "../features/features.h"
#include "classids.h"
#include "../settings/globals.h"

std::vector<int> c_base_player::hitboxes()
{
	std::vector<int> list;
	//list.clear();

	auto a_settings = &settings::aimbot::m_items[g::local_player->m_hActiveWeapon()->m_iItemDefinitionIndex()];

	if (a_settings->hitboxes.head)
		list.emplace_back(HITBOX_HEAD);

	if (a_settings->hitboxes.neck)
		list.emplace_back(HITBOX_NECK);

	if (a_settings->hitboxes.body)
	{
		list.emplace_back(HITBOX_UPPER_CHEST);
		list.emplace_back(HITBOX_LOWER_CHEST);
		list.emplace_back(HITBOX_THORAX);
		list.emplace_back(HITBOX_BELLY);
		list.emplace_back(HITBOX_PELVIS);
	}

	if (a_settings->hitboxes.arms)
	{
		list.emplace_back(HITBOX_RIGHT_UPPER_ARM);
		list.emplace_back(HITBOX_RIGHT_FOREARM);
		list.emplace_back(HITBOX_LEFT_UPPER_ARM);
		list.emplace_back(HITBOX_LEFT_FOREARM);
	}

	if (a_settings->hitboxes.legs)
	{
		list.emplace_back(HITBOX_RIGHT_THIGH);
		list.emplace_back(HITBOX_LEFT_THIGH);
		list.emplace_back(HITBOX_RIGHT_CALF);
		list.emplace_back(HITBOX_LEFT_CALF);
	}

	if (InAir())
	{
		list.pop_back();

		list.emplace_back(HITBOX_UPPER_CHEST);
		list.emplace_back(HITBOX_LOWER_CHEST);
		list.emplace_back(HITBOX_THORAX);
		list.emplace_back(HITBOX_BELLY);
		list.emplace_back(HITBOX_PELVIS);

		list.emplace_back(HITBOX_RIGHT_UPPER_ARM);
		list.emplace_back(HITBOX_RIGHT_FOREARM);
		list.emplace_back(HITBOX_LEFT_UPPER_ARM);
		list.emplace_back(HITBOX_LEFT_FOREARM);

		list.emplace_back(HITBOX_RIGHT_THIGH);
		list.emplace_back(HITBOX_LEFT_THIGH);
		list.emplace_back(HITBOX_RIGHT_CALF);
		list.emplace_back(HITBOX_LEFT_CALF);
	}
	else
	{
		if (a_settings->hitboxes.head)
			list.emplace_back(HITBOX_HEAD);

		if (a_settings->hitboxes.neck)
			list.emplace_back(HITBOX_NECK);

		if (a_settings->hitboxes.body)
		{
			list.emplace_back(HITBOX_UPPER_CHEST);
			list.emplace_back(HITBOX_LOWER_CHEST);
			list.emplace_back(HITBOX_THORAX);
			list.emplace_back(HITBOX_BELLY);
			list.emplace_back(HITBOX_PELVIS);
		}

		if (a_settings->hitboxes.legs)
		{
			list.emplace_back(HITBOX_RIGHT_THIGH);
			list.emplace_back(HITBOX_LEFT_THIGH);
			list.emplace_back(HITBOX_RIGHT_CALF);
			list.emplace_back(HITBOX_LEFT_CALF);
		}

		if (a_settings->hitboxes.arms)
		{
			list.emplace_back(HITBOX_RIGHT_UPPER_ARM);
			list.emplace_back(HITBOX_RIGHT_FOREARM);
			list.emplace_back(HITBOX_LEFT_UPPER_ARM);
			list.emplace_back(HITBOX_LEFT_FOREARM);
		}
	}

	if (a_settings->rcs_override_hitbox)
	{
		if (g::local_player->m_iShotsFired() >= 3)
		{
			list.pop_back();

			list.emplace_back(HITBOX_UPPER_CHEST);
			list.emplace_back(HITBOX_LOWER_CHEST);
			list.emplace_back(HITBOX_THORAX);
			list.emplace_back(HITBOX_BELLY);
			list.emplace_back(HITBOX_PELVIS);
		}
		else if (g::local_player->m_iShotsFired() < 3)
		{
			list.pop_back();

			if (a_settings->hitboxes.head)
				list.emplace_back(HITBOX_HEAD);

			if (a_settings->hitboxes.neck)
				list.emplace_back(HITBOX_NECK);

			if (a_settings->hitboxes.body)
			{
				list.emplace_back(HITBOX_UPPER_CHEST);
				list.emplace_back(HITBOX_LOWER_CHEST);
				list.emplace_back(HITBOX_THORAX);
				list.emplace_back(HITBOX_BELLY);
				list.emplace_back(HITBOX_PELVIS);
			}

			if (a_settings->hitboxes.legs)
			{
				list.emplace_back(HITBOX_RIGHT_THIGH);
				list.emplace_back(HITBOX_LEFT_THIGH);
				list.emplace_back(HITBOX_RIGHT_CALF);
				list.emplace_back(HITBOX_LEFT_CALF);
			}

			if (a_settings->hitboxes.arms)
			{
				list.emplace_back(HITBOX_RIGHT_UPPER_ARM);
				list.emplace_back(HITBOX_RIGHT_FOREARM);
				list.emplace_back(HITBOX_LEFT_UPPER_ARM);
				list.emplace_back(HITBOX_LEFT_FOREARM);
			}
		}
	}

	return list;
}

int c_base_entity::GetSequenceActivity(studiohdr_t* hdr, const int& sequence)
{
	if (!patterns::get_sequence_activity_addr)
	{
		console::print(xorstr_("patterns::get_sequence_activity_addr - nullptr.\n"));

		return -1;
	}

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(patterns::get_sequence_activity_addr);

	return get_sequence_activity(this, hdr, sequence);
}

void c_base_entity::UpdateVisibilityAllEntities()
{
	if (!patterns::update_visibility_entities_addr)
	{
		console::print(xorstr_("patterns::update_visibility_entities_addr - nullptr.\n"));

		return;
	}

	static auto fn_offset = reinterpret_cast<void(__thiscall*)(void*)>(patterns::update_visibility_entities_addr);

	fn_offset(this);
}

const matrix3x4_t& c_base_entity::m_rgflCoordinateFrame()
{
	return *(matrix3x4_t*)(uintptr_t(this) + 0x444);
}

Vector& c_base_entity::GetAbsOrigin()
{
	return CallVFunction<Vector& (__thiscall*)(void*)>(this, 10)(this);
}

void c_base_entity::SetAbsOrigin(const Vector& origin)
{
	if (!patterns::set_abs_origin_addr)
	{
		console::print(xorstr_("patterns::set_abs_origin_addr - nullptr.\n"));

		return;
	}

	static auto set_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(patterns::set_abs_origin_addr);

	set_origin_fn(this, origin);
}

QAngle& c_base_player::GetAbsAngles()
{
	return CallVFunction<QAngle&(__thiscall*)(void*)>(this, 11)(this);
}

void c_base_player::SetAbsAngles(const QAngle& angles)
{
	if (!patterns::set_abs_angle_addr)
	{
		console::print(xorstr_("patterns::set_abs_angle_addr - nullptr.\n"));

		return;
	}

	static auto set_angle_fn = reinterpret_cast<void(__thiscall*)(void*, const QAngle&)>(patterns::set_abs_angle_addr);

	set_angle_fn(this, angles);
}

float c_base_combat_weapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 483)(this);
}

float c_base_combat_weapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 453)(this);
}

void c_base_combat_weapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 484)(this);
}

CAnimationLayer* c_base_player::GetAnimOverlays()
{
	return *reinterpret_cast<CAnimationLayer**>(reinterpret_cast<DWORD>(this) + 0x2990);
}

CAnimationLayer* c_base_player::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];

	return nullptr;
}

CAnimState* c_base_player::GetPlayerAnimState()
{
	return *reinterpret_cast<CAnimState**>(reinterpret_cast<DWORD>(this) + 0x9960);
}

void c_base_player::ResetAnimationState(CAnimState* state) 
{
	using ResetAnimState_t = void(__thiscall*)(CAnimState*);

	static auto ResetAnimState = (ResetAnimState_t)utils::pattern_scan(("client.dll"), "56 6A 01 68 ? ? ? ? 8B F1");

	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void c_base_player::InvalidateBoneCache()
{
	if (!patterns::invalidate_bonecache_addr)
	{
		console::print(xorstr_("patterns::invalidate_bonecache_addr - nullptr.\n"));

		return;
	}

	const auto model_bone_counter = **reinterpret_cast<unsigned long**>(patterns::invalidate_bonecache_addr + 10);
	*reinterpret_cast<unsigned int*>(reinterpret_cast<DWORD>(this) + 0x2928) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*reinterpret_cast<unsigned int*>(reinterpret_cast<DWORD>(this) + 0x2690) = model_bone_counter - 1; // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}

void c_base_player::UpdateClientSideAnimation()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 224)(this);
}

bool c_base_entity::IsPlayer()
{
	return CallVFunction<bool(__thiscall*)(void*)>(this, 158)(this);
}

bool c_base_entity::IsWeapon()
{
	return CallVFunction<bool(__thiscall*)(c_base_entity*)>(this, 166)(this);
}

bool c_base_entity::IsPlantedC4()
{
	return GetClientClass()->m_ClassID == classids::CPlantedC4;
}

bool c_base_entity::IsDefuseKit()
{
	return GetClientClass()->m_ClassID == classids::CBaseAnimating;
}

CCSWeaponInfo* c_base_combat_weapon::GetWeaponData()
{
	return CallVFunction<CCSWeaponInfo* (__thiscall*)(void*)>(this, 461)(this);
}

bool c_base_combat_weapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

char* c_base_combat_weapon::GetGunIcon()
{
	if (!this)
		return (char*)" ";

	switch (this->m_iItemDefinitionIndex())
	{
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
		return (char*)"]";
	case WEAPON_DEAGLE:
		return (char*)"A";
	case WEAPON_ELITE:
		return (char*)"B";
	case WEAPON_FIVESEVEN:
		return (char*)"C";
	case WEAPON_GLOCK:
		return (char*)"D";
	case WEAPON_HKP2000:
		return (char*)"E";
	case WEAPON_P250:
		return (char*)"F";
	case WEAPON_USP_SILENCER:
		return (char*)"G";
	case WEAPON_TEC9:
		return (char*)"H";
	case WEAPON_CZ75A:
		return (char*)"I";
	case WEAPON_REVOLVER:
		return (char*)"J";
	case WEAPON_MAC10:
		return (char*)"K";
	case WEAPON_UMP45:
		return (char*)"L";
	case WEAPON_BIZON:
		return (char*)"M";
	case WEAPON_MP7:
		return (char*)"N";
	case WEAPON_MP5SD:
		return (char*)"N";
	case WEAPON_MP9:
		return (char*)"O";
	case WEAPON_P90:
		return (char*)"P";
	case WEAPON_GALILAR:
		return (char*)"Q";
	case WEAPON_FAMAS:
		return (char*)"R";
	case WEAPON_M4A1:
		return (char*)"S";
	case WEAPON_M4A1_SILENCER:
		return (char*)"T";
	case WEAPON_AUG:
		return (char*)"U";
	case WEAPON_SG556:
		return (char*)"V";
	case WEAPON_AK47:
		return (char*)"W";
	case WEAPON_G3SG1:
		return (char*)"X";
	case WEAPON_SCAR20:
		return (char*)"Y";
	case WEAPON_AWP:
		return (char*)"Z";
	case WEAPON_SSG08:
		return (char*)"a";
	case WEAPON_XM1014:
		return (char*)"b";
	case WEAPON_SAWEDOFF:
		return (char*)"c";
	case WEAPON_MAG7:
		return (char*)"d";
	case WEAPON_NOVA:
		return (char*)"e";
	case WEAPON_NEGEV:
		return (char*)"f";
	case WEAPON_M249:
		return (char*)"g";
	case WEAPON_TASER:
		return (char*)"h";
	case WEAPON_FLASHBANG:
		return (char*)"i";
	case WEAPON_HEGRENADE:
		return (char*)"j";
	case WEAPON_SMOKEGRENADE:
		return (char*)"k";
	case WEAPON_MOLOTOV:
		return (char*)"l";
	case WEAPON_DECOY:
		return (char*)"m";
	case WEAPON_INCGRENADE:
		return (char*)"n";
	case WEAPON_C4:
		return (char*)"o";
	default:
		return (char*)" ";
	}
}

float c_base_combat_weapon::GetGunStringSize()
{
	if (!this)
		return 0.f;

	switch (this->m_iItemDefinitionIndex())
	{
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
		return 26.f;
	case WEAPON_DEAGLE:
		return 21.f;
	case WEAPON_ELITE:
		return 26.f;
	case WEAPON_FIVESEVEN:
		return 19.f;
	case WEAPON_GLOCK:
		return 21.f;
	case WEAPON_HKP2000:
		return 20.f;
	case WEAPON_P250:
		return 18.f;
	case WEAPON_USP_SILENCER:
		return 26.f;
	case WEAPON_TEC9:
		return 21.f;
	case WEAPON_CZ75A:
		return 21.f;
	case WEAPON_REVOLVER:
		return 26.f;
	case WEAPON_MAC10:
		return 21.f;
	case WEAPON_UMP45:
		return 26.f;
	case WEAPON_BIZON:
		return 26.f;
	case WEAPON_MP7:
		return 19.f;
	case WEAPON_MP5SD:
		return 26.f;
	case WEAPON_MP9:
		return 26.f;
	case WEAPON_P90:
		return 26.f;
	case WEAPON_GALILAR:
		return 26.f;
	case WEAPON_FAMAS:
		return 26.f;
	case WEAPON_M4A1:
		return 26.f;
	case WEAPON_M4A1_SILENCER:
		return 31.f;
	case WEAPON_AUG:
		return 26.f;
	case WEAPON_SG556:
		return 26.f;
	case WEAPON_AK47:
		return 26.f;
	case WEAPON_G3SG1:
		return 28.f;
	case WEAPON_SCAR20:
		return 28.f;
	case WEAPON_AWP:
		return 31.f;
	case WEAPON_SSG08:
		return 28.f;
	case WEAPON_XM1014:
		return 28.f;
	case WEAPON_SAWEDOFF:
		return 28.f;
	case WEAPON_MAG7:
		return 28.f;
	case WEAPON_NOVA:
		return 28.f;
	case WEAPON_NEGEV:
		return 28.f;
	case WEAPON_M249:
		return 28.f;
	case WEAPON_TASER:
		return 10.f; //was 18 
	case WEAPON_FLASHBANG:
		return 10.f;
	case WEAPON_HEGRENADE:
		return 10.f;
	case WEAPON_SMOKEGRENADE:
		return 10.f;
	case WEAPON_MOLOTOV:
		return 10.f;
	case WEAPON_DECOY:
		return 10.f;
	case WEAPON_INCGRENADE:
		return 10.f;
	case WEAPON_C4:
		return 10.f;
	default:
		return 10.f;
	}
}

bool c_base_combat_weapon::CanFire()
{
	if (!g::local_player)
		return false;

	static auto stored_tick = 0;
	static decltype(this) stored_weapon = nullptr;
	if (stored_weapon != this || stored_tick >= g::local_player->m_nTickBase())
	{
		stored_weapon = this;
		stored_tick = g::local_player->m_nTickBase();

		return false;
	}

	if (IsReloading() || m_iClip1() <= 0 || !g::local_player)
		return false;

	auto flServerTime = g::local_player->m_nTickBase() * g::global_vars->interval_per_tick;

	return m_flNextPrimaryAttack() <= flServerTime;
}

bool c_base_combat_weapon::RequiresRecoilControl()
{
	//Credits: Mr. Krupinski (DanielKrupinski)
	const auto& weapon_data = GetWeaponData();

	if (weapon_data)
		return weapon_data->flRecoilMagnitude < 35.0f && weapon_data->flRecoveryTimeStand > weapon_data->flCycleTime;

	return false;
}

bool c_base_combat_weapon::IsGrenade()
{
	return GetWeaponData()->WeaponType == WEAPONTYPE_GRENADE;
}

bool c_base_combat_weapon::IsKnifeOrGrenade()
{
	return IsKnife() || IsGrenade() && !IsZeus();
}

bool c_base_combat_weapon::IsZeus()
{
	return m_iItemDefinitionIndex() == WEAPON_TASER;
}

bool c_base_combat_weapon::IsKnife()
{
	return m_iItemDefinitionIndex() != WEAPON_TASER && GetWeaponData()->WeaponType == WEAPONTYPE_KNIFE;
}

bool c_base_combat_weapon::IsPistol()
{
	return GetWeaponData()->WeaponType == WEAPONTYPE_PISTOL;
}

bool c_base_combat_weapon::IsSniper()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_AWP || index == WEAPON_SSG08 || index == WEAPON_SCAR20 || index == WEAPON_G3SG1;

	return GetWeaponData()->WeaponType == WEAPONTYPE_SNIPER_RIFLE;
}

bool c_base_combat_weapon::IsShotgun()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_NOVA || index == WEAPON_XM1014 || index == WEAPON_SAWEDOFF || index == WEAPON_MAG7;

	return GetWeaponData()->WeaponType == WEAPONTYPE_SHOTGUN;
}

bool c_base_combat_weapon::IsSmoke()
{
	if (!this)
		return false;

	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_SMOKEGRENADE;
}

bool c_base_combat_weapon::IsFlash()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_FLASHBANG;
}

Vector c_base_player::get_bone_position(int bone) 
{
	static matrix3x4_t bone_matrices[128];

	if (this->SetupBones(bone_matrices, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g::global_vars->curtime))
		return Vector(bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3]);

	return Vector(0.f, 0.f, 0.f);
}

bool c_base_player::IsEnemy()
{
	static auto game_type = g::cvar->find("game_type");
	
	if (game_type->GetInt() == 6)
	{
		return this->m_nSurvivalTeam() != g::local_player->m_nSurvivalTeam() || g::local_player->m_nSurvivalTeam() == -1;
	}
	
	return this->m_iTeamNum() != g::local_player->m_iTeamNum();
}

bool c_base_combat_weapon::IsReloading()
{
	static auto inReload = *reinterpret_cast<uint32_t*>(patterns::in_reload_addr + 0x2);
	return *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(this) + inReload);
}

Vector c_base_player::GetEyePos()
{
	return m_vecOrigin() + m_vecViewOffset();
}

Vector c_base_player::get_hitbox_position(int hitbox_id) 
{
	static matrix3x4_t bone_matrix[MAXSTUDIOBONES];

	if (this->SetupBones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g::global_vars->curtime)) 
	{
		auto studio_model = g::mdl_info->GetStudiomodel(this->GetModel());

		if (!studio_model)
			return Vector{};

		auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);

		if (!hitbox)
			return Vector{};

		Vector min = Vector{}, max = Vector{};

		math::VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
		math::VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

		return Vector(min.x + max.x, min.y + max.y, min.z + max.z) * 0.5f;
	}

	return Vector{};
}

player_info_t c_base_player::GetPlayerInfo()
{
	return g::engine_client->GetPlayerInfo(EntIndex());
}

bool c_base_player::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool c_base_player::IsDead()
{
	return m_lifeState() == LIFE_DEAD;
}

bool c_base_player::IsDying()
{
	return m_lifeState() == LIFE_DYING;
}

bool c_base_player::IsFlashed()
{
	return m_flFlashMaxAlphaOffset() > 200.0;
}

bool c_base_player::HasC4()
{
	static bool has_c4 = false;

	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		auto my_weapons = this->m_hMyWeapons()[i].Get();

		if (!my_weapons)
			continue;

		if (my_weapons->GetClientClass() && my_weapons->GetClientClass()->m_ClassID == classids::CC4)
		{
			has_c4 = true;

			return true;
		}

		if (has_c4)
			break;
	}

	return false;
}

bool c_base_player::InAir()
{
	return this->m_hGroundEntity().Get() == nullptr;
}

int& c_base_player::m_nMoveType()
{
	return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x25C);
}

void c_base_attributable_item::SetModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

void c_base_view_model::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 247)(this, sequence);
}

void c_base_view_model::SetModelIndex(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, sequence);
}

bool c_base_combat_weapon::check_detonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (m_iItemDefinitionIndex())
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = (int)(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		return (float)tick * interval > 1.5f && !(tick % (int)(0.2f / interval));
	default:
		return false;
	}
}
