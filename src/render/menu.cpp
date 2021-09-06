#include "render.h"

#include "../settings/globals.h"
#include "../settings/options.hpp"

#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../features/features.h"

#define VERSION "1.13.7"

static const char* const KeyNames[] = {
	"Unknown",
	"LBUTTON",
	"RBUTTON",
	"VK_CANCEL",
	"MBUTTON",
	"MOUSE4",
	"MOUSE5",
	"Unknown",
	"BACKSPACE",
	"TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"RETURN",
	"Unknown",
	"Unknown",
	"SHIFT",
	"CTRL",
	"ALT",
	"VK_PAUSE",
	"CAPSLOCK",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"SPACE",
	"VK_PRIOR",
	"NEXT",
	"END",
	"HOME",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"INSERT",
	"DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"MULTIPLY",
	"ADD",
	"SEPARATOR",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"NUMLOCK",
	"SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LSHIFT",
	"RSHIFT",
	"LCONTROL",
	"RCONTROL",
	"L-ALT",
	"R-ALT"
};

bool Hotkey(const char* label, int* k, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id, false);
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			*k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = *k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_OEM_8; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE)) //Ghetto method, but it works.
		{
			*k = 0;
			ImGui::ClearActiveID();
		}
		else {
			*k = key;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = "None";

	//ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 1.0f)), true, style.FrameRounding);
	ImGui::GetWindowDrawList()->AddRect(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonOutline)));

	if (*k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, KeyNames[*k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, "<Press a key>");
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;

	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
	ImGui::PopStyleColor();
	//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
	//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

void bind_button(const char* label, int& key)
{
	render::columns(2);
	{
		ImGui::Text(label);

		ImGui::NextColumn();

		ImGui::PushItemWidth(-1);

		char buf[1024];
		sprintf_s(buf, "##binds.%s", label);

		Hotkey(buf, &key, ImVec2(0.f, 0.f));

		ImGui::PopItemWidth();
	}
	render::columns(1);
}

namespace render
{
	namespace menu
	{
		auto current_tab = 0;
		auto s_visible = false;
		float window_alpha = 0.f;
		const auto window_size = ImVec2(720.f, 585.f); //680, 460

		bool is_visible()
		{
			return s_visible;
		}

		void toggle()
		{
			s_visible = !s_visible;
		}

		std::map<int, weapon_type_t> get_weapons(bool need_knife)
		{
			std::map<int, weapon_type_t> k_item_names =
			{
				{ 63,{ "CZ75 Auto", WEAPONTYPE_PISTOL } },
				{ 1,{ "Desert Eagle", WEAPONTYPE_PISTOL } },
				{ 2,{ "Dual Berettas", WEAPONTYPE_PISTOL } },
				{ 3,{ "Five-SeveN", WEAPONTYPE_PISTOL } },
				{ 32,{ "P2000", WEAPONTYPE_PISTOL } },
				{ 36,{ "P250", WEAPONTYPE_PISTOL } },
				{ 61,{ "USP-S", WEAPONTYPE_PISTOL } },
				{ 30,{ "Tec-9", WEAPONTYPE_PISTOL } },
				{ 64,{ "R8 Revolver", WEAPONTYPE_PISTOL } },
				{ 4,{ "Glock-18", WEAPONTYPE_PISTOL } },

				{ 27,{ "MAG-7", WEAPONTYPE_SHOTGUN } },
				{ 35,{ "Nova", WEAPONTYPE_SHOTGUN } },
				{ 29,{ "Sawed-Off", WEAPONTYPE_SHOTGUN } },
				{ 25,{ "XM1014", WEAPONTYPE_SHOTGUN } },

				{ 24,{ "UMP-45", WEAPONTYPE_SUBMACHINEGUN } },
				{ 19,{ "P90", WEAPONTYPE_SUBMACHINEGUN } },
				{ 26,{ "PP-Bizon", WEAPONTYPE_SUBMACHINEGUN } },
				{ 17,{ "MAC-10", WEAPONTYPE_SUBMACHINEGUN } },
				{ 33,{ "MP7", WEAPONTYPE_SUBMACHINEGUN } },
				{ 34,{ "MP9", WEAPONTYPE_SUBMACHINEGUN } },
				{ WEAPON_MP5SD,{ "MP5-SD", WEAPONTYPE_SUBMACHINEGUN } },

				{ 14,{ "M249", WEAPONTYPE_MACHINEGUN } },
				{ 28,{ "Negev", WEAPONTYPE_MACHINEGUN } },

				{ 7,{ "AK-47", WEAPONTYPE_RIFLE } },
				{ 8,{ "AUG", WEAPONTYPE_RIFLE } },
				{ 13,{ "Galil AR", WEAPONTYPE_RIFLE } },
				{ 60,{ "M4A1-S", WEAPONTYPE_RIFLE } },
				{ 16,{ "M4A4", WEAPONTYPE_RIFLE } },
				{ 39,{ "SG 553", WEAPONTYPE_RIFLE } },
				{ 10,{ "FAMAS", WEAPONTYPE_RIFLE } },

				{ 9,{ "AWP", WEAPONTYPE_SNIPER_RIFLE } },
				{ 11,{ "G3SG1", WEAPONTYPE_SNIPER_RIFLE } },
				{ 38,{ "SCAR-20", WEAPONTYPE_SNIPER_RIFLE } },
				{ 40,{ "SSG 08", WEAPONTYPE_SNIPER_RIFLE } },
			};

			if (need_knife)
			{
				k_item_names[WEAPON_KNIFE_T] = { "T", -1 };
				k_item_names[WEAPON_KNIFE] = { "CT", -1 };

				k_item_names[GLOVE_T_SIDE] = { "T", -2 };
				k_item_names[GLOVE_CT_SIDE] = { "CT", -2 };
			}

			return k_item_names;
		}

		std::map<int, const char*> get_groups(bool need_knife = false, bool need_groups = false)
		{
			std::map<int, const char*> groups =
			{
				{ WEAPONTYPE_PISTOL, "Pistols" },
				{ WEAPONTYPE_SHOTGUN, "Shotguns" },
				{ WEAPONTYPE_SUBMACHINEGUN, "Submachineguns" },
				{ WEAPONTYPE_MACHINEGUN, "Machineguns" },
				{ WEAPONTYPE_RIFLE, "Rifles" },
				{ WEAPONTYPE_SNIPER_RIFLE, "Snipers" },
			};

			if (need_knife)
			{
				groups[-1] = { "Knives" };
				groups[-2] = { "Gloves" };
			}

			if (need_groups)
			{
				groups[201] = "Desert Eagle";
				groups[240] = "SSG08";
				groups[209] = "AWP";
			}

			return groups;
		}

		bool selectable_weapons(
			int& selected_item,
			bool only_groups,
			std::string& weaponName,
			std::map<int, const char*> groups,
			std::map<int, weapon_type_t> k_item_names,
			std::vector<int> selected_weapons = {}
		)
		{
			if (!ImGui::BeginCombo("##aimbot.weapons", weaponName.c_str()))
				return false;

			auto current_item = selected_item;

			for (auto& group : groups)
			{
				if (only_groups)
				{
					if (selectable(group.second, group.first == selected_item))
						selected_item = group.first;

					if (group.first == selected_item)
						ImGui::SetItemDefaultFocus();
				}
				else
				{
					if (group.first == 201)
						break;

					if (group.first != WEAPONTYPE_PISTOL)
						ImGui::Separator();

					ImGui::TextDisabled(group.second);

					ImGui::Separator();

					for (auto& item : k_item_names)
					{
						if (item.second.type == group.first)
						{
							if (std::find(selected_weapons.begin(), selected_weapons.end(), item.first) != selected_weapons.end())
								continue;

							if (selectable(item.second.name, item.first == selected_item))
								selected_item = item.first;

							if (item.first == selected_item)
								ImGui::SetItemDefaultFocus();
						}
					}
				}
			}

			ImGui::EndCombo();

			return current_item != selected_item;
		}

		bool listbox_group_weapons(
			int& selected_item,
			std::map<int, const char*> groups,
			std::map<int, weapon_type_t> items,
			ImVec2 listbox_size,
			bool show_only_selected = false,
			std::vector<int> selected_weapons = { }
		)
		{
			auto current_value = selected_item;

			ImGui::ListBoxHeader("##items", listbox_size);
			{
				for (auto& group : groups)
				{
					ImGui::TextDisabled(group.second);
					ImGui::Separator();

					auto has_items = false;

					for (auto& item : items)
					{
						if (item.second.type == group.first)
						{
							if (show_only_selected)
								if (std::find(selected_weapons.begin(), selected_weapons.end(), item.first) == selected_weapons.end())
									continue;

							has_items = true;

							char name[128];
							sprintf(name, "%s##%d", item.second.name, item.first);
							if (selectable(name, item.first == selected_item))
								selected_item = item.first;
						}
					}

					if (has_items)
						ImGui::Separator();
				}
			}
			ImGui::ListBoxFooter();

			return current_value != selected_item;
		}

		void render_header()
		{
			auto win_pos = ImGui::GetWindowPos();
			auto draw_list = ImGui::GetWindowDrawList();

			if (!render::fonts::header_title || !render::fonts::header_buttons)
				return;

			ImVec2 title_size;
			ImGui::PushFont(render::fonts::header_title);
			{
				static const auto white = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 1.f));

				title_size = ImGui::CalcTextSize("Sensum");
				const auto rect_start = ImVec2(win_pos.x, win_pos.y);
				const auto rect_end = ImVec2(win_pos.x + window_size.x, win_pos.y + title_size.y + 16.f);
				const auto rect_height = rect_end.y - rect_start.y;

				draw_list->AddRectFilled(rect_start, rect_end, ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f)));

				ImGui::SetCursorPosX(10.f);
				ImGui::SetCursorPosY(rect_start.y + rect_height / 2 - win_pos.y - title_size.y / 2);

				static float _rainbow = 0.f;
				if (_rainbow > 1.f)
					_rainbow = 0.f;

				auto cursor = ImGui::GetCursorPos();

				ImGui::Text("");

				auto rainbow = _rainbow;

				for (auto& letter : "Sensum")
				{
					const auto rgb_color = Color::FromHSB(rainbow, 1.f, 1.f);

					auto color = ImVec4(rgb_color.r() / 255.f, rgb_color.g() / 255.f, rgb_color.b() / 255.f, 1.f);

					char buf[4];
					sprintf_s(buf, "%c", letter);

					draw_list->AddText(win_pos + cursor, ImGui::GetColorU32(color), buf);

					cursor.x += 0.5f + ImGui::CalcTextSize(buf).x;

					rainbow += 0.0025f;

					if (rainbow > 1.f)
						rainbow = 0.f;
				}

				_rainbow += 0.0007f;
			}
			ImGui::PopFont();

			ImGui::SameLine();

			ImGui::PushFont(render::fonts::header_buttons);
			{
				const auto menu_items = std::vector<std::string>
				{
					"Visuals",
					"Aimbot",
					"Misc",
					"Skins",
					"Players",
					"Colors",
					"Configs"
				};

				const auto menu_items_count = menu_items.size();

				const auto button_width = 72.f;
				const auto total_button_space = button_width * menu_items_count + 5.f * menu_items_count;
				ImGui::SetCursorPosX(window_size.x - total_button_space);

				ImGui::PushStyleColor(ImGuiCol_ButtonText, ImVec4(1.f, 1.f, 1.f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.1f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonOutline, ImVec4(0.8f, 0.1f, 0.1f, 1.f));

				auto& button_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

				for (size_t k = 0; k < menu_items_count; k++)
				{
					const auto selected = current_tab == k;
					if (selected)
						ImGui::PushStyleColor(ImGuiCol_Button, button_color);

					if (ImGui::Button(menu_items[k].c_str(), ImVec2(button_width, title_size.y)))
						current_tab = k;

					if (selected)
						ImGui::PopStyleColor();

					if (k != menu_items_count - 1)
						ImGui::SameLine();
				}

				ImGui::PopStyleColor(4);
			}
			ImGui::PopFont();
		}

		void background()
		{
			//nice bg
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.09f, 0.09f, 0.40f / 1.f * window_alpha));
			static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
			ImGui::Begin("##background", nullptr, flags);
			ImGui::End();
			ImGui::PopStyleColor();
		}

		void render_bottom()
		{
			ImVec2 win_pos = ImGui::GetWindowPos();

			static auto copyright = "Xardch & MartiNJ409";

#ifdef _DEBUG
			copyright = "Xardch & MartiNJ409 | Debug Mode - Expect problems!";
#endif

			static const auto copyright_size = ImGui::CalcTextSize(copyright);

			static char buildtime[256];
			sprintf(buildtime, "Built on %s | Version %s", __DATE__, VERSION);
			static const auto buildtime_size = ImGui::CalcTextSize(buildtime);
			static const auto name_size = ImGui::CalcTextSize("Sensum");

			auto draw_list = ImGui::GetWindowDrawList();
			const auto bottom_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f));
			const auto text_color = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 1.f));
			const auto start = ImVec2(win_pos.x, win_pos.y + window_size.y);
			const auto end = ImVec2(start.x + window_size.x, start.y - copyright_size.y - 4.f);

			draw_list->AddRectFilled(start, end, bottom_color);
			draw_list->AddText(ImVec2(start.x + 10.f, start.y - copyright_size.y - 2.f), text_color, copyright);
			draw_list->AddText(ImVec2(end.x - 10.f - buildtime_size.x, start.y - buildtime_size.y - 2.f), text_color, buildtime);
		}

		void show()
		{
			if (s_visible)
			{
				if (window_alpha < 1.f)
					window_alpha += 0.04f;
			}
			else
			{
				window_alpha -= 0.04f;

				if (window_alpha <= 0.f)
					return;
			}

			if (window_alpha <= 0.f)
				window_alpha = 0.01f;
			else if (window_alpha > 1.f)
				window_alpha = 1.f;

			background();

			const auto old_alpha = ImGui::GetStyle().Alpha;

			ImGui::GetStyle().Alpha = window_alpha;

			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

			auto* style = &ImGui::GetStyle();
			const auto old_window_padding = style->WindowPadding;
			style->WindowPadding = ImVec2(0, 0);

			static auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar; //ImGuiWindowFlags_NoMove
			ImGui::Begin("Sensum", nullptr, flags);
			{
				render_header();

				style->WindowPadding = old_window_padding;

				ImGui::BeginChild("##content", ImVec2(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
				{
					columns(current_tab == 4 ? 1 : 3);
					{
						if (current_tab == 0)
							visuals_tab();
						else if (current_tab == 1)
							aimbot_tab();
						else if (current_tab == 2)
							misc_tab();
						else if (current_tab == 3)
							skins_tab();
						else if (current_tab == 4)
							players_tab();
						else if (current_tab == 5)
							colors_tab();
						else if (current_tab == 6)
							configs_tab();
					}
					columns(1);
				}
				ImGui::EndChild();

				render_bottom();
			}
			ImGui::End();

			ImGui::GetStyle().Alpha = old_alpha;
		}
	}
}