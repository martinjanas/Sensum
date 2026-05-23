#pragma once
#include <stdint.h>

class CHandle
{
public:
    CHandle()
    {
        entry_index = 0xFFFFFFFF;
    }

    CHandle(const int nEntry, const int nSerial) noexcept
    {
        entry_index = nEntry | (nSerial << 15);
    }

    bool operator==(const CHandle& rhs) const
    {
        return entry_index == rhs.entry_index;
    }

    bool IsValid() const;
    int GetEntityIndex() const;
    int GetSerialNumber() const;

    uint32_t entry_index;
};

