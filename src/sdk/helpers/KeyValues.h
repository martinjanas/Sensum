#pragma once
#include <cstddef>
#include <cstdint>

#include "modules.h"

struct KV3ID_t
{
    const char* encoding_type; //0x0
    uint64_t key1; //0x8
    uint64_t key2; //0x10

    KV3ID_t(const char* encoding_type, uint64_t key1, uint64_t key2)
    {
        this->encoding_type = encoding_type;
        this->key1 = key1;
        this->key2 = key2;
    }
};

// encodings
const KV3ID_t g_KV3Encoding_Text = { "text", 0x41C58A33E21C7F3C, 0xDAA323A6DA77799 };
const KV3ID_t g_KV3Encoding_Binary = { "binary", 0x40C1F7D81B860500, 0x14E76782A47582AD };
const KV3ID_t g_KV3Encoding_BinaryLZ4 = { "binary_lz4", 0x4F5C63A16847348A, 0x19B1D96F805397A1 };
const KV3ID_t g_KV3Encoding_BinaryZSTD = { "binary_zstd", 0x4305FEF06F620A00, 0x29DBB14623045FA3 };
const KV3ID_t g_KV3Encoding_BinaryBC = { "binary_bc", 0x4F6C95BC95791A46, 0xD2DFB7A1BC050BA7 };
const KV3ID_t g_KV3Encoding_BinaryAuto = { "binary_auto", 0x45836B856EB109E6, 0x8C06046E3A7012A3 };

// formats
const KV3ID_t g_KV3Format_Generic = { "generic", 0x469806E97412167C, 0xE73790B53EE6F2AF };


class KeyValues
{
public:
    std::byte pad01[0x100];
    std::uint64_t uKey;
    void* pValue;
    std::byte pad02[0x8];

    static bool LoadKV3(KeyValues* kv, const char* material_vmat, const char* kv_name)
    {
        using fn = bool(__fastcall*)(void* thisptr, void* utlstring, const char* buffer, const KV3ID_t* format, const char* kv_name);
        const auto addr = modules::tier0.get_export("?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z").as();
        if (!addr)
            return false;

        auto load_kv3 = reinterpret_cast<fn>(addr);

        return load_kv3(kv, nullptr, material_vmat, &g_KV3Format_Generic, kv_name);
    }
};
