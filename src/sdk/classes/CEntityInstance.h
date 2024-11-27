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

    enum EInheritanceLevel : uint64_t
    {
        ELevelOne = 1,
        ELevelTwo,
        ELevelThree,
        ELevelFour,
        ELevelFive
    };

    template <EInheritanceLevel level = ELevelOne>
    fnv::hash GetClassNameHash()
    {
        const auto& class_info = GetSchemaClassInfo();
        if (!class_info)
            return 0;

        if (level == ELevelOne)
        {
            auto hash = fnv::hash_runtime(class_info->m_pszName);

            return hash;
        }

        if (!class_info->m_bHasBaseClass)
            return 0;

        const auto& level_two_info = class_info->m_pBaseClasses->m_pPrevByClass;
        if (!level_two_info)
            return 0;

        if (level == ELevelTwo)
        {
            auto hash = fnv::hash_runtime(level_two_info->m_pszName);

            return hash;
        }

        if (!level_two_info->m_bHasBaseClass)
            return 0;

        const auto& level_three_info = level_two_info->m_pBaseClasses->m_pPrevByClass;
        if (!level_three_info)
            return 0;

        if (level == ELevelThree)
        {
            auto hash = fnv::hash_runtime(level_three_info->m_pszName);

            return hash;
        }

        if (!level_three_info->m_bHasBaseClass)
            return 0;

        const auto& level_four_info = level_three_info->m_pBaseClasses->m_pPrevByClass;
        if (!level_four_info)
            return 0;

        if (level == ELevelFour)
        {
            auto hash = fnv::hash_runtime(level_four_info->m_pszName);

            return hash;
        }

        if (!level_four_info->m_bHasBaseClass)
            return 0;

        const auto& level_five_info = level_four_info->m_pBaseClasses->m_pPrevByClass;
        if (!level_five_info)
            return 0;

        if (level == ELevelFive)
        {
            auto hash = fnv::hash_runtime(level_five_info->m_pszName);

            return hash;
        }
    }

    bool IsController()
    {
        //CCSPlayerController : CBasePlayerController : C_BaseEntity
        const auto& class_two_hash = GetClassNameHash<ELevelTwo>();
        return class_two_hash == FNV("CBasePlayerController");
    }

    bool IsPawn()
    {
        //C_CSPlayerPawn : C_CSPlayerPawnBase : C_BasePlayerPawn  
        const auto& class_three_hash = GetClassNameHash<ELevelThree>();
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
        const auto& class_two_hash = GetClassNameHash<ELevelTwo>();
        return class_two_hash == FNV("C_BaseCSGrenadeProjectile");
    }

    //returns dropped weapon or grenade entities in the world
    bool IsDroppedWorldEntity()
    {
        //C_AK47 : C_CSWeaponBaseGun : C_CSWeaponBase
        //C_C4 : C_CSWeaponBase
        const auto& class_three_hash = GetClassNameHash<ELevelThree>(); //the inheritance level for guns and grenades
        const auto& class_two_hash = GetClassNameHash<ELevelTwo>(); //inheritance level for C4 on the ground
        return class_three_hash == FNV("C_CSWeaponBase") || class_two_hash == FNV("C_CSWeaponBase");
    }
};
