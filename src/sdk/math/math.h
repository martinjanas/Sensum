#pragma once
#include <DirectXMath.h>
#include "Vector.h"
#include "QAngle.h"

#define DEG2RAD(x) ((x) * static_cast<float>(3.14159265358979323846) / 180.0f)

namespace math //temp placement, move to cpp later
{
	void vector2angles(const Vector& forward, QAngle& angles)
	{
		float yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / 3.141592654f);
			if (yaw < 0)
				yaw += 360;

			const auto tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / 3.141592654f);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	void angle2vectors(const QAngle& angles, Vector& forward)
	{
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
}
