#pragma once
#include "../../sdk/classes/CEntityIdentity.h"
#include "../../sdk/interfaces/SchemaSystem.h"
//#include "../../sdk/classes/CHandle.h"

class CEntityInstance
{
public:
    CEntityIdentity* m_pEntity()
    {
        return reinterpret_cast<CEntityIdentity*>(reinterpret_cast<uintptr_t>(this) + 0x10);
    }

    SchemaClassInfoData_t* GetSchemaClassInfo()
    {
        SchemaClassInfoData_t* pClassInfo = nullptr;
        GetVirtual<void(__thiscall*)(void*, SchemaClassInfoData_t**)>(this, 34)(this, &pClassInfo);

        return pClassInfo;
    }

    //C_CSPlayerPawn: C_CSPlayerPawnBase: C_BasePlayerPawn
    //CCSPlayerController : CBasePlayerController: C_BaseEntity

    bool IsController()
    {
        const auto& class_info = GetSchemaClassInfo();

        if (!class_info)
            return false;

        if (!class_info->m_bHasBaseClass)
            return false;

        auto base_classes = class_info->m_pBaseClassses;

        if (!base_classes)
            return false;

        auto prev_class = base_classes->m_pPrevByClass;

        if (!prev_class)
            return false;

        auto prev_base_classes = prev_class->m_pBaseClassses;

        if (!prev_base_classes)
            return false;

        auto class_name_hash = fnv::hash_runtime(class_info->m_pszName);
        auto prev_class_hash = fnv::hash_runtime(base_classes->m_pPrevByClass->GetName().data());
        auto prev_prev_hash = fnv::hash_runtime(prev_base_classes->m_pPrevByClass->GetName().data());

        return class_name_hash == FNV("CCSPlayerController") && prev_class_hash == FNV("CBasePlayerController") && prev_prev_hash == FNV("C_BaseEntity");
    }

    bool IsPawn()
    {
        const auto& class_info = GetSchemaClassInfo();

        if (!class_info)
            return false;

        if (!class_info->m_bHasBaseClass)
            return false;

        auto base_classes = class_info->m_pBaseClassses;

        if (!base_classes)
            return false;

        auto prev_class = base_classes->m_pPrevByClass;

        if (!prev_class)
            return false;

        auto prev_base_classes = prev_class->m_pBaseClassses;

        if (!prev_base_classes)
            return false;

        auto class_name_hash = fnv::hash_runtime(class_info->m_pszName);
        auto prev_class_hash = fnv::hash_runtime(base_classes->m_pPrevByClass->GetName().data());
        auto prev_prev_hash = fnv::hash_runtime(prev_base_classes->m_pPrevByClass->GetName().data());

        return class_name_hash == FNV("C_CSPlayerPawn") && prev_class_hash == FNV("C_CSPlayerPawnBase") && prev_prev_hash == FNV("C_BasePlayerPawn");
    }

   /* bool InheritsFrom(const std::string_view& name)
    {
        const auto& class_info = GetSchemaClassInfo();

        if (!class_info)
            return false;

        if (!class_info->m_has_base_class)
            return false;

        auto base_classes = class_info->m_base_classes;

        if (!base_classes)
            return false;

        if (base_classes->m_prev_by_class->GetName() == name)
            return true;
    }*/
};
