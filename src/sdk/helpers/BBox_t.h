#pragma once

#include <limits>
#include "../../sdk/helpers/globals.h"
#include "../../sdk/sdk.h"

//struct BBox_t 
//{
//    BBox_t() 
//    { 
//        Invalidate(); 
//    }
//
//    void Invalidate() 
//    {
//        m_Mins.x = m_Mins.y =  FLT_MAX;
//        m_Maxs.x = m_Maxs.y = -FLT_MAX;
//
//        std::fill(std::begin(m_Vertices), std::end(m_Vertices), Vector(-FLT_MAX, -FLT_MAX));
//    }
//
//    Vector m_Mins, m_Maxs;
//    Vector m_Vertices[8];
//};

const int BOTTOM_RIGHT_BACK = 0;
const int BOTTOM_RIGHT_FRONT = 1;
const int BOTTOM_LEFT_BACK = 2;
const int BOTTOM_LEFT_FRONT = 3;
const int TOP_RIGHT_BACK = 4;
const int TOP_RIGHT_FRONT = 5;
const int TOP_LEFT_BACK = 6;
const int TOP_LEFT_FRONT = 7;

struct BBox_t
{
    Vector m_Mins, m_Maxs;
    Vector m_Vertices[8];

    BBox_t()
    {
        Invalidate();
    }

    void Invalidate()
    {
        m_Mins.x = m_Mins.y = FLT_MAX;
        m_Maxs.x = m_Maxs.y = -FLT_MAX;

        std::fill(std::begin(m_Vertices), std::end(m_Vertices), Vector(-FLT_MAX, -FLT_MAX));
    }

    float GetWidth() const
    {
        return m_Maxs.x - m_Mins.x;
    }

    float GetHeight() const
    {
        return m_Maxs.y - m_Mins.y;
    }

    Vector GetTopMid() const {
        return Vector((m_Mins.x + m_Maxs.x) * 0.5f, m_Mins.y);  // Midpoint between top-left and top-right
    }

    // Get the midpoint of the bottom edge
    Vector GetBottomMid() const {
        return Vector((m_Mins.x + m_Maxs.x) * 0.5f, m_Maxs.y);  // Midpoint between bottom-left and bottom-right
    }

    // Get the midpoint of the left edge
    Vector GetLeftMid() const {
        return Vector(m_Mins.x, (m_Mins.y + m_Maxs.y) * 0.5f);  // Midpoint between top-left and bottom-left
    }

    // Get the midpoint of the right edge
    Vector GetRightMid() const {
        return Vector(m_Maxs.x, (m_Mins.y + m_Maxs.y) * 0.5f);  // Midpoint between top-right and bottom-right
    }

    // Corner points (using m_Mins and m_Maxs directly)
    Vector GetTopLeft() const { return Vector(m_Mins.x, m_Mins.y); }
    Vector GetTopRight() const { return Vector(m_Maxs.x, m_Mins.y); }
    Vector GetBottomLeft() const { return Vector(m_Mins.x, m_Maxs.y); }
    Vector GetBottomRight() const { return Vector(m_Maxs.x, m_Maxs.y); }
};