#include "CBasePlayerWeapon.h"

CCSWeaponBaseVData* CBasePlayerWeapon::GetWeaponData() //crashing?
{
    uintptr_t baseAddr = reinterpret_cast<uintptr_t>(this->m_nSubclassID());
    if (!baseAddr)
        return nullptr;

    uintptr_t* addr = reinterpret_cast<uintptr_t*>(baseAddr + 0x8);

    return reinterpret_cast<CCSWeaponBaseVData*>(addr);
}
