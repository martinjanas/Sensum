#pragma once

struct matrix3x4_t 
{
    const float* operator[](int i) const 
    { 
        return data[i];
    }

    float* operator[](int i) 
    { 
        return data[i];
    }

    float data[3][4];
};



