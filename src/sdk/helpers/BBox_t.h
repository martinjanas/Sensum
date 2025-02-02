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
        m_Mins = Vector(FLT_MAX, FLT_MAX);
        m_Maxs = Vector(-FLT_MAX, -FLT_MAX);
        std::fill(std::begin(m_Vertices), std::end(m_Vertices), Vector(-FLT_MAX, -FLT_MAX));
    }

    bool IsValid()
    {
        return !isnan(m_Mins.x) && !isnan(m_Mins.y) && !isnan(m_Mins.z) && 
               !isnan(m_Maxs.x) && !isnan(m_Maxs.y) && !isnan(m_Maxs.z);
    }
    
    float GetWidth() const
    {
        return m_Maxs.x - m_Mins.x;
    }

    float GetHeight() const
    {
        return m_Maxs.y - m_Mins.y;
    }

    // Get center of the top face (top edge)
    Vector GetTop() const {
        return Vector((m_Mins.x + m_Maxs.x) * 0.5f, m_Mins.y);
    }

    // Get center of the bottom face (bottom edge)
    Vector GetBottom() const {
        return Vector((m_Mins.x + m_Maxs.x) * 0.5f, m_Maxs.y);
    }

    // Get center of the left face (left edge)
    Vector GetLeft() const {
        return Vector(m_Mins.x, (m_Mins.y + m_Maxs.y) * 0.5f);
    }

    // Get center of the right face (right edge)
    Vector GetRight() const {
        return Vector(m_Maxs.x, (m_Mins.y + m_Maxs.y) * 0.5f);
    }

    // Get the top-left corner
    Vector GetTopLeft() const { return Vector(m_Mins.x, m_Mins.y); }

    // Get the top-right corner
    Vector GetTopRight() const { return Vector(m_Maxs.x, m_Mins.y); }

    // Get the bottom-left corner
    Vector GetBottomLeft() const { return Vector(m_Mins.x, m_Maxs.y); }

    // Get the bottom-right corner
    Vector GetBottomRight() const { return Vector(m_Maxs.x, m_Maxs.y); }
};