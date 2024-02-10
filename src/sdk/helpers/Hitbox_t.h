#pragma once
#include "../math/Vector.h"
#include "../helpers/CUtlVector.h"

struct matrix4x2_t
{
public:
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
        };
        float m[4][2];
    };

    float* operator[](int i) { return m[i]; }
    const float* operator[](int i) const { return m[i]; }
};

class Hitbox_t
{
public:
    NETVAR(const char*, m_name, "CHitBox", "m_name");
    NETVAR(const char*, m_sSurfaceProperty, "CHitBox", "m_sSurfaceProperty");
    NETVAR(const char*, m_sBoneName, "CHitBox", "m_sBoneName");
    NETVAR(Vector, m_vMinBounds, "CHitBox", "m_vMinBounds");
    NETVAR(Vector, m_vMaxBounds, "CHitBox", "m_vMaxBounds");
    NETVAR(float, m_flShapeRadius, "CHitBox", "m_flShapeRadius");
    NETVAR(uint32_t, m_nBoneNameHash, "CHitBox", "m_nBoneNameHash");
    NETVAR(int, m_nGroupId, "CHitBox", "m_nGroupId");
    NETVAR(uint8_t, m_nShapeType, "CHitBox", "m_nShapeType");
    NETVAR(bool, m_bTranslationOnly, "CHitBox", "m_bTranslationOnly");
    NETVAR(void*, m_CRC, "CHitBox", "m_CRC");
    NETVAR(void*, m_cRenderColor, "CHitBox", "m_cRenderColor");
    NETVAR(uint16_t, m_nHitBoxIndex, "CHitBox", "m_nHitBoxIndex");
    //char pad[0x70];
};

template <class T>
class utl_memory {
public:
    T* memory;
    int allocation_count;
    int grow_size;

    T& operator[](int i) noexcept { return memory[i]; }
    const T& operator[](int i) const noexcept { return memory[i]; }
};

template <class T>
class util_vector {
public:
    int size;
    utl_memory<T> data;

    T& operator[](int i) noexcept { return data.memory[i]; }
    const T& operator[](int i) const noexcept { return data.memory[i]; }
};

class HitboxSet_t
{
public:
    NETVAR(const char*, m_name, "CHitBoxSet", "m_name");
    NETVAR(CUtlVector<Hitbox_t>, m_HitBoxes, "CHitBoxSet", "m_HitBoxes");
    NETVAR(uint32_t, m_nNameHash, "CHitBoxSet", "m_nNameHash");
    NETVAR(const char*, m_SourceFilename, "CHitBoxSet", "m_SourceFilename");
};
