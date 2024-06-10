#pragma once
#include "Vector.h"
#include "../../helpers/matrix.h"

class Quaternion
{
public:
    float x, y, z, w;
};

class alignas(16) Transform_t
{
public:
    alignas(16) Vector m_pos;
    alignas(16) Quaternion m_rot;

    matrix3x4_t ToMatrix3x4() const
    {
        matrix3x4_t matrix{};

        matrix[0][0] = 1.0f - 2.0f * m_rot.y * m_rot.y - 2.0f * m_rot.z * m_rot.z;
        matrix[1][0] = 2.0f * m_rot.x * m_rot.y + 2.0f * m_rot.w * m_rot.z;
        matrix[2][0] = 2.0f * m_rot.x * m_rot.z - 2.0f * m_rot.w * m_rot.y;

        matrix[0][1] = 2.0f * m_rot.x * m_rot.y - 2.0f * m_rot.w * m_rot.z;
        matrix[1][1] = 1.0f - 2.0f * m_rot.x * m_rot.x - 2.0f * m_rot.z * m_rot.z;
        matrix[2][1] = 2.0f * m_rot.y * m_rot.z + 2.0f * m_rot.w * m_rot.x;

        matrix[0][2] = 2.0f * m_rot.x * m_rot.z + 2.0f * m_rot.w * m_rot.y;
        matrix[1][2] = 2.0f * m_rot.y * m_rot.z - 2.0f * m_rot.w * m_rot.x;
        matrix[2][2] = 1.0f - 2.0f * m_rot.x * m_rot.x - 2.0f * m_rot.y * m_rot.y;

        matrix[0][3] = m_pos.x;
        matrix[1][3] = m_pos.y;
        matrix[2][3] = m_pos.z;

        return matrix;
    }

    matrix3x4_t ToMatrix3x4(const Vector& abs_origin) const
    {
        matrix3x4_t matrix{};

        matrix[0][0] = 1.0f - 2.0f * m_rot.y * m_rot.y - 2.0f * m_rot.z * m_rot.z;
        matrix[1][0] = 2.0f * m_rot.x * m_rot.y + 2.0f * m_rot.w * m_rot.z;
        matrix[2][0] = 2.0f * m_rot.x * m_rot.z - 2.0f * m_rot.w * m_rot.y;

        matrix[0][1] = 2.0f * m_rot.x * m_rot.y - 2.0f * m_rot.w * m_rot.z;
        matrix[1][1] = 1.0f - 2.0f * m_rot.x * m_rot.x - 2.0f * m_rot.z * m_rot.z;
        matrix[2][1] = 2.0f * m_rot.y * m_rot.z + 2.0f * m_rot.w * m_rot.x;

        matrix[0][2] = 2.0f * m_rot.x * m_rot.z + 2.0f * m_rot.w * m_rot.y;
        matrix[1][2] = 2.0f * m_rot.y * m_rot.z - 2.0f * m_rot.w * m_rot.x;
        matrix[2][2] = 1.0f - 2.0f * m_rot.x * m_rot.x - 2.0f * m_rot.y * m_rot.y;

        matrix[0][3] = abs_origin.x;
        matrix[1][3] = abs_origin.y;
        matrix[2][3] = abs_origin.z;

        return matrix;
    }
};
