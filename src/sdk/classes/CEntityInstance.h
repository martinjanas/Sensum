#pragma once
#include "../../sdk/classes/CEntityIdentity.h"
#include "../../sdk/interfaces/SchemaSystem.h"
//#include "../../sdk/classes/CHandle.h"

class CEntityInstance
{
public:
    NETVAR(CEntityIdentity*, m_pEntity, "CEntityInstance", "m_pEntity");

    SchemaClassInfoData_t* GetSchemaClassInfo()
    {
        SchemaClassInfoData_t* pClassInfo = nullptr;
        VTable::GetThiscall<void>(this, 38, &pClassInfo);

        return pClassInfo;
    }

    template <int class_hierarchy_level = 1>
    fnv::hash GetClassNameHash()
    {
        const auto& class_info = GetSchemaClassInfo();
        if (!class_info)
            return 0;

        const auto* current_info = class_info;
        for (int current_level = 1; current_level <= class_hierarchy_level; ++current_level)
        {
            if (!current_info || !current_info->m_bHasBaseClass)
                return 0;

            if (current_level == class_hierarchy_level)
                return fnv::hash_runtime(current_info->m_pszName);

            current_info = current_info->m_pBaseClasses ? current_info->m_pBaseClasses->m_pPrevByClass : nullptr;
        }

        return 0;
    }

    bool IsController()
    {
        //CCSPlayerController : CBasePlayerController : C_BaseEntity
        const auto& class_two_hash = GetClassNameHash<2>();
        return class_two_hash == FNV("CBasePlayerController");
    }

    bool IsPawn()
    {
        //C_CSPlayerPawn : C_CSPlayerPawnBase : C_BasePlayerPawn  
        const auto& class_three_hash = GetClassNameHash<3>();
        return class_three_hash == FNV("C_BasePlayerPawn");
    }

    bool IsPlantedC4()
    {
        const auto& class_one_hash = GetClassNameHash();
        return class_one_hash == FNV("C_PlantedC4");
    }

    bool IsGrenadeProjectile()
    {
        //C_SmokeGrenadeProjectile : C_BaseCSGrenadeProjectile
        const auto& class_two_hash = GetClassNameHash<2>();
        return class_two_hash == FNV("C_BaseCSGrenadeProjectile");
    }

    bool IsSmokeProjectile()
    {
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == FNV("C_SmokeGrenadeProjectile");
    }

    bool IsMolotovProjectile()
    {
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == FNV("C_MolotovProjectile");
    }
    
    bool IsFlashProjectile()
    {
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == FNV("C_FlashbangProjectile");
    }

    bool IsHEProjectile()
    {
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == FNV("C_HEGrenadeProjectile");
    }

    bool IsDecoyProjectile()
    {
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == FNV("C_DecoyProjectile");
    }
    
    //returns dropped weapon or grenade entities in the world
    bool IsDroppedWorldEntity()
    {
        //C_AK47 : C_CSWeaponBaseGun : C_CSWeaponBase
        //C_C4 : C_CSWeaponBase
        const auto& class_three_hash = GetClassNameHash<3>(); //the inheritance level for guns and grenades
        const auto& class_two_hash = GetClassNameHash<2>(); //inheritance level for C4 on the ground
        return class_three_hash == FNV("C_CSWeaponBase") || class_two_hash == FNV("C_CSWeaponBase");
    }
};
