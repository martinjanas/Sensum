#pragma once
#define NOMINMAX
#include <array>

#include "../helpers/CUtlTSHash.h"
#include "../helpers/modules.h"
#include "../helpers/vfunc.h"

#define PAD(name, size)          \
    char name[size] = {}; \

struct SchemaClassInfoData_t;

struct CSchemaVarName 
{
    const char* m_pszName;
    const char* m_pszType;
};

struct CSchemaNetworkValue 
{
    union {
        const char* m_pszValue;
        int m_nValue;
        float m_fValue;
        std::uintptr_t m_pPointer;
        CSchemaVarName m_VarValue;
        std::array<char, 32> m_szValue;
    };
};

struct SchemaMetadataEntryData_t 
{
    const char* m_szName;
    CSchemaNetworkValue* m_pNetworkValue;
};


struct SchemaClassFieldData_t 
{
    const char* m_pszName; // 0x0000
    void* m_pSchemaType; // 0x0008
    std::int32_t m_nSingleInheritanceOffset; // 0x0010
    std::int32_t m_nMetadataSize; // 0x0014
    SchemaMetadataEntryData_t* m_pMetadata; // 0x0018
};

struct SchemaBaseClassInfoData_t 
{
    std::uint32_t m_unOffset; // 0x0000
    SchemaClassInfoData_t* m_pPrevByClass; // 0x0008
};

/*struct SchemaClassInfoData_t
{
public:
    PAD(pad01, 0x8);
    const char* m_pszName; // 0x0008
    PAD(pad02, 12);
    std::int16_t m_nFieldSize; // 0x001C
    PAD(pad03, 3);
    std::int8_t m_nBaseClassSize; // 0x0023
    PAD(pad04, 4);
    SchemaClassFieldData_t* m_pFields; // 0x0028
    SchemaBaseClassInfoData_t* m_pBaseClasses; // 0x0038
};
static_assert(sizeof(SchemaClassFieldData_t) == 0x20);*/

class CSchemaSystemTypeScope
{
public:
    SchemaClassInfoData_t* FindDeclaredClass(const std::string_view szName)
    {
        SchemaClassInfoData_t* class_info;
        VTable::GetThiscall<void>(this, 2, &class_info, szName.data());

        return class_info;
    }

    CUtlTSHash<SchemaClassInfoData_t*> GetClassBindings() const
    {
        return m_ClassBindings;
    }
private:
    PAD(pad01, 1376);

    //Update 13.02.2026: m_ClassBindings is now at 0x560 or 1376 in decimal
    CUtlTSHash<SchemaClassInfoData_t*> m_ClassBindings = {}; // 0x560 or 1376
};

struct SchemaClassInfoData_t
{
public:
    SchemaClassInfoData_t* m_pSelf; // 0x0000
    const char* m_pszName; // 0x0008
    const char* m_pszModule; // 0x0010
    const char* m_pszName2; // 0x0018

    int m_nSizeOf; // 0x0018

    std::int16_t m_nFieldSize; // 0x001C

    std::int16_t m_nStaticMetadataSize; // 0x0020
    std::uint8_t m_unAlignOf; // 0x0022

    std::int8_t m_nBaseClassSize; // 0x0023

    // @note: @og: if there is no derived or base class, then it will be 1 otherwise derived class size + 1.
    std::int16_t m_nMultipleInheritanceDepth; // 0x0024
    std::int16_t m_nSingleInheritanceDepth; // 0x0026

    SchemaClassFieldData_t* m_pFields; // 0x0028

    SchemaBaseClassInfoData_t* m_pBaseClasses; // 0x0038
    void* m_pFieldMetadataOverrides; // 0x0040
    SchemaMetadataEntryData_t* m_pStaticMetadata; // 0x0048
    CSchemaSystemTypeScope* m_pTypeScope; // 0x0050

    void* m_pSchemaType; // 0x0058
    enum SchemaClassFlags_t m_nClassFlags : 32; // 0x0060

    std::uint32_t m_unSequence; // 0x0064 // @note: @og: idk
    void* m_pFn; // 0x0068
};
static_assert(offsetof(SchemaClassInfoData_t, m_pFn) == 0x68, "Offset of m_pFn should be 0x68");


class CSchemaSystem 
{
public:
    //@ref 13.02.26: schemasystem.dll @ "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B DA"
    CSchemaSystemTypeScope* FindTypeScopeForModule(const std::string_view pszModuleName)
    {
        return VTable::GetThiscall<CSchemaSystemTypeScope*>(this, 13, pszModuleName.data(), nullptr);
    }
};
