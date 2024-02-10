#pragma once

struct matrix3x4_t 
{
    auto operator[](int i) const 
    { 
        return data[i];
    }

    auto operator[](int i) 
    { 
        return data[i];
    }

    float data[3][4];
};



