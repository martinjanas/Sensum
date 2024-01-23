#include "../settings/settings.h"
#include "../features/features.h"
#include "../hooks/hooks.h"

#include "../settings/globals.h"
#include "../valve_sdk/classids.h"

#include "../helpers/fnv.h"

namespace chams
{
	void initialize_materials()
	{
		material_debug = g::mat_system->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL, true, 0);
	}

	static auto render_players = [](const bool& ignorez, const bool& health_chams, const Color& visible_color, const Color& occluded_color, const Color& health_color)
	{
		const auto color = ignorez ? occluded_color : (health_chams ? health_color : visible_color);
		material_debug->ColorModulate(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f);
		material_debug->AlphaModulate(color.a() / 255.0f);
		material_debug->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);

		g::mdl_render->ForcedMaterialOverride(material_debug);
	};

	void on_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone, decltype(hooks::draw_model_execute::original)& original)
	{
		c_base_player* player = reinterpret_cast<c_base_player*>(info.pRenderable->GetIClientUnknown()->GetBaseEntity());

		if (!player || !g::local_player)
			return;

		if (!player->IsPlayer())
			return;

		if (player->IsDormant())
			return;

		if (player->m_iTeamNum() != g::local_player->m_iTeamNum()) //enemy chams
		{
			if (!settings::chams::enemy::enabled)
				return;

			Color vis = settings::chams::enemy::color_visible;
			Color not_vis = settings::chams::enemy::color_not_visible;

			float r, g, b;

			int entity_health = player->m_iHealth();

			utils::scale_color_to_health(std::clamp(entity_health / 100.0f, 0.0f, 1.0f), r, g, b);

			Color health_color = Color(r, g, b, 1.0f);

			if (!settings::chams::enemy::visible_only)
			{
				render_players(true, false, vis, not_vis, health_color);
				original(g::mdl_render, ctx, &state, &info, bone);
			}
			render_players(false, settings::chams::enemy::health_chams, vis, not_vis, health_color);
		}
		else if (player->m_iTeamNum() == g::local_player->m_iTeamNum() && player != g::local_player)
		{
			if (!settings::chams::teammates::enabled)
				return;

			Color vis = settings::chams::teammates::color_visible;
			Color not_vis = settings::chams::teammates::color_not_visible;

			float r, g, b;

			int entity_health = player->m_iHealth();

			utils::scale_color_to_health(std::clamp(entity_health / 100.0f, 0.0f, 1.0f), r, g, b);

			Color health_color = Color(r, g, b, 1.0f);

			if (!settings::chams::enemy::visible_only)
			{
				render_players(true, false, vis, not_vis, health_color);
				original(g::mdl_render, ctx, &state, &info, bone);
			}
			render_players(false, settings::chams::teammates::health_chams, vis, not_vis, health_color);
		}
	}
}