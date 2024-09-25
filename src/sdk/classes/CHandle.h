#pragma once
#include <stdint.h>

class CHandle
{
public:
    CHandle()
    {
        entity_index = 0xFFFFFFFF;
    }

    CHandle(const int nEntry, const int nSerial) noexcept
    {
        entity_index = nEntry | (nSerial << 15);
    }

    bool operator==(const CHandle& rhs) const
    {
        return entity_index == rhs.entity_index;
    }

    bool IsValid() const;
    int GetEntryIndex() const;
    int GetSerialNumber() const;

    uint32_t entity_index;
};

