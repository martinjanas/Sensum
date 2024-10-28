#pragma once

struct matrix3x4_t 
{
    matrix3x4_t(float a1, float a2, float a3, float a4,
        float a5, float a6, float a7, float a8,
        float a9, float a10, float a11, float a12) {
        data[0][0] = a1;   data[0][1] = a2;   data[0][2] = a3;   data[0][3] = a4;
        data[1][0] = a5;   data[1][1] = a6;   data[1][2] = a7;   data[1][3] = a8;
        data[2][0] = a9;   data[2][1] = a10;  data[2][2] = a11;  data[2][3] = a12;
    }

    matrix3x4_t() = default;

    const float* operator[](int i) const 
    { 
        return data[i];
    }

    float* operator[](int i) 
    { 
        return data[i];
    }

    matrix3x4_t operator+(const matrix3x4_t& other) const {
        matrix3x4_t result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    // Overload * operator for scalar multiplication
    matrix3x4_t operator*(float scalar) const 
    {
        matrix3x4_t result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    matrix3x4_t operator*(const matrix3x4_t& other) const 
    {
        matrix3x4_t result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result[i][j] = data[i][0] * other[0][j] +
                    data[i][1] * other[1][j] +
                    data[i][2] * other[2][j];
            }
            result[i][3] = 0.0f; // Assuming no need for the 4th column in multiplication
        }
        return result;
    }

    float data[3][4];
};



