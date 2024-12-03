#pragma once
#include <cstddef>
#include "../math/Vector.h"
#include <wtypes.h>

class CViewSetup
{
public:
	std::byte pad01[0x494]; //0x494 ?
	float m_flOrthoLeft; // 0x0494
	float m_flOrthoTop; // 0x0498
	float m_flOrthoRight; // 0x049C
	float m_flOrthoBottom; // 0x04A0
	std::byte pad02[0x34]; //0x34?
	float m_flFov; // 0x04D8
	float m_flFovViewmodel; // 0x04DC
	Vector m_vecOrigin; // 0x04E0
	std::byte pad03[0xC];
	QAngle m_angView; // 0x04F8
	std::byte pad04[0x14];
	float m_flAspectRatio; // 0x0518
	std::byte pad05[0x71];
	BYTE nSomeFlags;
};

struct CViewRender
{
	std::byte pad01[0x8];
	CViewSetup m_viewSetup; // 0x0008

	virtual void func0() = 0;
};
