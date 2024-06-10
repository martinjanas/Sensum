#pragma once
#include "../../../sdk/math/Vector.h"
#include "../../../sdk/source_engine/CUtlVector.h"

enum EHitboxes
{
    HITBOX_HEAD = 0,
    HITBOX_NECK,
    HITBOX_PELVIS,
    HITBOX_BELLY,
    HITBOX_THORAX,
    HITBOX_LOWER_CHEST,
    HITBOX_UPPER_CHEST,
    HITBOX_RIGHT_THIGH, 
    HITBOX_LEFT_THIGH,
    HITBOX_RIGHT_CALF,
    HITBOX_LEFT_CALF, 
    HITBOX_RIGHT_FOOT,
    HITBOX_LEFT_FOOT, 
    HITBOX_RIGHT_HAND,
    HITBOX_LEFT_HAND,
    HITBOX_RIGHT_UPPER_ARM,
    HITBOX_RIGHT_FOREARM,
    HITBOX_LEFT_UPPER_ARM,
    HITBOX_LEFT_FOREARM,
    HITBOX_MAX
};

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
    char pad[0x70];
};

class HitboxSet_t
{
public:
    NETVAR(const char*, m_name, "CHitBoxSet", "m_name");
    NETVAR(CUtlVector<Hitbox_t>, m_HitBoxes, "CHitBoxSet", "m_HitBoxes");
    NETVAR(uint32_t, m_nNameHash, "CHitBoxSet", "m_nNameHash");
    NETVAR(const char*, m_SourceFilename, "CHitBoxSet", "m_SourceFilename");
};
