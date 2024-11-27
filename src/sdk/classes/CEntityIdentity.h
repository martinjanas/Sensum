#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/helpers/CUtlSymbolLarge.hpp"

class CEntityIdentity
{
public:
    NETVAR(const char*, m_name, "CEntityIdentity", "m_name");
    NETVAR(const char*, m_designerName, "CEntityIdentity", "m_designerName");
    NETVAR(uint32_t, m_flags, "CEntityIdentity", "m_flags");

    uint32_t GetHandleIndex()
    {
        return reinterpret_cast<uint32_t>(this + 0x10);
    }

    [[nodiscard]] bool IsValid()
    {
        return GetHandleIndex() != 0xFFFFFFFF;
    }

    [[nodiscard]] int GetEntityIndex() //this should be the entity's index
    {
        if (!IsValid())
            return 0x7FFF;

        return GetHandleIndex() & 0x7FFF;
    }

    [[nodiscard]] int GetSerialNumber()
    {
        return GetHandleIndex() >> 15;
    }
};
