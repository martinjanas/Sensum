#include "matrix.h"
#include "../../sdk/math/Vector.h"

Vector matrix3x4_t::operator*(const Vector& vec) const 
{
    Vector result;
    result.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3];
    result.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3];
    result.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3];
    return result;
}
