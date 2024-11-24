#pragma once
#include <utility>

//Example usage:
//GetThiscall<void*>(this, index, optional_arguments);
//GetThiscall<void*>(this, 1, size);

class VTable
{
public:
    template<typename T, typename... Args>
    static T GetThiscall(void* ppClass, int index, Args... args)
    {
        using fn = T(__thiscall*)(void*, Args...);
        auto func = Get<fn>(ppClass, index);

        return func(ppClass, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    static T GetCdecl(void* ppClass, int index, Args... args)
    {
        using fn = T(__cdecl*)(Args...);
        auto func = Get<fn>(ppClass, index);

        return func(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    static T GetStdcall(void* ppClass, int index, Args... args)
    {
        using fn = T(__stdcall*)(Args...);
        auto func = Get<fn>(ppClass, index);

        return func(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    static T GetFastcall(void* ppClass, int index, Args... args)
    {
        using fn = T(__fastcall*)(Args...);
        auto func = Get<fn>(ppClass, index);

        return func(std::forward<Args>(args)...);
    }
private:
    template<typename T>
    static auto Get(void* ppClass, int index)
    {
        auto vtable = *static_cast<void***>(ppClass);
        return reinterpret_cast<T>(vtable[index]);
    }
};