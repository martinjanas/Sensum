#include "../settings/settings.h"
#include "../features/features.h"

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

		// int
		int m_iFov = 90;
		int aimbot_fov = 20;
		int aimbot_hitbox = TARGET_HEAD | TARGET_CHEST;
		int smooth_mode = 1;

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