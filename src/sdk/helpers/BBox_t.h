#pragma once

struct BBox_t
{
    float left{}, top{}, right{}, bottom{};

    float width() const
    {
        return right - left;
    }

    float height() const
    {
        return bottom - top;
    }

    float center_x() const
    {
        return left + width() * 0.5f;
    }

    float center_y() const
    {
        return top + height() * 0.5f;
    }
};