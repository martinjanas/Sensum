#include "utils.h"
#include "math.h"
#include "input.h"
#include "console.h"
#include "../settings/globals.h"
#include "../security/importer.h"
#include "../features/features.h"
#include "../imgui/imgui_internal.h"
#include "../valve_sdk/csgostructs.hpp"
#include "..//render/render.h"
#include "../features/esp.hpp"

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include <chrono>

namespace utils
{
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
		c_base_player* plocal = (c_base_player*)g::entity_list->GetClientEntity(g::engine_client->GetLocalPlayer());

		if (!plocal)
			return Vector(0, 0, 0);

		QAngle vAngle = QAngle(0, 0, 0);

		g::engine_client->GetViewAngles(vAngle);

		float range = 5.f;

		float r_1, r_2;
		float x_1, y_1;

		Vector LocalRendOrig = plocal->GetRenderOrigin();
		Vector LocalViewOfst = plocal->m_vecViewOffset();

		Vector vEyeOrigin = LocalRendOrig + LocalViewOfst;

		r_1 = -(target.y - vEyeOrigin.y);
		r_2 = target.x - vEyeOrigin.x;
		float Yaw = vAngle.yaw - 90.0f;

		float yawToRadian = Yaw * (float)(M_PI / 180.0F);
		x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20.f;
		y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20.f;

		x_1 *= range;
		y_1 *= range;

		return Vector(x_1, y_1, 0);
	}

	Vector CalcDir(const Vector& vAngles)
	{
		Vector vForward;
		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(vAngles[1]));
		cy = cos(DEG2RAD(vAngles[1]));

		sp = sin(DEG2RAD(vAngles[0]));
		cp = cos(DEG2RAD(vAngles[0]));

		vForward.x = cp * cy;
		vForward.y = cp * sy;
		vForward.z = -sp;

		return vForward;
	}

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

				if (!strcmp(server, "loopback"))
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

	unsigned int GetVirtual(void* class_, unsigned int index) {
		return (unsigned int)(*(int**)class_)[index];
	}

	std::string get_weapon_name(void* weapon)
	{
		static const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));

		if (!weapon)
			return "";

		const auto wide_name = g::localize->Find(((c_base_combat_weapon*)weapon)->get_weapon_data()->szHudName);

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
	}

	bool hitchance(c_base_entity* entity, const QAngle& angles, const float& chance, const float& hit_count, const int& hit_group)
	{
		if (!g::local_player)
			return false;

		auto weapon = g::local_player->m_hActiveWeapon();
		if (!weapon || !weapon->IsWeapon())
			return false;

		Vector forward, right, up;
		Vector src = g::local_player->GetEyePos();
		math::angle2vectors(angles, forward, right, up);

		int cHits = 0;
		int cNeededHits = static_cast<int>(hit_count * (chance / 100.f));

		weapon->UpdateAccuracyPenalty();
		float weap_spread = weapon->GetSpread();
		float weap_inaccuracy = weapon->GetInaccuracy();

		Ray_t ray;
		trace_t tr;
		CTraceFilterPlayersOnlySkipOne filter(g::local_player);

		for (int i = 0; i < hit_count; i++)
		{
			float a = utils::random(0.f, 1.f);
			float b = utils::random(0.f, 2.f * M_PI);
			float c = utils::random(0.f, 1.f);
			float d = utils::random(0.f, 2.f * M_PI);

			float inaccuracy = a * weap_inaccuracy;
			float spread = c * weap_spread;

			if (weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
			{
				a = 1.f - a * a;
				a = 1.f - c * c;
			}

			Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

			direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
			direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
			direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
			direction.Normalized();

			QAngle viewAnglesSpread;
			math::vector2angles(direction, up, viewAnglesSpread);
			viewAnglesSpread.NormalizeClamp();

			Vector viewForward;
			math::angle2vectors(viewAnglesSpread, viewForward);
			viewForward.NormalizeInPlace();

			viewForward = src + (viewForward * weapon->get_weapon_data()->flRange);

			ray.Init(src, viewForward);
			g::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

			if (tr.hit_entity == entity && (hit_group == -1 || hit_group == tr.hitgroup))
				++cHits;

			const auto ch = static_cast<int>((static_cast<float>(cHits) / hit_count) * 100.f);
			if (ch >= chance)
				return true;

			if ((hit_count - i + cHits) < cNeededHits)
				return false;
		}

		return false;
	}

	bool can_lag(const bool& voice_check)
	{
		if (g::global_vars->interval_per_tick * 0.9f < g::global_vars->absoluteframetime)
			return false;

		if (voice_check && g::engine_client->IsVoiceRecording())
			return false;

		auto* channel_info = g::engine_client->GetNetChannelInfo();
		if (channel_info && (channel_info->GetAvgLoss(1) > 0.f || channel_info->GetAvgLoss(0) > 0.f))
			return false;

		return true;
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
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};

	void force_full_update()
	{
		return;
		static const auto full_update_fn = reinterpret_cast<void(*)(void)>(pattern_scan(FORCE_FULL_UPDATE));
		full_update_fn();

		g::client_state->ForceFullUpdate(); //edit
		return;
		if (g::local_player)
		{
			g::client_state->ForceFullUpdate(); //edit
			g::local_player->PostDataUpdate(0); //edit
		}

		if (!g::local_player || !g::local_player->IsAlive())
			return;

		static auto clear_hud_weapon_icon_ptr = utils::pattern_scan(CLEAR_HUD_WEAPON_ICON);
		static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clear_hud_weapon_icon_ptr);

		auto element = g::hud_system->FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");
		auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0x9c);
		if (!hud_weapons || *hud_weapons->get_weapon_count() == 0)
			return;

		for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
			i = clear_hud_weapon_icon_fn(hud_weapons, i);
	}

	bool is_line_goes_through_smoke(Vector vStartPos, Vector vEndPos)
	{
		static auto fn = reinterpret_cast<bool(*)(Vector, Vector)>(pattern_scan(IS_LINE_GOES_THROUGH_SMOKE_FN));

		return fn(vStartPos, vEndPos);
	}

	void set_player_name(const char* name)
	{
		globals::playername = name;

		static auto nameConvar = g::cvar->find("name");
		nameConvar->m_fnChangeCallbacks.m_Size = 0;

		nameConvar->SetValue(name);
	}

	std::map<std::string, HMODULE> modules = {};

	HMODULE get_module(const std::string& name)
	{
		const auto module_name = name == "client.dll" ? "client.dll" : name;

		if (modules.count(module_name) == 0 || !modules[module_name])
			modules[module_name] = LI_FN(GetModuleHandleA).cached()(module_name.c_str());

		return modules[module_name];
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

	void RankRevealAll()
	{
		using ServerRankRevealAll = char(__cdecl*)(int*);

		static uint8_t* fnServerRankRevealAll = utils::pattern_scan(("client.dll"), "55 8B EC 51 A1 ? ? ? ? 85 C0 75 37");

		if (fnServerRankRevealAll)
		{
			int v[3] = { 0,0,0 };

			reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
		}
	}
}