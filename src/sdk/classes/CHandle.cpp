#include "CHandle.h"
#include "../sdk.h"

bool CHandle::IsValid() const
{
    return entity_index != 0xFFFFFFFF;
}

int CHandle::GetEntryIndex() const
{
    return static_cast<int>(entity_index & 0x7FFF);
}

int CHandle::GetSerialNumber() const
{ 
    return static_cast<int>(entity_index >> 15);
}
