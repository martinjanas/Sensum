#pragma once

struct mat3x4_t
{
    float data[3][4]{ };

    float* operator[](int i) noexcept
    {
        return data[i];
    }

    const float* operator[](int i) const noexcept
    {
        return data[i];
    }
};



