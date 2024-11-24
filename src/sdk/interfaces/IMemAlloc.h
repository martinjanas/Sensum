#pragma once
#pragma warning(disable:4191)
#include <cassert>
#include <cstddef>

#include "../helpers/vfunc.h"

class IMemAlloc 
{
public:
    void* Alloc(std::size_t size) 
    {
        return VTable::GetThiscall<void*>(this, 1, size);
    }

    void* ReAlloc(const void* p, std::size_t size) 
    {
        return VTable::GetThiscall<void*>(this, 2, p, size);
    }

    void Free(const void* p) 
    {
        VTable::GetThiscall<void>(this, 3, p);
    }

    std::size_t GetSize(const void* p) 
    {
        return VTable::GetThiscall<std::size_t>(this, 17, p);
    }
};


