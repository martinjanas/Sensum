#include "csgostructs.hpp"
#include "../helpers/math.h"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../settings/globals.h"
#include "../helpers/entities.h"
#include "../features/features.h"

int c_base_entity::GetSequenceActivity(const int& sequence)
{
	auto hdr = g::mdl_info->GetStudiomodel(GetModel());
	if (!hdr)
		return -1;

	static const auto offset = utils::pattern_scan(GET_SEQUENCE_ACTIVITY);
	assert(offset);
	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(offset);

	return get_sequence_activity(this, hdr, sequence);
}

int c_base_entity::GetSequenceActivity(studiohdr_t* hdr, const int& sequence)
{
	static const auto offset = utils::pattern_scan(GET_SEQUENCE_ACTIVITY);
	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(offset);

	return get_sequence_activity(this, hdr, sequence);
}

void c_base_entity::UpdateVisibilityAllEntities()
{
	uint8_t* UpdateVisibilityAllEntitiesOffset = nullptr;

	if (!UpdateVisibilityAllEntitiesOffset)
		UpdateVisibilityAllEntitiesOffset = utils::pattern_scan(UPDATE_VISIBILITY_ENTITIES);

	auto fn_offset = reinterpret_cast<void(__thiscall*)(void*)>(UpdateVisibilityAllEntitiesOffset);

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
	static const auto offset = utils::pattern_scan(SET_ABS_ORIGIN);
	if (offset)
	{
		static auto set_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(offset);

		set_origin_fn(this, origin);
	}
}

QAngle& c_base_player::GetAbsAngles()
{
	return CallVFunction<QAngle& (__thiscall*)(void*)>(this, 11)(this);
}

bool c_base_player::IsNotTarget()
{
	if (!this || this == g::local_player)
		return true;

	if (m_iHealth() <= 0)
		return true;

	if (m_bGunGameImmunity())
		return true;

	if (m_fFlags() & FL_FROZEN)
		return true;

	int entIndex = EntIndex();
	return entIndex > g::global_vars->maxClients;
}

void c_base_player::SetAbsAngles(const QAngle& angles)
{
	static const auto offset = utils::pattern_scan(SET_ABS_ANGLE);
	if (offset)
	{
		static auto set_angle_fn = reinterpret_cast<void(__thiscall*)(void*, const QAngle&)>(offset);

		set_angle_fn(this, angles);
	}
}

int c_base_player::GetFOV() {
	if (m_iFOV() != 0)
		return m_iFOV();
	return m_iDefaultFOV();
}

void c_base_player::SetAngle2(QAngle angle)
{
	typedef void(__thiscall* SetAngleFn)(void*, const QAngle&);
	static SetAngleFn SetAngle = (SetAngleFn)((DWORD)utils::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	SetAngle(this, angle);
}

bool c_base_entity::is_dormant()
{
	return GetClientNetworkable()->IsDormant();
}

bool c_base_player::DrawSpecificEntity()
{
	return globals::sound;
}

float c_base_combat_weapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 482)(this);
}

float c_base_combat_weapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 452)(this);
}

const char* c_base_combat_weapon::GetWeaponName()
{
	return CallVFunction<const char* (__thiscall*)(void*)>(this, 385)(this);
}

void c_base_combat_weapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 483)(this);
}

void c_base_player::PVSFix()
{
	*reinterpret_cast<int*>(uintptr_t(this) + 0xA30) = g::global_vars->framecount;
	*reinterpret_cast<int*>(uintptr_t(this) + 0xA28) = 0;
}

CAnimationLayer* c_base_player::GetAnimOverlays()
{
	return *(CAnimationLayer**)(DWORD(this) + 0x2980);
}

CAnimationLayer* c_base_player::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];

	return nullptr;
}

QAngle* c_base_player::GetVAngles2() {
	static auto deadflag = netvar_manager::get().get_offset(fnv::hash_runtime("CBasePlayer->deadflag"));
	return (QAngle*)((uintptr_t)this + deadflag + 0x4);
}

CCSGOPlayerAnimState* c_base_player::GetPlayerAnimState()
{
	return *(CCSGOPlayerAnimState**)((DWORD)this + 0x3914);
}

void c_base_player::ResetAnimationState(CCSGOPlayerAnimState* state) {
	using ResetAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*);
	static auto ResetAnimState = (ResetAnimState_t)utils::pattern_scan(("client.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void c_base_player::InvalidateBoneCache()
{
	static auto invalidate_bone_cache = utils::pattern_scan(INVALIDATE_BONE_CACHE);
	if (invalidate_bone_cache)
	{
		const auto model_bone_counter = **reinterpret_cast<unsigned long**>(invalidate_bone_cache + 10);
		*reinterpret_cast<unsigned int*>(DWORD(this) + 0x2924) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
		*reinterpret_cast<unsigned int*>(DWORD(this) + 0x2690) = model_bone_counter - 1; // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
	}
}

float c_base_player::m_flSpawnTime() {
	return *(float*)((uintptr_t)this + 0xA370);
}

char* c_base_player::GetArmorIcon()
{
	if (m_ArmorValue() > 0)
	{
		if (m_bHasHelmet())
			return "q";
		else
			return "p";
	}
	else
		return " ";
}

void c_base_player::UpdateClientSideAnimation()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 223)(this);
}

bool c_base_entity::IsPlayer()
{
	return CallVFunction<bool(__thiscall*)(void*)>(this, 157)(this);
}

bool c_base_entity::IsWeapon()
{
	return CallVFunction<bool(__thiscall*)(c_base_entity*)>(this, 165)(this);
}

bool c_base_entity::IsPlantedC4()
{
	return GetClientClass()->m_ClassID == CPlantedC4;
}

bool c_base_entity::IsDefuseKit()
{
	return GetClientClass()->m_ClassID == CBaseAnimating;
}

CCSWeaponInfo* c_base_combat_weapon::get_weapon_data()
{
	return CallVFunction<CCSWeaponInfo* (__thiscall*)(void*)>(this, 460)(this);
}

bool c_base_combat_weapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

char* c_base_combat_weapon::GetGunIcon()
{
	if (!this)
		return " ";

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
		return "]";
	case WEAPON_DEAGLE:
		return "A";
	case WEAPON_ELITE:
		return "B";
	case WEAPON_FIVESEVEN:
		return "C";
	case WEAPON_GLOCK:
		return "D";
	case WEAPON_HKP2000:
		return "E";
	case WEAPON_P250:
		return "F";
	case WEAPON_USP_SILENCER:
		return "G";
	case WEAPON_TEC9:
		return "H";
	case WEAPON_CZ75A:
		return "I";
	case WEAPON_REVOLVER:
		return "J";
	case WEAPON_MAC10:
		return "K";
	case WEAPON_UMP45:
		return "L";
	case WEAPON_BIZON:
		return "M";
	case WEAPON_MP7:
		return "N";
	case WEAPON_MP5SD:
		return "N";
	case WEAPON_MP9:
		return "O";
	case WEAPON_P90:
		return "P";
	case WEAPON_GALILAR:
		return "Q";
	case WEAPON_FAMAS:
		return "R";
	case WEAPON_M4A1:
		return "S";
	case WEAPON_M4A1_SILENCER:
		return "T";
	case WEAPON_AUG:
		return "U";
	case WEAPON_SG556:
		return "V";
	case WEAPON_AK47:
		return "W";
	case WEAPON_G3SG1:
		return "X";
	case WEAPON_SCAR20:
		return "Y";
	case WEAPON_AWP:
		return "Z";
	case WEAPON_SSG08:
		return "a";
	case WEAPON_XM1014:
		return "b";
	case WEAPON_SAWEDOFF:
		return "c";
	case WEAPON_MAG7:
		return "d";
	case WEAPON_NOVA:
		return "e";
	case WEAPON_NEGEV:
		return "f";
	case WEAPON_M249:
		return "g";
	case WEAPON_TASER:
		return "h";
	case WEAPON_FLASHBANG:
		return "i";
	case WEAPON_HEGRENADE:
		return "j";
	case WEAPON_SMOKEGRENADE:
		return "k";
	case WEAPON_MOLOTOV:
		return "l";
	case WEAPON_DECOY:
		return "m";
	case WEAPON_INCGRENADE:
		return "n";
	case WEAPON_C4:
		return "o";
	default:
		return " ";
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
		return 18.f;
	case WEAPON_FLASHBANG:
		return 18.f;
	case WEAPON_HEGRENADE:
		return 18.f;
	case WEAPON_SMOKEGRENADE:
		return 18.f;
	case WEAPON_MOLOTOV:
		return 18.f;
	case WEAPON_DECOY:
		return 18.f;
	case WEAPON_INCGRENADE:
		return 18.f;
	case WEAPON_C4:
		return 18.f;
	default:
		return 0.f;
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

bool c_base_combat_weapon::IsGrenade()
{
	return get_weapon_data()->WeaponType == WEAPONTYPE_GRENADE;
}

bool c_base_combat_weapon::IsZeus()
{
	return m_iItemDefinitionIndex() == WEAPON_TASER;
}

bool c_base_combat_weapon::IsGun()
{
	switch (get_weapon_data()->WeaponType)
	{
	case WEAPONTYPE_C4:
	case WEAPONTYPE_GRENADE:
	case WEAPONTYPE_KNIFE:
	case WEAPONTYPE_UNKNOWN:
		return false;
	default:
		return true;
	}
}

bool c_base_combat_weapon::IsKnife()
{
	return m_iItemDefinitionIndex() != WEAPON_TASER && get_weapon_data()->WeaponType == WEAPONTYPE_KNIFE;
}

bool c_base_combat_weapon::IsRifle()
{
	switch (get_weapon_data()->WeaponType)
	{
	case WEAPONTYPE_RIFLE:
	case WEAPONTYPE_SUBMACHINEGUN:
	case WEAPONTYPE_SHOTGUN:
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool c_base_combat_weapon::IsPistol()
{
	return get_weapon_data()->WeaponType == WEAPONTYPE_PISTOL;
}

bool c_base_combat_weapon::IsSniper()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_AWP || index == WEAPON_SSG08 || index == WEAPON_SCAR20 || index == WEAPON_G3SG1;

	return get_weapon_data()->WeaponType == WEAPONTYPE_SNIPER_RIFLE;
}

bool c_base_combat_weapon::IsSmoke()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_SMOKEGRENADE;
}

bool c_base_combat_weapon::IsFlash()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_FLASHBANG;
}

bool c_base_combat_weapon::HasScope()
{
	auto index = m_iItemDefinitionIndex();

	return index == WEAPON_AWP || index == WEAPON_SSG08 || index == WEAPON_SCAR20 || index == WEAPON_G3SG1 || index == WEAPON_AUG || index == WEAPON_SG556;
}

bool c_base_combat_weapon::IsReloading()
{
	static auto inReload = *reinterpret_cast<uint32_t*>(utils::pattern_scan(INRELOAD) + 2);
	return *reinterpret_cast<bool*>(uintptr_t(this) + inReload);
}

CUserCmd*& c_base_player::m_pCurrentCommand()
{
	static auto current_command = *reinterpret_cast<uint32_t*>(utils::pattern_scan(CURRENT_COMMAND) + 2);
	return *reinterpret_cast<CUserCmd**>(uintptr_t(this) + current_command);
}

Vector c_base_player::GetEyePos()
{
	//return CallVFunction<Vector(__thiscall*)(void*)>(this, 279)(this);
	return m_vecOrigin() + m_vecViewOffset();
}

Vector c_base_player::get_hitbox_position(c_base_player* entity, int hitbox_id) {
	matrix3x4_t bone_matrix[MAXSTUDIOBONES];

	if (entity->SetupBones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = g::mdl_info->GetStudiomodel(entity->GetModel());

		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);

			if (hitbox) {
				auto min = Vector{}, max = Vector{};

				math::VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
				math::VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

				return Vector((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}
		}
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

bool c_base_player::IsUnknown()
{
	return !IsPlayer() || is_dormant() || this == g::local_player || m_iHealth() <= 0 || !IsAlive() || m_bGunGameImmunity() || (m_fFlags() & FL_FROZEN);
}

bool c_base_player::IsFlashed()
{
	return m_flFlashMaxAlphaOffset() > 200.0;
}

bool c_base_player::HasC4()
{
	static auto fnHasC4 = reinterpret_cast<bool(__thiscall*)(void*)>(utils::pattern_scan(HAS_C4));

	return fnHasC4(this);
}

bool c_base_player::CanSeePlayer(c_base_player* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(GetEyePos(), pos);
	g::engine_trace->trace_ray(ray, MASK_VISIBLE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

int& c_base_player::m_nMoveType()
{
	return *reinterpret_cast<int*>(uintptr_t(this) + 0x25C);
}

void c_base_attributable_item::SetModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

void c_base_view_model::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 214)(this, sequence);
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