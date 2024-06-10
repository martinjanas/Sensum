#pragma once
#include "../sdk.h"
#include "../../sdk/classes/entities/CBaseEntity.h"

class CHandle
{
public:
    bool operator==(CHandle rhs) const
    {
        return entity_index == rhs.entity_index;
    }

    bool IsValid() const
    {
        return entity_index != 0xFFFFFFFF;
    }

    uint32_t GetIndex() const
    {
        return entity_index & 0x7FFF;
    }

    template <typename T = CBaseEntity*>
    T Get() const
    {
        auto base_entity = g::entity_system->GetBaseEntity(GetIndex());

        return reinterpret_cast<T>(base_entity);
    }

    uint32_t entity_index;
};