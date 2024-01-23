#pragma once

#include "../settings/settings.h"
#include "../helpers/math.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/entities.h"
#include "../hooks/hooks.h"

#include <vector>
#include <map>
#include <deque>
#include <unordered_map>

namespace features
{
	void bhop(CUserCmd* cmd);
	void auto_strafe(CUserCmd* cmd);
	void thirdperson();
	void fast_stop(CUserCmd* cmd);
	void lefthand_knife();
	void selfnade(CUserCmd* cmd);
	void on_bomb_begindefuse(IGameEvent* event);
	void player_infobox();
	void buy_logger(IGameEvent* event);
}

namespace color_modulation
{
	void night_mode();
	void event();
	void handle();
}

namespace visuals
{
	void fetch_entities();
	void render(ImDrawList* draw_list);
	void drawring_3d();
	void drawring_3d_popflash();
	void RenderInfo();
	void RenderCircle();
	void RenderCirclePopflash();
	void SmokeHelperAimbot(CUserCmd* cmd);
	void PopflashHelperAimbot(CUserCmd* cmd);
}

namespace chams
{
	void on_draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, decltype(hooks::draw_model_execute::original)& original);

	void initialize_materials();

	inline extern IMaterial* material_debug = 0;
}

namespace esp
{
	void render(ImDrawList* draw_list);
	void render_helpers();
}

namespace aimbot
{
	void handle(CUserCmd* cmd);

	namespace punches
	{
		extern QAngle current;

		extern QAngle last;
	}
}

namespace skins
{
	extern std::map<short, item_setting> m_items;
	extern std::unordered_map<std::string, std::string> m_icon_overrides;

	extern std::vector<weapon_name_t> knife_names;
	extern std::vector<weapon_name_t> glove_names;

	void save();
	void load();
	
	void handle();

	const weapon_info_t* get_weapon_info(const short& defindex);
	const char* get_icon_override(const std::string& original);
	bool is_knife(const short& i);
}

namespace grenade_prediction
{
	void fetch_points(CUserCmd* cmd);
	void render(ImDrawList* draw_list);

	int  PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void TraceHull(Vector& src, Vector& end, trace_t& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	void Setup(Vector& vecSrc, Vector& vecThrow, QAngle viewangles);
}



