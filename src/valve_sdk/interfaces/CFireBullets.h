#pragma once

#include "../../valve_sdk/math/Vectors.hpp"
#include "../../valve_sdk/math/QAngle.hpp"

class CFireBullets
{
public:
	char pad_0000[8]; //0x0000
	int m_iPlayer1; //0x000C
	int m_iPlayer; //0x000C
	int m_iItemDefinitionIndex; //0x0010
	Vector m_vecOrigin; //0x0014
	QAngle m_absAngles; //0x0020
};
