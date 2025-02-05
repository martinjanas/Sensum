#include "math.h"

namespace math
{
    float distance_based_fov(const QAngle& delta, const float& distance)
    {
        float pitch_diff = std::sinf(fabsf(delta.pitch) * math::deg2rad) * distance;
        float yaw_diff = std::sinf(fabsf(delta.yaw) * math::deg2rad) * distance;

        float fov = std::hypotf(pitch_diff, yaw_diff);

        fov = std::clamp<float>(fov, 0.f, 180.f);

        return fov;
    }
}