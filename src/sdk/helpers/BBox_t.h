#pragma once

#include <limits>

struct BBox_t 
{
    BBox_t() 
    { 
        Invalidate(); 
    }

    void Invalidate() 
    {
        m_Mins.x = m_Mins.y =  FLT_MAX;
        m_Maxs.x = m_Maxs.y = -FLT_MAX;

        std::fill(std::begin(m_Vertices), std::end(m_Vertices), Vector(-FLT_MAX, -FLT_MAX));
    }

    Vector m_Mins, m_Maxs;
    Vector m_Vertices[8];
};