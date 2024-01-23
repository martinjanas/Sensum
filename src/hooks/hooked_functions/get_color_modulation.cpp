#include "../hooks.h"
#include "../../settings/settings.h"

namespace hooks
{
	static const char* blacklist_materials[]
	{
		"effects/flashbang", "effects/flashbang_white", "dev/scope_bluroverlay",
		"effects/overlaysmoke", "weapon", "glow"
	};

	void __fastcall get_color_modulation::hooked(IMaterial* ecx, void* edx, float* r, float* g, float* b)
	{
		original(ecx, r, g, b);

		if (!settings::visuals::night_mode)
			return original(ecx, r, g, b);

		const auto material = reinterpret_cast<IMaterial*>(ecx);

		if (!material)
			return original(ecx, r, g, b);

		const auto group_name = fnv::hash_runtime(material->GetTextureGroupName());
		const auto material_name = fnv::hash_runtime(material->GetName());

		if (group_name == FNV(TEXTURE_GROUP_VGUI) || group_name == FNV(TEXTURE_GROUP_OTHER) || group_name == FNV(TEXTURE_GROUP_MODEL))
			return original(ecx, r, g, b);

		for (const auto& mat : blacklist_materials)
		{
			if (material_name == fnv::hash_runtime(mat))
				return original(ecx, r, g, b);
		}

		switch (group_name)
		{
			case FNV(TEXTURE_GROUP_WORLD):
			case FNV(TEXTURE_GROUP_SKYBOX):
				*r *= 0.15f; *g *= 0.15f; *b *= 0.15f; 
				break;
			case FNV(TEXTURE_GROUP_STATIC_PROPS):
				*r *= 0.45f; *g *= 0.45f; *b *= 0.45f;
				break;
		}

	}
}