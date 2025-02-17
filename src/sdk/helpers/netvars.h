#pragma once
#include <map>
#include <string>
#include <type_traits>
#include "../../sdk/helpers/xxhash.h"

#define NETVAR(type, function, class_name, var_name) \
__forceinline std::add_lvalue_reference_t<type> function() \
{ \
    std::string str = std::string(class_name) + "->" + var_name; \
    auto hash = XXH3_64bits(str.c_str(), str.size()); \
    static auto offset = netvars::get_offset_by_hash_cached(hash);  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
}

#define NETVAR_OFFSET(type, function, class_name, var_name, offset) \
__forceinline std::add_lvalue_reference_t<type> function() \
{ \
    std::string str = std::string(class_name) + "->" + var_name; \
    auto hash = XXH3_64bits(str.c_str(), str.size()); \
    static auto addr = netvars::get_offset_by_hash_cached(hash) + offset;  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + addr); \
}

#define PNETVAR(type, function, class_name, var_name) \
auto function() \
{ \
    std::string str = std::string(class_name) + "->" + var_name; \
    auto hash = XXH3_64bits(str.c_str(), str.size()); \
    auto offset = netvars::get_offset_by_hash_cached(hash);  \
    return reinterpret_cast<std::add_pointer_t<type>>((uintptr_t)(this) + offset); \
}

#define NETVAR_HANDLE(type, function, class_name, var_name) \
__forceinline type function() \
{ \
    std::string str = std::string(class_name) + "->" + var_name; \
    auto hash = XXH3_64bits(str.c_str(), str.size()); \
    static auto offset = netvars::get_offset_by_hash_cached(hash); \
    auto handle = *reinterpret_cast<CHandle*>(reinterpret_cast<uintptr_t>(this) + offset); \
    return handle.Get<type>(); \
}

namespace netvars
{
    static std::map<XXH64_hash_t, uintptr_t> netvars_data;

    void init();
    uintptr_t get_offset_by_hash_cached(const XXH64_hash_t hash);
};

