#pragma once
#include <string_view>
#include "xxhash.h"

namespace xxh
{
    inline XXH64_hash_t get_hash(std::string_view str)
    {
        return XXH3_64bits(str.data(), str.size());
    }
}
