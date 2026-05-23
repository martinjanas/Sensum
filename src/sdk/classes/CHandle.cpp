#include "CHandle.h"
#include "../sdk.h"

bool CHandle::IsValid() const
{
    return entry_index != 0xFFFFFFFF;
}

int CHandle::GetEntityIndex() const
{
    return static_cast<int>(entry_index & 0x7FFF);
}

int CHandle::GetSerialNumber() const
{ 
    return static_cast<int>(entry_index >> 15);
}
