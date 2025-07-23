#pragma once
#pragma warning(disable:4191)
#include <cstddef>
#include "../helpers/vfunc.h"

class IMemAlloc : public Interface
{
public:
    IMemAlloc(void* obj, const char* name) : Interface(obj, name) { }

    void* Alloc(std::size_t size) 
    {
        return VTable::GetThiscall<void*>(this->m_obj, 1, size);
    }

    void* ReAlloc(const void* p, std::size_t size) 
    {
        return VTable::GetThiscall<void*>(this->m_obj, 2, p, size);
    }

    void Free(const void* p) 
    {
        VTable::GetThiscall<void>(this->m_obj, 3, p);
    }

    std::size_t GetSize(const void* p) 
    {
        return VTable::GetThiscall<std::size_t>(this->m_obj, 17, p);
    }
};



