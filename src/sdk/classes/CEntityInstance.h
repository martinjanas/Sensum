#pragma once
#include "../../sdk/classes/CEntityIdentity.h"
#include "../../sdk/interfaces/SchemaSystem.h"
#include "../helpers/xxh.h"
//#include "../../sdk/classes/CHandle.h"
#include "../helpers/console.h"

class CEntityInstance
{
public:
    NETVAR(CEntityIdentity*, m_pEntity, "CEntityInstance", "m_pEntity");

    // @ida xref
    /*__int64 __fastcall GetSchemaClassInfo(__int64 a1, __int64 a2)
    {
        __int64 v2; // rbx

        v2 = a2;
        nullsub_423();
        unknown_libname_916(v2, &unk_181B290A0);
        return v2;
    }*/

    //GetSchemaClassInfo is the last function in the CEntityInstance vtable.
    SchemaClassInfoData_t* GetSchemaClassInfo()
    {
        SchemaClassInfoData_t* pClassInfo = nullptr;
        VTable::GetThiscall<void>(this, 47, &pClassInfo);

        return pClassInfo;
    }

    template <int class_hierarchy_level = 1>
    XXH64_hash_t GetClassNameHash()
    {
        const auto& class_info = GetSchemaClassInfo();
        if (!class_info)
            return 0;

        const auto* current_info = class_info;
        for (int current_level = 1; current_level <= class_hierarchy_level; ++current_level)
        {
            if (!current_info || !current_info->m_nBaseClassSize)
                return 0;

            if (current_level == class_hierarchy_level)
                return xxh::get_hash(current_info->m_pszName);

            current_info = current_info->m_pBaseClasses ? current_info->m_pBaseClasses->m_pPrevByClass : nullptr;
        }

        return 0;
    }

    bool IsController()
    {
        static auto class_hash = xxh::get_hash("CBasePlayerController");

        //CCSPlayerController : CBasePlayerController : C_BaseEntity
        const auto& class_two_hash = GetClassNameHash<2>();
        return class_two_hash == class_hash;
    }

    bool IsPawn()
    {
        static auto class_hash = xxh::get_hash("C_BasePlayerPawn");
        
        //C_CSPlayerPawn : C_CSPlayerPawnBase : C_BasePlayerPawn  
        const auto& class_three_hash = GetClassNameHash<3>();
        return class_three_hash == class_hash;
    }

    bool IsPlantedC4()
    {
        static auto class_hash = xxh::get_hash("C_PlantedC4");
        
        const auto& class_one_hash = GetClassNameHash();
        return class_one_hash == class_hash;
    }

    bool IsGrenadeProjectile()
    {
        static auto class_hash = xxh::get_hash("C_BaseCSGrenadeProjectile");
        
        //C_SmokeGrenadeProjectile : C_BaseCSGrenadeProjectile
        const auto& class_two_hash = GetClassNameHash<2>();
        return class_two_hash == class_hash;
    }

    bool IsSmokeProjectile()
    {
        static auto class_hash = xxh::get_hash("C_SmokeGrenadeProjectile");
        
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == class_hash;
    }

    bool IsMolotovProjectile()
    {
        static auto class_hash = xxh::get_hash("C_MolotovProjectile");
        
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == class_hash;
    }
    
    bool IsFlashProjectile()
    {
        static auto class_hash = xxh::get_hash("C_FlashbangProjectile");
        
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == class_hash;
    }

    bool IsHEProjectile()
    {
        static auto class_hash = xxh::get_hash("C_HEGrenadeProjectile");
        
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == class_hash;
    }

    bool IsDecoyProjectile()
    {
        static auto class_hash = xxh::get_hash("C_DecoyProjectile");
        
        const auto& class_one_hash = GetClassNameHash<>();
        return class_one_hash == class_hash;
    }
    
    //returns dropped weapon or grenade entities in the world
    bool IsDroppedWorldEntity() //replace with more precise version?
    {
        static auto hash = xxh::get_hash("C_CSWeaponBase");
        
        //C_AK47 : C_CSWeaponBaseGun : C_CSWeaponBase
        //C_C4 : C_CSWeaponBase
        //C_IncendiaryGrenade : C_MolotovGrenade : C_BaseCSGrenade : C_CSWeaponBase
        
        const auto& class_four_hash = GetClassNameHash<4>(); //inheritance level for Incendiary grenade
        const auto& class_three_hash = GetClassNameHash<3>(); //the inheritance level for guns and grenades
        const auto& class_two_hash = GetClassNameHash<2>(); //inheritance level for C4 on the ground
        return class_four_hash == hash || class_three_hash == hash || class_two_hash == hash;
    }
};
