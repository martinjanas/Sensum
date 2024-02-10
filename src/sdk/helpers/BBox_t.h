#pragma once

#include <limits>

struct BBox_t
{
    LONG left{}, top{}, right{}, bottom{};

    BBox_t(LONG _left, LONG _top, LONG _right, LONG _bottom)
    {
        this->left = _left;
        this->top = _top;
        this->right = _right;
        this->bottom = _bottom;
    }

    BBox_t()
    {
        this->left = 0;
        this->top = 0;
        this->right = 0;
        this->bottom = 0;
    }

    LONG width() const
    {
        return right - left;
    }

    LONG height() const
    {
        return bottom - top;
    }

    LONG center_x() const
    {
        return left + width() * 0.5f;
    }

    LONG center_y() const
    {
        return top + height() * 0.5f;
    }
};

struct ABBox_t 
{
    ABBox_t() { Invalidate(); }

    void Invalidate() 
    {
        m_Mins.x = m_Mins.y = FLT_MAX;
        m_Maxs.x = m_Maxs.y = -FLT_MAX;

        std::fill(std::begin(m_Vertices), std::end(m_Vertices), Vector{ -FLT_MAX, -FLT_MAX });
    }

    Vector m_Mins, m_Maxs;
    Vector m_Vertices[8];
};