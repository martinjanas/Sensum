#include "../render.h"
#include "../../settings/globals.h"
#include "../../settings/settings.h"
#include "../../features/features.h"
#include "../../helpers/console.h"
#include "../menu_strings.h"

extern void bind_button(const char* label, int& key);
extern bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0.f, 0.f));

namespace render
{
	namespace menu
	{
		void misc_tab()
		{
				child("Customization", []()
				{
					ImGui::SliderFloatLeftAligned("Viewmodel FOV:", &settings::misc::viewmodel_fov, 54, 120, "%.0f *");
					ImGui::SliderIntLeftAligned("Camera FOV:", &settings::misc::debug_fov, 80, 120, "%.0f *");
				});

				ImGui::NextColumn();

				child("Extra", []()
				{
					checkbox("Radar", &settings::misc::radar);
					checkbox("Bunny Hop", &settings::misc::bhop);
					checkbox("Auto Strafe", &settings::misc::auto_strafe);
					checkbox("Grief Nade (?)", &settings::misc::selfnade);
					tooltip("Look up, hold mouse2, once fully primed start holding mouse1, once is HE in air - crouch, does -98 dmg.");
					checkbox("Post Processing", &globals::post_processing);
					checkbox("Noscope Overlay", &settings::misc::noscope);
					checkbox("Left Hand Knife", &settings::misc::lefthandknife);
					checkbox("Smoke Helper", &settings::misc::smoke_helper);
					checkbox("Flash Helper", &settings::misc::flash_helper);
					checkbox("Fast Stop", &settings::misc::fast_stop);
					checkbox("Print FOV (Debug)", &settings::misc::print_fov);
				});

				ImGui::NextColumn();

				child("Binds", []()
				{
					bind_button("Third Person", globals::binds::thirdperson::key);
				});
		}
	}
}
