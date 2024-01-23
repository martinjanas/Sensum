#include <algorithm>

#include "features.h"
#include "../settings/globals.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"
#include "../helpers/console.h"
#include "../helpers/entities.h"
#include "../helpers/autowall.h"
#include "../valve_sdk/interfaces/ISurface.h"
#include "../render/fonts/undefeated.hpp"
#include "../helpers/entities.h"

namespace esp
{
	decltype(entities::m_local) m_local;
	entities::player_data_t m_entities[MAX_PLAYERS];

	static std::string output;

	float last_time = 0.f;

	Vector origin, in_parent, in_child, out_parent, out_child, breast_bone;

	bool is_enabled()
	{
		if (!render::fonts::visuals || g::engine_client->IsConsoleVisible() || g::engine_client->IsDrawingLoadingImage() || (!globals::esp_menu_opened && render::menu::is_visible()))
			return false;

		return g::engine_client->IsInGame() && g::local_player && settings::esp::enabled;
	}

	void render_helpers()
	{
		if (settings::misc::smoke_helper)
		{
			visuals::RenderInfo();
			visuals::RenderCircle();
			visuals::drawring_3d();
		}

		if (settings::misc::flash_helper)
		{
			visuals::RenderCirclePopflash();
			visuals::drawring_3d_popflash();
		}
	}

	void render(ImDrawList* draw_list)
	{
		if (!is_enabled())
			return;

		if (entities::locker.try_lock())
		{
			memcpy(m_entities, entities::m_items.front().players, sizeof(m_entities));
			entities::locker.unlock();
		}

		if (entities::local_mutex.try_lock())
		{
			m_local = entities::m_local;
			entities::local_mutex.unlock();
		}

		int x, y;

		g::engine_client->GetScreenSize(x, y);

		int xx = x / 2;
		int yy = y / 2;

		QAngle viewangles;
		g::engine_client->GetViewAngles(viewangles);

		static const auto white_color = ImGui::GetColorU32(ImVec4::White);
		static const auto smoke_color = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 0.4f));
		static const auto orange_color = ImGui::GetColorU32(ImVec4::Orange);
		static const auto green_color = ImGui::GetColorU32(ImVec4::Green);
		static const auto black_color = ImGui::GetColorU32(ImVec4::Black);
		static const auto yellow_color = ImGui::GetColorU32(ImVec4::Yellow);
		static const auto red_color = ImGui::GetColorU32(ImVec4::Red);

		ImGui::PushFont(render::fonts::visuals);
		
		for (const auto& data : m_entities)
		{
			if (data.index == 0 || !data.hitboxes->points[0].IsValid())
				continue; 

			const auto bbox = utils::get_box(data.points);
		
			const auto on_screen = (bbox.left > 0 || bbox.right > 0) && (bbox.top > 0 || bbox.bottom > 0);
			
			if (!on_screen)
				continue;

			if (settings::esp::offscreen && !render::menu::is_visible() && !data.is_dormant)
			{
				if (!on_screen && utils::is_entity_audible(data.index, data.origin))
				{
					utils::render_dot(draw_list, data.origin, viewangles, IM_COL32(0, 255, 0, 255));
				}
				else if (m_local.is_scoped)
				{
					utils::render_dot(draw_list, data.origin, viewangles, IM_COL32(255, 0, 50, 255));
				}
				else utils::render_dot(draw_list, data.origin, viewangles, utils::is_entity_audible(data.index, data.origin) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 50, 255));
			}

			if (settings::esp::visible_on_sound)
			{
				if (settings::esp::visible_only)
				{
					if (utils::is_entity_audible(data.index, data.origin) && (m_local.is_flashed || data.in_smoke))
						continue;
					else if (!utils::is_entity_audible(data.index, data.origin) && (!data.is_visible || m_local.is_flashed || data.in_smoke))
						continue;
				}
			}
			else 
			{
				if (settings::esp::visible_only && (!data.is_visible || data.in_smoke || m_local.is_flashed))
				    continue;
			}

			const auto visible_color = utils::to_im32(settings::esp::visible_color);
			const auto occluded_color = utils::to_im32(settings::esp::occluded_color);

			const auto width = bbox.right - bbox.left;
			const auto height = bbox.bottom - bbox.top;

			const auto box_color = data.is_dormant ? smoke_color : data.is_visible && !data.in_smoke && !m_local.is_flashed ? visible_color : occluded_color;

			if (settings::esp::bone_esp)
			{
				for (int j = 0; j < data.hdr->numbones; j++)
				{
					mstudiobone_t* bone = data.hdr->GetBone(j);

					if (!bone)
						continue;

					if (!(bone->flags & BONE_USED_BY_HITBOX))
						continue;

					if (bone->parent != -1)
					{
						in_child = data.player->get_bone_position(j);
						in_parent = data.player->get_bone_position(bone->parent);

						Vector upper_dir = data.player->get_bone_position(6 + 1) - data.player->get_bone_position(6); //6 = chest bone
						breast_bone = data.player->get_bone_position(6) + upper_dir / 2;
						Vector delta_child = in_child - breast_bone;
						Vector delta_parent = in_parent - breast_bone;

						if ((delta_parent.Length() < 9 && delta_child.Length() < 9))
							in_parent = breast_bone;

						if (j == 6 - 1)
							in_child = breast_bone;

						if (std::abs(delta_child.z) < 5 && (delta_parent.Length() < 5 && delta_child.Length() < 5) || j == 6)
							continue;

						if ((bone->flags & BONE_USED_BY_HITBOX ^ BONE_USED_BY_HITBOX) && (delta_parent.Length() < 19 && delta_child.Length() < 19))
							continue;

						if (math::world2screen(in_parent, out_parent) && math::world2screen(in_child, out_child))
							globals::draw_list->AddLine(ImVec2(out_parent.x, out_parent.y), ImVec2(out_child.x, out_child.y), utils::to_im32(settings::esp::bone_esp_color));
					}
				}
			}

			if (settings::esp::names)
			{
				const auto name_size = ImGui::CalcTextSize(data.name.c_str());
				auto x = bbox.left + width / 2.f - name_size.x / 2.f;

				imdraw::outlined_text( data.name.c_str(), ImVec2(x, bbox.top - name_size.y), data.is_dormant ? smoke_color : white_color);
			}

			if (settings::esp::is_scoped && data.is_scoped)
			{
				static const auto scoped_text_size = ImGui::CalcTextSize("Scoped");
				auto x = bbox.left + width / 2.f - scoped_text_size.x / 2.f;

				imdraw::outlined_text("Scoped", ImVec2(x, bbox.top - scoped_text_size.y - 20.f), white_color);
			}

			if (settings::esp::is_flashed && data.is_flashed)
			{
				static const auto flashed_text_size = ImGui::CalcTextSize("Flashed");
				auto x = bbox.left + width / 2.f - flashed_text_size.x / 2.f;

				imdraw::outlined_text("Flashed", ImVec2(x, bbox.top - flashed_text_size.y - 12.f), yellow_color);
			}

			if (settings::esp::bomb_esp && data.is_c4_carrier)
			{
				static const auto defusing_text_size = ImGui::CalcTextSize("C4");

				imdraw::outlined_text("C4", ImVec2(bbox.right + 2.f, bbox.top - defusing_text_size.y + 2.f), green_color);
			}

			if (settings::esp::ammo && !data.is_knife_or_grenade)
			{
				static char buf[16];
				sprintf_s(buf, "(%i/%i)", data.m_iAmmo, data.m_MaxAmmo);

				const auto weapon_size = ImGui::CalcTextSize(buf);

				auto y_pos = bbox.bottom + 12.f;
				if (settings::esp::health && settings::esp::health_position == 2)
					y_pos += 7.f;

				if (settings::esp::armour && settings::esp::armour_position == 2)
					y_pos += 7.f;

				imdraw::outlined_text(buf, ImVec2(bbox.left + width / 2.f - weapon_size.x / 2.f, y_pos), white_color);
			}

			auto render_line = [bbox, draw_list](const Color& color, const int& value, const int& position, const bool& with_offset = false, const int& offset_position = 0)
			{
				const auto im_color = utils::to_im32(color);

				const auto width = (float(fabs(bbox.right - bbox.left))) / 100.f * (100.f - value);
				const auto height = (float(fabs(bbox.bottom - bbox.top))) / 100.f * (100.f - value);

				//black width + offset between lines
				const auto offset = with_offset && position == offset_position ? 7.f : 0.f;
				if (position == 0)
				{
					draw_list->AddRectFilled(ImVec2(bbox.left - 2.f - offset, bbox.top), ImVec2(bbox.left - 7.f - offset, bbox.bottom), black_color);
					draw_list->AddRectFilled(ImVec2(bbox.left - 3.f - offset, bbox.top + height), ImVec2(bbox.left - 6.f - offset, bbox.bottom), im_color);
				}
				else if (position == 1)
				{
					draw_list->AddRectFilled(ImVec2(bbox.right + 2.f + offset, bbox.top), ImVec2(bbox.right + 7.f + offset, bbox.bottom), black_color);
					draw_list->AddRectFilled(ImVec2(bbox.right + 3.f + offset, bbox.top + height), ImVec2(bbox.right + 6.f + offset, bbox.bottom), im_color);
				}
				else if (position == 2)
				{
					draw_list->AddRectFilled(ImVec2(bbox.left, bbox.bottom + 2.f + offset), ImVec2(bbox.right, bbox.bottom + 7.f + offset), black_color);
					draw_list->AddRectFilled(ImVec2(bbox.left, bbox.bottom + 3.f + offset), ImVec2(bbox.right - width, bbox.bottom + 6.f + offset), im_color);
				}
			};

			if (settings::esp::health)
			{
				float r, g, b;

				utils::scale_color_to_health(std::clamp(data.m_iHealth / 100.0f, 0.0f, 1.0f), r, g, b);

				render_line(Color(r, g, b, 1.0f), data.m_iHealth, settings::esp::health_position, false);
			}

			if (settings::esp::armour)
			{
				render_line(Color(0, 50, 255, 255), data.m_ArmorValue, settings::esp::armour_position, settings::esp::health, settings::esp::health_position);
			}

			if (settings::esp::weapons && !data.weapon.empty())
			{
				const auto weapon_size = ImGui::CalcTextSize(data.weapon.c_str());

				const auto icon_size = ImGui::CalcTextSize(data.icon.c_str());

				if (!render::fonts::weapon_icon)
					return;

				auto y_pos = bbox.bottom + 2.f;
				if (settings::esp::health && settings::esp::health_position == 2)
					y_pos += 7.f;

				if (settings::esp::armour && settings::esp::armour_position == 2)
					y_pos += 7.f;

				switch (settings::esp::weapon_mode)
				{
				case 0:
					imdraw::outlined_text(data.weapon.c_str(), ImVec2(bbox.left + width / 2.f - weapon_size.x / 2.f, y_pos), data.is_dormant ? smoke_color : orange_color);
					break;
				case 1:
					ImGui::PushFont(render::fonts::weapon_icon);
					imdraw::outlined_text(data.icon.c_str(), ImVec2(bbox.left + width / 2.f - data.wep_str_size / 2.f, y_pos), white_color);
					ImGui::PopFont();
					break;
				}
			}

			if (settings::esp::boxes)
			{
				static const auto thickness = 1.f;
				if (settings::esp::box_type == EBoxType::Normal)
					draw_list->AddRect(ImVec2(bbox.left, bbox.top), ImVec2(bbox.right, bbox.bottom), box_color, thickness, 15, 1.5f);
				else
				{
					static const auto delta = 5.f;

					std::pair<ImVec2, ImVec2> points[] =
					{
						{ImVec2(bbox.left, bbox.top), ImVec2(bbox.left + width / delta, bbox.top)}, //left top
						{ImVec2(bbox.left, bbox.bottom), ImVec2(bbox.left + width / delta, bbox.bottom)}, //left bottom

						{ImVec2(bbox.right, bbox.top), ImVec2(bbox.right - width / delta, bbox.top) }, //right top
						{ImVec2(bbox.right, bbox.bottom), ImVec2(bbox.right - width / delta, bbox.bottom)}, //right bottom

						{ImVec2(bbox.left, bbox.top), ImVec2(bbox.left, bbox.top + height / delta)}, //left top-bottom
						{ImVec2(bbox.left, bbox.bottom), ImVec2(bbox.left, bbox.bottom - height / delta)}, //left bottom-top

						{ImVec2(bbox.right, bbox.top), ImVec2(bbox.right, bbox.top + height / delta)}, //right top-bottom
						{ImVec2(bbox.right, bbox.bottom), ImVec2(bbox.right, bbox.bottom - height / delta)} //right bottom-top
					};

					for (const auto& point : points)
						draw_list->AddLine(point.first, point.second, box_color, thickness);
				}
			}
		}
		ImGui::PopFont();
	}
}
