#include "utils.h"
#include "../helpers/Hitbox_t.h"

namespace utils
{
    const char* hitbox_index_to_name(int index)
    {
        switch (index)
        {
        case HITBOX_HEAD:
            return "Head";
        case HITBOX_NECK:
            return "Neck";
        case HITBOX_PELVIS:
            return "Pelvis";
        case HITBOX_BELLY:
            return "Belly";
        case HITBOX_THORAX:
            return "Thorax";
        case HITBOX_LOWER_CHEST:
            return "Lower Chest";
        case HITBOX_UPPER_CHEST:
            return "Upper Chest";
        case HITBOX_RIGHT_THIGH:
            return "Right Thigh";
        case HITBOX_LEFT_THIGH:
            return "Left Thigh";
        case HITBOX_RIGHT_CALF:
            return "Right Calf";
        case HITBOX_LEFT_CALF:
            return "Left Calf";
        case HITBOX_RIGHT_FOOT:
            return "Right Foot";
        case HITBOX_LEFT_FOOT:
            return "Left Foot";
        case HITBOX_RIGHT_HAND:
            return "Right Hand";
        case HITBOX_LEFT_HAND:
            return "Left Hand";
        case HITBOX_RIGHT_UPPER_ARM:
            return "Right Upper Arm";
        case HITBOX_RIGHT_FOREARM:
            return "Right Forearm";
        case HITBOX_LEFT_UPPER_ARM:
            return "Left Upper Arm";
        case HITBOX_LEFT_FOREARM:
            return "Left Forearm";
        }
    }
}