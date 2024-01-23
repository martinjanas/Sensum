#include "utils.h"
#include "math.h"
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

#include "input.h"
#include "console.h"
#include "../settings/globals.h"
#include "../security/importer.h"
#include "../features/features.h"
#include "../imgui/imgui_internal.h"
#include "../valve_sdk/csgostructs.hpp"
#include "..//render/render.h"

namespace utils
{
	void render_dot(ImDrawList* draw_list, const Vector& origin, const QAngle& angles, const ImU32& color)
	{
		auto rotate_points = [](Vector* points, float rotation)->void
		{
			const auto points_center = (points[0] + points[1] + points[2]) / 3;
			for (int k = 0; k < 3; k++)
			{
				auto& point = points[k];
				point -= points_center;

				const auto temp_x = point.x;
				const auto temp_y = point.y;

				const auto theta = DEG2RAD(rotation);
				const auto c = cos(theta);
				const auto s = sin(theta);

				point.x = temp_x * c - temp_y * s;
				point.y = temp_x * s + temp_y * c;

				point += points_center;
			}
		};


		const auto display_size = ImGui::GetIO().DisplaySize;
		const auto screen_center = Vector(display_size.x * .5f, display_size.y * .5f);

		QAngle aim_angles;
		math::vector2angles(origin - g::local_player->m_vecOrigin(), aim_angles);

		const auto angle_yaw_rad = DEG2RAD(angles.yaw - aim_angles.yaw - 90);

		const auto size = 10;
		const auto radius = 45;

		const auto new_point_x = screen_center.x + ((((display_size.x - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
		const auto new_point_y = screen_center.y + ((((display_size.y - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

		Vector points[3] =
		{
			Vector(new_point_x - size, new_point_y - size),
			Vector(new_point_x + size, new_point_y),
			Vector(new_point_x - size, new_point_y + size)
		};

		rotate_points(points, angles.yaw - aim_angles.yaw - 90.f);

		draw_list->AddTriangleFilled({ points[0].x + 1.f, points[0].y + 1.f }, { points[1].x + 1.f, points[1].y + 1.f }, { points[2].x + 1.f, points[2].y + 1.f }, IM_COL32_BLACK);
		draw_list->AddTriangleFilled({ points[0].x, points[0].y }, { points[1].x, points[1].y }, { points[2].x, points[2].y }, color);
	}

	RECT get_box(const Vector* transformed_points)
	{
		static Vector screen_points[8];
		for (int i = 0; i < 8; i++)
		{
			if (!math::world2screen(transformed_points[i], screen_points[i]))
				return {};
		}

		auto top = screen_points[0].y;
		auto left = screen_points[0].x;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++)
		{
			if (left > screen_points[i].x)
				left = screen_points[i].x;
			if (top > screen_points[i].y)
				top = screen_points[i].y;
			if (right < screen_points[i].x)
				right = screen_points[i].x;
			if (bottom < screen_points[i].y)
				bottom = screen_points[i].y;
		}

		return RECT{ (long)left, (long)top, (long)right, (long)bottom };
	}

	void scale_color_to_health(float fraction, float& outR, float& outG, float& outB)
	{
		constexpr auto greenHue = 1.0f / 3.0f;
		constexpr auto redHue = 0.0f;
		ImGui::ColorConvertHSVtoRGB(std::lerp(redHue, greenHue, fraction), 1.0f, 1.0f, outR, outG, outB);
	}

	float get_interpolation_compensation()
	{
		static const auto cl_interp = g::cvar->find("cl_interp");
		static const auto max_ud_rate = g::cvar->find("sv_maxupdaterate");
		static const auto cl_interp_ratio = g::cvar->find("cl_interp_ratio");
		static const auto c_min_ratio = g::cvar->find("sv_client_min_interp_ratio");
		static const auto c_max_ratio = g::cvar->find("sv_client_max_interp_ratio");

		float ratio = cl_interp_ratio->GetFloat();
		if (ratio == 0)
			ratio = 1.0f;

		if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
			ratio = std::clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

		const auto ud_rate = max_ud_rate->GetInt();

		return std::max(cl_interp->GetFloat(), (ratio / ud_rate));
	}

	Vector CalcHelpPos(Vector target)
	{
		if (!g::local_player)
			return Vector{};

		QAngle viewangles;

		g::engine_client->GetViewAngles(viewangles);

		float range = 5.f;

		float r_1, r_2;
		float x_1, y_1;

		Vector eye_pos = g::local_player->GetEyePos();

		r_1 = -(target.y - eye_pos.y);
		r_2 =   target.x - eye_pos.x;

		float yaw = (viewangles.yaw - 90.0f) * (float)(M_PI / 180.0F);

		x_1 = (r_2 * std::cosf(yaw) - r_1 * std::sinf(yaw)) / 20.f;
		y_1 = (r_2 * std::sinf(yaw) + r_1 * std::cosf(yaw)) / 20.f;

		x_1 *= range;
		y_1 *= range;

		return Vector(x_1, y_1, 0);
	}

	Vector CalcDir(const Vector& vAngles)
	{
		Vector vForward;
		float  sp, sy, cp, cy;

		sy = sin(DEG2RAD(vAngles[1]));
		cy = cos(DEG2RAD(vAngles[1]));

		sp = sin(DEG2RAD(vAngles[0]));
		cp = cos(DEG2RAD(vAngles[0]));

		vForward.x = cp * cy;
		vForward.y = cp * sy;
		vForward.z = -sp;

		return vForward;
	}

	bool is_entity_audible(int entity_index, const Vector& origin)
	{
		for (int i = 0; i < g::active_channel->count; ++i)
		{
			float dist = g::local_player->m_vecOrigin().DistTo(origin);

			if (g::channels[g::active_channel->list[i]].sound_source == entity_index && dist <= 1064.f)
				return true;
		}

		return false;
	};

	bool Insecure()
	{
		return std::strstr(GetCommandLineA(), "-insecure");
	}

	bool IsPlayingMM()
	{
		ConVar* type = nullptr;
		ConVar* mode = nullptr;

		if (!mode)
			mode = g::cvar->find("game_mode");

		if (!type)
			type = g::cvar->find("game_type");

		if ((type->GetInt() == 0 && mode->GetInt() == 0)) //casual
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 1)) //demolition
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 0)) //arms race
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 2)) //deathmatch
			return false;

		if (type->GetInt() == 0 && mode->GetInt() == 1) //competitive
			return true;

		if (type->GetInt() == 0 && mode->GetInt() == 2) //wingman
			return true;

		if (type->GetInt() == 6 && mode->GetInt() == 0) //dangerzone
			return true;

		if (type->GetInt() == 6 && mode->GetInt() == 0) //scrimmage
			return true;

		return false;
	}

	bool IsPlayingMM_AND_IsValveServer() //returns true if server has mm gamemodes set (5v5, 2v2, scrimmage, dangerzone) and true if server is Valve official server.
	{
		ConVar* type = nullptr;
		ConVar* mode = nullptr;

		const char* server = "";

		if (g::engine_client->IsInGame())
		{
			auto nci = g::engine_client->GetNetChannelInfo();

			if (nci)
			{
				server = nci->GetAddress();

				if (!strcmp(server, "loopback")) //TODO: Convert to hash.
					server = "Local server";
				else if (g::game_rules_proxy->m_bIsValveDS())
					server = "Valve server";
			}
		}

		if (!mode)
			mode = g::cvar->find("game_mode");

		if (!type)
			type = g::cvar->find("game_type");

		if ((type->GetInt() == 0 && mode->GetInt() == 0) && server == "Valve server") //casual
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 1) && server == "Valve server") //demolition
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 0) && server == "Valve server") //arms race
			return false;

		if ((type->GetInt() == 1 && mode->GetInt() == 2) && server == "Valve server") //deathmatch
			return false;

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

		if ((type->GetInt() == 0 && mode->GetInt() == 0) && server != "Valve server") //casual
			return true;

		if ((type->GetInt() == 1 && mode->GetInt() == 1) && server != "Valve server") //demolition
			return true;

		if ((type->GetInt() == 1 && mode->GetInt() == 0) && server != "Valve server") //arms race
			return true;

		if ((type->GetInt() == 1 && mode->GetInt() == 2) && server != "Valve server") //deathmatch
			return true;

		if (type->GetInt() == 0 && mode->GetInt() == 1) //competitive
			return true;

		if (type->GetInt() == 0 && mode->GetInt() == 2) //wingman
			return true;

		if (type->GetInt() == 6 && mode->GetInt() == 0) //dangerzone
			return true;

		if (type->GetInt() == 6 && mode->GetInt() == 0) //scrimmage
			return true;

		return false;
	}

	void create_beam(const int& user_id, const Vector& end_pos)
	{
		if (!end_pos.IsValid())
			return;

		c_base_player* player = c_base_player::GetPlayerByUserId(user_id);
		if (!player || !player->IsPlayer() || player == g::local_player)
			return;

		if (player->m_iTeamNum() == g::local_player->m_iTeamNum() && !settings::misc::deathmatch)
			return;

		if (player->GetEyePos().DistTo(end_pos) < 0.1f)
			return;

		BeamInfo_t beamInfo;
		beamInfo.m_nType = TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/physbeam.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = 1.f;
		beamInfo.m_flWidth = 2.0f;
		beamInfo.m_flEndWidth = 2.0f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = player->m_iTeamNum() == team::team_ct ? 0.f : 240.f;
		beamInfo.m_flGreen = 50.f;
		beamInfo.m_flBlue = player->m_iTeamNum() == team::team_ct ? 240.f : 0.f;
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = FBEAM_FADEIN | FBEAM_FADEOUT;
		beamInfo.m_vecStart = player->GetEyePos();
		beamInfo.m_vecEnd = end_pos;

		Beam_t* beam = g::view_render_beams->CreateBeamPoints(beamInfo);
		if (beam)
			g::view_render_beams->DrawBeam(beam);
	}

	ImU32 to_im32(const Color& color, const float& alpha)
	{
		return ImGui::GetColorU32(ImVec4(color.r() / 255.f, color.g() / 255.f, color.b() / 255.f, alpha));
	}

	void* get_export(const char* module_name, const char* export_name)
	{
		HMODULE mod;
		while (!((mod = LI_FN(GetModuleHandleA).cached()(module_name))))
			LI_FN(Sleep).cached()(100);

		return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
	}

	unsigned int get_virtual(void* class_, unsigned int index) {
		return (unsigned int)(*(int**)class_)[index];
	}

	std::string get_weapon_name(void* weapon)
	{
		static const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));

		if (!weapon)
			return "";

		const auto wide_name = g::localize->Find(((c_base_combat_weapon*)weapon)->GetWeaponData()->szHudName);

		char weapon_name[256];
		V_UCS2ToUTF8(wide_name, weapon_name, sizeof(weapon_name));

		return weapon_name;
	}

	wchar_t* to_wstring(const char* str)
	{
		const auto size = strlen(str) + 1;

		auto* wc = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, str, -1, wc, size);

		return wc;

		delete[] wc; //Edit: Added delete
	}

	int get_active_key()
	{
		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		for (auto i = 0; i < 5; i++)
		{
			if (io.MouseDown[i])
			{
				switch (i)
				{
				case 0:
					return VK_LBUTTON;
				case 1:
					return VK_RBUTTON;
				case 2:
					return VK_MBUTTON;
				case 3:
					return VK_XBUTTON1;
				case 4:
					return VK_XBUTTON2;
				}
			}
		}

		for (auto i = VK_BACK; i <= VK_RMENU; i++)
		{
			if (io.KeysDown[i])
				return i;
		}

		if (input_system::is_key_down(ImGuiKey_Escape))
			return 0;

		return -1;
	}

	int random(const int& min, const int& max)
	{
		return rand() % (max - min + 1) + min;
	}

	float random(const float& min, const float& max)
	{
		return ((max - min) * ((float)rand() / RAND_MAX)) + min;
	}

	bool is_sniper(int iItemDefinitionIndex)
	{
		return iItemDefinitionIndex == WEAPON_AWP || iItemDefinitionIndex == WEAPON_SSG08 ||
			iItemDefinitionIndex == WEAPON_SCAR20 || iItemDefinitionIndex == WEAPON_G3SG1;
	}

	bool is_connected()
	{
		return g::engine_client->IsInGame() && g::local_player && g::local_player->IsAlive();
	}

	struct hud_weapons_t
	{
		std::int32_t* get_weapon_count()
		{
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x20); //was 0x80
		}
	};

	void force_full_update()
	{
		static const auto full_update_fn = reinterpret_cast<void(*)(void)>(pattern_scan(FORCE_FULL_UPDATE));
		full_update_fn();

		if (!g::local_player || !g::local_player->IsAlive())
			return;

 		static auto clear_hud_weapon_icon_ptr = utils::pattern_scan(CLEAR_HUD_WEAPON_ICON);
 		static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clear_hud_weapon_icon_ptr);
 
		auto element = g::hud_system->FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");
 		auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0x9c); //was 0x9c //0x28

 		if (!hud_weapons || *hud_weapons->get_weapon_count() == 0)
 			return;
 
 		for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
 			i = clear_hud_weapon_icon_fn(hud_weapons, i);
	}

	bool is_line_goes_through_smoke(Vector vStartPos, Vector vEndPos)
	{
		static auto fn = reinterpret_cast<bool(*)(Vector, Vector)>(patterns::is_line_goes_through_smoke_fn_addr);

		return fn(vStartPos, vEndPos);
	}

	std::map<std::string, HMODULE> modules = {};

	HMODULE get_module(const std::string& name)
	{
		if (modules.count(name) == 0 || !modules[name])
			modules[name] = LI_FN(GetModuleHandleA).cached()(name.c_str());

		return modules[name];
	}

	std::uint8_t* pattern_scan(const char* moduleName, const char* signature)
	{
		return pattern_scan(get_module(moduleName), signature);
	}

	std::uint8_t* pattern_scan(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.emplace_back(-1);
				}
				else
					bytes.emplace_back(strtoul(current, &current, 16));
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}

			if (found)
				return &scanBytes[i];
		}
		return nullptr;
	}
}