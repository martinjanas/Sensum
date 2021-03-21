#include <fstream>

#include "Chams.h"
#include "../settings/settings.h"
#include "../features/features.h"
#include "../hooks/hooks.h"
#include "esp.hpp"

void RenderPlayers(const bool& ignorez, const bool& flat, const bool& wireframe, const Color& visible_color, const Color& occluded_color)
{
	static IMaterial* material = g::mat_system->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_OTHER);

	const auto color = ignorez ? occluded_color : visible_color;
	material->ColorModulate(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f);
	material->AlphaModulate(color.a() / 255.0f);
	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->SetMaterialVarFlag(MATERIAL_VAR_FLAT, flat);

	/*material->FindVar("$envmap")->SetStringValue("env_cubemap");
	material->FindVar("$envmaptint")->SetVectorValue(.3f, .3f, .3f);
	material->FindVar("$envmapcontrast")->SetIntValue(1);
	material->FindVar("$envmapsaturation")->SetFloatValue(1.0f);
	material->FindVar("$phong")->SetIntValue(1);
	material->FindVar("$phongexponent")->SetFloatValue(15.0f);
	material->FindVar("$normalmapalphaenvmask")->SetIntValue(1);
	material->FindVar("$phongboost")->SetFloatValue(6.0f);*/
	
	g::mdl_render->ForcedMaterialOverride(material);
}

void Chams::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone)
{ 
	c_base_player* player = c_base_player::GetPlayerByIndex(info.entity_index);

	static auto original = hooks::draw_model_execute::original;

	const char* model_name = info.pModel->szName;

	if (info.entity_index > 0 && info.entity_index <= 64)
	{
		if (player && g::local_player)
		{
			if (player->m_iTeamNum() != g::local_player->m_iTeamNum()) //enemy chams
			{
				if (!settings::chams::enemy::enabled)
					return;

				Color vis = settings::chams::enemy::color_visible;
				Color not_vis = settings::chams::enemy::color_not_visible;

				bool flat = settings::chams::enemy::flat;
				bool wireframe = settings::chams::enemy::wireframe;

				if (!settings::chams::enemy::visible_only)
				{
					RenderPlayers(true, flat, wireframe, vis, not_vis);
					original(g::mdl_render, ctx, &state, &info, bone);
				}
				RenderPlayers(false, flat, wireframe, vis, not_vis);
			}
			else if (player->m_iTeamNum() == g::local_player->m_iTeamNum() && player != g::local_player)
			{
				if (!settings::chams::teammates::enabled)
					return;

				Color vis = settings::chams::teammates::color_visible;
				Color not_vis = settings::chams::teammates::color_not_visible;

				bool flat = settings::chams::teammates::flat;
				bool wireframe = settings::chams::enemy::wireframe;

				if (!settings::chams::enemy::visible_only)
				{
					RenderPlayers(true, flat, wireframe, vis, not_vis);
					original(g::mdl_render, ctx, &state, &info, bone);
				}
				RenderPlayers(false, flat, wireframe, vis, not_vis);
			}
			else if (player == g::local_player)
			{
				if (!settings::chams::localplayer::enabled)
					return;

				Color vis = settings::chams::localplayer::color;

				bool flat = settings::chams::localplayer::flat;
				bool wireframe = settings::chams::localplayer::wireframe;

				RenderPlayers(false, flat, wireframe, vis, vis);
				original(g::mdl_render, ctx, &state, &info, bone);
			}
		}
	}

	static IMaterial* material = g::mat_system->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);

	if (settings::chams::misc::weapon_chams) //Since I dont know the perfomance difference between strstr and std::string.find,
	{                                        // I will use strstr, more research needed.
		if (strstr(model_name, "weapons/v")) //Weapons in hand.
		{
			if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
			{
				material->ColorModulate(Color::Green.r() / 255.0f, Color::Green.g() / 255.0f, Color::Green.b() / 255.0f);
				material->AlphaModulate(Color::Green.a() / 255.0f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				g::mdl_render->ForcedMaterialOverride(material);
			}
		}
	}

	if (settings::chams::misc::arms_chams)
	{
		if (strstr(model_name, "arms"))
		{
			if (!strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
			{
				material->ColorModulate(Color::Blue.r() / 255.0f, Color::Blue.g() / 255.0f, Color::Blue.b() / 255.0f);
				material->AlphaModulate(Color::Blue.a() / 255.0f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				g::mdl_render->ForcedMaterialOverride(material);
			}
		}
	}

	if (settings::chams::misc::accesories_chams)
	{
		if (strstr(model_name, "_dropped.mdl") && strstr(model_name, "weapons/w")) //Weapons on back.
		{
			if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
			{
				material->ColorModulate(Color::Green.r() / 255.0f, Color::Green.g() / 255.0f, Color::Green.b() / 255.0f);
				material->AlphaModulate(Color::Green.a() / 255.0f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				g::mdl_render->ForcedMaterialOverride(material);
			}
		}

		if (strstr(model_name, "weapons/w")) //Weapons in enemy hands.
		{
			if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
			{
				material->ColorModulate(Color::White.r() / 255.0f, Color::White.g() / 255.0f, Color::White.b() / 255.0f);
				material->AlphaModulate(Color::White.a() / 255.0f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				g::mdl_render->ForcedMaterialOverride(material);
			}
		}

		if (strstr(model_name, "defuser")) //Defuse kit on body.
		{
			if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
			{
				material->ColorModulate(Color::Red.r() / 255.0f, Color::Red.g() / 255.0f, Color::Red.b() / 255.0f);
				material->AlphaModulate(Color::Red.a() / 255.0f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				g::mdl_render->ForcedMaterialOverride(material);
			}
		}
	}
}