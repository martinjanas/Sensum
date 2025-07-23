#pragma once
#include "impl/Interface.h"
#include "../helpers/vfunc.h"
#include "../helpers/KeyValues.h"
#include <array>
#include "../classes/CHandle.h"

class IMaterial
{
public:
    const char* GetName()
    {
        return VTable::GetThiscall<const char*>(this, 0); //working
    }

    const char* GetShareName()
    {
        return VTable::GetThiscall<const char*>(this, 1); //working aswell
    }
};

class CMaterialDrawDescription
{
public:

};

class CSceneObject
{
public:
    char pad_01[0xB8]; //0x0
    std::array<uint8_t, 4> color; //0xB8
    CHandle owner_handle; //0xBC
}; //Size: 0x0180

class CSceneData
{
public:
    std::byte pad01[0x8];                // 0x0
    CMaterialDrawDescription* mat_desc;	 // 0x8
    CHandle handle;                      // 0x10
    std::byte pad02[0x4];                // 0x14
    CSceneObject* scene_object;          // 0x18
    IMaterial* material;                 // 0x20, - material2 at 0x28
    std::byte pad04[0x28];               // 0x28
    std::array<uint8_t, 4> color;      // 0x50
}; // Size: 0x58

class CMaterialSystem : public Interface
{
public:
    CMaterialSystem(void* obj, const char* name) : Interface(obj, name) { }
	
    IMaterial* FindMaterial(IMaterial*** material, const char* name) //working fine
    {
        return VTable::GetThiscall<IMaterial*>(this->m_obj, 14, material, name);
    }

    IMaterial* CreateMaterial(IMaterial*** material, const char* mat_name, KeyValues* key_value, uint32_t num1, uint8_t num2)
    {
        return VTable::GetThiscall<IMaterial*>(this->m_obj, 29, material, mat_name, key_value, num1, num2);
    }
};
