#pragma once

#include "../settings/settings.h"
#include "../helpers/math.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/entities.h"

#include <vector>

namespace features
{
	void bhop(CUserCmd* cmd);
	void auto_strafe(CUserCmd* cmd);
	void thirdperson();
	void fast_stop(CUserCmd* cmd);
	void edge_jump_pre(CUserCmd* cmd);
	void edge_jump_post(CUserCmd* cmd);
	void aimlines(ImDrawList* draw_list);
	void selfnade(CUserCmd* cmd);
}

namespace color_modulation
{
	const char* GetMapOriginalSkybox();
	void NightMode();
	void event();
	void handle();
	void sky_changer();
	void set_material_tone();
	void music_kit_changer();
}

namespace visuals
{
	void fetch_entities();
	void render(ImDrawList* draw_list);
	void leftknife();
	void drawring_3d();
	void drawring_3d_popflash();
	void drawring_3d_hegrenade();
	void drawring_3d_molotov();
	void RenderInfo();
	void RenderInfoHeGrenade();
	void RenderInfoMolotov();
	void RenderCircle();
	void RenderCirclePopflash();
	void RenderCircleHeGrenade();
	void RenderCircleMolotov();
	void SmokeHelperAimbot(CUserCmd* cmd);
	void PopflashHelperAimbot(CUserCmd* cmd);
	void HeGrenadeHelperAimbot(CUserCmd* cmd);
	void MolotovHelperAimbot(CUserCmd* cmd);
	void glow_players();
	void glow_misc();
}

namespace chams
{
	void on_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void misc_chams();
}

namespace esp
{
	void render(ImDrawList* draw_list);
	void render_helpers();
}

namespace aimbot
{
	void handle(CUserCmd* pCmd);
	float get_fov();
	float GetFov();
	void backtrack_chams(IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone);
	void get_backtrack_data(CUserCmd* cmd);
	
	namespace punches
	{
		extern QAngle current;

		extern QAngle last;
		extern QAngle last_corrected;
	}
}

namespace skins
{
	extern std::map<int, item_setting> m_items;
	extern std::map<int, statrack_setting> statrack_items;
	extern std::unordered_map<std::string, std::string> m_icon_overrides;

	extern std::vector<paint_kit_t> skin_kits;

	extern std::vector<weapon_name_t> knife_names;
	extern std::vector<weapon_name_t> glove_names;

	void save();
	void load_statrack();
	void save_statrack();
	void load();
	void initialize_kits();

	void handle();

	const weapon_info_t* get_weapon_info(const int& defindex);
	const char* get_icon_override(const std::string& original);
}

namespace zeusbot
{
	void handle(CUserCmd* cmd);
}

namespace grenade_prediction
{
	void fetch_points(CUserCmd* cmd);
	void render(ImDrawList* draw_list);

	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void TraceHull(Vector& src, Vector& end, trace_t& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	void Setup(Vector& vecSrc, Vector& vecThrow, QAngle viewangles);
}

namespace entities
{
	void on_create_move(CUserCmd* cmd);
}

namespace offscreen_entities
{
	extern ImU32 sound_color;
	extern ImU32 origin_color;
}

namespace engine_prediction
{
	void start(CUserCmd* cmd);
	void finish(CUserCmd* cmd);
}

namespace clantag
{
	void set(const char* tag);
	void set(const char* tag, const char* label);

	void restore();
	void animate();
}

namespace slow_walk
{
	void handle(CUserCmd* cmd);
}

namespace resolver
{
	void handle();
}

namespace no_flash
{
	void handle();
}

namespace no_smoke
{
	void event();
	void handle();
}