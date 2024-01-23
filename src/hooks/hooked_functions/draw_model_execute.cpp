#include "../hooks.h"
#include "../../features/features.h"

namespace hooks
{
	std::once_flag once_flag_dme;

	void __stdcall draw_model_execute::hooked(IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone)
	{
		original(g::mdl_render, context, &state, &info, bone);

		std::call_once(once_flag_dme, [] { chams::initialize_materials(); });

		chams::on_draw_model_execute(context, state, info, bone, original);
		
		original(g::mdl_render, context, &state, &info, bone);

		g::mdl_render->ForcedMaterialOverride(nullptr);
	}
}