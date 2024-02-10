#include "../settings/settings.h"

namespace settings
{
	namespace visuals
	{
		// bool
		bool m_bFovChanger = false;
		bool m_bBoneEsp = false;
		bool m_bBoxEsp = false;
		bool m_bNameEsp = false;
		bool m_bHealthEsp = false;

		// int
		int m_iFov = 90;
		float radius_min = 1.f;
		float radius_max = 1.f;
		// colors
		ImVec4 m_fBoneColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fBoxColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fHealthColor = ImVec4(255, 255, 255, 255);
		ImVec4 m_fNameColor = ImVec4(255, 255, 255, 255);
	}
}