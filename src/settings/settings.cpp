#include "../settings/settings.h"

namespace settings
{
	namespace visuals
	{
		// bool
		bool m_bFovChanger = false;
		bool m_bBoneEsp = true;
		bool m_bBoxEsp = false;
		bool m_bNameEsp = false;
		bool m_bHealthEsp = false;
		bool const_smooth = false;

		// int
		int m_iFov = 90;
		int aimbot_fov = 20;

		float smooth = 7.f;
		float box_rounding = 0.f;
		float box_thickness = 2.f;
		float pitch = 1.5f;
		float yaw = 1.5f;

		// colors
		ImVec4 m_fBoneColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fBoxColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fHealthColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fNameColor = ImVec4(255, 255, 255, 255);
	}
}