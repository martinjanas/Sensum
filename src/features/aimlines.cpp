#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.h"
#include "../settings/globals.h"
#include "../imgui/imgui.h"

namespace features
{
	void aimlines(ImDrawList* draw_list) //I need to figure out how to make the lines not render through walls.
	{
		CGameTrace trace, _trace;
		CTraceFilter filter;
		Ray_t ray, _ray;

		Vector start, end, forward;
		QAngle entity_angles;

		if (!g::local_player)
			return;

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		float multiplier = 100.f;

		for (int i = 1; i < g::engine_client->GetMaxClients(); ++i)
		{
			c_base_player* player = c_base_player::GetPlayerByIndex(i);

			if (player && player->IsPlayer() && player != g::local_player && player->IsAlive() && !player->IsDormant() && (player->m_iTeamNum() != g::local_player->m_iTeamNum()))
			{
				float dist = g::local_player->m_vecOrigin().DistTo(player->m_vecOrigin());

				if (dist <= 1000.f)
				{
					math::angle2vectors(player->m_angEyeAngles(), forward);

					filter.pSkip = player;

					Vector head = player->get_hitbox_position(player, HITBOX_HEAD);

					_ray.Init(head, head + (forward * 8192.f));
					g::engine_trace->trace_ray(_ray, MASK_SOLID, &filter, &_trace);

					if (_trace.fraction != 1.f)
					{
						multiplier = head.DistTo(_trace.endpos);
					}

					Vector start = head;
					Vector end = start + (forward * multiplier);

					ray.Init(start, end);
					g::engine_trace->trace_ray(ray, MASK_SOLID, &filter, &trace);

					g::debug_overlay->AddLineOverlay(trace.startpos, trace.endpos, 255, 0, 0, true, -1.f);
					g::debug_overlay->AddBoxOverlay(trace.endpos, Vector(-2, -2, -2), Vector(2, 2, 2), player->m_angEyeAngles(), 0, 255, 0, 127, -1.f);
				}
			}
		}
	}
}