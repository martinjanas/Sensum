#pragma once
#include <DirectXMath.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <numbers>

namespace math //temp placement, move to cpp later
{
	constexpr double pi = std::numbers::pi;
	constexpr float  pi_f = std::numbers::pi_v<float>;

	constexpr float deg2rad = (pi_f / 180.0f);
	constexpr float rad2deg = (180.0f / pi_f);
}
