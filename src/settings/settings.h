#pragma once
#include "../thirdparty/ImGui/imgui.h"

namespace settings
{
	namespace visuals
	{
		// bool
		extern bool m_bFovChanger;
		extern bool m_bBoneEsp;
		extern bool m_bBoxEsp;
		extern bool m_bHealthEsp;
		extern bool m_bNameEsp;

		// int
		extern int m_iFov;
		extern int aimbot_fov;
		extern float smooth;

		extern float radius_min;
		extern float radius_max;

		// colors
		extern ImVec4 m_fBoneColor;
		extern ImVec4 m_fBoxColor;
		extern ImVec4 m_fHealthColor;
		extern ImVec4 m_fNameColor;
	}
}





