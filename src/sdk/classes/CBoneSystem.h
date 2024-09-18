#pragma once

#include <cstdint>

#include "../../sdk/classes/CStrongHandle.h"
#include "../../sdk/helpers/CUtlSymbolLarge.hpp"
#include "../../sdk/helpers/BitFlag.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/helpers/modules.h"

enum EBoneFlags : uint32_t 
{
    FLAG_NO_BONE_FLAGS = 0x0,
    FLAG_BONEFLEXDRIVER = 0x4,
    FLAG_CLOTH = 0x8,
    FLAG_PHYSICS = 0x10,
    FLAG_ATTACHMENT = 0x20,
    FLAG_ANIMATION = 0x40,
    FLAG_MESH = 0x80,
    FLAG_HITBOX = 0x100,
    FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
    FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
    FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
    FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
    FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
    FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
    FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
    FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
    FLAG_BONE_MERGE_READ = 0x40000,
    FLAG_BONE_MERGE_WRITE = 0x80000,
    FLAG_ALL_BONE_FLAGS = 0xfffff,
    BLEND_PREALIGNED = 0x100000,
    FLAG_RIGIDLENGTH = 0x200000,
    FLAG_PROCEDURAL = 0x400000,
};

enum EBones
{
    BONE_PELVIS = 0,           // Main body (hips)
    BONE_SPINE_0 = 1,          // Lower spine
    BONE_SPINE_1 = 2,          // Mid spine
    BONE_SPINE_2 = 3,          // Upper spine
    BONE_SPINE_3 = 4,          // Neck base
    BONE_NECK = 5,             // Neck
    BONE_HEAD = 6,             // Head

    // Left arm
    BONE_L_CLAVICLE = 7,       // Left clavicle
    BONE_L_UPPER_ARM = 8,      // Left upper arm
    BONE_L_FOREARM = 9,        // Left forearm
    BONE_L_HAND = 10,          // Left hand

    // Right arm
    BONE_R_CLAVICLE = 11,      // Right clavicle
    BONE_R_UPPER_ARM = 12,     // Right upper arm
    BONE_R_FOREARM = 13,       // Right forearm
    BONE_R_HAND = 14,          // Right hand

    // Left leg
    BONE_L_THIGH = 15,         // Left thigh
    BONE_L_CALF = 16,          // Left calf
    BONE_L_FOOT = 17,          // Left foot
    BONE_L_TOE = 18,           // Left toe

    // Right leg
    BONE_R_THIGH = 19,         // Right thigh
    BONE_R_CALF = 20,          // Right calf
    BONE_R_FOOT = 21,          // Right foot
    BONE_R_TOE = 22,           // Right toe

    // Fingers (left hand)
    BONE_L_FINGER_0 = 23,      // Left thumb
    BONE_L_FINGER_1 = 24,      // Left index finger
    BONE_L_FINGER_2 = 25,      // Left middle finger
    BONE_L_FINGER_3 = 26,      // Left ring finger
    BONE_L_FINGER_4 = 27,      // Left pinky finger

    // Fingers (right hand)
    BONE_R_FINGER_0 = 28,      // Right thumb
    BONE_R_FINGER_1 = 29,      // Right index finger
    BONE_R_FINGER_2 = 30,      // Right middle finger
    BONE_R_FINGER_3 = 31,      // Right ring finger
    BONE_R_FINGER_4 = 32,      // Right pinky finger

    // Additional bones (attachments, etc.)
    BONE_WEAPON = 33,          // Weapon attachment point
    BONE_WEAPON_HAND = 34,     // Hand that holds the weapon
    BONE_MISC_0 = 35,          // Miscellaneous bone
    BONE_MISC_1 = 36,          // Another miscellaneous bone

    // End of bone list
    BONE_MAX = 37              // Total number of bones
};

struct alignas(16) CBoneData 
{
    Vector position;
    float scale;
    Vector rotation;
};

class CModel 
{
public:
    BitFlag GetBoneFlags(const int index) 
    {
        using fn = int(__thiscall*)(void*, int); //int -> BitFlag?

        static auto addr = modules::client.pattern_scanner.scan("85 D2 78 16 3B 91", "GetBoneFlags").as();

        const auto& get_bone_flags = reinterpret_cast<fn>(addr);
       
        if (get_bone_flags)
            return BitFlag(get_bone_flags(this, index));

        return {};
    }

    int GetBoneParent(const int index) 
    {
        using fn = int(__thiscall*)(void*, int);

        static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 41 0F 10 14 3F", "GetBoneParent").add(0x1).abs().as();

        const auto& get_bone_parent = reinterpret_cast<fn>(addr);

        if (get_bone_parent)
            return get_bone_parent(this, index);

        return -1;
    }

    const char* GetBoneName(const std::int32_t index) 
    {
        using fn = const char* (__thiscall*)(void*, int);

        static auto addr = modules::client.pattern_scanner.scan("85 D2 78 25 3B 91", "GetBoneName").as();

        const auto& get_bone_name = reinterpret_cast<fn>(addr);

        if (get_bone_name)
            return get_bone_name(this, index);

        return nullptr;
    }

public:
    std::uint8_t padding_0[0x170];
    std::int32_t BoneCount;
};

class CModelState 
{
public:
    std::uint8_t padding_0[0x80];
    CBoneData* bones;
    std::uint8_t padding_1[0x18];
    CStrongHandle<CModel> modelHandle;
    CUtlSymbolLarge modelName;
};

