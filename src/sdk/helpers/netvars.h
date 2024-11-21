#pragma once
#include <map>
#include <type_traits>

#include "../../sdk/helpers/fnv.h"

#define NETVAR(type, function, class_name, var_name) \
__forceinline std::add_lvalue_reference_t<type> function() \
{ \
    constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
    static auto offset = netvars::get_offset_by_hash_cached(hash);  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
} \

#define NETVAR_OFFSET(type, function, class_name, var_name, offset) \
__forceinline std::add_lvalue_reference_t<type> function() \
{ \
    constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
    static auto addr = netvars::get_offset_by_hash_cached(hash) + offset;  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + addr); \
} \

#define PNETVAR(type, function, class_name, var_name) \
auto function() \
{ \
    constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
    auto offset = netvars::get_offset_by_hash_cached(hash);  \
    return reinterpret_cast<std::add_pointer_t<type>>((uintptr_t)(this) + offset); \
} \

#define NETVAR_HANDLE(type, function, class_name, var_name) \
__forceinline type function() \
{ \
    constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
    static auto offset = netvars::get_offset_by_hash_cached(hash); \
    auto handle = *reinterpret_cast<CHandle*>(reinterpret_cast<uintptr_t>(this) + offset); \
    return handle.Get<type>(); \
}
namespace netvars
{
    static std::map<fnv::hash, uintptr_t> netvars_data;

    void init();
    uintptr_t get_offset_by_hash_cached(const fnv::hash hash);
};

