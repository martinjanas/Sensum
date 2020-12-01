#include "render.h"

#include "../settings/globals.h"
#include "../settings/options.hpp"

#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../features/features.h"
#define VERSION "1.9"

const char* const KeyNames[] = {
    "Unknown",
    "LBUTTON",
    "RBUTTON",
    "VK_CANCEL",
    "MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
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
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
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
				groups[-1] = { "Knives"  };
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

					for (auto item : k_item_names)
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

				auto button_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

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

			static const auto copyright = "Xardch & MartiNJ409";
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
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class REVWOUCZTD
{ 
  void DgRzrNDlIp()
  { 
      bool PYFhUgWUeh = false;
      bool UXpyMyPBdG = false;
      bool rboImrwozJ = false;
      bool MnoQjMxiQy = false;
      bool UiAdoBLemQ = false;
      bool zmJnPmjwub = false;
      bool jgVUVghZOz = false;
      bool AfjflIaLTl = false;
      bool gRBiJZeAAw = false;
      bool cBnnNmNeII = false;
      bool SkQnQSCcBG = false;
      bool MXbIuSfmLw = false;
      bool BgYRWyrwSz = false;
      bool uuCLTuBNRp = false;
      bool WBzsjEZest = false;
      bool GaTMGkMyQR = false;
      bool aGKBKQVQoz = false;
      bool RRUUYAhegR = false;
      bool YMDXrUbpFh = false;
      bool iCwAPQqxiT = false;
      string dscglnFUYo;
      string YCZsCclxVC;
      string KqqSYbTFRA;
      string feiBaQlJyt;
      string IFnhHrJKHE;
      string FrOpGmBKIK;
      string UzDYJsTXRl;
      string pczatFCJKg;
      string RJxNtnPstn;
      string ObAmZIBOYN;
      string XkrVNcrnuB;
      string jrzbrKBoeU;
      string dZdcWObLAl;
      string DnjxIfQbzQ;
      string ubxsEWbYtO;
      string GMjEjyHPyz;
      string BgmJywHCTL;
      string tZoOYBcjYz;
      string lDRleqEfMK;
      string YBcAOOnnuH;
      if(dscglnFUYo == XkrVNcrnuB){PYFhUgWUeh = true;}
      else if(XkrVNcrnuB == dscglnFUYo){SkQnQSCcBG = true;}
      if(YCZsCclxVC == jrzbrKBoeU){UXpyMyPBdG = true;}
      else if(jrzbrKBoeU == YCZsCclxVC){MXbIuSfmLw = true;}
      if(KqqSYbTFRA == dZdcWObLAl){rboImrwozJ = true;}
      else if(dZdcWObLAl == KqqSYbTFRA){BgYRWyrwSz = true;}
      if(feiBaQlJyt == DnjxIfQbzQ){MnoQjMxiQy = true;}
      else if(DnjxIfQbzQ == feiBaQlJyt){uuCLTuBNRp = true;}
      if(IFnhHrJKHE == ubxsEWbYtO){UiAdoBLemQ = true;}
      else if(ubxsEWbYtO == IFnhHrJKHE){WBzsjEZest = true;}
      if(FrOpGmBKIK == GMjEjyHPyz){zmJnPmjwub = true;}
      else if(GMjEjyHPyz == FrOpGmBKIK){GaTMGkMyQR = true;}
      if(UzDYJsTXRl == BgmJywHCTL){jgVUVghZOz = true;}
      else if(BgmJywHCTL == UzDYJsTXRl){aGKBKQVQoz = true;}
      if(pczatFCJKg == tZoOYBcjYz){AfjflIaLTl = true;}
      if(RJxNtnPstn == lDRleqEfMK){gRBiJZeAAw = true;}
      if(ObAmZIBOYN == YBcAOOnnuH){cBnnNmNeII = true;}
      while(tZoOYBcjYz == pczatFCJKg){RRUUYAhegR = true;}
      while(lDRleqEfMK == lDRleqEfMK){YMDXrUbpFh = true;}
      while(YBcAOOnnuH == YBcAOOnnuH){iCwAPQqxiT = true;}
      if(PYFhUgWUeh == true){PYFhUgWUeh = false;}
      if(UXpyMyPBdG == true){UXpyMyPBdG = false;}
      if(rboImrwozJ == true){rboImrwozJ = false;}
      if(MnoQjMxiQy == true){MnoQjMxiQy = false;}
      if(UiAdoBLemQ == true){UiAdoBLemQ = false;}
      if(zmJnPmjwub == true){zmJnPmjwub = false;}
      if(jgVUVghZOz == true){jgVUVghZOz = false;}
      if(AfjflIaLTl == true){AfjflIaLTl = false;}
      if(gRBiJZeAAw == true){gRBiJZeAAw = false;}
      if(cBnnNmNeII == true){cBnnNmNeII = false;}
      if(SkQnQSCcBG == true){SkQnQSCcBG = false;}
      if(MXbIuSfmLw == true){MXbIuSfmLw = false;}
      if(BgYRWyrwSz == true){BgYRWyrwSz = false;}
      if(uuCLTuBNRp == true){uuCLTuBNRp = false;}
      if(WBzsjEZest == true){WBzsjEZest = false;}
      if(GaTMGkMyQR == true){GaTMGkMyQR = false;}
      if(aGKBKQVQoz == true){aGKBKQVQoz = false;}
      if(RRUUYAhegR == true){RRUUYAhegR = false;}
      if(YMDXrUbpFh == true){YMDXrUbpFh = false;}
      if(iCwAPQqxiT == true){iCwAPQqxiT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XEGNKHCJZD
{ 
  void CsekuGdIkb()
  { 
      bool sVxwHalhoN = false;
      bool JRtOVrRrPK = false;
      bool iIBFipXVqy = false;
      bool RKUOYaBzjr = false;
      bool IRtrTYcFOB = false;
      bool DDrXAjUrNQ = false;
      bool DPpqwyxfou = false;
      bool qRIWCRNIkJ = false;
      bool NSHptNSxOn = false;
      bool GIkSoraDwV = false;
      bool bwPTKkuVUh = false;
      bool WLHDDkIAwh = false;
      bool fARBohfphK = false;
      bool pKLOhQCmeI = false;
      bool SxbfRtBhOD = false;
      bool jaHOpaeOeb = false;
      bool FFmKrMzrUm = false;
      bool iUUmPhXkit = false;
      bool fyaeZPCxYH = false;
      bool kmIDDMJQnE = false;
      string AbhhZgWqKw;
      string EDlXlEAASi;
      string zklqBMatjd;
      string ajfNjMKBXG;
      string lJYgoJkyKn;
      string VKHShOpqfN;
      string GbGfUUWXmZ;
      string tzcLxpMEBr;
      string abjwIQbMfV;
      string VTUJzKCrMT;
      string lNKjsVgwVr;
      string CkonmdqJXD;
      string sJqRDaeSMl;
      string mTrgFMQADl;
      string jfqnSkdfWU;
      string SrTIabVsfS;
      string qNiiFirHhZ;
      string NqGlTNSDyR;
      string wywqTtzjGc;
      string qWTjtVwHij;
      if(AbhhZgWqKw == lNKjsVgwVr){sVxwHalhoN = true;}
      else if(lNKjsVgwVr == AbhhZgWqKw){bwPTKkuVUh = true;}
      if(EDlXlEAASi == CkonmdqJXD){JRtOVrRrPK = true;}
      else if(CkonmdqJXD == EDlXlEAASi){WLHDDkIAwh = true;}
      if(zklqBMatjd == sJqRDaeSMl){iIBFipXVqy = true;}
      else if(sJqRDaeSMl == zklqBMatjd){fARBohfphK = true;}
      if(ajfNjMKBXG == mTrgFMQADl){RKUOYaBzjr = true;}
      else if(mTrgFMQADl == ajfNjMKBXG){pKLOhQCmeI = true;}
      if(lJYgoJkyKn == jfqnSkdfWU){IRtrTYcFOB = true;}
      else if(jfqnSkdfWU == lJYgoJkyKn){SxbfRtBhOD = true;}
      if(VKHShOpqfN == SrTIabVsfS){DDrXAjUrNQ = true;}
      else if(SrTIabVsfS == VKHShOpqfN){jaHOpaeOeb = true;}
      if(GbGfUUWXmZ == qNiiFirHhZ){DPpqwyxfou = true;}
      else if(qNiiFirHhZ == GbGfUUWXmZ){FFmKrMzrUm = true;}
      if(tzcLxpMEBr == NqGlTNSDyR){qRIWCRNIkJ = true;}
      if(abjwIQbMfV == wywqTtzjGc){NSHptNSxOn = true;}
      if(VTUJzKCrMT == qWTjtVwHij){GIkSoraDwV = true;}
      while(NqGlTNSDyR == tzcLxpMEBr){iUUmPhXkit = true;}
      while(wywqTtzjGc == wywqTtzjGc){fyaeZPCxYH = true;}
      while(qWTjtVwHij == qWTjtVwHij){kmIDDMJQnE = true;}
      if(sVxwHalhoN == true){sVxwHalhoN = false;}
      if(JRtOVrRrPK == true){JRtOVrRrPK = false;}
      if(iIBFipXVqy == true){iIBFipXVqy = false;}
      if(RKUOYaBzjr == true){RKUOYaBzjr = false;}
      if(IRtrTYcFOB == true){IRtrTYcFOB = false;}
      if(DDrXAjUrNQ == true){DDrXAjUrNQ = false;}
      if(DPpqwyxfou == true){DPpqwyxfou = false;}
      if(qRIWCRNIkJ == true){qRIWCRNIkJ = false;}
      if(NSHptNSxOn == true){NSHptNSxOn = false;}
      if(GIkSoraDwV == true){GIkSoraDwV = false;}
      if(bwPTKkuVUh == true){bwPTKkuVUh = false;}
      if(WLHDDkIAwh == true){WLHDDkIAwh = false;}
      if(fARBohfphK == true){fARBohfphK = false;}
      if(pKLOhQCmeI == true){pKLOhQCmeI = false;}
      if(SxbfRtBhOD == true){SxbfRtBhOD = false;}
      if(jaHOpaeOeb == true){jaHOpaeOeb = false;}
      if(FFmKrMzrUm == true){FFmKrMzrUm = false;}
      if(iUUmPhXkit == true){iUUmPhXkit = false;}
      if(fyaeZPCxYH == true){fyaeZPCxYH = false;}
      if(kmIDDMJQnE == true){kmIDDMJQnE = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VJELSWAFQI
{ 
  void GZbYHjTAAK()
  { 
      bool lJdghtVIXF = false;
      bool NLinOcBTmZ = false;
      bool BEauUPBjqh = false;
      bool JsWpEPGwqR = false;
      bool RIsAyzmkdn = false;
      bool qVTMZghhMS = false;
      bool rlQouTorjy = false;
      bool VFcVHrLawU = false;
      bool bQFDqmZKQW = false;
      bool hXiYYfqDEe = false;
      bool KuAeRlsxEh = false;
      bool VHkrqbfNyI = false;
      bool tebNQyVDxb = false;
      bool AHWRKgPZkF = false;
      bool sbFhBalqhx = false;
      bool zdEwjfTmOT = false;
      bool PoAcgNjThJ = false;
      bool LFcsOSZsLp = false;
      bool LeVzcWCahd = false;
      bool NbLRoDabBm = false;
      string WwBsTyXmhb;
      string xILnibmPtF;
      string GkAFPxFHoj;
      string ilKBgpdxxW;
      string TIAnCSmUXD;
      string XoMUAkwudV;
      string sQxyrKxJHs;
      string yoSXlyjLDw;
      string JjilMtZpBu;
      string shTacDKCwq;
      string VcMeEhWniB;
      string AkrhMIgKky;
      string kHIMzPVqti;
      string nhOLZfWEhd;
      string XkGztIpkNT;
      string WzQyFNUAen;
      string MetzNSVgHf;
      string lbcHaAAIRp;
      string ZjdZMnRQFM;
      string LVZsQrpHla;
      if(WwBsTyXmhb == VcMeEhWniB){lJdghtVIXF = true;}
      else if(VcMeEhWniB == WwBsTyXmhb){KuAeRlsxEh = true;}
      if(xILnibmPtF == AkrhMIgKky){NLinOcBTmZ = true;}
      else if(AkrhMIgKky == xILnibmPtF){VHkrqbfNyI = true;}
      if(GkAFPxFHoj == kHIMzPVqti){BEauUPBjqh = true;}
      else if(kHIMzPVqti == GkAFPxFHoj){tebNQyVDxb = true;}
      if(ilKBgpdxxW == nhOLZfWEhd){JsWpEPGwqR = true;}
      else if(nhOLZfWEhd == ilKBgpdxxW){AHWRKgPZkF = true;}
      if(TIAnCSmUXD == XkGztIpkNT){RIsAyzmkdn = true;}
      else if(XkGztIpkNT == TIAnCSmUXD){sbFhBalqhx = true;}
      if(XoMUAkwudV == WzQyFNUAen){qVTMZghhMS = true;}
      else if(WzQyFNUAen == XoMUAkwudV){zdEwjfTmOT = true;}
      if(sQxyrKxJHs == MetzNSVgHf){rlQouTorjy = true;}
      else if(MetzNSVgHf == sQxyrKxJHs){PoAcgNjThJ = true;}
      if(yoSXlyjLDw == lbcHaAAIRp){VFcVHrLawU = true;}
      if(JjilMtZpBu == ZjdZMnRQFM){bQFDqmZKQW = true;}
      if(shTacDKCwq == LVZsQrpHla){hXiYYfqDEe = true;}
      while(lbcHaAAIRp == yoSXlyjLDw){LFcsOSZsLp = true;}
      while(ZjdZMnRQFM == ZjdZMnRQFM){LeVzcWCahd = true;}
      while(LVZsQrpHla == LVZsQrpHla){NbLRoDabBm = true;}
      if(lJdghtVIXF == true){lJdghtVIXF = false;}
      if(NLinOcBTmZ == true){NLinOcBTmZ = false;}
      if(BEauUPBjqh == true){BEauUPBjqh = false;}
      if(JsWpEPGwqR == true){JsWpEPGwqR = false;}
      if(RIsAyzmkdn == true){RIsAyzmkdn = false;}
      if(qVTMZghhMS == true){qVTMZghhMS = false;}
      if(rlQouTorjy == true){rlQouTorjy = false;}
      if(VFcVHrLawU == true){VFcVHrLawU = false;}
      if(bQFDqmZKQW == true){bQFDqmZKQW = false;}
      if(hXiYYfqDEe == true){hXiYYfqDEe = false;}
      if(KuAeRlsxEh == true){KuAeRlsxEh = false;}
      if(VHkrqbfNyI == true){VHkrqbfNyI = false;}
      if(tebNQyVDxb == true){tebNQyVDxb = false;}
      if(AHWRKgPZkF == true){AHWRKgPZkF = false;}
      if(sbFhBalqhx == true){sbFhBalqhx = false;}
      if(zdEwjfTmOT == true){zdEwjfTmOT = false;}
      if(PoAcgNjThJ == true){PoAcgNjThJ = false;}
      if(LFcsOSZsLp == true){LFcsOSZsLp = false;}
      if(LeVzcWCahd == true){LeVzcWCahd = false;}
      if(NbLRoDabBm == true){NbLRoDabBm = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BSNZABCFBF
{ 
  void hntZQhIKzH()
  { 
      bool MfcxXizXSE = false;
      bool eKqDzFbQjO = false;
      bool CfBUKMypKV = false;
      bool HWFKfdyYlM = false;
      bool mKqiOrfFzO = false;
      bool ByqKFgeImL = false;
      bool kJMxwIEfWU = false;
      bool GCdXarnRFR = false;
      bool grpwTJeuEz = false;
      bool zplaadytmG = false;
      bool mzhdIyTTiN = false;
      bool rMCUerSQcl = false;
      bool cIaXUPQDwx = false;
      bool ssezdgJLmU = false;
      bool oKMpGlnQwi = false;
      bool KgRAceYKJr = false;
      bool hQXlUyhDpx = false;
      bool hHcHxSwjQi = false;
      bool bzHNlMbdVz = false;
      bool IzCtmYmZVI = false;
      string jzmEihWjgQ;
      string ECLoleOzlB;
      string upFhNsJLoy;
      string gMazKScSEw;
      string NgTefJHDNq;
      string wZfDsstFpp;
      string XLtijrbMeT;
      string CuuomOVZCE;
      string lGyxCHfWGQ;
      string XXcOXCljum;
      string huYsoAeqPe;
      string WUDkEQMxgb;
      string LEFNweSgQk;
      string bldOWqhFIB;
      string boGatDUqKG;
      string pnDSacoKwJ;
      string gyZeCkpnOM;
      string iGneAWyeTs;
      string KbVJumhKmt;
      string CWIyUdOaYI;
      if(jzmEihWjgQ == huYsoAeqPe){MfcxXizXSE = true;}
      else if(huYsoAeqPe == jzmEihWjgQ){mzhdIyTTiN = true;}
      if(ECLoleOzlB == WUDkEQMxgb){eKqDzFbQjO = true;}
      else if(WUDkEQMxgb == ECLoleOzlB){rMCUerSQcl = true;}
      if(upFhNsJLoy == LEFNweSgQk){CfBUKMypKV = true;}
      else if(LEFNweSgQk == upFhNsJLoy){cIaXUPQDwx = true;}
      if(gMazKScSEw == bldOWqhFIB){HWFKfdyYlM = true;}
      else if(bldOWqhFIB == gMazKScSEw){ssezdgJLmU = true;}
      if(NgTefJHDNq == boGatDUqKG){mKqiOrfFzO = true;}
      else if(boGatDUqKG == NgTefJHDNq){oKMpGlnQwi = true;}
      if(wZfDsstFpp == pnDSacoKwJ){ByqKFgeImL = true;}
      else if(pnDSacoKwJ == wZfDsstFpp){KgRAceYKJr = true;}
      if(XLtijrbMeT == gyZeCkpnOM){kJMxwIEfWU = true;}
      else if(gyZeCkpnOM == XLtijrbMeT){hQXlUyhDpx = true;}
      if(CuuomOVZCE == iGneAWyeTs){GCdXarnRFR = true;}
      if(lGyxCHfWGQ == KbVJumhKmt){grpwTJeuEz = true;}
      if(XXcOXCljum == CWIyUdOaYI){zplaadytmG = true;}
      while(iGneAWyeTs == CuuomOVZCE){hHcHxSwjQi = true;}
      while(KbVJumhKmt == KbVJumhKmt){bzHNlMbdVz = true;}
      while(CWIyUdOaYI == CWIyUdOaYI){IzCtmYmZVI = true;}
      if(MfcxXizXSE == true){MfcxXizXSE = false;}
      if(eKqDzFbQjO == true){eKqDzFbQjO = false;}
      if(CfBUKMypKV == true){CfBUKMypKV = false;}
      if(HWFKfdyYlM == true){HWFKfdyYlM = false;}
      if(mKqiOrfFzO == true){mKqiOrfFzO = false;}
      if(ByqKFgeImL == true){ByqKFgeImL = false;}
      if(kJMxwIEfWU == true){kJMxwIEfWU = false;}
      if(GCdXarnRFR == true){GCdXarnRFR = false;}
      if(grpwTJeuEz == true){grpwTJeuEz = false;}
      if(zplaadytmG == true){zplaadytmG = false;}
      if(mzhdIyTTiN == true){mzhdIyTTiN = false;}
      if(rMCUerSQcl == true){rMCUerSQcl = false;}
      if(cIaXUPQDwx == true){cIaXUPQDwx = false;}
      if(ssezdgJLmU == true){ssezdgJLmU = false;}
      if(oKMpGlnQwi == true){oKMpGlnQwi = false;}
      if(KgRAceYKJr == true){KgRAceYKJr = false;}
      if(hQXlUyhDpx == true){hQXlUyhDpx = false;}
      if(hHcHxSwjQi == true){hHcHxSwjQi = false;}
      if(bzHNlMbdVz == true){bzHNlMbdVz = false;}
      if(IzCtmYmZVI == true){IzCtmYmZVI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MREJFFBNAF
{ 
  void GsXJbMKTCk()
  { 
      bool nJfKjnzwnw = false;
      bool CQbbcUzHOn = false;
      bool caQTlsuFPf = false;
      bool XsiKSAyRxx = false;
      bool RmgRytnBRc = false;
      bool OFCDcNsMYp = false;
      bool rtfHYJgRIc = false;
      bool WhJQWotyUq = false;
      bool EtmmQRqegF = false;
      bool HkrGFXGYso = false;
      bool MVsoFfwkgm = false;
      bool StqgkyGrqG = false;
      bool RCkDVkpDLy = false;
      bool hUFfeiByhj = false;
      bool VGyslMtLUG = false;
      bool ROglWJkoST = false;
      bool psYiExnKBC = false;
      bool xHKGjLDqRI = false;
      bool qdhEfTnTJU = false;
      bool yLeNschPnn = false;
      string sEcOzPQgHo;
      string RAqujUiMxW;
      string sXfldgOqZT;
      string BPLuqHGUXM;
      string YqIIVcNeDC;
      string szzwYUXpjS;
      string BZHmnCowEb;
      string OOokyBPXPB;
      string TinWaBPOVW;
      string jPjFKzjVUT;
      string mRaamdKOyX;
      string FzaiyRljLl;
      string lZZhrzeiZy;
      string RsDuNCIMix;
      string qBQyRjoAGs;
      string NAKigMqrin;
      string zbnzGLweUw;
      string JYAOtkJaPH;
      string KdbgkRiUNB;
      string KPcuyOMQrF;
      if(sEcOzPQgHo == mRaamdKOyX){nJfKjnzwnw = true;}
      else if(mRaamdKOyX == sEcOzPQgHo){MVsoFfwkgm = true;}
      if(RAqujUiMxW == FzaiyRljLl){CQbbcUzHOn = true;}
      else if(FzaiyRljLl == RAqujUiMxW){StqgkyGrqG = true;}
      if(sXfldgOqZT == lZZhrzeiZy){caQTlsuFPf = true;}
      else if(lZZhrzeiZy == sXfldgOqZT){RCkDVkpDLy = true;}
      if(BPLuqHGUXM == RsDuNCIMix){XsiKSAyRxx = true;}
      else if(RsDuNCIMix == BPLuqHGUXM){hUFfeiByhj = true;}
      if(YqIIVcNeDC == qBQyRjoAGs){RmgRytnBRc = true;}
      else if(qBQyRjoAGs == YqIIVcNeDC){VGyslMtLUG = true;}
      if(szzwYUXpjS == NAKigMqrin){OFCDcNsMYp = true;}
      else if(NAKigMqrin == szzwYUXpjS){ROglWJkoST = true;}
      if(BZHmnCowEb == zbnzGLweUw){rtfHYJgRIc = true;}
      else if(zbnzGLweUw == BZHmnCowEb){psYiExnKBC = true;}
      if(OOokyBPXPB == JYAOtkJaPH){WhJQWotyUq = true;}
      if(TinWaBPOVW == KdbgkRiUNB){EtmmQRqegF = true;}
      if(jPjFKzjVUT == KPcuyOMQrF){HkrGFXGYso = true;}
      while(JYAOtkJaPH == OOokyBPXPB){xHKGjLDqRI = true;}
      while(KdbgkRiUNB == KdbgkRiUNB){qdhEfTnTJU = true;}
      while(KPcuyOMQrF == KPcuyOMQrF){yLeNschPnn = true;}
      if(nJfKjnzwnw == true){nJfKjnzwnw = false;}
      if(CQbbcUzHOn == true){CQbbcUzHOn = false;}
      if(caQTlsuFPf == true){caQTlsuFPf = false;}
      if(XsiKSAyRxx == true){XsiKSAyRxx = false;}
      if(RmgRytnBRc == true){RmgRytnBRc = false;}
      if(OFCDcNsMYp == true){OFCDcNsMYp = false;}
      if(rtfHYJgRIc == true){rtfHYJgRIc = false;}
      if(WhJQWotyUq == true){WhJQWotyUq = false;}
      if(EtmmQRqegF == true){EtmmQRqegF = false;}
      if(HkrGFXGYso == true){HkrGFXGYso = false;}
      if(MVsoFfwkgm == true){MVsoFfwkgm = false;}
      if(StqgkyGrqG == true){StqgkyGrqG = false;}
      if(RCkDVkpDLy == true){RCkDVkpDLy = false;}
      if(hUFfeiByhj == true){hUFfeiByhj = false;}
      if(VGyslMtLUG == true){VGyslMtLUG = false;}
      if(ROglWJkoST == true){ROglWJkoST = false;}
      if(psYiExnKBC == true){psYiExnKBC = false;}
      if(xHKGjLDqRI == true){xHKGjLDqRI = false;}
      if(qdhEfTnTJU == true){qdhEfTnTJU = false;}
      if(yLeNschPnn == true){yLeNschPnn = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BVPSZAOYVT
{ 
  void nDbRXBnUfJ()
  { 
      bool bMkPnfMfOk = false;
      bool bjiWYTQzAw = false;
      bool MCXXoxEHrc = false;
      bool RtyFgHLzpI = false;
      bool YtUSVYOVzI = false;
      bool pVpkBZoMap = false;
      bool aEnlSgldpQ = false;
      bool HQJqWShFCP = false;
      bool ihIimwDMDG = false;
      bool RyYAhTFiAn = false;
      bool mjfhBYMyNh = false;
      bool rjKKGrwFzV = false;
      bool dYaizIrIJC = false;
      bool MzENbzCagN = false;
      bool jEyUOhnNso = false;
      bool XAidKOJwir = false;
      bool gDoKVsexhH = false;
      bool oVXklYZFUQ = false;
      bool UePUeMPaCN = false;
      bool pFqogfPnzV = false;
      string lXcBbMpkJq;
      string QTXHUnLNCU;
      string QhwjbSnoWf;
      string WiOBULlPwG;
      string BBrptkgMME;
      string QaoaaEbsZJ;
      string YANcThFVmI;
      string CuQwTPpFoq;
      string kacSOwsmaQ;
      string jrpmgBmOWM;
      string eLZDCVJsHK;
      string OVDhXxWuNW;
      string nRuVXJEdOS;
      string caibLZBYQs;
      string mAfPCOoHOj;
      string dqAzYNPDfr;
      string QfeohmlbWy;
      string uyeUjnkfTI;
      string TxOenkZjDy;
      string JysXcGVqhO;
      if(lXcBbMpkJq == eLZDCVJsHK){bMkPnfMfOk = true;}
      else if(eLZDCVJsHK == lXcBbMpkJq){mjfhBYMyNh = true;}
      if(QTXHUnLNCU == OVDhXxWuNW){bjiWYTQzAw = true;}
      else if(OVDhXxWuNW == QTXHUnLNCU){rjKKGrwFzV = true;}
      if(QhwjbSnoWf == nRuVXJEdOS){MCXXoxEHrc = true;}
      else if(nRuVXJEdOS == QhwjbSnoWf){dYaizIrIJC = true;}
      if(WiOBULlPwG == caibLZBYQs){RtyFgHLzpI = true;}
      else if(caibLZBYQs == WiOBULlPwG){MzENbzCagN = true;}
      if(BBrptkgMME == mAfPCOoHOj){YtUSVYOVzI = true;}
      else if(mAfPCOoHOj == BBrptkgMME){jEyUOhnNso = true;}
      if(QaoaaEbsZJ == dqAzYNPDfr){pVpkBZoMap = true;}
      else if(dqAzYNPDfr == QaoaaEbsZJ){XAidKOJwir = true;}
      if(YANcThFVmI == QfeohmlbWy){aEnlSgldpQ = true;}
      else if(QfeohmlbWy == YANcThFVmI){gDoKVsexhH = true;}
      if(CuQwTPpFoq == uyeUjnkfTI){HQJqWShFCP = true;}
      if(kacSOwsmaQ == TxOenkZjDy){ihIimwDMDG = true;}
      if(jrpmgBmOWM == JysXcGVqhO){RyYAhTFiAn = true;}
      while(uyeUjnkfTI == CuQwTPpFoq){oVXklYZFUQ = true;}
      while(TxOenkZjDy == TxOenkZjDy){UePUeMPaCN = true;}
      while(JysXcGVqhO == JysXcGVqhO){pFqogfPnzV = true;}
      if(bMkPnfMfOk == true){bMkPnfMfOk = false;}
      if(bjiWYTQzAw == true){bjiWYTQzAw = false;}
      if(MCXXoxEHrc == true){MCXXoxEHrc = false;}
      if(RtyFgHLzpI == true){RtyFgHLzpI = false;}
      if(YtUSVYOVzI == true){YtUSVYOVzI = false;}
      if(pVpkBZoMap == true){pVpkBZoMap = false;}
      if(aEnlSgldpQ == true){aEnlSgldpQ = false;}
      if(HQJqWShFCP == true){HQJqWShFCP = false;}
      if(ihIimwDMDG == true){ihIimwDMDG = false;}
      if(RyYAhTFiAn == true){RyYAhTFiAn = false;}
      if(mjfhBYMyNh == true){mjfhBYMyNh = false;}
      if(rjKKGrwFzV == true){rjKKGrwFzV = false;}
      if(dYaizIrIJC == true){dYaizIrIJC = false;}
      if(MzENbzCagN == true){MzENbzCagN = false;}
      if(jEyUOhnNso == true){jEyUOhnNso = false;}
      if(XAidKOJwir == true){XAidKOJwir = false;}
      if(gDoKVsexhH == true){gDoKVsexhH = false;}
      if(oVXklYZFUQ == true){oVXklYZFUQ = false;}
      if(UePUeMPaCN == true){UePUeMPaCN = false;}
      if(pFqogfPnzV == true){pFqogfPnzV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OOXNMQRLXU
{ 
  void gVVOMGpRWX()
  { 
      bool QOVMfKqbUi = false;
      bool zecWOYYzIj = false;
      bool PPmjCArdTo = false;
      bool WXlFNMBglR = false;
      bool UVOHLnXGJz = false;
      bool POanzAbkKe = false;
      bool SMHfMbrWIB = false;
      bool XrNqPsBfXN = false;
      bool bkTUYIxyGN = false;
      bool kPWGzHOhtj = false;
      bool uAxHaXBiYF = false;
      bool zwNxDgslGD = false;
      bool etPuQIUGrH = false;
      bool eSkmRanLsB = false;
      bool HNZBNnCZCR = false;
      bool ObaqFBjCqS = false;
      bool CtqQafLZeb = false;
      bool sgrEwQUOgb = false;
      bool FuEZsCqthQ = false;
      bool pkUazNVJah = false;
      string VAdcbyxdSp;
      string AfqsyVFdLs;
      string RswrSKAVlD;
      string yprnlDTnhe;
      string GBucuUbEsM;
      string ArueQcDlKQ;
      string xbqOqKmDHu;
      string BQuflPxTwy;
      string iwbZDFtMWI;
      string zhcRheKfkk;
      string gXxxKixPrB;
      string eioMhAzRLl;
      string PagyuTmdyH;
      string ERiLQLzeOT;
      string IeTUcQrQig;
      string esTzUFqyBp;
      string uTQkwNAduW;
      string BUCrdkGLUe;
      string bWtRbLxjqL;
      string TBcYXoNiYP;
      if(VAdcbyxdSp == gXxxKixPrB){QOVMfKqbUi = true;}
      else if(gXxxKixPrB == VAdcbyxdSp){uAxHaXBiYF = true;}
      if(AfqsyVFdLs == eioMhAzRLl){zecWOYYzIj = true;}
      else if(eioMhAzRLl == AfqsyVFdLs){zwNxDgslGD = true;}
      if(RswrSKAVlD == PagyuTmdyH){PPmjCArdTo = true;}
      else if(PagyuTmdyH == RswrSKAVlD){etPuQIUGrH = true;}
      if(yprnlDTnhe == ERiLQLzeOT){WXlFNMBglR = true;}
      else if(ERiLQLzeOT == yprnlDTnhe){eSkmRanLsB = true;}
      if(GBucuUbEsM == IeTUcQrQig){UVOHLnXGJz = true;}
      else if(IeTUcQrQig == GBucuUbEsM){HNZBNnCZCR = true;}
      if(ArueQcDlKQ == esTzUFqyBp){POanzAbkKe = true;}
      else if(esTzUFqyBp == ArueQcDlKQ){ObaqFBjCqS = true;}
      if(xbqOqKmDHu == uTQkwNAduW){SMHfMbrWIB = true;}
      else if(uTQkwNAduW == xbqOqKmDHu){CtqQafLZeb = true;}
      if(BQuflPxTwy == BUCrdkGLUe){XrNqPsBfXN = true;}
      if(iwbZDFtMWI == bWtRbLxjqL){bkTUYIxyGN = true;}
      if(zhcRheKfkk == TBcYXoNiYP){kPWGzHOhtj = true;}
      while(BUCrdkGLUe == BQuflPxTwy){sgrEwQUOgb = true;}
      while(bWtRbLxjqL == bWtRbLxjqL){FuEZsCqthQ = true;}
      while(TBcYXoNiYP == TBcYXoNiYP){pkUazNVJah = true;}
      if(QOVMfKqbUi == true){QOVMfKqbUi = false;}
      if(zecWOYYzIj == true){zecWOYYzIj = false;}
      if(PPmjCArdTo == true){PPmjCArdTo = false;}
      if(WXlFNMBglR == true){WXlFNMBglR = false;}
      if(UVOHLnXGJz == true){UVOHLnXGJz = false;}
      if(POanzAbkKe == true){POanzAbkKe = false;}
      if(SMHfMbrWIB == true){SMHfMbrWIB = false;}
      if(XrNqPsBfXN == true){XrNqPsBfXN = false;}
      if(bkTUYIxyGN == true){bkTUYIxyGN = false;}
      if(kPWGzHOhtj == true){kPWGzHOhtj = false;}
      if(uAxHaXBiYF == true){uAxHaXBiYF = false;}
      if(zwNxDgslGD == true){zwNxDgslGD = false;}
      if(etPuQIUGrH == true){etPuQIUGrH = false;}
      if(eSkmRanLsB == true){eSkmRanLsB = false;}
      if(HNZBNnCZCR == true){HNZBNnCZCR = false;}
      if(ObaqFBjCqS == true){ObaqFBjCqS = false;}
      if(CtqQafLZeb == true){CtqQafLZeb = false;}
      if(sgrEwQUOgb == true){sgrEwQUOgb = false;}
      if(FuEZsCqthQ == true){FuEZsCqthQ = false;}
      if(pkUazNVJah == true){pkUazNVJah = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LKBCZPQVGS
{ 
  void TfHtoRZzft()
  { 
      bool QDTmfCkeAU = false;
      bool XGkYfcqCpr = false;
      bool FxRGiCjDMz = false;
      bool iTfGCmzIdY = false;
      bool RggxpeTYlP = false;
      bool EnqMySlNDc = false;
      bool SgSBkEadWr = false;
      bool dhYRZYPTPD = false;
      bool iKRwtubSIi = false;
      bool BlEAFkNfMl = false;
      bool EoCslVOYmY = false;
      bool CSxAtHOVGo = false;
      bool bIVmqrdXLn = false;
      bool KjGdzsIXKY = false;
      bool fDeifinEGx = false;
      bool VqZWJwSOCs = false;
      bool qqMyNSLTBB = false;
      bool zqLgJYcmGH = false;
      bool DZRNSSKjTC = false;
      bool ZsVQgopgVN = false;
      string tjeolJKSwW;
      string BIkupibZaI;
      string FBYwReXiMG;
      string WMdrdSSVPh;
      string lZWWLEVthT;
      string LABAksLhWH;
      string laFpzndrRX;
      string QGKrWedBkN;
      string IdBstWLcGk;
      string GkyHuKTgXX;
      string ZdcJDEoIdG;
      string OJxIzIZMub;
      string cAlgQeIDwL;
      string KxdNndAqpo;
      string gCzAVxnoHm;
      string jHkutYrwsX;
      string wlLRuRfjUU;
      string HhWFtZBkKT;
      string qSuJaDLhSO;
      string lEySsuphFE;
      if(tjeolJKSwW == ZdcJDEoIdG){QDTmfCkeAU = true;}
      else if(ZdcJDEoIdG == tjeolJKSwW){EoCslVOYmY = true;}
      if(BIkupibZaI == OJxIzIZMub){XGkYfcqCpr = true;}
      else if(OJxIzIZMub == BIkupibZaI){CSxAtHOVGo = true;}
      if(FBYwReXiMG == cAlgQeIDwL){FxRGiCjDMz = true;}
      else if(cAlgQeIDwL == FBYwReXiMG){bIVmqrdXLn = true;}
      if(WMdrdSSVPh == KxdNndAqpo){iTfGCmzIdY = true;}
      else if(KxdNndAqpo == WMdrdSSVPh){KjGdzsIXKY = true;}
      if(lZWWLEVthT == gCzAVxnoHm){RggxpeTYlP = true;}
      else if(gCzAVxnoHm == lZWWLEVthT){fDeifinEGx = true;}
      if(LABAksLhWH == jHkutYrwsX){EnqMySlNDc = true;}
      else if(jHkutYrwsX == LABAksLhWH){VqZWJwSOCs = true;}
      if(laFpzndrRX == wlLRuRfjUU){SgSBkEadWr = true;}
      else if(wlLRuRfjUU == laFpzndrRX){qqMyNSLTBB = true;}
      if(QGKrWedBkN == HhWFtZBkKT){dhYRZYPTPD = true;}
      if(IdBstWLcGk == qSuJaDLhSO){iKRwtubSIi = true;}
      if(GkyHuKTgXX == lEySsuphFE){BlEAFkNfMl = true;}
      while(HhWFtZBkKT == QGKrWedBkN){zqLgJYcmGH = true;}
      while(qSuJaDLhSO == qSuJaDLhSO){DZRNSSKjTC = true;}
      while(lEySsuphFE == lEySsuphFE){ZsVQgopgVN = true;}
      if(QDTmfCkeAU == true){QDTmfCkeAU = false;}
      if(XGkYfcqCpr == true){XGkYfcqCpr = false;}
      if(FxRGiCjDMz == true){FxRGiCjDMz = false;}
      if(iTfGCmzIdY == true){iTfGCmzIdY = false;}
      if(RggxpeTYlP == true){RggxpeTYlP = false;}
      if(EnqMySlNDc == true){EnqMySlNDc = false;}
      if(SgSBkEadWr == true){SgSBkEadWr = false;}
      if(dhYRZYPTPD == true){dhYRZYPTPD = false;}
      if(iKRwtubSIi == true){iKRwtubSIi = false;}
      if(BlEAFkNfMl == true){BlEAFkNfMl = false;}
      if(EoCslVOYmY == true){EoCslVOYmY = false;}
      if(CSxAtHOVGo == true){CSxAtHOVGo = false;}
      if(bIVmqrdXLn == true){bIVmqrdXLn = false;}
      if(KjGdzsIXKY == true){KjGdzsIXKY = false;}
      if(fDeifinEGx == true){fDeifinEGx = false;}
      if(VqZWJwSOCs == true){VqZWJwSOCs = false;}
      if(qqMyNSLTBB == true){qqMyNSLTBB = false;}
      if(zqLgJYcmGH == true){zqLgJYcmGH = false;}
      if(DZRNSSKjTC == true){DZRNSSKjTC = false;}
      if(ZsVQgopgVN == true){ZsVQgopgVN = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MIEVEXFWHK
{ 
  void uOJsTSXdot()
  { 
      bool eahDYbZhYD = false;
      bool SLTrTxMDTo = false;
      bool VJIGbISpTt = false;
      bool rqNbClDAPC = false;
      bool UDdLayYcGO = false;
      bool YUQqgVSaaP = false;
      bool JjNdKbyrph = false;
      bool sLmQTRexfd = false;
      bool jpjFVXZVtf = false;
      bool SbZzskpAAy = false;
      bool AflYUNgAWO = false;
      bool pUuxfTAIHm = false;
      bool BbyJOQknXi = false;
      bool bewgdDDLjI = false;
      bool pZzepygcSY = false;
      bool khrMxXhcJD = false;
      bool bsbnmLYOTc = false;
      bool QoXzxbcSTQ = false;
      bool jIUyysmijF = false;
      bool VZBggwxLqe = false;
      string utkKhtHiNH;
      string UtOPQUnPeP;
      string lBdqfyVfMo;
      string MQJNKdBzbt;
      string YkNhFnRqlM;
      string qzsiYtQsei;
      string XbWYuEjAEm;
      string TnmkqBnukq;
      string gdszxNXaoQ;
      string NHUsVxOnmB;
      string DQlzsCLHrX;
      string GUImCthWah;
      string gJLnZMTtPt;
      string lzIjmPqtGu;
      string BPhRJrzgpa;
      string PqNEUtFyZf;
      string iQqykHMXzm;
      string CmQffQatdg;
      string MlEneukDPs;
      string KsUZLkXjfC;
      if(utkKhtHiNH == DQlzsCLHrX){eahDYbZhYD = true;}
      else if(DQlzsCLHrX == utkKhtHiNH){AflYUNgAWO = true;}
      if(UtOPQUnPeP == GUImCthWah){SLTrTxMDTo = true;}
      else if(GUImCthWah == UtOPQUnPeP){pUuxfTAIHm = true;}
      if(lBdqfyVfMo == gJLnZMTtPt){VJIGbISpTt = true;}
      else if(gJLnZMTtPt == lBdqfyVfMo){BbyJOQknXi = true;}
      if(MQJNKdBzbt == lzIjmPqtGu){rqNbClDAPC = true;}
      else if(lzIjmPqtGu == MQJNKdBzbt){bewgdDDLjI = true;}
      if(YkNhFnRqlM == BPhRJrzgpa){UDdLayYcGO = true;}
      else if(BPhRJrzgpa == YkNhFnRqlM){pZzepygcSY = true;}
      if(qzsiYtQsei == PqNEUtFyZf){YUQqgVSaaP = true;}
      else if(PqNEUtFyZf == qzsiYtQsei){khrMxXhcJD = true;}
      if(XbWYuEjAEm == iQqykHMXzm){JjNdKbyrph = true;}
      else if(iQqykHMXzm == XbWYuEjAEm){bsbnmLYOTc = true;}
      if(TnmkqBnukq == CmQffQatdg){sLmQTRexfd = true;}
      if(gdszxNXaoQ == MlEneukDPs){jpjFVXZVtf = true;}
      if(NHUsVxOnmB == KsUZLkXjfC){SbZzskpAAy = true;}
      while(CmQffQatdg == TnmkqBnukq){QoXzxbcSTQ = true;}
      while(MlEneukDPs == MlEneukDPs){jIUyysmijF = true;}
      while(KsUZLkXjfC == KsUZLkXjfC){VZBggwxLqe = true;}
      if(eahDYbZhYD == true){eahDYbZhYD = false;}
      if(SLTrTxMDTo == true){SLTrTxMDTo = false;}
      if(VJIGbISpTt == true){VJIGbISpTt = false;}
      if(rqNbClDAPC == true){rqNbClDAPC = false;}
      if(UDdLayYcGO == true){UDdLayYcGO = false;}
      if(YUQqgVSaaP == true){YUQqgVSaaP = false;}
      if(JjNdKbyrph == true){JjNdKbyrph = false;}
      if(sLmQTRexfd == true){sLmQTRexfd = false;}
      if(jpjFVXZVtf == true){jpjFVXZVtf = false;}
      if(SbZzskpAAy == true){SbZzskpAAy = false;}
      if(AflYUNgAWO == true){AflYUNgAWO = false;}
      if(pUuxfTAIHm == true){pUuxfTAIHm = false;}
      if(BbyJOQknXi == true){BbyJOQknXi = false;}
      if(bewgdDDLjI == true){bewgdDDLjI = false;}
      if(pZzepygcSY == true){pZzepygcSY = false;}
      if(khrMxXhcJD == true){khrMxXhcJD = false;}
      if(bsbnmLYOTc == true){bsbnmLYOTc = false;}
      if(QoXzxbcSTQ == true){QoXzxbcSTQ = false;}
      if(jIUyysmijF == true){jIUyysmijF = false;}
      if(VZBggwxLqe == true){VZBggwxLqe = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZRPVQRAVZB
{ 
  void pULLPGTipi()
  { 
      bool EQDmgKjUWB = false;
      bool REBGgYYQqP = false;
      bool HibCQAoZzl = false;
      bool usQLlPjceJ = false;
      bool nTeVueJnQX = false;
      bool KUilyVoAWf = false;
      bool VKVjFXnoTQ = false;
      bool oukgbmDFHk = false;
      bool qBIabaJdNQ = false;
      bool ajGyndytPW = false;
      bool qUTziOiBKK = false;
      bool RirkoUBtId = false;
      bool NKbDpFolsy = false;
      bool oIojtVgJrM = false;
      bool rITDyrqUJT = false;
      bool bDllJgsQJd = false;
      bool ZwIMUJsIqS = false;
      bool mrCulFpXEy = false;
      bool NTcZVlWDaN = false;
      bool cQzOmuzHho = false;
      string RzDNpyoLSi;
      string XscSfgbyow;
      string ailMGrmuCj;
      string SbWGogNnux;
      string NBFEaODSbe;
      string HycJthMqGy;
      string LtIiPuVVcz;
      string SnCrhYdhkr;
      string oTTXcUzNLH;
      string TVgxkTrWne;
      string sgocLSujaU;
      string EeFOIqZjFu;
      string wHJVfxGFGO;
      string gGtGyHeqMJ;
      string YOmqDoPSnt;
      string sIQrilXTxO;
      string MpdMnxjzPB;
      string MiXwVgTSRp;
      string gmQMUXoBXM;
      string jLtyxikaCF;
      if(RzDNpyoLSi == sgocLSujaU){EQDmgKjUWB = true;}
      else if(sgocLSujaU == RzDNpyoLSi){qUTziOiBKK = true;}
      if(XscSfgbyow == EeFOIqZjFu){REBGgYYQqP = true;}
      else if(EeFOIqZjFu == XscSfgbyow){RirkoUBtId = true;}
      if(ailMGrmuCj == wHJVfxGFGO){HibCQAoZzl = true;}
      else if(wHJVfxGFGO == ailMGrmuCj){NKbDpFolsy = true;}
      if(SbWGogNnux == gGtGyHeqMJ){usQLlPjceJ = true;}
      else if(gGtGyHeqMJ == SbWGogNnux){oIojtVgJrM = true;}
      if(NBFEaODSbe == YOmqDoPSnt){nTeVueJnQX = true;}
      else if(YOmqDoPSnt == NBFEaODSbe){rITDyrqUJT = true;}
      if(HycJthMqGy == sIQrilXTxO){KUilyVoAWf = true;}
      else if(sIQrilXTxO == HycJthMqGy){bDllJgsQJd = true;}
      if(LtIiPuVVcz == MpdMnxjzPB){VKVjFXnoTQ = true;}
      else if(MpdMnxjzPB == LtIiPuVVcz){ZwIMUJsIqS = true;}
      if(SnCrhYdhkr == MiXwVgTSRp){oukgbmDFHk = true;}
      if(oTTXcUzNLH == gmQMUXoBXM){qBIabaJdNQ = true;}
      if(TVgxkTrWne == jLtyxikaCF){ajGyndytPW = true;}
      while(MiXwVgTSRp == SnCrhYdhkr){mrCulFpXEy = true;}
      while(gmQMUXoBXM == gmQMUXoBXM){NTcZVlWDaN = true;}
      while(jLtyxikaCF == jLtyxikaCF){cQzOmuzHho = true;}
      if(EQDmgKjUWB == true){EQDmgKjUWB = false;}
      if(REBGgYYQqP == true){REBGgYYQqP = false;}
      if(HibCQAoZzl == true){HibCQAoZzl = false;}
      if(usQLlPjceJ == true){usQLlPjceJ = false;}
      if(nTeVueJnQX == true){nTeVueJnQX = false;}
      if(KUilyVoAWf == true){KUilyVoAWf = false;}
      if(VKVjFXnoTQ == true){VKVjFXnoTQ = false;}
      if(oukgbmDFHk == true){oukgbmDFHk = false;}
      if(qBIabaJdNQ == true){qBIabaJdNQ = false;}
      if(ajGyndytPW == true){ajGyndytPW = false;}
      if(qUTziOiBKK == true){qUTziOiBKK = false;}
      if(RirkoUBtId == true){RirkoUBtId = false;}
      if(NKbDpFolsy == true){NKbDpFolsy = false;}
      if(oIojtVgJrM == true){oIojtVgJrM = false;}
      if(rITDyrqUJT == true){rITDyrqUJT = false;}
      if(bDllJgsQJd == true){bDllJgsQJd = false;}
      if(ZwIMUJsIqS == true){ZwIMUJsIqS = false;}
      if(mrCulFpXEy == true){mrCulFpXEy = false;}
      if(NTcZVlWDaN == true){NTcZVlWDaN = false;}
      if(cQzOmuzHho == true){cQzOmuzHho = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VPKNLGVIED
{ 
  void YdgkMSSIki()
  { 
      bool lhKEygcdLs = false;
      bool JHICEnoggi = false;
      bool IEcdIazjJm = false;
      bool YfIoBSwZPz = false;
      bool QuDCiKqIpc = false;
      bool YgqVjhFguX = false;
      bool gsUYUdyTAl = false;
      bool qPRgSNAZtx = false;
      bool iqsfOrnmFT = false;
      bool MPdrRXaehF = false;
      bool MXHElJOKDV = false;
      bool gflxnoznJb = false;
      bool oLOgZakZOt = false;
      bool lVqnkLjnsF = false;
      bool ddiimfcfDc = false;
      bool XlUGQBJaeK = false;
      bool XouIjLnyMc = false;
      bool FLtEyKbess = false;
      bool yEVlWAAkWj = false;
      bool BIJigniazb = false;
      string hmiUzxZuRh;
      string rtjWemGScd;
      string ayOYceoayh;
      string pDhdXdTEPy;
      string CkboRQYymh;
      string lfMLSiXJqU;
      string IzxjrnMGyB;
      string SBDdaoWZHM;
      string FqmPwJqVKn;
      string FKMtpDnrwd;
      string ERAlOjbhkc;
      string MyCeORLgmi;
      string wpwJuIoJmX;
      string AdcdHnpBBM;
      string gJkgbHHheY;
      string qHENgOVppF;
      string uSuMbhUYWa;
      string aKeaBmEOhn;
      string BHdNfsZPws;
      string RzKuRNNXFs;
      if(hmiUzxZuRh == ERAlOjbhkc){lhKEygcdLs = true;}
      else if(ERAlOjbhkc == hmiUzxZuRh){MXHElJOKDV = true;}
      if(rtjWemGScd == MyCeORLgmi){JHICEnoggi = true;}
      else if(MyCeORLgmi == rtjWemGScd){gflxnoznJb = true;}
      if(ayOYceoayh == wpwJuIoJmX){IEcdIazjJm = true;}
      else if(wpwJuIoJmX == ayOYceoayh){oLOgZakZOt = true;}
      if(pDhdXdTEPy == AdcdHnpBBM){YfIoBSwZPz = true;}
      else if(AdcdHnpBBM == pDhdXdTEPy){lVqnkLjnsF = true;}
      if(CkboRQYymh == gJkgbHHheY){QuDCiKqIpc = true;}
      else if(gJkgbHHheY == CkboRQYymh){ddiimfcfDc = true;}
      if(lfMLSiXJqU == qHENgOVppF){YgqVjhFguX = true;}
      else if(qHENgOVppF == lfMLSiXJqU){XlUGQBJaeK = true;}
      if(IzxjrnMGyB == uSuMbhUYWa){gsUYUdyTAl = true;}
      else if(uSuMbhUYWa == IzxjrnMGyB){XouIjLnyMc = true;}
      if(SBDdaoWZHM == aKeaBmEOhn){qPRgSNAZtx = true;}
      if(FqmPwJqVKn == BHdNfsZPws){iqsfOrnmFT = true;}
      if(FKMtpDnrwd == RzKuRNNXFs){MPdrRXaehF = true;}
      while(aKeaBmEOhn == SBDdaoWZHM){FLtEyKbess = true;}
      while(BHdNfsZPws == BHdNfsZPws){yEVlWAAkWj = true;}
      while(RzKuRNNXFs == RzKuRNNXFs){BIJigniazb = true;}
      if(lhKEygcdLs == true){lhKEygcdLs = false;}
      if(JHICEnoggi == true){JHICEnoggi = false;}
      if(IEcdIazjJm == true){IEcdIazjJm = false;}
      if(YfIoBSwZPz == true){YfIoBSwZPz = false;}
      if(QuDCiKqIpc == true){QuDCiKqIpc = false;}
      if(YgqVjhFguX == true){YgqVjhFguX = false;}
      if(gsUYUdyTAl == true){gsUYUdyTAl = false;}
      if(qPRgSNAZtx == true){qPRgSNAZtx = false;}
      if(iqsfOrnmFT == true){iqsfOrnmFT = false;}
      if(MPdrRXaehF == true){MPdrRXaehF = false;}
      if(MXHElJOKDV == true){MXHElJOKDV = false;}
      if(gflxnoznJb == true){gflxnoznJb = false;}
      if(oLOgZakZOt == true){oLOgZakZOt = false;}
      if(lVqnkLjnsF == true){lVqnkLjnsF = false;}
      if(ddiimfcfDc == true){ddiimfcfDc = false;}
      if(XlUGQBJaeK == true){XlUGQBJaeK = false;}
      if(XouIjLnyMc == true){XouIjLnyMc = false;}
      if(FLtEyKbess == true){FLtEyKbess = false;}
      if(yEVlWAAkWj == true){yEVlWAAkWj = false;}
      if(BIJigniazb == true){BIJigniazb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LDVTJNOTEP
{ 
  void gLLIPrFfxM()
  { 
      bool FLxgBqfHua = false;
      bool ghOsHZyEBf = false;
      bool YmcwnyVYUH = false;
      bool QdahGkOMgL = false;
      bool VxJpMCcuFT = false;
      bool DTZXzShAsD = false;
      bool UGmnJCGflV = false;
      bool KkXpKbdmjk = false;
      bool VZTADxDDin = false;
      bool DOTdpOKVfJ = false;
      bool wwJiJRwjMX = false;
      bool ntQIgtlljM = false;
      bool zDxyRwRszw = false;
      bool RIHaoYQjqk = false;
      bool BTqKzJxKwu = false;
      bool bXkTJryKRN = false;
      bool zCjQGfmpTI = false;
      bool cBuyKeLmzI = false;
      bool yHxDTsWmQw = false;
      bool ZGSckyjaET = false;
      string PAgZSotqSM;
      string xcnYgBnLTb;
      string kScBAQrhQH;
      string QREgGWOgft;
      string SuaHzwlyYs;
      string qZoQqxXeYB;
      string xMaKweYGSJ;
      string lGeYJoqVCE;
      string dsIXQpcmoI;
      string ziHdxFUQLT;
      string XffOGktPHC;
      string FCcUXEECtF;
      string CkRJJCyBbD;
      string TaJhfyNsQf;
      string uMflDxGlGr;
      string BRVFQlNVED;
      string lsnDhdrtoY;
      string pKlOJpFeGL;
      string wMtiCBFERs;
      string krnMsCYbEY;
      if(PAgZSotqSM == XffOGktPHC){FLxgBqfHua = true;}
      else if(XffOGktPHC == PAgZSotqSM){wwJiJRwjMX = true;}
      if(xcnYgBnLTb == FCcUXEECtF){ghOsHZyEBf = true;}
      else if(FCcUXEECtF == xcnYgBnLTb){ntQIgtlljM = true;}
      if(kScBAQrhQH == CkRJJCyBbD){YmcwnyVYUH = true;}
      else if(CkRJJCyBbD == kScBAQrhQH){zDxyRwRszw = true;}
      if(QREgGWOgft == TaJhfyNsQf){QdahGkOMgL = true;}
      else if(TaJhfyNsQf == QREgGWOgft){RIHaoYQjqk = true;}
      if(SuaHzwlyYs == uMflDxGlGr){VxJpMCcuFT = true;}
      else if(uMflDxGlGr == SuaHzwlyYs){BTqKzJxKwu = true;}
      if(qZoQqxXeYB == BRVFQlNVED){DTZXzShAsD = true;}
      else if(BRVFQlNVED == qZoQqxXeYB){bXkTJryKRN = true;}
      if(xMaKweYGSJ == lsnDhdrtoY){UGmnJCGflV = true;}
      else if(lsnDhdrtoY == xMaKweYGSJ){zCjQGfmpTI = true;}
      if(lGeYJoqVCE == pKlOJpFeGL){KkXpKbdmjk = true;}
      if(dsIXQpcmoI == wMtiCBFERs){VZTADxDDin = true;}
      if(ziHdxFUQLT == krnMsCYbEY){DOTdpOKVfJ = true;}
      while(pKlOJpFeGL == lGeYJoqVCE){cBuyKeLmzI = true;}
      while(wMtiCBFERs == wMtiCBFERs){yHxDTsWmQw = true;}
      while(krnMsCYbEY == krnMsCYbEY){ZGSckyjaET = true;}
      if(FLxgBqfHua == true){FLxgBqfHua = false;}
      if(ghOsHZyEBf == true){ghOsHZyEBf = false;}
      if(YmcwnyVYUH == true){YmcwnyVYUH = false;}
      if(QdahGkOMgL == true){QdahGkOMgL = false;}
      if(VxJpMCcuFT == true){VxJpMCcuFT = false;}
      if(DTZXzShAsD == true){DTZXzShAsD = false;}
      if(UGmnJCGflV == true){UGmnJCGflV = false;}
      if(KkXpKbdmjk == true){KkXpKbdmjk = false;}
      if(VZTADxDDin == true){VZTADxDDin = false;}
      if(DOTdpOKVfJ == true){DOTdpOKVfJ = false;}
      if(wwJiJRwjMX == true){wwJiJRwjMX = false;}
      if(ntQIgtlljM == true){ntQIgtlljM = false;}
      if(zDxyRwRszw == true){zDxyRwRszw = false;}
      if(RIHaoYQjqk == true){RIHaoYQjqk = false;}
      if(BTqKzJxKwu == true){BTqKzJxKwu = false;}
      if(bXkTJryKRN == true){bXkTJryKRN = false;}
      if(zCjQGfmpTI == true){zCjQGfmpTI = false;}
      if(cBuyKeLmzI == true){cBuyKeLmzI = false;}
      if(yHxDTsWmQw == true){yHxDTsWmQw = false;}
      if(ZGSckyjaET == true){ZGSckyjaET = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VGDOXDGITP
{ 
  void HesoTcWisQ()
  { 
      bool zQqlYTlBnO = false;
      bool PfHGrKfcPl = false;
      bool OowASGnUGN = false;
      bool ZmLzQsUjdn = false;
      bool bRiFDyFBut = false;
      bool UGifYKTpTS = false;
      bool fQuZobUHpz = false;
      bool puzaJwGpwT = false;
      bool AlZLsTEeZG = false;
      bool YePKILWIQa = false;
      bool cxKuEYGHVc = false;
      bool DcszRtqifU = false;
      bool WohjaKfnSy = false;
      bool ZxZUqDadHU = false;
      bool MxJxCukPrN = false;
      bool UncuDmPaUs = false;
      bool rSnXCGDXIO = false;
      bool TMaLIZfjhf = false;
      bool YPwFdlUdRD = false;
      bool nzJObtJltw = false;
      string hMHBSxQBGU;
      string xSXRBgFiXW;
      string lpjFaiJHQt;
      string thnuZaudGI;
      string DfLrNrkwaf;
      string WjpRQqcCaj;
      string nJkLZPecra;
      string kabPTFeqqi;
      string xMfkXJqVeh;
      string xsDVTtuzRx;
      string MIEAtuHayK;
      string MkKqlaczmP;
      string YJxJcrXFuZ;
      string JeYsWoIfpb;
      string CPWTVMhLjr;
      string QuMBYCKuWC;
      string WBfYBgMRUx;
      string SwJKNJldmP;
      string JMVCIOSWUT;
      string eMInEAiySn;
      if(hMHBSxQBGU == MIEAtuHayK){zQqlYTlBnO = true;}
      else if(MIEAtuHayK == hMHBSxQBGU){cxKuEYGHVc = true;}
      if(xSXRBgFiXW == MkKqlaczmP){PfHGrKfcPl = true;}
      else if(MkKqlaczmP == xSXRBgFiXW){DcszRtqifU = true;}
      if(lpjFaiJHQt == YJxJcrXFuZ){OowASGnUGN = true;}
      else if(YJxJcrXFuZ == lpjFaiJHQt){WohjaKfnSy = true;}
      if(thnuZaudGI == JeYsWoIfpb){ZmLzQsUjdn = true;}
      else if(JeYsWoIfpb == thnuZaudGI){ZxZUqDadHU = true;}
      if(DfLrNrkwaf == CPWTVMhLjr){bRiFDyFBut = true;}
      else if(CPWTVMhLjr == DfLrNrkwaf){MxJxCukPrN = true;}
      if(WjpRQqcCaj == QuMBYCKuWC){UGifYKTpTS = true;}
      else if(QuMBYCKuWC == WjpRQqcCaj){UncuDmPaUs = true;}
      if(nJkLZPecra == WBfYBgMRUx){fQuZobUHpz = true;}
      else if(WBfYBgMRUx == nJkLZPecra){rSnXCGDXIO = true;}
      if(kabPTFeqqi == SwJKNJldmP){puzaJwGpwT = true;}
      if(xMfkXJqVeh == JMVCIOSWUT){AlZLsTEeZG = true;}
      if(xsDVTtuzRx == eMInEAiySn){YePKILWIQa = true;}
      while(SwJKNJldmP == kabPTFeqqi){TMaLIZfjhf = true;}
      while(JMVCIOSWUT == JMVCIOSWUT){YPwFdlUdRD = true;}
      while(eMInEAiySn == eMInEAiySn){nzJObtJltw = true;}
      if(zQqlYTlBnO == true){zQqlYTlBnO = false;}
      if(PfHGrKfcPl == true){PfHGrKfcPl = false;}
      if(OowASGnUGN == true){OowASGnUGN = false;}
      if(ZmLzQsUjdn == true){ZmLzQsUjdn = false;}
      if(bRiFDyFBut == true){bRiFDyFBut = false;}
      if(UGifYKTpTS == true){UGifYKTpTS = false;}
      if(fQuZobUHpz == true){fQuZobUHpz = false;}
      if(puzaJwGpwT == true){puzaJwGpwT = false;}
      if(AlZLsTEeZG == true){AlZLsTEeZG = false;}
      if(YePKILWIQa == true){YePKILWIQa = false;}
      if(cxKuEYGHVc == true){cxKuEYGHVc = false;}
      if(DcszRtqifU == true){DcszRtqifU = false;}
      if(WohjaKfnSy == true){WohjaKfnSy = false;}
      if(ZxZUqDadHU == true){ZxZUqDadHU = false;}
      if(MxJxCukPrN == true){MxJxCukPrN = false;}
      if(UncuDmPaUs == true){UncuDmPaUs = false;}
      if(rSnXCGDXIO == true){rSnXCGDXIO = false;}
      if(TMaLIZfjhf == true){TMaLIZfjhf = false;}
      if(YPwFdlUdRD == true){YPwFdlUdRD = false;}
      if(nzJObtJltw == true){nzJObtJltw = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class USMPPPMZBX
{ 
  void IWDrkaRByR()
  { 
      bool rIHqZbYiJz = false;
      bool iIAfaMeguy = false;
      bool lBKnsFsLzh = false;
      bool cipEAtXiin = false;
      bool EgucldnGNK = false;
      bool pXzUlqxTyU = false;
      bool hMjaWASwff = false;
      bool owxHtEFgBP = false;
      bool QmynrFiUSI = false;
      bool wEkOGqwTGl = false;
      bool KrZgUtZrag = false;
      bool kQRhDtOwiR = false;
      bool QxcWjkKiBC = false;
      bool HlZPEXNQSx = false;
      bool eqskXflCKL = false;
      bool cAkoUMOCwj = false;
      bool ippYYzpBdf = false;
      bool gxcllZDqzE = false;
      bool yIYoZHAktq = false;
      bool JKTiNoAUMY = false;
      string sGjmDodXyH;
      string lXatbAxmBp;
      string rDTUKkfJMF;
      string utuuqLIncq;
      string TAlGBbFhgT;
      string zOeFUpmPbS;
      string JTKsMrbfDP;
      string qIJKWqQNSi;
      string fxbsFfPpZA;
      string YrLKgzpstJ;
      string zYeQbpETid;
      string ZZwsmMqRmf;
      string clGCqyxydW;
      string BWSJiGRLRP;
      string buHwZzwRXU;
      string HYGNgNScYc;
      string rFpLbCGmJf;
      string tfzWEgeKbr;
      string CeaJPcQEcV;
      string molnXVEuTX;
      if(sGjmDodXyH == zYeQbpETid){rIHqZbYiJz = true;}
      else if(zYeQbpETid == sGjmDodXyH){KrZgUtZrag = true;}
      if(lXatbAxmBp == ZZwsmMqRmf){iIAfaMeguy = true;}
      else if(ZZwsmMqRmf == lXatbAxmBp){kQRhDtOwiR = true;}
      if(rDTUKkfJMF == clGCqyxydW){lBKnsFsLzh = true;}
      else if(clGCqyxydW == rDTUKkfJMF){QxcWjkKiBC = true;}
      if(utuuqLIncq == BWSJiGRLRP){cipEAtXiin = true;}
      else if(BWSJiGRLRP == utuuqLIncq){HlZPEXNQSx = true;}
      if(TAlGBbFhgT == buHwZzwRXU){EgucldnGNK = true;}
      else if(buHwZzwRXU == TAlGBbFhgT){eqskXflCKL = true;}
      if(zOeFUpmPbS == HYGNgNScYc){pXzUlqxTyU = true;}
      else if(HYGNgNScYc == zOeFUpmPbS){cAkoUMOCwj = true;}
      if(JTKsMrbfDP == rFpLbCGmJf){hMjaWASwff = true;}
      else if(rFpLbCGmJf == JTKsMrbfDP){ippYYzpBdf = true;}
      if(qIJKWqQNSi == tfzWEgeKbr){owxHtEFgBP = true;}
      if(fxbsFfPpZA == CeaJPcQEcV){QmynrFiUSI = true;}
      if(YrLKgzpstJ == molnXVEuTX){wEkOGqwTGl = true;}
      while(tfzWEgeKbr == qIJKWqQNSi){gxcllZDqzE = true;}
      while(CeaJPcQEcV == CeaJPcQEcV){yIYoZHAktq = true;}
      while(molnXVEuTX == molnXVEuTX){JKTiNoAUMY = true;}
      if(rIHqZbYiJz == true){rIHqZbYiJz = false;}
      if(iIAfaMeguy == true){iIAfaMeguy = false;}
      if(lBKnsFsLzh == true){lBKnsFsLzh = false;}
      if(cipEAtXiin == true){cipEAtXiin = false;}
      if(EgucldnGNK == true){EgucldnGNK = false;}
      if(pXzUlqxTyU == true){pXzUlqxTyU = false;}
      if(hMjaWASwff == true){hMjaWASwff = false;}
      if(owxHtEFgBP == true){owxHtEFgBP = false;}
      if(QmynrFiUSI == true){QmynrFiUSI = false;}
      if(wEkOGqwTGl == true){wEkOGqwTGl = false;}
      if(KrZgUtZrag == true){KrZgUtZrag = false;}
      if(kQRhDtOwiR == true){kQRhDtOwiR = false;}
      if(QxcWjkKiBC == true){QxcWjkKiBC = false;}
      if(HlZPEXNQSx == true){HlZPEXNQSx = false;}
      if(eqskXflCKL == true){eqskXflCKL = false;}
      if(cAkoUMOCwj == true){cAkoUMOCwj = false;}
      if(ippYYzpBdf == true){ippYYzpBdf = false;}
      if(gxcllZDqzE == true){gxcllZDqzE = false;}
      if(yIYoZHAktq == true){yIYoZHAktq = false;}
      if(JKTiNoAUMY == true){JKTiNoAUMY = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GEZWZBZNCW
{ 
  void usUScaTJiE()
  { 
      bool LzNcgqSdaL = false;
      bool gWPHRotpFW = false;
      bool apbptOUQzs = false;
      bool TRmkxppFyQ = false;
      bool SybVMGfeIP = false;
      bool SkdsWVXXhS = false;
      bool WCdhqPjdKa = false;
      bool ynqoEbNGcm = false;
      bool ioHjZintOa = false;
      bool SSXfsQTtdx = false;
      bool WBUgQesoot = false;
      bool HQttjhttHH = false;
      bool bpOiIfQKun = false;
      bool HSbeQgUZSu = false;
      bool oyEUSCXcwL = false;
      bool VZmWVfmkYk = false;
      bool NisBlnnOcD = false;
      bool NTaMRcktQG = false;
      bool HmOjRhfFJj = false;
      bool siAhgGFDhd = false;
      string rixkymHaKh;
      string hcTTXrDgsa;
      string nirPLNGLGK;
      string gBJOWZlmQa;
      string GGldeAGecN;
      string OdLplJqgYk;
      string fuTAAgSEwx;
      string FQPSThwNkK;
      string GbirwDoTcM;
      string iFNfuLlldx;
      string hCheIxNsVE;
      string McwOdDLbDw;
      string qybiOzDjWI;
      string wKjmcEtDjy;
      string pwxXAFIzBq;
      string zKhgGiayQI;
      string scVNICsIkY;
      string JHqEGuBDiI;
      string pEifIkbDgt;
      string dumaENaiPx;
      if(rixkymHaKh == hCheIxNsVE){LzNcgqSdaL = true;}
      else if(hCheIxNsVE == rixkymHaKh){WBUgQesoot = true;}
      if(hcTTXrDgsa == McwOdDLbDw){gWPHRotpFW = true;}
      else if(McwOdDLbDw == hcTTXrDgsa){HQttjhttHH = true;}
      if(nirPLNGLGK == qybiOzDjWI){apbptOUQzs = true;}
      else if(qybiOzDjWI == nirPLNGLGK){bpOiIfQKun = true;}
      if(gBJOWZlmQa == wKjmcEtDjy){TRmkxppFyQ = true;}
      else if(wKjmcEtDjy == gBJOWZlmQa){HSbeQgUZSu = true;}
      if(GGldeAGecN == pwxXAFIzBq){SybVMGfeIP = true;}
      else if(pwxXAFIzBq == GGldeAGecN){oyEUSCXcwL = true;}
      if(OdLplJqgYk == zKhgGiayQI){SkdsWVXXhS = true;}
      else if(zKhgGiayQI == OdLplJqgYk){VZmWVfmkYk = true;}
      if(fuTAAgSEwx == scVNICsIkY){WCdhqPjdKa = true;}
      else if(scVNICsIkY == fuTAAgSEwx){NisBlnnOcD = true;}
      if(FQPSThwNkK == JHqEGuBDiI){ynqoEbNGcm = true;}
      if(GbirwDoTcM == pEifIkbDgt){ioHjZintOa = true;}
      if(iFNfuLlldx == dumaENaiPx){SSXfsQTtdx = true;}
      while(JHqEGuBDiI == FQPSThwNkK){NTaMRcktQG = true;}
      while(pEifIkbDgt == pEifIkbDgt){HmOjRhfFJj = true;}
      while(dumaENaiPx == dumaENaiPx){siAhgGFDhd = true;}
      if(LzNcgqSdaL == true){LzNcgqSdaL = false;}
      if(gWPHRotpFW == true){gWPHRotpFW = false;}
      if(apbptOUQzs == true){apbptOUQzs = false;}
      if(TRmkxppFyQ == true){TRmkxppFyQ = false;}
      if(SybVMGfeIP == true){SybVMGfeIP = false;}
      if(SkdsWVXXhS == true){SkdsWVXXhS = false;}
      if(WCdhqPjdKa == true){WCdhqPjdKa = false;}
      if(ynqoEbNGcm == true){ynqoEbNGcm = false;}
      if(ioHjZintOa == true){ioHjZintOa = false;}
      if(SSXfsQTtdx == true){SSXfsQTtdx = false;}
      if(WBUgQesoot == true){WBUgQesoot = false;}
      if(HQttjhttHH == true){HQttjhttHH = false;}
      if(bpOiIfQKun == true){bpOiIfQKun = false;}
      if(HSbeQgUZSu == true){HSbeQgUZSu = false;}
      if(oyEUSCXcwL == true){oyEUSCXcwL = false;}
      if(VZmWVfmkYk == true){VZmWVfmkYk = false;}
      if(NisBlnnOcD == true){NisBlnnOcD = false;}
      if(NTaMRcktQG == true){NTaMRcktQG = false;}
      if(HmOjRhfFJj == true){HmOjRhfFJj = false;}
      if(siAhgGFDhd == true){siAhgGFDhd = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VIRPKIMZQQ
{ 
  void QUDeJMlTVJ()
  { 
      bool SZLeNOUcHX = false;
      bool KkNZYwVHCF = false;
      bool LtsLaDQClx = false;
      bool IYFnItYUmi = false;
      bool uTeagWcJpZ = false;
      bool LyIWlYfADK = false;
      bool xRqKOlfYQd = false;
      bool WAUTJrOReE = false;
      bool ZHMLlJQhCj = false;
      bool NYRckcbUMu = false;
      bool qErLCKdoMs = false;
      bool KNVyjpFfXF = false;
      bool UxVgPyhrhg = false;
      bool PsFjNVPAIa = false;
      bool fbUIZzDSru = false;
      bool WwwHZshKwE = false;
      bool blGfcQIlkN = false;
      bool ZFWwYscMHQ = false;
      bool tPzmJTwyJU = false;
      bool nEEDysUofI = false;
      string IkSAFtWXed;
      string LJBRqVfrPI;
      string lijihWYYJc;
      string IzsCYIiYGO;
      string xZAodFxmLT;
      string pVSWiZBjSi;
      string VyOBULQzrh;
      string mGlBLMhtEb;
      string YkNommgVTy;
      string GyGXrzKzUS;
      string oytcgijQLV;
      string qGepJkkYNh;
      string etetanANIL;
      string BiZlmaqjHE;
      string DPKxGzSPNH;
      string OlllmuSABs;
      string hVIzUIgsDi;
      string tAQIePjhmb;
      string UkAAxPwmzr;
      string HpiOWgGKby;
      if(IkSAFtWXed == oytcgijQLV){SZLeNOUcHX = true;}
      else if(oytcgijQLV == IkSAFtWXed){qErLCKdoMs = true;}
      if(LJBRqVfrPI == qGepJkkYNh){KkNZYwVHCF = true;}
      else if(qGepJkkYNh == LJBRqVfrPI){KNVyjpFfXF = true;}
      if(lijihWYYJc == etetanANIL){LtsLaDQClx = true;}
      else if(etetanANIL == lijihWYYJc){UxVgPyhrhg = true;}
      if(IzsCYIiYGO == BiZlmaqjHE){IYFnItYUmi = true;}
      else if(BiZlmaqjHE == IzsCYIiYGO){PsFjNVPAIa = true;}
      if(xZAodFxmLT == DPKxGzSPNH){uTeagWcJpZ = true;}
      else if(DPKxGzSPNH == xZAodFxmLT){fbUIZzDSru = true;}
      if(pVSWiZBjSi == OlllmuSABs){LyIWlYfADK = true;}
      else if(OlllmuSABs == pVSWiZBjSi){WwwHZshKwE = true;}
      if(VyOBULQzrh == hVIzUIgsDi){xRqKOlfYQd = true;}
      else if(hVIzUIgsDi == VyOBULQzrh){blGfcQIlkN = true;}
      if(mGlBLMhtEb == tAQIePjhmb){WAUTJrOReE = true;}
      if(YkNommgVTy == UkAAxPwmzr){ZHMLlJQhCj = true;}
      if(GyGXrzKzUS == HpiOWgGKby){NYRckcbUMu = true;}
      while(tAQIePjhmb == mGlBLMhtEb){ZFWwYscMHQ = true;}
      while(UkAAxPwmzr == UkAAxPwmzr){tPzmJTwyJU = true;}
      while(HpiOWgGKby == HpiOWgGKby){nEEDysUofI = true;}
      if(SZLeNOUcHX == true){SZLeNOUcHX = false;}
      if(KkNZYwVHCF == true){KkNZYwVHCF = false;}
      if(LtsLaDQClx == true){LtsLaDQClx = false;}
      if(IYFnItYUmi == true){IYFnItYUmi = false;}
      if(uTeagWcJpZ == true){uTeagWcJpZ = false;}
      if(LyIWlYfADK == true){LyIWlYfADK = false;}
      if(xRqKOlfYQd == true){xRqKOlfYQd = false;}
      if(WAUTJrOReE == true){WAUTJrOReE = false;}
      if(ZHMLlJQhCj == true){ZHMLlJQhCj = false;}
      if(NYRckcbUMu == true){NYRckcbUMu = false;}
      if(qErLCKdoMs == true){qErLCKdoMs = false;}
      if(KNVyjpFfXF == true){KNVyjpFfXF = false;}
      if(UxVgPyhrhg == true){UxVgPyhrhg = false;}
      if(PsFjNVPAIa == true){PsFjNVPAIa = false;}
      if(fbUIZzDSru == true){fbUIZzDSru = false;}
      if(WwwHZshKwE == true){WwwHZshKwE = false;}
      if(blGfcQIlkN == true){blGfcQIlkN = false;}
      if(ZFWwYscMHQ == true){ZFWwYscMHQ = false;}
      if(tPzmJTwyJU == true){tPzmJTwyJU = false;}
      if(nEEDysUofI == true){nEEDysUofI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DCLADWIFWY
{ 
  void ePOBXNAAqD()
  { 
      bool lALMWbNucj = false;
      bool yxCUADlSJr = false;
      bool sExGxGCSfa = false;
      bool lYyeQEyqyR = false;
      bool fPEhfQxMZm = false;
      bool CSfYIhCFPQ = false;
      bool YuPCyYyWEw = false;
      bool FuTqLczPaf = false;
      bool IprrXSrYWh = false;
      bool BsbhrjACjG = false;
      bool RaIdhHyoMf = false;
      bool imdiHiDWde = false;
      bool fZAVuMSlya = false;
      bool cweTNhgOKQ = false;
      bool TDMqzOOrZJ = false;
      bool gcYEZrZlhV = false;
      bool nMpkClHJUY = false;
      bool BnMNLcMJWg = false;
      bool xaLotzXZsV = false;
      bool IufobHManM = false;
      string YRRmqUKrdw;
      string alffUHzoqJ;
      string CRstwySOKc;
      string DRZTyIeQCW;
      string yQtfOhrHKD;
      string DFGVaHYWpr;
      string CuhBsJMMgL;
      string chlTKkXMfN;
      string GKEGZyLDsq;
      string jHZJUDismF;
      string VfZyaUSlKo;
      string jLqclqpaau;
      string xISoaUHbyH;
      string CdZaqajFfE;
      string eYsDHXQxXd;
      string MIRrHNMjXF;
      string NpmDlFlQkL;
      string GktWPXOPhC;
      string EKEUttLhSd;
      string NAhapolpQC;
      if(YRRmqUKrdw == VfZyaUSlKo){lALMWbNucj = true;}
      else if(VfZyaUSlKo == YRRmqUKrdw){RaIdhHyoMf = true;}
      if(alffUHzoqJ == jLqclqpaau){yxCUADlSJr = true;}
      else if(jLqclqpaau == alffUHzoqJ){imdiHiDWde = true;}
      if(CRstwySOKc == xISoaUHbyH){sExGxGCSfa = true;}
      else if(xISoaUHbyH == CRstwySOKc){fZAVuMSlya = true;}
      if(DRZTyIeQCW == CdZaqajFfE){lYyeQEyqyR = true;}
      else if(CdZaqajFfE == DRZTyIeQCW){cweTNhgOKQ = true;}
      if(yQtfOhrHKD == eYsDHXQxXd){fPEhfQxMZm = true;}
      else if(eYsDHXQxXd == yQtfOhrHKD){TDMqzOOrZJ = true;}
      if(DFGVaHYWpr == MIRrHNMjXF){CSfYIhCFPQ = true;}
      else if(MIRrHNMjXF == DFGVaHYWpr){gcYEZrZlhV = true;}
      if(CuhBsJMMgL == NpmDlFlQkL){YuPCyYyWEw = true;}
      else if(NpmDlFlQkL == CuhBsJMMgL){nMpkClHJUY = true;}
      if(chlTKkXMfN == GktWPXOPhC){FuTqLczPaf = true;}
      if(GKEGZyLDsq == EKEUttLhSd){IprrXSrYWh = true;}
      if(jHZJUDismF == NAhapolpQC){BsbhrjACjG = true;}
      while(GktWPXOPhC == chlTKkXMfN){BnMNLcMJWg = true;}
      while(EKEUttLhSd == EKEUttLhSd){xaLotzXZsV = true;}
      while(NAhapolpQC == NAhapolpQC){IufobHManM = true;}
      if(lALMWbNucj == true){lALMWbNucj = false;}
      if(yxCUADlSJr == true){yxCUADlSJr = false;}
      if(sExGxGCSfa == true){sExGxGCSfa = false;}
      if(lYyeQEyqyR == true){lYyeQEyqyR = false;}
      if(fPEhfQxMZm == true){fPEhfQxMZm = false;}
      if(CSfYIhCFPQ == true){CSfYIhCFPQ = false;}
      if(YuPCyYyWEw == true){YuPCyYyWEw = false;}
      if(FuTqLczPaf == true){FuTqLczPaf = false;}
      if(IprrXSrYWh == true){IprrXSrYWh = false;}
      if(BsbhrjACjG == true){BsbhrjACjG = false;}
      if(RaIdhHyoMf == true){RaIdhHyoMf = false;}
      if(imdiHiDWde == true){imdiHiDWde = false;}
      if(fZAVuMSlya == true){fZAVuMSlya = false;}
      if(cweTNhgOKQ == true){cweTNhgOKQ = false;}
      if(TDMqzOOrZJ == true){TDMqzOOrZJ = false;}
      if(gcYEZrZlhV == true){gcYEZrZlhV = false;}
      if(nMpkClHJUY == true){nMpkClHJUY = false;}
      if(BnMNLcMJWg == true){BnMNLcMJWg = false;}
      if(xaLotzXZsV == true){xaLotzXZsV = false;}
      if(IufobHManM == true){IufobHManM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KSWMWTHRYA
{ 
  void FasYJguWuD()
  { 
      bool iUGCXQKOZW = false;
      bool KDlfXzkGsc = false;
      bool BJpgJVbtar = false;
      bool nPiyzlTRmT = false;
      bool lHFVNlDfSV = false;
      bool fayqMYdYrB = false;
      bool oPmgiuTnfk = false;
      bool fSSYptEUTX = false;
      bool SwbEMMQrkZ = false;
      bool HtZkeWTXmR = false;
      bool ROzgpMIYBS = false;
      bool UJFThrniTW = false;
      bool dbCFckrhtT = false;
      bool ZfgBFDxtNA = false;
      bool ErqRFyoGCm = false;
      bool hkDPnpLsDF = false;
      bool TadgsnbDNd = false;
      bool ttUTRljjAe = false;
      bool wleHlocRNP = false;
      bool rDpIgdNJYQ = false;
      string dIOcPSJBJZ;
      string dmlbsihSDn;
      string LlZrwQdzkL;
      string uzUajKxnxY;
      string dQBtrMPpDa;
      string FxpeNqiWfy;
      string nKPxddSKrh;
      string ltUeKtkeAj;
      string BmuJGyJpNr;
      string baIzjTBDPN;
      string tEyVoLbKUS;
      string GDVuxAOFqf;
      string UbxzGKlVtp;
      string shOlpxoZUt;
      string WyVtlXEMBi;
      string FuLjsRHzOd;
      string YFQdZXojFz;
      string LnkwJVGDMi;
      string GZTKkxWesX;
      string poLpVQhqTO;
      if(dIOcPSJBJZ == tEyVoLbKUS){iUGCXQKOZW = true;}
      else if(tEyVoLbKUS == dIOcPSJBJZ){ROzgpMIYBS = true;}
      if(dmlbsihSDn == GDVuxAOFqf){KDlfXzkGsc = true;}
      else if(GDVuxAOFqf == dmlbsihSDn){UJFThrniTW = true;}
      if(LlZrwQdzkL == UbxzGKlVtp){BJpgJVbtar = true;}
      else if(UbxzGKlVtp == LlZrwQdzkL){dbCFckrhtT = true;}
      if(uzUajKxnxY == shOlpxoZUt){nPiyzlTRmT = true;}
      else if(shOlpxoZUt == uzUajKxnxY){ZfgBFDxtNA = true;}
      if(dQBtrMPpDa == WyVtlXEMBi){lHFVNlDfSV = true;}
      else if(WyVtlXEMBi == dQBtrMPpDa){ErqRFyoGCm = true;}
      if(FxpeNqiWfy == FuLjsRHzOd){fayqMYdYrB = true;}
      else if(FuLjsRHzOd == FxpeNqiWfy){hkDPnpLsDF = true;}
      if(nKPxddSKrh == YFQdZXojFz){oPmgiuTnfk = true;}
      else if(YFQdZXojFz == nKPxddSKrh){TadgsnbDNd = true;}
      if(ltUeKtkeAj == LnkwJVGDMi){fSSYptEUTX = true;}
      if(BmuJGyJpNr == GZTKkxWesX){SwbEMMQrkZ = true;}
      if(baIzjTBDPN == poLpVQhqTO){HtZkeWTXmR = true;}
      while(LnkwJVGDMi == ltUeKtkeAj){ttUTRljjAe = true;}
      while(GZTKkxWesX == GZTKkxWesX){wleHlocRNP = true;}
      while(poLpVQhqTO == poLpVQhqTO){rDpIgdNJYQ = true;}
      if(iUGCXQKOZW == true){iUGCXQKOZW = false;}
      if(KDlfXzkGsc == true){KDlfXzkGsc = false;}
      if(BJpgJVbtar == true){BJpgJVbtar = false;}
      if(nPiyzlTRmT == true){nPiyzlTRmT = false;}
      if(lHFVNlDfSV == true){lHFVNlDfSV = false;}
      if(fayqMYdYrB == true){fayqMYdYrB = false;}
      if(oPmgiuTnfk == true){oPmgiuTnfk = false;}
      if(fSSYptEUTX == true){fSSYptEUTX = false;}
      if(SwbEMMQrkZ == true){SwbEMMQrkZ = false;}
      if(HtZkeWTXmR == true){HtZkeWTXmR = false;}
      if(ROzgpMIYBS == true){ROzgpMIYBS = false;}
      if(UJFThrniTW == true){UJFThrniTW = false;}
      if(dbCFckrhtT == true){dbCFckrhtT = false;}
      if(ZfgBFDxtNA == true){ZfgBFDxtNA = false;}
      if(ErqRFyoGCm == true){ErqRFyoGCm = false;}
      if(hkDPnpLsDF == true){hkDPnpLsDF = false;}
      if(TadgsnbDNd == true){TadgsnbDNd = false;}
      if(ttUTRljjAe == true){ttUTRljjAe = false;}
      if(wleHlocRNP == true){wleHlocRNP = false;}
      if(rDpIgdNJYQ == true){rDpIgdNJYQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MBKVYRKSAW
{ 
  void qcCADSpGCF()
  { 
      bool AilCrcBewX = false;
      bool nXQUPBRNQi = false;
      bool jwfITdfzuy = false;
      bool kgOYhxODuH = false;
      bool rUdLBMKNFf = false;
      bool csXGKEDdiN = false;
      bool wBjxlGJrlS = false;
      bool mOSIpOSUGH = false;
      bool SStPVrIHNd = false;
      bool tkTgLqkfYu = false;
      bool DSalrOaPQu = false;
      bool lhWbucHTat = false;
      bool xBdIYBtkwz = false;
      bool dluLEzqHWH = false;
      bool RspRrLNrli = false;
      bool iREdxBoSNq = false;
      bool ToTRRzlLWu = false;
      bool lzwTcEKMPM = false;
      bool mBzNPMgQNC = false;
      bool KYsfmbYYzr = false;
      string KgbxcYXBVi;
      string lDJWYEcniU;
      string IlenaxIWVj;
      string VmWaxKFGDg;
      string wlIOSDiKYb;
      string KTpCOXBmWe;
      string QBelMzfiZO;
      string zVjCYWdQVW;
      string NyytYBwTLY;
      string IRjXQuGVBT;
      string WAkKcnimdr;
      string skRzRSDkNL;
      string CMXAGglMlc;
      string PIPBANMmQj;
      string RJpSfWAkGy;
      string rVIoRKckEw;
      string CWBBcjVQyE;
      string FPTcssVETi;
      string gBFmktAZjp;
      string jbiUgtHnpu;
      if(KgbxcYXBVi == WAkKcnimdr){AilCrcBewX = true;}
      else if(WAkKcnimdr == KgbxcYXBVi){DSalrOaPQu = true;}
      if(lDJWYEcniU == skRzRSDkNL){nXQUPBRNQi = true;}
      else if(skRzRSDkNL == lDJWYEcniU){lhWbucHTat = true;}
      if(IlenaxIWVj == CMXAGglMlc){jwfITdfzuy = true;}
      else if(CMXAGglMlc == IlenaxIWVj){xBdIYBtkwz = true;}
      if(VmWaxKFGDg == PIPBANMmQj){kgOYhxODuH = true;}
      else if(PIPBANMmQj == VmWaxKFGDg){dluLEzqHWH = true;}
      if(wlIOSDiKYb == RJpSfWAkGy){rUdLBMKNFf = true;}
      else if(RJpSfWAkGy == wlIOSDiKYb){RspRrLNrli = true;}
      if(KTpCOXBmWe == rVIoRKckEw){csXGKEDdiN = true;}
      else if(rVIoRKckEw == KTpCOXBmWe){iREdxBoSNq = true;}
      if(QBelMzfiZO == CWBBcjVQyE){wBjxlGJrlS = true;}
      else if(CWBBcjVQyE == QBelMzfiZO){ToTRRzlLWu = true;}
      if(zVjCYWdQVW == FPTcssVETi){mOSIpOSUGH = true;}
      if(NyytYBwTLY == gBFmktAZjp){SStPVrIHNd = true;}
      if(IRjXQuGVBT == jbiUgtHnpu){tkTgLqkfYu = true;}
      while(FPTcssVETi == zVjCYWdQVW){lzwTcEKMPM = true;}
      while(gBFmktAZjp == gBFmktAZjp){mBzNPMgQNC = true;}
      while(jbiUgtHnpu == jbiUgtHnpu){KYsfmbYYzr = true;}
      if(AilCrcBewX == true){AilCrcBewX = false;}
      if(nXQUPBRNQi == true){nXQUPBRNQi = false;}
      if(jwfITdfzuy == true){jwfITdfzuy = false;}
      if(kgOYhxODuH == true){kgOYhxODuH = false;}
      if(rUdLBMKNFf == true){rUdLBMKNFf = false;}
      if(csXGKEDdiN == true){csXGKEDdiN = false;}
      if(wBjxlGJrlS == true){wBjxlGJrlS = false;}
      if(mOSIpOSUGH == true){mOSIpOSUGH = false;}
      if(SStPVrIHNd == true){SStPVrIHNd = false;}
      if(tkTgLqkfYu == true){tkTgLqkfYu = false;}
      if(DSalrOaPQu == true){DSalrOaPQu = false;}
      if(lhWbucHTat == true){lhWbucHTat = false;}
      if(xBdIYBtkwz == true){xBdIYBtkwz = false;}
      if(dluLEzqHWH == true){dluLEzqHWH = false;}
      if(RspRrLNrli == true){RspRrLNrli = false;}
      if(iREdxBoSNq == true){iREdxBoSNq = false;}
      if(ToTRRzlLWu == true){ToTRRzlLWu = false;}
      if(lzwTcEKMPM == true){lzwTcEKMPM = false;}
      if(mBzNPMgQNC == true){mBzNPMgQNC = false;}
      if(KYsfmbYYzr == true){KYsfmbYYzr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HKVHFNBYUS
{ 
  void LuzMIDsEfk()
  { 
      bool uizcHLnzlD = false;
      bool KZQBZndBMf = false;
      bool PuHnMuuwJw = false;
      bool wkfUQnpMcw = false;
      bool dKeetmCpmd = false;
      bool EhPKwzbLQL = false;
      bool ProoGYsukl = false;
      bool xQYPBEQbtY = false;
      bool rFIfqNVVYQ = false;
      bool LewQglVoXU = false;
      bool tHFAfnJzIq = false;
      bool hARqSKiJys = false;
      bool DXWTZSUHiP = false;
      bool iaDYdkzHzD = false;
      bool DVMghLNEyV = false;
      bool CLqeRVkkaN = false;
      bool SCQsIGwrNh = false;
      bool KaJtogKpkO = false;
      bool EPJMMWjYGI = false;
      bool zOgkjBFTYb = false;
      string XNQOqdXLJF;
      string llPHHTAWea;
      string gZhGenNqhY;
      string HTDBsipApT;
      string ZxmyDRVeEZ;
      string NrEdyxfKjD;
      string PfdkyRiNNX;
      string dAfKBjmjDM;
      string RimHKOPYDu;
      string DfQmDsTBAk;
      string iHQgwNMscO;
      string KwGWkgsVZi;
      string CKKSTxirGH;
      string QkiRdKRFLx;
      string CVwnkHYsCW;
      string zxbRiYwyNy;
      string dTlUyYMosZ;
      string OnJcffNWgL;
      string XpojISJQhF;
      string NTrNSijbpL;
      if(XNQOqdXLJF == iHQgwNMscO){uizcHLnzlD = true;}
      else if(iHQgwNMscO == XNQOqdXLJF){tHFAfnJzIq = true;}
      if(llPHHTAWea == KwGWkgsVZi){KZQBZndBMf = true;}
      else if(KwGWkgsVZi == llPHHTAWea){hARqSKiJys = true;}
      if(gZhGenNqhY == CKKSTxirGH){PuHnMuuwJw = true;}
      else if(CKKSTxirGH == gZhGenNqhY){DXWTZSUHiP = true;}
      if(HTDBsipApT == QkiRdKRFLx){wkfUQnpMcw = true;}
      else if(QkiRdKRFLx == HTDBsipApT){iaDYdkzHzD = true;}
      if(ZxmyDRVeEZ == CVwnkHYsCW){dKeetmCpmd = true;}
      else if(CVwnkHYsCW == ZxmyDRVeEZ){DVMghLNEyV = true;}
      if(NrEdyxfKjD == zxbRiYwyNy){EhPKwzbLQL = true;}
      else if(zxbRiYwyNy == NrEdyxfKjD){CLqeRVkkaN = true;}
      if(PfdkyRiNNX == dTlUyYMosZ){ProoGYsukl = true;}
      else if(dTlUyYMosZ == PfdkyRiNNX){SCQsIGwrNh = true;}
      if(dAfKBjmjDM == OnJcffNWgL){xQYPBEQbtY = true;}
      if(RimHKOPYDu == XpojISJQhF){rFIfqNVVYQ = true;}
      if(DfQmDsTBAk == NTrNSijbpL){LewQglVoXU = true;}
      while(OnJcffNWgL == dAfKBjmjDM){KaJtogKpkO = true;}
      while(XpojISJQhF == XpojISJQhF){EPJMMWjYGI = true;}
      while(NTrNSijbpL == NTrNSijbpL){zOgkjBFTYb = true;}
      if(uizcHLnzlD == true){uizcHLnzlD = false;}
      if(KZQBZndBMf == true){KZQBZndBMf = false;}
      if(PuHnMuuwJw == true){PuHnMuuwJw = false;}
      if(wkfUQnpMcw == true){wkfUQnpMcw = false;}
      if(dKeetmCpmd == true){dKeetmCpmd = false;}
      if(EhPKwzbLQL == true){EhPKwzbLQL = false;}
      if(ProoGYsukl == true){ProoGYsukl = false;}
      if(xQYPBEQbtY == true){xQYPBEQbtY = false;}
      if(rFIfqNVVYQ == true){rFIfqNVVYQ = false;}
      if(LewQglVoXU == true){LewQglVoXU = false;}
      if(tHFAfnJzIq == true){tHFAfnJzIq = false;}
      if(hARqSKiJys == true){hARqSKiJys = false;}
      if(DXWTZSUHiP == true){DXWTZSUHiP = false;}
      if(iaDYdkzHzD == true){iaDYdkzHzD = false;}
      if(DVMghLNEyV == true){DVMghLNEyV = false;}
      if(CLqeRVkkaN == true){CLqeRVkkaN = false;}
      if(SCQsIGwrNh == true){SCQsIGwrNh = false;}
      if(KaJtogKpkO == true){KaJtogKpkO = false;}
      if(EPJMMWjYGI == true){EPJMMWjYGI = false;}
      if(zOgkjBFTYb == true){zOgkjBFTYb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CDPTBHCBKQ
{ 
  void GHCQRBBGKP()
  { 
      bool QrgOrehBJh = false;
      bool WwlecDanxL = false;
      bool ZBukRLSQYX = false;
      bool XTsfzSPSpp = false;
      bool yMGftZMJKi = false;
      bool SzpNkOwwxf = false;
      bool LRbkJDDFfK = false;
      bool nIyGMMuPpq = false;
      bool zIFoFXcftj = false;
      bool WSVJdgjUVp = false;
      bool WGcAuKBjAX = false;
      bool TfFFgDLmfk = false;
      bool aXKumUlnbf = false;
      bool cZtAUrfEtu = false;
      bool VseWQqNPba = false;
      bool VxojPzwgNZ = false;
      bool XNHUuskFko = false;
      bool XGrkOgrnew = false;
      bool HWLBJekKjV = false;
      bool uyqUWIlJaX = false;
      string PIiTHAkTdg;
      string sqlUmtlJsl;
      string QOHneuTPJM;
      string MBtZPtNWqU;
      string zdbRLoKYlg;
      string tNgMyBAtnL;
      string gVJQTrwLEr;
      string fORYlmlpbo;
      string kudcSyThbO;
      string sThendTKxT;
      string syTsAfuIaK;
      string OKRzakqgyU;
      string YOipyMflkH;
      string SarIwlbjbl;
      string DLGQziBiEC;
      string llMbueOcun;
      string zJWSrnSJCo;
      string JHELRMHSqP;
      string KwhhVfNQNR;
      string WyonmsnnYK;
      if(PIiTHAkTdg == syTsAfuIaK){QrgOrehBJh = true;}
      else if(syTsAfuIaK == PIiTHAkTdg){WGcAuKBjAX = true;}
      if(sqlUmtlJsl == OKRzakqgyU){WwlecDanxL = true;}
      else if(OKRzakqgyU == sqlUmtlJsl){TfFFgDLmfk = true;}
      if(QOHneuTPJM == YOipyMflkH){ZBukRLSQYX = true;}
      else if(YOipyMflkH == QOHneuTPJM){aXKumUlnbf = true;}
      if(MBtZPtNWqU == SarIwlbjbl){XTsfzSPSpp = true;}
      else if(SarIwlbjbl == MBtZPtNWqU){cZtAUrfEtu = true;}
      if(zdbRLoKYlg == DLGQziBiEC){yMGftZMJKi = true;}
      else if(DLGQziBiEC == zdbRLoKYlg){VseWQqNPba = true;}
      if(tNgMyBAtnL == llMbueOcun){SzpNkOwwxf = true;}
      else if(llMbueOcun == tNgMyBAtnL){VxojPzwgNZ = true;}
      if(gVJQTrwLEr == zJWSrnSJCo){LRbkJDDFfK = true;}
      else if(zJWSrnSJCo == gVJQTrwLEr){XNHUuskFko = true;}
      if(fORYlmlpbo == JHELRMHSqP){nIyGMMuPpq = true;}
      if(kudcSyThbO == KwhhVfNQNR){zIFoFXcftj = true;}
      if(sThendTKxT == WyonmsnnYK){WSVJdgjUVp = true;}
      while(JHELRMHSqP == fORYlmlpbo){XGrkOgrnew = true;}
      while(KwhhVfNQNR == KwhhVfNQNR){HWLBJekKjV = true;}
      while(WyonmsnnYK == WyonmsnnYK){uyqUWIlJaX = true;}
      if(QrgOrehBJh == true){QrgOrehBJh = false;}
      if(WwlecDanxL == true){WwlecDanxL = false;}
      if(ZBukRLSQYX == true){ZBukRLSQYX = false;}
      if(XTsfzSPSpp == true){XTsfzSPSpp = false;}
      if(yMGftZMJKi == true){yMGftZMJKi = false;}
      if(SzpNkOwwxf == true){SzpNkOwwxf = false;}
      if(LRbkJDDFfK == true){LRbkJDDFfK = false;}
      if(nIyGMMuPpq == true){nIyGMMuPpq = false;}
      if(zIFoFXcftj == true){zIFoFXcftj = false;}
      if(WSVJdgjUVp == true){WSVJdgjUVp = false;}
      if(WGcAuKBjAX == true){WGcAuKBjAX = false;}
      if(TfFFgDLmfk == true){TfFFgDLmfk = false;}
      if(aXKumUlnbf == true){aXKumUlnbf = false;}
      if(cZtAUrfEtu == true){cZtAUrfEtu = false;}
      if(VseWQqNPba == true){VseWQqNPba = false;}
      if(VxojPzwgNZ == true){VxojPzwgNZ = false;}
      if(XNHUuskFko == true){XNHUuskFko = false;}
      if(XGrkOgrnew == true){XGrkOgrnew = false;}
      if(HWLBJekKjV == true){HWLBJekKjV = false;}
      if(uyqUWIlJaX == true){uyqUWIlJaX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RLNZFPGWXU
{ 
  void POkOORRUFG()
  { 
      bool jDPRdqChLn = false;
      bool HtGupuUBZU = false;
      bool abILjNIwGy = false;
      bool ouTTHqlYNm = false;
      bool nnkAZhkWGE = false;
      bool LcTUNItDhQ = false;
      bool iGbMzMamnz = false;
      bool IummgmqIkC = false;
      bool bkoBlLbzFy = false;
      bool wiqxDhLXAN = false;
      bool GxhHwQogmZ = false;
      bool QkRfnxRaZm = false;
      bool jbgoCZEpiz = false;
      bool bTIXueCgEV = false;
      bool VuLfhLQKIF = false;
      bool PjxMdeQrFW = false;
      bool LzxnyxctjJ = false;
      bool ujVERoPBHp = false;
      bool IshciqVusP = false;
      bool NJbhKWxWgj = false;
      string aYSWWwhcZu;
      string dghAFYTHFW;
      string bMLdheRFlN;
      string yrwRJhhXOd;
      string aPGKjTgzLW;
      string FZyxNOMnnq;
      string ebqMwkkQpW;
      string purawMFBtO;
      string xRtPHYtyDu;
      string UtiWyzYtNy;
      string iQJngJgWBB;
      string ILsZsNgbEW;
      string qlMtBuLUcw;
      string nLmwegHFhy;
      string rXxClJAJdj;
      string SwopHwEPmk;
      string xLsEMxYxtM;
      string FasGeSoJIq;
      string tCqrdqVwnh;
      string brSxAfdspn;
      if(aYSWWwhcZu == iQJngJgWBB){jDPRdqChLn = true;}
      else if(iQJngJgWBB == aYSWWwhcZu){GxhHwQogmZ = true;}
      if(dghAFYTHFW == ILsZsNgbEW){HtGupuUBZU = true;}
      else if(ILsZsNgbEW == dghAFYTHFW){QkRfnxRaZm = true;}
      if(bMLdheRFlN == qlMtBuLUcw){abILjNIwGy = true;}
      else if(qlMtBuLUcw == bMLdheRFlN){jbgoCZEpiz = true;}
      if(yrwRJhhXOd == nLmwegHFhy){ouTTHqlYNm = true;}
      else if(nLmwegHFhy == yrwRJhhXOd){bTIXueCgEV = true;}
      if(aPGKjTgzLW == rXxClJAJdj){nnkAZhkWGE = true;}
      else if(rXxClJAJdj == aPGKjTgzLW){VuLfhLQKIF = true;}
      if(FZyxNOMnnq == SwopHwEPmk){LcTUNItDhQ = true;}
      else if(SwopHwEPmk == FZyxNOMnnq){PjxMdeQrFW = true;}
      if(ebqMwkkQpW == xLsEMxYxtM){iGbMzMamnz = true;}
      else if(xLsEMxYxtM == ebqMwkkQpW){LzxnyxctjJ = true;}
      if(purawMFBtO == FasGeSoJIq){IummgmqIkC = true;}
      if(xRtPHYtyDu == tCqrdqVwnh){bkoBlLbzFy = true;}
      if(UtiWyzYtNy == brSxAfdspn){wiqxDhLXAN = true;}
      while(FasGeSoJIq == purawMFBtO){ujVERoPBHp = true;}
      while(tCqrdqVwnh == tCqrdqVwnh){IshciqVusP = true;}
      while(brSxAfdspn == brSxAfdspn){NJbhKWxWgj = true;}
      if(jDPRdqChLn == true){jDPRdqChLn = false;}
      if(HtGupuUBZU == true){HtGupuUBZU = false;}
      if(abILjNIwGy == true){abILjNIwGy = false;}
      if(ouTTHqlYNm == true){ouTTHqlYNm = false;}
      if(nnkAZhkWGE == true){nnkAZhkWGE = false;}
      if(LcTUNItDhQ == true){LcTUNItDhQ = false;}
      if(iGbMzMamnz == true){iGbMzMamnz = false;}
      if(IummgmqIkC == true){IummgmqIkC = false;}
      if(bkoBlLbzFy == true){bkoBlLbzFy = false;}
      if(wiqxDhLXAN == true){wiqxDhLXAN = false;}
      if(GxhHwQogmZ == true){GxhHwQogmZ = false;}
      if(QkRfnxRaZm == true){QkRfnxRaZm = false;}
      if(jbgoCZEpiz == true){jbgoCZEpiz = false;}
      if(bTIXueCgEV == true){bTIXueCgEV = false;}
      if(VuLfhLQKIF == true){VuLfhLQKIF = false;}
      if(PjxMdeQrFW == true){PjxMdeQrFW = false;}
      if(LzxnyxctjJ == true){LzxnyxctjJ = false;}
      if(ujVERoPBHp == true){ujVERoPBHp = false;}
      if(IshciqVusP == true){IshciqVusP = false;}
      if(NJbhKWxWgj == true){NJbhKWxWgj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WIEVXHVEDT
{ 
  void nBBRtAsYhb()
  { 
      bool gYbyhVTprV = false;
      bool ZeHsQNEBPK = false;
      bool CVBsopiRfu = false;
      bool pqOCMUFTdT = false;
      bool zaSknrjyUa = false;
      bool jhRbNKXhIq = false;
      bool mBJxfiQEgw = false;
      bool HjWuKaDQEx = false;
      bool GjxELIYloB = false;
      bool xXKmgxQVFF = false;
      bool aeRYTzKWQm = false;
      bool HHcLkCEFkX = false;
      bool JdPeyiYjNN = false;
      bool KhKzVaSYkL = false;
      bool xBVCtptzBe = false;
      bool sKjgwwOlZj = false;
      bool aaICbZwdpJ = false;
      bool jeUZZHHNkI = false;
      bool oZhiUzycDY = false;
      bool LfzBkVsTZT = false;
      string oMYZSDnmiU;
      string jRboBwWzPc;
      string eFisfAEIzg;
      string QaWYKLZzsS;
      string jNQZZHAHjM;
      string jFmnmicetR;
      string zfnBEIuDdo;
      string WWFwsQoITY;
      string AwYlxKmilb;
      string htFbTEZOVQ;
      string LxctoMAptV;
      string sULUQCGBEs;
      string VMQCHcRdcn;
      string qamCNDTexj;
      string KwnQiNhjLQ;
      string xXKbuyfamL;
      string uWNGdSgIHM;
      string iaDUiYYASs;
      string thSMpTQLHO;
      string XrrlnnihlP;
      if(oMYZSDnmiU == LxctoMAptV){gYbyhVTprV = true;}
      else if(LxctoMAptV == oMYZSDnmiU){aeRYTzKWQm = true;}
      if(jRboBwWzPc == sULUQCGBEs){ZeHsQNEBPK = true;}
      else if(sULUQCGBEs == jRboBwWzPc){HHcLkCEFkX = true;}
      if(eFisfAEIzg == VMQCHcRdcn){CVBsopiRfu = true;}
      else if(VMQCHcRdcn == eFisfAEIzg){JdPeyiYjNN = true;}
      if(QaWYKLZzsS == qamCNDTexj){pqOCMUFTdT = true;}
      else if(qamCNDTexj == QaWYKLZzsS){KhKzVaSYkL = true;}
      if(jNQZZHAHjM == KwnQiNhjLQ){zaSknrjyUa = true;}
      else if(KwnQiNhjLQ == jNQZZHAHjM){xBVCtptzBe = true;}
      if(jFmnmicetR == xXKbuyfamL){jhRbNKXhIq = true;}
      else if(xXKbuyfamL == jFmnmicetR){sKjgwwOlZj = true;}
      if(zfnBEIuDdo == uWNGdSgIHM){mBJxfiQEgw = true;}
      else if(uWNGdSgIHM == zfnBEIuDdo){aaICbZwdpJ = true;}
      if(WWFwsQoITY == iaDUiYYASs){HjWuKaDQEx = true;}
      if(AwYlxKmilb == thSMpTQLHO){GjxELIYloB = true;}
      if(htFbTEZOVQ == XrrlnnihlP){xXKmgxQVFF = true;}
      while(iaDUiYYASs == WWFwsQoITY){jeUZZHHNkI = true;}
      while(thSMpTQLHO == thSMpTQLHO){oZhiUzycDY = true;}
      while(XrrlnnihlP == XrrlnnihlP){LfzBkVsTZT = true;}
      if(gYbyhVTprV == true){gYbyhVTprV = false;}
      if(ZeHsQNEBPK == true){ZeHsQNEBPK = false;}
      if(CVBsopiRfu == true){CVBsopiRfu = false;}
      if(pqOCMUFTdT == true){pqOCMUFTdT = false;}
      if(zaSknrjyUa == true){zaSknrjyUa = false;}
      if(jhRbNKXhIq == true){jhRbNKXhIq = false;}
      if(mBJxfiQEgw == true){mBJxfiQEgw = false;}
      if(HjWuKaDQEx == true){HjWuKaDQEx = false;}
      if(GjxELIYloB == true){GjxELIYloB = false;}
      if(xXKmgxQVFF == true){xXKmgxQVFF = false;}
      if(aeRYTzKWQm == true){aeRYTzKWQm = false;}
      if(HHcLkCEFkX == true){HHcLkCEFkX = false;}
      if(JdPeyiYjNN == true){JdPeyiYjNN = false;}
      if(KhKzVaSYkL == true){KhKzVaSYkL = false;}
      if(xBVCtptzBe == true){xBVCtptzBe = false;}
      if(sKjgwwOlZj == true){sKjgwwOlZj = false;}
      if(aaICbZwdpJ == true){aaICbZwdpJ = false;}
      if(jeUZZHHNkI == true){jeUZZHHNkI = false;}
      if(oZhiUzycDY == true){oZhiUzycDY = false;}
      if(LfzBkVsTZT == true){LfzBkVsTZT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CPBLZXYUIM
{ 
  void DpghPeVBqs()
  { 
      bool RKmiMKojhI = false;
      bool dcurHNkzlj = false;
      bool uFSJzJBuLV = false;
      bool fscuFXsMDt = false;
      bool KmzTkaNPmf = false;
      bool bOtIRLqdiC = false;
      bool JtYMbkzzlj = false;
      bool TYwrZHFzYX = false;
      bool wlHfsQyUdt = false;
      bool fPcAkPOmqA = false;
      bool aNdCLDSCiJ = false;
      bool FrFLFWmsTL = false;
      bool leVrEDuuGK = false;
      bool sSIXwhIETo = false;
      bool ywlUnnKRuN = false;
      bool cVTeOwqkyI = false;
      bool IWuYQmigOX = false;
      bool OhYdNosBDP = false;
      bool rcAsgWzcJA = false;
      bool qWGLfaBNEB = false;
      string VTjSGmbNfn;
      string USqoClJbCV;
      string nuiAIxBfTL;
      string zFfxmePNCw;
      string wBBlrnswQC;
      string joQpDZkiKJ;
      string kwsUoPmtCX;
      string VwZFZmsxIT;
      string DFPRbKootM;
      string tgCrKAHgxc;
      string BzsrfnOKQP;
      string fnapGefbre;
      string ZyAocosguY;
      string fMAeRDMWln;
      string ueXdgFGLle;
      string TfnRDDzDPp;
      string tgPqjTnuYD;
      string SJpEiuZWIX;
      string iguPFAXhbO;
      string EIHusNOgsN;
      if(VTjSGmbNfn == BzsrfnOKQP){RKmiMKojhI = true;}
      else if(BzsrfnOKQP == VTjSGmbNfn){aNdCLDSCiJ = true;}
      if(USqoClJbCV == fnapGefbre){dcurHNkzlj = true;}
      else if(fnapGefbre == USqoClJbCV){FrFLFWmsTL = true;}
      if(nuiAIxBfTL == ZyAocosguY){uFSJzJBuLV = true;}
      else if(ZyAocosguY == nuiAIxBfTL){leVrEDuuGK = true;}
      if(zFfxmePNCw == fMAeRDMWln){fscuFXsMDt = true;}
      else if(fMAeRDMWln == zFfxmePNCw){sSIXwhIETo = true;}
      if(wBBlrnswQC == ueXdgFGLle){KmzTkaNPmf = true;}
      else if(ueXdgFGLle == wBBlrnswQC){ywlUnnKRuN = true;}
      if(joQpDZkiKJ == TfnRDDzDPp){bOtIRLqdiC = true;}
      else if(TfnRDDzDPp == joQpDZkiKJ){cVTeOwqkyI = true;}
      if(kwsUoPmtCX == tgPqjTnuYD){JtYMbkzzlj = true;}
      else if(tgPqjTnuYD == kwsUoPmtCX){IWuYQmigOX = true;}
      if(VwZFZmsxIT == SJpEiuZWIX){TYwrZHFzYX = true;}
      if(DFPRbKootM == iguPFAXhbO){wlHfsQyUdt = true;}
      if(tgCrKAHgxc == EIHusNOgsN){fPcAkPOmqA = true;}
      while(SJpEiuZWIX == VwZFZmsxIT){OhYdNosBDP = true;}
      while(iguPFAXhbO == iguPFAXhbO){rcAsgWzcJA = true;}
      while(EIHusNOgsN == EIHusNOgsN){qWGLfaBNEB = true;}
      if(RKmiMKojhI == true){RKmiMKojhI = false;}
      if(dcurHNkzlj == true){dcurHNkzlj = false;}
      if(uFSJzJBuLV == true){uFSJzJBuLV = false;}
      if(fscuFXsMDt == true){fscuFXsMDt = false;}
      if(KmzTkaNPmf == true){KmzTkaNPmf = false;}
      if(bOtIRLqdiC == true){bOtIRLqdiC = false;}
      if(JtYMbkzzlj == true){JtYMbkzzlj = false;}
      if(TYwrZHFzYX == true){TYwrZHFzYX = false;}
      if(wlHfsQyUdt == true){wlHfsQyUdt = false;}
      if(fPcAkPOmqA == true){fPcAkPOmqA = false;}
      if(aNdCLDSCiJ == true){aNdCLDSCiJ = false;}
      if(FrFLFWmsTL == true){FrFLFWmsTL = false;}
      if(leVrEDuuGK == true){leVrEDuuGK = false;}
      if(sSIXwhIETo == true){sSIXwhIETo = false;}
      if(ywlUnnKRuN == true){ywlUnnKRuN = false;}
      if(cVTeOwqkyI == true){cVTeOwqkyI = false;}
      if(IWuYQmigOX == true){IWuYQmigOX = false;}
      if(OhYdNosBDP == true){OhYdNosBDP = false;}
      if(rcAsgWzcJA == true){rcAsgWzcJA = false;}
      if(qWGLfaBNEB == true){qWGLfaBNEB = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KABUXGZXPW
{ 
  void KOSDGfhpqZ()
  { 
      bool JUaUJNiNWA = false;
      bool AAfoSbVJBY = false;
      bool QEpNsDiwdO = false;
      bool zdigjAxxVY = false;
      bool FguqqpfkXl = false;
      bool LkZzYEdNaF = false;
      bool eliHBVRzFS = false;
      bool eXncSiIzVC = false;
      bool XxYrDNEiHf = false;
      bool ViVPsTxnpU = false;
      bool YgfFHmxxgp = false;
      bool ueqYhgMqEm = false;
      bool RhqYwwCXiy = false;
      bool kUJBcpQSjP = false;
      bool PHBRkpqxgM = false;
      bool HFiCCqoHIL = false;
      bool kYNhEZVpur = false;
      bool QsHnXKfZnF = false;
      bool EEqWqelHyl = false;
      bool BJKYeqsTMK = false;
      string FcifgUiQDu;
      string qKmqiHObcn;
      string DTAzwBCtEW;
      string zgWjCSJVRU;
      string iBzekSDOZL;
      string zTCFZkcjif;
      string RoLgwJnDoP;
      string yrJIFBUUNd;
      string xnTrWeeQKQ;
      string oYllDspRia;
      string dAZJNGKtAY;
      string PIVFhdUSJQ;
      string eWsXZYFWEL;
      string LDkmzxNKeP;
      string UjQkBgDgEY;
      string YWHSuHHCin;
      string BmooNLXhFj;
      string PKnswtGHVs;
      string VYRemnuSrr;
      string pLaqOgubXo;
      if(FcifgUiQDu == dAZJNGKtAY){JUaUJNiNWA = true;}
      else if(dAZJNGKtAY == FcifgUiQDu){YgfFHmxxgp = true;}
      if(qKmqiHObcn == PIVFhdUSJQ){AAfoSbVJBY = true;}
      else if(PIVFhdUSJQ == qKmqiHObcn){ueqYhgMqEm = true;}
      if(DTAzwBCtEW == eWsXZYFWEL){QEpNsDiwdO = true;}
      else if(eWsXZYFWEL == DTAzwBCtEW){RhqYwwCXiy = true;}
      if(zgWjCSJVRU == LDkmzxNKeP){zdigjAxxVY = true;}
      else if(LDkmzxNKeP == zgWjCSJVRU){kUJBcpQSjP = true;}
      if(iBzekSDOZL == UjQkBgDgEY){FguqqpfkXl = true;}
      else if(UjQkBgDgEY == iBzekSDOZL){PHBRkpqxgM = true;}
      if(zTCFZkcjif == YWHSuHHCin){LkZzYEdNaF = true;}
      else if(YWHSuHHCin == zTCFZkcjif){HFiCCqoHIL = true;}
      if(RoLgwJnDoP == BmooNLXhFj){eliHBVRzFS = true;}
      else if(BmooNLXhFj == RoLgwJnDoP){kYNhEZVpur = true;}
      if(yrJIFBUUNd == PKnswtGHVs){eXncSiIzVC = true;}
      if(xnTrWeeQKQ == VYRemnuSrr){XxYrDNEiHf = true;}
      if(oYllDspRia == pLaqOgubXo){ViVPsTxnpU = true;}
      while(PKnswtGHVs == yrJIFBUUNd){QsHnXKfZnF = true;}
      while(VYRemnuSrr == VYRemnuSrr){EEqWqelHyl = true;}
      while(pLaqOgubXo == pLaqOgubXo){BJKYeqsTMK = true;}
      if(JUaUJNiNWA == true){JUaUJNiNWA = false;}
      if(AAfoSbVJBY == true){AAfoSbVJBY = false;}
      if(QEpNsDiwdO == true){QEpNsDiwdO = false;}
      if(zdigjAxxVY == true){zdigjAxxVY = false;}
      if(FguqqpfkXl == true){FguqqpfkXl = false;}
      if(LkZzYEdNaF == true){LkZzYEdNaF = false;}
      if(eliHBVRzFS == true){eliHBVRzFS = false;}
      if(eXncSiIzVC == true){eXncSiIzVC = false;}
      if(XxYrDNEiHf == true){XxYrDNEiHf = false;}
      if(ViVPsTxnpU == true){ViVPsTxnpU = false;}
      if(YgfFHmxxgp == true){YgfFHmxxgp = false;}
      if(ueqYhgMqEm == true){ueqYhgMqEm = false;}
      if(RhqYwwCXiy == true){RhqYwwCXiy = false;}
      if(kUJBcpQSjP == true){kUJBcpQSjP = false;}
      if(PHBRkpqxgM == true){PHBRkpqxgM = false;}
      if(HFiCCqoHIL == true){HFiCCqoHIL = false;}
      if(kYNhEZVpur == true){kYNhEZVpur = false;}
      if(QsHnXKfZnF == true){QsHnXKfZnF = false;}
      if(EEqWqelHyl == true){EEqWqelHyl = false;}
      if(BJKYeqsTMK == true){BJKYeqsTMK = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FPBXNBPKPF
{ 
  void IexSpeArlK()
  { 
      bool cxGJFrOXsU = false;
      bool ybFVeMIKyL = false;
      bool riThDsWzcZ = false;
      bool HwxsnxjjYS = false;
      bool HMtRhnwTjO = false;
      bool EoJWktdYFM = false;
      bool UPuSMCeMhh = false;
      bool cXleEfnBzP = false;
      bool xCONVSmZIs = false;
      bool TLdLxrRaFf = false;
      bool JjlClfDdSE = false;
      bool FbZpOSAgxt = false;
      bool OEEnNQtZhL = false;
      bool ONwSFNPMbo = false;
      bool OJHDnqlmxd = false;
      bool qjcKQEexnU = false;
      bool lcuGprfGjB = false;
      bool DnFIeFwAAV = false;
      bool eGcDUejkBB = false;
      bool hRXGDlNBNw = false;
      string hTnnAaNoVC;
      string xhUnjOAjsc;
      string rRcakfQoRp;
      string JnYWMenVpk;
      string SHrajfDgIA;
      string tTciUPqNCU;
      string JXWfrJJyaE;
      string ZdsHKkYBqF;
      string cjgQupLoBx;
      string naSVaujMha;
      string WEFDaPZVYj;
      string poWxrZpFjC;
      string yUxdQzLXfP;
      string BMSPiXdcpf;
      string PoQRGzEUdB;
      string EdbfcBHbma;
      string DCoIicVGOT;
      string uMOEmIDfWQ;
      string uqtdKWjmmg;
      string PBpPbMAHhd;
      if(hTnnAaNoVC == WEFDaPZVYj){cxGJFrOXsU = true;}
      else if(WEFDaPZVYj == hTnnAaNoVC){JjlClfDdSE = true;}
      if(xhUnjOAjsc == poWxrZpFjC){ybFVeMIKyL = true;}
      else if(poWxrZpFjC == xhUnjOAjsc){FbZpOSAgxt = true;}
      if(rRcakfQoRp == yUxdQzLXfP){riThDsWzcZ = true;}
      else if(yUxdQzLXfP == rRcakfQoRp){OEEnNQtZhL = true;}
      if(JnYWMenVpk == BMSPiXdcpf){HwxsnxjjYS = true;}
      else if(BMSPiXdcpf == JnYWMenVpk){ONwSFNPMbo = true;}
      if(SHrajfDgIA == PoQRGzEUdB){HMtRhnwTjO = true;}
      else if(PoQRGzEUdB == SHrajfDgIA){OJHDnqlmxd = true;}
      if(tTciUPqNCU == EdbfcBHbma){EoJWktdYFM = true;}
      else if(EdbfcBHbma == tTciUPqNCU){qjcKQEexnU = true;}
      if(JXWfrJJyaE == DCoIicVGOT){UPuSMCeMhh = true;}
      else if(DCoIicVGOT == JXWfrJJyaE){lcuGprfGjB = true;}
      if(ZdsHKkYBqF == uMOEmIDfWQ){cXleEfnBzP = true;}
      if(cjgQupLoBx == uqtdKWjmmg){xCONVSmZIs = true;}
      if(naSVaujMha == PBpPbMAHhd){TLdLxrRaFf = true;}
      while(uMOEmIDfWQ == ZdsHKkYBqF){DnFIeFwAAV = true;}
      while(uqtdKWjmmg == uqtdKWjmmg){eGcDUejkBB = true;}
      while(PBpPbMAHhd == PBpPbMAHhd){hRXGDlNBNw = true;}
      if(cxGJFrOXsU == true){cxGJFrOXsU = false;}
      if(ybFVeMIKyL == true){ybFVeMIKyL = false;}
      if(riThDsWzcZ == true){riThDsWzcZ = false;}
      if(HwxsnxjjYS == true){HwxsnxjjYS = false;}
      if(HMtRhnwTjO == true){HMtRhnwTjO = false;}
      if(EoJWktdYFM == true){EoJWktdYFM = false;}
      if(UPuSMCeMhh == true){UPuSMCeMhh = false;}
      if(cXleEfnBzP == true){cXleEfnBzP = false;}
      if(xCONVSmZIs == true){xCONVSmZIs = false;}
      if(TLdLxrRaFf == true){TLdLxrRaFf = false;}
      if(JjlClfDdSE == true){JjlClfDdSE = false;}
      if(FbZpOSAgxt == true){FbZpOSAgxt = false;}
      if(OEEnNQtZhL == true){OEEnNQtZhL = false;}
      if(ONwSFNPMbo == true){ONwSFNPMbo = false;}
      if(OJHDnqlmxd == true){OJHDnqlmxd = false;}
      if(qjcKQEexnU == true){qjcKQEexnU = false;}
      if(lcuGprfGjB == true){lcuGprfGjB = false;}
      if(DnFIeFwAAV == true){DnFIeFwAAV = false;}
      if(eGcDUejkBB == true){eGcDUejkBB = false;}
      if(hRXGDlNBNw == true){hRXGDlNBNw = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SPEESSNZFU
{ 
  void zsUynmrCjS()
  { 
      bool zitsldVnkn = false;
      bool OGNhJuRBVw = false;
      bool ZsaQuuCpdK = false;
      bool PbeefQKhKX = false;
      bool SFLOuXAeBF = false;
      bool NEGDdGHcxp = false;
      bool afKZYlaiCs = false;
      bool kjqxdUgLVW = false;
      bool uWoXxKVaBY = false;
      bool IhjTxlukpJ = false;
      bool wQbHUcNJYI = false;
      bool YwHatDqlcN = false;
      bool QzhdkDzWAr = false;
      bool ZuVMZeugDT = false;
      bool xMCrzMZJZs = false;
      bool tIIbsxiyWQ = false;
      bool dpIgidatAP = false;
      bool IKhJQDxodY = false;
      bool cpxGxQmsTc = false;
      bool uwJlpHDUIV = false;
      string LNCxrETiOj;
      string OplEJPffDU;
      string CTiJsdXRaF;
      string HrTjujyeZw;
      string mzITIsHLIl;
      string btxBAresEO;
      string fBIJbWWCIZ;
      string RBGHgCLPAz;
      string WMJncMuSjD;
      string EFZLPkqzql;
      string WEChUanpTx;
      string fOrErTqTfP;
      string aDSFULatqw;
      string LMYNcRKaSB;
      string YEzlWoMVTu;
      string tWSbKSlwNI;
      string AsyWkTpdaZ;
      string JPAepVEfhe;
      string zEawBnsxDJ;
      string eNNKBfGbAD;
      if(LNCxrETiOj == WEChUanpTx){zitsldVnkn = true;}
      else if(WEChUanpTx == LNCxrETiOj){wQbHUcNJYI = true;}
      if(OplEJPffDU == fOrErTqTfP){OGNhJuRBVw = true;}
      else if(fOrErTqTfP == OplEJPffDU){YwHatDqlcN = true;}
      if(CTiJsdXRaF == aDSFULatqw){ZsaQuuCpdK = true;}
      else if(aDSFULatqw == CTiJsdXRaF){QzhdkDzWAr = true;}
      if(HrTjujyeZw == LMYNcRKaSB){PbeefQKhKX = true;}
      else if(LMYNcRKaSB == HrTjujyeZw){ZuVMZeugDT = true;}
      if(mzITIsHLIl == YEzlWoMVTu){SFLOuXAeBF = true;}
      else if(YEzlWoMVTu == mzITIsHLIl){xMCrzMZJZs = true;}
      if(btxBAresEO == tWSbKSlwNI){NEGDdGHcxp = true;}
      else if(tWSbKSlwNI == btxBAresEO){tIIbsxiyWQ = true;}
      if(fBIJbWWCIZ == AsyWkTpdaZ){afKZYlaiCs = true;}
      else if(AsyWkTpdaZ == fBIJbWWCIZ){dpIgidatAP = true;}
      if(RBGHgCLPAz == JPAepVEfhe){kjqxdUgLVW = true;}
      if(WMJncMuSjD == zEawBnsxDJ){uWoXxKVaBY = true;}
      if(EFZLPkqzql == eNNKBfGbAD){IhjTxlukpJ = true;}
      while(JPAepVEfhe == RBGHgCLPAz){IKhJQDxodY = true;}
      while(zEawBnsxDJ == zEawBnsxDJ){cpxGxQmsTc = true;}
      while(eNNKBfGbAD == eNNKBfGbAD){uwJlpHDUIV = true;}
      if(zitsldVnkn == true){zitsldVnkn = false;}
      if(OGNhJuRBVw == true){OGNhJuRBVw = false;}
      if(ZsaQuuCpdK == true){ZsaQuuCpdK = false;}
      if(PbeefQKhKX == true){PbeefQKhKX = false;}
      if(SFLOuXAeBF == true){SFLOuXAeBF = false;}
      if(NEGDdGHcxp == true){NEGDdGHcxp = false;}
      if(afKZYlaiCs == true){afKZYlaiCs = false;}
      if(kjqxdUgLVW == true){kjqxdUgLVW = false;}
      if(uWoXxKVaBY == true){uWoXxKVaBY = false;}
      if(IhjTxlukpJ == true){IhjTxlukpJ = false;}
      if(wQbHUcNJYI == true){wQbHUcNJYI = false;}
      if(YwHatDqlcN == true){YwHatDqlcN = false;}
      if(QzhdkDzWAr == true){QzhdkDzWAr = false;}
      if(ZuVMZeugDT == true){ZuVMZeugDT = false;}
      if(xMCrzMZJZs == true){xMCrzMZJZs = false;}
      if(tIIbsxiyWQ == true){tIIbsxiyWQ = false;}
      if(dpIgidatAP == true){dpIgidatAP = false;}
      if(IKhJQDxodY == true){IKhJQDxodY = false;}
      if(cpxGxQmsTc == true){cpxGxQmsTc = false;}
      if(uwJlpHDUIV == true){uwJlpHDUIV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CRZLRRKRFH
{ 
  void LiqpRSITeW()
  { 
      bool BpTAPmRlhj = false;
      bool xQsdeKqiBW = false;
      bool wwkuXmpaNH = false;
      bool JccQmWbZBe = false;
      bool qhoJHMJRrp = false;
      bool qhJZoEEYcc = false;
      bool xpYaaUmbqI = false;
      bool hQhVeOGbmK = false;
      bool dXhWPaSDbh = false;
      bool RDxYhAIOhm = false;
      bool YhZjHZAmua = false;
      bool xckRzYIBKz = false;
      bool BjzJqdTInd = false;
      bool FrsAluxyPp = false;
      bool afHBjnSeZJ = false;
      bool rHDiknhQrd = false;
      bool GrzQUFijfD = false;
      bool aJlUYVbyZC = false;
      bool LniekxoKPx = false;
      bool NgFliigqfe = false;
      string ewFEffLObg;
      string VnhCPucJxq;
      string rGRGPtrRHc;
      string rhbwKSFAJf;
      string VlSEtSfrZQ;
      string zwohMXeyok;
      string msGynmMlqx;
      string obAQUGnNMx;
      string Ozqqdwrsil;
      string mGLXsbfTQG;
      string FsBbaWuYNG;
      string jRkqVnCOUq;
      string tTCFJhQcXs;
      string ZTwFqUyOPG;
      string BTfcCVVTBF;
      string ZQzYLlqwSr;
      string GSBTfQUeAB;
      string omyNpanZCn;
      string MwaRSJTXtF;
      string dCwXAiRLwl;
      if(ewFEffLObg == FsBbaWuYNG){BpTAPmRlhj = true;}
      else if(FsBbaWuYNG == ewFEffLObg){YhZjHZAmua = true;}
      if(VnhCPucJxq == jRkqVnCOUq){xQsdeKqiBW = true;}
      else if(jRkqVnCOUq == VnhCPucJxq){xckRzYIBKz = true;}
      if(rGRGPtrRHc == tTCFJhQcXs){wwkuXmpaNH = true;}
      else if(tTCFJhQcXs == rGRGPtrRHc){BjzJqdTInd = true;}
      if(rhbwKSFAJf == ZTwFqUyOPG){JccQmWbZBe = true;}
      else if(ZTwFqUyOPG == rhbwKSFAJf){FrsAluxyPp = true;}
      if(VlSEtSfrZQ == BTfcCVVTBF){qhoJHMJRrp = true;}
      else if(BTfcCVVTBF == VlSEtSfrZQ){afHBjnSeZJ = true;}
      if(zwohMXeyok == ZQzYLlqwSr){qhJZoEEYcc = true;}
      else if(ZQzYLlqwSr == zwohMXeyok){rHDiknhQrd = true;}
      if(msGynmMlqx == GSBTfQUeAB){xpYaaUmbqI = true;}
      else if(GSBTfQUeAB == msGynmMlqx){GrzQUFijfD = true;}
      if(obAQUGnNMx == omyNpanZCn){hQhVeOGbmK = true;}
      if(Ozqqdwrsil == MwaRSJTXtF){dXhWPaSDbh = true;}
      if(mGLXsbfTQG == dCwXAiRLwl){RDxYhAIOhm = true;}
      while(omyNpanZCn == obAQUGnNMx){aJlUYVbyZC = true;}
      while(MwaRSJTXtF == MwaRSJTXtF){LniekxoKPx = true;}
      while(dCwXAiRLwl == dCwXAiRLwl){NgFliigqfe = true;}
      if(BpTAPmRlhj == true){BpTAPmRlhj = false;}
      if(xQsdeKqiBW == true){xQsdeKqiBW = false;}
      if(wwkuXmpaNH == true){wwkuXmpaNH = false;}
      if(JccQmWbZBe == true){JccQmWbZBe = false;}
      if(qhoJHMJRrp == true){qhoJHMJRrp = false;}
      if(qhJZoEEYcc == true){qhJZoEEYcc = false;}
      if(xpYaaUmbqI == true){xpYaaUmbqI = false;}
      if(hQhVeOGbmK == true){hQhVeOGbmK = false;}
      if(dXhWPaSDbh == true){dXhWPaSDbh = false;}
      if(RDxYhAIOhm == true){RDxYhAIOhm = false;}
      if(YhZjHZAmua == true){YhZjHZAmua = false;}
      if(xckRzYIBKz == true){xckRzYIBKz = false;}
      if(BjzJqdTInd == true){BjzJqdTInd = false;}
      if(FrsAluxyPp == true){FrsAluxyPp = false;}
      if(afHBjnSeZJ == true){afHBjnSeZJ = false;}
      if(rHDiknhQrd == true){rHDiknhQrd = false;}
      if(GrzQUFijfD == true){GrzQUFijfD = false;}
      if(aJlUYVbyZC == true){aJlUYVbyZC = false;}
      if(LniekxoKPx == true){LniekxoKPx = false;}
      if(NgFliigqfe == true){NgFliigqfe = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RNVRAWUARD
{ 
  void nCqAnyYEBk()
  { 
      bool IOTipyjulX = false;
      bool ctgDoEbpnx = false;
      bool tYbMChrCVg = false;
      bool rQSHrUbmVZ = false;
      bool aVqiTJIzEY = false;
      bool obxQujlHgI = false;
      bool FgYFeMbYKT = false;
      bool boTNVXgsrs = false;
      bool YsywqPTBMI = false;
      bool hUFNUtwlEH = false;
      bool gjnjxzIitW = false;
      bool CsOniLhcNO = false;
      bool rZLBlHiGsb = false;
      bool bunjLoFOzr = false;
      bool HjHpGnFRdI = false;
      bool LSXoOHXlyo = false;
      bool PBoRtGFdbt = false;
      bool GTdVGISUux = false;
      bool BRckPIXzVE = false;
      bool QSQJbGuowz = false;
      string fmkOMaTjxS;
      string ltnoGNGKTo;
      string beFERQoorT;
      string bFSMVMFPWo;
      string anzpxHGtMQ;
      string QzGqnAhgxu;
      string keqdqfbbSs;
      string HqndNnfOGJ;
      string JubbCbybXU;
      string eYRaDwwUzQ;
      string feaTIZLcdA;
      string USZnkpDPbO;
      string gADxNiZMZQ;
      string qKPpceTHSc;
      string QVxWAMydpN;
      string sfskifadFZ;
      string zzCMIKDiKG;
      string WuwyyLCgxB;
      string juWMXLQdGx;
      string DoCscRSkOo;
      if(fmkOMaTjxS == feaTIZLcdA){IOTipyjulX = true;}
      else if(feaTIZLcdA == fmkOMaTjxS){gjnjxzIitW = true;}
      if(ltnoGNGKTo == USZnkpDPbO){ctgDoEbpnx = true;}
      else if(USZnkpDPbO == ltnoGNGKTo){CsOniLhcNO = true;}
      if(beFERQoorT == gADxNiZMZQ){tYbMChrCVg = true;}
      else if(gADxNiZMZQ == beFERQoorT){rZLBlHiGsb = true;}
      if(bFSMVMFPWo == qKPpceTHSc){rQSHrUbmVZ = true;}
      else if(qKPpceTHSc == bFSMVMFPWo){bunjLoFOzr = true;}
      if(anzpxHGtMQ == QVxWAMydpN){aVqiTJIzEY = true;}
      else if(QVxWAMydpN == anzpxHGtMQ){HjHpGnFRdI = true;}
      if(QzGqnAhgxu == sfskifadFZ){obxQujlHgI = true;}
      else if(sfskifadFZ == QzGqnAhgxu){LSXoOHXlyo = true;}
      if(keqdqfbbSs == zzCMIKDiKG){FgYFeMbYKT = true;}
      else if(zzCMIKDiKG == keqdqfbbSs){PBoRtGFdbt = true;}
      if(HqndNnfOGJ == WuwyyLCgxB){boTNVXgsrs = true;}
      if(JubbCbybXU == juWMXLQdGx){YsywqPTBMI = true;}
      if(eYRaDwwUzQ == DoCscRSkOo){hUFNUtwlEH = true;}
      while(WuwyyLCgxB == HqndNnfOGJ){GTdVGISUux = true;}
      while(juWMXLQdGx == juWMXLQdGx){BRckPIXzVE = true;}
      while(DoCscRSkOo == DoCscRSkOo){QSQJbGuowz = true;}
      if(IOTipyjulX == true){IOTipyjulX = false;}
      if(ctgDoEbpnx == true){ctgDoEbpnx = false;}
      if(tYbMChrCVg == true){tYbMChrCVg = false;}
      if(rQSHrUbmVZ == true){rQSHrUbmVZ = false;}
      if(aVqiTJIzEY == true){aVqiTJIzEY = false;}
      if(obxQujlHgI == true){obxQujlHgI = false;}
      if(FgYFeMbYKT == true){FgYFeMbYKT = false;}
      if(boTNVXgsrs == true){boTNVXgsrs = false;}
      if(YsywqPTBMI == true){YsywqPTBMI = false;}
      if(hUFNUtwlEH == true){hUFNUtwlEH = false;}
      if(gjnjxzIitW == true){gjnjxzIitW = false;}
      if(CsOniLhcNO == true){CsOniLhcNO = false;}
      if(rZLBlHiGsb == true){rZLBlHiGsb = false;}
      if(bunjLoFOzr == true){bunjLoFOzr = false;}
      if(HjHpGnFRdI == true){HjHpGnFRdI = false;}
      if(LSXoOHXlyo == true){LSXoOHXlyo = false;}
      if(PBoRtGFdbt == true){PBoRtGFdbt = false;}
      if(GTdVGISUux == true){GTdVGISUux = false;}
      if(BRckPIXzVE == true){BRckPIXzVE = false;}
      if(QSQJbGuowz == true){QSQJbGuowz = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NNNZYTJFHK
{ 
  void sAanPYcbgZ()
  { 
      bool MJhYpyypWA = false;
      bool VRRasiRDMK = false;
      bool QogAmJcoHQ = false;
      bool GzrJtQBRky = false;
      bool EIazmETOwX = false;
      bool lKqfVXcglg = false;
      bool UIhFsVYHgb = false;
      bool gASOGHOhNh = false;
      bool oOsidwIMnW = false;
      bool nTfEcuZUGQ = false;
      bool fLhUqlrbYN = false;
      bool ItTDqGPzax = false;
      bool NMSKmNUNkL = false;
      bool gsbwCcfYrr = false;
      bool DfmaNtzbMW = false;
      bool eatIDSsXgc = false;
      bool RdpNfOSXAN = false;
      bool ZqGisgZrws = false;
      bool NrBOfGzSrj = false;
      bool jIqOZZoYty = false;
      string IMBhgFzpfC;
      string QegHBVPydr;
      string HtVqPnypBY;
      string WRdslRGDIm;
      string THyGjqbjZD;
      string GZgttOqKRr;
      string HJnGfyAmVi;
      string PMppXAuOGY;
      string LNPySxjizW;
      string awkPeKTrZk;
      string poNpnsipWt;
      string TbDrgtsomi;
      string OQnzHTbZLN;
      string gDoViWzZKY;
      string NsMECzrrij;
      string RdEqVZAOiU;
      string RJeINPlhJJ;
      string tTeOUVDiwH;
      string aiqawCzxAJ;
      string VkpoGEOEqm;
      if(IMBhgFzpfC == poNpnsipWt){MJhYpyypWA = true;}
      else if(poNpnsipWt == IMBhgFzpfC){fLhUqlrbYN = true;}
      if(QegHBVPydr == TbDrgtsomi){VRRasiRDMK = true;}
      else if(TbDrgtsomi == QegHBVPydr){ItTDqGPzax = true;}
      if(HtVqPnypBY == OQnzHTbZLN){QogAmJcoHQ = true;}
      else if(OQnzHTbZLN == HtVqPnypBY){NMSKmNUNkL = true;}
      if(WRdslRGDIm == gDoViWzZKY){GzrJtQBRky = true;}
      else if(gDoViWzZKY == WRdslRGDIm){gsbwCcfYrr = true;}
      if(THyGjqbjZD == NsMECzrrij){EIazmETOwX = true;}
      else if(NsMECzrrij == THyGjqbjZD){DfmaNtzbMW = true;}
      if(GZgttOqKRr == RdEqVZAOiU){lKqfVXcglg = true;}
      else if(RdEqVZAOiU == GZgttOqKRr){eatIDSsXgc = true;}
      if(HJnGfyAmVi == RJeINPlhJJ){UIhFsVYHgb = true;}
      else if(RJeINPlhJJ == HJnGfyAmVi){RdpNfOSXAN = true;}
      if(PMppXAuOGY == tTeOUVDiwH){gASOGHOhNh = true;}
      if(LNPySxjizW == aiqawCzxAJ){oOsidwIMnW = true;}
      if(awkPeKTrZk == VkpoGEOEqm){nTfEcuZUGQ = true;}
      while(tTeOUVDiwH == PMppXAuOGY){ZqGisgZrws = true;}
      while(aiqawCzxAJ == aiqawCzxAJ){NrBOfGzSrj = true;}
      while(VkpoGEOEqm == VkpoGEOEqm){jIqOZZoYty = true;}
      if(MJhYpyypWA == true){MJhYpyypWA = false;}
      if(VRRasiRDMK == true){VRRasiRDMK = false;}
      if(QogAmJcoHQ == true){QogAmJcoHQ = false;}
      if(GzrJtQBRky == true){GzrJtQBRky = false;}
      if(EIazmETOwX == true){EIazmETOwX = false;}
      if(lKqfVXcglg == true){lKqfVXcglg = false;}
      if(UIhFsVYHgb == true){UIhFsVYHgb = false;}
      if(gASOGHOhNh == true){gASOGHOhNh = false;}
      if(oOsidwIMnW == true){oOsidwIMnW = false;}
      if(nTfEcuZUGQ == true){nTfEcuZUGQ = false;}
      if(fLhUqlrbYN == true){fLhUqlrbYN = false;}
      if(ItTDqGPzax == true){ItTDqGPzax = false;}
      if(NMSKmNUNkL == true){NMSKmNUNkL = false;}
      if(gsbwCcfYrr == true){gsbwCcfYrr = false;}
      if(DfmaNtzbMW == true){DfmaNtzbMW = false;}
      if(eatIDSsXgc == true){eatIDSsXgc = false;}
      if(RdpNfOSXAN == true){RdpNfOSXAN = false;}
      if(ZqGisgZrws == true){ZqGisgZrws = false;}
      if(NrBOfGzSrj == true){NrBOfGzSrj = false;}
      if(jIqOZZoYty == true){jIqOZZoYty = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SJWXYDABHN
{ 
  void jESUuCnoFU()
  { 
      bool HKgbYecqtF = false;
      bool ukGCzqdWlS = false;
      bool CykMCAJCJH = false;
      bool SppTlMyBMC = false;
      bool HTHlQVwwrK = false;
      bool yTIJrIYUZX = false;
      bool VZgllEtmQl = false;
      bool sbJjLiqywf = false;
      bool DMKyJzhgad = false;
      bool mdbogglUAL = false;
      bool aJfFRVJXxJ = false;
      bool QaXsUIPicm = false;
      bool WNLlyOAUDe = false;
      bool wVykEtgEfm = false;
      bool oJiRMMGhTX = false;
      bool RDrMoNysGD = false;
      bool UoUSNucmnZ = false;
      bool bKCoxNdsZl = false;
      bool HSAeNrdkwq = false;
      bool wmycRAnQEQ = false;
      string CQwSzLbQjf;
      string pRdYpViURD;
      string UVEgVMReMK;
      string ELJmQAmWwy;
      string oiQZCruAmQ;
      string XwoEwgLOwm;
      string twEZbeHpHo;
      string iMYSNkFeed;
      string QRrwlHVFpj;
      string nXZIEreqyX;
      string jErxhLXWpK;
      string bgjSVbEHmT;
      string ICzWBKmPNV;
      string GNfGsMYuNa;
      string jzPfIHmGlb;
      string nWDdOhEVwW;
      string XHITDNAcYJ;
      string DIsMjJlTOs;
      string XfdlgjKBrL;
      string HDzqCbpucu;
      if(CQwSzLbQjf == jErxhLXWpK){HKgbYecqtF = true;}
      else if(jErxhLXWpK == CQwSzLbQjf){aJfFRVJXxJ = true;}
      if(pRdYpViURD == bgjSVbEHmT){ukGCzqdWlS = true;}
      else if(bgjSVbEHmT == pRdYpViURD){QaXsUIPicm = true;}
      if(UVEgVMReMK == ICzWBKmPNV){CykMCAJCJH = true;}
      else if(ICzWBKmPNV == UVEgVMReMK){WNLlyOAUDe = true;}
      if(ELJmQAmWwy == GNfGsMYuNa){SppTlMyBMC = true;}
      else if(GNfGsMYuNa == ELJmQAmWwy){wVykEtgEfm = true;}
      if(oiQZCruAmQ == jzPfIHmGlb){HTHlQVwwrK = true;}
      else if(jzPfIHmGlb == oiQZCruAmQ){oJiRMMGhTX = true;}
      if(XwoEwgLOwm == nWDdOhEVwW){yTIJrIYUZX = true;}
      else if(nWDdOhEVwW == XwoEwgLOwm){RDrMoNysGD = true;}
      if(twEZbeHpHo == XHITDNAcYJ){VZgllEtmQl = true;}
      else if(XHITDNAcYJ == twEZbeHpHo){UoUSNucmnZ = true;}
      if(iMYSNkFeed == DIsMjJlTOs){sbJjLiqywf = true;}
      if(QRrwlHVFpj == XfdlgjKBrL){DMKyJzhgad = true;}
      if(nXZIEreqyX == HDzqCbpucu){mdbogglUAL = true;}
      while(DIsMjJlTOs == iMYSNkFeed){bKCoxNdsZl = true;}
      while(XfdlgjKBrL == XfdlgjKBrL){HSAeNrdkwq = true;}
      while(HDzqCbpucu == HDzqCbpucu){wmycRAnQEQ = true;}
      if(HKgbYecqtF == true){HKgbYecqtF = false;}
      if(ukGCzqdWlS == true){ukGCzqdWlS = false;}
      if(CykMCAJCJH == true){CykMCAJCJH = false;}
      if(SppTlMyBMC == true){SppTlMyBMC = false;}
      if(HTHlQVwwrK == true){HTHlQVwwrK = false;}
      if(yTIJrIYUZX == true){yTIJrIYUZX = false;}
      if(VZgllEtmQl == true){VZgllEtmQl = false;}
      if(sbJjLiqywf == true){sbJjLiqywf = false;}
      if(DMKyJzhgad == true){DMKyJzhgad = false;}
      if(mdbogglUAL == true){mdbogglUAL = false;}
      if(aJfFRVJXxJ == true){aJfFRVJXxJ = false;}
      if(QaXsUIPicm == true){QaXsUIPicm = false;}
      if(WNLlyOAUDe == true){WNLlyOAUDe = false;}
      if(wVykEtgEfm == true){wVykEtgEfm = false;}
      if(oJiRMMGhTX == true){oJiRMMGhTX = false;}
      if(RDrMoNysGD == true){RDrMoNysGD = false;}
      if(UoUSNucmnZ == true){UoUSNucmnZ = false;}
      if(bKCoxNdsZl == true){bKCoxNdsZl = false;}
      if(HSAeNrdkwq == true){HSAeNrdkwq = false;}
      if(wmycRAnQEQ == true){wmycRAnQEQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PQOHOOMQIH
{ 
  void JldiRLhCMq()
  { 
      bool IWJRlzugAa = false;
      bool WUDrEcBOMp = false;
      bool AaDmVGfsGQ = false;
      bool wbMKPMxQEh = false;
      bool EMJdorPeXC = false;
      bool bfNuQgTVwf = false;
      bool SCajdtIscq = false;
      bool fkKdkmRZyd = false;
      bool pAVDoPOQsh = false;
      bool saUYOClZzu = false;
      bool jWBgWpVjJd = false;
      bool PtXSoSgWuN = false;
      bool eqAuyEQlOi = false;
      bool ArZCsTyofy = false;
      bool teHOSzDCiw = false;
      bool VPdkPsWGhS = false;
      bool ZjUacXTUYU = false;
      bool TrqxTAjkbz = false;
      bool BNCSNkNomA = false;
      bool xrGNXJKflu = false;
      string FxETdsUwfz;
      string OdcVjoSPXl;
      string fPztBjzHqS;
      string ILHQKhSLMg;
      string IJhsMSHekY;
      string FYAIepxKSs;
      string tNFNqCVMKC;
      string VHDqdWhGgh;
      string cTVSMiKykU;
      string jIrSmtETCK;
      string wuUhCyVJdI;
      string IpdjQZqlBH;
      string rWaoRNOkEj;
      string RNEWkgznQO;
      string NAUbMtdIBM;
      string CRStRZTfQn;
      string ijFtpukHzi;
      string ZDVlsqCybX;
      string jGiqgAtKEL;
      string NTXbfcuZlR;
      if(FxETdsUwfz == wuUhCyVJdI){IWJRlzugAa = true;}
      else if(wuUhCyVJdI == FxETdsUwfz){jWBgWpVjJd = true;}
      if(OdcVjoSPXl == IpdjQZqlBH){WUDrEcBOMp = true;}
      else if(IpdjQZqlBH == OdcVjoSPXl){PtXSoSgWuN = true;}
      if(fPztBjzHqS == rWaoRNOkEj){AaDmVGfsGQ = true;}
      else if(rWaoRNOkEj == fPztBjzHqS){eqAuyEQlOi = true;}
      if(ILHQKhSLMg == RNEWkgznQO){wbMKPMxQEh = true;}
      else if(RNEWkgznQO == ILHQKhSLMg){ArZCsTyofy = true;}
      if(IJhsMSHekY == NAUbMtdIBM){EMJdorPeXC = true;}
      else if(NAUbMtdIBM == IJhsMSHekY){teHOSzDCiw = true;}
      if(FYAIepxKSs == CRStRZTfQn){bfNuQgTVwf = true;}
      else if(CRStRZTfQn == FYAIepxKSs){VPdkPsWGhS = true;}
      if(tNFNqCVMKC == ijFtpukHzi){SCajdtIscq = true;}
      else if(ijFtpukHzi == tNFNqCVMKC){ZjUacXTUYU = true;}
      if(VHDqdWhGgh == ZDVlsqCybX){fkKdkmRZyd = true;}
      if(cTVSMiKykU == jGiqgAtKEL){pAVDoPOQsh = true;}
      if(jIrSmtETCK == NTXbfcuZlR){saUYOClZzu = true;}
      while(ZDVlsqCybX == VHDqdWhGgh){TrqxTAjkbz = true;}
      while(jGiqgAtKEL == jGiqgAtKEL){BNCSNkNomA = true;}
      while(NTXbfcuZlR == NTXbfcuZlR){xrGNXJKflu = true;}
      if(IWJRlzugAa == true){IWJRlzugAa = false;}
      if(WUDrEcBOMp == true){WUDrEcBOMp = false;}
      if(AaDmVGfsGQ == true){AaDmVGfsGQ = false;}
      if(wbMKPMxQEh == true){wbMKPMxQEh = false;}
      if(EMJdorPeXC == true){EMJdorPeXC = false;}
      if(bfNuQgTVwf == true){bfNuQgTVwf = false;}
      if(SCajdtIscq == true){SCajdtIscq = false;}
      if(fkKdkmRZyd == true){fkKdkmRZyd = false;}
      if(pAVDoPOQsh == true){pAVDoPOQsh = false;}
      if(saUYOClZzu == true){saUYOClZzu = false;}
      if(jWBgWpVjJd == true){jWBgWpVjJd = false;}
      if(PtXSoSgWuN == true){PtXSoSgWuN = false;}
      if(eqAuyEQlOi == true){eqAuyEQlOi = false;}
      if(ArZCsTyofy == true){ArZCsTyofy = false;}
      if(teHOSzDCiw == true){teHOSzDCiw = false;}
      if(VPdkPsWGhS == true){VPdkPsWGhS = false;}
      if(ZjUacXTUYU == true){ZjUacXTUYU = false;}
      if(TrqxTAjkbz == true){TrqxTAjkbz = false;}
      if(BNCSNkNomA == true){BNCSNkNomA = false;}
      if(xrGNXJKflu == true){xrGNXJKflu = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BMXAQQPKRG
{ 
  void UcHRakoLbR()
  { 
      bool nuTqopAyXM = false;
      bool OIyIuAhbfu = false;
      bool pTstkSxIgp = false;
      bool lakTocFVmb = false;
      bool xMrGtKAtIs = false;
      bool CLNtrUCZFW = false;
      bool rqRhZmXeSS = false;
      bool tLnEycbmza = false;
      bool RDznnZNqbU = false;
      bool ZfQQiGuKfH = false;
      bool mVapypXIuO = false;
      bool sOZeFoqcsz = false;
      bool YgCxrOuWVq = false;
      bool gQGrmubJpx = false;
      bool CEuMbKFkLt = false;
      bool PRQghGSdyi = false;
      bool uAmEgbIgqJ = false;
      bool MhmuxTkpVg = false;
      bool IJLabPAZKg = false;
      bool byKGCBRqeA = false;
      string BuoktqRlKw;
      string aEBeaFUWkk;
      string QHWNzolyca;
      string EbrCSyAFIc;
      string yHCzDJrzSF;
      string lSwsOsXalE;
      string ETIiryMyLc;
      string FPNHmJFWJp;
      string wmeXokfynY;
      string ZYSDXeyTGE;
      string xtmpAVFnGE;
      string nAQwHRnksH;
      string SNKmDfwJiI;
      string KlPErCzoXS;
      string frrikYHWwG;
      string pNZzljiAlT;
      string ZuEysCjLDE;
      string RzMaaZUaBd;
      string YYNcRxLQSI;
      string TzCEsjnsph;
      if(BuoktqRlKw == xtmpAVFnGE){nuTqopAyXM = true;}
      else if(xtmpAVFnGE == BuoktqRlKw){mVapypXIuO = true;}
      if(aEBeaFUWkk == nAQwHRnksH){OIyIuAhbfu = true;}
      else if(nAQwHRnksH == aEBeaFUWkk){sOZeFoqcsz = true;}
      if(QHWNzolyca == SNKmDfwJiI){pTstkSxIgp = true;}
      else if(SNKmDfwJiI == QHWNzolyca){YgCxrOuWVq = true;}
      if(EbrCSyAFIc == KlPErCzoXS){lakTocFVmb = true;}
      else if(KlPErCzoXS == EbrCSyAFIc){gQGrmubJpx = true;}
      if(yHCzDJrzSF == frrikYHWwG){xMrGtKAtIs = true;}
      else if(frrikYHWwG == yHCzDJrzSF){CEuMbKFkLt = true;}
      if(lSwsOsXalE == pNZzljiAlT){CLNtrUCZFW = true;}
      else if(pNZzljiAlT == lSwsOsXalE){PRQghGSdyi = true;}
      if(ETIiryMyLc == ZuEysCjLDE){rqRhZmXeSS = true;}
      else if(ZuEysCjLDE == ETIiryMyLc){uAmEgbIgqJ = true;}
      if(FPNHmJFWJp == RzMaaZUaBd){tLnEycbmza = true;}
      if(wmeXokfynY == YYNcRxLQSI){RDznnZNqbU = true;}
      if(ZYSDXeyTGE == TzCEsjnsph){ZfQQiGuKfH = true;}
      while(RzMaaZUaBd == FPNHmJFWJp){MhmuxTkpVg = true;}
      while(YYNcRxLQSI == YYNcRxLQSI){IJLabPAZKg = true;}
      while(TzCEsjnsph == TzCEsjnsph){byKGCBRqeA = true;}
      if(nuTqopAyXM == true){nuTqopAyXM = false;}
      if(OIyIuAhbfu == true){OIyIuAhbfu = false;}
      if(pTstkSxIgp == true){pTstkSxIgp = false;}
      if(lakTocFVmb == true){lakTocFVmb = false;}
      if(xMrGtKAtIs == true){xMrGtKAtIs = false;}
      if(CLNtrUCZFW == true){CLNtrUCZFW = false;}
      if(rqRhZmXeSS == true){rqRhZmXeSS = false;}
      if(tLnEycbmza == true){tLnEycbmza = false;}
      if(RDznnZNqbU == true){RDznnZNqbU = false;}
      if(ZfQQiGuKfH == true){ZfQQiGuKfH = false;}
      if(mVapypXIuO == true){mVapypXIuO = false;}
      if(sOZeFoqcsz == true){sOZeFoqcsz = false;}
      if(YgCxrOuWVq == true){YgCxrOuWVq = false;}
      if(gQGrmubJpx == true){gQGrmubJpx = false;}
      if(CEuMbKFkLt == true){CEuMbKFkLt = false;}
      if(PRQghGSdyi == true){PRQghGSdyi = false;}
      if(uAmEgbIgqJ == true){uAmEgbIgqJ = false;}
      if(MhmuxTkpVg == true){MhmuxTkpVg = false;}
      if(IJLabPAZKg == true){IJLabPAZKg = false;}
      if(byKGCBRqeA == true){byKGCBRqeA = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CCJQJZWBSM
{ 
  void zheAJOHRrr()
  { 
      bool cpIUsKIQBD = false;
      bool NBHDxOSxpI = false;
      bool iBqCEVatWW = false;
      bool isyIiWrEou = false;
      bool FBQDyBBETP = false;
      bool AdjKHbzGQW = false;
      bool jNteTUCzTa = false;
      bool VNHuCgknDD = false;
      bool UVaGVEInBH = false;
      bool XlnSKrHkid = false;
      bool yCkstlWoIZ = false;
      bool pLCNCOmmIP = false;
      bool MOxLCjYojK = false;
      bool ZzEYESjgMi = false;
      bool hSeaQMiaPw = false;
      bool FTbTRiEZpJ = false;
      bool NgFxzqHcIi = false;
      bool dFaZhsiVtp = false;
      bool AyYmEGWkHV = false;
      bool OKpCVANhbZ = false;
      string chaVHpSCBM;
      string gRDmsrJnFH;
      string mpBDufgLTX;
      string GPKyQHfGgc;
      string HsqzAdhTMa;
      string pARtAlfWZb;
      string IGhcGRGFVl;
      string LDGcztzCDR;
      string yNzrgFxQxe;
      string aIfFTAKNrh;
      string WbxaOmtotG;
      string mdTIcuHpuB;
      string TpIqKDXBIy;
      string ikJUtVPYct;
      string LIVprxoWbY;
      string sUSqOgTGQL;
      string PrguWBTyOG;
      string eiMBoKTRyM;
      string NAZiFOSSGM;
      string CIIReSbaTp;
      if(chaVHpSCBM == WbxaOmtotG){cpIUsKIQBD = true;}
      else if(WbxaOmtotG == chaVHpSCBM){yCkstlWoIZ = true;}
      if(gRDmsrJnFH == mdTIcuHpuB){NBHDxOSxpI = true;}
      else if(mdTIcuHpuB == gRDmsrJnFH){pLCNCOmmIP = true;}
      if(mpBDufgLTX == TpIqKDXBIy){iBqCEVatWW = true;}
      else if(TpIqKDXBIy == mpBDufgLTX){MOxLCjYojK = true;}
      if(GPKyQHfGgc == ikJUtVPYct){isyIiWrEou = true;}
      else if(ikJUtVPYct == GPKyQHfGgc){ZzEYESjgMi = true;}
      if(HsqzAdhTMa == LIVprxoWbY){FBQDyBBETP = true;}
      else if(LIVprxoWbY == HsqzAdhTMa){hSeaQMiaPw = true;}
      if(pARtAlfWZb == sUSqOgTGQL){AdjKHbzGQW = true;}
      else if(sUSqOgTGQL == pARtAlfWZb){FTbTRiEZpJ = true;}
      if(IGhcGRGFVl == PrguWBTyOG){jNteTUCzTa = true;}
      else if(PrguWBTyOG == IGhcGRGFVl){NgFxzqHcIi = true;}
      if(LDGcztzCDR == eiMBoKTRyM){VNHuCgknDD = true;}
      if(yNzrgFxQxe == NAZiFOSSGM){UVaGVEInBH = true;}
      if(aIfFTAKNrh == CIIReSbaTp){XlnSKrHkid = true;}
      while(eiMBoKTRyM == LDGcztzCDR){dFaZhsiVtp = true;}
      while(NAZiFOSSGM == NAZiFOSSGM){AyYmEGWkHV = true;}
      while(CIIReSbaTp == CIIReSbaTp){OKpCVANhbZ = true;}
      if(cpIUsKIQBD == true){cpIUsKIQBD = false;}
      if(NBHDxOSxpI == true){NBHDxOSxpI = false;}
      if(iBqCEVatWW == true){iBqCEVatWW = false;}
      if(isyIiWrEou == true){isyIiWrEou = false;}
      if(FBQDyBBETP == true){FBQDyBBETP = false;}
      if(AdjKHbzGQW == true){AdjKHbzGQW = false;}
      if(jNteTUCzTa == true){jNteTUCzTa = false;}
      if(VNHuCgknDD == true){VNHuCgknDD = false;}
      if(UVaGVEInBH == true){UVaGVEInBH = false;}
      if(XlnSKrHkid == true){XlnSKrHkid = false;}
      if(yCkstlWoIZ == true){yCkstlWoIZ = false;}
      if(pLCNCOmmIP == true){pLCNCOmmIP = false;}
      if(MOxLCjYojK == true){MOxLCjYojK = false;}
      if(ZzEYESjgMi == true){ZzEYESjgMi = false;}
      if(hSeaQMiaPw == true){hSeaQMiaPw = false;}
      if(FTbTRiEZpJ == true){FTbTRiEZpJ = false;}
      if(NgFxzqHcIi == true){NgFxzqHcIi = false;}
      if(dFaZhsiVtp == true){dFaZhsiVtp = false;}
      if(AyYmEGWkHV == true){AyYmEGWkHV = false;}
      if(OKpCVANhbZ == true){OKpCVANhbZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZGFXONWMUS
{ 
  void SIpNVuhfpn()
  { 
      bool jBsVgwSGfB = false;
      bool JqbqciTJzC = false;
      bool jTQJfyZLqm = false;
      bool AZjMDnMDEN = false;
      bool YNBibrqtZF = false;
      bool GLNwkQjjpi = false;
      bool WHPrBiHPno = false;
      bool WPElcVRhhm = false;
      bool GelkfnMnal = false;
      bool DcNVpRyIgr = false;
      bool jbGjipGaSP = false;
      bool zCBCcrVdMM = false;
      bool WSoBltCiwL = false;
      bool XIAoDeXBCo = false;
      bool hlULmBaKsx = false;
      bool uUwUVQlEyo = false;
      bool mXdGqqBMzy = false;
      bool UTaWekydOj = false;
      bool SrpyPDLfSl = false;
      bool rSCNBJHOTu = false;
      string EYYjtFMEAS;
      string uwPBghJXZp;
      string XkrDMGLNpW;
      string GMiZGsfrzq;
      string kfdnoUsNqK;
      string EePhLTfVWn;
      string tzOdLhMpum;
      string BRXhFqHDNq;
      string NxVeRbxBNl;
      string NKULwlRzcG;
      string IDSEJlGgcT;
      string MbHeZxcjLi;
      string kLbIYqwLlS;
      string ZHaGiKeRUe;
      string VPXsZkCplz;
      string IhmsVxaKeR;
      string ptuKtEABwP;
      string RLOgNUmihn;
      string oGZlKrUgjl;
      string jTlDoaUNyV;
      if(EYYjtFMEAS == IDSEJlGgcT){jBsVgwSGfB = true;}
      else if(IDSEJlGgcT == EYYjtFMEAS){jbGjipGaSP = true;}
      if(uwPBghJXZp == MbHeZxcjLi){JqbqciTJzC = true;}
      else if(MbHeZxcjLi == uwPBghJXZp){zCBCcrVdMM = true;}
      if(XkrDMGLNpW == kLbIYqwLlS){jTQJfyZLqm = true;}
      else if(kLbIYqwLlS == XkrDMGLNpW){WSoBltCiwL = true;}
      if(GMiZGsfrzq == ZHaGiKeRUe){AZjMDnMDEN = true;}
      else if(ZHaGiKeRUe == GMiZGsfrzq){XIAoDeXBCo = true;}
      if(kfdnoUsNqK == VPXsZkCplz){YNBibrqtZF = true;}
      else if(VPXsZkCplz == kfdnoUsNqK){hlULmBaKsx = true;}
      if(EePhLTfVWn == IhmsVxaKeR){GLNwkQjjpi = true;}
      else if(IhmsVxaKeR == EePhLTfVWn){uUwUVQlEyo = true;}
      if(tzOdLhMpum == ptuKtEABwP){WHPrBiHPno = true;}
      else if(ptuKtEABwP == tzOdLhMpum){mXdGqqBMzy = true;}
      if(BRXhFqHDNq == RLOgNUmihn){WPElcVRhhm = true;}
      if(NxVeRbxBNl == oGZlKrUgjl){GelkfnMnal = true;}
      if(NKULwlRzcG == jTlDoaUNyV){DcNVpRyIgr = true;}
      while(RLOgNUmihn == BRXhFqHDNq){UTaWekydOj = true;}
      while(oGZlKrUgjl == oGZlKrUgjl){SrpyPDLfSl = true;}
      while(jTlDoaUNyV == jTlDoaUNyV){rSCNBJHOTu = true;}
      if(jBsVgwSGfB == true){jBsVgwSGfB = false;}
      if(JqbqciTJzC == true){JqbqciTJzC = false;}
      if(jTQJfyZLqm == true){jTQJfyZLqm = false;}
      if(AZjMDnMDEN == true){AZjMDnMDEN = false;}
      if(YNBibrqtZF == true){YNBibrqtZF = false;}
      if(GLNwkQjjpi == true){GLNwkQjjpi = false;}
      if(WHPrBiHPno == true){WHPrBiHPno = false;}
      if(WPElcVRhhm == true){WPElcVRhhm = false;}
      if(GelkfnMnal == true){GelkfnMnal = false;}
      if(DcNVpRyIgr == true){DcNVpRyIgr = false;}
      if(jbGjipGaSP == true){jbGjipGaSP = false;}
      if(zCBCcrVdMM == true){zCBCcrVdMM = false;}
      if(WSoBltCiwL == true){WSoBltCiwL = false;}
      if(XIAoDeXBCo == true){XIAoDeXBCo = false;}
      if(hlULmBaKsx == true){hlULmBaKsx = false;}
      if(uUwUVQlEyo == true){uUwUVQlEyo = false;}
      if(mXdGqqBMzy == true){mXdGqqBMzy = false;}
      if(UTaWekydOj == true){UTaWekydOj = false;}
      if(SrpyPDLfSl == true){SrpyPDLfSl = false;}
      if(rSCNBJHOTu == true){rSCNBJHOTu = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HQYSFSCBCV
{ 
  void QfRaRyAoob()
  { 
      bool tKorAtfJiz = false;
      bool uixZrBbKtm = false;
      bool HsZjLXHiMW = false;
      bool lKMSuzJhHz = false;
      bool layEalJgbb = false;
      bool ffBKsBjEgc = false;
      bool mwwLMjVdes = false;
      bool rsCuBftUJY = false;
      bool NZHjqFIBYs = false;
      bool PEBeMpemqp = false;
      bool PeCijmdGJN = false;
      bool HOuHOMnMTH = false;
      bool aUFkdmqWhK = false;
      bool BGcTJhXYzD = false;
      bool bCqLlAJtQz = false;
      bool BRSjOuZREw = false;
      bool uGXyLBkbyf = false;
      bool oPMQeYdszc = false;
      bool dxGhJjmsEN = false;
      bool HGTDHUerAj = false;
      string zeWWZeUhds;
      string xfKrQHoKLh;
      string dQIPbHXgdl;
      string nsOiFNHUnp;
      string jPMITKjSfD;
      string NZbnWhVHuS;
      string HnDrEDRalD;
      string ATVVyOEzwQ;
      string WEAaXOQZMX;
      string qAZVhuMTEP;
      string sIfcDtDGpI;
      string cVKduCmbTp;
      string mqOOLztAmC;
      string CjugXOXXXd;
      string KJjEtUouZe;
      string IjzNbCsRQD;
      string ACSnFrZdGg;
      string eOEpecGKUp;
      string KyaIsRkgEV;
      string UGHUdHqwyB;
      if(zeWWZeUhds == sIfcDtDGpI){tKorAtfJiz = true;}
      else if(sIfcDtDGpI == zeWWZeUhds){PeCijmdGJN = true;}
      if(xfKrQHoKLh == cVKduCmbTp){uixZrBbKtm = true;}
      else if(cVKduCmbTp == xfKrQHoKLh){HOuHOMnMTH = true;}
      if(dQIPbHXgdl == mqOOLztAmC){HsZjLXHiMW = true;}
      else if(mqOOLztAmC == dQIPbHXgdl){aUFkdmqWhK = true;}
      if(nsOiFNHUnp == CjugXOXXXd){lKMSuzJhHz = true;}
      else if(CjugXOXXXd == nsOiFNHUnp){BGcTJhXYzD = true;}
      if(jPMITKjSfD == KJjEtUouZe){layEalJgbb = true;}
      else if(KJjEtUouZe == jPMITKjSfD){bCqLlAJtQz = true;}
      if(NZbnWhVHuS == IjzNbCsRQD){ffBKsBjEgc = true;}
      else if(IjzNbCsRQD == NZbnWhVHuS){BRSjOuZREw = true;}
      if(HnDrEDRalD == ACSnFrZdGg){mwwLMjVdes = true;}
      else if(ACSnFrZdGg == HnDrEDRalD){uGXyLBkbyf = true;}
      if(ATVVyOEzwQ == eOEpecGKUp){rsCuBftUJY = true;}
      if(WEAaXOQZMX == KyaIsRkgEV){NZHjqFIBYs = true;}
      if(qAZVhuMTEP == UGHUdHqwyB){PEBeMpemqp = true;}
      while(eOEpecGKUp == ATVVyOEzwQ){oPMQeYdszc = true;}
      while(KyaIsRkgEV == KyaIsRkgEV){dxGhJjmsEN = true;}
      while(UGHUdHqwyB == UGHUdHqwyB){HGTDHUerAj = true;}
      if(tKorAtfJiz == true){tKorAtfJiz = false;}
      if(uixZrBbKtm == true){uixZrBbKtm = false;}
      if(HsZjLXHiMW == true){HsZjLXHiMW = false;}
      if(lKMSuzJhHz == true){lKMSuzJhHz = false;}
      if(layEalJgbb == true){layEalJgbb = false;}
      if(ffBKsBjEgc == true){ffBKsBjEgc = false;}
      if(mwwLMjVdes == true){mwwLMjVdes = false;}
      if(rsCuBftUJY == true){rsCuBftUJY = false;}
      if(NZHjqFIBYs == true){NZHjqFIBYs = false;}
      if(PEBeMpemqp == true){PEBeMpemqp = false;}
      if(PeCijmdGJN == true){PeCijmdGJN = false;}
      if(HOuHOMnMTH == true){HOuHOMnMTH = false;}
      if(aUFkdmqWhK == true){aUFkdmqWhK = false;}
      if(BGcTJhXYzD == true){BGcTJhXYzD = false;}
      if(bCqLlAJtQz == true){bCqLlAJtQz = false;}
      if(BRSjOuZREw == true){BRSjOuZREw = false;}
      if(uGXyLBkbyf == true){uGXyLBkbyf = false;}
      if(oPMQeYdszc == true){oPMQeYdszc = false;}
      if(dxGhJjmsEN == true){dxGhJjmsEN = false;}
      if(HGTDHUerAj == true){HGTDHUerAj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JXDMAISYGX
{ 
  void RZlBiukqGf()
  { 
      bool HzoQiGBdtb = false;
      bool BmcePzUFAz = false;
      bool sCqhnXmLhT = false;
      bool oxWgFWtAWm = false;
      bool dmcJuYirRD = false;
      bool tWyysFIGbK = false;
      bool SpwalEQuZs = false;
      bool ETiNVGYgmP = false;
      bool outdYSzXnA = false;
      bool OmXMTnqCWq = false;
      bool HmRYyZmjcm = false;
      bool fbTMUXwSBT = false;
      bool FrHZeRuWtg = false;
      bool PWJwHBcVey = false;
      bool ePtBTlSLqw = false;
      bool rXADlPfrtg = false;
      bool eQouXJSXES = false;
      bool tamDopxGFr = false;
      bool yieyoqWFKr = false;
      bool VkbxTNZzdK = false;
      string OdkZRYXCMa;
      string UwPQEELGVX;
      string QBmBNUDtfQ;
      string RsHdFAUJji;
      string VfhWYdscqX;
      string psWIrMqjMx;
      string UcWwJbokLD;
      string jCWmCkTPbj;
      string baCIZYWylr;
      string JjxmNrJlNx;
      string nHQmmnWjFH;
      string saHbUtaOWk;
      string EpzTtzFjYK;
      string agBewTJPUl;
      string BmsdzhcURT;
      string RuPcsyIxrm;
      string ilKtfLZcfA;
      string jKskiQwUTM;
      string WjRFsfRXlx;
      string LMWhCPbzym;
      if(OdkZRYXCMa == nHQmmnWjFH){HzoQiGBdtb = true;}
      else if(nHQmmnWjFH == OdkZRYXCMa){HmRYyZmjcm = true;}
      if(UwPQEELGVX == saHbUtaOWk){BmcePzUFAz = true;}
      else if(saHbUtaOWk == UwPQEELGVX){fbTMUXwSBT = true;}
      if(QBmBNUDtfQ == EpzTtzFjYK){sCqhnXmLhT = true;}
      else if(EpzTtzFjYK == QBmBNUDtfQ){FrHZeRuWtg = true;}
      if(RsHdFAUJji == agBewTJPUl){oxWgFWtAWm = true;}
      else if(agBewTJPUl == RsHdFAUJji){PWJwHBcVey = true;}
      if(VfhWYdscqX == BmsdzhcURT){dmcJuYirRD = true;}
      else if(BmsdzhcURT == VfhWYdscqX){ePtBTlSLqw = true;}
      if(psWIrMqjMx == RuPcsyIxrm){tWyysFIGbK = true;}
      else if(RuPcsyIxrm == psWIrMqjMx){rXADlPfrtg = true;}
      if(UcWwJbokLD == ilKtfLZcfA){SpwalEQuZs = true;}
      else if(ilKtfLZcfA == UcWwJbokLD){eQouXJSXES = true;}
      if(jCWmCkTPbj == jKskiQwUTM){ETiNVGYgmP = true;}
      if(baCIZYWylr == WjRFsfRXlx){outdYSzXnA = true;}
      if(JjxmNrJlNx == LMWhCPbzym){OmXMTnqCWq = true;}
      while(jKskiQwUTM == jCWmCkTPbj){tamDopxGFr = true;}
      while(WjRFsfRXlx == WjRFsfRXlx){yieyoqWFKr = true;}
      while(LMWhCPbzym == LMWhCPbzym){VkbxTNZzdK = true;}
      if(HzoQiGBdtb == true){HzoQiGBdtb = false;}
      if(BmcePzUFAz == true){BmcePzUFAz = false;}
      if(sCqhnXmLhT == true){sCqhnXmLhT = false;}
      if(oxWgFWtAWm == true){oxWgFWtAWm = false;}
      if(dmcJuYirRD == true){dmcJuYirRD = false;}
      if(tWyysFIGbK == true){tWyysFIGbK = false;}
      if(SpwalEQuZs == true){SpwalEQuZs = false;}
      if(ETiNVGYgmP == true){ETiNVGYgmP = false;}
      if(outdYSzXnA == true){outdYSzXnA = false;}
      if(OmXMTnqCWq == true){OmXMTnqCWq = false;}
      if(HmRYyZmjcm == true){HmRYyZmjcm = false;}
      if(fbTMUXwSBT == true){fbTMUXwSBT = false;}
      if(FrHZeRuWtg == true){FrHZeRuWtg = false;}
      if(PWJwHBcVey == true){PWJwHBcVey = false;}
      if(ePtBTlSLqw == true){ePtBTlSLqw = false;}
      if(rXADlPfrtg == true){rXADlPfrtg = false;}
      if(eQouXJSXES == true){eQouXJSXES = false;}
      if(tamDopxGFr == true){tamDopxGFr = false;}
      if(yieyoqWFKr == true){yieyoqWFKr = false;}
      if(VkbxTNZzdK == true){VkbxTNZzdK = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SZJZJPCGUZ
{ 
  void eJSAQETLJg()
  { 
      bool TDyULYRULD = false;
      bool FthrRkDQxW = false;
      bool aCBKwZyHHe = false;
      bool hlQDIXpRnN = false;
      bool kfMnglVZLB = false;
      bool udFCTRCmXl = false;
      bool aJNfUScejt = false;
      bool tlbjktpMdJ = false;
      bool DYwGiEfOip = false;
      bool VDsBmaSfSN = false;
      bool JpoRGJBVQY = false;
      bool aoJNhBNHwQ = false;
      bool lNusYqrZxK = false;
      bool bKsPANiezZ = false;
      bool DKyKPfsfSk = false;
      bool NLiCautwrO = false;
      bool QaXGwQusMU = false;
      bool ShHWNbPkcr = false;
      bool xMGLxPaqnF = false;
      bool SlljIblqNy = false;
      string EkTyXRLySE;
      string UOAWGKYJxw;
      string fdVLLkSdyq;
      string TRifnzWNfC;
      string aMuuVEthzH;
      string UDAhAZOHll;
      string yhzajDeThH;
      string nLctVTiZpH;
      string qjdpVPQBeN;
      string jOasDsMTLR;
      string YZArSJZGMg;
      string qoReuNDIFU;
      string ldoTWYjjpb;
      string GOOXhRjlWL;
      string oKnyWuxuyp;
      string oDNpfukfUs;
      string ZpQjZbasxc;
      string dQksebppxb;
      string bnlAlDAXrl;
      string gERdlztDYt;
      if(EkTyXRLySE == YZArSJZGMg){TDyULYRULD = true;}
      else if(YZArSJZGMg == EkTyXRLySE){JpoRGJBVQY = true;}
      if(UOAWGKYJxw == qoReuNDIFU){FthrRkDQxW = true;}
      else if(qoReuNDIFU == UOAWGKYJxw){aoJNhBNHwQ = true;}
      if(fdVLLkSdyq == ldoTWYjjpb){aCBKwZyHHe = true;}
      else if(ldoTWYjjpb == fdVLLkSdyq){lNusYqrZxK = true;}
      if(TRifnzWNfC == GOOXhRjlWL){hlQDIXpRnN = true;}
      else if(GOOXhRjlWL == TRifnzWNfC){bKsPANiezZ = true;}
      if(aMuuVEthzH == oKnyWuxuyp){kfMnglVZLB = true;}
      else if(oKnyWuxuyp == aMuuVEthzH){DKyKPfsfSk = true;}
      if(UDAhAZOHll == oDNpfukfUs){udFCTRCmXl = true;}
      else if(oDNpfukfUs == UDAhAZOHll){NLiCautwrO = true;}
      if(yhzajDeThH == ZpQjZbasxc){aJNfUScejt = true;}
      else if(ZpQjZbasxc == yhzajDeThH){QaXGwQusMU = true;}
      if(nLctVTiZpH == dQksebppxb){tlbjktpMdJ = true;}
      if(qjdpVPQBeN == bnlAlDAXrl){DYwGiEfOip = true;}
      if(jOasDsMTLR == gERdlztDYt){VDsBmaSfSN = true;}
      while(dQksebppxb == nLctVTiZpH){ShHWNbPkcr = true;}
      while(bnlAlDAXrl == bnlAlDAXrl){xMGLxPaqnF = true;}
      while(gERdlztDYt == gERdlztDYt){SlljIblqNy = true;}
      if(TDyULYRULD == true){TDyULYRULD = false;}
      if(FthrRkDQxW == true){FthrRkDQxW = false;}
      if(aCBKwZyHHe == true){aCBKwZyHHe = false;}
      if(hlQDIXpRnN == true){hlQDIXpRnN = false;}
      if(kfMnglVZLB == true){kfMnglVZLB = false;}
      if(udFCTRCmXl == true){udFCTRCmXl = false;}
      if(aJNfUScejt == true){aJNfUScejt = false;}
      if(tlbjktpMdJ == true){tlbjktpMdJ = false;}
      if(DYwGiEfOip == true){DYwGiEfOip = false;}
      if(VDsBmaSfSN == true){VDsBmaSfSN = false;}
      if(JpoRGJBVQY == true){JpoRGJBVQY = false;}
      if(aoJNhBNHwQ == true){aoJNhBNHwQ = false;}
      if(lNusYqrZxK == true){lNusYqrZxK = false;}
      if(bKsPANiezZ == true){bKsPANiezZ = false;}
      if(DKyKPfsfSk == true){DKyKPfsfSk = false;}
      if(NLiCautwrO == true){NLiCautwrO = false;}
      if(QaXGwQusMU == true){QaXGwQusMU = false;}
      if(ShHWNbPkcr == true){ShHWNbPkcr = false;}
      if(xMGLxPaqnF == true){xMGLxPaqnF = false;}
      if(SlljIblqNy == true){SlljIblqNy = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZXCTSAHKYW
{ 
  void KJmSQibIIu()
  { 
      bool liYBQycCCj = false;
      bool ypnnBsmYda = false;
      bool otlwwIeDzZ = false;
      bool TkqhuONfcC = false;
      bool cyrMknlyAO = false;
      bool SrVNzrxbEI = false;
      bool SZCbQNyFrl = false;
      bool fGuaxqBYLI = false;
      bool WgDkGaOchD = false;
      bool RlbTiDuPwW = false;
      bool ofGNiMwuPI = false;
      bool qXPuklVRUo = false;
      bool yVXoUtebrt = false;
      bool ukcnTpapCt = false;
      bool dTMZLtVfhw = false;
      bool gdtHYTIQso = false;
      bool gUDVMFuabu = false;
      bool LfPaKeIyca = false;
      bool XysuMRpUPz = false;
      bool hJwNMJPqMG = false;
      string lPRhtVqqeI;
      string gYskpMWPzP;
      string qVZJMtBaSb;
      string wRgwykbaDm;
      string GMGDDrVBlB;
      string SqXFCQoHUz;
      string TPXDMOrxlF;
      string kwMMIeOVGU;
      string YAoRIFtbth;
      string uTFDSqoDqn;
      string AYOfQVdLwX;
      string BTBAaTaGtH;
      string TQCDJQqJsR;
      string oKeRTAkIAm;
      string aieWfbSEbz;
      string QiJiUjVLVz;
      string eURehqoifa;
      string hRFHgkkfXh;
      string ylgsBGBuxi;
      string dZlBHGQAoo;
      if(lPRhtVqqeI == AYOfQVdLwX){liYBQycCCj = true;}
      else if(AYOfQVdLwX == lPRhtVqqeI){ofGNiMwuPI = true;}
      if(gYskpMWPzP == BTBAaTaGtH){ypnnBsmYda = true;}
      else if(BTBAaTaGtH == gYskpMWPzP){qXPuklVRUo = true;}
      if(qVZJMtBaSb == TQCDJQqJsR){otlwwIeDzZ = true;}
      else if(TQCDJQqJsR == qVZJMtBaSb){yVXoUtebrt = true;}
      if(wRgwykbaDm == oKeRTAkIAm){TkqhuONfcC = true;}
      else if(oKeRTAkIAm == wRgwykbaDm){ukcnTpapCt = true;}
      if(GMGDDrVBlB == aieWfbSEbz){cyrMknlyAO = true;}
      else if(aieWfbSEbz == GMGDDrVBlB){dTMZLtVfhw = true;}
      if(SqXFCQoHUz == QiJiUjVLVz){SrVNzrxbEI = true;}
      else if(QiJiUjVLVz == SqXFCQoHUz){gdtHYTIQso = true;}
      if(TPXDMOrxlF == eURehqoifa){SZCbQNyFrl = true;}
      else if(eURehqoifa == TPXDMOrxlF){gUDVMFuabu = true;}
      if(kwMMIeOVGU == hRFHgkkfXh){fGuaxqBYLI = true;}
      if(YAoRIFtbth == ylgsBGBuxi){WgDkGaOchD = true;}
      if(uTFDSqoDqn == dZlBHGQAoo){RlbTiDuPwW = true;}
      while(hRFHgkkfXh == kwMMIeOVGU){LfPaKeIyca = true;}
      while(ylgsBGBuxi == ylgsBGBuxi){XysuMRpUPz = true;}
      while(dZlBHGQAoo == dZlBHGQAoo){hJwNMJPqMG = true;}
      if(liYBQycCCj == true){liYBQycCCj = false;}
      if(ypnnBsmYda == true){ypnnBsmYda = false;}
      if(otlwwIeDzZ == true){otlwwIeDzZ = false;}
      if(TkqhuONfcC == true){TkqhuONfcC = false;}
      if(cyrMknlyAO == true){cyrMknlyAO = false;}
      if(SrVNzrxbEI == true){SrVNzrxbEI = false;}
      if(SZCbQNyFrl == true){SZCbQNyFrl = false;}
      if(fGuaxqBYLI == true){fGuaxqBYLI = false;}
      if(WgDkGaOchD == true){WgDkGaOchD = false;}
      if(RlbTiDuPwW == true){RlbTiDuPwW = false;}
      if(ofGNiMwuPI == true){ofGNiMwuPI = false;}
      if(qXPuklVRUo == true){qXPuklVRUo = false;}
      if(yVXoUtebrt == true){yVXoUtebrt = false;}
      if(ukcnTpapCt == true){ukcnTpapCt = false;}
      if(dTMZLtVfhw == true){dTMZLtVfhw = false;}
      if(gdtHYTIQso == true){gdtHYTIQso = false;}
      if(gUDVMFuabu == true){gUDVMFuabu = false;}
      if(LfPaKeIyca == true){LfPaKeIyca = false;}
      if(XysuMRpUPz == true){XysuMRpUPz = false;}
      if(hJwNMJPqMG == true){hJwNMJPqMG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JZWLBPYZTL
{ 
  void WHpIxVQBiQ()
  { 
      bool NMabPRmipk = false;
      bool CezcytEtgo = false;
      bool eNCDPjLlYf = false;
      bool ZXNDoEAIJZ = false;
      bool EGFgrxYhsM = false;
      bool QGolpAKXMe = false;
      bool FAJKKeRwns = false;
      bool tukGjXHVKR = false;
      bool KdrMPThaNn = false;
      bool tbudUDxuVN = false;
      bool ecszSVUhBR = false;
      bool rnNuTaIwuq = false;
      bool wqhzzRAGlg = false;
      bool aMAwmgkrtt = false;
      bool yAZuwGOAJs = false;
      bool uAAeRntsAA = false;
      bool UNuUqenQHl = false;
      bool pPCAruQmDr = false;
      bool CzxskaLUey = false;
      bool mqqsYkFFEQ = false;
      string MFJdVuSkzi;
      string QlwwNrBbDA;
      string lKOZJUDFVo;
      string RisDRONjLY;
      string PVhVoeIRGu;
      string oLwqFCqiTB;
      string rMOJRhrsey;
      string eQQejPcrkd;
      string MkyZAFlCxP;
      string WePKfCGXEH;
      string lPmHQXuRGp;
      string ZyxpoHNoIL;
      string upuObzBGyg;
      string JEncqwxqJc;
      string KUwHpXgZUY;
      string oROrLwTYfQ;
      string VoTZfUKTyr;
      string JWefBatnYD;
      string RbkYrqpwRu;
      string cYKrGUAVaU;
      if(MFJdVuSkzi == lPmHQXuRGp){NMabPRmipk = true;}
      else if(lPmHQXuRGp == MFJdVuSkzi){ecszSVUhBR = true;}
      if(QlwwNrBbDA == ZyxpoHNoIL){CezcytEtgo = true;}
      else if(ZyxpoHNoIL == QlwwNrBbDA){rnNuTaIwuq = true;}
      if(lKOZJUDFVo == upuObzBGyg){eNCDPjLlYf = true;}
      else if(upuObzBGyg == lKOZJUDFVo){wqhzzRAGlg = true;}
      if(RisDRONjLY == JEncqwxqJc){ZXNDoEAIJZ = true;}
      else if(JEncqwxqJc == RisDRONjLY){aMAwmgkrtt = true;}
      if(PVhVoeIRGu == KUwHpXgZUY){EGFgrxYhsM = true;}
      else if(KUwHpXgZUY == PVhVoeIRGu){yAZuwGOAJs = true;}
      if(oLwqFCqiTB == oROrLwTYfQ){QGolpAKXMe = true;}
      else if(oROrLwTYfQ == oLwqFCqiTB){uAAeRntsAA = true;}
      if(rMOJRhrsey == VoTZfUKTyr){FAJKKeRwns = true;}
      else if(VoTZfUKTyr == rMOJRhrsey){UNuUqenQHl = true;}
      if(eQQejPcrkd == JWefBatnYD){tukGjXHVKR = true;}
      if(MkyZAFlCxP == RbkYrqpwRu){KdrMPThaNn = true;}
      if(WePKfCGXEH == cYKrGUAVaU){tbudUDxuVN = true;}
      while(JWefBatnYD == eQQejPcrkd){pPCAruQmDr = true;}
      while(RbkYrqpwRu == RbkYrqpwRu){CzxskaLUey = true;}
      while(cYKrGUAVaU == cYKrGUAVaU){mqqsYkFFEQ = true;}
      if(NMabPRmipk == true){NMabPRmipk = false;}
      if(CezcytEtgo == true){CezcytEtgo = false;}
      if(eNCDPjLlYf == true){eNCDPjLlYf = false;}
      if(ZXNDoEAIJZ == true){ZXNDoEAIJZ = false;}
      if(EGFgrxYhsM == true){EGFgrxYhsM = false;}
      if(QGolpAKXMe == true){QGolpAKXMe = false;}
      if(FAJKKeRwns == true){FAJKKeRwns = false;}
      if(tukGjXHVKR == true){tukGjXHVKR = false;}
      if(KdrMPThaNn == true){KdrMPThaNn = false;}
      if(tbudUDxuVN == true){tbudUDxuVN = false;}
      if(ecszSVUhBR == true){ecszSVUhBR = false;}
      if(rnNuTaIwuq == true){rnNuTaIwuq = false;}
      if(wqhzzRAGlg == true){wqhzzRAGlg = false;}
      if(aMAwmgkrtt == true){aMAwmgkrtt = false;}
      if(yAZuwGOAJs == true){yAZuwGOAJs = false;}
      if(uAAeRntsAA == true){uAAeRntsAA = false;}
      if(UNuUqenQHl == true){UNuUqenQHl = false;}
      if(pPCAruQmDr == true){pPCAruQmDr = false;}
      if(CzxskaLUey == true){CzxskaLUey = false;}
      if(mqqsYkFFEQ == true){mqqsYkFFEQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RBJYMJTDKK
{ 
  void hcYEaWYiIz()
  { 
      bool JjmfYMQBRf = false;
      bool GQntrbEQYs = false;
      bool cLJSfSJoMH = false;
      bool IVjdLhUUZK = false;
      bool uJssLgtKTM = false;
      bool QSDAkxZUzF = false;
      bool CaoKWkgKhJ = false;
      bool HjpOyTSQGz = false;
      bool YzUAqpSisO = false;
      bool HzUUITUhVa = false;
      bool zicKFfboCR = false;
      bool eJOAfCGnSg = false;
      bool RyQTBHOVFd = false;
      bool yjtfzoWKuK = false;
      bool NdfbiykUnF = false;
      bool NfPoIBSIIx = false;
      bool RGiYStPhBq = false;
      bool fusISnSojD = false;
      bool VthAQSUFWy = false;
      bool WNCxsHtHJo = false;
      string lmMmFMqBrw;
      string TWLyIpLVFt;
      string KpCNaMKTGZ;
      string AnctAuBubq;
      string imwuwXrWfl;
      string hJBHIwkzMs;
      string CXtSuetQyN;
      string nPsNKXsGQC;
      string UmmUqqmVlE;
      string KChioLrdAR;
      string daYSxRPnwk;
      string CdhJXPjZaE;
      string rXesClgKXz;
      string eOGzYsNUXY;
      string azZABgTYKZ;
      string bSDuYFufPK;
      string kFPwFnWikd;
      string BOCBcsydqV;
      string SIAyXLLKZS;
      string yhEMnVjctN;
      if(lmMmFMqBrw == daYSxRPnwk){JjmfYMQBRf = true;}
      else if(daYSxRPnwk == lmMmFMqBrw){zicKFfboCR = true;}
      if(TWLyIpLVFt == CdhJXPjZaE){GQntrbEQYs = true;}
      else if(CdhJXPjZaE == TWLyIpLVFt){eJOAfCGnSg = true;}
      if(KpCNaMKTGZ == rXesClgKXz){cLJSfSJoMH = true;}
      else if(rXesClgKXz == KpCNaMKTGZ){RyQTBHOVFd = true;}
      if(AnctAuBubq == eOGzYsNUXY){IVjdLhUUZK = true;}
      else if(eOGzYsNUXY == AnctAuBubq){yjtfzoWKuK = true;}
      if(imwuwXrWfl == azZABgTYKZ){uJssLgtKTM = true;}
      else if(azZABgTYKZ == imwuwXrWfl){NdfbiykUnF = true;}
      if(hJBHIwkzMs == bSDuYFufPK){QSDAkxZUzF = true;}
      else if(bSDuYFufPK == hJBHIwkzMs){NfPoIBSIIx = true;}
      if(CXtSuetQyN == kFPwFnWikd){CaoKWkgKhJ = true;}
      else if(kFPwFnWikd == CXtSuetQyN){RGiYStPhBq = true;}
      if(nPsNKXsGQC == BOCBcsydqV){HjpOyTSQGz = true;}
      if(UmmUqqmVlE == SIAyXLLKZS){YzUAqpSisO = true;}
      if(KChioLrdAR == yhEMnVjctN){HzUUITUhVa = true;}
      while(BOCBcsydqV == nPsNKXsGQC){fusISnSojD = true;}
      while(SIAyXLLKZS == SIAyXLLKZS){VthAQSUFWy = true;}
      while(yhEMnVjctN == yhEMnVjctN){WNCxsHtHJo = true;}
      if(JjmfYMQBRf == true){JjmfYMQBRf = false;}
      if(GQntrbEQYs == true){GQntrbEQYs = false;}
      if(cLJSfSJoMH == true){cLJSfSJoMH = false;}
      if(IVjdLhUUZK == true){IVjdLhUUZK = false;}
      if(uJssLgtKTM == true){uJssLgtKTM = false;}
      if(QSDAkxZUzF == true){QSDAkxZUzF = false;}
      if(CaoKWkgKhJ == true){CaoKWkgKhJ = false;}
      if(HjpOyTSQGz == true){HjpOyTSQGz = false;}
      if(YzUAqpSisO == true){YzUAqpSisO = false;}
      if(HzUUITUhVa == true){HzUUITUhVa = false;}
      if(zicKFfboCR == true){zicKFfboCR = false;}
      if(eJOAfCGnSg == true){eJOAfCGnSg = false;}
      if(RyQTBHOVFd == true){RyQTBHOVFd = false;}
      if(yjtfzoWKuK == true){yjtfzoWKuK = false;}
      if(NdfbiykUnF == true){NdfbiykUnF = false;}
      if(NfPoIBSIIx == true){NfPoIBSIIx = false;}
      if(RGiYStPhBq == true){RGiYStPhBq = false;}
      if(fusISnSojD == true){fusISnSojD = false;}
      if(VthAQSUFWy == true){VthAQSUFWy = false;}
      if(WNCxsHtHJo == true){WNCxsHtHJo = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CVKZQEVBLZ
{ 
  void pbhyslNLqp()
  { 
      bool kMEMAypPYL = false;
      bool xbfrCcIhzu = false;
      bool lDXNekzEMO = false;
      bool bfpxRNJeSi = false;
      bool IubxlWEHKM = false;
      bool DeMiogoqhO = false;
      bool ylCKiRkian = false;
      bool TcPQmqbZaH = false;
      bool grszKxTHHk = false;
      bool XRxDjVrQNU = false;
      bool lUgVsZPuuy = false;
      bool EPQZhGrDzl = false;
      bool xdaoQBfLgQ = false;
      bool yKLFVLKWFk = false;
      bool hXLJfIBXVd = false;
      bool coRbXItqFr = false;
      bool tDOezsDKDV = false;
      bool wUxOEYSRJH = false;
      bool zHoCmJsjhN = false;
      bool RSRMpdJgJl = false;
      string VHenTujkOS;
      string ghWHbqleLu;
      string iMNbRErHon;
      string gWqzJomwel;
      string VLdYORPAZB;
      string yiQkXJtDWO;
      string DcwakJpWxm;
      string DpzYYWhnNA;
      string oqhnpxGFaA;
      string tiYQisPDnE;
      string asCmxIjqMB;
      string bFpWZLuHkn;
      string DkuxrTQipe;
      string GdAJYwouph;
      string NOsZYosCpW;
      string yHSzAoxEtc;
      string qBBgtxEfxd;
      string VARDaorVYC;
      string aOrSgyLAIj;
      string ykAMpMMLea;
      if(VHenTujkOS == asCmxIjqMB){kMEMAypPYL = true;}
      else if(asCmxIjqMB == VHenTujkOS){lUgVsZPuuy = true;}
      if(ghWHbqleLu == bFpWZLuHkn){xbfrCcIhzu = true;}
      else if(bFpWZLuHkn == ghWHbqleLu){EPQZhGrDzl = true;}
      if(iMNbRErHon == DkuxrTQipe){lDXNekzEMO = true;}
      else if(DkuxrTQipe == iMNbRErHon){xdaoQBfLgQ = true;}
      if(gWqzJomwel == GdAJYwouph){bfpxRNJeSi = true;}
      else if(GdAJYwouph == gWqzJomwel){yKLFVLKWFk = true;}
      if(VLdYORPAZB == NOsZYosCpW){IubxlWEHKM = true;}
      else if(NOsZYosCpW == VLdYORPAZB){hXLJfIBXVd = true;}
      if(yiQkXJtDWO == yHSzAoxEtc){DeMiogoqhO = true;}
      else if(yHSzAoxEtc == yiQkXJtDWO){coRbXItqFr = true;}
      if(DcwakJpWxm == qBBgtxEfxd){ylCKiRkian = true;}
      else if(qBBgtxEfxd == DcwakJpWxm){tDOezsDKDV = true;}
      if(DpzYYWhnNA == VARDaorVYC){TcPQmqbZaH = true;}
      if(oqhnpxGFaA == aOrSgyLAIj){grszKxTHHk = true;}
      if(tiYQisPDnE == ykAMpMMLea){XRxDjVrQNU = true;}
      while(VARDaorVYC == DpzYYWhnNA){wUxOEYSRJH = true;}
      while(aOrSgyLAIj == aOrSgyLAIj){zHoCmJsjhN = true;}
      while(ykAMpMMLea == ykAMpMMLea){RSRMpdJgJl = true;}
      if(kMEMAypPYL == true){kMEMAypPYL = false;}
      if(xbfrCcIhzu == true){xbfrCcIhzu = false;}
      if(lDXNekzEMO == true){lDXNekzEMO = false;}
      if(bfpxRNJeSi == true){bfpxRNJeSi = false;}
      if(IubxlWEHKM == true){IubxlWEHKM = false;}
      if(DeMiogoqhO == true){DeMiogoqhO = false;}
      if(ylCKiRkian == true){ylCKiRkian = false;}
      if(TcPQmqbZaH == true){TcPQmqbZaH = false;}
      if(grszKxTHHk == true){grszKxTHHk = false;}
      if(XRxDjVrQNU == true){XRxDjVrQNU = false;}
      if(lUgVsZPuuy == true){lUgVsZPuuy = false;}
      if(EPQZhGrDzl == true){EPQZhGrDzl = false;}
      if(xdaoQBfLgQ == true){xdaoQBfLgQ = false;}
      if(yKLFVLKWFk == true){yKLFVLKWFk = false;}
      if(hXLJfIBXVd == true){hXLJfIBXVd = false;}
      if(coRbXItqFr == true){coRbXItqFr = false;}
      if(tDOezsDKDV == true){tDOezsDKDV = false;}
      if(wUxOEYSRJH == true){wUxOEYSRJH = false;}
      if(zHoCmJsjhN == true){zHoCmJsjhN = false;}
      if(RSRMpdJgJl == true){RSRMpdJgJl = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NAATJOCAWP
{ 
  void yjqbsCWXmc()
  { 
      bool ffsLKNPFMj = false;
      bool MggbioMzWo = false;
      bool JpxBiOGNmG = false;
      bool YfsVjpCADW = false;
      bool nabRpCHLiE = false;
      bool JcpFjphqbu = false;
      bool GBSGKfmKrr = false;
      bool mLwABhOfLg = false;
      bool Gudhwwpmlt = false;
      bool HKYHPlIUGJ = false;
      bool OxTqRygoEB = false;
      bool aCfuoLXjJO = false;
      bool ozYxjbmnkP = false;
      bool YsYBRJbmQy = false;
      bool PAijSFecSD = false;
      bool OYLrUOXVGp = false;
      bool LrEocgddce = false;
      bool jjTKznQnee = false;
      bool wJVgBzhggD = false;
      bool kFfZmWwtGk = false;
      string OgMPBlmxMy;
      string SWiQLqEcye;
      string gliLaUSNMr;
      string RxpWXQUziU;
      string GiSsxzNmOf;
      string iISXpcxBnf;
      string SYjUgUfLst;
      string WEsdYxSpOb;
      string HezgIJeSgt;
      string SSKDBgyyMb;
      string SBtTIdOoyr;
      string KHmMYnZDLW;
      string pNeyaDspbq;
      string yTcmIGjfXd;
      string toTojKfydN;
      string HumokFWurF;
      string jYNHspezES;
      string DsfgbBsoOQ;
      string WuLNjDgnBC;
      string iomRsVbMus;
      if(OgMPBlmxMy == SBtTIdOoyr){ffsLKNPFMj = true;}
      else if(SBtTIdOoyr == OgMPBlmxMy){OxTqRygoEB = true;}
      if(SWiQLqEcye == KHmMYnZDLW){MggbioMzWo = true;}
      else if(KHmMYnZDLW == SWiQLqEcye){aCfuoLXjJO = true;}
      if(gliLaUSNMr == pNeyaDspbq){JpxBiOGNmG = true;}
      else if(pNeyaDspbq == gliLaUSNMr){ozYxjbmnkP = true;}
      if(RxpWXQUziU == yTcmIGjfXd){YfsVjpCADW = true;}
      else if(yTcmIGjfXd == RxpWXQUziU){YsYBRJbmQy = true;}
      if(GiSsxzNmOf == toTojKfydN){nabRpCHLiE = true;}
      else if(toTojKfydN == GiSsxzNmOf){PAijSFecSD = true;}
      if(iISXpcxBnf == HumokFWurF){JcpFjphqbu = true;}
      else if(HumokFWurF == iISXpcxBnf){OYLrUOXVGp = true;}
      if(SYjUgUfLst == jYNHspezES){GBSGKfmKrr = true;}
      else if(jYNHspezES == SYjUgUfLst){LrEocgddce = true;}
      if(WEsdYxSpOb == DsfgbBsoOQ){mLwABhOfLg = true;}
      if(HezgIJeSgt == WuLNjDgnBC){Gudhwwpmlt = true;}
      if(SSKDBgyyMb == iomRsVbMus){HKYHPlIUGJ = true;}
      while(DsfgbBsoOQ == WEsdYxSpOb){jjTKznQnee = true;}
      while(WuLNjDgnBC == WuLNjDgnBC){wJVgBzhggD = true;}
      while(iomRsVbMus == iomRsVbMus){kFfZmWwtGk = true;}
      if(ffsLKNPFMj == true){ffsLKNPFMj = false;}
      if(MggbioMzWo == true){MggbioMzWo = false;}
      if(JpxBiOGNmG == true){JpxBiOGNmG = false;}
      if(YfsVjpCADW == true){YfsVjpCADW = false;}
      if(nabRpCHLiE == true){nabRpCHLiE = false;}
      if(JcpFjphqbu == true){JcpFjphqbu = false;}
      if(GBSGKfmKrr == true){GBSGKfmKrr = false;}
      if(mLwABhOfLg == true){mLwABhOfLg = false;}
      if(Gudhwwpmlt == true){Gudhwwpmlt = false;}
      if(HKYHPlIUGJ == true){HKYHPlIUGJ = false;}
      if(OxTqRygoEB == true){OxTqRygoEB = false;}
      if(aCfuoLXjJO == true){aCfuoLXjJO = false;}
      if(ozYxjbmnkP == true){ozYxjbmnkP = false;}
      if(YsYBRJbmQy == true){YsYBRJbmQy = false;}
      if(PAijSFecSD == true){PAijSFecSD = false;}
      if(OYLrUOXVGp == true){OYLrUOXVGp = false;}
      if(LrEocgddce == true){LrEocgddce = false;}
      if(jjTKznQnee == true){jjTKznQnee = false;}
      if(wJVgBzhggD == true){wJVgBzhggD = false;}
      if(kFfZmWwtGk == true){kFfZmWwtGk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RFLAJWICFP
{ 
  void gtIEspjulp()
  { 
      bool YfrGIVIuek = false;
      bool VQDbDYQUhL = false;
      bool wSAFQhjoyp = false;
      bool DKromBZnFf = false;
      bool ohuptqXMPF = false;
      bool sBejeFempZ = false;
      bool kTwnhKXNoC = false;
      bool zbOCWOXJLY = false;
      bool KXZsxBfHJg = false;
      bool WmyftUkHIi = false;
      bool TJXNFQlcNQ = false;
      bool hUbQPNtDqf = false;
      bool dPEHeLdiHB = false;
      bool cPAbFAALmf = false;
      bool ODtszBLgPg = false;
      bool agPcFDFPht = false;
      bool mDUMmRTzLO = false;
      bool NhQLRZVKHd = false;
      bool AwSZZwAXWC = false;
      bool tjdrrLhhFh = false;
      string mydrljFPma;
      string BAWLTjWMKj;
      string SprgnlBIVr;
      string suCxWXVxSF;
      string rXRjzOYXOp;
      string xhWYSxGxEA;
      string hwsthGyYow;
      string OYtErtXHgZ;
      string NdtfuKYceY;
      string RYsSWueWCs;
      string PziRaXnRfY;
      string ZzPelEBNzW;
      string hUtPdpfbgw;
      string OAFmJNtbEW;
      string WHxFzSINfS;
      string tRXeLtFSnk;
      string rMjjJYscUY;
      string JYIcAaXnPL;
      string gSuOBxcZbP;
      string IanTdKWMwa;
      if(mydrljFPma == PziRaXnRfY){YfrGIVIuek = true;}
      else if(PziRaXnRfY == mydrljFPma){TJXNFQlcNQ = true;}
      if(BAWLTjWMKj == ZzPelEBNzW){VQDbDYQUhL = true;}
      else if(ZzPelEBNzW == BAWLTjWMKj){hUbQPNtDqf = true;}
      if(SprgnlBIVr == hUtPdpfbgw){wSAFQhjoyp = true;}
      else if(hUtPdpfbgw == SprgnlBIVr){dPEHeLdiHB = true;}
      if(suCxWXVxSF == OAFmJNtbEW){DKromBZnFf = true;}
      else if(OAFmJNtbEW == suCxWXVxSF){cPAbFAALmf = true;}
      if(rXRjzOYXOp == WHxFzSINfS){ohuptqXMPF = true;}
      else if(WHxFzSINfS == rXRjzOYXOp){ODtszBLgPg = true;}
      if(xhWYSxGxEA == tRXeLtFSnk){sBejeFempZ = true;}
      else if(tRXeLtFSnk == xhWYSxGxEA){agPcFDFPht = true;}
      if(hwsthGyYow == rMjjJYscUY){kTwnhKXNoC = true;}
      else if(rMjjJYscUY == hwsthGyYow){mDUMmRTzLO = true;}
      if(OYtErtXHgZ == JYIcAaXnPL){zbOCWOXJLY = true;}
      if(NdtfuKYceY == gSuOBxcZbP){KXZsxBfHJg = true;}
      if(RYsSWueWCs == IanTdKWMwa){WmyftUkHIi = true;}
      while(JYIcAaXnPL == OYtErtXHgZ){NhQLRZVKHd = true;}
      while(gSuOBxcZbP == gSuOBxcZbP){AwSZZwAXWC = true;}
      while(IanTdKWMwa == IanTdKWMwa){tjdrrLhhFh = true;}
      if(YfrGIVIuek == true){YfrGIVIuek = false;}
      if(VQDbDYQUhL == true){VQDbDYQUhL = false;}
      if(wSAFQhjoyp == true){wSAFQhjoyp = false;}
      if(DKromBZnFf == true){DKromBZnFf = false;}
      if(ohuptqXMPF == true){ohuptqXMPF = false;}
      if(sBejeFempZ == true){sBejeFempZ = false;}
      if(kTwnhKXNoC == true){kTwnhKXNoC = false;}
      if(zbOCWOXJLY == true){zbOCWOXJLY = false;}
      if(KXZsxBfHJg == true){KXZsxBfHJg = false;}
      if(WmyftUkHIi == true){WmyftUkHIi = false;}
      if(TJXNFQlcNQ == true){TJXNFQlcNQ = false;}
      if(hUbQPNtDqf == true){hUbQPNtDqf = false;}
      if(dPEHeLdiHB == true){dPEHeLdiHB = false;}
      if(cPAbFAALmf == true){cPAbFAALmf = false;}
      if(ODtszBLgPg == true){ODtszBLgPg = false;}
      if(agPcFDFPht == true){agPcFDFPht = false;}
      if(mDUMmRTzLO == true){mDUMmRTzLO = false;}
      if(NhQLRZVKHd == true){NhQLRZVKHd = false;}
      if(AwSZZwAXWC == true){AwSZZwAXWC = false;}
      if(tjdrrLhhFh == true){tjdrrLhhFh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RTRWEIMISK
{ 
  void BOpDaBlUpi()
  { 
      bool LUpbwxMbBA = false;
      bool VDqwZunXhf = false;
      bool uNIbxbOeRu = false;
      bool EPpDdOuKwN = false;
      bool zYOEaXQDir = false;
      bool zrNWrxMOeU = false;
      bool ihKFwGNmSK = false;
      bool AdaUgVzEBY = false;
      bool LKwYwLCTLf = false;
      bool CgfbeROJIU = false;
      bool KcjXrVQsoV = false;
      bool yHNFIXkkJj = false;
      bool EsbmHSxyoP = false;
      bool HoufOhZXfZ = false;
      bool uIdpgtTZSG = false;
      bool fcAYQQAxQZ = false;
      bool YRDZNJMgqZ = false;
      bool yNEinuPlGM = false;
      bool zWfijdoWPx = false;
      bool hcHGBcEEbX = false;
      string UZKhrWhKAW;
      string EYIkqmPzDJ;
      string XrGfLizZJP;
      string ojNUsozqJa;
      string WFwTmuYAUP;
      string IHQFVcSeVI;
      string WHAjhTKmrC;
      string mXpbmRumrQ;
      string KyFtHgTIzQ;
      string VUVADOFLlT;
      string kkIRbjRnRD;
      string gflFyYCFcA;
      string zCnafSCoXX;
      string LjBYfYomRW;
      string PfpujphPCe;
      string MAhkThxhlJ;
      string hbPtrwdNZZ;
      string iOmGjTLsqo;
      string jlmrLLBPEx;
      string bHAInnPwCm;
      if(UZKhrWhKAW == kkIRbjRnRD){LUpbwxMbBA = true;}
      else if(kkIRbjRnRD == UZKhrWhKAW){KcjXrVQsoV = true;}
      if(EYIkqmPzDJ == gflFyYCFcA){VDqwZunXhf = true;}
      else if(gflFyYCFcA == EYIkqmPzDJ){yHNFIXkkJj = true;}
      if(XrGfLizZJP == zCnafSCoXX){uNIbxbOeRu = true;}
      else if(zCnafSCoXX == XrGfLizZJP){EsbmHSxyoP = true;}
      if(ojNUsozqJa == LjBYfYomRW){EPpDdOuKwN = true;}
      else if(LjBYfYomRW == ojNUsozqJa){HoufOhZXfZ = true;}
      if(WFwTmuYAUP == PfpujphPCe){zYOEaXQDir = true;}
      else if(PfpujphPCe == WFwTmuYAUP){uIdpgtTZSG = true;}
      if(IHQFVcSeVI == MAhkThxhlJ){zrNWrxMOeU = true;}
      else if(MAhkThxhlJ == IHQFVcSeVI){fcAYQQAxQZ = true;}
      if(WHAjhTKmrC == hbPtrwdNZZ){ihKFwGNmSK = true;}
      else if(hbPtrwdNZZ == WHAjhTKmrC){YRDZNJMgqZ = true;}
      if(mXpbmRumrQ == iOmGjTLsqo){AdaUgVzEBY = true;}
      if(KyFtHgTIzQ == jlmrLLBPEx){LKwYwLCTLf = true;}
      if(VUVADOFLlT == bHAInnPwCm){CgfbeROJIU = true;}
      while(iOmGjTLsqo == mXpbmRumrQ){yNEinuPlGM = true;}
      while(jlmrLLBPEx == jlmrLLBPEx){zWfijdoWPx = true;}
      while(bHAInnPwCm == bHAInnPwCm){hcHGBcEEbX = true;}
      if(LUpbwxMbBA == true){LUpbwxMbBA = false;}
      if(VDqwZunXhf == true){VDqwZunXhf = false;}
      if(uNIbxbOeRu == true){uNIbxbOeRu = false;}
      if(EPpDdOuKwN == true){EPpDdOuKwN = false;}
      if(zYOEaXQDir == true){zYOEaXQDir = false;}
      if(zrNWrxMOeU == true){zrNWrxMOeU = false;}
      if(ihKFwGNmSK == true){ihKFwGNmSK = false;}
      if(AdaUgVzEBY == true){AdaUgVzEBY = false;}
      if(LKwYwLCTLf == true){LKwYwLCTLf = false;}
      if(CgfbeROJIU == true){CgfbeROJIU = false;}
      if(KcjXrVQsoV == true){KcjXrVQsoV = false;}
      if(yHNFIXkkJj == true){yHNFIXkkJj = false;}
      if(EsbmHSxyoP == true){EsbmHSxyoP = false;}
      if(HoufOhZXfZ == true){HoufOhZXfZ = false;}
      if(uIdpgtTZSG == true){uIdpgtTZSG = false;}
      if(fcAYQQAxQZ == true){fcAYQQAxQZ = false;}
      if(YRDZNJMgqZ == true){YRDZNJMgqZ = false;}
      if(yNEinuPlGM == true){yNEinuPlGM = false;}
      if(zWfijdoWPx == true){zWfijdoWPx = false;}
      if(hcHGBcEEbX == true){hcHGBcEEbX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AZLMGJRCKN
{ 
  void mIpEUagWyp()
  { 
      bool IDgLnWjpLs = false;
      bool sHbWCIFPSf = false;
      bool xeQiOmEuIm = false;
      bool ADZjfaqlid = false;
      bool dtVnwZQdbB = false;
      bool UGAFidmywA = false;
      bool RogXrogrpa = false;
      bool QTgoBCIclo = false;
      bool WZTGStCUAB = false;
      bool nrohLoNxwH = false;
      bool sYmAkBZKDg = false;
      bool UKLHdrUZFy = false;
      bool lLqomXdydM = false;
      bool oLfeuylSVQ = false;
      bool TqRSfdghES = false;
      bool TmryUhgyNM = false;
      bool WagqrJdgLf = false;
      bool GBDndttXII = false;
      bool qwtKrBShKN = false;
      bool RmFJtsKZaW = false;
      string jSOzajxiqq;
      string ZmgDgZGbyL;
      string btYUrRGAWd;
      string KCxnghsqPR;
      string pHgzWOEuSw;
      string aVLPnpstHJ;
      string dxMNgXprWy;
      string gOMfUTNZxc;
      string MKgaCnpgAq;
      string ozjFnsuTSz;
      string XLYUOLJfuT;
      string bTwdxSQkKr;
      string hIrfJgeGrb;
      string YtHyFAlLLq;
      string dKlfOUCGIU;
      string bJJZQPSsKg;
      string gDmSthLyYw;
      string gFZSOXWLQn;
      string eYyWAOkggo;
      string gsAdKaWSkg;
      if(jSOzajxiqq == XLYUOLJfuT){IDgLnWjpLs = true;}
      else if(XLYUOLJfuT == jSOzajxiqq){sYmAkBZKDg = true;}
      if(ZmgDgZGbyL == bTwdxSQkKr){sHbWCIFPSf = true;}
      else if(bTwdxSQkKr == ZmgDgZGbyL){UKLHdrUZFy = true;}
      if(btYUrRGAWd == hIrfJgeGrb){xeQiOmEuIm = true;}
      else if(hIrfJgeGrb == btYUrRGAWd){lLqomXdydM = true;}
      if(KCxnghsqPR == YtHyFAlLLq){ADZjfaqlid = true;}
      else if(YtHyFAlLLq == KCxnghsqPR){oLfeuylSVQ = true;}
      if(pHgzWOEuSw == dKlfOUCGIU){dtVnwZQdbB = true;}
      else if(dKlfOUCGIU == pHgzWOEuSw){TqRSfdghES = true;}
      if(aVLPnpstHJ == bJJZQPSsKg){UGAFidmywA = true;}
      else if(bJJZQPSsKg == aVLPnpstHJ){TmryUhgyNM = true;}
      if(dxMNgXprWy == gDmSthLyYw){RogXrogrpa = true;}
      else if(gDmSthLyYw == dxMNgXprWy){WagqrJdgLf = true;}
      if(gOMfUTNZxc == gFZSOXWLQn){QTgoBCIclo = true;}
      if(MKgaCnpgAq == eYyWAOkggo){WZTGStCUAB = true;}
      if(ozjFnsuTSz == gsAdKaWSkg){nrohLoNxwH = true;}
      while(gFZSOXWLQn == gOMfUTNZxc){GBDndttXII = true;}
      while(eYyWAOkggo == eYyWAOkggo){qwtKrBShKN = true;}
      while(gsAdKaWSkg == gsAdKaWSkg){RmFJtsKZaW = true;}
      if(IDgLnWjpLs == true){IDgLnWjpLs = false;}
      if(sHbWCIFPSf == true){sHbWCIFPSf = false;}
      if(xeQiOmEuIm == true){xeQiOmEuIm = false;}
      if(ADZjfaqlid == true){ADZjfaqlid = false;}
      if(dtVnwZQdbB == true){dtVnwZQdbB = false;}
      if(UGAFidmywA == true){UGAFidmywA = false;}
      if(RogXrogrpa == true){RogXrogrpa = false;}
      if(QTgoBCIclo == true){QTgoBCIclo = false;}
      if(WZTGStCUAB == true){WZTGStCUAB = false;}
      if(nrohLoNxwH == true){nrohLoNxwH = false;}
      if(sYmAkBZKDg == true){sYmAkBZKDg = false;}
      if(UKLHdrUZFy == true){UKLHdrUZFy = false;}
      if(lLqomXdydM == true){lLqomXdydM = false;}
      if(oLfeuylSVQ == true){oLfeuylSVQ = false;}
      if(TqRSfdghES == true){TqRSfdghES = false;}
      if(TmryUhgyNM == true){TmryUhgyNM = false;}
      if(WagqrJdgLf == true){WagqrJdgLf = false;}
      if(GBDndttXII == true){GBDndttXII = false;}
      if(qwtKrBShKN == true){qwtKrBShKN = false;}
      if(RmFJtsKZaW == true){RmFJtsKZaW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZOTPWKMLRD
{ 
  void SUCMHdZLbw()
  { 
      bool xVmMGpwtdw = false;
      bool eahQszyiXj = false;
      bool GfkeBGusDu = false;
      bool CwuYKkJREY = false;
      bool rgIIgHmjnh = false;
      bool bgEfWRjILA = false;
      bool tCqhthfIGH = false;
      bool ogdyrZXcxs = false;
      bool SKFUTVrOBs = false;
      bool ABFEdooZNz = false;
      bool addfstCFnw = false;
      bool tHKSgiwVNY = false;
      bool oRGMGnbZTC = false;
      bool jxOyRogYbY = false;
      bool SmZFyspmWt = false;
      bool rTMoJJWQZF = false;
      bool ScklMxsMAr = false;
      bool CXtnFCqBOM = false;
      bool xnCZJPkYoH = false;
      bool SPyVFoBnrB = false;
      string EpqEkdKLMb;
      string VsWubUSrtR;
      string aPYZnXBNDs;
      string qtCxlkjCOn;
      string GkNLRgNDoE;
      string ohuAXoRQPN;
      string bwHSjbpZIq;
      string kPcFLoBNIH;
      string JxYzDJzCYx;
      string TSmBLJdtap;
      string qntxSZFoni;
      string mDHHEdXHPa;
      string bmBLXScatB;
      string KGlSQUDagd;
      string EKOoJPKewh;
      string lWeANDmiZA;
      string wdulYgRoqi;
      string awZkWRYLBI;
      string HxeLOnalEM;
      string hUBggaPbQc;
      if(EpqEkdKLMb == qntxSZFoni){xVmMGpwtdw = true;}
      else if(qntxSZFoni == EpqEkdKLMb){addfstCFnw = true;}
      if(VsWubUSrtR == mDHHEdXHPa){eahQszyiXj = true;}
      else if(mDHHEdXHPa == VsWubUSrtR){tHKSgiwVNY = true;}
      if(aPYZnXBNDs == bmBLXScatB){GfkeBGusDu = true;}
      else if(bmBLXScatB == aPYZnXBNDs){oRGMGnbZTC = true;}
      if(qtCxlkjCOn == KGlSQUDagd){CwuYKkJREY = true;}
      else if(KGlSQUDagd == qtCxlkjCOn){jxOyRogYbY = true;}
      if(GkNLRgNDoE == EKOoJPKewh){rgIIgHmjnh = true;}
      else if(EKOoJPKewh == GkNLRgNDoE){SmZFyspmWt = true;}
      if(ohuAXoRQPN == lWeANDmiZA){bgEfWRjILA = true;}
      else if(lWeANDmiZA == ohuAXoRQPN){rTMoJJWQZF = true;}
      if(bwHSjbpZIq == wdulYgRoqi){tCqhthfIGH = true;}
      else if(wdulYgRoqi == bwHSjbpZIq){ScklMxsMAr = true;}
      if(kPcFLoBNIH == awZkWRYLBI){ogdyrZXcxs = true;}
      if(JxYzDJzCYx == HxeLOnalEM){SKFUTVrOBs = true;}
      if(TSmBLJdtap == hUBggaPbQc){ABFEdooZNz = true;}
      while(awZkWRYLBI == kPcFLoBNIH){CXtnFCqBOM = true;}
      while(HxeLOnalEM == HxeLOnalEM){xnCZJPkYoH = true;}
      while(hUBggaPbQc == hUBggaPbQc){SPyVFoBnrB = true;}
      if(xVmMGpwtdw == true){xVmMGpwtdw = false;}
      if(eahQszyiXj == true){eahQszyiXj = false;}
      if(GfkeBGusDu == true){GfkeBGusDu = false;}
      if(CwuYKkJREY == true){CwuYKkJREY = false;}
      if(rgIIgHmjnh == true){rgIIgHmjnh = false;}
      if(bgEfWRjILA == true){bgEfWRjILA = false;}
      if(tCqhthfIGH == true){tCqhthfIGH = false;}
      if(ogdyrZXcxs == true){ogdyrZXcxs = false;}
      if(SKFUTVrOBs == true){SKFUTVrOBs = false;}
      if(ABFEdooZNz == true){ABFEdooZNz = false;}
      if(addfstCFnw == true){addfstCFnw = false;}
      if(tHKSgiwVNY == true){tHKSgiwVNY = false;}
      if(oRGMGnbZTC == true){oRGMGnbZTC = false;}
      if(jxOyRogYbY == true){jxOyRogYbY = false;}
      if(SmZFyspmWt == true){SmZFyspmWt = false;}
      if(rTMoJJWQZF == true){rTMoJJWQZF = false;}
      if(ScklMxsMAr == true){ScklMxsMAr = false;}
      if(CXtnFCqBOM == true){CXtnFCqBOM = false;}
      if(xnCZJPkYoH == true){xnCZJPkYoH = false;}
      if(SPyVFoBnrB == true){SPyVFoBnrB = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WQXSOBQDWB
{ 
  void CJhtrrSAma()
  { 
      bool QHICqjMDDq = false;
      bool NYPlSabWgr = false;
      bool uctnAPrZuf = false;
      bool DlwrYIapxR = false;
      bool BTPYgEMnem = false;
      bool sAGLXmNUCa = false;
      bool qmHWmbcWNJ = false;
      bool AkYOpbNUHu = false;
      bool CEtHmDfjUk = false;
      bool uGnVZOLskF = false;
      bool apaVmHaqUw = false;
      bool ZrNGYmrAij = false;
      bool usCbOtxCnb = false;
      bool PUiCTMcYUf = false;
      bool boOYLCsgxm = false;
      bool VLJBJhmyGs = false;
      bool JXCsfFJLlL = false;
      bool JTeDHGedPM = false;
      bool AQFBGEKhrl = false;
      bool YeRbWDlVrG = false;
      string KbJnUwGOhs;
      string ZgiCFEcYyL;
      string hboIFzbAJk;
      string SAbIUULrkx;
      string gstVAnKDQK;
      string XhiSsimAiV;
      string TGExaYEZgk;
      string sknAengSIR;
      string nYizMjQJPl;
      string gKIOdZoxua;
      string ijVfRmthmE;
      string BTrQhtWEjq;
      string IuKuDibZWE;
      string yjXhhDCDgZ;
      string ohVKjNRrgi;
      string DjNERmZaci;
      string LUKhGRwKid;
      string rTXJBaZAKp;
      string QjrnBNWSPt;
      string dbayOUWmES;
      if(KbJnUwGOhs == ijVfRmthmE){QHICqjMDDq = true;}
      else if(ijVfRmthmE == KbJnUwGOhs){apaVmHaqUw = true;}
      if(ZgiCFEcYyL == BTrQhtWEjq){NYPlSabWgr = true;}
      else if(BTrQhtWEjq == ZgiCFEcYyL){ZrNGYmrAij = true;}
      if(hboIFzbAJk == IuKuDibZWE){uctnAPrZuf = true;}
      else if(IuKuDibZWE == hboIFzbAJk){usCbOtxCnb = true;}
      if(SAbIUULrkx == yjXhhDCDgZ){DlwrYIapxR = true;}
      else if(yjXhhDCDgZ == SAbIUULrkx){PUiCTMcYUf = true;}
      if(gstVAnKDQK == ohVKjNRrgi){BTPYgEMnem = true;}
      else if(ohVKjNRrgi == gstVAnKDQK){boOYLCsgxm = true;}
      if(XhiSsimAiV == DjNERmZaci){sAGLXmNUCa = true;}
      else if(DjNERmZaci == XhiSsimAiV){VLJBJhmyGs = true;}
      if(TGExaYEZgk == LUKhGRwKid){qmHWmbcWNJ = true;}
      else if(LUKhGRwKid == TGExaYEZgk){JXCsfFJLlL = true;}
      if(sknAengSIR == rTXJBaZAKp){AkYOpbNUHu = true;}
      if(nYizMjQJPl == QjrnBNWSPt){CEtHmDfjUk = true;}
      if(gKIOdZoxua == dbayOUWmES){uGnVZOLskF = true;}
      while(rTXJBaZAKp == sknAengSIR){JTeDHGedPM = true;}
      while(QjrnBNWSPt == QjrnBNWSPt){AQFBGEKhrl = true;}
      while(dbayOUWmES == dbayOUWmES){YeRbWDlVrG = true;}
      if(QHICqjMDDq == true){QHICqjMDDq = false;}
      if(NYPlSabWgr == true){NYPlSabWgr = false;}
      if(uctnAPrZuf == true){uctnAPrZuf = false;}
      if(DlwrYIapxR == true){DlwrYIapxR = false;}
      if(BTPYgEMnem == true){BTPYgEMnem = false;}
      if(sAGLXmNUCa == true){sAGLXmNUCa = false;}
      if(qmHWmbcWNJ == true){qmHWmbcWNJ = false;}
      if(AkYOpbNUHu == true){AkYOpbNUHu = false;}
      if(CEtHmDfjUk == true){CEtHmDfjUk = false;}
      if(uGnVZOLskF == true){uGnVZOLskF = false;}
      if(apaVmHaqUw == true){apaVmHaqUw = false;}
      if(ZrNGYmrAij == true){ZrNGYmrAij = false;}
      if(usCbOtxCnb == true){usCbOtxCnb = false;}
      if(PUiCTMcYUf == true){PUiCTMcYUf = false;}
      if(boOYLCsgxm == true){boOYLCsgxm = false;}
      if(VLJBJhmyGs == true){VLJBJhmyGs = false;}
      if(JXCsfFJLlL == true){JXCsfFJLlL = false;}
      if(JTeDHGedPM == true){JTeDHGedPM = false;}
      if(AQFBGEKhrl == true){AQFBGEKhrl = false;}
      if(YeRbWDlVrG == true){YeRbWDlVrG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BUDTLROSJX
{ 
  void fygJECptBI()
  { 
      bool iNLwUeobYs = false;
      bool wOJNXFWtfj = false;
      bool geweSZLCoG = false;
      bool CNCIuCTdlk = false;
      bool WyIICMxpoE = false;
      bool IHWEbDQfdt = false;
      bool SlmRqiLARU = false;
      bool lsItMiVnbF = false;
      bool bVNkxtpxgq = false;
      bool lmzqSkLpQT = false;
      bool BxXTROybrs = false;
      bool hcDXRxXBTw = false;
      bool sNSZiysQbb = false;
      bool WeWIEextTi = false;
      bool wNFnJIjbZV = false;
      bool MKWTandyjS = false;
      bool UgAPVCFuTl = false;
      bool zdhkAeHeQZ = false;
      bool ORqGOMIDgQ = false;
      bool BzJAOONoSG = false;
      string ZXeebdcUHB;
      string RBjWwEWyIr;
      string DpAbuhREeM;
      string rycFLWOSNf;
      string cNRiXDOWqI;
      string KzlVEiWCjt;
      string MXouITYeKZ;
      string oEjbPpYREr;
      string SuMVBsMHAb;
      string zLiSTEXVZE;
      string ejLRTGPJGO;
      string KEyrbRKStP;
      string GRdiZdVRIK;
      string RCrzMhiuCt;
      string dCAlQuMpqC;
      string tFbKxNGbqo;
      string kxAwwHMyet;
      string hPPzlDQrRd;
      string QeZGPMagCz;
      string aHDNJfBDoI;
      if(ZXeebdcUHB == ejLRTGPJGO){iNLwUeobYs = true;}
      else if(ejLRTGPJGO == ZXeebdcUHB){BxXTROybrs = true;}
      if(RBjWwEWyIr == KEyrbRKStP){wOJNXFWtfj = true;}
      else if(KEyrbRKStP == RBjWwEWyIr){hcDXRxXBTw = true;}
      if(DpAbuhREeM == GRdiZdVRIK){geweSZLCoG = true;}
      else if(GRdiZdVRIK == DpAbuhREeM){sNSZiysQbb = true;}
      if(rycFLWOSNf == RCrzMhiuCt){CNCIuCTdlk = true;}
      else if(RCrzMhiuCt == rycFLWOSNf){WeWIEextTi = true;}
      if(cNRiXDOWqI == dCAlQuMpqC){WyIICMxpoE = true;}
      else if(dCAlQuMpqC == cNRiXDOWqI){wNFnJIjbZV = true;}
      if(KzlVEiWCjt == tFbKxNGbqo){IHWEbDQfdt = true;}
      else if(tFbKxNGbqo == KzlVEiWCjt){MKWTandyjS = true;}
      if(MXouITYeKZ == kxAwwHMyet){SlmRqiLARU = true;}
      else if(kxAwwHMyet == MXouITYeKZ){UgAPVCFuTl = true;}
      if(oEjbPpYREr == hPPzlDQrRd){lsItMiVnbF = true;}
      if(SuMVBsMHAb == QeZGPMagCz){bVNkxtpxgq = true;}
      if(zLiSTEXVZE == aHDNJfBDoI){lmzqSkLpQT = true;}
      while(hPPzlDQrRd == oEjbPpYREr){zdhkAeHeQZ = true;}
      while(QeZGPMagCz == QeZGPMagCz){ORqGOMIDgQ = true;}
      while(aHDNJfBDoI == aHDNJfBDoI){BzJAOONoSG = true;}
      if(iNLwUeobYs == true){iNLwUeobYs = false;}
      if(wOJNXFWtfj == true){wOJNXFWtfj = false;}
      if(geweSZLCoG == true){geweSZLCoG = false;}
      if(CNCIuCTdlk == true){CNCIuCTdlk = false;}
      if(WyIICMxpoE == true){WyIICMxpoE = false;}
      if(IHWEbDQfdt == true){IHWEbDQfdt = false;}
      if(SlmRqiLARU == true){SlmRqiLARU = false;}
      if(lsItMiVnbF == true){lsItMiVnbF = false;}
      if(bVNkxtpxgq == true){bVNkxtpxgq = false;}
      if(lmzqSkLpQT == true){lmzqSkLpQT = false;}
      if(BxXTROybrs == true){BxXTROybrs = false;}
      if(hcDXRxXBTw == true){hcDXRxXBTw = false;}
      if(sNSZiysQbb == true){sNSZiysQbb = false;}
      if(WeWIEextTi == true){WeWIEextTi = false;}
      if(wNFnJIjbZV == true){wNFnJIjbZV = false;}
      if(MKWTandyjS == true){MKWTandyjS = false;}
      if(UgAPVCFuTl == true){UgAPVCFuTl = false;}
      if(zdhkAeHeQZ == true){zdhkAeHeQZ = false;}
      if(ORqGOMIDgQ == true){ORqGOMIDgQ = false;}
      if(BzJAOONoSG == true){BzJAOONoSG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class STKKHBYNVQ
{ 
  void pcZwFXaWKX()
  { 
      bool ZEEpLCFyfa = false;
      bool ykUdrkLECN = false;
      bool XceWipZfMb = false;
      bool JgHnPMnYsG = false;
      bool tJJoidpmKW = false;
      bool kJMLlwYhGV = false;
      bool cZkDRQcfOf = false;
      bool wmKuTNTomF = false;
      bool zmVoqAUwBT = false;
      bool mGRXdUYGXS = false;
      bool AJYeZxzQqY = false;
      bool cjtAVoDuMQ = false;
      bool ugetspUmQy = false;
      bool oNRUEoOqOs = false;
      bool BEEcqCLQBo = false;
      bool QcsGWTedlj = false;
      bool bLzsjOSYwf = false;
      bool ZMfSPqcCDp = false;
      bool dEkbhLTKxk = false;
      bool KkEeRoFLsb = false;
      string BQWMJzwXOA;
      string dEemOQQjXN;
      string quCzhNfgqq;
      string BklUCXkVOM;
      string lRwDnkwssF;
      string ZzlsdFOXeu;
      string SPgWzsRKkq;
      string etVLbZzzKw;
      string RmmlQBysqw;
      string INqSsWjYDt;
      string TkICZfdaRD;
      string GPMhdfbuFx;
      string uYTniRCCRE;
      string oauQcfejLk;
      string XWbmpsAJnM;
      string toSFzuRTTI;
      string caIMNfTwEy;
      string BqiUZXwgyo;
      string QrOEJAHpQh;
      string OLQCHxNOYK;
      if(BQWMJzwXOA == TkICZfdaRD){ZEEpLCFyfa = true;}
      else if(TkICZfdaRD == BQWMJzwXOA){AJYeZxzQqY = true;}
      if(dEemOQQjXN == GPMhdfbuFx){ykUdrkLECN = true;}
      else if(GPMhdfbuFx == dEemOQQjXN){cjtAVoDuMQ = true;}
      if(quCzhNfgqq == uYTniRCCRE){XceWipZfMb = true;}
      else if(uYTniRCCRE == quCzhNfgqq){ugetspUmQy = true;}
      if(BklUCXkVOM == oauQcfejLk){JgHnPMnYsG = true;}
      else if(oauQcfejLk == BklUCXkVOM){oNRUEoOqOs = true;}
      if(lRwDnkwssF == XWbmpsAJnM){tJJoidpmKW = true;}
      else if(XWbmpsAJnM == lRwDnkwssF){BEEcqCLQBo = true;}
      if(ZzlsdFOXeu == toSFzuRTTI){kJMLlwYhGV = true;}
      else if(toSFzuRTTI == ZzlsdFOXeu){QcsGWTedlj = true;}
      if(SPgWzsRKkq == caIMNfTwEy){cZkDRQcfOf = true;}
      else if(caIMNfTwEy == SPgWzsRKkq){bLzsjOSYwf = true;}
      if(etVLbZzzKw == BqiUZXwgyo){wmKuTNTomF = true;}
      if(RmmlQBysqw == QrOEJAHpQh){zmVoqAUwBT = true;}
      if(INqSsWjYDt == OLQCHxNOYK){mGRXdUYGXS = true;}
      while(BqiUZXwgyo == etVLbZzzKw){ZMfSPqcCDp = true;}
      while(QrOEJAHpQh == QrOEJAHpQh){dEkbhLTKxk = true;}
      while(OLQCHxNOYK == OLQCHxNOYK){KkEeRoFLsb = true;}
      if(ZEEpLCFyfa == true){ZEEpLCFyfa = false;}
      if(ykUdrkLECN == true){ykUdrkLECN = false;}
      if(XceWipZfMb == true){XceWipZfMb = false;}
      if(JgHnPMnYsG == true){JgHnPMnYsG = false;}
      if(tJJoidpmKW == true){tJJoidpmKW = false;}
      if(kJMLlwYhGV == true){kJMLlwYhGV = false;}
      if(cZkDRQcfOf == true){cZkDRQcfOf = false;}
      if(wmKuTNTomF == true){wmKuTNTomF = false;}
      if(zmVoqAUwBT == true){zmVoqAUwBT = false;}
      if(mGRXdUYGXS == true){mGRXdUYGXS = false;}
      if(AJYeZxzQqY == true){AJYeZxzQqY = false;}
      if(cjtAVoDuMQ == true){cjtAVoDuMQ = false;}
      if(ugetspUmQy == true){ugetspUmQy = false;}
      if(oNRUEoOqOs == true){oNRUEoOqOs = false;}
      if(BEEcqCLQBo == true){BEEcqCLQBo = false;}
      if(QcsGWTedlj == true){QcsGWTedlj = false;}
      if(bLzsjOSYwf == true){bLzsjOSYwf = false;}
      if(ZMfSPqcCDp == true){ZMfSPqcCDp = false;}
      if(dEkbhLTKxk == true){dEkbhLTKxk = false;}
      if(KkEeRoFLsb == true){KkEeRoFLsb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OWZGCVXRZG
{ 
  void bTWJikCxrr()
  { 
      bool wMqHtjZktI = false;
      bool oaFAkFlzxe = false;
      bool UhIiqpdPzU = false;
      bool EozNLURoQF = false;
      bool eFKCZlBnFX = false;
      bool OjshoOEIPE = false;
      bool GJkZDzarnP = false;
      bool nxiqAPWGIF = false;
      bool noJbFTwRYx = false;
      bool QJYLzUTkyZ = false;
      bool KIAojRABgY = false;
      bool qxzKPRzITi = false;
      bool DJTqzeXWre = false;
      bool ulHhtfoUrm = false;
      bool StjzFSJOwg = false;
      bool UKQPWBoIpE = false;
      bool tdICcFpQPj = false;
      bool beQgPIYUkf = false;
      bool UJaKJkUrLs = false;
      bool qfdmsokhVA = false;
      string KwZYJKrNZW;
      string qerdWrFfyu;
      string xDxLVMQdAx;
      string nAHUuoVdKU;
      string hMNFXDSGgF;
      string pMquEHzOCw;
      string sNMciqWXhg;
      string WaUdNjprmR;
      string sJxtGbILPn;
      string pHaNaTzjWf;
      string gtYziOqNhT;
      string RHajCiQSVG;
      string iKHtqOzHCt;
      string kFUHkzkxLr;
      string ktduWigpKr;
      string DgOPwVldIT;
      string OiKmiLlTzD;
      string xSHcVnfLXI;
      string hJwKpEpEKP;
      string MHoqcBjpMK;
      if(KwZYJKrNZW == gtYziOqNhT){wMqHtjZktI = true;}
      else if(gtYziOqNhT == KwZYJKrNZW){KIAojRABgY = true;}
      if(qerdWrFfyu == RHajCiQSVG){oaFAkFlzxe = true;}
      else if(RHajCiQSVG == qerdWrFfyu){qxzKPRzITi = true;}
      if(xDxLVMQdAx == iKHtqOzHCt){UhIiqpdPzU = true;}
      else if(iKHtqOzHCt == xDxLVMQdAx){DJTqzeXWre = true;}
      if(nAHUuoVdKU == kFUHkzkxLr){EozNLURoQF = true;}
      else if(kFUHkzkxLr == nAHUuoVdKU){ulHhtfoUrm = true;}
      if(hMNFXDSGgF == ktduWigpKr){eFKCZlBnFX = true;}
      else if(ktduWigpKr == hMNFXDSGgF){StjzFSJOwg = true;}
      if(pMquEHzOCw == DgOPwVldIT){OjshoOEIPE = true;}
      else if(DgOPwVldIT == pMquEHzOCw){UKQPWBoIpE = true;}
      if(sNMciqWXhg == OiKmiLlTzD){GJkZDzarnP = true;}
      else if(OiKmiLlTzD == sNMciqWXhg){tdICcFpQPj = true;}
      if(WaUdNjprmR == xSHcVnfLXI){nxiqAPWGIF = true;}
      if(sJxtGbILPn == hJwKpEpEKP){noJbFTwRYx = true;}
      if(pHaNaTzjWf == MHoqcBjpMK){QJYLzUTkyZ = true;}
      while(xSHcVnfLXI == WaUdNjprmR){beQgPIYUkf = true;}
      while(hJwKpEpEKP == hJwKpEpEKP){UJaKJkUrLs = true;}
      while(MHoqcBjpMK == MHoqcBjpMK){qfdmsokhVA = true;}
      if(wMqHtjZktI == true){wMqHtjZktI = false;}
      if(oaFAkFlzxe == true){oaFAkFlzxe = false;}
      if(UhIiqpdPzU == true){UhIiqpdPzU = false;}
      if(EozNLURoQF == true){EozNLURoQF = false;}
      if(eFKCZlBnFX == true){eFKCZlBnFX = false;}
      if(OjshoOEIPE == true){OjshoOEIPE = false;}
      if(GJkZDzarnP == true){GJkZDzarnP = false;}
      if(nxiqAPWGIF == true){nxiqAPWGIF = false;}
      if(noJbFTwRYx == true){noJbFTwRYx = false;}
      if(QJYLzUTkyZ == true){QJYLzUTkyZ = false;}
      if(KIAojRABgY == true){KIAojRABgY = false;}
      if(qxzKPRzITi == true){qxzKPRzITi = false;}
      if(DJTqzeXWre == true){DJTqzeXWre = false;}
      if(ulHhtfoUrm == true){ulHhtfoUrm = false;}
      if(StjzFSJOwg == true){StjzFSJOwg = false;}
      if(UKQPWBoIpE == true){UKQPWBoIpE = false;}
      if(tdICcFpQPj == true){tdICcFpQPj = false;}
      if(beQgPIYUkf == true){beQgPIYUkf = false;}
      if(UJaKJkUrLs == true){UJaKJkUrLs = false;}
      if(qfdmsokhVA == true){qfdmsokhVA = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ISUGNDBVQD
{ 
  void GyMqbSzOnK()
  { 
      bool cFJkVyBzkN = false;
      bool CuLgErcRLV = false;
      bool hASyDllTEi = false;
      bool ayChRKICtY = false;
      bool alkGweEugy = false;
      bool qpQlRblLgi = false;
      bool MguBmDrcOt = false;
      bool iZXmtwPMBX = false;
      bool gpmGrjhQQp = false;
      bool XwUKLspiSs = false;
      bool mbglPQRcrU = false;
      bool LZKIBWtoJC = false;
      bool UbMYWlFNaB = false;
      bool FgsIkFJAqa = false;
      bool SFYudylLIo = false;
      bool zBYQXKOHPc = false;
      bool LEBpOefBWw = false;
      bool NsxhjPDQtX = false;
      bool AsElCEHcka = false;
      bool pBjNPODhgz = false;
      string YMmnElSCLA;
      string tgdQQoEiaw;
      string NyzwWjUWKc;
      string JIQYtrlxzB;
      string dKZQIecQud;
      string qEulkZNqQU;
      string QurhqlQFVY;
      string CGiYSCYhmq;
      string GEbglEMdnU;
      string eugekhSqZW;
      string OXtVajUVTh;
      string AeDZNMTLwl;
      string ZlQBtHZEnk;
      string jgIQbhJFFB;
      string jpKFTzrHQY;
      string NHumxcGsbT;
      string KQhixTWlxe;
      string COFuCiaKht;
      string PSnPWbkVTz;
      string aQckpTIbHz;
      if(YMmnElSCLA == OXtVajUVTh){cFJkVyBzkN = true;}
      else if(OXtVajUVTh == YMmnElSCLA){mbglPQRcrU = true;}
      if(tgdQQoEiaw == AeDZNMTLwl){CuLgErcRLV = true;}
      else if(AeDZNMTLwl == tgdQQoEiaw){LZKIBWtoJC = true;}
      if(NyzwWjUWKc == ZlQBtHZEnk){hASyDllTEi = true;}
      else if(ZlQBtHZEnk == NyzwWjUWKc){UbMYWlFNaB = true;}
      if(JIQYtrlxzB == jgIQbhJFFB){ayChRKICtY = true;}
      else if(jgIQbhJFFB == JIQYtrlxzB){FgsIkFJAqa = true;}
      if(dKZQIecQud == jpKFTzrHQY){alkGweEugy = true;}
      else if(jpKFTzrHQY == dKZQIecQud){SFYudylLIo = true;}
      if(qEulkZNqQU == NHumxcGsbT){qpQlRblLgi = true;}
      else if(NHumxcGsbT == qEulkZNqQU){zBYQXKOHPc = true;}
      if(QurhqlQFVY == KQhixTWlxe){MguBmDrcOt = true;}
      else if(KQhixTWlxe == QurhqlQFVY){LEBpOefBWw = true;}
      if(CGiYSCYhmq == COFuCiaKht){iZXmtwPMBX = true;}
      if(GEbglEMdnU == PSnPWbkVTz){gpmGrjhQQp = true;}
      if(eugekhSqZW == aQckpTIbHz){XwUKLspiSs = true;}
      while(COFuCiaKht == CGiYSCYhmq){NsxhjPDQtX = true;}
      while(PSnPWbkVTz == PSnPWbkVTz){AsElCEHcka = true;}
      while(aQckpTIbHz == aQckpTIbHz){pBjNPODhgz = true;}
      if(cFJkVyBzkN == true){cFJkVyBzkN = false;}
      if(CuLgErcRLV == true){CuLgErcRLV = false;}
      if(hASyDllTEi == true){hASyDllTEi = false;}
      if(ayChRKICtY == true){ayChRKICtY = false;}
      if(alkGweEugy == true){alkGweEugy = false;}
      if(qpQlRblLgi == true){qpQlRblLgi = false;}
      if(MguBmDrcOt == true){MguBmDrcOt = false;}
      if(iZXmtwPMBX == true){iZXmtwPMBX = false;}
      if(gpmGrjhQQp == true){gpmGrjhQQp = false;}
      if(XwUKLspiSs == true){XwUKLspiSs = false;}
      if(mbglPQRcrU == true){mbglPQRcrU = false;}
      if(LZKIBWtoJC == true){LZKIBWtoJC = false;}
      if(UbMYWlFNaB == true){UbMYWlFNaB = false;}
      if(FgsIkFJAqa == true){FgsIkFJAqa = false;}
      if(SFYudylLIo == true){SFYudylLIo = false;}
      if(zBYQXKOHPc == true){zBYQXKOHPc = false;}
      if(LEBpOefBWw == true){LEBpOefBWw = false;}
      if(NsxhjPDQtX == true){NsxhjPDQtX = false;}
      if(AsElCEHcka == true){AsElCEHcka = false;}
      if(pBjNPODhgz == true){pBjNPODhgz = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YYACDIQQTJ
{ 
  void LnuRxnXlWl()
  { 
      bool VPfySyxxNN = false;
      bool ozQxCilbhl = false;
      bool SdWkyOhnJS = false;
      bool ABcdmuAJRN = false;
      bool kEkLQTHrPE = false;
      bool NLagOrsuzE = false;
      bool QJmtMnVmKG = false;
      bool hcKTAOsKUd = false;
      bool GpXEFnZuPt = false;
      bool BFJMxaQpyb = false;
      bool xUgIWkBGfz = false;
      bool zhZUxZlioY = false;
      bool NaXPFbSxVY = false;
      bool xCMrglJOhT = false;
      bool lSPPuSenPX = false;
      bool ohAZNWwQDt = false;
      bool SEkGBwVhnK = false;
      bool QXAUluLIUl = false;
      bool wdgyjPBmeJ = false;
      bool orKmBghINy = false;
      string ecforeZGna;
      string TlRIIZBxxA;
      string RStsbNFrFN;
      string BgohdqUfaQ;
      string YpXTcJQRfo;
      string ongGjLilNd;
      string ZgBaZmwjeQ;
      string EVVfbbtZQR;
      string wmLPXMzixS;
      string IhRxqroxto;
      string MgmPjTSAeI;
      string QIVuglUkCM;
      string mNlLIWDXoj;
      string mwfaEOzweE;
      string tzDoBzLWQF;
      string WDugBzuaGN;
      string PtLuVTcupR;
      string lXFySnJwKJ;
      string LgHjMiBzYF;
      string SXoRnznITg;
      if(ecforeZGna == MgmPjTSAeI){VPfySyxxNN = true;}
      else if(MgmPjTSAeI == ecforeZGna){xUgIWkBGfz = true;}
      if(TlRIIZBxxA == QIVuglUkCM){ozQxCilbhl = true;}
      else if(QIVuglUkCM == TlRIIZBxxA){zhZUxZlioY = true;}
      if(RStsbNFrFN == mNlLIWDXoj){SdWkyOhnJS = true;}
      else if(mNlLIWDXoj == RStsbNFrFN){NaXPFbSxVY = true;}
      if(BgohdqUfaQ == mwfaEOzweE){ABcdmuAJRN = true;}
      else if(mwfaEOzweE == BgohdqUfaQ){xCMrglJOhT = true;}
      if(YpXTcJQRfo == tzDoBzLWQF){kEkLQTHrPE = true;}
      else if(tzDoBzLWQF == YpXTcJQRfo){lSPPuSenPX = true;}
      if(ongGjLilNd == WDugBzuaGN){NLagOrsuzE = true;}
      else if(WDugBzuaGN == ongGjLilNd){ohAZNWwQDt = true;}
      if(ZgBaZmwjeQ == PtLuVTcupR){QJmtMnVmKG = true;}
      else if(PtLuVTcupR == ZgBaZmwjeQ){SEkGBwVhnK = true;}
      if(EVVfbbtZQR == lXFySnJwKJ){hcKTAOsKUd = true;}
      if(wmLPXMzixS == LgHjMiBzYF){GpXEFnZuPt = true;}
      if(IhRxqroxto == SXoRnznITg){BFJMxaQpyb = true;}
      while(lXFySnJwKJ == EVVfbbtZQR){QXAUluLIUl = true;}
      while(LgHjMiBzYF == LgHjMiBzYF){wdgyjPBmeJ = true;}
      while(SXoRnznITg == SXoRnznITg){orKmBghINy = true;}
      if(VPfySyxxNN == true){VPfySyxxNN = false;}
      if(ozQxCilbhl == true){ozQxCilbhl = false;}
      if(SdWkyOhnJS == true){SdWkyOhnJS = false;}
      if(ABcdmuAJRN == true){ABcdmuAJRN = false;}
      if(kEkLQTHrPE == true){kEkLQTHrPE = false;}
      if(NLagOrsuzE == true){NLagOrsuzE = false;}
      if(QJmtMnVmKG == true){QJmtMnVmKG = false;}
      if(hcKTAOsKUd == true){hcKTAOsKUd = false;}
      if(GpXEFnZuPt == true){GpXEFnZuPt = false;}
      if(BFJMxaQpyb == true){BFJMxaQpyb = false;}
      if(xUgIWkBGfz == true){xUgIWkBGfz = false;}
      if(zhZUxZlioY == true){zhZUxZlioY = false;}
      if(NaXPFbSxVY == true){NaXPFbSxVY = false;}
      if(xCMrglJOhT == true){xCMrglJOhT = false;}
      if(lSPPuSenPX == true){lSPPuSenPX = false;}
      if(ohAZNWwQDt == true){ohAZNWwQDt = false;}
      if(SEkGBwVhnK == true){SEkGBwVhnK = false;}
      if(QXAUluLIUl == true){QXAUluLIUl = false;}
      if(wdgyjPBmeJ == true){wdgyjPBmeJ = false;}
      if(orKmBghINy == true){orKmBghINy = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RJJUVWQZHQ
{ 
  void rpCqRIEMiz()
  { 
      bool dzLIGpPjuY = false;
      bool yAhyXIOXGs = false;
      bool lHukbXNPat = false;
      bool FQUUfTDFdG = false;
      bool TbfUZOfXgN = false;
      bool foLQSVDZfh = false;
      bool czIASgeNSD = false;
      bool FJtzZJaaLN = false;
      bool rpiMOPFDCJ = false;
      bool QcYwdTTfBr = false;
      bool iSEcIVuLqI = false;
      bool ucOSplQulD = false;
      bool bbKImcjyVF = false;
      bool puYTeYkFEr = false;
      bool UsMsOmOGFr = false;
      bool DXsyPHHwpe = false;
      bool lxpkRzEunQ = false;
      bool AKqtiiOQsz = false;
      bool NdrdBSCZDW = false;
      bool QoOYFXCabP = false;
      string GlxMMaGlYa;
      string TxAltmaEnj;
      string oqhBwlOcWe;
      string zVpALNNDIM;
      string URUYgxwJOZ;
      string dkNqSOEwQZ;
      string MdOtayGzZg;
      string ZWubOWVJUy;
      string pFNxBNHzhw;
      string IQmizqDaxh;
      string pwjhAGpkbu;
      string subRjHPRrM;
      string pHRcXIiMcX;
      string TtLlHsUfwt;
      string XWWlDNoUyr;
      string akhjrieHJq;
      string WcIEbyioJp;
      string ljVyoTjeYK;
      string TONjxiMbtY;
      string ONWFWuNSKE;
      if(GlxMMaGlYa == pwjhAGpkbu){dzLIGpPjuY = true;}
      else if(pwjhAGpkbu == GlxMMaGlYa){iSEcIVuLqI = true;}
      if(TxAltmaEnj == subRjHPRrM){yAhyXIOXGs = true;}
      else if(subRjHPRrM == TxAltmaEnj){ucOSplQulD = true;}
      if(oqhBwlOcWe == pHRcXIiMcX){lHukbXNPat = true;}
      else if(pHRcXIiMcX == oqhBwlOcWe){bbKImcjyVF = true;}
      if(zVpALNNDIM == TtLlHsUfwt){FQUUfTDFdG = true;}
      else if(TtLlHsUfwt == zVpALNNDIM){puYTeYkFEr = true;}
      if(URUYgxwJOZ == XWWlDNoUyr){TbfUZOfXgN = true;}
      else if(XWWlDNoUyr == URUYgxwJOZ){UsMsOmOGFr = true;}
      if(dkNqSOEwQZ == akhjrieHJq){foLQSVDZfh = true;}
      else if(akhjrieHJq == dkNqSOEwQZ){DXsyPHHwpe = true;}
      if(MdOtayGzZg == WcIEbyioJp){czIASgeNSD = true;}
      else if(WcIEbyioJp == MdOtayGzZg){lxpkRzEunQ = true;}
      if(ZWubOWVJUy == ljVyoTjeYK){FJtzZJaaLN = true;}
      if(pFNxBNHzhw == TONjxiMbtY){rpiMOPFDCJ = true;}
      if(IQmizqDaxh == ONWFWuNSKE){QcYwdTTfBr = true;}
      while(ljVyoTjeYK == ZWubOWVJUy){AKqtiiOQsz = true;}
      while(TONjxiMbtY == TONjxiMbtY){NdrdBSCZDW = true;}
      while(ONWFWuNSKE == ONWFWuNSKE){QoOYFXCabP = true;}
      if(dzLIGpPjuY == true){dzLIGpPjuY = false;}
      if(yAhyXIOXGs == true){yAhyXIOXGs = false;}
      if(lHukbXNPat == true){lHukbXNPat = false;}
      if(FQUUfTDFdG == true){FQUUfTDFdG = false;}
      if(TbfUZOfXgN == true){TbfUZOfXgN = false;}
      if(foLQSVDZfh == true){foLQSVDZfh = false;}
      if(czIASgeNSD == true){czIASgeNSD = false;}
      if(FJtzZJaaLN == true){FJtzZJaaLN = false;}
      if(rpiMOPFDCJ == true){rpiMOPFDCJ = false;}
      if(QcYwdTTfBr == true){QcYwdTTfBr = false;}
      if(iSEcIVuLqI == true){iSEcIVuLqI = false;}
      if(ucOSplQulD == true){ucOSplQulD = false;}
      if(bbKImcjyVF == true){bbKImcjyVF = false;}
      if(puYTeYkFEr == true){puYTeYkFEr = false;}
      if(UsMsOmOGFr == true){UsMsOmOGFr = false;}
      if(DXsyPHHwpe == true){DXsyPHHwpe = false;}
      if(lxpkRzEunQ == true){lxpkRzEunQ = false;}
      if(AKqtiiOQsz == true){AKqtiiOQsz = false;}
      if(NdrdBSCZDW == true){NdrdBSCZDW = false;}
      if(QoOYFXCabP == true){QoOYFXCabP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CZWQWDRAXX
{ 
  void eGzcQUKllr()
  { 
      bool AbyNfVqrZA = false;
      bool PrkVVotQJB = false;
      bool GObzZYkBKp = false;
      bool xJOdsyNhhJ = false;
      bool PEeTftBLpg = false;
      bool cWYCpkAymH = false;
      bool kphiDFcHbW = false;
      bool aLizlCjITe = false;
      bool WZKcNZkMmH = false;
      bool yOtZYZVuQZ = false;
      bool QtiTFQnZPn = false;
      bool BreIkLeEVc = false;
      bool UQTUXMQIZL = false;
      bool fnqsObpqXw = false;
      bool lKwzFGjloN = false;
      bool tQZmFzaNus = false;
      bool zezDHNAlsI = false;
      bool UmgYqJTUOR = false;
      bool beTMRONEnO = false;
      bool BbwdxGLrqr = false;
      string nYubrkhBWf;
      string QJNFlrViQK;
      string uOKyzujtYm;
      string jELxAsUVeF;
      string oFOHCVHtVT;
      string HNxeMZNwcx;
      string BmcjuLxQEr;
      string WzDKVpzDwc;
      string noYFfWGgja;
      string eYshEBcdLq;
      string YqpcwHKlIW;
      string ZKAzILNWEY;
      string FQfcNyGUGT;
      string MVqFExzwJX;
      string LTXNBTXtYr;
      string nFYlkIKMOx;
      string BofoJDTuuL;
      string LRNgMkrSbs;
      string aLuOfHwETu;
      string lmgMWWnhDx;
      if(nYubrkhBWf == YqpcwHKlIW){AbyNfVqrZA = true;}
      else if(YqpcwHKlIW == nYubrkhBWf){QtiTFQnZPn = true;}
      if(QJNFlrViQK == ZKAzILNWEY){PrkVVotQJB = true;}
      else if(ZKAzILNWEY == QJNFlrViQK){BreIkLeEVc = true;}
      if(uOKyzujtYm == FQfcNyGUGT){GObzZYkBKp = true;}
      else if(FQfcNyGUGT == uOKyzujtYm){UQTUXMQIZL = true;}
      if(jELxAsUVeF == MVqFExzwJX){xJOdsyNhhJ = true;}
      else if(MVqFExzwJX == jELxAsUVeF){fnqsObpqXw = true;}
      if(oFOHCVHtVT == LTXNBTXtYr){PEeTftBLpg = true;}
      else if(LTXNBTXtYr == oFOHCVHtVT){lKwzFGjloN = true;}
      if(HNxeMZNwcx == nFYlkIKMOx){cWYCpkAymH = true;}
      else if(nFYlkIKMOx == HNxeMZNwcx){tQZmFzaNus = true;}
      if(BmcjuLxQEr == BofoJDTuuL){kphiDFcHbW = true;}
      else if(BofoJDTuuL == BmcjuLxQEr){zezDHNAlsI = true;}
      if(WzDKVpzDwc == LRNgMkrSbs){aLizlCjITe = true;}
      if(noYFfWGgja == aLuOfHwETu){WZKcNZkMmH = true;}
      if(eYshEBcdLq == lmgMWWnhDx){yOtZYZVuQZ = true;}
      while(LRNgMkrSbs == WzDKVpzDwc){UmgYqJTUOR = true;}
      while(aLuOfHwETu == aLuOfHwETu){beTMRONEnO = true;}
      while(lmgMWWnhDx == lmgMWWnhDx){BbwdxGLrqr = true;}
      if(AbyNfVqrZA == true){AbyNfVqrZA = false;}
      if(PrkVVotQJB == true){PrkVVotQJB = false;}
      if(GObzZYkBKp == true){GObzZYkBKp = false;}
      if(xJOdsyNhhJ == true){xJOdsyNhhJ = false;}
      if(PEeTftBLpg == true){PEeTftBLpg = false;}
      if(cWYCpkAymH == true){cWYCpkAymH = false;}
      if(kphiDFcHbW == true){kphiDFcHbW = false;}
      if(aLizlCjITe == true){aLizlCjITe = false;}
      if(WZKcNZkMmH == true){WZKcNZkMmH = false;}
      if(yOtZYZVuQZ == true){yOtZYZVuQZ = false;}
      if(QtiTFQnZPn == true){QtiTFQnZPn = false;}
      if(BreIkLeEVc == true){BreIkLeEVc = false;}
      if(UQTUXMQIZL == true){UQTUXMQIZL = false;}
      if(fnqsObpqXw == true){fnqsObpqXw = false;}
      if(lKwzFGjloN == true){lKwzFGjloN = false;}
      if(tQZmFzaNus == true){tQZmFzaNus = false;}
      if(zezDHNAlsI == true){zezDHNAlsI = false;}
      if(UmgYqJTUOR == true){UmgYqJTUOR = false;}
      if(beTMRONEnO == true){beTMRONEnO = false;}
      if(BbwdxGLrqr == true){BbwdxGLrqr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NQGYEOWWSX
{ 
  void zNhEjsWDCT()
  { 
      bool gNzAraeNQX = false;
      bool bwiitGpzck = false;
      bool UDDetWwPPu = false;
      bool tIKTCXVWby = false;
      bool RhCnkcYfcH = false;
      bool nsweQcKTGV = false;
      bool dPxUcTkOgW = false;
      bool jSWdTmAOtQ = false;
      bool xPlIHALeDZ = false;
      bool MCQGTCjhKR = false;
      bool qKzGWVWRLd = false;
      bool buZoXOcoNb = false;
      bool HFLHJhBQKR = false;
      bool CSEcZmQfll = false;
      bool dsTYxRcbhF = false;
      bool YuUhJkzrwo = false;
      bool TFHtaJqwOk = false;
      bool xwCbyOllJr = false;
      bool EZiErHyVTn = false;
      bool mYjwRYkYxn = false;
      string YtFVXlGQxt;
      string JsxkNXrWQG;
      string hQrdMNNWYw;
      string dgtaRnrTuX;
      string PEOYqTTRgZ;
      string OgFEcIBaJK;
      string XiqBPczWSZ;
      string eXbrqdBTmr;
      string tuoygxaoxJ;
      string lQJjzfMMJO;
      string UwyGtpFMNz;
      string GhnQQbtrUS;
      string BJYJnOOqrL;
      string QoKYXxxhEp;
      string rNbyDEcrTX;
      string lTrqMKXoSf;
      string bNUChRWPsT;
      string eLndZnhaGi;
      string kVXUYxwTMN;
      string YHPXNdsZPl;
      if(YtFVXlGQxt == UwyGtpFMNz){gNzAraeNQX = true;}
      else if(UwyGtpFMNz == YtFVXlGQxt){qKzGWVWRLd = true;}
      if(JsxkNXrWQG == GhnQQbtrUS){bwiitGpzck = true;}
      else if(GhnQQbtrUS == JsxkNXrWQG){buZoXOcoNb = true;}
      if(hQrdMNNWYw == BJYJnOOqrL){UDDetWwPPu = true;}
      else if(BJYJnOOqrL == hQrdMNNWYw){HFLHJhBQKR = true;}
      if(dgtaRnrTuX == QoKYXxxhEp){tIKTCXVWby = true;}
      else if(QoKYXxxhEp == dgtaRnrTuX){CSEcZmQfll = true;}
      if(PEOYqTTRgZ == rNbyDEcrTX){RhCnkcYfcH = true;}
      else if(rNbyDEcrTX == PEOYqTTRgZ){dsTYxRcbhF = true;}
      if(OgFEcIBaJK == lTrqMKXoSf){nsweQcKTGV = true;}
      else if(lTrqMKXoSf == OgFEcIBaJK){YuUhJkzrwo = true;}
      if(XiqBPczWSZ == bNUChRWPsT){dPxUcTkOgW = true;}
      else if(bNUChRWPsT == XiqBPczWSZ){TFHtaJqwOk = true;}
      if(eXbrqdBTmr == eLndZnhaGi){jSWdTmAOtQ = true;}
      if(tuoygxaoxJ == kVXUYxwTMN){xPlIHALeDZ = true;}
      if(lQJjzfMMJO == YHPXNdsZPl){MCQGTCjhKR = true;}
      while(eLndZnhaGi == eXbrqdBTmr){xwCbyOllJr = true;}
      while(kVXUYxwTMN == kVXUYxwTMN){EZiErHyVTn = true;}
      while(YHPXNdsZPl == YHPXNdsZPl){mYjwRYkYxn = true;}
      if(gNzAraeNQX == true){gNzAraeNQX = false;}
      if(bwiitGpzck == true){bwiitGpzck = false;}
      if(UDDetWwPPu == true){UDDetWwPPu = false;}
      if(tIKTCXVWby == true){tIKTCXVWby = false;}
      if(RhCnkcYfcH == true){RhCnkcYfcH = false;}
      if(nsweQcKTGV == true){nsweQcKTGV = false;}
      if(dPxUcTkOgW == true){dPxUcTkOgW = false;}
      if(jSWdTmAOtQ == true){jSWdTmAOtQ = false;}
      if(xPlIHALeDZ == true){xPlIHALeDZ = false;}
      if(MCQGTCjhKR == true){MCQGTCjhKR = false;}
      if(qKzGWVWRLd == true){qKzGWVWRLd = false;}
      if(buZoXOcoNb == true){buZoXOcoNb = false;}
      if(HFLHJhBQKR == true){HFLHJhBQKR = false;}
      if(CSEcZmQfll == true){CSEcZmQfll = false;}
      if(dsTYxRcbhF == true){dsTYxRcbhF = false;}
      if(YuUhJkzrwo == true){YuUhJkzrwo = false;}
      if(TFHtaJqwOk == true){TFHtaJqwOk = false;}
      if(xwCbyOllJr == true){xwCbyOllJr = false;}
      if(EZiErHyVTn == true){EZiErHyVTn = false;}
      if(mYjwRYkYxn == true){mYjwRYkYxn = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ITMVXRAATK
{ 
  void OmNZzXKewW()
  { 
      bool yracmzrEBa = false;
      bool CcNopKjqhm = false;
      bool mOQINZpOAe = false;
      bool ljHcXXGhJS = false;
      bool MZQdOSJhaQ = false;
      bool zQCKPiaSPN = false;
      bool JdEVoSVkQR = false;
      bool DChPZMxmst = false;
      bool PHnifUgxMn = false;
      bool wnRwIiVuow = false;
      bool xYQINwimjC = false;
      bool lnYUxCoUoT = false;
      bool kXXcUOjkGp = false;
      bool pCyxxppeVj = false;
      bool ltNKCIPxyG = false;
      bool sBFULYAnmy = false;
      bool LxfsLipbXH = false;
      bool LKzFhpKNZM = false;
      bool zmlAxowckX = false;
      bool oCrWYnOTyS = false;
      string cbwpxrxmKG;
      string hEwUydoBOq;
      string qJTMMxNEcS;
      string oWjnSCrraf;
      string ClUbYwmyIV;
      string XRETaSHidz;
      string erKrRKUICw;
      string lwZhoEnrnP;
      string KOwHuWSPMz;
      string tIObAJacKN;
      string bewMjarVeV;
      string dPuNoQwDwt;
      string rEuCBchxge;
      string dLhZneSiXT;
      string exjpVeMAmj;
      string MXgPVWitLG;
      string runArSgUZJ;
      string xNfbOtUFfV;
      string PhKwRFeREQ;
      string wXXQbiIjxD;
      if(cbwpxrxmKG == bewMjarVeV){yracmzrEBa = true;}
      else if(bewMjarVeV == cbwpxrxmKG){xYQINwimjC = true;}
      if(hEwUydoBOq == dPuNoQwDwt){CcNopKjqhm = true;}
      else if(dPuNoQwDwt == hEwUydoBOq){lnYUxCoUoT = true;}
      if(qJTMMxNEcS == rEuCBchxge){mOQINZpOAe = true;}
      else if(rEuCBchxge == qJTMMxNEcS){kXXcUOjkGp = true;}
      if(oWjnSCrraf == dLhZneSiXT){ljHcXXGhJS = true;}
      else if(dLhZneSiXT == oWjnSCrraf){pCyxxppeVj = true;}
      if(ClUbYwmyIV == exjpVeMAmj){MZQdOSJhaQ = true;}
      else if(exjpVeMAmj == ClUbYwmyIV){ltNKCIPxyG = true;}
      if(XRETaSHidz == MXgPVWitLG){zQCKPiaSPN = true;}
      else if(MXgPVWitLG == XRETaSHidz){sBFULYAnmy = true;}
      if(erKrRKUICw == runArSgUZJ){JdEVoSVkQR = true;}
      else if(runArSgUZJ == erKrRKUICw){LxfsLipbXH = true;}
      if(lwZhoEnrnP == xNfbOtUFfV){DChPZMxmst = true;}
      if(KOwHuWSPMz == PhKwRFeREQ){PHnifUgxMn = true;}
      if(tIObAJacKN == wXXQbiIjxD){wnRwIiVuow = true;}
      while(xNfbOtUFfV == lwZhoEnrnP){LKzFhpKNZM = true;}
      while(PhKwRFeREQ == PhKwRFeREQ){zmlAxowckX = true;}
      while(wXXQbiIjxD == wXXQbiIjxD){oCrWYnOTyS = true;}
      if(yracmzrEBa == true){yracmzrEBa = false;}
      if(CcNopKjqhm == true){CcNopKjqhm = false;}
      if(mOQINZpOAe == true){mOQINZpOAe = false;}
      if(ljHcXXGhJS == true){ljHcXXGhJS = false;}
      if(MZQdOSJhaQ == true){MZQdOSJhaQ = false;}
      if(zQCKPiaSPN == true){zQCKPiaSPN = false;}
      if(JdEVoSVkQR == true){JdEVoSVkQR = false;}
      if(DChPZMxmst == true){DChPZMxmst = false;}
      if(PHnifUgxMn == true){PHnifUgxMn = false;}
      if(wnRwIiVuow == true){wnRwIiVuow = false;}
      if(xYQINwimjC == true){xYQINwimjC = false;}
      if(lnYUxCoUoT == true){lnYUxCoUoT = false;}
      if(kXXcUOjkGp == true){kXXcUOjkGp = false;}
      if(pCyxxppeVj == true){pCyxxppeVj = false;}
      if(ltNKCIPxyG == true){ltNKCIPxyG = false;}
      if(sBFULYAnmy == true){sBFULYAnmy = false;}
      if(LxfsLipbXH == true){LxfsLipbXH = false;}
      if(LKzFhpKNZM == true){LKzFhpKNZM = false;}
      if(zmlAxowckX == true){zmlAxowckX = false;}
      if(oCrWYnOTyS == true){oCrWYnOTyS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DCTEIIJBNT
{ 
  void rREKrQIFQt()
  { 
      bool laNYPZPiVp = false;
      bool bmgwasIuSX = false;
      bool GzdkEprpFO = false;
      bool zERlcyXESC = false;
      bool pHjteKAJAj = false;
      bool BsmxPmwbUY = false;
      bool gZcGpXVtSc = false;
      bool UEbPgzPiGs = false;
      bool oipUMGQUWp = false;
      bool BwjsVQdcDH = false;
      bool oGpXPxQWPP = false;
      bool WMVajyiItj = false;
      bool GBXcaYUitQ = false;
      bool VmezEirSFP = false;
      bool psEGSoaIJX = false;
      bool gVhTAnhNwp = false;
      bool bgcztjgVWB = false;
      bool FQCesMwQJl = false;
      bool yLYWizELyP = false;
      bool AeVOmIQFMi = false;
      string VIqUUoGZGz;
      string zkNtQiEZZd;
      string byoLsHFFoj;
      string oZeFuOzUQT;
      string oLfZLCVQUy;
      string FBGijWTebN;
      string jAjVJZGHzC;
      string ijJFnQRPAp;
      string lnjqenSuUX;
      string uUrDinbJPk;
      string wNQSOVrAuT;
      string EEelauPsqV;
      string GKmaGOqFVA;
      string oIzPsXXRbI;
      string wSAASqpFbm;
      string XhRtdlZoLA;
      string eldtewEOrC;
      string XyRiYnJmIP;
      string YkchbGpFkH;
      string yhFadKnFUV;
      if(VIqUUoGZGz == wNQSOVrAuT){laNYPZPiVp = true;}
      else if(wNQSOVrAuT == VIqUUoGZGz){oGpXPxQWPP = true;}
      if(zkNtQiEZZd == EEelauPsqV){bmgwasIuSX = true;}
      else if(EEelauPsqV == zkNtQiEZZd){WMVajyiItj = true;}
      if(byoLsHFFoj == GKmaGOqFVA){GzdkEprpFO = true;}
      else if(GKmaGOqFVA == byoLsHFFoj){GBXcaYUitQ = true;}
      if(oZeFuOzUQT == oIzPsXXRbI){zERlcyXESC = true;}
      else if(oIzPsXXRbI == oZeFuOzUQT){VmezEirSFP = true;}
      if(oLfZLCVQUy == wSAASqpFbm){pHjteKAJAj = true;}
      else if(wSAASqpFbm == oLfZLCVQUy){psEGSoaIJX = true;}
      if(FBGijWTebN == XhRtdlZoLA){BsmxPmwbUY = true;}
      else if(XhRtdlZoLA == FBGijWTebN){gVhTAnhNwp = true;}
      if(jAjVJZGHzC == eldtewEOrC){gZcGpXVtSc = true;}
      else if(eldtewEOrC == jAjVJZGHzC){bgcztjgVWB = true;}
      if(ijJFnQRPAp == XyRiYnJmIP){UEbPgzPiGs = true;}
      if(lnjqenSuUX == YkchbGpFkH){oipUMGQUWp = true;}
      if(uUrDinbJPk == yhFadKnFUV){BwjsVQdcDH = true;}
      while(XyRiYnJmIP == ijJFnQRPAp){FQCesMwQJl = true;}
      while(YkchbGpFkH == YkchbGpFkH){yLYWizELyP = true;}
      while(yhFadKnFUV == yhFadKnFUV){AeVOmIQFMi = true;}
      if(laNYPZPiVp == true){laNYPZPiVp = false;}
      if(bmgwasIuSX == true){bmgwasIuSX = false;}
      if(GzdkEprpFO == true){GzdkEprpFO = false;}
      if(zERlcyXESC == true){zERlcyXESC = false;}
      if(pHjteKAJAj == true){pHjteKAJAj = false;}
      if(BsmxPmwbUY == true){BsmxPmwbUY = false;}
      if(gZcGpXVtSc == true){gZcGpXVtSc = false;}
      if(UEbPgzPiGs == true){UEbPgzPiGs = false;}
      if(oipUMGQUWp == true){oipUMGQUWp = false;}
      if(BwjsVQdcDH == true){BwjsVQdcDH = false;}
      if(oGpXPxQWPP == true){oGpXPxQWPP = false;}
      if(WMVajyiItj == true){WMVajyiItj = false;}
      if(GBXcaYUitQ == true){GBXcaYUitQ = false;}
      if(VmezEirSFP == true){VmezEirSFP = false;}
      if(psEGSoaIJX == true){psEGSoaIJX = false;}
      if(gVhTAnhNwp == true){gVhTAnhNwp = false;}
      if(bgcztjgVWB == true){bgcztjgVWB = false;}
      if(FQCesMwQJl == true){FQCesMwQJl = false;}
      if(yLYWizELyP == true){yLYWizELyP = false;}
      if(AeVOmIQFMi == true){AeVOmIQFMi = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CJPHADSOPG
{ 
  void nhBSRtSeHw()
  { 
      bool LZmnGDcZJW = false;
      bool LZaYDxwaYs = false;
      bool bOoLLLocDE = false;
      bool otQflxsISM = false;
      bool jPJJXwMgEU = false;
      bool ePcKlwkbct = false;
      bool OAVEgQIulF = false;
      bool EmHMJLFemO = false;
      bool zIYhZZRGsp = false;
      bool MQfYxisEbH = false;
      bool mRtyQrHPAZ = false;
      bool pFoNuFbmeH = false;
      bool yCBwsmHiOZ = false;
      bool RADfagNZRh = false;
      bool RBFSzHbRcr = false;
      bool HGZTIkPyFc = false;
      bool jYBZIpCImN = false;
      bool qBVxpwwhwl = false;
      bool LpFVRLaiXp = false;
      bool QPUnxsAfKJ = false;
      string hRXdoPordP;
      string lKRtCUWEWP;
      string CcZQIuDcpS;
      string hweqmbEoPa;
      string gnaZMYCAnK;
      string bBalxFqGVs;
      string LLCKhpeZGa;
      string QhouiOCAEj;
      string sCoeqgUsth;
      string jxTVPUlPqW;
      string sbsNtsLeCi;
      string DqCkzIWWTB;
      string UlfYpSOSfO;
      string blIhpRIbkx;
      string UopKesthVG;
      string yPwpBHuUWb;
      string dsznGQhUth;
      string AtlWgewUsx;
      string XgwYPMoSfi;
      string WnFISbpBEN;
      if(hRXdoPordP == sbsNtsLeCi){LZmnGDcZJW = true;}
      else if(sbsNtsLeCi == hRXdoPordP){mRtyQrHPAZ = true;}
      if(lKRtCUWEWP == DqCkzIWWTB){LZaYDxwaYs = true;}
      else if(DqCkzIWWTB == lKRtCUWEWP){pFoNuFbmeH = true;}
      if(CcZQIuDcpS == UlfYpSOSfO){bOoLLLocDE = true;}
      else if(UlfYpSOSfO == CcZQIuDcpS){yCBwsmHiOZ = true;}
      if(hweqmbEoPa == blIhpRIbkx){otQflxsISM = true;}
      else if(blIhpRIbkx == hweqmbEoPa){RADfagNZRh = true;}
      if(gnaZMYCAnK == UopKesthVG){jPJJXwMgEU = true;}
      else if(UopKesthVG == gnaZMYCAnK){RBFSzHbRcr = true;}
      if(bBalxFqGVs == yPwpBHuUWb){ePcKlwkbct = true;}
      else if(yPwpBHuUWb == bBalxFqGVs){HGZTIkPyFc = true;}
      if(LLCKhpeZGa == dsznGQhUth){OAVEgQIulF = true;}
      else if(dsznGQhUth == LLCKhpeZGa){jYBZIpCImN = true;}
      if(QhouiOCAEj == AtlWgewUsx){EmHMJLFemO = true;}
      if(sCoeqgUsth == XgwYPMoSfi){zIYhZZRGsp = true;}
      if(jxTVPUlPqW == WnFISbpBEN){MQfYxisEbH = true;}
      while(AtlWgewUsx == QhouiOCAEj){qBVxpwwhwl = true;}
      while(XgwYPMoSfi == XgwYPMoSfi){LpFVRLaiXp = true;}
      while(WnFISbpBEN == WnFISbpBEN){QPUnxsAfKJ = true;}
      if(LZmnGDcZJW == true){LZmnGDcZJW = false;}
      if(LZaYDxwaYs == true){LZaYDxwaYs = false;}
      if(bOoLLLocDE == true){bOoLLLocDE = false;}
      if(otQflxsISM == true){otQflxsISM = false;}
      if(jPJJXwMgEU == true){jPJJXwMgEU = false;}
      if(ePcKlwkbct == true){ePcKlwkbct = false;}
      if(OAVEgQIulF == true){OAVEgQIulF = false;}
      if(EmHMJLFemO == true){EmHMJLFemO = false;}
      if(zIYhZZRGsp == true){zIYhZZRGsp = false;}
      if(MQfYxisEbH == true){MQfYxisEbH = false;}
      if(mRtyQrHPAZ == true){mRtyQrHPAZ = false;}
      if(pFoNuFbmeH == true){pFoNuFbmeH = false;}
      if(yCBwsmHiOZ == true){yCBwsmHiOZ = false;}
      if(RADfagNZRh == true){RADfagNZRh = false;}
      if(RBFSzHbRcr == true){RBFSzHbRcr = false;}
      if(HGZTIkPyFc == true){HGZTIkPyFc = false;}
      if(jYBZIpCImN == true){jYBZIpCImN = false;}
      if(qBVxpwwhwl == true){qBVxpwwhwl = false;}
      if(LpFVRLaiXp == true){LpFVRLaiXp = false;}
      if(QPUnxsAfKJ == true){QPUnxsAfKJ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class EZTIBQICMB
{ 
  void GjCYDWIoCF()
  { 
      bool WqrJcGYopj = false;
      bool BFPxmMNyoS = false;
      bool JCYcnbpVEU = false;
      bool NzDWQfSOsw = false;
      bool UcLQbIllLp = false;
      bool bFomrURPUA = false;
      bool QDkUWCCrfG = false;
      bool syErxfpIAf = false;
      bool oDJOsIJPew = false;
      bool ODsreRrqtZ = false;
      bool GtFFwcAwpM = false;
      bool eJPoLNtWaO = false;
      bool rmNjrpFMMe = false;
      bool nzNZYuxDRQ = false;
      bool HNZSREnDgR = false;
      bool XVPfhTfVMG = false;
      bool XnjKXfuoPP = false;
      bool PpEKGeMuSm = false;
      bool CBetHSihrF = false;
      bool PbpLsTdOtk = false;
      string zBhOKQmmLk;
      string znafZKmfyH;
      string ZPGGOHGVyS;
      string CiofFbXOyn;
      string tNWTJblUzm;
      string IEJpMjtaAD;
      string JRoriPyuZx;
      string lYgJMMrmxl;
      string UlWKKyYzSV;
      string hwNuebXdks;
      string dqtBAiXzcA;
      string qGdOhWuehj;
      string qzbEPpKlej;
      string nOCDIdrUJa;
      string aYVSeaRzNH;
      string gswACdOokK;
      string YkUrMbEaGD;
      string eZUGJRcJZn;
      string XLXWDtSCBd;
      string OCjzCoDdFC;
      if(zBhOKQmmLk == dqtBAiXzcA){WqrJcGYopj = true;}
      else if(dqtBAiXzcA == zBhOKQmmLk){GtFFwcAwpM = true;}
      if(znafZKmfyH == qGdOhWuehj){BFPxmMNyoS = true;}
      else if(qGdOhWuehj == znafZKmfyH){eJPoLNtWaO = true;}
      if(ZPGGOHGVyS == qzbEPpKlej){JCYcnbpVEU = true;}
      else if(qzbEPpKlej == ZPGGOHGVyS){rmNjrpFMMe = true;}
      if(CiofFbXOyn == nOCDIdrUJa){NzDWQfSOsw = true;}
      else if(nOCDIdrUJa == CiofFbXOyn){nzNZYuxDRQ = true;}
      if(tNWTJblUzm == aYVSeaRzNH){UcLQbIllLp = true;}
      else if(aYVSeaRzNH == tNWTJblUzm){HNZSREnDgR = true;}
      if(IEJpMjtaAD == gswACdOokK){bFomrURPUA = true;}
      else if(gswACdOokK == IEJpMjtaAD){XVPfhTfVMG = true;}
      if(JRoriPyuZx == YkUrMbEaGD){QDkUWCCrfG = true;}
      else if(YkUrMbEaGD == JRoriPyuZx){XnjKXfuoPP = true;}
      if(lYgJMMrmxl == eZUGJRcJZn){syErxfpIAf = true;}
      if(UlWKKyYzSV == XLXWDtSCBd){oDJOsIJPew = true;}
      if(hwNuebXdks == OCjzCoDdFC){ODsreRrqtZ = true;}
      while(eZUGJRcJZn == lYgJMMrmxl){PpEKGeMuSm = true;}
      while(XLXWDtSCBd == XLXWDtSCBd){CBetHSihrF = true;}
      while(OCjzCoDdFC == OCjzCoDdFC){PbpLsTdOtk = true;}
      if(WqrJcGYopj == true){WqrJcGYopj = false;}
      if(BFPxmMNyoS == true){BFPxmMNyoS = false;}
      if(JCYcnbpVEU == true){JCYcnbpVEU = false;}
      if(NzDWQfSOsw == true){NzDWQfSOsw = false;}
      if(UcLQbIllLp == true){UcLQbIllLp = false;}
      if(bFomrURPUA == true){bFomrURPUA = false;}
      if(QDkUWCCrfG == true){QDkUWCCrfG = false;}
      if(syErxfpIAf == true){syErxfpIAf = false;}
      if(oDJOsIJPew == true){oDJOsIJPew = false;}
      if(ODsreRrqtZ == true){ODsreRrqtZ = false;}
      if(GtFFwcAwpM == true){GtFFwcAwpM = false;}
      if(eJPoLNtWaO == true){eJPoLNtWaO = false;}
      if(rmNjrpFMMe == true){rmNjrpFMMe = false;}
      if(nzNZYuxDRQ == true){nzNZYuxDRQ = false;}
      if(HNZSREnDgR == true){HNZSREnDgR = false;}
      if(XVPfhTfVMG == true){XVPfhTfVMG = false;}
      if(XnjKXfuoPP == true){XnjKXfuoPP = false;}
      if(PpEKGeMuSm == true){PpEKGeMuSm = false;}
      if(CBetHSihrF == true){CBetHSihrF = false;}
      if(PbpLsTdOtk == true){PbpLsTdOtk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VSIBYBCTPQ
{ 
  void XbEFAzfBFa()
  { 
      bool ruhqneDoUe = false;
      bool fCtacqChtc = false;
      bool dDKarzWTUp = false;
      bool GmXzbNyTnD = false;
      bool JLOSppclGZ = false;
      bool khJEiojGzn = false;
      bool IiUdXrdtyP = false;
      bool rhtpSSorjB = false;
      bool VjMuEYdTQm = false;
      bool cqUzuapjty = false;
      bool LhmqgDnSlP = false;
      bool zrWxRXVZYz = false;
      bool OKdLlANttr = false;
      bool cExlpnwVNB = false;
      bool jKgAmkUXhX = false;
      bool EQKtjBEbFf = false;
      bool osXsmttkHd = false;
      bool aALXPDccXA = false;
      bool fpMWnMtCBn = false;
      bool iTPhLqlnNr = false;
      string MLrqyLorji;
      string sOApgEfcMB;
      string cbccMdNwwl;
      string AWjqjhczfL;
      string ybyOZYtTsa;
      string OyycKjiOdU;
      string xsuxiWeJFz;
      string bfxBCAcNDm;
      string jtCVWCXHgz;
      string zOULOFgBfC;
      string tjNtziAxZJ;
      string HUKpjDqZZf;
      string zXcFzMKlmx;
      string TbhBszfHVn;
      string BagWreEeaK;
      string OGazDWHHul;
      string XNJcoelIxj;
      string WiDkeXqFNU;
      string IfLbiMAiyV;
      string rXMjQnQDgN;
      if(MLrqyLorji == tjNtziAxZJ){ruhqneDoUe = true;}
      else if(tjNtziAxZJ == MLrqyLorji){LhmqgDnSlP = true;}
      if(sOApgEfcMB == HUKpjDqZZf){fCtacqChtc = true;}
      else if(HUKpjDqZZf == sOApgEfcMB){zrWxRXVZYz = true;}
      if(cbccMdNwwl == zXcFzMKlmx){dDKarzWTUp = true;}
      else if(zXcFzMKlmx == cbccMdNwwl){OKdLlANttr = true;}
      if(AWjqjhczfL == TbhBszfHVn){GmXzbNyTnD = true;}
      else if(TbhBszfHVn == AWjqjhczfL){cExlpnwVNB = true;}
      if(ybyOZYtTsa == BagWreEeaK){JLOSppclGZ = true;}
      else if(BagWreEeaK == ybyOZYtTsa){jKgAmkUXhX = true;}
      if(OyycKjiOdU == OGazDWHHul){khJEiojGzn = true;}
      else if(OGazDWHHul == OyycKjiOdU){EQKtjBEbFf = true;}
      if(xsuxiWeJFz == XNJcoelIxj){IiUdXrdtyP = true;}
      else if(XNJcoelIxj == xsuxiWeJFz){osXsmttkHd = true;}
      if(bfxBCAcNDm == WiDkeXqFNU){rhtpSSorjB = true;}
      if(jtCVWCXHgz == IfLbiMAiyV){VjMuEYdTQm = true;}
      if(zOULOFgBfC == rXMjQnQDgN){cqUzuapjty = true;}
      while(WiDkeXqFNU == bfxBCAcNDm){aALXPDccXA = true;}
      while(IfLbiMAiyV == IfLbiMAiyV){fpMWnMtCBn = true;}
      while(rXMjQnQDgN == rXMjQnQDgN){iTPhLqlnNr = true;}
      if(ruhqneDoUe == true){ruhqneDoUe = false;}
      if(fCtacqChtc == true){fCtacqChtc = false;}
      if(dDKarzWTUp == true){dDKarzWTUp = false;}
      if(GmXzbNyTnD == true){GmXzbNyTnD = false;}
      if(JLOSppclGZ == true){JLOSppclGZ = false;}
      if(khJEiojGzn == true){khJEiojGzn = false;}
      if(IiUdXrdtyP == true){IiUdXrdtyP = false;}
      if(rhtpSSorjB == true){rhtpSSorjB = false;}
      if(VjMuEYdTQm == true){VjMuEYdTQm = false;}
      if(cqUzuapjty == true){cqUzuapjty = false;}
      if(LhmqgDnSlP == true){LhmqgDnSlP = false;}
      if(zrWxRXVZYz == true){zrWxRXVZYz = false;}
      if(OKdLlANttr == true){OKdLlANttr = false;}
      if(cExlpnwVNB == true){cExlpnwVNB = false;}
      if(jKgAmkUXhX == true){jKgAmkUXhX = false;}
      if(EQKtjBEbFf == true){EQKtjBEbFf = false;}
      if(osXsmttkHd == true){osXsmttkHd = false;}
      if(aALXPDccXA == true){aALXPDccXA = false;}
      if(fpMWnMtCBn == true){fpMWnMtCBn = false;}
      if(iTPhLqlnNr == true){iTPhLqlnNr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CFBCYFCCHW
{ 
  void IzWLRrepIO()
  { 
      bool VRmoHpzhSI = false;
      bool IGwRBStqkb = false;
      bool upWusGfTlN = false;
      bool TCZmJsIYHm = false;
      bool GjrCckgpRc = false;
      bool IpRTUftrwB = false;
      bool JfuEnGgJHI = false;
      bool KUjxULyURH = false;
      bool ZoBIFdjGSd = false;
      bool yPcGcanmNV = false;
      bool EXiISnGUyI = false;
      bool gZZnbTdmzK = false;
      bool WaqdKurcTs = false;
      bool XKGuDBGrpQ = false;
      bool WPHJwXoneJ = false;
      bool wBnCzcgEyW = false;
      bool ucAFkVSekZ = false;
      bool MkwhmGTggl = false;
      bool FDyKwVWDuZ = false;
      bool UCtSVtOCXZ = false;
      string cfEtcMgUZh;
      string eBlNEChKpH;
      string goVYdZpgun;
      string niqhKejQkb;
      string ZfwJztehZp;
      string wZdEcmJePC;
      string VAYlswJECY;
      string RQYNhAsMZj;
      string uIGFqNEmFW;
      string VFAhFYGKQC;
      string jaeAYeAPWb;
      string txFfGNtTte;
      string qVyeDLPqHI;
      string shwWzUcFfL;
      string NoQccXATbr;
      string emQkKNbzxM;
      string OJHNhMBhgl;
      string jomiRCFUCf;
      string KwOPcGRKLk;
      string rdITwDhCuP;
      if(cfEtcMgUZh == jaeAYeAPWb){VRmoHpzhSI = true;}
      else if(jaeAYeAPWb == cfEtcMgUZh){EXiISnGUyI = true;}
      if(eBlNEChKpH == txFfGNtTte){IGwRBStqkb = true;}
      else if(txFfGNtTte == eBlNEChKpH){gZZnbTdmzK = true;}
      if(goVYdZpgun == qVyeDLPqHI){upWusGfTlN = true;}
      else if(qVyeDLPqHI == goVYdZpgun){WaqdKurcTs = true;}
      if(niqhKejQkb == shwWzUcFfL){TCZmJsIYHm = true;}
      else if(shwWzUcFfL == niqhKejQkb){XKGuDBGrpQ = true;}
      if(ZfwJztehZp == NoQccXATbr){GjrCckgpRc = true;}
      else if(NoQccXATbr == ZfwJztehZp){WPHJwXoneJ = true;}
      if(wZdEcmJePC == emQkKNbzxM){IpRTUftrwB = true;}
      else if(emQkKNbzxM == wZdEcmJePC){wBnCzcgEyW = true;}
      if(VAYlswJECY == OJHNhMBhgl){JfuEnGgJHI = true;}
      else if(OJHNhMBhgl == VAYlswJECY){ucAFkVSekZ = true;}
      if(RQYNhAsMZj == jomiRCFUCf){KUjxULyURH = true;}
      if(uIGFqNEmFW == KwOPcGRKLk){ZoBIFdjGSd = true;}
      if(VFAhFYGKQC == rdITwDhCuP){yPcGcanmNV = true;}
      while(jomiRCFUCf == RQYNhAsMZj){MkwhmGTggl = true;}
      while(KwOPcGRKLk == KwOPcGRKLk){FDyKwVWDuZ = true;}
      while(rdITwDhCuP == rdITwDhCuP){UCtSVtOCXZ = true;}
      if(VRmoHpzhSI == true){VRmoHpzhSI = false;}
      if(IGwRBStqkb == true){IGwRBStqkb = false;}
      if(upWusGfTlN == true){upWusGfTlN = false;}
      if(TCZmJsIYHm == true){TCZmJsIYHm = false;}
      if(GjrCckgpRc == true){GjrCckgpRc = false;}
      if(IpRTUftrwB == true){IpRTUftrwB = false;}
      if(JfuEnGgJHI == true){JfuEnGgJHI = false;}
      if(KUjxULyURH == true){KUjxULyURH = false;}
      if(ZoBIFdjGSd == true){ZoBIFdjGSd = false;}
      if(yPcGcanmNV == true){yPcGcanmNV = false;}
      if(EXiISnGUyI == true){EXiISnGUyI = false;}
      if(gZZnbTdmzK == true){gZZnbTdmzK = false;}
      if(WaqdKurcTs == true){WaqdKurcTs = false;}
      if(XKGuDBGrpQ == true){XKGuDBGrpQ = false;}
      if(WPHJwXoneJ == true){WPHJwXoneJ = false;}
      if(wBnCzcgEyW == true){wBnCzcgEyW = false;}
      if(ucAFkVSekZ == true){ucAFkVSekZ = false;}
      if(MkwhmGTggl == true){MkwhmGTggl = false;}
      if(FDyKwVWDuZ == true){FDyKwVWDuZ = false;}
      if(UCtSVtOCXZ == true){UCtSVtOCXZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MEMLAHKGLD
{ 
  void hErQqpSHYn()
  { 
      bool pdgEADNafB = false;
      bool CpyTUxxeGR = false;
      bool qLJEeDxxrn = false;
      bool GEyftKFLJD = false;
      bool tsEzfYsmik = false;
      bool EiZIgdUcZx = false;
      bool GQiqtxqRYJ = false;
      bool qkVDXyHJXF = false;
      bool KJOHjXeozQ = false;
      bool pclLwdJMFi = false;
      bool gaNYsTMVki = false;
      bool WKETlwCbHV = false;
      bool RBSHTNyJGs = false;
      bool ntEoYiwoNP = false;
      bool KJEPteYSYA = false;
      bool bhEiQeHFFf = false;
      bool VDOqtcFPtD = false;
      bool AMNrLIxCNn = false;
      bool oejGRGoVXs = false;
      bool bpjhtHtMVR = false;
      string OFrKcbjFxE;
      string MkWWDVPANI;
      string uQNpLjshoq;
      string FmixOkWcPU;
      string XtAbVVPCtm;
      string fNnLyxqBzs;
      string hCQFyYCZHR;
      string ylksjAQlTo;
      string UqmCLrzWyR;
      string sUdWmVducw;
      string Tptcnscubx;
      string gFgJXqOOsG;
      string ZfapXcIOFN;
      string WVIXVdwKNS;
      string PEtBjmnUyu;
      string HOBWWmPEEK;
      string kNoNZLxJms;
      string uDAZpVkYFk;
      string omQJGITkAh;
      string OqGDhBOgaY;
      if(OFrKcbjFxE == Tptcnscubx){pdgEADNafB = true;}
      else if(Tptcnscubx == OFrKcbjFxE){gaNYsTMVki = true;}
      if(MkWWDVPANI == gFgJXqOOsG){CpyTUxxeGR = true;}
      else if(gFgJXqOOsG == MkWWDVPANI){WKETlwCbHV = true;}
      if(uQNpLjshoq == ZfapXcIOFN){qLJEeDxxrn = true;}
      else if(ZfapXcIOFN == uQNpLjshoq){RBSHTNyJGs = true;}
      if(FmixOkWcPU == WVIXVdwKNS){GEyftKFLJD = true;}
      else if(WVIXVdwKNS == FmixOkWcPU){ntEoYiwoNP = true;}
      if(XtAbVVPCtm == PEtBjmnUyu){tsEzfYsmik = true;}
      else if(PEtBjmnUyu == XtAbVVPCtm){KJEPteYSYA = true;}
      if(fNnLyxqBzs == HOBWWmPEEK){EiZIgdUcZx = true;}
      else if(HOBWWmPEEK == fNnLyxqBzs){bhEiQeHFFf = true;}
      if(hCQFyYCZHR == kNoNZLxJms){GQiqtxqRYJ = true;}
      else if(kNoNZLxJms == hCQFyYCZHR){VDOqtcFPtD = true;}
      if(ylksjAQlTo == uDAZpVkYFk){qkVDXyHJXF = true;}
      if(UqmCLrzWyR == omQJGITkAh){KJOHjXeozQ = true;}
      if(sUdWmVducw == OqGDhBOgaY){pclLwdJMFi = true;}
      while(uDAZpVkYFk == ylksjAQlTo){AMNrLIxCNn = true;}
      while(omQJGITkAh == omQJGITkAh){oejGRGoVXs = true;}
      while(OqGDhBOgaY == OqGDhBOgaY){bpjhtHtMVR = true;}
      if(pdgEADNafB == true){pdgEADNafB = false;}
      if(CpyTUxxeGR == true){CpyTUxxeGR = false;}
      if(qLJEeDxxrn == true){qLJEeDxxrn = false;}
      if(GEyftKFLJD == true){GEyftKFLJD = false;}
      if(tsEzfYsmik == true){tsEzfYsmik = false;}
      if(EiZIgdUcZx == true){EiZIgdUcZx = false;}
      if(GQiqtxqRYJ == true){GQiqtxqRYJ = false;}
      if(qkVDXyHJXF == true){qkVDXyHJXF = false;}
      if(KJOHjXeozQ == true){KJOHjXeozQ = false;}
      if(pclLwdJMFi == true){pclLwdJMFi = false;}
      if(gaNYsTMVki == true){gaNYsTMVki = false;}
      if(WKETlwCbHV == true){WKETlwCbHV = false;}
      if(RBSHTNyJGs == true){RBSHTNyJGs = false;}
      if(ntEoYiwoNP == true){ntEoYiwoNP = false;}
      if(KJEPteYSYA == true){KJEPteYSYA = false;}
      if(bhEiQeHFFf == true){bhEiQeHFFf = false;}
      if(VDOqtcFPtD == true){VDOqtcFPtD = false;}
      if(AMNrLIxCNn == true){AMNrLIxCNn = false;}
      if(oejGRGoVXs == true){oejGRGoVXs = false;}
      if(bpjhtHtMVR == true){bpjhtHtMVR = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ROZINMKDVS
{ 
  void krcSMTVGwU()
  { 
      bool ibJwdiCNPk = false;
      bool BOfflqitqm = false;
      bool BrgZnwDnDE = false;
      bool StBVmlxYPS = false;
      bool GLjuLGABBd = false;
      bool ewprVqtjIg = false;
      bool eOKKSGMJsA = false;
      bool tJdYgpHkNg = false;
      bool wClYZWDJAw = false;
      bool rMPhmqBYcF = false;
      bool sqIBjURRKf = false;
      bool iIWbkcbkku = false;
      bool CDJwIgtwko = false;
      bool niIImFskoE = false;
      bool ElyTMipnwK = false;
      bool PQmgCphMrD = false;
      bool aVgVrAANnf = false;
      bool ipugpQFoiZ = false;
      bool pGzZZWuaGx = false;
      bool XgsldiFcbM = false;
      string WZDFHnxuXT;
      string LUGEWSJutV;
      string qpACCFdljG;
      string lDYiVYhdkB;
      string FHxAqceDyp;
      string ioigUgZBDC;
      string KLKQgIuXix;
      string keJrQjrAKY;
      string ZamtXxUqNn;
      string zgaiqswfSi;
      string hHTBEDepQk;
      string nYpdbiXQsj;
      string nDKtxEEaBW;
      string nMhbqSGloU;
      string ReaczapxGH;
      string UelhtdwZDA;
      string yDnswbqOmJ;
      string clbYSJchtW;
      string NHUtBIZzgN;
      string VfhnVNphfX;
      if(WZDFHnxuXT == hHTBEDepQk){ibJwdiCNPk = true;}
      else if(hHTBEDepQk == WZDFHnxuXT){sqIBjURRKf = true;}
      if(LUGEWSJutV == nYpdbiXQsj){BOfflqitqm = true;}
      else if(nYpdbiXQsj == LUGEWSJutV){iIWbkcbkku = true;}
      if(qpACCFdljG == nDKtxEEaBW){BrgZnwDnDE = true;}
      else if(nDKtxEEaBW == qpACCFdljG){CDJwIgtwko = true;}
      if(lDYiVYhdkB == nMhbqSGloU){StBVmlxYPS = true;}
      else if(nMhbqSGloU == lDYiVYhdkB){niIImFskoE = true;}
      if(FHxAqceDyp == ReaczapxGH){GLjuLGABBd = true;}
      else if(ReaczapxGH == FHxAqceDyp){ElyTMipnwK = true;}
      if(ioigUgZBDC == UelhtdwZDA){ewprVqtjIg = true;}
      else if(UelhtdwZDA == ioigUgZBDC){PQmgCphMrD = true;}
      if(KLKQgIuXix == yDnswbqOmJ){eOKKSGMJsA = true;}
      else if(yDnswbqOmJ == KLKQgIuXix){aVgVrAANnf = true;}
      if(keJrQjrAKY == clbYSJchtW){tJdYgpHkNg = true;}
      if(ZamtXxUqNn == NHUtBIZzgN){wClYZWDJAw = true;}
      if(zgaiqswfSi == VfhnVNphfX){rMPhmqBYcF = true;}
      while(clbYSJchtW == keJrQjrAKY){ipugpQFoiZ = true;}
      while(NHUtBIZzgN == NHUtBIZzgN){pGzZZWuaGx = true;}
      while(VfhnVNphfX == VfhnVNphfX){XgsldiFcbM = true;}
      if(ibJwdiCNPk == true){ibJwdiCNPk = false;}
      if(BOfflqitqm == true){BOfflqitqm = false;}
      if(BrgZnwDnDE == true){BrgZnwDnDE = false;}
      if(StBVmlxYPS == true){StBVmlxYPS = false;}
      if(GLjuLGABBd == true){GLjuLGABBd = false;}
      if(ewprVqtjIg == true){ewprVqtjIg = false;}
      if(eOKKSGMJsA == true){eOKKSGMJsA = false;}
      if(tJdYgpHkNg == true){tJdYgpHkNg = false;}
      if(wClYZWDJAw == true){wClYZWDJAw = false;}
      if(rMPhmqBYcF == true){rMPhmqBYcF = false;}
      if(sqIBjURRKf == true){sqIBjURRKf = false;}
      if(iIWbkcbkku == true){iIWbkcbkku = false;}
      if(CDJwIgtwko == true){CDJwIgtwko = false;}
      if(niIImFskoE == true){niIImFskoE = false;}
      if(ElyTMipnwK == true){ElyTMipnwK = false;}
      if(PQmgCphMrD == true){PQmgCphMrD = false;}
      if(aVgVrAANnf == true){aVgVrAANnf = false;}
      if(ipugpQFoiZ == true){ipugpQFoiZ = false;}
      if(pGzZZWuaGx == true){pGzZZWuaGx = false;}
      if(XgsldiFcbM == true){XgsldiFcbM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PUVGJWXXKJ
{ 
  void AMwcNLlifm()
  { 
      bool cHoAelxdCS = false;
      bool FtXOzzNKUC = false;
      bool lFzkJyHRQK = false;
      bool VXhjidGgxw = false;
      bool bFfFLBrxqQ = false;
      bool TFXXMWHEqg = false;
      bool eeMqwgLmeV = false;
      bool sfNXafZsZH = false;
      bool FJaAqQAKWM = false;
      bool VCpPRwsnJd = false;
      bool oERUXmrzwg = false;
      bool wNeibUEMzf = false;
      bool fBQSzqpxGd = false;
      bool mljuSArFbE = false;
      bool XANoPHWGnw = false;
      bool rfocynnzUg = false;
      bool wusStKciPa = false;
      bool cyjtIemaDr = false;
      bool TjIKlGwHxe = false;
      bool MrjuIcHuuE = false;
      string JyHORAMnoX;
      string hjEpEKGZuL;
      string RtoeGzhMcz;
      string qahiArLmcu;
      string LyuwEkjASB;
      string CfKsYOJcIS;
      string RzKpfHCUoM;
      string zSxAASzraU;
      string OhsbwLPoJk;
      string hheJdLnEPw;
      string iOKGBjiKrG;
      string PQCmEjGhsU;
      string qMLEdQmmWn;
      string etcdlzWmaM;
      string cEwCURfqFs;
      string FFKtFVDGbj;
      string wunDbVnYBE;
      string NSHgrlQIiM;
      string kwhciYihYQ;
      string uPLpQgQraz;
      if(JyHORAMnoX == iOKGBjiKrG){cHoAelxdCS = true;}
      else if(iOKGBjiKrG == JyHORAMnoX){oERUXmrzwg = true;}
      if(hjEpEKGZuL == PQCmEjGhsU){FtXOzzNKUC = true;}
      else if(PQCmEjGhsU == hjEpEKGZuL){wNeibUEMzf = true;}
      if(RtoeGzhMcz == qMLEdQmmWn){lFzkJyHRQK = true;}
      else if(qMLEdQmmWn == RtoeGzhMcz){fBQSzqpxGd = true;}
      if(qahiArLmcu == etcdlzWmaM){VXhjidGgxw = true;}
      else if(etcdlzWmaM == qahiArLmcu){mljuSArFbE = true;}
      if(LyuwEkjASB == cEwCURfqFs){bFfFLBrxqQ = true;}
      else if(cEwCURfqFs == LyuwEkjASB){XANoPHWGnw = true;}
      if(CfKsYOJcIS == FFKtFVDGbj){TFXXMWHEqg = true;}
      else if(FFKtFVDGbj == CfKsYOJcIS){rfocynnzUg = true;}
      if(RzKpfHCUoM == wunDbVnYBE){eeMqwgLmeV = true;}
      else if(wunDbVnYBE == RzKpfHCUoM){wusStKciPa = true;}
      if(zSxAASzraU == NSHgrlQIiM){sfNXafZsZH = true;}
      if(OhsbwLPoJk == kwhciYihYQ){FJaAqQAKWM = true;}
      if(hheJdLnEPw == uPLpQgQraz){VCpPRwsnJd = true;}
      while(NSHgrlQIiM == zSxAASzraU){cyjtIemaDr = true;}
      while(kwhciYihYQ == kwhciYihYQ){TjIKlGwHxe = true;}
      while(uPLpQgQraz == uPLpQgQraz){MrjuIcHuuE = true;}
      if(cHoAelxdCS == true){cHoAelxdCS = false;}
      if(FtXOzzNKUC == true){FtXOzzNKUC = false;}
      if(lFzkJyHRQK == true){lFzkJyHRQK = false;}
      if(VXhjidGgxw == true){VXhjidGgxw = false;}
      if(bFfFLBrxqQ == true){bFfFLBrxqQ = false;}
      if(TFXXMWHEqg == true){TFXXMWHEqg = false;}
      if(eeMqwgLmeV == true){eeMqwgLmeV = false;}
      if(sfNXafZsZH == true){sfNXafZsZH = false;}
      if(FJaAqQAKWM == true){FJaAqQAKWM = false;}
      if(VCpPRwsnJd == true){VCpPRwsnJd = false;}
      if(oERUXmrzwg == true){oERUXmrzwg = false;}
      if(wNeibUEMzf == true){wNeibUEMzf = false;}
      if(fBQSzqpxGd == true){fBQSzqpxGd = false;}
      if(mljuSArFbE == true){mljuSArFbE = false;}
      if(XANoPHWGnw == true){XANoPHWGnw = false;}
      if(rfocynnzUg == true){rfocynnzUg = false;}
      if(wusStKciPa == true){wusStKciPa = false;}
      if(cyjtIemaDr == true){cyjtIemaDr = false;}
      if(TjIKlGwHxe == true){TjIKlGwHxe = false;}
      if(MrjuIcHuuE == true){MrjuIcHuuE = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OLJCNBZKCC
{ 
  void OlphakwaTD()
  { 
      bool OPEnlHMBst = false;
      bool IojocLrRny = false;
      bool presCAXQhd = false;
      bool ZQkkrupodG = false;
      bool HsQqQTeLza = false;
      bool kxdkmWkKkk = false;
      bool juTOUrDzwh = false;
      bool UDngpxbmci = false;
      bool VMQGsbmAxV = false;
      bool JfDrWMYBWX = false;
      bool PfORFhJwto = false;
      bool tygBOWdMVr = false;
      bool EDBZXnzuKd = false;
      bool YZPETpxzBL = false;
      bool SqGIbLyBdA = false;
      bool hLfdSLdotQ = false;
      bool hNlPQUhchY = false;
      bool ZeLKZNxryL = false;
      bool pxRKwMslkH = false;
      bool LIEjcEEprG = false;
      string PiblceyDuW;
      string XzDOfPfStB;
      string UNtwmyMDsP;
      string LpXjTWBGJy;
      string zbNVDYzWgu;
      string yKuCnCnfzd;
      string StJeKgaSuJ;
      string NZJwBszXrO;
      string zoWyRBuVwR;
      string iUHeeFRiLV;
      string xfbXgYAyAt;
      string CPmjecXLMJ;
      string lFWcarRdQw;
      string aQMVRSNyQs;
      string AwVCOSgWAB;
      string gGVCjrJyxn;
      string GWgDSkDQSL;
      string PYtTIoiUOH;
      string LudPOUtKlY;
      string IOBcldPSid;
      if(PiblceyDuW == xfbXgYAyAt){OPEnlHMBst = true;}
      else if(xfbXgYAyAt == PiblceyDuW){PfORFhJwto = true;}
      if(XzDOfPfStB == CPmjecXLMJ){IojocLrRny = true;}
      else if(CPmjecXLMJ == XzDOfPfStB){tygBOWdMVr = true;}
      if(UNtwmyMDsP == lFWcarRdQw){presCAXQhd = true;}
      else if(lFWcarRdQw == UNtwmyMDsP){EDBZXnzuKd = true;}
      if(LpXjTWBGJy == aQMVRSNyQs){ZQkkrupodG = true;}
      else if(aQMVRSNyQs == LpXjTWBGJy){YZPETpxzBL = true;}
      if(zbNVDYzWgu == AwVCOSgWAB){HsQqQTeLza = true;}
      else if(AwVCOSgWAB == zbNVDYzWgu){SqGIbLyBdA = true;}
      if(yKuCnCnfzd == gGVCjrJyxn){kxdkmWkKkk = true;}
      else if(gGVCjrJyxn == yKuCnCnfzd){hLfdSLdotQ = true;}
      if(StJeKgaSuJ == GWgDSkDQSL){juTOUrDzwh = true;}
      else if(GWgDSkDQSL == StJeKgaSuJ){hNlPQUhchY = true;}
      if(NZJwBszXrO == PYtTIoiUOH){UDngpxbmci = true;}
      if(zoWyRBuVwR == LudPOUtKlY){VMQGsbmAxV = true;}
      if(iUHeeFRiLV == IOBcldPSid){JfDrWMYBWX = true;}
      while(PYtTIoiUOH == NZJwBszXrO){ZeLKZNxryL = true;}
      while(LudPOUtKlY == LudPOUtKlY){pxRKwMslkH = true;}
      while(IOBcldPSid == IOBcldPSid){LIEjcEEprG = true;}
      if(OPEnlHMBst == true){OPEnlHMBst = false;}
      if(IojocLrRny == true){IojocLrRny = false;}
      if(presCAXQhd == true){presCAXQhd = false;}
      if(ZQkkrupodG == true){ZQkkrupodG = false;}
      if(HsQqQTeLza == true){HsQqQTeLza = false;}
      if(kxdkmWkKkk == true){kxdkmWkKkk = false;}
      if(juTOUrDzwh == true){juTOUrDzwh = false;}
      if(UDngpxbmci == true){UDngpxbmci = false;}
      if(VMQGsbmAxV == true){VMQGsbmAxV = false;}
      if(JfDrWMYBWX == true){JfDrWMYBWX = false;}
      if(PfORFhJwto == true){PfORFhJwto = false;}
      if(tygBOWdMVr == true){tygBOWdMVr = false;}
      if(EDBZXnzuKd == true){EDBZXnzuKd = false;}
      if(YZPETpxzBL == true){YZPETpxzBL = false;}
      if(SqGIbLyBdA == true){SqGIbLyBdA = false;}
      if(hLfdSLdotQ == true){hLfdSLdotQ = false;}
      if(hNlPQUhchY == true){hNlPQUhchY = false;}
      if(ZeLKZNxryL == true){ZeLKZNxryL = false;}
      if(pxRKwMslkH == true){pxRKwMslkH = false;}
      if(LIEjcEEprG == true){LIEjcEEprG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YDWPFKCCBC
{ 
  void mtrqiYzOxG()
  { 
      bool NBOuOpHtRb = false;
      bool FtpEbLRuGA = false;
      bool ebBucylJsA = false;
      bool lPtKWYfpRM = false;
      bool fgEdYUqwLi = false;
      bool sljJwlASRP = false;
      bool DyBRopzegD = false;
      bool fbctTxWWVI = false;
      bool GpxJGAfkOk = false;
      bool XMXBIhflni = false;
      bool ULOIDRIooy = false;
      bool OdwJJNAqLo = false;
      bool JOQweSDakl = false;
      bool QjhmmubnGo = false;
      bool qpuQVsPOru = false;
      bool goazZGuSck = false;
      bool NhEDhduJVN = false;
      bool kMjVxTSAZl = false;
      bool CnhoFaJhMg = false;
      bool AJLUbJkrhq = false;
      string zWMMVQalMB;
      string JVdYSxfiow;
      string AuVghyVVSc;
      string kWUxELwmQe;
      string KNJSIUiPtn;
      string juMcSEBfeW;
      string uETiPlAgTH;
      string ArSKlllCXM;
      string PFbespKrdA;
      string KQItjFtZCW;
      string UBulmqbCqf;
      string SyHSGobopZ;
      string bICTxjjYaf;
      string rLzAqNJoqB;
      string OIBxhLIomt;
      string cEicuMzboD;
      string GDbKmGyGar;
      string fmxoKSGEkk;
      string atkfDmbWFe;
      string OhAnuKgqJa;
      if(zWMMVQalMB == UBulmqbCqf){NBOuOpHtRb = true;}
      else if(UBulmqbCqf == zWMMVQalMB){ULOIDRIooy = true;}
      if(JVdYSxfiow == SyHSGobopZ){FtpEbLRuGA = true;}
      else if(SyHSGobopZ == JVdYSxfiow){OdwJJNAqLo = true;}
      if(AuVghyVVSc == bICTxjjYaf){ebBucylJsA = true;}
      else if(bICTxjjYaf == AuVghyVVSc){JOQweSDakl = true;}
      if(kWUxELwmQe == rLzAqNJoqB){lPtKWYfpRM = true;}
      else if(rLzAqNJoqB == kWUxELwmQe){QjhmmubnGo = true;}
      if(KNJSIUiPtn == OIBxhLIomt){fgEdYUqwLi = true;}
      else if(OIBxhLIomt == KNJSIUiPtn){qpuQVsPOru = true;}
      if(juMcSEBfeW == cEicuMzboD){sljJwlASRP = true;}
      else if(cEicuMzboD == juMcSEBfeW){goazZGuSck = true;}
      if(uETiPlAgTH == GDbKmGyGar){DyBRopzegD = true;}
      else if(GDbKmGyGar == uETiPlAgTH){NhEDhduJVN = true;}
      if(ArSKlllCXM == fmxoKSGEkk){fbctTxWWVI = true;}
      if(PFbespKrdA == atkfDmbWFe){GpxJGAfkOk = true;}
      if(KQItjFtZCW == OhAnuKgqJa){XMXBIhflni = true;}
      while(fmxoKSGEkk == ArSKlllCXM){kMjVxTSAZl = true;}
      while(atkfDmbWFe == atkfDmbWFe){CnhoFaJhMg = true;}
      while(OhAnuKgqJa == OhAnuKgqJa){AJLUbJkrhq = true;}
      if(NBOuOpHtRb == true){NBOuOpHtRb = false;}
      if(FtpEbLRuGA == true){FtpEbLRuGA = false;}
      if(ebBucylJsA == true){ebBucylJsA = false;}
      if(lPtKWYfpRM == true){lPtKWYfpRM = false;}
      if(fgEdYUqwLi == true){fgEdYUqwLi = false;}
      if(sljJwlASRP == true){sljJwlASRP = false;}
      if(DyBRopzegD == true){DyBRopzegD = false;}
      if(fbctTxWWVI == true){fbctTxWWVI = false;}
      if(GpxJGAfkOk == true){GpxJGAfkOk = false;}
      if(XMXBIhflni == true){XMXBIhflni = false;}
      if(ULOIDRIooy == true){ULOIDRIooy = false;}
      if(OdwJJNAqLo == true){OdwJJNAqLo = false;}
      if(JOQweSDakl == true){JOQweSDakl = false;}
      if(QjhmmubnGo == true){QjhmmubnGo = false;}
      if(qpuQVsPOru == true){qpuQVsPOru = false;}
      if(goazZGuSck == true){goazZGuSck = false;}
      if(NhEDhduJVN == true){NhEDhduJVN = false;}
      if(kMjVxTSAZl == true){kMjVxTSAZl = false;}
      if(CnhoFaJhMg == true){CnhoFaJhMg = false;}
      if(AJLUbJkrhq == true){AJLUbJkrhq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SXQXGHWXCP
{ 
  void DiaEcImibh()
  { 
      bool RrpEPsxpFD = false;
      bool zZHfqyEGnk = false;
      bool OsYrVohuYi = false;
      bool yhOYVixPpV = false;
      bool IWGdZbSwJN = false;
      bool WrcCzXTyia = false;
      bool KBQVFQLiJH = false;
      bool AoPRbrltRt = false;
      bool VllLLGFMZx = false;
      bool OzgPKHFkYI = false;
      bool PilBNgbKZG = false;
      bool lYfnEYOFJy = false;
      bool LzJLsBIAGU = false;
      bool lBUJdlzkye = false;
      bool dtlSLoSrLl = false;
      bool rzaZwQZaIL = false;
      bool YbDEUjEWKR = false;
      bool ZdHeUQMpzD = false;
      bool iQnUclZtMN = false;
      bool MYclFBphxb = false;
      string FHUuqsCqgG;
      string dwYSKwVxzV;
      string mjSePBLVSM;
      string VJnUetebUy;
      string rfgMhZNOuI;
      string dyzzyUnoep;
      string hgYTjUIDnQ;
      string QtbBcGCFxp;
      string nwZKiXKoWJ;
      string FpxePKHMcM;
      string RJZdrjVxVq;
      string RhqoCkGNkM;
      string KSgeKZwxkS;
      string zocjXdVwRz;
      string BMTeqWzHce;
      string DsbQCybuTR;
      string mYMCqcFGQX;
      string PKwkKyzBmm;
      string anTGOGNMhY;
      string CZbAMbzQmO;
      if(FHUuqsCqgG == RJZdrjVxVq){RrpEPsxpFD = true;}
      else if(RJZdrjVxVq == FHUuqsCqgG){PilBNgbKZG = true;}
      if(dwYSKwVxzV == RhqoCkGNkM){zZHfqyEGnk = true;}
      else if(RhqoCkGNkM == dwYSKwVxzV){lYfnEYOFJy = true;}
      if(mjSePBLVSM == KSgeKZwxkS){OsYrVohuYi = true;}
      else if(KSgeKZwxkS == mjSePBLVSM){LzJLsBIAGU = true;}
      if(VJnUetebUy == zocjXdVwRz){yhOYVixPpV = true;}
      else if(zocjXdVwRz == VJnUetebUy){lBUJdlzkye = true;}
      if(rfgMhZNOuI == BMTeqWzHce){IWGdZbSwJN = true;}
      else if(BMTeqWzHce == rfgMhZNOuI){dtlSLoSrLl = true;}
      if(dyzzyUnoep == DsbQCybuTR){WrcCzXTyia = true;}
      else if(DsbQCybuTR == dyzzyUnoep){rzaZwQZaIL = true;}
      if(hgYTjUIDnQ == mYMCqcFGQX){KBQVFQLiJH = true;}
      else if(mYMCqcFGQX == hgYTjUIDnQ){YbDEUjEWKR = true;}
      if(QtbBcGCFxp == PKwkKyzBmm){AoPRbrltRt = true;}
      if(nwZKiXKoWJ == anTGOGNMhY){VllLLGFMZx = true;}
      if(FpxePKHMcM == CZbAMbzQmO){OzgPKHFkYI = true;}
      while(PKwkKyzBmm == QtbBcGCFxp){ZdHeUQMpzD = true;}
      while(anTGOGNMhY == anTGOGNMhY){iQnUclZtMN = true;}
      while(CZbAMbzQmO == CZbAMbzQmO){MYclFBphxb = true;}
      if(RrpEPsxpFD == true){RrpEPsxpFD = false;}
      if(zZHfqyEGnk == true){zZHfqyEGnk = false;}
      if(OsYrVohuYi == true){OsYrVohuYi = false;}
      if(yhOYVixPpV == true){yhOYVixPpV = false;}
      if(IWGdZbSwJN == true){IWGdZbSwJN = false;}
      if(WrcCzXTyia == true){WrcCzXTyia = false;}
      if(KBQVFQLiJH == true){KBQVFQLiJH = false;}
      if(AoPRbrltRt == true){AoPRbrltRt = false;}
      if(VllLLGFMZx == true){VllLLGFMZx = false;}
      if(OzgPKHFkYI == true){OzgPKHFkYI = false;}
      if(PilBNgbKZG == true){PilBNgbKZG = false;}
      if(lYfnEYOFJy == true){lYfnEYOFJy = false;}
      if(LzJLsBIAGU == true){LzJLsBIAGU = false;}
      if(lBUJdlzkye == true){lBUJdlzkye = false;}
      if(dtlSLoSrLl == true){dtlSLoSrLl = false;}
      if(rzaZwQZaIL == true){rzaZwQZaIL = false;}
      if(YbDEUjEWKR == true){YbDEUjEWKR = false;}
      if(ZdHeUQMpzD == true){ZdHeUQMpzD = false;}
      if(iQnUclZtMN == true){iQnUclZtMN = false;}
      if(MYclFBphxb == true){MYclFBphxb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZEUPWJSJCY
{ 
  void wgkqoxetoy()
  { 
      bool ukugLKKhar = false;
      bool QFMzwIYESw = false;
      bool EhbyPcugkF = false;
      bool atHnHbqYuD = false;
      bool HhEzbHFLeh = false;
      bool jgDtxNJwQi = false;
      bool PcwVRWxXqT = false;
      bool RAIOoVdbVI = false;
      bool WjCCUspMpt = false;
      bool RtHMnekihF = false;
      bool JgTKPjXnFb = false;
      bool jgZILfasAY = false;
      bool fjgwCXrwaJ = false;
      bool jutPypPqmk = false;
      bool UCGOKkTzAI = false;
      bool DrhJZYzWIr = false;
      bool qaNwqolKJU = false;
      bool PjGODccSlR = false;
      bool DsuSGrMkoY = false;
      bool prhttsoKXV = false;
      string xwgGVsiodf;
      string KYVyrreqQn;
      string UzxDMAmphG;
      string sgoIRpBzYd;
      string aUxgrkXzPT;
      string zLLYAyzctV;
      string FrHWldiMgC;
      string qeRlMzBPLb;
      string bnZNHhUPHS;
      string qXFDYhNatN;
      string BKRNcUsYLB;
      string wiRStRRtPE;
      string ITFNLTYONe;
      string laVxaRVMkm;
      string nwDGWIGmIG;
      string PTKpEgEDCJ;
      string ueKgOUMUrb;
      string uFJcPMQzqD;
      string NlFGORQers;
      string MSKbqJkVqC;
      if(xwgGVsiodf == BKRNcUsYLB){ukugLKKhar = true;}
      else if(BKRNcUsYLB == xwgGVsiodf){JgTKPjXnFb = true;}
      if(KYVyrreqQn == wiRStRRtPE){QFMzwIYESw = true;}
      else if(wiRStRRtPE == KYVyrreqQn){jgZILfasAY = true;}
      if(UzxDMAmphG == ITFNLTYONe){EhbyPcugkF = true;}
      else if(ITFNLTYONe == UzxDMAmphG){fjgwCXrwaJ = true;}
      if(sgoIRpBzYd == laVxaRVMkm){atHnHbqYuD = true;}
      else if(laVxaRVMkm == sgoIRpBzYd){jutPypPqmk = true;}
      if(aUxgrkXzPT == nwDGWIGmIG){HhEzbHFLeh = true;}
      else if(nwDGWIGmIG == aUxgrkXzPT){UCGOKkTzAI = true;}
      if(zLLYAyzctV == PTKpEgEDCJ){jgDtxNJwQi = true;}
      else if(PTKpEgEDCJ == zLLYAyzctV){DrhJZYzWIr = true;}
      if(FrHWldiMgC == ueKgOUMUrb){PcwVRWxXqT = true;}
      else if(ueKgOUMUrb == FrHWldiMgC){qaNwqolKJU = true;}
      if(qeRlMzBPLb == uFJcPMQzqD){RAIOoVdbVI = true;}
      if(bnZNHhUPHS == NlFGORQers){WjCCUspMpt = true;}
      if(qXFDYhNatN == MSKbqJkVqC){RtHMnekihF = true;}
      while(uFJcPMQzqD == qeRlMzBPLb){PjGODccSlR = true;}
      while(NlFGORQers == NlFGORQers){DsuSGrMkoY = true;}
      while(MSKbqJkVqC == MSKbqJkVqC){prhttsoKXV = true;}
      if(ukugLKKhar == true){ukugLKKhar = false;}
      if(QFMzwIYESw == true){QFMzwIYESw = false;}
      if(EhbyPcugkF == true){EhbyPcugkF = false;}
      if(atHnHbqYuD == true){atHnHbqYuD = false;}
      if(HhEzbHFLeh == true){HhEzbHFLeh = false;}
      if(jgDtxNJwQi == true){jgDtxNJwQi = false;}
      if(PcwVRWxXqT == true){PcwVRWxXqT = false;}
      if(RAIOoVdbVI == true){RAIOoVdbVI = false;}
      if(WjCCUspMpt == true){WjCCUspMpt = false;}
      if(RtHMnekihF == true){RtHMnekihF = false;}
      if(JgTKPjXnFb == true){JgTKPjXnFb = false;}
      if(jgZILfasAY == true){jgZILfasAY = false;}
      if(fjgwCXrwaJ == true){fjgwCXrwaJ = false;}
      if(jutPypPqmk == true){jutPypPqmk = false;}
      if(UCGOKkTzAI == true){UCGOKkTzAI = false;}
      if(DrhJZYzWIr == true){DrhJZYzWIr = false;}
      if(qaNwqolKJU == true){qaNwqolKJU = false;}
      if(PjGODccSlR == true){PjGODccSlR = false;}
      if(DsuSGrMkoY == true){DsuSGrMkoY = false;}
      if(prhttsoKXV == true){prhttsoKXV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WBUABLZPOX
{ 
  void OwdkPkXjDD()
  { 
      bool ABoXXRSGEz = false;
      bool cbUUqnySPj = false;
      bool caALURODXh = false;
      bool OTitmEabKa = false;
      bool PWzlttEjZa = false;
      bool GZFsrYAdDG = false;
      bool COhVkIrahm = false;
      bool NVHnTGaOQZ = false;
      bool MddwanOpBN = false;
      bool DwYEYAndAk = false;
      bool CdCmNkSQYl = false;
      bool xlMdsXjlux = false;
      bool tCYTiQxDPS = false;
      bool SxNeXibqip = false;
      bool VyQPPRjtLA = false;
      bool QCiDqzFhDw = false;
      bool mPAFSzXgGK = false;
      bool wsCEaukIDR = false;
      bool gCqPCPOKnX = false;
      bool csmmCGyKwP = false;
      string MgrpJRMpVd;
      string MfFSnLnOen;
      string yEZKpxXpEy;
      string SNzNQNAzeN;
      string pHbuHQngBB;
      string iySjeIGhAM;
      string tWkXLygqCa;
      string LoiRyqceCZ;
      string IpnDOTuOiI;
      string HSzSSTVDiJ;
      string jxeaptPAln;
      string faeZDDhhIq;
      string CRHsSsoPyd;
      string VYuspLLNNU;
      string EucucfmYPP;
      string lmuqHRokHw;
      string EzBHRGeljq;
      string qUUnmHoMQm;
      string FPAirhdEkX;
      string WZFQPFsUcV;
      if(MgrpJRMpVd == jxeaptPAln){ABoXXRSGEz = true;}
      else if(jxeaptPAln == MgrpJRMpVd){CdCmNkSQYl = true;}
      if(MfFSnLnOen == faeZDDhhIq){cbUUqnySPj = true;}
      else if(faeZDDhhIq == MfFSnLnOen){xlMdsXjlux = true;}
      if(yEZKpxXpEy == CRHsSsoPyd){caALURODXh = true;}
      else if(CRHsSsoPyd == yEZKpxXpEy){tCYTiQxDPS = true;}
      if(SNzNQNAzeN == VYuspLLNNU){OTitmEabKa = true;}
      else if(VYuspLLNNU == SNzNQNAzeN){SxNeXibqip = true;}
      if(pHbuHQngBB == EucucfmYPP){PWzlttEjZa = true;}
      else if(EucucfmYPP == pHbuHQngBB){VyQPPRjtLA = true;}
      if(iySjeIGhAM == lmuqHRokHw){GZFsrYAdDG = true;}
      else if(lmuqHRokHw == iySjeIGhAM){QCiDqzFhDw = true;}
      if(tWkXLygqCa == EzBHRGeljq){COhVkIrahm = true;}
      else if(EzBHRGeljq == tWkXLygqCa){mPAFSzXgGK = true;}
      if(LoiRyqceCZ == qUUnmHoMQm){NVHnTGaOQZ = true;}
      if(IpnDOTuOiI == FPAirhdEkX){MddwanOpBN = true;}
      if(HSzSSTVDiJ == WZFQPFsUcV){DwYEYAndAk = true;}
      while(qUUnmHoMQm == LoiRyqceCZ){wsCEaukIDR = true;}
      while(FPAirhdEkX == FPAirhdEkX){gCqPCPOKnX = true;}
      while(WZFQPFsUcV == WZFQPFsUcV){csmmCGyKwP = true;}
      if(ABoXXRSGEz == true){ABoXXRSGEz = false;}
      if(cbUUqnySPj == true){cbUUqnySPj = false;}
      if(caALURODXh == true){caALURODXh = false;}
      if(OTitmEabKa == true){OTitmEabKa = false;}
      if(PWzlttEjZa == true){PWzlttEjZa = false;}
      if(GZFsrYAdDG == true){GZFsrYAdDG = false;}
      if(COhVkIrahm == true){COhVkIrahm = false;}
      if(NVHnTGaOQZ == true){NVHnTGaOQZ = false;}
      if(MddwanOpBN == true){MddwanOpBN = false;}
      if(DwYEYAndAk == true){DwYEYAndAk = false;}
      if(CdCmNkSQYl == true){CdCmNkSQYl = false;}
      if(xlMdsXjlux == true){xlMdsXjlux = false;}
      if(tCYTiQxDPS == true){tCYTiQxDPS = false;}
      if(SxNeXibqip == true){SxNeXibqip = false;}
      if(VyQPPRjtLA == true){VyQPPRjtLA = false;}
      if(QCiDqzFhDw == true){QCiDqzFhDw = false;}
      if(mPAFSzXgGK == true){mPAFSzXgGK = false;}
      if(wsCEaukIDR == true){wsCEaukIDR = false;}
      if(gCqPCPOKnX == true){gCqPCPOKnX = false;}
      if(csmmCGyKwP == true){csmmCGyKwP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IVYZUUJFAH
{ 
  void QfunYrFFBg()
  { 
      bool UgzkTTqpFo = false;
      bool AXHbtMLxEA = false;
      bool wjKwnICHeE = false;
      bool janBjlNhor = false;
      bool bQdWHlKADB = false;
      bool NEcbmkWCwL = false;
      bool QjKBFYiFqH = false;
      bool zcHMYeFmoC = false;
      bool yBEdpeyLOz = false;
      bool OLkdarFbQh = false;
      bool CEJuCjwQDH = false;
      bool ejNtMOfgLZ = false;
      bool aWpqsnrdkg = false;
      bool ZcVGzsrIaF = false;
      bool igIHUexqQL = false;
      bool LYQqHeORBO = false;
      bool rSeWhBNGtS = false;
      bool WWGFqsOiVN = false;
      bool pAMZUAMezU = false;
      bool eZHLMQClZT = false;
      string JKRgASCfcd;
      string aZlHYXPMaG;
      string xyhudBGeTM;
      string FMtqwEkadr;
      string KWkskwzcuX;
      string fQsWJKbVCy;
      string MXLQYNfuKU;
      string mohiwaNoKh;
      string CciwgnSXgk;
      string bOXeelLUUW;
      string QDSZkqhKZi;
      string xmKUwjnOjj;
      string tHOZsXQCDJ;
      string PrTnQyKJKo;
      string FIyxnnEDit;
      string ywkjbJorgN;
      string afGPTETWDx;
      string ZeEtdlJRkF;
      string TNTJLTHjMH;
      string JRbxOZBhci;
      if(JKRgASCfcd == QDSZkqhKZi){UgzkTTqpFo = true;}
      else if(QDSZkqhKZi == JKRgASCfcd){CEJuCjwQDH = true;}
      if(aZlHYXPMaG == xmKUwjnOjj){AXHbtMLxEA = true;}
      else if(xmKUwjnOjj == aZlHYXPMaG){ejNtMOfgLZ = true;}
      if(xyhudBGeTM == tHOZsXQCDJ){wjKwnICHeE = true;}
      else if(tHOZsXQCDJ == xyhudBGeTM){aWpqsnrdkg = true;}
      if(FMtqwEkadr == PrTnQyKJKo){janBjlNhor = true;}
      else if(PrTnQyKJKo == FMtqwEkadr){ZcVGzsrIaF = true;}
      if(KWkskwzcuX == FIyxnnEDit){bQdWHlKADB = true;}
      else if(FIyxnnEDit == KWkskwzcuX){igIHUexqQL = true;}
      if(fQsWJKbVCy == ywkjbJorgN){NEcbmkWCwL = true;}
      else if(ywkjbJorgN == fQsWJKbVCy){LYQqHeORBO = true;}
      if(MXLQYNfuKU == afGPTETWDx){QjKBFYiFqH = true;}
      else if(afGPTETWDx == MXLQYNfuKU){rSeWhBNGtS = true;}
      if(mohiwaNoKh == ZeEtdlJRkF){zcHMYeFmoC = true;}
      if(CciwgnSXgk == TNTJLTHjMH){yBEdpeyLOz = true;}
      if(bOXeelLUUW == JRbxOZBhci){OLkdarFbQh = true;}
      while(ZeEtdlJRkF == mohiwaNoKh){WWGFqsOiVN = true;}
      while(TNTJLTHjMH == TNTJLTHjMH){pAMZUAMezU = true;}
      while(JRbxOZBhci == JRbxOZBhci){eZHLMQClZT = true;}
      if(UgzkTTqpFo == true){UgzkTTqpFo = false;}
      if(AXHbtMLxEA == true){AXHbtMLxEA = false;}
      if(wjKwnICHeE == true){wjKwnICHeE = false;}
      if(janBjlNhor == true){janBjlNhor = false;}
      if(bQdWHlKADB == true){bQdWHlKADB = false;}
      if(NEcbmkWCwL == true){NEcbmkWCwL = false;}
      if(QjKBFYiFqH == true){QjKBFYiFqH = false;}
      if(zcHMYeFmoC == true){zcHMYeFmoC = false;}
      if(yBEdpeyLOz == true){yBEdpeyLOz = false;}
      if(OLkdarFbQh == true){OLkdarFbQh = false;}
      if(CEJuCjwQDH == true){CEJuCjwQDH = false;}
      if(ejNtMOfgLZ == true){ejNtMOfgLZ = false;}
      if(aWpqsnrdkg == true){aWpqsnrdkg = false;}
      if(ZcVGzsrIaF == true){ZcVGzsrIaF = false;}
      if(igIHUexqQL == true){igIHUexqQL = false;}
      if(LYQqHeORBO == true){LYQqHeORBO = false;}
      if(rSeWhBNGtS == true){rSeWhBNGtS = false;}
      if(WWGFqsOiVN == true){WWGFqsOiVN = false;}
      if(pAMZUAMezU == true){pAMZUAMezU = false;}
      if(eZHLMQClZT == true){eZHLMQClZT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OLOEXHGDNH
{ 
  void zffsyOeFpC()
  { 
      bool TYWBrljsyi = false;
      bool BhLoeZJAwc = false;
      bool TxQTkGxSDT = false;
      bool UqjTYUsoNI = false;
      bool OmOturONdz = false;
      bool XGPRlWICPl = false;
      bool oRpeOKxqQK = false;
      bool fUmmOfhVYr = false;
      bool rioEuOaShB = false;
      bool LpYbTxxifj = false;
      bool uWxSSDTcDF = false;
      bool nldWtFbkjz = false;
      bool ieJOzpuKSQ = false;
      bool hypnxUlVlr = false;
      bool uyObMYMChO = false;
      bool UMzfAPNhTb = false;
      bool HIJlXpwoZY = false;
      bool SGzrEkPgBZ = false;
      bool dscEwVzKnh = false;
      bool HDCJjlncEW = false;
      string FgJdfCzDNb;
      string ecqbHqsbdw;
      string hgHkAekYXM;
      string FlydjeaiNh;
      string tBOHtobJiC;
      string ESiYTXYYcB;
      string RZeAWseMrc;
      string PKOuBgSzpL;
      string CapUIlCZKL;
      string FCGLPUVOkB;
      string JOQFmtDmWm;
      string FZiRzpiJqj;
      string pFCfXNEDff;
      string KEOnBmKjhO;
      string iOCWGeTnMY;
      string kLWAKEMjgY;
      string pAHpbBKtQL;
      string fzbcBbCeYr;
      string tPpGtzgNdw;
      string ZZCIYhGLuF;
      if(FgJdfCzDNb == JOQFmtDmWm){TYWBrljsyi = true;}
      else if(JOQFmtDmWm == FgJdfCzDNb){uWxSSDTcDF = true;}
      if(ecqbHqsbdw == FZiRzpiJqj){BhLoeZJAwc = true;}
      else if(FZiRzpiJqj == ecqbHqsbdw){nldWtFbkjz = true;}
      if(hgHkAekYXM == pFCfXNEDff){TxQTkGxSDT = true;}
      else if(pFCfXNEDff == hgHkAekYXM){ieJOzpuKSQ = true;}
      if(FlydjeaiNh == KEOnBmKjhO){UqjTYUsoNI = true;}
      else if(KEOnBmKjhO == FlydjeaiNh){hypnxUlVlr = true;}
      if(tBOHtobJiC == iOCWGeTnMY){OmOturONdz = true;}
      else if(iOCWGeTnMY == tBOHtobJiC){uyObMYMChO = true;}
      if(ESiYTXYYcB == kLWAKEMjgY){XGPRlWICPl = true;}
      else if(kLWAKEMjgY == ESiYTXYYcB){UMzfAPNhTb = true;}
      if(RZeAWseMrc == pAHpbBKtQL){oRpeOKxqQK = true;}
      else if(pAHpbBKtQL == RZeAWseMrc){HIJlXpwoZY = true;}
      if(PKOuBgSzpL == fzbcBbCeYr){fUmmOfhVYr = true;}
      if(CapUIlCZKL == tPpGtzgNdw){rioEuOaShB = true;}
      if(FCGLPUVOkB == ZZCIYhGLuF){LpYbTxxifj = true;}
      while(fzbcBbCeYr == PKOuBgSzpL){SGzrEkPgBZ = true;}
      while(tPpGtzgNdw == tPpGtzgNdw){dscEwVzKnh = true;}
      while(ZZCIYhGLuF == ZZCIYhGLuF){HDCJjlncEW = true;}
      if(TYWBrljsyi == true){TYWBrljsyi = false;}
      if(BhLoeZJAwc == true){BhLoeZJAwc = false;}
      if(TxQTkGxSDT == true){TxQTkGxSDT = false;}
      if(UqjTYUsoNI == true){UqjTYUsoNI = false;}
      if(OmOturONdz == true){OmOturONdz = false;}
      if(XGPRlWICPl == true){XGPRlWICPl = false;}
      if(oRpeOKxqQK == true){oRpeOKxqQK = false;}
      if(fUmmOfhVYr == true){fUmmOfhVYr = false;}
      if(rioEuOaShB == true){rioEuOaShB = false;}
      if(LpYbTxxifj == true){LpYbTxxifj = false;}
      if(uWxSSDTcDF == true){uWxSSDTcDF = false;}
      if(nldWtFbkjz == true){nldWtFbkjz = false;}
      if(ieJOzpuKSQ == true){ieJOzpuKSQ = false;}
      if(hypnxUlVlr == true){hypnxUlVlr = false;}
      if(uyObMYMChO == true){uyObMYMChO = false;}
      if(UMzfAPNhTb == true){UMzfAPNhTb = false;}
      if(HIJlXpwoZY == true){HIJlXpwoZY = false;}
      if(SGzrEkPgBZ == true){SGzrEkPgBZ = false;}
      if(dscEwVzKnh == true){dscEwVzKnh = false;}
      if(HDCJjlncEW == true){HDCJjlncEW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ASDSZUKWSV
{ 
  void SKTouVVRVG()
  { 
      bool SqdPQgSqqY = false;
      bool AwemBVtSIV = false;
      bool lJfqcSWArz = false;
      bool ffSteCQppe = false;
      bool QLoaQhYUTh = false;
      bool ShwXeIKlmy = false;
      bool JRyrsUcjIz = false;
      bool EEccFZTEat = false;
      bool sCgKgyiuPV = false;
      bool qeCqVzNxhw = false;
      bool FoLUcOFSSF = false;
      bool nGLrEGknKt = false;
      bool khUmIZBkHF = false;
      bool MWWdnDqLhR = false;
      bool sQcKgkSKlO = false;
      bool OZlmLXUYMC = false;
      bool MTKMrRihEB = false;
      bool PYFsBTEJFW = false;
      bool GqysqljiRQ = false;
      bool iRzeFYFPRP = false;
      string RJtzXzajqa;
      string YHCWZuzOjB;
      string xoOliUYTSE;
      string KVYLPgSQUg;
      string rurwRgLRio;
      string iVYDgcuHPD;
      string ZHRmPDhpHr;
      string bisDJCwRUC;
      string IGjdUowVdm;
      string lKARHjcFfN;
      string yUKqMzbVte;
      string fcmNtSXnRa;
      string HMMTGQhbuY;
      string YplGDmurQU;
      string NaazHIXmWn;
      string MomFGlIqku;
      string QzZVFopfJH;
      string WeWwNgkMMx;
      string suFHMiDgAR;
      string dEHVDNAFsR;
      if(RJtzXzajqa == yUKqMzbVte){SqdPQgSqqY = true;}
      else if(yUKqMzbVte == RJtzXzajqa){FoLUcOFSSF = true;}
      if(YHCWZuzOjB == fcmNtSXnRa){AwemBVtSIV = true;}
      else if(fcmNtSXnRa == YHCWZuzOjB){nGLrEGknKt = true;}
      if(xoOliUYTSE == HMMTGQhbuY){lJfqcSWArz = true;}
      else if(HMMTGQhbuY == xoOliUYTSE){khUmIZBkHF = true;}
      if(KVYLPgSQUg == YplGDmurQU){ffSteCQppe = true;}
      else if(YplGDmurQU == KVYLPgSQUg){MWWdnDqLhR = true;}
      if(rurwRgLRio == NaazHIXmWn){QLoaQhYUTh = true;}
      else if(NaazHIXmWn == rurwRgLRio){sQcKgkSKlO = true;}
      if(iVYDgcuHPD == MomFGlIqku){ShwXeIKlmy = true;}
      else if(MomFGlIqku == iVYDgcuHPD){OZlmLXUYMC = true;}
      if(ZHRmPDhpHr == QzZVFopfJH){JRyrsUcjIz = true;}
      else if(QzZVFopfJH == ZHRmPDhpHr){MTKMrRihEB = true;}
      if(bisDJCwRUC == WeWwNgkMMx){EEccFZTEat = true;}
      if(IGjdUowVdm == suFHMiDgAR){sCgKgyiuPV = true;}
      if(lKARHjcFfN == dEHVDNAFsR){qeCqVzNxhw = true;}
      while(WeWwNgkMMx == bisDJCwRUC){PYFsBTEJFW = true;}
      while(suFHMiDgAR == suFHMiDgAR){GqysqljiRQ = true;}
      while(dEHVDNAFsR == dEHVDNAFsR){iRzeFYFPRP = true;}
      if(SqdPQgSqqY == true){SqdPQgSqqY = false;}
      if(AwemBVtSIV == true){AwemBVtSIV = false;}
      if(lJfqcSWArz == true){lJfqcSWArz = false;}
      if(ffSteCQppe == true){ffSteCQppe = false;}
      if(QLoaQhYUTh == true){QLoaQhYUTh = false;}
      if(ShwXeIKlmy == true){ShwXeIKlmy = false;}
      if(JRyrsUcjIz == true){JRyrsUcjIz = false;}
      if(EEccFZTEat == true){EEccFZTEat = false;}
      if(sCgKgyiuPV == true){sCgKgyiuPV = false;}
      if(qeCqVzNxhw == true){qeCqVzNxhw = false;}
      if(FoLUcOFSSF == true){FoLUcOFSSF = false;}
      if(nGLrEGknKt == true){nGLrEGknKt = false;}
      if(khUmIZBkHF == true){khUmIZBkHF = false;}
      if(MWWdnDqLhR == true){MWWdnDqLhR = false;}
      if(sQcKgkSKlO == true){sQcKgkSKlO = false;}
      if(OZlmLXUYMC == true){OZlmLXUYMC = false;}
      if(MTKMrRihEB == true){MTKMrRihEB = false;}
      if(PYFsBTEJFW == true){PYFsBTEJFW = false;}
      if(GqysqljiRQ == true){GqysqljiRQ = false;}
      if(iRzeFYFPRP == true){iRzeFYFPRP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BYNHSIXSPJ
{ 
  void itBDHrZIQU()
  { 
      bool nXefkkBVcf = false;
      bool ZMmpEXEODX = false;
      bool aKCTKPMVYX = false;
      bool cdYIGIKDHp = false;
      bool kxWNiJbNJY = false;
      bool KRTfzdVKUU = false;
      bool YewFCumdju = false;
      bool NAHMmcnnkB = false;
      bool myMANLKsJc = false;
      bool mzilRFARLm = false;
      bool VxdNLKJHsV = false;
      bool ogZTgZSwqq = false;
      bool ktUHHOQUug = false;
      bool bWcyepUynC = false;
      bool BCAmmFYPFq = false;
      bool lDZTBbcVGw = false;
      bool soMLsSDbnq = false;
      bool FUIooishUC = false;
      bool oBPDtYwJxC = false;
      bool MyrqJLpnsU = false;
      string FBLoEZzsJJ;
      string HRLqrCYCpY;
      string eOFJJPDWZF;
      string fGEWPWBCwB;
      string lmdimrVxAn;
      string hqDtZqwBen;
      string VcTEdANnaU;
      string BVFnlqkVVQ;
      string tGVkWnDJwY;
      string azWHfITYFm;
      string egVflLkPHh;
      string FaeOtPGhgo;
      string xDqBtByxIc;
      string XLPikMjpyK;
      string CmBOTzUINP;
      string CkrPobpUkk;
      string NnGdKtKwch;
      string eQiVmJBcht;
      string gojralTzsJ;
      string WPENxOfWDZ;
      if(FBLoEZzsJJ == egVflLkPHh){nXefkkBVcf = true;}
      else if(egVflLkPHh == FBLoEZzsJJ){VxdNLKJHsV = true;}
      if(HRLqrCYCpY == FaeOtPGhgo){ZMmpEXEODX = true;}
      else if(FaeOtPGhgo == HRLqrCYCpY){ogZTgZSwqq = true;}
      if(eOFJJPDWZF == xDqBtByxIc){aKCTKPMVYX = true;}
      else if(xDqBtByxIc == eOFJJPDWZF){ktUHHOQUug = true;}
      if(fGEWPWBCwB == XLPikMjpyK){cdYIGIKDHp = true;}
      else if(XLPikMjpyK == fGEWPWBCwB){bWcyepUynC = true;}
      if(lmdimrVxAn == CmBOTzUINP){kxWNiJbNJY = true;}
      else if(CmBOTzUINP == lmdimrVxAn){BCAmmFYPFq = true;}
      if(hqDtZqwBen == CkrPobpUkk){KRTfzdVKUU = true;}
      else if(CkrPobpUkk == hqDtZqwBen){lDZTBbcVGw = true;}
      if(VcTEdANnaU == NnGdKtKwch){YewFCumdju = true;}
      else if(NnGdKtKwch == VcTEdANnaU){soMLsSDbnq = true;}
      if(BVFnlqkVVQ == eQiVmJBcht){NAHMmcnnkB = true;}
      if(tGVkWnDJwY == gojralTzsJ){myMANLKsJc = true;}
      if(azWHfITYFm == WPENxOfWDZ){mzilRFARLm = true;}
      while(eQiVmJBcht == BVFnlqkVVQ){FUIooishUC = true;}
      while(gojralTzsJ == gojralTzsJ){oBPDtYwJxC = true;}
      while(WPENxOfWDZ == WPENxOfWDZ){MyrqJLpnsU = true;}
      if(nXefkkBVcf == true){nXefkkBVcf = false;}
      if(ZMmpEXEODX == true){ZMmpEXEODX = false;}
      if(aKCTKPMVYX == true){aKCTKPMVYX = false;}
      if(cdYIGIKDHp == true){cdYIGIKDHp = false;}
      if(kxWNiJbNJY == true){kxWNiJbNJY = false;}
      if(KRTfzdVKUU == true){KRTfzdVKUU = false;}
      if(YewFCumdju == true){YewFCumdju = false;}
      if(NAHMmcnnkB == true){NAHMmcnnkB = false;}
      if(myMANLKsJc == true){myMANLKsJc = false;}
      if(mzilRFARLm == true){mzilRFARLm = false;}
      if(VxdNLKJHsV == true){VxdNLKJHsV = false;}
      if(ogZTgZSwqq == true){ogZTgZSwqq = false;}
      if(ktUHHOQUug == true){ktUHHOQUug = false;}
      if(bWcyepUynC == true){bWcyepUynC = false;}
      if(BCAmmFYPFq == true){BCAmmFYPFq = false;}
      if(lDZTBbcVGw == true){lDZTBbcVGw = false;}
      if(soMLsSDbnq == true){soMLsSDbnq = false;}
      if(FUIooishUC == true){FUIooishUC = false;}
      if(oBPDtYwJxC == true){oBPDtYwJxC = false;}
      if(MyrqJLpnsU == true){MyrqJLpnsU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MMNOZEUSJU
{ 
  void MmMeMXcSmI()
  { 
      bool iBqNMXdnou = false;
      bool kfpSlJXUfz = false;
      bool pidseFeDYH = false;
      bool BeNaNCWSfC = false;
      bool myzXKmCDMR = false;
      bool XdrSxAWxhH = false;
      bool sZPhsFSPFj = false;
      bool lFncLriGhr = false;
      bool CaqGiXltdN = false;
      bool eYjYJMhQdE = false;
      bool UmybeSoPHE = false;
      bool QQPpFSiJyV = false;
      bool TXlqFNUPTy = false;
      bool KSbdWPbOaQ = false;
      bool hWmYxNJtTp = false;
      bool dXRHkxAHWR = false;
      bool yZwItxokSp = false;
      bool qUhhnwjNtr = false;
      bool EZwcUlATsZ = false;
      bool OUqJRIaedz = false;
      string kSORAudHro;
      string OfIlKHYUGV;
      string TRhdrIPljc;
      string jSKomwtcip;
      string GwCCFoxggD;
      string NjkXnSkjZq;
      string frXOEwUSyO;
      string MwlQPXOeYC;
      string LhEfPzhmBT;
      string LKCrKNXnxJ;
      string UeiSqQyUIQ;
      string EFLWfynqzE;
      string NgRdCBssFB;
      string zahOhgylny;
      string BNejPGyfMM;
      string ZgnsumMEPo;
      string WFOGRcXxny;
      string zBCADACgNw;
      string DNgsaRlnSu;
      string TsWybwyXSm;
      if(kSORAudHro == UeiSqQyUIQ){iBqNMXdnou = true;}
      else if(UeiSqQyUIQ == kSORAudHro){UmybeSoPHE = true;}
      if(OfIlKHYUGV == EFLWfynqzE){kfpSlJXUfz = true;}
      else if(EFLWfynqzE == OfIlKHYUGV){QQPpFSiJyV = true;}
      if(TRhdrIPljc == NgRdCBssFB){pidseFeDYH = true;}
      else if(NgRdCBssFB == TRhdrIPljc){TXlqFNUPTy = true;}
      if(jSKomwtcip == zahOhgylny){BeNaNCWSfC = true;}
      else if(zahOhgylny == jSKomwtcip){KSbdWPbOaQ = true;}
      if(GwCCFoxggD == BNejPGyfMM){myzXKmCDMR = true;}
      else if(BNejPGyfMM == GwCCFoxggD){hWmYxNJtTp = true;}
      if(NjkXnSkjZq == ZgnsumMEPo){XdrSxAWxhH = true;}
      else if(ZgnsumMEPo == NjkXnSkjZq){dXRHkxAHWR = true;}
      if(frXOEwUSyO == WFOGRcXxny){sZPhsFSPFj = true;}
      else if(WFOGRcXxny == frXOEwUSyO){yZwItxokSp = true;}
      if(MwlQPXOeYC == zBCADACgNw){lFncLriGhr = true;}
      if(LhEfPzhmBT == DNgsaRlnSu){CaqGiXltdN = true;}
      if(LKCrKNXnxJ == TsWybwyXSm){eYjYJMhQdE = true;}
      while(zBCADACgNw == MwlQPXOeYC){qUhhnwjNtr = true;}
      while(DNgsaRlnSu == DNgsaRlnSu){EZwcUlATsZ = true;}
      while(TsWybwyXSm == TsWybwyXSm){OUqJRIaedz = true;}
      if(iBqNMXdnou == true){iBqNMXdnou = false;}
      if(kfpSlJXUfz == true){kfpSlJXUfz = false;}
      if(pidseFeDYH == true){pidseFeDYH = false;}
      if(BeNaNCWSfC == true){BeNaNCWSfC = false;}
      if(myzXKmCDMR == true){myzXKmCDMR = false;}
      if(XdrSxAWxhH == true){XdrSxAWxhH = false;}
      if(sZPhsFSPFj == true){sZPhsFSPFj = false;}
      if(lFncLriGhr == true){lFncLriGhr = false;}
      if(CaqGiXltdN == true){CaqGiXltdN = false;}
      if(eYjYJMhQdE == true){eYjYJMhQdE = false;}
      if(UmybeSoPHE == true){UmybeSoPHE = false;}
      if(QQPpFSiJyV == true){QQPpFSiJyV = false;}
      if(TXlqFNUPTy == true){TXlqFNUPTy = false;}
      if(KSbdWPbOaQ == true){KSbdWPbOaQ = false;}
      if(hWmYxNJtTp == true){hWmYxNJtTp = false;}
      if(dXRHkxAHWR == true){dXRHkxAHWR = false;}
      if(yZwItxokSp == true){yZwItxokSp = false;}
      if(qUhhnwjNtr == true){qUhhnwjNtr = false;}
      if(EZwcUlATsZ == true){EZwcUlATsZ = false;}
      if(OUqJRIaedz == true){OUqJRIaedz = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CSMWYJHMUW
{ 
  void iCAZIGCutN()
  { 
      bool jJQPMSBhRK = false;
      bool AePfKunRGq = false;
      bool KlyJGscdYg = false;
      bool ecLzonntCC = false;
      bool ocSFIuzccp = false;
      bool gUIXiyLGJS = false;
      bool oXJkfXTPpW = false;
      bool HtPDChSrAL = false;
      bool sABhzKTmQq = false;
      bool bgkiMRShZk = false;
      bool bjlGilFEUA = false;
      bool eTGxZoWakU = false;
      bool DfzztZJfpk = false;
      bool PkRPjIOuyM = false;
      bool EPSPMIamJX = false;
      bool uqxEgwkmLI = false;
      bool cYNjksCpNQ = false;
      bool aabXQnNill = false;
      bool zWWSpXmMim = false;
      bool XaipjeBheS = false;
      string bUPdkmPOkC;
      string JysKiHCGNm;
      string WHrweJMLTN;
      string CmXcJOBGmb;
      string IdNrlBwUIn;
      string lZCfkTcSza;
      string ZDuXzWZQAT;
      string GwTldsszJc;
      string MrDebbjrqp;
      string lhGLCEipsP;
      string afTnhoGQBr;
      string ZdJVNgAPwX;
      string lhZKhXoHjZ;
      string JDGtCgYOxl;
      string UTpyQHYjxU;
      string ESsUcrGJGJ;
      string AzgbhVXOTg;
      string IlRKCSqatZ;
      string yEKOUpKkQJ;
      string ZOInlVmOPt;
      if(bUPdkmPOkC == afTnhoGQBr){jJQPMSBhRK = true;}
      else if(afTnhoGQBr == bUPdkmPOkC){bjlGilFEUA = true;}
      if(JysKiHCGNm == ZdJVNgAPwX){AePfKunRGq = true;}
      else if(ZdJVNgAPwX == JysKiHCGNm){eTGxZoWakU = true;}
      if(WHrweJMLTN == lhZKhXoHjZ){KlyJGscdYg = true;}
      else if(lhZKhXoHjZ == WHrweJMLTN){DfzztZJfpk = true;}
      if(CmXcJOBGmb == JDGtCgYOxl){ecLzonntCC = true;}
      else if(JDGtCgYOxl == CmXcJOBGmb){PkRPjIOuyM = true;}
      if(IdNrlBwUIn == UTpyQHYjxU){ocSFIuzccp = true;}
      else if(UTpyQHYjxU == IdNrlBwUIn){EPSPMIamJX = true;}
      if(lZCfkTcSza == ESsUcrGJGJ){gUIXiyLGJS = true;}
      else if(ESsUcrGJGJ == lZCfkTcSza){uqxEgwkmLI = true;}
      if(ZDuXzWZQAT == AzgbhVXOTg){oXJkfXTPpW = true;}
      else if(AzgbhVXOTg == ZDuXzWZQAT){cYNjksCpNQ = true;}
      if(GwTldsszJc == IlRKCSqatZ){HtPDChSrAL = true;}
      if(MrDebbjrqp == yEKOUpKkQJ){sABhzKTmQq = true;}
      if(lhGLCEipsP == ZOInlVmOPt){bgkiMRShZk = true;}
      while(IlRKCSqatZ == GwTldsszJc){aabXQnNill = true;}
      while(yEKOUpKkQJ == yEKOUpKkQJ){zWWSpXmMim = true;}
      while(ZOInlVmOPt == ZOInlVmOPt){XaipjeBheS = true;}
      if(jJQPMSBhRK == true){jJQPMSBhRK = false;}
      if(AePfKunRGq == true){AePfKunRGq = false;}
      if(KlyJGscdYg == true){KlyJGscdYg = false;}
      if(ecLzonntCC == true){ecLzonntCC = false;}
      if(ocSFIuzccp == true){ocSFIuzccp = false;}
      if(gUIXiyLGJS == true){gUIXiyLGJS = false;}
      if(oXJkfXTPpW == true){oXJkfXTPpW = false;}
      if(HtPDChSrAL == true){HtPDChSrAL = false;}
      if(sABhzKTmQq == true){sABhzKTmQq = false;}
      if(bgkiMRShZk == true){bgkiMRShZk = false;}
      if(bjlGilFEUA == true){bjlGilFEUA = false;}
      if(eTGxZoWakU == true){eTGxZoWakU = false;}
      if(DfzztZJfpk == true){DfzztZJfpk = false;}
      if(PkRPjIOuyM == true){PkRPjIOuyM = false;}
      if(EPSPMIamJX == true){EPSPMIamJX = false;}
      if(uqxEgwkmLI == true){uqxEgwkmLI = false;}
      if(cYNjksCpNQ == true){cYNjksCpNQ = false;}
      if(aabXQnNill == true){aabXQnNill = false;}
      if(zWWSpXmMim == true){zWWSpXmMim = false;}
      if(XaipjeBheS == true){XaipjeBheS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LADELKWABM
{ 
  void JEKTJkxjzp()
  { 
      bool wWtbaTISjK = false;
      bool BLDpkisIOV = false;
      bool ibKoSOTrtC = false;
      bool gxUOjzDIOR = false;
      bool TkfZbROCqw = false;
      bool HfHLUTLFJL = false;
      bool APXgdEqoGk = false;
      bool ektxCxwjMB = false;
      bool ARBxuApIsQ = false;
      bool myyDRDMzuX = false;
      bool ZZwGQVlKJX = false;
      bool XbrIKhOZkd = false;
      bool Ohuoyfmpez = false;
      bool jxnnaaUugZ = false;
      bool OezVsKFori = false;
      bool nlNRJrdGHj = false;
      bool iueKnZmDhj = false;
      bool NTsLOihZwm = false;
      bool axyuKxiWeh = false;
      bool wNmwpSzohS = false;
      string JVLuxSqVtb;
      string zcFSOQLQnZ;
      string EAxqNQfFnz;
      string bFrhiIzQtO;
      string uPZWjzXNyL;
      string pJVzuJXUhg;
      string lkguAskXiG;
      string RuVYdoaaHN;
      string rSHcIRjsmZ;
      string kdhMbKheWp;
      string NzeqtumwUF;
      string iwMTnTcEmh;
      string JrjJUmMIxT;
      string GwERArmPiN;
      string enXgqnROXH;
      string gdrnfVPVWG;
      string hdyBzmruTw;
      string gMUYDVTDSf;
      string VyjplcDQkd;
      string cRsbeAoWhD;
      if(JVLuxSqVtb == NzeqtumwUF){wWtbaTISjK = true;}
      else if(NzeqtumwUF == JVLuxSqVtb){ZZwGQVlKJX = true;}
      if(zcFSOQLQnZ == iwMTnTcEmh){BLDpkisIOV = true;}
      else if(iwMTnTcEmh == zcFSOQLQnZ){XbrIKhOZkd = true;}
      if(EAxqNQfFnz == JrjJUmMIxT){ibKoSOTrtC = true;}
      else if(JrjJUmMIxT == EAxqNQfFnz){Ohuoyfmpez = true;}
      if(bFrhiIzQtO == GwERArmPiN){gxUOjzDIOR = true;}
      else if(GwERArmPiN == bFrhiIzQtO){jxnnaaUugZ = true;}
      if(uPZWjzXNyL == enXgqnROXH){TkfZbROCqw = true;}
      else if(enXgqnROXH == uPZWjzXNyL){OezVsKFori = true;}
      if(pJVzuJXUhg == gdrnfVPVWG){HfHLUTLFJL = true;}
      else if(gdrnfVPVWG == pJVzuJXUhg){nlNRJrdGHj = true;}
      if(lkguAskXiG == hdyBzmruTw){APXgdEqoGk = true;}
      else if(hdyBzmruTw == lkguAskXiG){iueKnZmDhj = true;}
      if(RuVYdoaaHN == gMUYDVTDSf){ektxCxwjMB = true;}
      if(rSHcIRjsmZ == VyjplcDQkd){ARBxuApIsQ = true;}
      if(kdhMbKheWp == cRsbeAoWhD){myyDRDMzuX = true;}
      while(gMUYDVTDSf == RuVYdoaaHN){NTsLOihZwm = true;}
      while(VyjplcDQkd == VyjplcDQkd){axyuKxiWeh = true;}
      while(cRsbeAoWhD == cRsbeAoWhD){wNmwpSzohS = true;}
      if(wWtbaTISjK == true){wWtbaTISjK = false;}
      if(BLDpkisIOV == true){BLDpkisIOV = false;}
      if(ibKoSOTrtC == true){ibKoSOTrtC = false;}
      if(gxUOjzDIOR == true){gxUOjzDIOR = false;}
      if(TkfZbROCqw == true){TkfZbROCqw = false;}
      if(HfHLUTLFJL == true){HfHLUTLFJL = false;}
      if(APXgdEqoGk == true){APXgdEqoGk = false;}
      if(ektxCxwjMB == true){ektxCxwjMB = false;}
      if(ARBxuApIsQ == true){ARBxuApIsQ = false;}
      if(myyDRDMzuX == true){myyDRDMzuX = false;}
      if(ZZwGQVlKJX == true){ZZwGQVlKJX = false;}
      if(XbrIKhOZkd == true){XbrIKhOZkd = false;}
      if(Ohuoyfmpez == true){Ohuoyfmpez = false;}
      if(jxnnaaUugZ == true){jxnnaaUugZ = false;}
      if(OezVsKFori == true){OezVsKFori = false;}
      if(nlNRJrdGHj == true){nlNRJrdGHj = false;}
      if(iueKnZmDhj == true){iueKnZmDhj = false;}
      if(NTsLOihZwm == true){NTsLOihZwm = false;}
      if(axyuKxiWeh == true){axyuKxiWeh = false;}
      if(wNmwpSzohS == true){wNmwpSzohS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VRZEFTWZEA
{ 
  void umrzglQijZ()
  { 
      bool AQNWfKyjbA = false;
      bool FpSJJsZREZ = false;
      bool lqRWqFOldJ = false;
      bool EMqfQrPdLM = false;
      bool wJLmdptZnp = false;
      bool IpUYRIzqeZ = false;
      bool uLGSurzlrR = false;
      bool cuFrccferA = false;
      bool KoynBOrROH = false;
      bool sYykQkCrqk = false;
      bool pIgWMMjbuo = false;
      bool cUyePGMRRf = false;
      bool AhWjXDxUar = false;
      bool ZmmGKCHbao = false;
      bool ePVZeYaTVJ = false;
      bool XjaKkIkxyJ = false;
      bool OPBDOogqTM = false;
      bool zkqcBkMxFz = false;
      bool bcyRToLbVB = false;
      bool RKcQZPORKo = false;
      string jpjdXzIteG;
      string hXNCzpkBVG;
      string TJZipefTao;
      string blVheifnZz;
      string HEFynhXtmK;
      string PyUJNZPBnr;
      string ToTpdwBeXP;
      string KwbVzOEbtH;
      string GlgQYdQMTE;
      string TdzgfRkOTr;
      string grVWzTVzJz;
      string VmHglhICRz;
      string eoLTwfwuog;
      string dpHBaNPJRF;
      string IRCUifWSty;
      string JiwTNqgCZS;
      string WZJcUefFFX;
      string RykMqMlpjQ;
      string JdAqXusnni;
      string gMiyihHZzD;
      if(jpjdXzIteG == grVWzTVzJz){AQNWfKyjbA = true;}
      else if(grVWzTVzJz == jpjdXzIteG){pIgWMMjbuo = true;}
      if(hXNCzpkBVG == VmHglhICRz){FpSJJsZREZ = true;}
      else if(VmHglhICRz == hXNCzpkBVG){cUyePGMRRf = true;}
      if(TJZipefTao == eoLTwfwuog){lqRWqFOldJ = true;}
      else if(eoLTwfwuog == TJZipefTao){AhWjXDxUar = true;}
      if(blVheifnZz == dpHBaNPJRF){EMqfQrPdLM = true;}
      else if(dpHBaNPJRF == blVheifnZz){ZmmGKCHbao = true;}
      if(HEFynhXtmK == IRCUifWSty){wJLmdptZnp = true;}
      else if(IRCUifWSty == HEFynhXtmK){ePVZeYaTVJ = true;}
      if(PyUJNZPBnr == JiwTNqgCZS){IpUYRIzqeZ = true;}
      else if(JiwTNqgCZS == PyUJNZPBnr){XjaKkIkxyJ = true;}
      if(ToTpdwBeXP == WZJcUefFFX){uLGSurzlrR = true;}
      else if(WZJcUefFFX == ToTpdwBeXP){OPBDOogqTM = true;}
      if(KwbVzOEbtH == RykMqMlpjQ){cuFrccferA = true;}
      if(GlgQYdQMTE == JdAqXusnni){KoynBOrROH = true;}
      if(TdzgfRkOTr == gMiyihHZzD){sYykQkCrqk = true;}
      while(RykMqMlpjQ == KwbVzOEbtH){zkqcBkMxFz = true;}
      while(JdAqXusnni == JdAqXusnni){bcyRToLbVB = true;}
      while(gMiyihHZzD == gMiyihHZzD){RKcQZPORKo = true;}
      if(AQNWfKyjbA == true){AQNWfKyjbA = false;}
      if(FpSJJsZREZ == true){FpSJJsZREZ = false;}
      if(lqRWqFOldJ == true){lqRWqFOldJ = false;}
      if(EMqfQrPdLM == true){EMqfQrPdLM = false;}
      if(wJLmdptZnp == true){wJLmdptZnp = false;}
      if(IpUYRIzqeZ == true){IpUYRIzqeZ = false;}
      if(uLGSurzlrR == true){uLGSurzlrR = false;}
      if(cuFrccferA == true){cuFrccferA = false;}
      if(KoynBOrROH == true){KoynBOrROH = false;}
      if(sYykQkCrqk == true){sYykQkCrqk = false;}
      if(pIgWMMjbuo == true){pIgWMMjbuo = false;}
      if(cUyePGMRRf == true){cUyePGMRRf = false;}
      if(AhWjXDxUar == true){AhWjXDxUar = false;}
      if(ZmmGKCHbao == true){ZmmGKCHbao = false;}
      if(ePVZeYaTVJ == true){ePVZeYaTVJ = false;}
      if(XjaKkIkxyJ == true){XjaKkIkxyJ = false;}
      if(OPBDOogqTM == true){OPBDOogqTM = false;}
      if(zkqcBkMxFz == true){zkqcBkMxFz = false;}
      if(bcyRToLbVB == true){bcyRToLbVB = false;}
      if(RKcQZPORKo == true){RKcQZPORKo = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RFGVDYHGTK
{ 
  void iLKbRCAeac()
  { 
      bool OZifGODRDU = false;
      bool RIYhofTMXy = false;
      bool UhzLafXiIF = false;
      bool HbHSaIQNTn = false;
      bool gkXopEZWoi = false;
      bool WUdOigaXEa = false;
      bool pTJxMpAFHf = false;
      bool PFsjRiJJVW = false;
      bool GBtZhmxrjt = false;
      bool hsJsxMbabA = false;
      bool DTOITRbtxz = false;
      bool WQBgTnZiOZ = false;
      bool rHVNyWuAlE = false;
      bool HihUSmlkFQ = false;
      bool KDujWQqbYI = false;
      bool lrLBIwxBJx = false;
      bool VbIrrnigOg = false;
      bool CodyPnMkqh = false;
      bool eTacROXfMW = false;
      bool ojaItiuCri = false;
      string dfVrokIjkz;
      string SSyIDaiUnV;
      string eIPwNbZhXo;
      string RlOBQbpFKT;
      string ZtFkDGqlOc;
      string MkSQUVQYdN;
      string LZMuhfExra;
      string jdKTujDesG;
      string jmSucfTyft;
      string LQaxIAhJCW;
      string CtsnrhayVj;
      string SlfXcXHQSC;
      string LGQyPTCNKt;
      string BtbVDpsdCt;
      string EVbwbYjDTk;
      string rRpALucuYX;
      string eNmahXJKTm;
      string ddNXxpdUVp;
      string iztZCjDbKI;
      string MyFeOqhBND;
      if(dfVrokIjkz == CtsnrhayVj){OZifGODRDU = true;}
      else if(CtsnrhayVj == dfVrokIjkz){DTOITRbtxz = true;}
      if(SSyIDaiUnV == SlfXcXHQSC){RIYhofTMXy = true;}
      else if(SlfXcXHQSC == SSyIDaiUnV){WQBgTnZiOZ = true;}
      if(eIPwNbZhXo == LGQyPTCNKt){UhzLafXiIF = true;}
      else if(LGQyPTCNKt == eIPwNbZhXo){rHVNyWuAlE = true;}
      if(RlOBQbpFKT == BtbVDpsdCt){HbHSaIQNTn = true;}
      else if(BtbVDpsdCt == RlOBQbpFKT){HihUSmlkFQ = true;}
      if(ZtFkDGqlOc == EVbwbYjDTk){gkXopEZWoi = true;}
      else if(EVbwbYjDTk == ZtFkDGqlOc){KDujWQqbYI = true;}
      if(MkSQUVQYdN == rRpALucuYX){WUdOigaXEa = true;}
      else if(rRpALucuYX == MkSQUVQYdN){lrLBIwxBJx = true;}
      if(LZMuhfExra == eNmahXJKTm){pTJxMpAFHf = true;}
      else if(eNmahXJKTm == LZMuhfExra){VbIrrnigOg = true;}
      if(jdKTujDesG == ddNXxpdUVp){PFsjRiJJVW = true;}
      if(jmSucfTyft == iztZCjDbKI){GBtZhmxrjt = true;}
      if(LQaxIAhJCW == MyFeOqhBND){hsJsxMbabA = true;}
      while(ddNXxpdUVp == jdKTujDesG){CodyPnMkqh = true;}
      while(iztZCjDbKI == iztZCjDbKI){eTacROXfMW = true;}
      while(MyFeOqhBND == MyFeOqhBND){ojaItiuCri = true;}
      if(OZifGODRDU == true){OZifGODRDU = false;}
      if(RIYhofTMXy == true){RIYhofTMXy = false;}
      if(UhzLafXiIF == true){UhzLafXiIF = false;}
      if(HbHSaIQNTn == true){HbHSaIQNTn = false;}
      if(gkXopEZWoi == true){gkXopEZWoi = false;}
      if(WUdOigaXEa == true){WUdOigaXEa = false;}
      if(pTJxMpAFHf == true){pTJxMpAFHf = false;}
      if(PFsjRiJJVW == true){PFsjRiJJVW = false;}
      if(GBtZhmxrjt == true){GBtZhmxrjt = false;}
      if(hsJsxMbabA == true){hsJsxMbabA = false;}
      if(DTOITRbtxz == true){DTOITRbtxz = false;}
      if(WQBgTnZiOZ == true){WQBgTnZiOZ = false;}
      if(rHVNyWuAlE == true){rHVNyWuAlE = false;}
      if(HihUSmlkFQ == true){HihUSmlkFQ = false;}
      if(KDujWQqbYI == true){KDujWQqbYI = false;}
      if(lrLBIwxBJx == true){lrLBIwxBJx = false;}
      if(VbIrrnigOg == true){VbIrrnigOg = false;}
      if(CodyPnMkqh == true){CodyPnMkqh = false;}
      if(eTacROXfMW == true){eTacROXfMW = false;}
      if(ojaItiuCri == true){ojaItiuCri = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OVFQLBCOPU
{ 
  void ozMWPseXtU()
  { 
      bool tugrOFYaqI = false;
      bool zOJOzZPnzq = false;
      bool PWpYtScdFY = false;
      bool PcYapZuZIC = false;
      bool arNMfkToMF = false;
      bool YhbcqWEHyw = false;
      bool buURrWpiXZ = false;
      bool eofxPXJdsI = false;
      bool znRzAhAwiS = false;
      bool UmaMZfWuTJ = false;
      bool tjQmaiCeMT = false;
      bool NaOpxmdVRK = false;
      bool VtzEEqNSWj = false;
      bool AOJiOOWeXQ = false;
      bool fViOFtXkPl = false;
      bool CBcJnAkMhi = false;
      bool ViHbTzRfah = false;
      bool QzxBdzKkzK = false;
      bool TbDjhykzuN = false;
      bool dKwouwOHuG = false;
      string qLuBOoPuSk;
      string dSDUyxYxoW;
      string CNepnxlZoA;
      string qgwhllmlEe;
      string JwzhLNfzRI;
      string CfEMOdgFNR;
      string DpGStwWVhM;
      string KlrJMGNwdZ;
      string QlHgyKbiSI;
      string FhYmbTpgnO;
      string qKRFyPntAH;
      string QjamAQhYTi;
      string TmDZGcNFhs;
      string QgIPiGUChh;
      string WhiKqVGklK;
      string kQMEtDGqzV;
      string ChLHqjaLEs;
      string wfqlMlErhk;
      string VbSXHATEUe;
      string zcDMfnVbTH;
      if(qLuBOoPuSk == qKRFyPntAH){tugrOFYaqI = true;}
      else if(qKRFyPntAH == qLuBOoPuSk){tjQmaiCeMT = true;}
      if(dSDUyxYxoW == QjamAQhYTi){zOJOzZPnzq = true;}
      else if(QjamAQhYTi == dSDUyxYxoW){NaOpxmdVRK = true;}
      if(CNepnxlZoA == TmDZGcNFhs){PWpYtScdFY = true;}
      else if(TmDZGcNFhs == CNepnxlZoA){VtzEEqNSWj = true;}
      if(qgwhllmlEe == QgIPiGUChh){PcYapZuZIC = true;}
      else if(QgIPiGUChh == qgwhllmlEe){AOJiOOWeXQ = true;}
      if(JwzhLNfzRI == WhiKqVGklK){arNMfkToMF = true;}
      else if(WhiKqVGklK == JwzhLNfzRI){fViOFtXkPl = true;}
      if(CfEMOdgFNR == kQMEtDGqzV){YhbcqWEHyw = true;}
      else if(kQMEtDGqzV == CfEMOdgFNR){CBcJnAkMhi = true;}
      if(DpGStwWVhM == ChLHqjaLEs){buURrWpiXZ = true;}
      else if(ChLHqjaLEs == DpGStwWVhM){ViHbTzRfah = true;}
      if(KlrJMGNwdZ == wfqlMlErhk){eofxPXJdsI = true;}
      if(QlHgyKbiSI == VbSXHATEUe){znRzAhAwiS = true;}
      if(FhYmbTpgnO == zcDMfnVbTH){UmaMZfWuTJ = true;}
      while(wfqlMlErhk == KlrJMGNwdZ){QzxBdzKkzK = true;}
      while(VbSXHATEUe == VbSXHATEUe){TbDjhykzuN = true;}
      while(zcDMfnVbTH == zcDMfnVbTH){dKwouwOHuG = true;}
      if(tugrOFYaqI == true){tugrOFYaqI = false;}
      if(zOJOzZPnzq == true){zOJOzZPnzq = false;}
      if(PWpYtScdFY == true){PWpYtScdFY = false;}
      if(PcYapZuZIC == true){PcYapZuZIC = false;}
      if(arNMfkToMF == true){arNMfkToMF = false;}
      if(YhbcqWEHyw == true){YhbcqWEHyw = false;}
      if(buURrWpiXZ == true){buURrWpiXZ = false;}
      if(eofxPXJdsI == true){eofxPXJdsI = false;}
      if(znRzAhAwiS == true){znRzAhAwiS = false;}
      if(UmaMZfWuTJ == true){UmaMZfWuTJ = false;}
      if(tjQmaiCeMT == true){tjQmaiCeMT = false;}
      if(NaOpxmdVRK == true){NaOpxmdVRK = false;}
      if(VtzEEqNSWj == true){VtzEEqNSWj = false;}
      if(AOJiOOWeXQ == true){AOJiOOWeXQ = false;}
      if(fViOFtXkPl == true){fViOFtXkPl = false;}
      if(CBcJnAkMhi == true){CBcJnAkMhi = false;}
      if(ViHbTzRfah == true){ViHbTzRfah = false;}
      if(QzxBdzKkzK == true){QzxBdzKkzK = false;}
      if(TbDjhykzuN == true){TbDjhykzuN = false;}
      if(dKwouwOHuG == true){dKwouwOHuG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WXVCGFGRWG
{ 
  void KTrfMiLdZm()
  { 
      bool zwiiFIBzFl = false;
      bool nnylgEOwbM = false;
      bool zsUcsNIVUm = false;
      bool wUfzwwgPGU = false;
      bool DGpTFxDrAj = false;
      bool DVbqgFaGpq = false;
      bool AedSoSHypj = false;
      bool WzoHJsDBXw = false;
      bool KPVkiZsoRp = false;
      bool UaYOUYbXdl = false;
      bool oECPMEfThn = false;
      bool BZhtgjJsiP = false;
      bool ZSgzPwZamk = false;
      bool RfWkGFNFXd = false;
      bool tYGdouregj = false;
      bool OhnOMVWCAQ = false;
      bool eFgwWVDxma = false;
      bool WDVEVALDKU = false;
      bool DusCViqHYj = false;
      bool hKFGznqKGS = false;
      string wpGpGfxKJf;
      string uEysKAIumq;
      string qkofOaETFq;
      string qujtSmeHgq;
      string XTtkmOjZoJ;
      string qmixxLBZae;
      string adFSozUDaW;
      string UBNuSWqDsz;
      string peeuzEMmwH;
      string lEfBPtObwm;
      string tfyRopYmdZ;
      string UxganuzJSk;
      string mhHTjPULfY;
      string IOHFFKfMGJ;
      string eYIVmoXENT;
      string uEILzMYehY;
      string UMHFRFoaVd;
      string OmDJFNYkgu;
      string rQyLcEFwFs;
      string kBegnRRDEg;
      if(wpGpGfxKJf == tfyRopYmdZ){zwiiFIBzFl = true;}
      else if(tfyRopYmdZ == wpGpGfxKJf){oECPMEfThn = true;}
      if(uEysKAIumq == UxganuzJSk){nnylgEOwbM = true;}
      else if(UxganuzJSk == uEysKAIumq){BZhtgjJsiP = true;}
      if(qkofOaETFq == mhHTjPULfY){zsUcsNIVUm = true;}
      else if(mhHTjPULfY == qkofOaETFq){ZSgzPwZamk = true;}
      if(qujtSmeHgq == IOHFFKfMGJ){wUfzwwgPGU = true;}
      else if(IOHFFKfMGJ == qujtSmeHgq){RfWkGFNFXd = true;}
      if(XTtkmOjZoJ == eYIVmoXENT){DGpTFxDrAj = true;}
      else if(eYIVmoXENT == XTtkmOjZoJ){tYGdouregj = true;}
      if(qmixxLBZae == uEILzMYehY){DVbqgFaGpq = true;}
      else if(uEILzMYehY == qmixxLBZae){OhnOMVWCAQ = true;}
      if(adFSozUDaW == UMHFRFoaVd){AedSoSHypj = true;}
      else if(UMHFRFoaVd == adFSozUDaW){eFgwWVDxma = true;}
      if(UBNuSWqDsz == OmDJFNYkgu){WzoHJsDBXw = true;}
      if(peeuzEMmwH == rQyLcEFwFs){KPVkiZsoRp = true;}
      if(lEfBPtObwm == kBegnRRDEg){UaYOUYbXdl = true;}
      while(OmDJFNYkgu == UBNuSWqDsz){WDVEVALDKU = true;}
      while(rQyLcEFwFs == rQyLcEFwFs){DusCViqHYj = true;}
      while(kBegnRRDEg == kBegnRRDEg){hKFGznqKGS = true;}
      if(zwiiFIBzFl == true){zwiiFIBzFl = false;}
      if(nnylgEOwbM == true){nnylgEOwbM = false;}
      if(zsUcsNIVUm == true){zsUcsNIVUm = false;}
      if(wUfzwwgPGU == true){wUfzwwgPGU = false;}
      if(DGpTFxDrAj == true){DGpTFxDrAj = false;}
      if(DVbqgFaGpq == true){DVbqgFaGpq = false;}
      if(AedSoSHypj == true){AedSoSHypj = false;}
      if(WzoHJsDBXw == true){WzoHJsDBXw = false;}
      if(KPVkiZsoRp == true){KPVkiZsoRp = false;}
      if(UaYOUYbXdl == true){UaYOUYbXdl = false;}
      if(oECPMEfThn == true){oECPMEfThn = false;}
      if(BZhtgjJsiP == true){BZhtgjJsiP = false;}
      if(ZSgzPwZamk == true){ZSgzPwZamk = false;}
      if(RfWkGFNFXd == true){RfWkGFNFXd = false;}
      if(tYGdouregj == true){tYGdouregj = false;}
      if(OhnOMVWCAQ == true){OhnOMVWCAQ = false;}
      if(eFgwWVDxma == true){eFgwWVDxma = false;}
      if(WDVEVALDKU == true){WDVEVALDKU = false;}
      if(DusCViqHYj == true){DusCViqHYj = false;}
      if(hKFGznqKGS == true){hKFGznqKGS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WJXIZNOWJD
{ 
  void gOpdVBLklQ()
  { 
      bool qKxCsgWSng = false;
      bool TcaZiUOFzh = false;
      bool tHqgkoewbC = false;
      bool PNyFhRYdjI = false;
      bool yMbbceYVnw = false;
      bool dShkKiTPRE = false;
      bool CaqlRGdVqR = false;
      bool wjUpigeODF = false;
      bool PIIeXweNtz = false;
      bool yIMfGXVpKW = false;
      bool ACeVAtyrBd = false;
      bool DELLnsRLNU = false;
      bool aPbrSmWojK = false;
      bool LoEgEMdled = false;
      bool ymMPusxBqN = false;
      bool GJboukamDM = false;
      bool WIHqROAeGa = false;
      bool OffjWnDLwZ = false;
      bool pVxWEBMOkT = false;
      bool lqiSGoeVyf = false;
      string sfinaRKzAx;
      string XGjGQtSoaO;
      string kUkJDubENB;
      string BYcIUkPCWC;
      string kfNPOKJNUd;
      string EpURVXuZjf;
      string pFlzuycalf;
      string dPSYdFGUts;
      string kNjixLDLmw;
      string ppSpLesOTP;
      string giboAnXwnP;
      string RZequbVCCj;
      string uRnoonxbMy;
      string MrBdMCluiI;
      string nHWWePLbui;
      string IpMjUOtlzL;
      string quioDZYAer;
      string oaTsumuRKZ;
      string cVgdpVeQJx;
      string HAuWRsXgSF;
      if(sfinaRKzAx == giboAnXwnP){qKxCsgWSng = true;}
      else if(giboAnXwnP == sfinaRKzAx){ACeVAtyrBd = true;}
      if(XGjGQtSoaO == RZequbVCCj){TcaZiUOFzh = true;}
      else if(RZequbVCCj == XGjGQtSoaO){DELLnsRLNU = true;}
      if(kUkJDubENB == uRnoonxbMy){tHqgkoewbC = true;}
      else if(uRnoonxbMy == kUkJDubENB){aPbrSmWojK = true;}
      if(BYcIUkPCWC == MrBdMCluiI){PNyFhRYdjI = true;}
      else if(MrBdMCluiI == BYcIUkPCWC){LoEgEMdled = true;}
      if(kfNPOKJNUd == nHWWePLbui){yMbbceYVnw = true;}
      else if(nHWWePLbui == kfNPOKJNUd){ymMPusxBqN = true;}
      if(EpURVXuZjf == IpMjUOtlzL){dShkKiTPRE = true;}
      else if(IpMjUOtlzL == EpURVXuZjf){GJboukamDM = true;}
      if(pFlzuycalf == quioDZYAer){CaqlRGdVqR = true;}
      else if(quioDZYAer == pFlzuycalf){WIHqROAeGa = true;}
      if(dPSYdFGUts == oaTsumuRKZ){wjUpigeODF = true;}
      if(kNjixLDLmw == cVgdpVeQJx){PIIeXweNtz = true;}
      if(ppSpLesOTP == HAuWRsXgSF){yIMfGXVpKW = true;}
      while(oaTsumuRKZ == dPSYdFGUts){OffjWnDLwZ = true;}
      while(cVgdpVeQJx == cVgdpVeQJx){pVxWEBMOkT = true;}
      while(HAuWRsXgSF == HAuWRsXgSF){lqiSGoeVyf = true;}
      if(qKxCsgWSng == true){qKxCsgWSng = false;}
      if(TcaZiUOFzh == true){TcaZiUOFzh = false;}
      if(tHqgkoewbC == true){tHqgkoewbC = false;}
      if(PNyFhRYdjI == true){PNyFhRYdjI = false;}
      if(yMbbceYVnw == true){yMbbceYVnw = false;}
      if(dShkKiTPRE == true){dShkKiTPRE = false;}
      if(CaqlRGdVqR == true){CaqlRGdVqR = false;}
      if(wjUpigeODF == true){wjUpigeODF = false;}
      if(PIIeXweNtz == true){PIIeXweNtz = false;}
      if(yIMfGXVpKW == true){yIMfGXVpKW = false;}
      if(ACeVAtyrBd == true){ACeVAtyrBd = false;}
      if(DELLnsRLNU == true){DELLnsRLNU = false;}
      if(aPbrSmWojK == true){aPbrSmWojK = false;}
      if(LoEgEMdled == true){LoEgEMdled = false;}
      if(ymMPusxBqN == true){ymMPusxBqN = false;}
      if(GJboukamDM == true){GJboukamDM = false;}
      if(WIHqROAeGa == true){WIHqROAeGa = false;}
      if(OffjWnDLwZ == true){OffjWnDLwZ = false;}
      if(pVxWEBMOkT == true){pVxWEBMOkT = false;}
      if(lqiSGoeVyf == true){lqiSGoeVyf = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SSCLIOCLDW
{ 
  void kcWcFnxPSc()
  { 
      bool PKPjkYeCxq = false;
      bool oEdTmPkBRx = false;
      bool IDRbQMKaQn = false;
      bool yYJFphRXuR = false;
      bool WRrccODRcA = false;
      bool fLczcLsnTX = false;
      bool NEkIrPtYcF = false;
      bool xNFAaqLnyC = false;
      bool IsxQImwKce = false;
      bool odEWDSfikM = false;
      bool TenXCSGbYl = false;
      bool zEoQPUHUAF = false;
      bool TjwyPSIhxI = false;
      bool KcDkhdEYNo = false;
      bool fTkpejomHh = false;
      bool KLhesjhpHj = false;
      bool JXeQaWADhk = false;
      bool QhGLYAyFSe = false;
      bool dUZOxytXry = false;
      bool uNQdfInUPH = false;
      string MUfEulFmBM;
      string VYLFwrWzzX;
      string ABaoemHxgI;
      string OZehFnxDoI;
      string jPVQHYsbxP;
      string HgjHKLtqZH;
      string eHFsxwscAI;
      string uTKpnjeJhF;
      string tfARAhoWXt;
      string sFchdbKbSK;
      string eQboUnWtTZ;
      string fkJTNyHnsP;
      string HWEFOoqcLL;
      string ZDqWZdELwM;
      string KuDxOIwJTe;
      string UWiCDILVZf;
      string wthIAcslsi;
      string twqgFLTHqG;
      string rgIfpCToTR;
      string ShkQPQNtxk;
      if(MUfEulFmBM == eQboUnWtTZ){PKPjkYeCxq = true;}
      else if(eQboUnWtTZ == MUfEulFmBM){TenXCSGbYl = true;}
      if(VYLFwrWzzX == fkJTNyHnsP){oEdTmPkBRx = true;}
      else if(fkJTNyHnsP == VYLFwrWzzX){zEoQPUHUAF = true;}
      if(ABaoemHxgI == HWEFOoqcLL){IDRbQMKaQn = true;}
      else if(HWEFOoqcLL == ABaoemHxgI){TjwyPSIhxI = true;}
      if(OZehFnxDoI == ZDqWZdELwM){yYJFphRXuR = true;}
      else if(ZDqWZdELwM == OZehFnxDoI){KcDkhdEYNo = true;}
      if(jPVQHYsbxP == KuDxOIwJTe){WRrccODRcA = true;}
      else if(KuDxOIwJTe == jPVQHYsbxP){fTkpejomHh = true;}
      if(HgjHKLtqZH == UWiCDILVZf){fLczcLsnTX = true;}
      else if(UWiCDILVZf == HgjHKLtqZH){KLhesjhpHj = true;}
      if(eHFsxwscAI == wthIAcslsi){NEkIrPtYcF = true;}
      else if(wthIAcslsi == eHFsxwscAI){JXeQaWADhk = true;}
      if(uTKpnjeJhF == twqgFLTHqG){xNFAaqLnyC = true;}
      if(tfARAhoWXt == rgIfpCToTR){IsxQImwKce = true;}
      if(sFchdbKbSK == ShkQPQNtxk){odEWDSfikM = true;}
      while(twqgFLTHqG == uTKpnjeJhF){QhGLYAyFSe = true;}
      while(rgIfpCToTR == rgIfpCToTR){dUZOxytXry = true;}
      while(ShkQPQNtxk == ShkQPQNtxk){uNQdfInUPH = true;}
      if(PKPjkYeCxq == true){PKPjkYeCxq = false;}
      if(oEdTmPkBRx == true){oEdTmPkBRx = false;}
      if(IDRbQMKaQn == true){IDRbQMKaQn = false;}
      if(yYJFphRXuR == true){yYJFphRXuR = false;}
      if(WRrccODRcA == true){WRrccODRcA = false;}
      if(fLczcLsnTX == true){fLczcLsnTX = false;}
      if(NEkIrPtYcF == true){NEkIrPtYcF = false;}
      if(xNFAaqLnyC == true){xNFAaqLnyC = false;}
      if(IsxQImwKce == true){IsxQImwKce = false;}
      if(odEWDSfikM == true){odEWDSfikM = false;}
      if(TenXCSGbYl == true){TenXCSGbYl = false;}
      if(zEoQPUHUAF == true){zEoQPUHUAF = false;}
      if(TjwyPSIhxI == true){TjwyPSIhxI = false;}
      if(KcDkhdEYNo == true){KcDkhdEYNo = false;}
      if(fTkpejomHh == true){fTkpejomHh = false;}
      if(KLhesjhpHj == true){KLhesjhpHj = false;}
      if(JXeQaWADhk == true){JXeQaWADhk = false;}
      if(QhGLYAyFSe == true){QhGLYAyFSe = false;}
      if(dUZOxytXry == true){dUZOxytXry = false;}
      if(uNQdfInUPH == true){uNQdfInUPH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JDBZTHVQNJ
{ 
  void syhDuGPFwM()
  { 
      bool oaQwIGMweL = false;
      bool JWsfujpKuA = false;
      bool krwqIgjmuN = false;
      bool TqlpokEZXw = false;
      bool BKccqSLKXU = false;
      bool nqYTmwdYma = false;
      bool VYBRrSkDqJ = false;
      bool eQmyzaOSzp = false;
      bool UYPZZbSRCr = false;
      bool GemxZcJLkY = false;
      bool gITNoXEZUM = false;
      bool hKYwFwIaqP = false;
      bool zSAyCAdpYV = false;
      bool QDhaJMFCjC = false;
      bool WrxpKuGpqd = false;
      bool ThUUYOIVKo = false;
      bool sOByuebyms = false;
      bool qFpnsGUJmG = false;
      bool tmyuBihNBu = false;
      bool DiquIIqzuf = false;
      string ZQTRbEPIbY;
      string cBXTjlCscS;
      string LSurmXuEYa;
      string XBbEWruBPF;
      string WgHHYVloWU;
      string iFpcZHUbeD;
      string ILiBBaZTxq;
      string qAcXudKmTQ;
      string lGgDrSLydP;
      string WTWMwLyGGx;
      string uSqedhbRgu;
      string xgHNRBdeJh;
      string PbohTXdQaj;
      string fDgWUSOCYJ;
      string psQiRoLItu;
      string hSwXhTcJKG;
      string ELqxOsAkry;
      string mbGArVKGPd;
      string LUVfCNHIoB;
      string oFqSEdCTTY;
      if(ZQTRbEPIbY == uSqedhbRgu){oaQwIGMweL = true;}
      else if(uSqedhbRgu == ZQTRbEPIbY){gITNoXEZUM = true;}
      if(cBXTjlCscS == xgHNRBdeJh){JWsfujpKuA = true;}
      else if(xgHNRBdeJh == cBXTjlCscS){hKYwFwIaqP = true;}
      if(LSurmXuEYa == PbohTXdQaj){krwqIgjmuN = true;}
      else if(PbohTXdQaj == LSurmXuEYa){zSAyCAdpYV = true;}
      if(XBbEWruBPF == fDgWUSOCYJ){TqlpokEZXw = true;}
      else if(fDgWUSOCYJ == XBbEWruBPF){QDhaJMFCjC = true;}
      if(WgHHYVloWU == psQiRoLItu){BKccqSLKXU = true;}
      else if(psQiRoLItu == WgHHYVloWU){WrxpKuGpqd = true;}
      if(iFpcZHUbeD == hSwXhTcJKG){nqYTmwdYma = true;}
      else if(hSwXhTcJKG == iFpcZHUbeD){ThUUYOIVKo = true;}
      if(ILiBBaZTxq == ELqxOsAkry){VYBRrSkDqJ = true;}
      else if(ELqxOsAkry == ILiBBaZTxq){sOByuebyms = true;}
      if(qAcXudKmTQ == mbGArVKGPd){eQmyzaOSzp = true;}
      if(lGgDrSLydP == LUVfCNHIoB){UYPZZbSRCr = true;}
      if(WTWMwLyGGx == oFqSEdCTTY){GemxZcJLkY = true;}
      while(mbGArVKGPd == qAcXudKmTQ){qFpnsGUJmG = true;}
      while(LUVfCNHIoB == LUVfCNHIoB){tmyuBihNBu = true;}
      while(oFqSEdCTTY == oFqSEdCTTY){DiquIIqzuf = true;}
      if(oaQwIGMweL == true){oaQwIGMweL = false;}
      if(JWsfujpKuA == true){JWsfujpKuA = false;}
      if(krwqIgjmuN == true){krwqIgjmuN = false;}
      if(TqlpokEZXw == true){TqlpokEZXw = false;}
      if(BKccqSLKXU == true){BKccqSLKXU = false;}
      if(nqYTmwdYma == true){nqYTmwdYma = false;}
      if(VYBRrSkDqJ == true){VYBRrSkDqJ = false;}
      if(eQmyzaOSzp == true){eQmyzaOSzp = false;}
      if(UYPZZbSRCr == true){UYPZZbSRCr = false;}
      if(GemxZcJLkY == true){GemxZcJLkY = false;}
      if(gITNoXEZUM == true){gITNoXEZUM = false;}
      if(hKYwFwIaqP == true){hKYwFwIaqP = false;}
      if(zSAyCAdpYV == true){zSAyCAdpYV = false;}
      if(QDhaJMFCjC == true){QDhaJMFCjC = false;}
      if(WrxpKuGpqd == true){WrxpKuGpqd = false;}
      if(ThUUYOIVKo == true){ThUUYOIVKo = false;}
      if(sOByuebyms == true){sOByuebyms = false;}
      if(qFpnsGUJmG == true){qFpnsGUJmG = false;}
      if(tmyuBihNBu == true){tmyuBihNBu = false;}
      if(DiquIIqzuf == true){DiquIIqzuf = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IARMWIGATD
{ 
  void zFfVSjnUaI()
  { 
      bool ySiucVruZz = false;
      bool BbzSBcMMBD = false;
      bool jLKsrWkfJt = false;
      bool PIFZFSIqIX = false;
      bool OzsLpYBlwG = false;
      bool OebQHmHoDG = false;
      bool jMkfjsWZNh = false;
      bool TUEcokSzDS = false;
      bool UziDfkgnRS = false;
      bool KTnCmBNMbw = false;
      bool CbXOmsaybW = false;
      bool TIljkkchzz = false;
      bool MhIuXAfBpm = false;
      bool WFWGPsaUQm = false;
      bool JhMuyCgDQC = false;
      bool YfMcjUScql = false;
      bool MYIozOkfUr = false;
      bool JHRModUNwU = false;
      bool SbjmOqrtVu = false;
      bool ZyddZkZXui = false;
      string kENIrRnpHo;
      string SotyhxhDlH;
      string WyBBLlCpXV;
      string prFXXbqiqH;
      string ZFZrdgceKW;
      string hnQgFmDLwF;
      string OxRlzZiWZF;
      string GzMegRXIWd;
      string JXfBmCbYSk;
      string YelkrcmnsE;
      string yFFEPhakzn;
      string OuoRwqRQyw;
      string xKEngbMamE;
      string djPSfDSblE;
      string apYdWdpUnB;
      string KWowGnBQWm;
      string UzyuJeeWks;
      string XodsVHlZQD;
      string lrmksGtTLr;
      string BBEyjaqcTq;
      if(kENIrRnpHo == yFFEPhakzn){ySiucVruZz = true;}
      else if(yFFEPhakzn == kENIrRnpHo){CbXOmsaybW = true;}
      if(SotyhxhDlH == OuoRwqRQyw){BbzSBcMMBD = true;}
      else if(OuoRwqRQyw == SotyhxhDlH){TIljkkchzz = true;}
      if(WyBBLlCpXV == xKEngbMamE){jLKsrWkfJt = true;}
      else if(xKEngbMamE == WyBBLlCpXV){MhIuXAfBpm = true;}
      if(prFXXbqiqH == djPSfDSblE){PIFZFSIqIX = true;}
      else if(djPSfDSblE == prFXXbqiqH){WFWGPsaUQm = true;}
      if(ZFZrdgceKW == apYdWdpUnB){OzsLpYBlwG = true;}
      else if(apYdWdpUnB == ZFZrdgceKW){JhMuyCgDQC = true;}
      if(hnQgFmDLwF == KWowGnBQWm){OebQHmHoDG = true;}
      else if(KWowGnBQWm == hnQgFmDLwF){YfMcjUScql = true;}
      if(OxRlzZiWZF == UzyuJeeWks){jMkfjsWZNh = true;}
      else if(UzyuJeeWks == OxRlzZiWZF){MYIozOkfUr = true;}
      if(GzMegRXIWd == XodsVHlZQD){TUEcokSzDS = true;}
      if(JXfBmCbYSk == lrmksGtTLr){UziDfkgnRS = true;}
      if(YelkrcmnsE == BBEyjaqcTq){KTnCmBNMbw = true;}
      while(XodsVHlZQD == GzMegRXIWd){JHRModUNwU = true;}
      while(lrmksGtTLr == lrmksGtTLr){SbjmOqrtVu = true;}
      while(BBEyjaqcTq == BBEyjaqcTq){ZyddZkZXui = true;}
      if(ySiucVruZz == true){ySiucVruZz = false;}
      if(BbzSBcMMBD == true){BbzSBcMMBD = false;}
      if(jLKsrWkfJt == true){jLKsrWkfJt = false;}
      if(PIFZFSIqIX == true){PIFZFSIqIX = false;}
      if(OzsLpYBlwG == true){OzsLpYBlwG = false;}
      if(OebQHmHoDG == true){OebQHmHoDG = false;}
      if(jMkfjsWZNh == true){jMkfjsWZNh = false;}
      if(TUEcokSzDS == true){TUEcokSzDS = false;}
      if(UziDfkgnRS == true){UziDfkgnRS = false;}
      if(KTnCmBNMbw == true){KTnCmBNMbw = false;}
      if(CbXOmsaybW == true){CbXOmsaybW = false;}
      if(TIljkkchzz == true){TIljkkchzz = false;}
      if(MhIuXAfBpm == true){MhIuXAfBpm = false;}
      if(WFWGPsaUQm == true){WFWGPsaUQm = false;}
      if(JhMuyCgDQC == true){JhMuyCgDQC = false;}
      if(YfMcjUScql == true){YfMcjUScql = false;}
      if(MYIozOkfUr == true){MYIozOkfUr = false;}
      if(JHRModUNwU == true){JHRModUNwU = false;}
      if(SbjmOqrtVu == true){SbjmOqrtVu = false;}
      if(ZyddZkZXui == true){ZyddZkZXui = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VHREHDKYCG
{ 
  void dWHhjYOGqd()
  { 
      bool lmRLkTSxmV = false;
      bool dgCTkjNQIz = false;
      bool TKUrXTGOjP = false;
      bool gaGEjeDbTI = false;
      bool JiZhyTpcyb = false;
      bool zmCegaSZmR = false;
      bool MDNWoCaxjG = false;
      bool glwzokjlsE = false;
      bool cdQHQsYaXI = false;
      bool McnCuGUDsu = false;
      bool rsrquMpCpu = false;
      bool qgfhnilFFu = false;
      bool czYatDxtEi = false;
      bool PgrxMjkZMD = false;
      bool DWkbRexGNM = false;
      bool TOyxSrTRwq = false;
      bool PuyGfKnhVE = false;
      bool xMsBrFOwJO = false;
      bool GqgqPBiteT = false;
      bool PTcUnHYdwH = false;
      string IVWIljbmuF;
      string wzZZxnGEyc;
      string LsfHFOJbez;
      string VkiBqCMABk;
      string OLHscOiuRJ;
      string SelAWOoPCX;
      string ZAHwjKQXht;
      string bFDNOzlmdp;
      string eNJVfqzhec;
      string NxydKNEKJS;
      string yLNWhNbYlX;
      string bCkLrMplPr;
      string LhsFgdHYCA;
      string ubdLrTAmmS;
      string PZwHAKMlkx;
      string XRuQGPSHpD;
      string FCVkbAXYTY;
      string MDyxcklkFV;
      string RUnFccfNEb;
      string AswjMMEKeA;
      if(IVWIljbmuF == yLNWhNbYlX){lmRLkTSxmV = true;}
      else if(yLNWhNbYlX == IVWIljbmuF){rsrquMpCpu = true;}
      if(wzZZxnGEyc == bCkLrMplPr){dgCTkjNQIz = true;}
      else if(bCkLrMplPr == wzZZxnGEyc){qgfhnilFFu = true;}
      if(LsfHFOJbez == LhsFgdHYCA){TKUrXTGOjP = true;}
      else if(LhsFgdHYCA == LsfHFOJbez){czYatDxtEi = true;}
      if(VkiBqCMABk == ubdLrTAmmS){gaGEjeDbTI = true;}
      else if(ubdLrTAmmS == VkiBqCMABk){PgrxMjkZMD = true;}
      if(OLHscOiuRJ == PZwHAKMlkx){JiZhyTpcyb = true;}
      else if(PZwHAKMlkx == OLHscOiuRJ){DWkbRexGNM = true;}
      if(SelAWOoPCX == XRuQGPSHpD){zmCegaSZmR = true;}
      else if(XRuQGPSHpD == SelAWOoPCX){TOyxSrTRwq = true;}
      if(ZAHwjKQXht == FCVkbAXYTY){MDNWoCaxjG = true;}
      else if(FCVkbAXYTY == ZAHwjKQXht){PuyGfKnhVE = true;}
      if(bFDNOzlmdp == MDyxcklkFV){glwzokjlsE = true;}
      if(eNJVfqzhec == RUnFccfNEb){cdQHQsYaXI = true;}
      if(NxydKNEKJS == AswjMMEKeA){McnCuGUDsu = true;}
      while(MDyxcklkFV == bFDNOzlmdp){xMsBrFOwJO = true;}
      while(RUnFccfNEb == RUnFccfNEb){GqgqPBiteT = true;}
      while(AswjMMEKeA == AswjMMEKeA){PTcUnHYdwH = true;}
      if(lmRLkTSxmV == true){lmRLkTSxmV = false;}
      if(dgCTkjNQIz == true){dgCTkjNQIz = false;}
      if(TKUrXTGOjP == true){TKUrXTGOjP = false;}
      if(gaGEjeDbTI == true){gaGEjeDbTI = false;}
      if(JiZhyTpcyb == true){JiZhyTpcyb = false;}
      if(zmCegaSZmR == true){zmCegaSZmR = false;}
      if(MDNWoCaxjG == true){MDNWoCaxjG = false;}
      if(glwzokjlsE == true){glwzokjlsE = false;}
      if(cdQHQsYaXI == true){cdQHQsYaXI = false;}
      if(McnCuGUDsu == true){McnCuGUDsu = false;}
      if(rsrquMpCpu == true){rsrquMpCpu = false;}
      if(qgfhnilFFu == true){qgfhnilFFu = false;}
      if(czYatDxtEi == true){czYatDxtEi = false;}
      if(PgrxMjkZMD == true){PgrxMjkZMD = false;}
      if(DWkbRexGNM == true){DWkbRexGNM = false;}
      if(TOyxSrTRwq == true){TOyxSrTRwq = false;}
      if(PuyGfKnhVE == true){PuyGfKnhVE = false;}
      if(xMsBrFOwJO == true){xMsBrFOwJO = false;}
      if(GqgqPBiteT == true){GqgqPBiteT = false;}
      if(PTcUnHYdwH == true){PTcUnHYdwH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NCLXSLSZUQ
{ 
  void NexkmnPrzr()
  { 
      bool usiYfMCzEk = false;
      bool qOZoxZOtFR = false;
      bool lLsLRgdWXG = false;
      bool CrcXPMoNUj = false;
      bool ATXMzorcWD = false;
      bool AneRYjzXHz = false;
      bool hfaxhykBZj = false;
      bool jAYUQSmfLc = false;
      bool nAzUGNMLze = false;
      bool MRGUCUrZdB = false;
      bool DpDlSzOJBF = false;
      bool wTLdLgfFqT = false;
      bool kZCtWSsESu = false;
      bool ZNBKZxxNCH = false;
      bool zCKsisGcJU = false;
      bool omiKPcnIOz = false;
      bool FKqFeiSAEN = false;
      bool iRCiUhPIKd = false;
      bool LQtyFmdquw = false;
      bool IoyDBnLENA = false;
      string UdSAdkEeYH;
      string gqrCHrTBwc;
      string GwTEqEccwH;
      string SOfDOlIIVL;
      string zuwCBuguQd;
      string UQLkeYJNIY;
      string HbiQoRKonV;
      string jKfjcKzRDu;
      string XhMRmCDuXo;
      string btzwEXLeNi;
      string jrHtsKkaHT;
      string YrKNBYOQdm;
      string NPQytoKifF;
      string yTDTKeaSYc;
      string QcmEPijlue;
      string IuBaVTaIzD;
      string zCtCUFnqCV;
      string ywrBVkcMiK;
      string FYmAwDScyq;
      string DRSUcOhkiK;
      if(UdSAdkEeYH == jrHtsKkaHT){usiYfMCzEk = true;}
      else if(jrHtsKkaHT == UdSAdkEeYH){DpDlSzOJBF = true;}
      if(gqrCHrTBwc == YrKNBYOQdm){qOZoxZOtFR = true;}
      else if(YrKNBYOQdm == gqrCHrTBwc){wTLdLgfFqT = true;}
      if(GwTEqEccwH == NPQytoKifF){lLsLRgdWXG = true;}
      else if(NPQytoKifF == GwTEqEccwH){kZCtWSsESu = true;}
      if(SOfDOlIIVL == yTDTKeaSYc){CrcXPMoNUj = true;}
      else if(yTDTKeaSYc == SOfDOlIIVL){ZNBKZxxNCH = true;}
      if(zuwCBuguQd == QcmEPijlue){ATXMzorcWD = true;}
      else if(QcmEPijlue == zuwCBuguQd){zCKsisGcJU = true;}
      if(UQLkeYJNIY == IuBaVTaIzD){AneRYjzXHz = true;}
      else if(IuBaVTaIzD == UQLkeYJNIY){omiKPcnIOz = true;}
      if(HbiQoRKonV == zCtCUFnqCV){hfaxhykBZj = true;}
      else if(zCtCUFnqCV == HbiQoRKonV){FKqFeiSAEN = true;}
      if(jKfjcKzRDu == ywrBVkcMiK){jAYUQSmfLc = true;}
      if(XhMRmCDuXo == FYmAwDScyq){nAzUGNMLze = true;}
      if(btzwEXLeNi == DRSUcOhkiK){MRGUCUrZdB = true;}
      while(ywrBVkcMiK == jKfjcKzRDu){iRCiUhPIKd = true;}
      while(FYmAwDScyq == FYmAwDScyq){LQtyFmdquw = true;}
      while(DRSUcOhkiK == DRSUcOhkiK){IoyDBnLENA = true;}
      if(usiYfMCzEk == true){usiYfMCzEk = false;}
      if(qOZoxZOtFR == true){qOZoxZOtFR = false;}
      if(lLsLRgdWXG == true){lLsLRgdWXG = false;}
      if(CrcXPMoNUj == true){CrcXPMoNUj = false;}
      if(ATXMzorcWD == true){ATXMzorcWD = false;}
      if(AneRYjzXHz == true){AneRYjzXHz = false;}
      if(hfaxhykBZj == true){hfaxhykBZj = false;}
      if(jAYUQSmfLc == true){jAYUQSmfLc = false;}
      if(nAzUGNMLze == true){nAzUGNMLze = false;}
      if(MRGUCUrZdB == true){MRGUCUrZdB = false;}
      if(DpDlSzOJBF == true){DpDlSzOJBF = false;}
      if(wTLdLgfFqT == true){wTLdLgfFqT = false;}
      if(kZCtWSsESu == true){kZCtWSsESu = false;}
      if(ZNBKZxxNCH == true){ZNBKZxxNCH = false;}
      if(zCKsisGcJU == true){zCKsisGcJU = false;}
      if(omiKPcnIOz == true){omiKPcnIOz = false;}
      if(FKqFeiSAEN == true){FKqFeiSAEN = false;}
      if(iRCiUhPIKd == true){iRCiUhPIKd = false;}
      if(LQtyFmdquw == true){LQtyFmdquw = false;}
      if(IoyDBnLENA == true){IoyDBnLENA = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GLUWTVLEUP
{ 
  void sOHWlynqDE()
  { 
      bool XKesbQBasR = false;
      bool xcrwrZcYLP = false;
      bool hKylZUbQES = false;
      bool DPnxoetdLV = false;
      bool IrOxJMnOSl = false;
      bool UkqsSMLsSG = false;
      bool nkVaNgnuHN = false;
      bool ezMpHpuAcN = false;
      bool gNyRySXteb = false;
      bool SBKFgXVnjb = false;
      bool XsGEHUXBlb = false;
      bool QLPFZamEcu = false;
      bool IsQetcnHIj = false;
      bool UHQBomGsKc = false;
      bool uVICLHkFfG = false;
      bool jRKnSdcaKd = false;
      bool gPsCHItOPl = false;
      bool TtsAAmkNTm = false;
      bool rhbCzCxbhK = false;
      bool KeifuecFtC = false;
      string dgXcTJTbfT;
      string greUmoALMH;
      string klEGEPUcXt;
      string byhxFFwNrr;
      string EAGOxupFtl;
      string jyyryqbQYE;
      string nqElqiKgOg;
      string rmTUDjepII;
      string YOhkJczoyO;
      string cfnnDMwUXO;
      string kIqNsxecnO;
      string LlHPeZIScN;
      string iUhADMyeQp;
      string nKStWPXUqL;
      string xVBjCqXXId;
      string EcrJeQapJg;
      string zAMxaBTNsP;
      string CmaQPNTyyM;
      string oZWmlKSIPG;
      string ogLKLxJDJA;
      if(dgXcTJTbfT == kIqNsxecnO){XKesbQBasR = true;}
      else if(kIqNsxecnO == dgXcTJTbfT){XsGEHUXBlb = true;}
      if(greUmoALMH == LlHPeZIScN){xcrwrZcYLP = true;}
      else if(LlHPeZIScN == greUmoALMH){QLPFZamEcu = true;}
      if(klEGEPUcXt == iUhADMyeQp){hKylZUbQES = true;}
      else if(iUhADMyeQp == klEGEPUcXt){IsQetcnHIj = true;}
      if(byhxFFwNrr == nKStWPXUqL){DPnxoetdLV = true;}
      else if(nKStWPXUqL == byhxFFwNrr){UHQBomGsKc = true;}
      if(EAGOxupFtl == xVBjCqXXId){IrOxJMnOSl = true;}
      else if(xVBjCqXXId == EAGOxupFtl){uVICLHkFfG = true;}
      if(jyyryqbQYE == EcrJeQapJg){UkqsSMLsSG = true;}
      else if(EcrJeQapJg == jyyryqbQYE){jRKnSdcaKd = true;}
      if(nqElqiKgOg == zAMxaBTNsP){nkVaNgnuHN = true;}
      else if(zAMxaBTNsP == nqElqiKgOg){gPsCHItOPl = true;}
      if(rmTUDjepII == CmaQPNTyyM){ezMpHpuAcN = true;}
      if(YOhkJczoyO == oZWmlKSIPG){gNyRySXteb = true;}
      if(cfnnDMwUXO == ogLKLxJDJA){SBKFgXVnjb = true;}
      while(CmaQPNTyyM == rmTUDjepII){TtsAAmkNTm = true;}
      while(oZWmlKSIPG == oZWmlKSIPG){rhbCzCxbhK = true;}
      while(ogLKLxJDJA == ogLKLxJDJA){KeifuecFtC = true;}
      if(XKesbQBasR == true){XKesbQBasR = false;}
      if(xcrwrZcYLP == true){xcrwrZcYLP = false;}
      if(hKylZUbQES == true){hKylZUbQES = false;}
      if(DPnxoetdLV == true){DPnxoetdLV = false;}
      if(IrOxJMnOSl == true){IrOxJMnOSl = false;}
      if(UkqsSMLsSG == true){UkqsSMLsSG = false;}
      if(nkVaNgnuHN == true){nkVaNgnuHN = false;}
      if(ezMpHpuAcN == true){ezMpHpuAcN = false;}
      if(gNyRySXteb == true){gNyRySXteb = false;}
      if(SBKFgXVnjb == true){SBKFgXVnjb = false;}
      if(XsGEHUXBlb == true){XsGEHUXBlb = false;}
      if(QLPFZamEcu == true){QLPFZamEcu = false;}
      if(IsQetcnHIj == true){IsQetcnHIj = false;}
      if(UHQBomGsKc == true){UHQBomGsKc = false;}
      if(uVICLHkFfG == true){uVICLHkFfG = false;}
      if(jRKnSdcaKd == true){jRKnSdcaKd = false;}
      if(gPsCHItOPl == true){gPsCHItOPl = false;}
      if(TtsAAmkNTm == true){TtsAAmkNTm = false;}
      if(rhbCzCxbhK == true){rhbCzCxbhK = false;}
      if(KeifuecFtC == true){KeifuecFtC = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VASRCBQOTE
{ 
  void RqXYDjNdBp()
  { 
      bool utBJjuxciQ = false;
      bool hRMwxkAflh = false;
      bool FyXIAFFdpn = false;
      bool yVwUXbNbwB = false;
      bool ZgEoFJwQFw = false;
      bool UgCGPwWrId = false;
      bool oTluSMVrXU = false;
      bool WYikIXsiDq = false;
      bool JoXtTRToEa = false;
      bool FVSSLFuJVs = false;
      bool nEsjZuxRxq = false;
      bool DnoRTAurbB = false;
      bool sgmwOBGQYD = false;
      bool biMKDFumUX = false;
      bool TCqNXRfAyK = false;
      bool JDgUAHQwVX = false;
      bool ogrtPTaeOO = false;
      bool FHWEgpoZYw = false;
      bool UFSNsYCrKu = false;
      bool nzRwhmKwxD = false;
      string YoKcFLBPMd;
      string KpYnLSLlpZ;
      string CefEnhyLbH;
      string wLmpDJoSSl;
      string BUMpKeSLRq;
      string IduTnsIFnX;
      string MpmFhzChjB;
      string dsTEXhSosJ;
      string PlsjyRPTBK;
      string AiduYLgHIL;
      string bjbQNPKquG;
      string ZTLeIZwCGP;
      string iifZAtOYeM;
      string BKghgeKcWh;
      string HYXujgXzmy;
      string OdArbsDCPT;
      string AlLiFPldxc;
      string OTfXIKKkoS;
      string WPkfxWJnFn;
      string bINCgUMnoU;
      if(YoKcFLBPMd == bjbQNPKquG){utBJjuxciQ = true;}
      else if(bjbQNPKquG == YoKcFLBPMd){nEsjZuxRxq = true;}
      if(KpYnLSLlpZ == ZTLeIZwCGP){hRMwxkAflh = true;}
      else if(ZTLeIZwCGP == KpYnLSLlpZ){DnoRTAurbB = true;}
      if(CefEnhyLbH == iifZAtOYeM){FyXIAFFdpn = true;}
      else if(iifZAtOYeM == CefEnhyLbH){sgmwOBGQYD = true;}
      if(wLmpDJoSSl == BKghgeKcWh){yVwUXbNbwB = true;}
      else if(BKghgeKcWh == wLmpDJoSSl){biMKDFumUX = true;}
      if(BUMpKeSLRq == HYXujgXzmy){ZgEoFJwQFw = true;}
      else if(HYXujgXzmy == BUMpKeSLRq){TCqNXRfAyK = true;}
      if(IduTnsIFnX == OdArbsDCPT){UgCGPwWrId = true;}
      else if(OdArbsDCPT == IduTnsIFnX){JDgUAHQwVX = true;}
      if(MpmFhzChjB == AlLiFPldxc){oTluSMVrXU = true;}
      else if(AlLiFPldxc == MpmFhzChjB){ogrtPTaeOO = true;}
      if(dsTEXhSosJ == OTfXIKKkoS){WYikIXsiDq = true;}
      if(PlsjyRPTBK == WPkfxWJnFn){JoXtTRToEa = true;}
      if(AiduYLgHIL == bINCgUMnoU){FVSSLFuJVs = true;}
      while(OTfXIKKkoS == dsTEXhSosJ){FHWEgpoZYw = true;}
      while(WPkfxWJnFn == WPkfxWJnFn){UFSNsYCrKu = true;}
      while(bINCgUMnoU == bINCgUMnoU){nzRwhmKwxD = true;}
      if(utBJjuxciQ == true){utBJjuxciQ = false;}
      if(hRMwxkAflh == true){hRMwxkAflh = false;}
      if(FyXIAFFdpn == true){FyXIAFFdpn = false;}
      if(yVwUXbNbwB == true){yVwUXbNbwB = false;}
      if(ZgEoFJwQFw == true){ZgEoFJwQFw = false;}
      if(UgCGPwWrId == true){UgCGPwWrId = false;}
      if(oTluSMVrXU == true){oTluSMVrXU = false;}
      if(WYikIXsiDq == true){WYikIXsiDq = false;}
      if(JoXtTRToEa == true){JoXtTRToEa = false;}
      if(FVSSLFuJVs == true){FVSSLFuJVs = false;}
      if(nEsjZuxRxq == true){nEsjZuxRxq = false;}
      if(DnoRTAurbB == true){DnoRTAurbB = false;}
      if(sgmwOBGQYD == true){sgmwOBGQYD = false;}
      if(biMKDFumUX == true){biMKDFumUX = false;}
      if(TCqNXRfAyK == true){TCqNXRfAyK = false;}
      if(JDgUAHQwVX == true){JDgUAHQwVX = false;}
      if(ogrtPTaeOO == true){ogrtPTaeOO = false;}
      if(FHWEgpoZYw == true){FHWEgpoZYw = false;}
      if(UFSNsYCrKu == true){UFSNsYCrKu = false;}
      if(nzRwhmKwxD == true){nzRwhmKwxD = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZZOBQKREIJ
{ 
  void HLzkhqewOR()
  { 
      bool boWHIdOtPy = false;
      bool kHfLdaSOuk = false;
      bool HFeLxYWIZX = false;
      bool APhwtUtWfa = false;
      bool ftQHEIhOUa = false;
      bool LhwUFWKKJj = false;
      bool GFUqyVYjQJ = false;
      bool fuhUFNnWGo = false;
      bool qHnyLRSMqX = false;
      bool KjNwTZYYJi = false;
      bool jyxLtPzbPq = false;
      bool hmDUsyRWff = false;
      bool CMrsywhlAe = false;
      bool nONXEsJcDw = false;
      bool SpwzoXfrDU = false;
      bool ICsdfWFEMy = false;
      bool hNbkDSYfPz = false;
      bool onSjMMPsEa = false;
      bool QphfVkVjqg = false;
      bool mFQEOEOLuV = false;
      string ESTultBpFV;
      string AfkEFuQIKV;
      string LtpjkYuWhH;
      string TILWYYxSKl;
      string KZpxmLmlxW;
      string fuPpjJbjKM;
      string zeHyCrTOff;
      string xyxYLkDswZ;
      string kIYLbYrVlO;
      string xNIxlfSruu;
      string qYFQYgxqdz;
      string GimjbpFuVc;
      string zybQijoAle;
      string SKyBiMDHmt;
      string aBqmINZXVw;
      string aWVRurwohn;
      string rxEzfukkBO;
      string mxIbCxHoIV;
      string JQdoEunfFp;
      string QNZzCdxqGM;
      if(ESTultBpFV == qYFQYgxqdz){boWHIdOtPy = true;}
      else if(qYFQYgxqdz == ESTultBpFV){jyxLtPzbPq = true;}
      if(AfkEFuQIKV == GimjbpFuVc){kHfLdaSOuk = true;}
      else if(GimjbpFuVc == AfkEFuQIKV){hmDUsyRWff = true;}
      if(LtpjkYuWhH == zybQijoAle){HFeLxYWIZX = true;}
      else if(zybQijoAle == LtpjkYuWhH){CMrsywhlAe = true;}
      if(TILWYYxSKl == SKyBiMDHmt){APhwtUtWfa = true;}
      else if(SKyBiMDHmt == TILWYYxSKl){nONXEsJcDw = true;}
      if(KZpxmLmlxW == aBqmINZXVw){ftQHEIhOUa = true;}
      else if(aBqmINZXVw == KZpxmLmlxW){SpwzoXfrDU = true;}
      if(fuPpjJbjKM == aWVRurwohn){LhwUFWKKJj = true;}
      else if(aWVRurwohn == fuPpjJbjKM){ICsdfWFEMy = true;}
      if(zeHyCrTOff == rxEzfukkBO){GFUqyVYjQJ = true;}
      else if(rxEzfukkBO == zeHyCrTOff){hNbkDSYfPz = true;}
      if(xyxYLkDswZ == mxIbCxHoIV){fuhUFNnWGo = true;}
      if(kIYLbYrVlO == JQdoEunfFp){qHnyLRSMqX = true;}
      if(xNIxlfSruu == QNZzCdxqGM){KjNwTZYYJi = true;}
      while(mxIbCxHoIV == xyxYLkDswZ){onSjMMPsEa = true;}
      while(JQdoEunfFp == JQdoEunfFp){QphfVkVjqg = true;}
      while(QNZzCdxqGM == QNZzCdxqGM){mFQEOEOLuV = true;}
      if(boWHIdOtPy == true){boWHIdOtPy = false;}
      if(kHfLdaSOuk == true){kHfLdaSOuk = false;}
      if(HFeLxYWIZX == true){HFeLxYWIZX = false;}
      if(APhwtUtWfa == true){APhwtUtWfa = false;}
      if(ftQHEIhOUa == true){ftQHEIhOUa = false;}
      if(LhwUFWKKJj == true){LhwUFWKKJj = false;}
      if(GFUqyVYjQJ == true){GFUqyVYjQJ = false;}
      if(fuhUFNnWGo == true){fuhUFNnWGo = false;}
      if(qHnyLRSMqX == true){qHnyLRSMqX = false;}
      if(KjNwTZYYJi == true){KjNwTZYYJi = false;}
      if(jyxLtPzbPq == true){jyxLtPzbPq = false;}
      if(hmDUsyRWff == true){hmDUsyRWff = false;}
      if(CMrsywhlAe == true){CMrsywhlAe = false;}
      if(nONXEsJcDw == true){nONXEsJcDw = false;}
      if(SpwzoXfrDU == true){SpwzoXfrDU = false;}
      if(ICsdfWFEMy == true){ICsdfWFEMy = false;}
      if(hNbkDSYfPz == true){hNbkDSYfPz = false;}
      if(onSjMMPsEa == true){onSjMMPsEa = false;}
      if(QphfVkVjqg == true){QphfVkVjqg = false;}
      if(mFQEOEOLuV == true){mFQEOEOLuV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ONTZLYZDKU
{ 
  void uObRaShJJr()
  { 
      bool yDDStyFKTo = false;
      bool ZMyEBpriyH = false;
      bool slkMsUHNuA = false;
      bool RJQmRcaqmL = false;
      bool qBkGTbwgwP = false;
      bool fcZboLRqZZ = false;
      bool qqOTXPuKlY = false;
      bool PBulPqBdrQ = false;
      bool XaReUnjoyF = false;
      bool KjRNUAmQiE = false;
      bool fktWAdhmLx = false;
      bool KnyIBPYizj = false;
      bool eGLfNejgRO = false;
      bool Ekunkmgtep = false;
      bool FwcBuWkJMF = false;
      bool aEQzYYZNJU = false;
      bool tBVxsZrKWO = false;
      bool idEJdidKkq = false;
      bool taTMmATwOM = false;
      bool MSkJwDZfIV = false;
      string NxlngagnPZ;
      string OEmpxKGLpT;
      string uTRDGwCyUA;
      string oXwsMKOzTL;
      string aZFbPftIOs;
      string PDmEmseihk;
      string XiSOkqHscl;
      string uGresUOomZ;
      string CtibklkrFS;
      string ffRimqHpqY;
      string XAebxJLHKR;
      string douHidzpdy;
      string GPnKGPnJqB;
      string NYyOBBOGea;
      string ZcgmnpjJzJ;
      string qQMrSIMUVg;
      string UCQIEFqgGg;
      string OsLCYciAVQ;
      string fdJhKwLFws;
      string ntkxGXLYih;
      if(NxlngagnPZ == XAebxJLHKR){yDDStyFKTo = true;}
      else if(XAebxJLHKR == NxlngagnPZ){fktWAdhmLx = true;}
      if(OEmpxKGLpT == douHidzpdy){ZMyEBpriyH = true;}
      else if(douHidzpdy == OEmpxKGLpT){KnyIBPYizj = true;}
      if(uTRDGwCyUA == GPnKGPnJqB){slkMsUHNuA = true;}
      else if(GPnKGPnJqB == uTRDGwCyUA){eGLfNejgRO = true;}
      if(oXwsMKOzTL == NYyOBBOGea){RJQmRcaqmL = true;}
      else if(NYyOBBOGea == oXwsMKOzTL){Ekunkmgtep = true;}
      if(aZFbPftIOs == ZcgmnpjJzJ){qBkGTbwgwP = true;}
      else if(ZcgmnpjJzJ == aZFbPftIOs){FwcBuWkJMF = true;}
      if(PDmEmseihk == qQMrSIMUVg){fcZboLRqZZ = true;}
      else if(qQMrSIMUVg == PDmEmseihk){aEQzYYZNJU = true;}
      if(XiSOkqHscl == UCQIEFqgGg){qqOTXPuKlY = true;}
      else if(UCQIEFqgGg == XiSOkqHscl){tBVxsZrKWO = true;}
      if(uGresUOomZ == OsLCYciAVQ){PBulPqBdrQ = true;}
      if(CtibklkrFS == fdJhKwLFws){XaReUnjoyF = true;}
      if(ffRimqHpqY == ntkxGXLYih){KjRNUAmQiE = true;}
      while(OsLCYciAVQ == uGresUOomZ){idEJdidKkq = true;}
      while(fdJhKwLFws == fdJhKwLFws){taTMmATwOM = true;}
      while(ntkxGXLYih == ntkxGXLYih){MSkJwDZfIV = true;}
      if(yDDStyFKTo == true){yDDStyFKTo = false;}
      if(ZMyEBpriyH == true){ZMyEBpriyH = false;}
      if(slkMsUHNuA == true){slkMsUHNuA = false;}
      if(RJQmRcaqmL == true){RJQmRcaqmL = false;}
      if(qBkGTbwgwP == true){qBkGTbwgwP = false;}
      if(fcZboLRqZZ == true){fcZboLRqZZ = false;}
      if(qqOTXPuKlY == true){qqOTXPuKlY = false;}
      if(PBulPqBdrQ == true){PBulPqBdrQ = false;}
      if(XaReUnjoyF == true){XaReUnjoyF = false;}
      if(KjRNUAmQiE == true){KjRNUAmQiE = false;}
      if(fktWAdhmLx == true){fktWAdhmLx = false;}
      if(KnyIBPYizj == true){KnyIBPYizj = false;}
      if(eGLfNejgRO == true){eGLfNejgRO = false;}
      if(Ekunkmgtep == true){Ekunkmgtep = false;}
      if(FwcBuWkJMF == true){FwcBuWkJMF = false;}
      if(aEQzYYZNJU == true){aEQzYYZNJU = false;}
      if(tBVxsZrKWO == true){tBVxsZrKWO = false;}
      if(idEJdidKkq == true){idEJdidKkq = false;}
      if(taTMmATwOM == true){taTMmATwOM = false;}
      if(MSkJwDZfIV == true){MSkJwDZfIV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FNNAHLHXBP
{ 
  void SVneIyrIPY()
  { 
      bool FrBzEEspTB = false;
      bool ZKkuctIAGf = false;
      bool rHxnCEEGGZ = false;
      bool rqIoHwXcsi = false;
      bool wFLNoEmfdH = false;
      bool xRBxJyuNwA = false;
      bool zljsIiBunf = false;
      bool WfGXimwiyx = false;
      bool RYczcaoLfI = false;
      bool BKfsiJVREG = false;
      bool CqqxkRKdNV = false;
      bool BLOziaJjQh = false;
      bool spPedLANnY = false;
      bool RdertZgeNK = false;
      bool PLxJGpufJI = false;
      bool AkBVMLbMSY = false;
      bool RwooEXfVEI = false;
      bool tdWFRhsFXn = false;
      bool PTDfYfcJfh = false;
      bool yUMosSWIEi = false;
      string RqeazTyIzf;
      string hmYwzlqHpx;
      string IFUYPbdgkA;
      string mNPYmgzLTz;
      string UReQIyhSgz;
      string eRTtILVFLX;
      string oRXzrUWAIR;
      string mfnpbpaDQQ;
      string sKAkUUsUQa;
      string LsAuKDseXn;
      string agKOLzdcSR;
      string kSotddRmeX;
      string aBjhGseseY;
      string wweboMAkYV;
      string qPCypsEGQg;
      string kGAAMZwcIR;
      string TkytUnmMER;
      string cgNJgKzlqN;
      string oYbxmowePP;
      string PXbGCGRIcB;
      if(RqeazTyIzf == agKOLzdcSR){FrBzEEspTB = true;}
      else if(agKOLzdcSR == RqeazTyIzf){CqqxkRKdNV = true;}
      if(hmYwzlqHpx == kSotddRmeX){ZKkuctIAGf = true;}
      else if(kSotddRmeX == hmYwzlqHpx){BLOziaJjQh = true;}
      if(IFUYPbdgkA == aBjhGseseY){rHxnCEEGGZ = true;}
      else if(aBjhGseseY == IFUYPbdgkA){spPedLANnY = true;}
      if(mNPYmgzLTz == wweboMAkYV){rqIoHwXcsi = true;}
      else if(wweboMAkYV == mNPYmgzLTz){RdertZgeNK = true;}
      if(UReQIyhSgz == qPCypsEGQg){wFLNoEmfdH = true;}
      else if(qPCypsEGQg == UReQIyhSgz){PLxJGpufJI = true;}
      if(eRTtILVFLX == kGAAMZwcIR){xRBxJyuNwA = true;}
      else if(kGAAMZwcIR == eRTtILVFLX){AkBVMLbMSY = true;}
      if(oRXzrUWAIR == TkytUnmMER){zljsIiBunf = true;}
      else if(TkytUnmMER == oRXzrUWAIR){RwooEXfVEI = true;}
      if(mfnpbpaDQQ == cgNJgKzlqN){WfGXimwiyx = true;}
      if(sKAkUUsUQa == oYbxmowePP){RYczcaoLfI = true;}
      if(LsAuKDseXn == PXbGCGRIcB){BKfsiJVREG = true;}
      while(cgNJgKzlqN == mfnpbpaDQQ){tdWFRhsFXn = true;}
      while(oYbxmowePP == oYbxmowePP){PTDfYfcJfh = true;}
      while(PXbGCGRIcB == PXbGCGRIcB){yUMosSWIEi = true;}
      if(FrBzEEspTB == true){FrBzEEspTB = false;}
      if(ZKkuctIAGf == true){ZKkuctIAGf = false;}
      if(rHxnCEEGGZ == true){rHxnCEEGGZ = false;}
      if(rqIoHwXcsi == true){rqIoHwXcsi = false;}
      if(wFLNoEmfdH == true){wFLNoEmfdH = false;}
      if(xRBxJyuNwA == true){xRBxJyuNwA = false;}
      if(zljsIiBunf == true){zljsIiBunf = false;}
      if(WfGXimwiyx == true){WfGXimwiyx = false;}
      if(RYczcaoLfI == true){RYczcaoLfI = false;}
      if(BKfsiJVREG == true){BKfsiJVREG = false;}
      if(CqqxkRKdNV == true){CqqxkRKdNV = false;}
      if(BLOziaJjQh == true){BLOziaJjQh = false;}
      if(spPedLANnY == true){spPedLANnY = false;}
      if(RdertZgeNK == true){RdertZgeNK = false;}
      if(PLxJGpufJI == true){PLxJGpufJI = false;}
      if(AkBVMLbMSY == true){AkBVMLbMSY = false;}
      if(RwooEXfVEI == true){RwooEXfVEI = false;}
      if(tdWFRhsFXn == true){tdWFRhsFXn = false;}
      if(PTDfYfcJfh == true){PTDfYfcJfh = false;}
      if(yUMosSWIEi == true){yUMosSWIEi = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UJPZMTHPZF
{ 
  void BXzXAZfuXO()
  { 
      bool DmnSxiZcXf = false;
      bool mrbtnKjTIe = false;
      bool NXTcmSxlhB = false;
      bool mbeRkPQZfs = false;
      bool lyJFbHBcEX = false;
      bool UGhMdeSKee = false;
      bool lntVsKNJJX = false;
      bool sKnOrONNAe = false;
      bool QUhJfftRmF = false;
      bool xFtBHyEnMR = false;
      bool trTfeKSTTO = false;
      bool QEKANRGjoN = false;
      bool QuVDamPmGt = false;
      bool KZBijYABeV = false;
      bool MaxXnrutmp = false;
      bool rUyzBzVPFr = false;
      bool ufMZEqDbPo = false;
      bool QEFTApyxPh = false;
      bool rVzQBpbBgk = false;
      bool CkumagNXTx = false;
      string tOWxpFFbzD;
      string XTdQEHKFiF;
      string kELYNDCrpi;
      string zqcgheddwG;
      string rDYaMrSLFR;
      string ilxgooCyim;
      string CNlPEfqxiZ;
      string cgIbNhejLV;
      string jqlXAMtwqr;
      string bftJEeRJWf;
      string AUbfnBcUTH;
      string KSlNCKaNPq;
      string luGqxxlzXh;
      string PVMfUOOFiy;
      string iYbigJsgGs;
      string EDtesFOkiq;
      string PdxlqoFGrm;
      string wUdQHNSWJn;
      string DFsdcMaglO;
      string AcWriFaprr;
      if(tOWxpFFbzD == AUbfnBcUTH){DmnSxiZcXf = true;}
      else if(AUbfnBcUTH == tOWxpFFbzD){trTfeKSTTO = true;}
      if(XTdQEHKFiF == KSlNCKaNPq){mrbtnKjTIe = true;}
      else if(KSlNCKaNPq == XTdQEHKFiF){QEKANRGjoN = true;}
      if(kELYNDCrpi == luGqxxlzXh){NXTcmSxlhB = true;}
      else if(luGqxxlzXh == kELYNDCrpi){QuVDamPmGt = true;}
      if(zqcgheddwG == PVMfUOOFiy){mbeRkPQZfs = true;}
      else if(PVMfUOOFiy == zqcgheddwG){KZBijYABeV = true;}
      if(rDYaMrSLFR == iYbigJsgGs){lyJFbHBcEX = true;}
      else if(iYbigJsgGs == rDYaMrSLFR){MaxXnrutmp = true;}
      if(ilxgooCyim == EDtesFOkiq){UGhMdeSKee = true;}
      else if(EDtesFOkiq == ilxgooCyim){rUyzBzVPFr = true;}
      if(CNlPEfqxiZ == PdxlqoFGrm){lntVsKNJJX = true;}
      else if(PdxlqoFGrm == CNlPEfqxiZ){ufMZEqDbPo = true;}
      if(cgIbNhejLV == wUdQHNSWJn){sKnOrONNAe = true;}
      if(jqlXAMtwqr == DFsdcMaglO){QUhJfftRmF = true;}
      if(bftJEeRJWf == AcWriFaprr){xFtBHyEnMR = true;}
      while(wUdQHNSWJn == cgIbNhejLV){QEFTApyxPh = true;}
      while(DFsdcMaglO == DFsdcMaglO){rVzQBpbBgk = true;}
      while(AcWriFaprr == AcWriFaprr){CkumagNXTx = true;}
      if(DmnSxiZcXf == true){DmnSxiZcXf = false;}
      if(mrbtnKjTIe == true){mrbtnKjTIe = false;}
      if(NXTcmSxlhB == true){NXTcmSxlhB = false;}
      if(mbeRkPQZfs == true){mbeRkPQZfs = false;}
      if(lyJFbHBcEX == true){lyJFbHBcEX = false;}
      if(UGhMdeSKee == true){UGhMdeSKee = false;}
      if(lntVsKNJJX == true){lntVsKNJJX = false;}
      if(sKnOrONNAe == true){sKnOrONNAe = false;}
      if(QUhJfftRmF == true){QUhJfftRmF = false;}
      if(xFtBHyEnMR == true){xFtBHyEnMR = false;}
      if(trTfeKSTTO == true){trTfeKSTTO = false;}
      if(QEKANRGjoN == true){QEKANRGjoN = false;}
      if(QuVDamPmGt == true){QuVDamPmGt = false;}
      if(KZBijYABeV == true){KZBijYABeV = false;}
      if(MaxXnrutmp == true){MaxXnrutmp = false;}
      if(rUyzBzVPFr == true){rUyzBzVPFr = false;}
      if(ufMZEqDbPo == true){ufMZEqDbPo = false;}
      if(QEFTApyxPh == true){QEFTApyxPh = false;}
      if(rVzQBpbBgk == true){rVzQBpbBgk = false;}
      if(CkumagNXTx == true){CkumagNXTx = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SSPERYDWWU
{ 
  void giLIgcZkzN()
  { 
      bool aWixadpVNQ = false;
      bool gFniZZLtre = false;
      bool rSbWYxPcTR = false;
      bool HaWJAJolTz = false;
      bool ZnnUxkzOxq = false;
      bool uLPGpItMXc = false;
      bool NMCwJFhdiP = false;
      bool jZarFDdHZn = false;
      bool yBfbVuKQTq = false;
      bool OcNplNlrKq = false;
      bool CUfhHVjXBl = false;
      bool dmmjeMgZZO = false;
      bool tRJqAHLkFq = false;
      bool RlwhkcUxUA = false;
      bool KmOhNbodLZ = false;
      bool gLSwwkKogz = false;
      bool KzRJTrqEzB = false;
      bool qiEZDHDbcY = false;
      bool RgYfKFTzlA = false;
      bool RfQVoKVEOV = false;
      string EuiobRVpon;
      string PbsptjcugQ;
      string eHjxatdFJQ;
      string SNwQEQCqPG;
      string fCgiqGixMG;
      string sXVbewKlNg;
      string mPgXVBfcld;
      string hlfZkbaBGT;
      string LgrCWtqTkH;
      string FQciSPUIMo;
      string aGLiYHruqC;
      string VJesUGXeAa;
      string JLqdXRIxGV;
      string GSnQsFUIdP;
      string CwchlwkjNM;
      string rpHlePOgKG;
      string yuoUwDUNAP;
      string kOAkYNhJrb;
      string gYitcQKBld;
      string pnyWQGGHlh;
      if(EuiobRVpon == aGLiYHruqC){aWixadpVNQ = true;}
      else if(aGLiYHruqC == EuiobRVpon){CUfhHVjXBl = true;}
      if(PbsptjcugQ == VJesUGXeAa){gFniZZLtre = true;}
      else if(VJesUGXeAa == PbsptjcugQ){dmmjeMgZZO = true;}
      if(eHjxatdFJQ == JLqdXRIxGV){rSbWYxPcTR = true;}
      else if(JLqdXRIxGV == eHjxatdFJQ){tRJqAHLkFq = true;}
      if(SNwQEQCqPG == GSnQsFUIdP){HaWJAJolTz = true;}
      else if(GSnQsFUIdP == SNwQEQCqPG){RlwhkcUxUA = true;}
      if(fCgiqGixMG == CwchlwkjNM){ZnnUxkzOxq = true;}
      else if(CwchlwkjNM == fCgiqGixMG){KmOhNbodLZ = true;}
      if(sXVbewKlNg == rpHlePOgKG){uLPGpItMXc = true;}
      else if(rpHlePOgKG == sXVbewKlNg){gLSwwkKogz = true;}
      if(mPgXVBfcld == yuoUwDUNAP){NMCwJFhdiP = true;}
      else if(yuoUwDUNAP == mPgXVBfcld){KzRJTrqEzB = true;}
      if(hlfZkbaBGT == kOAkYNhJrb){jZarFDdHZn = true;}
      if(LgrCWtqTkH == gYitcQKBld){yBfbVuKQTq = true;}
      if(FQciSPUIMo == pnyWQGGHlh){OcNplNlrKq = true;}
      while(kOAkYNhJrb == hlfZkbaBGT){qiEZDHDbcY = true;}
      while(gYitcQKBld == gYitcQKBld){RgYfKFTzlA = true;}
      while(pnyWQGGHlh == pnyWQGGHlh){RfQVoKVEOV = true;}
      if(aWixadpVNQ == true){aWixadpVNQ = false;}
      if(gFniZZLtre == true){gFniZZLtre = false;}
      if(rSbWYxPcTR == true){rSbWYxPcTR = false;}
      if(HaWJAJolTz == true){HaWJAJolTz = false;}
      if(ZnnUxkzOxq == true){ZnnUxkzOxq = false;}
      if(uLPGpItMXc == true){uLPGpItMXc = false;}
      if(NMCwJFhdiP == true){NMCwJFhdiP = false;}
      if(jZarFDdHZn == true){jZarFDdHZn = false;}
      if(yBfbVuKQTq == true){yBfbVuKQTq = false;}
      if(OcNplNlrKq == true){OcNplNlrKq = false;}
      if(CUfhHVjXBl == true){CUfhHVjXBl = false;}
      if(dmmjeMgZZO == true){dmmjeMgZZO = false;}
      if(tRJqAHLkFq == true){tRJqAHLkFq = false;}
      if(RlwhkcUxUA == true){RlwhkcUxUA = false;}
      if(KmOhNbodLZ == true){KmOhNbodLZ = false;}
      if(gLSwwkKogz == true){gLSwwkKogz = false;}
      if(KzRJTrqEzB == true){KzRJTrqEzB = false;}
      if(qiEZDHDbcY == true){qiEZDHDbcY = false;}
      if(RgYfKFTzlA == true){RgYfKFTzlA = false;}
      if(RfQVoKVEOV == true){RfQVoKVEOV = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JOKVQYGIMA
{ 
  void ilDhwNCAjZ()
  { 
      bool ZjeixGfasB = false;
      bool okNlgimNQd = false;
      bool ZdTZtdIBYf = false;
      bool cRXJRKKQwc = false;
      bool lxeSqTWyfS = false;
      bool KeQBUuQEnS = false;
      bool yeBNYWNEUV = false;
      bool XWQnrczAia = false;
      bool GJUFVxAVoV = false;
      bool sGbcdGuQtU = false;
      bool GScKumNcJq = false;
      bool wQVNMnJwqb = false;
      bool VqPzbRtnJk = false;
      bool hhGNUmfKKh = false;
      bool iXMxYcjOPp = false;
      bool xzZRzODdhc = false;
      bool sZTYINJHsY = false;
      bool IzkIryeRZz = false;
      bool gfXTniInOF = false;
      bool jCcgoJDmoL = false;
      string EgfsKkkHjQ;
      string xFYeIyUzEd;
      string JExNXByMMK;
      string rNBIQLXhmS;
      string iubMcckdxY;
      string kizgWziRVJ;
      string mYtZYiEQOx;
      string alMluNbjkt;
      string PTPnYINUkQ;
      string bFMcfKACns;
      string YUPzxyWNkI;
      string XzgbtSJFAP;
      string VifxUUZiGk;
      string MZwrVtDPpZ;
      string lAibscYkiM;
      string AuBXWooSdc;
      string XnHAwyCnGz;
      string PsOadVADEr;
      string uoAsjRXXUf;
      string FkkNByshft;
      if(EgfsKkkHjQ == YUPzxyWNkI){ZjeixGfasB = true;}
      else if(YUPzxyWNkI == EgfsKkkHjQ){GScKumNcJq = true;}
      if(xFYeIyUzEd == XzgbtSJFAP){okNlgimNQd = true;}
      else if(XzgbtSJFAP == xFYeIyUzEd){wQVNMnJwqb = true;}
      if(JExNXByMMK == VifxUUZiGk){ZdTZtdIBYf = true;}
      else if(VifxUUZiGk == JExNXByMMK){VqPzbRtnJk = true;}
      if(rNBIQLXhmS == MZwrVtDPpZ){cRXJRKKQwc = true;}
      else if(MZwrVtDPpZ == rNBIQLXhmS){hhGNUmfKKh = true;}
      if(iubMcckdxY == lAibscYkiM){lxeSqTWyfS = true;}
      else if(lAibscYkiM == iubMcckdxY){iXMxYcjOPp = true;}
      if(kizgWziRVJ == AuBXWooSdc){KeQBUuQEnS = true;}
      else if(AuBXWooSdc == kizgWziRVJ){xzZRzODdhc = true;}
      if(mYtZYiEQOx == XnHAwyCnGz){yeBNYWNEUV = true;}
      else if(XnHAwyCnGz == mYtZYiEQOx){sZTYINJHsY = true;}
      if(alMluNbjkt == PsOadVADEr){XWQnrczAia = true;}
      if(PTPnYINUkQ == uoAsjRXXUf){GJUFVxAVoV = true;}
      if(bFMcfKACns == FkkNByshft){sGbcdGuQtU = true;}
      while(PsOadVADEr == alMluNbjkt){IzkIryeRZz = true;}
      while(uoAsjRXXUf == uoAsjRXXUf){gfXTniInOF = true;}
      while(FkkNByshft == FkkNByshft){jCcgoJDmoL = true;}
      if(ZjeixGfasB == true){ZjeixGfasB = false;}
      if(okNlgimNQd == true){okNlgimNQd = false;}
      if(ZdTZtdIBYf == true){ZdTZtdIBYf = false;}
      if(cRXJRKKQwc == true){cRXJRKKQwc = false;}
      if(lxeSqTWyfS == true){lxeSqTWyfS = false;}
      if(KeQBUuQEnS == true){KeQBUuQEnS = false;}
      if(yeBNYWNEUV == true){yeBNYWNEUV = false;}
      if(XWQnrczAia == true){XWQnrczAia = false;}
      if(GJUFVxAVoV == true){GJUFVxAVoV = false;}
      if(sGbcdGuQtU == true){sGbcdGuQtU = false;}
      if(GScKumNcJq == true){GScKumNcJq = false;}
      if(wQVNMnJwqb == true){wQVNMnJwqb = false;}
      if(VqPzbRtnJk == true){VqPzbRtnJk = false;}
      if(hhGNUmfKKh == true){hhGNUmfKKh = false;}
      if(iXMxYcjOPp == true){iXMxYcjOPp = false;}
      if(xzZRzODdhc == true){xzZRzODdhc = false;}
      if(sZTYINJHsY == true){sZTYINJHsY = false;}
      if(IzkIryeRZz == true){IzkIryeRZz = false;}
      if(gfXTniInOF == true){gfXTniInOF = false;}
      if(jCcgoJDmoL == true){jCcgoJDmoL = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UXDPFODXAX
{ 
  void cnCEpbtWHE()
  { 
      bool LHNxouIVfg = false;
      bool GPFbUQissP = false;
      bool jeUbLgsbzB = false;
      bool lNTYgwkiEg = false;
      bool VUUNalGKaU = false;
      bool wHnFpglllI = false;
      bool BfVetrwfuY = false;
      bool eznMtOJHAY = false;
      bool ahBeEeZOiA = false;
      bool JzxueozoSh = false;
      bool oZPCoxSaKa = false;
      bool WekMhSepaP = false;
      bool ShxuOfCshF = false;
      bool PUnSiASyoH = false;
      bool MyEHceNjNL = false;
      bool JIODPHcUnI = false;
      bool tZsELCpSdh = false;
      bool AgBSCUdoRi = false;
      bool SSeWwGFkDb = false;
      bool WxKfMmEzwZ = false;
      string tgYislzVtY;
      string VCuYMXXxxt;
      string jYfiJEEtDO;
      string pqaTwnsmWa;
      string WtcZYblhef;
      string XjmxwVBoZn;
      string gFRfidbbbY;
      string ejaWWouFJh;
      string RWPWpFDcWa;
      string mDNhsRcxqt;
      string zKFGBmJQZG;
      string ZueqVKotoc;
      string ZyEAkbWscE;
      string MyOPhHQIVD;
      string wmCPBrXWLi;
      string bhHNTBFEVY;
      string QPmVtafPIN;
      string sgYlgmuozs;
      string EzgtbdqBug;
      string CLmwlfAgUZ;
      if(tgYislzVtY == zKFGBmJQZG){LHNxouIVfg = true;}
      else if(zKFGBmJQZG == tgYislzVtY){oZPCoxSaKa = true;}
      if(VCuYMXXxxt == ZueqVKotoc){GPFbUQissP = true;}
      else if(ZueqVKotoc == VCuYMXXxxt){WekMhSepaP = true;}
      if(jYfiJEEtDO == ZyEAkbWscE){jeUbLgsbzB = true;}
      else if(ZyEAkbWscE == jYfiJEEtDO){ShxuOfCshF = true;}
      if(pqaTwnsmWa == MyOPhHQIVD){lNTYgwkiEg = true;}
      else if(MyOPhHQIVD == pqaTwnsmWa){PUnSiASyoH = true;}
      if(WtcZYblhef == wmCPBrXWLi){VUUNalGKaU = true;}
      else if(wmCPBrXWLi == WtcZYblhef){MyEHceNjNL = true;}
      if(XjmxwVBoZn == bhHNTBFEVY){wHnFpglllI = true;}
      else if(bhHNTBFEVY == XjmxwVBoZn){JIODPHcUnI = true;}
      if(gFRfidbbbY == QPmVtafPIN){BfVetrwfuY = true;}
      else if(QPmVtafPIN == gFRfidbbbY){tZsELCpSdh = true;}
      if(ejaWWouFJh == sgYlgmuozs){eznMtOJHAY = true;}
      if(RWPWpFDcWa == EzgtbdqBug){ahBeEeZOiA = true;}
      if(mDNhsRcxqt == CLmwlfAgUZ){JzxueozoSh = true;}
      while(sgYlgmuozs == ejaWWouFJh){AgBSCUdoRi = true;}
      while(EzgtbdqBug == EzgtbdqBug){SSeWwGFkDb = true;}
      while(CLmwlfAgUZ == CLmwlfAgUZ){WxKfMmEzwZ = true;}
      if(LHNxouIVfg == true){LHNxouIVfg = false;}
      if(GPFbUQissP == true){GPFbUQissP = false;}
      if(jeUbLgsbzB == true){jeUbLgsbzB = false;}
      if(lNTYgwkiEg == true){lNTYgwkiEg = false;}
      if(VUUNalGKaU == true){VUUNalGKaU = false;}
      if(wHnFpglllI == true){wHnFpglllI = false;}
      if(BfVetrwfuY == true){BfVetrwfuY = false;}
      if(eznMtOJHAY == true){eznMtOJHAY = false;}
      if(ahBeEeZOiA == true){ahBeEeZOiA = false;}
      if(JzxueozoSh == true){JzxueozoSh = false;}
      if(oZPCoxSaKa == true){oZPCoxSaKa = false;}
      if(WekMhSepaP == true){WekMhSepaP = false;}
      if(ShxuOfCshF == true){ShxuOfCshF = false;}
      if(PUnSiASyoH == true){PUnSiASyoH = false;}
      if(MyEHceNjNL == true){MyEHceNjNL = false;}
      if(JIODPHcUnI == true){JIODPHcUnI = false;}
      if(tZsELCpSdh == true){tZsELCpSdh = false;}
      if(AgBSCUdoRi == true){AgBSCUdoRi = false;}
      if(SSeWwGFkDb == true){SSeWwGFkDb = false;}
      if(WxKfMmEzwZ == true){WxKfMmEzwZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AAJDSDRJKA
{ 
  void pynqZoEKub()
  { 
      bool mnuzcUoWYU = false;
      bool mqsVgGmAZH = false;
      bool cyKjKNGKpL = false;
      bool VNpXloqQOO = false;
      bool WsPuVpGKAa = false;
      bool pTMnModLoO = false;
      bool OdfNyYBVjr = false;
      bool OVkLjjhPNU = false;
      bool HwJPVBACKw = false;
      bool KebzAsMCww = false;
      bool nkZILUiADb = false;
      bool HNxUHRTQrw = false;
      bool XyKmgiskLp = false;
      bool XeSRfWVBEA = false;
      bool SAyiowhHrh = false;
      bool YuKzXpZlbb = false;
      bool axnMEbufok = false;
      bool wSMqdXzbyj = false;
      bool BlsdRbUTsp = false;
      bool wiYbZULleM = false;
      string FLskpZcsQU;
      string BFkfNaBoiY;
      string NuiyOywnLU;
      string HJKFwuOyln;
      string LApwGfImPP;
      string XOVNbJcuVr;
      string dngwNqtejS;
      string MDCtNNFlqh;
      string oJYwwrnwdm;
      string RSbtKWStiO;
      string kdDwDULaAO;
      string DqDnCTPkzU;
      string DxAwAsaIip;
      string tKDbwKGbra;
      string fMwPiaPnqq;
      string HhuNLncGwK;
      string tDrJTsmarr;
      string NmsXnpBkNZ;
      string cmQriMPoQI;
      string gzfhgcUmup;
      if(FLskpZcsQU == kdDwDULaAO){mnuzcUoWYU = true;}
      else if(kdDwDULaAO == FLskpZcsQU){nkZILUiADb = true;}
      if(BFkfNaBoiY == DqDnCTPkzU){mqsVgGmAZH = true;}
      else if(DqDnCTPkzU == BFkfNaBoiY){HNxUHRTQrw = true;}
      if(NuiyOywnLU == DxAwAsaIip){cyKjKNGKpL = true;}
      else if(DxAwAsaIip == NuiyOywnLU){XyKmgiskLp = true;}
      if(HJKFwuOyln == tKDbwKGbra){VNpXloqQOO = true;}
      else if(tKDbwKGbra == HJKFwuOyln){XeSRfWVBEA = true;}
      if(LApwGfImPP == fMwPiaPnqq){WsPuVpGKAa = true;}
      else if(fMwPiaPnqq == LApwGfImPP){SAyiowhHrh = true;}
      if(XOVNbJcuVr == HhuNLncGwK){pTMnModLoO = true;}
      else if(HhuNLncGwK == XOVNbJcuVr){YuKzXpZlbb = true;}
      if(dngwNqtejS == tDrJTsmarr){OdfNyYBVjr = true;}
      else if(tDrJTsmarr == dngwNqtejS){axnMEbufok = true;}
      if(MDCtNNFlqh == NmsXnpBkNZ){OVkLjjhPNU = true;}
      if(oJYwwrnwdm == cmQriMPoQI){HwJPVBACKw = true;}
      if(RSbtKWStiO == gzfhgcUmup){KebzAsMCww = true;}
      while(NmsXnpBkNZ == MDCtNNFlqh){wSMqdXzbyj = true;}
      while(cmQriMPoQI == cmQriMPoQI){BlsdRbUTsp = true;}
      while(gzfhgcUmup == gzfhgcUmup){wiYbZULleM = true;}
      if(mnuzcUoWYU == true){mnuzcUoWYU = false;}
      if(mqsVgGmAZH == true){mqsVgGmAZH = false;}
      if(cyKjKNGKpL == true){cyKjKNGKpL = false;}
      if(VNpXloqQOO == true){VNpXloqQOO = false;}
      if(WsPuVpGKAa == true){WsPuVpGKAa = false;}
      if(pTMnModLoO == true){pTMnModLoO = false;}
      if(OdfNyYBVjr == true){OdfNyYBVjr = false;}
      if(OVkLjjhPNU == true){OVkLjjhPNU = false;}
      if(HwJPVBACKw == true){HwJPVBACKw = false;}
      if(KebzAsMCww == true){KebzAsMCww = false;}
      if(nkZILUiADb == true){nkZILUiADb = false;}
      if(HNxUHRTQrw == true){HNxUHRTQrw = false;}
      if(XyKmgiskLp == true){XyKmgiskLp = false;}
      if(XeSRfWVBEA == true){XeSRfWVBEA = false;}
      if(SAyiowhHrh == true){SAyiowhHrh = false;}
      if(YuKzXpZlbb == true){YuKzXpZlbb = false;}
      if(axnMEbufok == true){axnMEbufok = false;}
      if(wSMqdXzbyj == true){wSMqdXzbyj = false;}
      if(BlsdRbUTsp == true){BlsdRbUTsp = false;}
      if(wiYbZULleM == true){wiYbZULleM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JMWEATNKLI
{ 
  void XMGQNlPCEO()
  { 
      bool LDCOfFXnsp = false;
      bool XgbgrFuXun = false;
      bool xcNUWDyqBX = false;
      bool YfuswfwbyY = false;
      bool SruZHamVgR = false;
      bool mmtaPipgrb = false;
      bool GflVQqlhjM = false;
      bool JTZWVFgnNu = false;
      bool QcKijlkUcN = false;
      bool pOiVRlFZIP = false;
      bool oEkTHTNXEK = false;
      bool BBTxLdUCke = false;
      bool GQEVVujDEi = false;
      bool zHexFLqpbQ = false;
      bool XNPRMnulJn = false;
      bool RCocJXzPyM = false;
      bool jJSVLtVVrA = false;
      bool mdaUGrAlIg = false;
      bool zysQRwRCng = false;
      bool LarOfOttGS = false;
      string iiIobUWZAz;
      string DlDFYErRFr;
      string IEMZFyYMKR;
      string dOVxsEkMEM;
      string IWwwLxBstN;
      string jkXtudVhid;
      string wKOhUnPGMT;
      string TeMsilHPNI;
      string xeRwgsqbCO;
      string iCdaPyHNbX;
      string hugZtFXeKI;
      string WFUafQrSBb;
      string wmDykRBkck;
      string esnpZByqBb;
      string UMQsqcgCtz;
      string GnMzatrioc;
      string ElBZddRdAe;
      string SAQYGjWKGI;
      string jyqddpfnPe;
      string IOCYXiKpON;
      if(iiIobUWZAz == hugZtFXeKI){LDCOfFXnsp = true;}
      else if(hugZtFXeKI == iiIobUWZAz){oEkTHTNXEK = true;}
      if(DlDFYErRFr == WFUafQrSBb){XgbgrFuXun = true;}
      else if(WFUafQrSBb == DlDFYErRFr){BBTxLdUCke = true;}
      if(IEMZFyYMKR == wmDykRBkck){xcNUWDyqBX = true;}
      else if(wmDykRBkck == IEMZFyYMKR){GQEVVujDEi = true;}
      if(dOVxsEkMEM == esnpZByqBb){YfuswfwbyY = true;}
      else if(esnpZByqBb == dOVxsEkMEM){zHexFLqpbQ = true;}
      if(IWwwLxBstN == UMQsqcgCtz){SruZHamVgR = true;}
      else if(UMQsqcgCtz == IWwwLxBstN){XNPRMnulJn = true;}
      if(jkXtudVhid == GnMzatrioc){mmtaPipgrb = true;}
      else if(GnMzatrioc == jkXtudVhid){RCocJXzPyM = true;}
      if(wKOhUnPGMT == ElBZddRdAe){GflVQqlhjM = true;}
      else if(ElBZddRdAe == wKOhUnPGMT){jJSVLtVVrA = true;}
      if(TeMsilHPNI == SAQYGjWKGI){JTZWVFgnNu = true;}
      if(xeRwgsqbCO == jyqddpfnPe){QcKijlkUcN = true;}
      if(iCdaPyHNbX == IOCYXiKpON){pOiVRlFZIP = true;}
      while(SAQYGjWKGI == TeMsilHPNI){mdaUGrAlIg = true;}
      while(jyqddpfnPe == jyqddpfnPe){zysQRwRCng = true;}
      while(IOCYXiKpON == IOCYXiKpON){LarOfOttGS = true;}
      if(LDCOfFXnsp == true){LDCOfFXnsp = false;}
      if(XgbgrFuXun == true){XgbgrFuXun = false;}
      if(xcNUWDyqBX == true){xcNUWDyqBX = false;}
      if(YfuswfwbyY == true){YfuswfwbyY = false;}
      if(SruZHamVgR == true){SruZHamVgR = false;}
      if(mmtaPipgrb == true){mmtaPipgrb = false;}
      if(GflVQqlhjM == true){GflVQqlhjM = false;}
      if(JTZWVFgnNu == true){JTZWVFgnNu = false;}
      if(QcKijlkUcN == true){QcKijlkUcN = false;}
      if(pOiVRlFZIP == true){pOiVRlFZIP = false;}
      if(oEkTHTNXEK == true){oEkTHTNXEK = false;}
      if(BBTxLdUCke == true){BBTxLdUCke = false;}
      if(GQEVVujDEi == true){GQEVVujDEi = false;}
      if(zHexFLqpbQ == true){zHexFLqpbQ = false;}
      if(XNPRMnulJn == true){XNPRMnulJn = false;}
      if(RCocJXzPyM == true){RCocJXzPyM = false;}
      if(jJSVLtVVrA == true){jJSVLtVVrA = false;}
      if(mdaUGrAlIg == true){mdaUGrAlIg = false;}
      if(zysQRwRCng == true){zysQRwRCng = false;}
      if(LarOfOttGS == true){LarOfOttGS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GGKEJAVQKI
{ 
  void fxeXbeGitl()
  { 
      bool BGXeMFgyEO = false;
      bool yjCKEUmpbw = false;
      bool buSrOafdDq = false;
      bool MCcUbrDxdU = false;
      bool xHYFsgeAWo = false;
      bool qGNCqGnfQl = false;
      bool ZCtNCwHghL = false;
      bool QpLHyUVMzg = false;
      bool ueHOzFnZcV = false;
      bool DXfrutMLCx = false;
      bool ZxSKXBtGEV = false;
      bool kagiptufNw = false;
      bool HgDMHsJTLE = false;
      bool IJJBHyMwpy = false;
      bool LhUHkcqwBA = false;
      bool ygxGorsGCe = false;
      bool qZtANaQebr = false;
      bool hypPkYLwae = false;
      bool CMSLFbMDLP = false;
      bool EGtLGkmUsK = false;
      string jXXQUfYzXW;
      string ALKxhhDZKg;
      string QkAjqUnppT;
      string IkGsCaOxSt;
      string VekFcgeezP;
      string iKLlczXnZz;
      string qhJEOOFRtG;
      string TFGPLlztHJ;
      string pJqZjyZTPC;
      string DaXTDmVgpN;
      string bNjrwdDuaq;
      string AcgQIhpiTQ;
      string okBrDshmMX;
      string QRDsgcZzdG;
      string tesYCZtGKU;
      string wlMZYpLrnD;
      string uPhDZhYmie;
      string IqENVJrbod;
      string yacqadFdUq;
      string QgLaKexpzT;
      if(jXXQUfYzXW == bNjrwdDuaq){BGXeMFgyEO = true;}
      else if(bNjrwdDuaq == jXXQUfYzXW){ZxSKXBtGEV = true;}
      if(ALKxhhDZKg == AcgQIhpiTQ){yjCKEUmpbw = true;}
      else if(AcgQIhpiTQ == ALKxhhDZKg){kagiptufNw = true;}
      if(QkAjqUnppT == okBrDshmMX){buSrOafdDq = true;}
      else if(okBrDshmMX == QkAjqUnppT){HgDMHsJTLE = true;}
      if(IkGsCaOxSt == QRDsgcZzdG){MCcUbrDxdU = true;}
      else if(QRDsgcZzdG == IkGsCaOxSt){IJJBHyMwpy = true;}
      if(VekFcgeezP == tesYCZtGKU){xHYFsgeAWo = true;}
      else if(tesYCZtGKU == VekFcgeezP){LhUHkcqwBA = true;}
      if(iKLlczXnZz == wlMZYpLrnD){qGNCqGnfQl = true;}
      else if(wlMZYpLrnD == iKLlczXnZz){ygxGorsGCe = true;}
      if(qhJEOOFRtG == uPhDZhYmie){ZCtNCwHghL = true;}
      else if(uPhDZhYmie == qhJEOOFRtG){qZtANaQebr = true;}
      if(TFGPLlztHJ == IqENVJrbod){QpLHyUVMzg = true;}
      if(pJqZjyZTPC == yacqadFdUq){ueHOzFnZcV = true;}
      if(DaXTDmVgpN == QgLaKexpzT){DXfrutMLCx = true;}
      while(IqENVJrbod == TFGPLlztHJ){hypPkYLwae = true;}
      while(yacqadFdUq == yacqadFdUq){CMSLFbMDLP = true;}
      while(QgLaKexpzT == QgLaKexpzT){EGtLGkmUsK = true;}
      if(BGXeMFgyEO == true){BGXeMFgyEO = false;}
      if(yjCKEUmpbw == true){yjCKEUmpbw = false;}
      if(buSrOafdDq == true){buSrOafdDq = false;}
      if(MCcUbrDxdU == true){MCcUbrDxdU = false;}
      if(xHYFsgeAWo == true){xHYFsgeAWo = false;}
      if(qGNCqGnfQl == true){qGNCqGnfQl = false;}
      if(ZCtNCwHghL == true){ZCtNCwHghL = false;}
      if(QpLHyUVMzg == true){QpLHyUVMzg = false;}
      if(ueHOzFnZcV == true){ueHOzFnZcV = false;}
      if(DXfrutMLCx == true){DXfrutMLCx = false;}
      if(ZxSKXBtGEV == true){ZxSKXBtGEV = false;}
      if(kagiptufNw == true){kagiptufNw = false;}
      if(HgDMHsJTLE == true){HgDMHsJTLE = false;}
      if(IJJBHyMwpy == true){IJJBHyMwpy = false;}
      if(LhUHkcqwBA == true){LhUHkcqwBA = false;}
      if(ygxGorsGCe == true){ygxGorsGCe = false;}
      if(qZtANaQebr == true){qZtANaQebr = false;}
      if(hypPkYLwae == true){hypPkYLwae = false;}
      if(CMSLFbMDLP == true){CMSLFbMDLP = false;}
      if(EGtLGkmUsK == true){EGtLGkmUsK = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FHMYZSVMBO
{ 
  void damNkOdLmm()
  { 
      bool KMpJzTPoYQ = false;
      bool IKEZIoNiIK = false;
      bool MpAcxxgjlC = false;
      bool ykguYUIHwy = false;
      bool uybKCsXibk = false;
      bool zisJsUMNLC = false;
      bool mfCWSWdIAn = false;
      bool gFlJOOOoaE = false;
      bool YyHBCOrWCE = false;
      bool HtutLPuEre = false;
      bool OPhnDtxite = false;
      bool GWmGcnYBTD = false;
      bool eGFwfmJrxz = false;
      bool SJxwpIPSxI = false;
      bool RIcWxkGoQX = false;
      bool qtknzHJlIQ = false;
      bool jEltLxZqQT = false;
      bool XFELkWdiyu = false;
      bool fUaZHYYJwh = false;
      bool zdKfYNVhKA = false;
      string MJJeOVIiHT;
      string TaEEVShXtq;
      string rFpQUaPhyz;
      string MFfDBBUgRo;
      string tPmEKEFRRK;
      string SNGAtHSsJm;
      string GajIGNtBCU;
      string yAGEwbSlGP;
      string UxQngzzfIt;
      string hbXdhTfdxw;
      string KEOxMRHBDR;
      string nflsSIJubH;
      string OOrIlWccVc;
      string uooJFQiTaO;
      string rxbpFBodLZ;
      string JjBohuMGKp;
      string ndeaVzpbmB;
      string ZpUMPQlBCj;
      string gzQwSLmAbK;
      string EbIkaNRBUG;
      if(MJJeOVIiHT == KEOxMRHBDR){KMpJzTPoYQ = true;}
      else if(KEOxMRHBDR == MJJeOVIiHT){OPhnDtxite = true;}
      if(TaEEVShXtq == nflsSIJubH){IKEZIoNiIK = true;}
      else if(nflsSIJubH == TaEEVShXtq){GWmGcnYBTD = true;}
      if(rFpQUaPhyz == OOrIlWccVc){MpAcxxgjlC = true;}
      else if(OOrIlWccVc == rFpQUaPhyz){eGFwfmJrxz = true;}
      if(MFfDBBUgRo == uooJFQiTaO){ykguYUIHwy = true;}
      else if(uooJFQiTaO == MFfDBBUgRo){SJxwpIPSxI = true;}
      if(tPmEKEFRRK == rxbpFBodLZ){uybKCsXibk = true;}
      else if(rxbpFBodLZ == tPmEKEFRRK){RIcWxkGoQX = true;}
      if(SNGAtHSsJm == JjBohuMGKp){zisJsUMNLC = true;}
      else if(JjBohuMGKp == SNGAtHSsJm){qtknzHJlIQ = true;}
      if(GajIGNtBCU == ndeaVzpbmB){mfCWSWdIAn = true;}
      else if(ndeaVzpbmB == GajIGNtBCU){jEltLxZqQT = true;}
      if(yAGEwbSlGP == ZpUMPQlBCj){gFlJOOOoaE = true;}
      if(UxQngzzfIt == gzQwSLmAbK){YyHBCOrWCE = true;}
      if(hbXdhTfdxw == EbIkaNRBUG){HtutLPuEre = true;}
      while(ZpUMPQlBCj == yAGEwbSlGP){XFELkWdiyu = true;}
      while(gzQwSLmAbK == gzQwSLmAbK){fUaZHYYJwh = true;}
      while(EbIkaNRBUG == EbIkaNRBUG){zdKfYNVhKA = true;}
      if(KMpJzTPoYQ == true){KMpJzTPoYQ = false;}
      if(IKEZIoNiIK == true){IKEZIoNiIK = false;}
      if(MpAcxxgjlC == true){MpAcxxgjlC = false;}
      if(ykguYUIHwy == true){ykguYUIHwy = false;}
      if(uybKCsXibk == true){uybKCsXibk = false;}
      if(zisJsUMNLC == true){zisJsUMNLC = false;}
      if(mfCWSWdIAn == true){mfCWSWdIAn = false;}
      if(gFlJOOOoaE == true){gFlJOOOoaE = false;}
      if(YyHBCOrWCE == true){YyHBCOrWCE = false;}
      if(HtutLPuEre == true){HtutLPuEre = false;}
      if(OPhnDtxite == true){OPhnDtxite = false;}
      if(GWmGcnYBTD == true){GWmGcnYBTD = false;}
      if(eGFwfmJrxz == true){eGFwfmJrxz = false;}
      if(SJxwpIPSxI == true){SJxwpIPSxI = false;}
      if(RIcWxkGoQX == true){RIcWxkGoQX = false;}
      if(qtknzHJlIQ == true){qtknzHJlIQ = false;}
      if(jEltLxZqQT == true){jEltLxZqQT = false;}
      if(XFELkWdiyu == true){XFELkWdiyu = false;}
      if(fUaZHYYJwh == true){fUaZHYYJwh = false;}
      if(zdKfYNVhKA == true){zdKfYNVhKA = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AUZNDYTXLX
{ 
  void TpRtoHKznq()
  { 
      bool jhrgccKxUA = false;
      bool UEZGPbwoHO = false;
      bool oNpUBWrBiB = false;
      bool KYIoDjrJiA = false;
      bool slRSjeKEbX = false;
      bool opUeiWqXeG = false;
      bool jIOTpuwjCf = false;
      bool jlGGVCuXjV = false;
      bool MmZKMUDdFg = false;
      bool rTdcPjXxco = false;
      bool iZCunCiOIf = false;
      bool aTmIsxhDGV = false;
      bool qRdUmPHdqk = false;
      bool eytumlkdWK = false;
      bool koUZGYgUiS = false;
      bool BUqHxQugIX = false;
      bool tEXWBcYrVH = false;
      bool LDgwjCxEON = false;
      bool iltXiKGmxW = false;
      bool pEVyPfRxXd = false;
      string UThmVcbSfr;
      string ChuSpXDekd;
      string nnKqYjXsgK;
      string OxIsTmChdW;
      string aIxwTLXzUO;
      string juOzFfIqOl;
      string EilVdFYCzC;
      string VVAOCeqNWN;
      string LYYSaShJmm;
      string eLSrRrWaSN;
      string lVARrOuzwa;
      string ouTBafKSuK;
      string XPOLXKCIxK;
      string HPuJapYGZM;
      string XhNMyjrRRk;
      string gDfIpWGnth;
      string LmCkSYzJZj;
      string HEsimpBCNn;
      string RjpMdDjlSE;
      string ijLqAkecsY;
      if(UThmVcbSfr == lVARrOuzwa){jhrgccKxUA = true;}
      else if(lVARrOuzwa == UThmVcbSfr){iZCunCiOIf = true;}
      if(ChuSpXDekd == ouTBafKSuK){UEZGPbwoHO = true;}
      else if(ouTBafKSuK == ChuSpXDekd){aTmIsxhDGV = true;}
      if(nnKqYjXsgK == XPOLXKCIxK){oNpUBWrBiB = true;}
      else if(XPOLXKCIxK == nnKqYjXsgK){qRdUmPHdqk = true;}
      if(OxIsTmChdW == HPuJapYGZM){KYIoDjrJiA = true;}
      else if(HPuJapYGZM == OxIsTmChdW){eytumlkdWK = true;}
      if(aIxwTLXzUO == XhNMyjrRRk){slRSjeKEbX = true;}
      else if(XhNMyjrRRk == aIxwTLXzUO){koUZGYgUiS = true;}
      if(juOzFfIqOl == gDfIpWGnth){opUeiWqXeG = true;}
      else if(gDfIpWGnth == juOzFfIqOl){BUqHxQugIX = true;}
      if(EilVdFYCzC == LmCkSYzJZj){jIOTpuwjCf = true;}
      else if(LmCkSYzJZj == EilVdFYCzC){tEXWBcYrVH = true;}
      if(VVAOCeqNWN == HEsimpBCNn){jlGGVCuXjV = true;}
      if(LYYSaShJmm == RjpMdDjlSE){MmZKMUDdFg = true;}
      if(eLSrRrWaSN == ijLqAkecsY){rTdcPjXxco = true;}
      while(HEsimpBCNn == VVAOCeqNWN){LDgwjCxEON = true;}
      while(RjpMdDjlSE == RjpMdDjlSE){iltXiKGmxW = true;}
      while(ijLqAkecsY == ijLqAkecsY){pEVyPfRxXd = true;}
      if(jhrgccKxUA == true){jhrgccKxUA = false;}
      if(UEZGPbwoHO == true){UEZGPbwoHO = false;}
      if(oNpUBWrBiB == true){oNpUBWrBiB = false;}
      if(KYIoDjrJiA == true){KYIoDjrJiA = false;}
      if(slRSjeKEbX == true){slRSjeKEbX = false;}
      if(opUeiWqXeG == true){opUeiWqXeG = false;}
      if(jIOTpuwjCf == true){jIOTpuwjCf = false;}
      if(jlGGVCuXjV == true){jlGGVCuXjV = false;}
      if(MmZKMUDdFg == true){MmZKMUDdFg = false;}
      if(rTdcPjXxco == true){rTdcPjXxco = false;}
      if(iZCunCiOIf == true){iZCunCiOIf = false;}
      if(aTmIsxhDGV == true){aTmIsxhDGV = false;}
      if(qRdUmPHdqk == true){qRdUmPHdqk = false;}
      if(eytumlkdWK == true){eytumlkdWK = false;}
      if(koUZGYgUiS == true){koUZGYgUiS = false;}
      if(BUqHxQugIX == true){BUqHxQugIX = false;}
      if(tEXWBcYrVH == true){tEXWBcYrVH = false;}
      if(LDgwjCxEON == true){LDgwjCxEON = false;}
      if(iltXiKGmxW == true){iltXiKGmxW = false;}
      if(pEVyPfRxXd == true){pEVyPfRxXd = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MJWJNDUTPW
{ 
  void NTrGOJTpLZ()
  { 
      bool myhuKNXXxR = false;
      bool KiuyRZTDFh = false;
      bool dxVYGuEANL = false;
      bool iJxUlRoZBZ = false;
      bool KHFOqrZlJE = false;
      bool IJaupobJoO = false;
      bool JXJliBlZMA = false;
      bool jsCeWVrGgO = false;
      bool URpJCYhBew = false;
      bool ofklQmySfp = false;
      bool FAZJTOHuYU = false;
      bool xVbZtqgziJ = false;
      bool SjuPKEIPOi = false;
      bool WYapIUrgom = false;
      bool RmcJyTDKrY = false;
      bool GziWXUFJyP = false;
      bool GqfoxYtkRY = false;
      bool yzJwqeHkQq = false;
      bool AxXocXoESm = false;
      bool OiiZfaGmMZ = false;
      string hOPpszNUiF;
      string fMXZqzwbTq;
      string AupgzIRUZG;
      string qXoKWBicgg;
      string wgNHwjrHZQ;
      string yjpeSmXiqj;
      string hsucOHWNDo;
      string QSumtTgijO;
      string QKtJHHgJri;
      string czlwzCkkso;
      string MeEkzenDjy;
      string FoybTWzlUH;
      string ZNUTMQgynq;
      string hwouBkGJJb;
      string SdYsdmBJcb;
      string DzPlOzKjXh;
      string xoPsQJACah;
      string QpbrnwoaoY;
      string PXVLnhfCgB;
      string aVkbdcNfcU;
      if(hOPpszNUiF == MeEkzenDjy){myhuKNXXxR = true;}
      else if(MeEkzenDjy == hOPpszNUiF){FAZJTOHuYU = true;}
      if(fMXZqzwbTq == FoybTWzlUH){KiuyRZTDFh = true;}
      else if(FoybTWzlUH == fMXZqzwbTq){xVbZtqgziJ = true;}
      if(AupgzIRUZG == ZNUTMQgynq){dxVYGuEANL = true;}
      else if(ZNUTMQgynq == AupgzIRUZG){SjuPKEIPOi = true;}
      if(qXoKWBicgg == hwouBkGJJb){iJxUlRoZBZ = true;}
      else if(hwouBkGJJb == qXoKWBicgg){WYapIUrgom = true;}
      if(wgNHwjrHZQ == SdYsdmBJcb){KHFOqrZlJE = true;}
      else if(SdYsdmBJcb == wgNHwjrHZQ){RmcJyTDKrY = true;}
      if(yjpeSmXiqj == DzPlOzKjXh){IJaupobJoO = true;}
      else if(DzPlOzKjXh == yjpeSmXiqj){GziWXUFJyP = true;}
      if(hsucOHWNDo == xoPsQJACah){JXJliBlZMA = true;}
      else if(xoPsQJACah == hsucOHWNDo){GqfoxYtkRY = true;}
      if(QSumtTgijO == QpbrnwoaoY){jsCeWVrGgO = true;}
      if(QKtJHHgJri == PXVLnhfCgB){URpJCYhBew = true;}
      if(czlwzCkkso == aVkbdcNfcU){ofklQmySfp = true;}
      while(QpbrnwoaoY == QSumtTgijO){yzJwqeHkQq = true;}
      while(PXVLnhfCgB == PXVLnhfCgB){AxXocXoESm = true;}
      while(aVkbdcNfcU == aVkbdcNfcU){OiiZfaGmMZ = true;}
      if(myhuKNXXxR == true){myhuKNXXxR = false;}
      if(KiuyRZTDFh == true){KiuyRZTDFh = false;}
      if(dxVYGuEANL == true){dxVYGuEANL = false;}
      if(iJxUlRoZBZ == true){iJxUlRoZBZ = false;}
      if(KHFOqrZlJE == true){KHFOqrZlJE = false;}
      if(IJaupobJoO == true){IJaupobJoO = false;}
      if(JXJliBlZMA == true){JXJliBlZMA = false;}
      if(jsCeWVrGgO == true){jsCeWVrGgO = false;}
      if(URpJCYhBew == true){URpJCYhBew = false;}
      if(ofklQmySfp == true){ofklQmySfp = false;}
      if(FAZJTOHuYU == true){FAZJTOHuYU = false;}
      if(xVbZtqgziJ == true){xVbZtqgziJ = false;}
      if(SjuPKEIPOi == true){SjuPKEIPOi = false;}
      if(WYapIUrgom == true){WYapIUrgom = false;}
      if(RmcJyTDKrY == true){RmcJyTDKrY = false;}
      if(GziWXUFJyP == true){GziWXUFJyP = false;}
      if(GqfoxYtkRY == true){GqfoxYtkRY = false;}
      if(yzJwqeHkQq == true){yzJwqeHkQq = false;}
      if(AxXocXoESm == true){AxXocXoESm = false;}
      if(OiiZfaGmMZ == true){OiiZfaGmMZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MFGBSMEPYD
{ 
  void DDRzZyTUYc()
  { 
      bool mCqJhhVJaz = false;
      bool XHSSIVRxaJ = false;
      bool DNYSbTWQXn = false;
      bool WefDZHosch = false;
      bool iexkOlNFLp = false;
      bool YycSZZdYgO = false;
      bool LFlmcrbPsB = false;
      bool gYUraYQmEp = false;
      bool FQFhPYBMdA = false;
      bool cQMNWcrKIV = false;
      bool JyMJIzcjqB = false;
      bool IqTpItzEWD = false;
      bool pOksXPLGVy = false;
      bool fFdhfHDuYP = false;
      bool wRVbqlwWEb = false;
      bool cnmEfuMTls = false;
      bool nVOsUCMlJB = false;
      bool qEfHHYrIBl = false;
      bool SoCsUrSYCj = false;
      bool tjJfaZDcHn = false;
      string VQZwMQsMxM;
      string CwIGIhrDxa;
      string WRaQVIuUNB;
      string HWmnDejUMl;
      string DeoBVsjIwl;
      string EsOITEoIZD;
      string UYzwEfITDc;
      string xXqLXrbZtc;
      string YmYawOHeTy;
      string oatJQdeswp;
      string jPFSogCDjb;
      string bPOxdhAXzp;
      string XdAEmVcgts;
      string MidErIgFlP;
      string LEtrWZMwei;
      string cHsFSGPAfS;
      string DUIqOslJFV;
      string AukjeSiIhM;
      string JgCLPFoXkl;
      string sgYTMquDcd;
      if(VQZwMQsMxM == jPFSogCDjb){mCqJhhVJaz = true;}
      else if(jPFSogCDjb == VQZwMQsMxM){JyMJIzcjqB = true;}
      if(CwIGIhrDxa == bPOxdhAXzp){XHSSIVRxaJ = true;}
      else if(bPOxdhAXzp == CwIGIhrDxa){IqTpItzEWD = true;}
      if(WRaQVIuUNB == XdAEmVcgts){DNYSbTWQXn = true;}
      else if(XdAEmVcgts == WRaQVIuUNB){pOksXPLGVy = true;}
      if(HWmnDejUMl == MidErIgFlP){WefDZHosch = true;}
      else if(MidErIgFlP == HWmnDejUMl){fFdhfHDuYP = true;}
      if(DeoBVsjIwl == LEtrWZMwei){iexkOlNFLp = true;}
      else if(LEtrWZMwei == DeoBVsjIwl){wRVbqlwWEb = true;}
      if(EsOITEoIZD == cHsFSGPAfS){YycSZZdYgO = true;}
      else if(cHsFSGPAfS == EsOITEoIZD){cnmEfuMTls = true;}
      if(UYzwEfITDc == DUIqOslJFV){LFlmcrbPsB = true;}
      else if(DUIqOslJFV == UYzwEfITDc){nVOsUCMlJB = true;}
      if(xXqLXrbZtc == AukjeSiIhM){gYUraYQmEp = true;}
      if(YmYawOHeTy == JgCLPFoXkl){FQFhPYBMdA = true;}
      if(oatJQdeswp == sgYTMquDcd){cQMNWcrKIV = true;}
      while(AukjeSiIhM == xXqLXrbZtc){qEfHHYrIBl = true;}
      while(JgCLPFoXkl == JgCLPFoXkl){SoCsUrSYCj = true;}
      while(sgYTMquDcd == sgYTMquDcd){tjJfaZDcHn = true;}
      if(mCqJhhVJaz == true){mCqJhhVJaz = false;}
      if(XHSSIVRxaJ == true){XHSSIVRxaJ = false;}
      if(DNYSbTWQXn == true){DNYSbTWQXn = false;}
      if(WefDZHosch == true){WefDZHosch = false;}
      if(iexkOlNFLp == true){iexkOlNFLp = false;}
      if(YycSZZdYgO == true){YycSZZdYgO = false;}
      if(LFlmcrbPsB == true){LFlmcrbPsB = false;}
      if(gYUraYQmEp == true){gYUraYQmEp = false;}
      if(FQFhPYBMdA == true){FQFhPYBMdA = false;}
      if(cQMNWcrKIV == true){cQMNWcrKIV = false;}
      if(JyMJIzcjqB == true){JyMJIzcjqB = false;}
      if(IqTpItzEWD == true){IqTpItzEWD = false;}
      if(pOksXPLGVy == true){pOksXPLGVy = false;}
      if(fFdhfHDuYP == true){fFdhfHDuYP = false;}
      if(wRVbqlwWEb == true){wRVbqlwWEb = false;}
      if(cnmEfuMTls == true){cnmEfuMTls = false;}
      if(nVOsUCMlJB == true){nVOsUCMlJB = false;}
      if(qEfHHYrIBl == true){qEfHHYrIBl = false;}
      if(SoCsUrSYCj == true){SoCsUrSYCj = false;}
      if(tjJfaZDcHn == true){tjJfaZDcHn = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BVWWQIXGXR
{ 
  void RfqeTWfnzt()
  { 
      bool CFaurtGKAE = false;
      bool CgmlZJjeMr = false;
      bool ZithboSnJB = false;
      bool kAWPEeXkRE = false;
      bool toczzgbuVB = false;
      bool rSeMcSLsSD = false;
      bool aSBySBJWht = false;
      bool lzMHxjGuhu = false;
      bool NXRHISUJmk = false;
      bool TdRNcbndfy = false;
      bool WEMbPsFXPr = false;
      bool MPLXoDAspz = false;
      bool fzjxZHhzdV = false;
      bool wbOiXJKprP = false;
      bool NxtuYDSsYZ = false;
      bool aunfRWUPBI = false;
      bool LGssukBbAf = false;
      bool rKAaliEoYW = false;
      bool nnZEtBmhRA = false;
      bool ntnjDHEAzm = false;
      string StBPbBZire;
      string LwDcSRSouI;
      string zHOLrEFHmM;
      string dHmrwOiPWR;
      string obDDLCwiSh;
      string WBZaILeoWW;
      string uRufBDJLgE;
      string hCScSXEKzp;
      string IWoFochLjj;
      string isrzVXfWCI;
      string rkCNzTiwHk;
      string HUogNYbppX;
      string UxawUoKSHu;
      string qgHqxXUaGU;
      string slEycODKlx;
      string upfemuJFsK;
      string JQoMsZMyjp;
      string EBjVdisbSG;
      string kzfKzBAcDC;
      string NOhuLcxrFC;
      if(StBPbBZire == rkCNzTiwHk){CFaurtGKAE = true;}
      else if(rkCNzTiwHk == StBPbBZire){WEMbPsFXPr = true;}
      if(LwDcSRSouI == HUogNYbppX){CgmlZJjeMr = true;}
      else if(HUogNYbppX == LwDcSRSouI){MPLXoDAspz = true;}
      if(zHOLrEFHmM == UxawUoKSHu){ZithboSnJB = true;}
      else if(UxawUoKSHu == zHOLrEFHmM){fzjxZHhzdV = true;}
      if(dHmrwOiPWR == qgHqxXUaGU){kAWPEeXkRE = true;}
      else if(qgHqxXUaGU == dHmrwOiPWR){wbOiXJKprP = true;}
      if(obDDLCwiSh == slEycODKlx){toczzgbuVB = true;}
      else if(slEycODKlx == obDDLCwiSh){NxtuYDSsYZ = true;}
      if(WBZaILeoWW == upfemuJFsK){rSeMcSLsSD = true;}
      else if(upfemuJFsK == WBZaILeoWW){aunfRWUPBI = true;}
      if(uRufBDJLgE == JQoMsZMyjp){aSBySBJWht = true;}
      else if(JQoMsZMyjp == uRufBDJLgE){LGssukBbAf = true;}
      if(hCScSXEKzp == EBjVdisbSG){lzMHxjGuhu = true;}
      if(IWoFochLjj == kzfKzBAcDC){NXRHISUJmk = true;}
      if(isrzVXfWCI == NOhuLcxrFC){TdRNcbndfy = true;}
      while(EBjVdisbSG == hCScSXEKzp){rKAaliEoYW = true;}
      while(kzfKzBAcDC == kzfKzBAcDC){nnZEtBmhRA = true;}
      while(NOhuLcxrFC == NOhuLcxrFC){ntnjDHEAzm = true;}
      if(CFaurtGKAE == true){CFaurtGKAE = false;}
      if(CgmlZJjeMr == true){CgmlZJjeMr = false;}
      if(ZithboSnJB == true){ZithboSnJB = false;}
      if(kAWPEeXkRE == true){kAWPEeXkRE = false;}
      if(toczzgbuVB == true){toczzgbuVB = false;}
      if(rSeMcSLsSD == true){rSeMcSLsSD = false;}
      if(aSBySBJWht == true){aSBySBJWht = false;}
      if(lzMHxjGuhu == true){lzMHxjGuhu = false;}
      if(NXRHISUJmk == true){NXRHISUJmk = false;}
      if(TdRNcbndfy == true){TdRNcbndfy = false;}
      if(WEMbPsFXPr == true){WEMbPsFXPr = false;}
      if(MPLXoDAspz == true){MPLXoDAspz = false;}
      if(fzjxZHhzdV == true){fzjxZHhzdV = false;}
      if(wbOiXJKprP == true){wbOiXJKprP = false;}
      if(NxtuYDSsYZ == true){NxtuYDSsYZ = false;}
      if(aunfRWUPBI == true){aunfRWUPBI = false;}
      if(LGssukBbAf == true){LGssukBbAf = false;}
      if(rKAaliEoYW == true){rKAaliEoYW = false;}
      if(nnZEtBmhRA == true){nnZEtBmhRA = false;}
      if(ntnjDHEAzm == true){ntnjDHEAzm = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class EGYENFWEAE
{ 
  void SxIPsNybqs()
  { 
      bool YTEuquSzKV = false;
      bool xQuWOCtwCU = false;
      bool HUDKrsSdFu = false;
      bool lfAcgOQzgV = false;
      bool JbVndWtCRB = false;
      bool moyrbEQtCx = false;
      bool bnsPjQPSoz = false;
      bool ltOfLfyPfV = false;
      bool zQMEIxhHAc = false;
      bool yWxitUhGDN = false;
      bool HbMSPrZSgw = false;
      bool GANgyLboGm = false;
      bool iHRMAPxflq = false;
      bool xqLSOXDsnP = false;
      bool WOaEVyIQkh = false;
      bool rrzQdzJzSA = false;
      bool tsfoyOouSa = false;
      bool HhApPTfMqC = false;
      bool GguDyFrtnO = false;
      bool NEanUdikGH = false;
      string dWpzscQSjY;
      string zymPOPMope;
      string gamwQWDGgm;
      string nyCushuRuh;
      string rGraWbxNrk;
      string cFaKeDjshW;
      string YJkwjAuqWf;
      string UusHlahLZn;
      string YpOuJiXrQo;
      string yBjXJhUwpV;
      string gGqLQhLSXE;
      string VXSsxiuplB;
      string bCaghSKVyE;
      string GlVEHSzDeb;
      string XFULTaipZa;
      string mdKnseeJdy;
      string WoTTxiBOle;
      string irLKdWgONG;
      string iUuXQbbgKb;
      string mpYgEhkBNj;
      if(dWpzscQSjY == gGqLQhLSXE){YTEuquSzKV = true;}
      else if(gGqLQhLSXE == dWpzscQSjY){HbMSPrZSgw = true;}
      if(zymPOPMope == VXSsxiuplB){xQuWOCtwCU = true;}
      else if(VXSsxiuplB == zymPOPMope){GANgyLboGm = true;}
      if(gamwQWDGgm == bCaghSKVyE){HUDKrsSdFu = true;}
      else if(bCaghSKVyE == gamwQWDGgm){iHRMAPxflq = true;}
      if(nyCushuRuh == GlVEHSzDeb){lfAcgOQzgV = true;}
      else if(GlVEHSzDeb == nyCushuRuh){xqLSOXDsnP = true;}
      if(rGraWbxNrk == XFULTaipZa){JbVndWtCRB = true;}
      else if(XFULTaipZa == rGraWbxNrk){WOaEVyIQkh = true;}
      if(cFaKeDjshW == mdKnseeJdy){moyrbEQtCx = true;}
      else if(mdKnseeJdy == cFaKeDjshW){rrzQdzJzSA = true;}
      if(YJkwjAuqWf == WoTTxiBOle){bnsPjQPSoz = true;}
      else if(WoTTxiBOle == YJkwjAuqWf){tsfoyOouSa = true;}
      if(UusHlahLZn == irLKdWgONG){ltOfLfyPfV = true;}
      if(YpOuJiXrQo == iUuXQbbgKb){zQMEIxhHAc = true;}
      if(yBjXJhUwpV == mpYgEhkBNj){yWxitUhGDN = true;}
      while(irLKdWgONG == UusHlahLZn){HhApPTfMqC = true;}
      while(iUuXQbbgKb == iUuXQbbgKb){GguDyFrtnO = true;}
      while(mpYgEhkBNj == mpYgEhkBNj){NEanUdikGH = true;}
      if(YTEuquSzKV == true){YTEuquSzKV = false;}
      if(xQuWOCtwCU == true){xQuWOCtwCU = false;}
      if(HUDKrsSdFu == true){HUDKrsSdFu = false;}
      if(lfAcgOQzgV == true){lfAcgOQzgV = false;}
      if(JbVndWtCRB == true){JbVndWtCRB = false;}
      if(moyrbEQtCx == true){moyrbEQtCx = false;}
      if(bnsPjQPSoz == true){bnsPjQPSoz = false;}
      if(ltOfLfyPfV == true){ltOfLfyPfV = false;}
      if(zQMEIxhHAc == true){zQMEIxhHAc = false;}
      if(yWxitUhGDN == true){yWxitUhGDN = false;}
      if(HbMSPrZSgw == true){HbMSPrZSgw = false;}
      if(GANgyLboGm == true){GANgyLboGm = false;}
      if(iHRMAPxflq == true){iHRMAPxflq = false;}
      if(xqLSOXDsnP == true){xqLSOXDsnP = false;}
      if(WOaEVyIQkh == true){WOaEVyIQkh = false;}
      if(rrzQdzJzSA == true){rrzQdzJzSA = false;}
      if(tsfoyOouSa == true){tsfoyOouSa = false;}
      if(HhApPTfMqC == true){HhApPTfMqC = false;}
      if(GguDyFrtnO == true){GguDyFrtnO = false;}
      if(NEanUdikGH == true){NEanUdikGH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GOIEOAZYYX
{ 
  void HiawuokCVk()
  { 
      bool AwhREWZBFh = false;
      bool ZXtLVcIGhF = false;
      bool xXmfxAiDCo = false;
      bool UDdzRHcbXD = false;
      bool zyZYPwLOnB = false;
      bool huLMibIBUE = false;
      bool nWzPNWtusO = false;
      bool zghdFyyOQA = false;
      bool nUOAGZqQPp = false;
      bool yaGSPcyKLs = false;
      bool CPUZBBMgGk = false;
      bool dtkTQSHzIn = false;
      bool ecfDQXZONc = false;
      bool znDXyXzoyE = false;
      bool MKKeMDpmCx = false;
      bool UzBXOZjRIc = false;
      bool bxTdkopMUr = false;
      bool TSADaTRBqM = false;
      bool hmMQHAlfJY = false;
      bool mhfUcOqgsW = false;
      string NcSjxXQUxN;
      string ctSYONEHqG;
      string sRhrUsSzPc;
      string RQdjFTWCqD;
      string IqCGWltCuK;
      string GtJMtwuaPD;
      string EQEUwMQhSt;
      string pCPmxBPaVh;
      string zQEFWWGRpJ;
      string XwlmzqmgKK;
      string VLiAUDdWOs;
      string fTFhEnMzyY;
      string GzrJWtPcnF;
      string NiQKLmgDkS;
      string rjCwdDpEiL;
      string enhkEWTZdZ;
      string nBhIexDtVQ;
      string GIOffVwueS;
      string DLsbaZAeGo;
      string TGZKdqBsRf;
      if(NcSjxXQUxN == VLiAUDdWOs){AwhREWZBFh = true;}
      else if(VLiAUDdWOs == NcSjxXQUxN){CPUZBBMgGk = true;}
      if(ctSYONEHqG == fTFhEnMzyY){ZXtLVcIGhF = true;}
      else if(fTFhEnMzyY == ctSYONEHqG){dtkTQSHzIn = true;}
      if(sRhrUsSzPc == GzrJWtPcnF){xXmfxAiDCo = true;}
      else if(GzrJWtPcnF == sRhrUsSzPc){ecfDQXZONc = true;}
      if(RQdjFTWCqD == NiQKLmgDkS){UDdzRHcbXD = true;}
      else if(NiQKLmgDkS == RQdjFTWCqD){znDXyXzoyE = true;}
      if(IqCGWltCuK == rjCwdDpEiL){zyZYPwLOnB = true;}
      else if(rjCwdDpEiL == IqCGWltCuK){MKKeMDpmCx = true;}
      if(GtJMtwuaPD == enhkEWTZdZ){huLMibIBUE = true;}
      else if(enhkEWTZdZ == GtJMtwuaPD){UzBXOZjRIc = true;}
      if(EQEUwMQhSt == nBhIexDtVQ){nWzPNWtusO = true;}
      else if(nBhIexDtVQ == EQEUwMQhSt){bxTdkopMUr = true;}
      if(pCPmxBPaVh == GIOffVwueS){zghdFyyOQA = true;}
      if(zQEFWWGRpJ == DLsbaZAeGo){nUOAGZqQPp = true;}
      if(XwlmzqmgKK == TGZKdqBsRf){yaGSPcyKLs = true;}
      while(GIOffVwueS == pCPmxBPaVh){TSADaTRBqM = true;}
      while(DLsbaZAeGo == DLsbaZAeGo){hmMQHAlfJY = true;}
      while(TGZKdqBsRf == TGZKdqBsRf){mhfUcOqgsW = true;}
      if(AwhREWZBFh == true){AwhREWZBFh = false;}
      if(ZXtLVcIGhF == true){ZXtLVcIGhF = false;}
      if(xXmfxAiDCo == true){xXmfxAiDCo = false;}
      if(UDdzRHcbXD == true){UDdzRHcbXD = false;}
      if(zyZYPwLOnB == true){zyZYPwLOnB = false;}
      if(huLMibIBUE == true){huLMibIBUE = false;}
      if(nWzPNWtusO == true){nWzPNWtusO = false;}
      if(zghdFyyOQA == true){zghdFyyOQA = false;}
      if(nUOAGZqQPp == true){nUOAGZqQPp = false;}
      if(yaGSPcyKLs == true){yaGSPcyKLs = false;}
      if(CPUZBBMgGk == true){CPUZBBMgGk = false;}
      if(dtkTQSHzIn == true){dtkTQSHzIn = false;}
      if(ecfDQXZONc == true){ecfDQXZONc = false;}
      if(znDXyXzoyE == true){znDXyXzoyE = false;}
      if(MKKeMDpmCx == true){MKKeMDpmCx = false;}
      if(UzBXOZjRIc == true){UzBXOZjRIc = false;}
      if(bxTdkopMUr == true){bxTdkopMUr = false;}
      if(TSADaTRBqM == true){TSADaTRBqM = false;}
      if(hmMQHAlfJY == true){hmMQHAlfJY = false;}
      if(mhfUcOqgsW == true){mhfUcOqgsW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NGAZUXIEPS
{ 
  void orGTHlQHVU()
  { 
      bool ZuGEGILJaK = false;
      bool MFsCHOLapl = false;
      bool ncAPForALo = false;
      bool iZBEsnjLrr = false;
      bool VbnawiBxPg = false;
      bool XNqMqwupyQ = false;
      bool QLykbmEnIS = false;
      bool dzhxPMnYTW = false;
      bool ntjeRVhnMx = false;
      bool stNbbKcDzI = false;
      bool dMnxCYetxe = false;
      bool IVVbcVaTSF = false;
      bool FPaLZjlGVE = false;
      bool ijZLrbFmbs = false;
      bool pmqhnWXkzF = false;
      bool iJjsDNkcWS = false;
      bool mYXruXuKjA = false;
      bool PnrZPRYsWC = false;
      bool LAHQVZAgId = false;
      bool pcPIGRPzmQ = false;
      string mhVXTbkysn;
      string piuKrsZzbV;
      string QiZYmLfuoK;
      string zafZlhlTwq;
      string EeHZjFsqRQ;
      string hwyLWDIwjH;
      string upJinOCVXK;
      string HGDjQlaAog;
      string SocXbDcUDh;
      string ZoinuOUXkU;
      string DXSHJbrOtj;
      string cHGDoGNlHk;
      string lhRokRWPnC;
      string QCPUdtWVOA;
      string LOZSFRhfzo;
      string AiQAiUFcVq;
      string FbZrTZrlYt;
      string irWGYXHzjT;
      string jpQyBhlPlI;
      string otzWuinhye;
      if(mhVXTbkysn == DXSHJbrOtj){ZuGEGILJaK = true;}
      else if(DXSHJbrOtj == mhVXTbkysn){dMnxCYetxe = true;}
      if(piuKrsZzbV == cHGDoGNlHk){MFsCHOLapl = true;}
      else if(cHGDoGNlHk == piuKrsZzbV){IVVbcVaTSF = true;}
      if(QiZYmLfuoK == lhRokRWPnC){ncAPForALo = true;}
      else if(lhRokRWPnC == QiZYmLfuoK){FPaLZjlGVE = true;}
      if(zafZlhlTwq == QCPUdtWVOA){iZBEsnjLrr = true;}
      else if(QCPUdtWVOA == zafZlhlTwq){ijZLrbFmbs = true;}
      if(EeHZjFsqRQ == LOZSFRhfzo){VbnawiBxPg = true;}
      else if(LOZSFRhfzo == EeHZjFsqRQ){pmqhnWXkzF = true;}
      if(hwyLWDIwjH == AiQAiUFcVq){XNqMqwupyQ = true;}
      else if(AiQAiUFcVq == hwyLWDIwjH){iJjsDNkcWS = true;}
      if(upJinOCVXK == FbZrTZrlYt){QLykbmEnIS = true;}
      else if(FbZrTZrlYt == upJinOCVXK){mYXruXuKjA = true;}
      if(HGDjQlaAog == irWGYXHzjT){dzhxPMnYTW = true;}
      if(SocXbDcUDh == jpQyBhlPlI){ntjeRVhnMx = true;}
      if(ZoinuOUXkU == otzWuinhye){stNbbKcDzI = true;}
      while(irWGYXHzjT == HGDjQlaAog){PnrZPRYsWC = true;}
      while(jpQyBhlPlI == jpQyBhlPlI){LAHQVZAgId = true;}
      while(otzWuinhye == otzWuinhye){pcPIGRPzmQ = true;}
      if(ZuGEGILJaK == true){ZuGEGILJaK = false;}
      if(MFsCHOLapl == true){MFsCHOLapl = false;}
      if(ncAPForALo == true){ncAPForALo = false;}
      if(iZBEsnjLrr == true){iZBEsnjLrr = false;}
      if(VbnawiBxPg == true){VbnawiBxPg = false;}
      if(XNqMqwupyQ == true){XNqMqwupyQ = false;}
      if(QLykbmEnIS == true){QLykbmEnIS = false;}
      if(dzhxPMnYTW == true){dzhxPMnYTW = false;}
      if(ntjeRVhnMx == true){ntjeRVhnMx = false;}
      if(stNbbKcDzI == true){stNbbKcDzI = false;}
      if(dMnxCYetxe == true){dMnxCYetxe = false;}
      if(IVVbcVaTSF == true){IVVbcVaTSF = false;}
      if(FPaLZjlGVE == true){FPaLZjlGVE = false;}
      if(ijZLrbFmbs == true){ijZLrbFmbs = false;}
      if(pmqhnWXkzF == true){pmqhnWXkzF = false;}
      if(iJjsDNkcWS == true){iJjsDNkcWS = false;}
      if(mYXruXuKjA == true){mYXruXuKjA = false;}
      if(PnrZPRYsWC == true){PnrZPRYsWC = false;}
      if(LAHQVZAgId == true){LAHQVZAgId = false;}
      if(pcPIGRPzmQ == true){pcPIGRPzmQ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RYQDFRLAIO
{ 
  void MKHDWZjreA()
  { 
      bool BLdbHXHpgM = false;
      bool ZhkaJxeQhb = false;
      bool EIXnPaWmkZ = false;
      bool mNeySiUESd = false;
      bool HkstUiiiBg = false;
      bool FFnFSrmpLf = false;
      bool BTFGYLDmHZ = false;
      bool baxETSPcSf = false;
      bool rmlDThZZeZ = false;
      bool uerlbnelVI = false;
      bool fuaaFeHikA = false;
      bool cKWfnOhCmp = false;
      bool kdHhewiKbf = false;
      bool ncHKAwLKxn = false;
      bool apnewkIMyN = false;
      bool GbyeXCnswJ = false;
      bool wwHEMzCMfr = false;
      bool DWFyPtocls = false;
      bool mWMOZLeblx = false;
      bool IRNmrdiiRw = false;
      string qnpeoHbIYy;
      string zVPAnNAoAd;
      string myVMnzdmjA;
      string syjqJBbptA;
      string ftUErkFWjc;
      string RuSnFrRcpo;
      string coXXFBkXql;
      string kKtAxrdRUD;
      string tHCHhxewlR;
      string izzEXqWwzq;
      string zgfxqHBSpM;
      string NAImuVlPpH;
      string VolYDlLobG;
      string VhRcENaEQa;
      string wMFYDiqMJC;
      string EGGrVJuGtE;
      string wLpiqfhehh;
      string tgzPKJjMVT;
      string uzfZBObZHt;
      string RTKLdkVVhX;
      if(qnpeoHbIYy == zgfxqHBSpM){BLdbHXHpgM = true;}
      else if(zgfxqHBSpM == qnpeoHbIYy){fuaaFeHikA = true;}
      if(zVPAnNAoAd == NAImuVlPpH){ZhkaJxeQhb = true;}
      else if(NAImuVlPpH == zVPAnNAoAd){cKWfnOhCmp = true;}
      if(myVMnzdmjA == VolYDlLobG){EIXnPaWmkZ = true;}
      else if(VolYDlLobG == myVMnzdmjA){kdHhewiKbf = true;}
      if(syjqJBbptA == VhRcENaEQa){mNeySiUESd = true;}
      else if(VhRcENaEQa == syjqJBbptA){ncHKAwLKxn = true;}
      if(ftUErkFWjc == wMFYDiqMJC){HkstUiiiBg = true;}
      else if(wMFYDiqMJC == ftUErkFWjc){apnewkIMyN = true;}
      if(RuSnFrRcpo == EGGrVJuGtE){FFnFSrmpLf = true;}
      else if(EGGrVJuGtE == RuSnFrRcpo){GbyeXCnswJ = true;}
      if(coXXFBkXql == wLpiqfhehh){BTFGYLDmHZ = true;}
      else if(wLpiqfhehh == coXXFBkXql){wwHEMzCMfr = true;}
      if(kKtAxrdRUD == tgzPKJjMVT){baxETSPcSf = true;}
      if(tHCHhxewlR == uzfZBObZHt){rmlDThZZeZ = true;}
      if(izzEXqWwzq == RTKLdkVVhX){uerlbnelVI = true;}
      while(tgzPKJjMVT == kKtAxrdRUD){DWFyPtocls = true;}
      while(uzfZBObZHt == uzfZBObZHt){mWMOZLeblx = true;}
      while(RTKLdkVVhX == RTKLdkVVhX){IRNmrdiiRw = true;}
      if(BLdbHXHpgM == true){BLdbHXHpgM = false;}
      if(ZhkaJxeQhb == true){ZhkaJxeQhb = false;}
      if(EIXnPaWmkZ == true){EIXnPaWmkZ = false;}
      if(mNeySiUESd == true){mNeySiUESd = false;}
      if(HkstUiiiBg == true){HkstUiiiBg = false;}
      if(FFnFSrmpLf == true){FFnFSrmpLf = false;}
      if(BTFGYLDmHZ == true){BTFGYLDmHZ = false;}
      if(baxETSPcSf == true){baxETSPcSf = false;}
      if(rmlDThZZeZ == true){rmlDThZZeZ = false;}
      if(uerlbnelVI == true){uerlbnelVI = false;}
      if(fuaaFeHikA == true){fuaaFeHikA = false;}
      if(cKWfnOhCmp == true){cKWfnOhCmp = false;}
      if(kdHhewiKbf == true){kdHhewiKbf = false;}
      if(ncHKAwLKxn == true){ncHKAwLKxn = false;}
      if(apnewkIMyN == true){apnewkIMyN = false;}
      if(GbyeXCnswJ == true){GbyeXCnswJ = false;}
      if(wwHEMzCMfr == true){wwHEMzCMfr = false;}
      if(DWFyPtocls == true){DWFyPtocls = false;}
      if(mWMOZLeblx == true){mWMOZLeblx = false;}
      if(IRNmrdiiRw == true){IRNmrdiiRw = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YJOJBEUABR
{ 
  void JBCyAICaEK()
  { 
      bool sExIArDLRY = false;
      bool bUfuwBPxWf = false;
      bool LNqcAzBZpi = false;
      bool eaWrQHQWib = false;
      bool qsjKTGMiPY = false;
      bool HGbByeNTHB = false;
      bool qdVitAymdo = false;
      bool jsNtnRcgJt = false;
      bool mSJQbXzVtk = false;
      bool twOhkNXBPS = false;
      bool HYEaoOsgMf = false;
      bool BfKMNXTEGV = false;
      bool BcsrumdeJa = false;
      bool tbQqmNNibN = false;
      bool eRIOjNlCTM = false;
      bool sSttRdfLfB = false;
      bool jNWllZPSYA = false;
      bool KkcsqiUdIh = false;
      bool BaLwgagWZI = false;
      bool hCCZcQtWcJ = false;
      string MbwBRQTsTK;
      string KZxqiZCDPi;
      string KxOKSDGzOU;
      string MwkPnOjZaY;
      string FbgrqFSQrq;
      string dlBLtONlhN;
      string KUeyxOngDT;
      string pbcqchPVTN;
      string koGWqatYUZ;
      string AyqMNkxiub;
      string QkNXBBJJiM;
      string yqRUbUNHGW;
      string TPMdSrkPcq;
      string kgaqwAbiVp;
      string QkpIaJfVjs;
      string WobXIBYiEI;
      string PtgqaQQaYb;
      string relQFiZDCJ;
      string MOejUuFCiu;
      string asdMZWDygJ;
      if(MbwBRQTsTK == QkNXBBJJiM){sExIArDLRY = true;}
      else if(QkNXBBJJiM == MbwBRQTsTK){HYEaoOsgMf = true;}
      if(KZxqiZCDPi == yqRUbUNHGW){bUfuwBPxWf = true;}
      else if(yqRUbUNHGW == KZxqiZCDPi){BfKMNXTEGV = true;}
      if(KxOKSDGzOU == TPMdSrkPcq){LNqcAzBZpi = true;}
      else if(TPMdSrkPcq == KxOKSDGzOU){BcsrumdeJa = true;}
      if(MwkPnOjZaY == kgaqwAbiVp){eaWrQHQWib = true;}
      else if(kgaqwAbiVp == MwkPnOjZaY){tbQqmNNibN = true;}
      if(FbgrqFSQrq == QkpIaJfVjs){qsjKTGMiPY = true;}
      else if(QkpIaJfVjs == FbgrqFSQrq){eRIOjNlCTM = true;}
      if(dlBLtONlhN == WobXIBYiEI){HGbByeNTHB = true;}
      else if(WobXIBYiEI == dlBLtONlhN){sSttRdfLfB = true;}
      if(KUeyxOngDT == PtgqaQQaYb){qdVitAymdo = true;}
      else if(PtgqaQQaYb == KUeyxOngDT){jNWllZPSYA = true;}
      if(pbcqchPVTN == relQFiZDCJ){jsNtnRcgJt = true;}
      if(koGWqatYUZ == MOejUuFCiu){mSJQbXzVtk = true;}
      if(AyqMNkxiub == asdMZWDygJ){twOhkNXBPS = true;}
      while(relQFiZDCJ == pbcqchPVTN){KkcsqiUdIh = true;}
      while(MOejUuFCiu == MOejUuFCiu){BaLwgagWZI = true;}
      while(asdMZWDygJ == asdMZWDygJ){hCCZcQtWcJ = true;}
      if(sExIArDLRY == true){sExIArDLRY = false;}
      if(bUfuwBPxWf == true){bUfuwBPxWf = false;}
      if(LNqcAzBZpi == true){LNqcAzBZpi = false;}
      if(eaWrQHQWib == true){eaWrQHQWib = false;}
      if(qsjKTGMiPY == true){qsjKTGMiPY = false;}
      if(HGbByeNTHB == true){HGbByeNTHB = false;}
      if(qdVitAymdo == true){qdVitAymdo = false;}
      if(jsNtnRcgJt == true){jsNtnRcgJt = false;}
      if(mSJQbXzVtk == true){mSJQbXzVtk = false;}
      if(twOhkNXBPS == true){twOhkNXBPS = false;}
      if(HYEaoOsgMf == true){HYEaoOsgMf = false;}
      if(BfKMNXTEGV == true){BfKMNXTEGV = false;}
      if(BcsrumdeJa == true){BcsrumdeJa = false;}
      if(tbQqmNNibN == true){tbQqmNNibN = false;}
      if(eRIOjNlCTM == true){eRIOjNlCTM = false;}
      if(sSttRdfLfB == true){sSttRdfLfB = false;}
      if(jNWllZPSYA == true){jNWllZPSYA = false;}
      if(KkcsqiUdIh == true){KkcsqiUdIh = false;}
      if(BaLwgagWZI == true){BaLwgagWZI = false;}
      if(hCCZcQtWcJ == true){hCCZcQtWcJ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LQPQQGMCVX
{ 
  void TkwLrIxIbx()
  { 
      bool CxoCFPUSQe = false;
      bool reWfZfhTwa = false;
      bool LQgzkeZZmw = false;
      bool CBDRHAckpw = false;
      bool AoyBQLsYMx = false;
      bool edZYckPpQa = false;
      bool pufUbqbFYY = false;
      bool DwczaWRfiZ = false;
      bool kBMpRYadnV = false;
      bool kmUOGEIdYY = false;
      bool imzmshAayG = false;
      bool TjHppnhpDC = false;
      bool PxRCnQbNrV = false;
      bool NnGJqxBZwi = false;
      bool XAfgCAedYS = false;
      bool VSZVJkVwzc = false;
      bool Glibsedyzr = false;
      bool lItUeOQjAm = false;
      bool kxoZAuNjzF = false;
      bool bFZSbkREnX = false;
      string OHQjkrOYmT;
      string UqwiXzyfKH;
      string XihnKfOILB;
      string GerBAbJEiU;
      string cQNcjRabdV;
      string JjDwGfYbZE;
      string cFMkMpytiu;
      string otwoeGUEEr;
      string EmeprVbwuT;
      string CgddqWDcPs;
      string SgrifOnNQz;
      string LUXgApaWsq;
      string OCyOrHPRxI;
      string CqnKYXtuKc;
      string hNewIhJxdB;
      string ugjYeQhdpX;
      string zVPIwTfMHb;
      string ueLQIAXCGJ;
      string ySnwMVHOfI;
      string pQFldMxVzo;
      if(OHQjkrOYmT == SgrifOnNQz){CxoCFPUSQe = true;}
      else if(SgrifOnNQz == OHQjkrOYmT){imzmshAayG = true;}
      if(UqwiXzyfKH == LUXgApaWsq){reWfZfhTwa = true;}
      else if(LUXgApaWsq == UqwiXzyfKH){TjHppnhpDC = true;}
      if(XihnKfOILB == OCyOrHPRxI){LQgzkeZZmw = true;}
      else if(OCyOrHPRxI == XihnKfOILB){PxRCnQbNrV = true;}
      if(GerBAbJEiU == CqnKYXtuKc){CBDRHAckpw = true;}
      else if(CqnKYXtuKc == GerBAbJEiU){NnGJqxBZwi = true;}
      if(cQNcjRabdV == hNewIhJxdB){AoyBQLsYMx = true;}
      else if(hNewIhJxdB == cQNcjRabdV){XAfgCAedYS = true;}
      if(JjDwGfYbZE == ugjYeQhdpX){edZYckPpQa = true;}
      else if(ugjYeQhdpX == JjDwGfYbZE){VSZVJkVwzc = true;}
      if(cFMkMpytiu == zVPIwTfMHb){pufUbqbFYY = true;}
      else if(zVPIwTfMHb == cFMkMpytiu){Glibsedyzr = true;}
      if(otwoeGUEEr == ueLQIAXCGJ){DwczaWRfiZ = true;}
      if(EmeprVbwuT == ySnwMVHOfI){kBMpRYadnV = true;}
      if(CgddqWDcPs == pQFldMxVzo){kmUOGEIdYY = true;}
      while(ueLQIAXCGJ == otwoeGUEEr){lItUeOQjAm = true;}
      while(ySnwMVHOfI == ySnwMVHOfI){kxoZAuNjzF = true;}
      while(pQFldMxVzo == pQFldMxVzo){bFZSbkREnX = true;}
      if(CxoCFPUSQe == true){CxoCFPUSQe = false;}
      if(reWfZfhTwa == true){reWfZfhTwa = false;}
      if(LQgzkeZZmw == true){LQgzkeZZmw = false;}
      if(CBDRHAckpw == true){CBDRHAckpw = false;}
      if(AoyBQLsYMx == true){AoyBQLsYMx = false;}
      if(edZYckPpQa == true){edZYckPpQa = false;}
      if(pufUbqbFYY == true){pufUbqbFYY = false;}
      if(DwczaWRfiZ == true){DwczaWRfiZ = false;}
      if(kBMpRYadnV == true){kBMpRYadnV = false;}
      if(kmUOGEIdYY == true){kmUOGEIdYY = false;}
      if(imzmshAayG == true){imzmshAayG = false;}
      if(TjHppnhpDC == true){TjHppnhpDC = false;}
      if(PxRCnQbNrV == true){PxRCnQbNrV = false;}
      if(NnGJqxBZwi == true){NnGJqxBZwi = false;}
      if(XAfgCAedYS == true){XAfgCAedYS = false;}
      if(VSZVJkVwzc == true){VSZVJkVwzc = false;}
      if(Glibsedyzr == true){Glibsedyzr = false;}
      if(lItUeOQjAm == true){lItUeOQjAm = false;}
      if(kxoZAuNjzF == true){kxoZAuNjzF = false;}
      if(bFZSbkREnX == true){bFZSbkREnX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class EKVLCQSMSS
{ 
  void RgxXycLQYw()
  { 
      bool XcbXsgxxBX = false;
      bool mOsWEwKKJR = false;
      bool JaZChzYASD = false;
      bool rmZChKIwph = false;
      bool YgBMSsteYA = false;
      bool BUftfwDZps = false;
      bool Hwdyfznpot = false;
      bool mNUbsbZNjo = false;
      bool FtaSBWHfVk = false;
      bool HiGwabCDfS = false;
      bool EDpeMmAEAD = false;
      bool WpoAnMULwI = false;
      bool rIUDlEHXAQ = false;
      bool ZVgQkBEwFO = false;
      bool WCeRORsFIN = false;
      bool GqGXauilrI = false;
      bool ebDidAJYOl = false;
      bool IYFglpcCoU = false;
      bool KBPmKhPIpo = false;
      bool SRDxitzunN = false;
      string UKXIpNZOLT;
      string duaTzFSupM;
      string EoVDwBUHTe;
      string KxpDnpydAj;
      string dZoebOImsb;
      string sHOeQeoTMo;
      string rSufIfFXfI;
      string GntGhdApbg;
      string rgNrkDhLlI;
      string BeRLqLeIVu;
      string JkHjUFNIwm;
      string eyqfxLECuF;
      string yskWHbWiOW;
      string yqTgWHYdXe;
      string pZPzdCmkOf;
      string HbMQwQzkbs;
      string hesYnneKIp;
      string maePOYckrF;
      string nkUiOVdDjG;
      string HmBdxLfpLp;
      if(UKXIpNZOLT == JkHjUFNIwm){XcbXsgxxBX = true;}
      else if(JkHjUFNIwm == UKXIpNZOLT){EDpeMmAEAD = true;}
      if(duaTzFSupM == eyqfxLECuF){mOsWEwKKJR = true;}
      else if(eyqfxLECuF == duaTzFSupM){WpoAnMULwI = true;}
      if(EoVDwBUHTe == yskWHbWiOW){JaZChzYASD = true;}
      else if(yskWHbWiOW == EoVDwBUHTe){rIUDlEHXAQ = true;}
      if(KxpDnpydAj == yqTgWHYdXe){rmZChKIwph = true;}
      else if(yqTgWHYdXe == KxpDnpydAj){ZVgQkBEwFO = true;}
      if(dZoebOImsb == pZPzdCmkOf){YgBMSsteYA = true;}
      else if(pZPzdCmkOf == dZoebOImsb){WCeRORsFIN = true;}
      if(sHOeQeoTMo == HbMQwQzkbs){BUftfwDZps = true;}
      else if(HbMQwQzkbs == sHOeQeoTMo){GqGXauilrI = true;}
      if(rSufIfFXfI == hesYnneKIp){Hwdyfznpot = true;}
      else if(hesYnneKIp == rSufIfFXfI){ebDidAJYOl = true;}
      if(GntGhdApbg == maePOYckrF){mNUbsbZNjo = true;}
      if(rgNrkDhLlI == nkUiOVdDjG){FtaSBWHfVk = true;}
      if(BeRLqLeIVu == HmBdxLfpLp){HiGwabCDfS = true;}
      while(maePOYckrF == GntGhdApbg){IYFglpcCoU = true;}
      while(nkUiOVdDjG == nkUiOVdDjG){KBPmKhPIpo = true;}
      while(HmBdxLfpLp == HmBdxLfpLp){SRDxitzunN = true;}
      if(XcbXsgxxBX == true){XcbXsgxxBX = false;}
      if(mOsWEwKKJR == true){mOsWEwKKJR = false;}
      if(JaZChzYASD == true){JaZChzYASD = false;}
      if(rmZChKIwph == true){rmZChKIwph = false;}
      if(YgBMSsteYA == true){YgBMSsteYA = false;}
      if(BUftfwDZps == true){BUftfwDZps = false;}
      if(Hwdyfznpot == true){Hwdyfznpot = false;}
      if(mNUbsbZNjo == true){mNUbsbZNjo = false;}
      if(FtaSBWHfVk == true){FtaSBWHfVk = false;}
      if(HiGwabCDfS == true){HiGwabCDfS = false;}
      if(EDpeMmAEAD == true){EDpeMmAEAD = false;}
      if(WpoAnMULwI == true){WpoAnMULwI = false;}
      if(rIUDlEHXAQ == true){rIUDlEHXAQ = false;}
      if(ZVgQkBEwFO == true){ZVgQkBEwFO = false;}
      if(WCeRORsFIN == true){WCeRORsFIN = false;}
      if(GqGXauilrI == true){GqGXauilrI = false;}
      if(ebDidAJYOl == true){ebDidAJYOl = false;}
      if(IYFglpcCoU == true){IYFglpcCoU = false;}
      if(KBPmKhPIpo == true){KBPmKhPIpo = false;}
      if(SRDxitzunN == true){SRDxitzunN = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KIORAAKOPC
{ 
  void lhQpbPjUmZ()
  { 
      bool ymklGEpCat = false;
      bool YKIUDqDZbg = false;
      bool IJJGqCSykn = false;
      bool ssfblZQhnm = false;
      bool WgDMofiDRL = false;
      bool AYDoEndeuD = false;
      bool lqRlBedfic = false;
      bool aVlnKbGxQK = false;
      bool TOfZnTppXy = false;
      bool wkmjBeOQKJ = false;
      bool BCUnjelPeb = false;
      bool ggNjiWLryg = false;
      bool omIjgoiBme = false;
      bool MplHJRsroJ = false;
      bool gaLuQPWCAe = false;
      bool TWSdTNtjHt = false;
      bool ukbzPqtGaR = false;
      bool cSCozObMTQ = false;
      bool RfBHLoXImu = false;
      bool WIjtNwyMfY = false;
      string msfCEGYMfs;
      string TOkLnttaeT;
      string aQZfmbpmku;
      string nafhsrNAhA;
      string HpNpoEmIbP;
      string wDsVAVTAOb;
      string QkbkErhDRw;
      string ftseMjDiph;
      string ogLVzkhJhX;
      string XNlGjVOclx;
      string WjqeiCKOJP;
      string SkJfmiYFkS;
      string KYVsYRHmtp;
      string VawsLFoflx;
      string aSzREmEFTA;
      string xIqSlyOxGR;
      string tWTTtrnQTg;
      string trSnrrYDok;
      string NCTtrMidGN;
      string pqYUDDyeWM;
      if(msfCEGYMfs == WjqeiCKOJP){ymklGEpCat = true;}
      else if(WjqeiCKOJP == msfCEGYMfs){BCUnjelPeb = true;}
      if(TOkLnttaeT == SkJfmiYFkS){YKIUDqDZbg = true;}
      else if(SkJfmiYFkS == TOkLnttaeT){ggNjiWLryg = true;}
      if(aQZfmbpmku == KYVsYRHmtp){IJJGqCSykn = true;}
      else if(KYVsYRHmtp == aQZfmbpmku){omIjgoiBme = true;}
      if(nafhsrNAhA == VawsLFoflx){ssfblZQhnm = true;}
      else if(VawsLFoflx == nafhsrNAhA){MplHJRsroJ = true;}
      if(HpNpoEmIbP == aSzREmEFTA){WgDMofiDRL = true;}
      else if(aSzREmEFTA == HpNpoEmIbP){gaLuQPWCAe = true;}
      if(wDsVAVTAOb == xIqSlyOxGR){AYDoEndeuD = true;}
      else if(xIqSlyOxGR == wDsVAVTAOb){TWSdTNtjHt = true;}
      if(QkbkErhDRw == tWTTtrnQTg){lqRlBedfic = true;}
      else if(tWTTtrnQTg == QkbkErhDRw){ukbzPqtGaR = true;}
      if(ftseMjDiph == trSnrrYDok){aVlnKbGxQK = true;}
      if(ogLVzkhJhX == NCTtrMidGN){TOfZnTppXy = true;}
      if(XNlGjVOclx == pqYUDDyeWM){wkmjBeOQKJ = true;}
      while(trSnrrYDok == ftseMjDiph){cSCozObMTQ = true;}
      while(NCTtrMidGN == NCTtrMidGN){RfBHLoXImu = true;}
      while(pqYUDDyeWM == pqYUDDyeWM){WIjtNwyMfY = true;}
      if(ymklGEpCat == true){ymklGEpCat = false;}
      if(YKIUDqDZbg == true){YKIUDqDZbg = false;}
      if(IJJGqCSykn == true){IJJGqCSykn = false;}
      if(ssfblZQhnm == true){ssfblZQhnm = false;}
      if(WgDMofiDRL == true){WgDMofiDRL = false;}
      if(AYDoEndeuD == true){AYDoEndeuD = false;}
      if(lqRlBedfic == true){lqRlBedfic = false;}
      if(aVlnKbGxQK == true){aVlnKbGxQK = false;}
      if(TOfZnTppXy == true){TOfZnTppXy = false;}
      if(wkmjBeOQKJ == true){wkmjBeOQKJ = false;}
      if(BCUnjelPeb == true){BCUnjelPeb = false;}
      if(ggNjiWLryg == true){ggNjiWLryg = false;}
      if(omIjgoiBme == true){omIjgoiBme = false;}
      if(MplHJRsroJ == true){MplHJRsroJ = false;}
      if(gaLuQPWCAe == true){gaLuQPWCAe = false;}
      if(TWSdTNtjHt == true){TWSdTNtjHt = false;}
      if(ukbzPqtGaR == true){ukbzPqtGaR = false;}
      if(cSCozObMTQ == true){cSCozObMTQ = false;}
      if(RfBHLoXImu == true){RfBHLoXImu = false;}
      if(WIjtNwyMfY == true){WIjtNwyMfY = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZEMKXBMPKL
{ 
  void JXnRKenqSb()
  { 
      bool hYCbkUbuXh = false;
      bool SVAeWedjyy = false;
      bool YYZsnzbgyO = false;
      bool KLFmsxAnzd = false;
      bool EwslmUHSJa = false;
      bool REZsiIzJnl = false;
      bool ruxIWGPNkZ = false;
      bool dpkTsbfmGS = false;
      bool qIXWwGlMDk = false;
      bool xqAMkJzjOf = false;
      bool EthjxbSFzH = false;
      bool cLDUtiXZMQ = false;
      bool UDJAbmoYhi = false;
      bool buXZOwamZT = false;
      bool mfMwHlmfRC = false;
      bool lYXyKAkgHK = false;
      bool mtXPegmdGx = false;
      bool HjArcoqMsp = false;
      bool zIGfxDXZON = false;
      bool LtfPgsIwRZ = false;
      string lbxTKThkzN;
      string JzaZPJwuWw;
      string WttFublmqe;
      string xpeeYyGhcm;
      string XWLEKcgRYk;
      string CkhQEqgqYh;
      string gtsUPSTuBQ;
      string DORiXAiouR;
      string uUNGbLIOag;
      string QOItFccqnr;
      string IlPHgnSyrD;
      string IrwHmMgmrE;
      string JFTTAnFXdu;
      string XqBCIIEFYn;
      string rJVWYYSBGs;
      string wpzjhCRwHr;
      string huUxfQpsye;
      string HaNDLAjwub;
      string VDQjzgrRTF;
      string pIyfFwoXqH;
      if(lbxTKThkzN == IlPHgnSyrD){hYCbkUbuXh = true;}
      else if(IlPHgnSyrD == lbxTKThkzN){EthjxbSFzH = true;}
      if(JzaZPJwuWw == IrwHmMgmrE){SVAeWedjyy = true;}
      else if(IrwHmMgmrE == JzaZPJwuWw){cLDUtiXZMQ = true;}
      if(WttFublmqe == JFTTAnFXdu){YYZsnzbgyO = true;}
      else if(JFTTAnFXdu == WttFublmqe){UDJAbmoYhi = true;}
      if(xpeeYyGhcm == XqBCIIEFYn){KLFmsxAnzd = true;}
      else if(XqBCIIEFYn == xpeeYyGhcm){buXZOwamZT = true;}
      if(XWLEKcgRYk == rJVWYYSBGs){EwslmUHSJa = true;}
      else if(rJVWYYSBGs == XWLEKcgRYk){mfMwHlmfRC = true;}
      if(CkhQEqgqYh == wpzjhCRwHr){REZsiIzJnl = true;}
      else if(wpzjhCRwHr == CkhQEqgqYh){lYXyKAkgHK = true;}
      if(gtsUPSTuBQ == huUxfQpsye){ruxIWGPNkZ = true;}
      else if(huUxfQpsye == gtsUPSTuBQ){mtXPegmdGx = true;}
      if(DORiXAiouR == HaNDLAjwub){dpkTsbfmGS = true;}
      if(uUNGbLIOag == VDQjzgrRTF){qIXWwGlMDk = true;}
      if(QOItFccqnr == pIyfFwoXqH){xqAMkJzjOf = true;}
      while(HaNDLAjwub == DORiXAiouR){HjArcoqMsp = true;}
      while(VDQjzgrRTF == VDQjzgrRTF){zIGfxDXZON = true;}
      while(pIyfFwoXqH == pIyfFwoXqH){LtfPgsIwRZ = true;}
      if(hYCbkUbuXh == true){hYCbkUbuXh = false;}
      if(SVAeWedjyy == true){SVAeWedjyy = false;}
      if(YYZsnzbgyO == true){YYZsnzbgyO = false;}
      if(KLFmsxAnzd == true){KLFmsxAnzd = false;}
      if(EwslmUHSJa == true){EwslmUHSJa = false;}
      if(REZsiIzJnl == true){REZsiIzJnl = false;}
      if(ruxIWGPNkZ == true){ruxIWGPNkZ = false;}
      if(dpkTsbfmGS == true){dpkTsbfmGS = false;}
      if(qIXWwGlMDk == true){qIXWwGlMDk = false;}
      if(xqAMkJzjOf == true){xqAMkJzjOf = false;}
      if(EthjxbSFzH == true){EthjxbSFzH = false;}
      if(cLDUtiXZMQ == true){cLDUtiXZMQ = false;}
      if(UDJAbmoYhi == true){UDJAbmoYhi = false;}
      if(buXZOwamZT == true){buXZOwamZT = false;}
      if(mfMwHlmfRC == true){mfMwHlmfRC = false;}
      if(lYXyKAkgHK == true){lYXyKAkgHK = false;}
      if(mtXPegmdGx == true){mtXPegmdGx = false;}
      if(HjArcoqMsp == true){HjArcoqMsp = false;}
      if(zIGfxDXZON == true){zIGfxDXZON = false;}
      if(LtfPgsIwRZ == true){LtfPgsIwRZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class QVYJVJYUJL
{ 
  void tCAyQlLMUx()
  { 
      bool KOppiHyVfU = false;
      bool kbIXwXiCsM = false;
      bool VRXprFnZYU = false;
      bool ZQauLmhfnT = false;
      bool RytzhRqyDe = false;
      bool VjTXPrpAyd = false;
      bool TXwLtTAMLJ = false;
      bool oCNZtXWYHa = false;
      bool OoVsypxCfT = false;
      bool whhKztBiFN = false;
      bool QdmspSVsdr = false;
      bool URrXOeCDMD = false;
      bool WxgrMHUDaA = false;
      bool VnStJkCbDf = false;
      bool UDYmyeoJwU = false;
      bool rxVBpSwtfi = false;
      bool xSScVgyyNd = false;
      bool cZAgiTYtth = false;
      bool RLGFIXRRDq = false;
      bool nHuTiCjuRR = false;
      string UyMDhGNDGG;
      string UNnaZaKFOo;
      string MTECARZEZH;
      string JpChjXNtLQ;
      string CaZjooUmVT;
      string kxmDCBtJDQ;
      string fIdzweLFpC;
      string MPsxmksUNL;
      string wliFVMDyei;
      string cEqujsxanJ;
      string cOxSGVDoPR;
      string HZwqTtZYci;
      string bMZHYIXyNt;
      string nRfDihoPyA;
      string JOiNaDHzbo;
      string SXyFhPnosy;
      string xFaVcossIk;
      string FKZbYNdMgV;
      string BYVXbPTaUS;
      string szWADEmNkk;
      if(UyMDhGNDGG == cOxSGVDoPR){KOppiHyVfU = true;}
      else if(cOxSGVDoPR == UyMDhGNDGG){QdmspSVsdr = true;}
      if(UNnaZaKFOo == HZwqTtZYci){kbIXwXiCsM = true;}
      else if(HZwqTtZYci == UNnaZaKFOo){URrXOeCDMD = true;}
      if(MTECARZEZH == bMZHYIXyNt){VRXprFnZYU = true;}
      else if(bMZHYIXyNt == MTECARZEZH){WxgrMHUDaA = true;}
      if(JpChjXNtLQ == nRfDihoPyA){ZQauLmhfnT = true;}
      else if(nRfDihoPyA == JpChjXNtLQ){VnStJkCbDf = true;}
      if(CaZjooUmVT == JOiNaDHzbo){RytzhRqyDe = true;}
      else if(JOiNaDHzbo == CaZjooUmVT){UDYmyeoJwU = true;}
      if(kxmDCBtJDQ == SXyFhPnosy){VjTXPrpAyd = true;}
      else if(SXyFhPnosy == kxmDCBtJDQ){rxVBpSwtfi = true;}
      if(fIdzweLFpC == xFaVcossIk){TXwLtTAMLJ = true;}
      else if(xFaVcossIk == fIdzweLFpC){xSScVgyyNd = true;}
      if(MPsxmksUNL == FKZbYNdMgV){oCNZtXWYHa = true;}
      if(wliFVMDyei == BYVXbPTaUS){OoVsypxCfT = true;}
      if(cEqujsxanJ == szWADEmNkk){whhKztBiFN = true;}
      while(FKZbYNdMgV == MPsxmksUNL){cZAgiTYtth = true;}
      while(BYVXbPTaUS == BYVXbPTaUS){RLGFIXRRDq = true;}
      while(szWADEmNkk == szWADEmNkk){nHuTiCjuRR = true;}
      if(KOppiHyVfU == true){KOppiHyVfU = false;}
      if(kbIXwXiCsM == true){kbIXwXiCsM = false;}
      if(VRXprFnZYU == true){VRXprFnZYU = false;}
      if(ZQauLmhfnT == true){ZQauLmhfnT = false;}
      if(RytzhRqyDe == true){RytzhRqyDe = false;}
      if(VjTXPrpAyd == true){VjTXPrpAyd = false;}
      if(TXwLtTAMLJ == true){TXwLtTAMLJ = false;}
      if(oCNZtXWYHa == true){oCNZtXWYHa = false;}
      if(OoVsypxCfT == true){OoVsypxCfT = false;}
      if(whhKztBiFN == true){whhKztBiFN = false;}
      if(QdmspSVsdr == true){QdmspSVsdr = false;}
      if(URrXOeCDMD == true){URrXOeCDMD = false;}
      if(WxgrMHUDaA == true){WxgrMHUDaA = false;}
      if(VnStJkCbDf == true){VnStJkCbDf = false;}
      if(UDYmyeoJwU == true){UDYmyeoJwU = false;}
      if(rxVBpSwtfi == true){rxVBpSwtfi = false;}
      if(xSScVgyyNd == true){xSScVgyyNd = false;}
      if(cZAgiTYtth == true){cZAgiTYtth = false;}
      if(RLGFIXRRDq == true){RLGFIXRRDq = false;}
      if(nHuTiCjuRR == true){nHuTiCjuRR = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AEOEQFUPWB
{ 
  void oeyumzWsQm()
  { 
      bool CsWsuzqRzd = false;
      bool gsJktftnBE = false;
      bool KHlxURalbx = false;
      bool YPiuVtowQD = false;
      bool bhjJrEikPk = false;
      bool jYhQstHYYd = false;
      bool XFpCERKOPA = false;
      bool qxWybcXcFP = false;
      bool hoVpNzgSBa = false;
      bool lPWmeWrhEZ = false;
      bool tUVOtlBinQ = false;
      bool WdTIuiizoF = false;
      bool kqMnNbajSF = false;
      bool koXfVNTGwa = false;
      bool xxdjEIwOqw = false;
      bool mrszoCHgBS = false;
      bool LQPifgGmlh = false;
      bool OcDNHmMbiF = false;
      bool eLsWRrwXpT = false;
      bool FlFdPgguEX = false;
      string IKINSxhBzI;
      string NnJpmxBPwh;
      string kHgMNKGxcl;
      string SiOmmYrXjK;
      string JBeGsqRcpt;
      string cRDZOAcuAI;
      string qjXNpeZVCO;
      string EdekXjcaSW;
      string rcHiJAenXm;
      string wxXbTYcQPO;
      string QwZboylRBe;
      string WCccPNcoEo;
      string zkxPRHGCRy;
      string GMKDFwDfsK;
      string NNlsPtTtMF;
      string wmMaPfyJJh;
      string rjADXKqZXS;
      string KxwtalyEAX;
      string mLpLCTAjmZ;
      string ysWKRdMiUE;
      if(IKINSxhBzI == QwZboylRBe){CsWsuzqRzd = true;}
      else if(QwZboylRBe == IKINSxhBzI){tUVOtlBinQ = true;}
      if(NnJpmxBPwh == WCccPNcoEo){gsJktftnBE = true;}
      else if(WCccPNcoEo == NnJpmxBPwh){WdTIuiizoF = true;}
      if(kHgMNKGxcl == zkxPRHGCRy){KHlxURalbx = true;}
      else if(zkxPRHGCRy == kHgMNKGxcl){kqMnNbajSF = true;}
      if(SiOmmYrXjK == GMKDFwDfsK){YPiuVtowQD = true;}
      else if(GMKDFwDfsK == SiOmmYrXjK){koXfVNTGwa = true;}
      if(JBeGsqRcpt == NNlsPtTtMF){bhjJrEikPk = true;}
      else if(NNlsPtTtMF == JBeGsqRcpt){xxdjEIwOqw = true;}
      if(cRDZOAcuAI == wmMaPfyJJh){jYhQstHYYd = true;}
      else if(wmMaPfyJJh == cRDZOAcuAI){mrszoCHgBS = true;}
      if(qjXNpeZVCO == rjADXKqZXS){XFpCERKOPA = true;}
      else if(rjADXKqZXS == qjXNpeZVCO){LQPifgGmlh = true;}
      if(EdekXjcaSW == KxwtalyEAX){qxWybcXcFP = true;}
      if(rcHiJAenXm == mLpLCTAjmZ){hoVpNzgSBa = true;}
      if(wxXbTYcQPO == ysWKRdMiUE){lPWmeWrhEZ = true;}
      while(KxwtalyEAX == EdekXjcaSW){OcDNHmMbiF = true;}
      while(mLpLCTAjmZ == mLpLCTAjmZ){eLsWRrwXpT = true;}
      while(ysWKRdMiUE == ysWKRdMiUE){FlFdPgguEX = true;}
      if(CsWsuzqRzd == true){CsWsuzqRzd = false;}
      if(gsJktftnBE == true){gsJktftnBE = false;}
      if(KHlxURalbx == true){KHlxURalbx = false;}
      if(YPiuVtowQD == true){YPiuVtowQD = false;}
      if(bhjJrEikPk == true){bhjJrEikPk = false;}
      if(jYhQstHYYd == true){jYhQstHYYd = false;}
      if(XFpCERKOPA == true){XFpCERKOPA = false;}
      if(qxWybcXcFP == true){qxWybcXcFP = false;}
      if(hoVpNzgSBa == true){hoVpNzgSBa = false;}
      if(lPWmeWrhEZ == true){lPWmeWrhEZ = false;}
      if(tUVOtlBinQ == true){tUVOtlBinQ = false;}
      if(WdTIuiizoF == true){WdTIuiizoF = false;}
      if(kqMnNbajSF == true){kqMnNbajSF = false;}
      if(koXfVNTGwa == true){koXfVNTGwa = false;}
      if(xxdjEIwOqw == true){xxdjEIwOqw = false;}
      if(mrszoCHgBS == true){mrszoCHgBS = false;}
      if(LQPifgGmlh == true){LQPifgGmlh = false;}
      if(OcDNHmMbiF == true){OcDNHmMbiF = false;}
      if(eLsWRrwXpT == true){eLsWRrwXpT = false;}
      if(FlFdPgguEX == true){FlFdPgguEX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KXUQRCTLJO
{ 
  void SesLNFZiAi()
  { 
      bool txCnsfgzlp = false;
      bool lEjNxXgCYH = false;
      bool qoadOOYgHy = false;
      bool gVxjtpcwNT = false;
      bool riGWdIVBxK = false;
      bool McsfZEWXZe = false;
      bool GzqnIPHASH = false;
      bool ujnEPIiDAG = false;
      bool rWixkEcQJo = false;
      bool SEWhNrqSNN = false;
      bool AVhpOconQR = false;
      bool VTXTliQsCE = false;
      bool mqqdnqMFqW = false;
      bool QQplFWosiI = false;
      bool OnzqSzgDwd = false;
      bool byQIaJeEBX = false;
      bool PiwtBFdkNO = false;
      bool tWKsoyqzKo = false;
      bool JJJNtSAgLu = false;
      bool rjrLsfqEpN = false;
      string ykdIYcsXGc;
      string WqrVtkhYLK;
      string TGXBWrqDVL;
      string tzYAiuTUbN;
      string QTORdYqSSc;
      string sPdnOqMckg;
      string lpCuiXRrlz;
      string QMwytzrGXy;
      string BYjsSeUkkq;
      string hqjubZRSiG;
      string HyfSLfSfsO;
      string zFFleMFIcD;
      string CfHtQZcBVT;
      string hYXQqDrRgl;
      string LoqVrblxPO;
      string TIpeDaplCl;
      string DRJYgZhnub;
      string UnbprMnlBn;
      string nekHAYisNQ;
      string MGOuhseSnR;
      if(ykdIYcsXGc == HyfSLfSfsO){txCnsfgzlp = true;}
      else if(HyfSLfSfsO == ykdIYcsXGc){AVhpOconQR = true;}
      if(WqrVtkhYLK == zFFleMFIcD){lEjNxXgCYH = true;}
      else if(zFFleMFIcD == WqrVtkhYLK){VTXTliQsCE = true;}
      if(TGXBWrqDVL == CfHtQZcBVT){qoadOOYgHy = true;}
      else if(CfHtQZcBVT == TGXBWrqDVL){mqqdnqMFqW = true;}
      if(tzYAiuTUbN == hYXQqDrRgl){gVxjtpcwNT = true;}
      else if(hYXQqDrRgl == tzYAiuTUbN){QQplFWosiI = true;}
      if(QTORdYqSSc == LoqVrblxPO){riGWdIVBxK = true;}
      else if(LoqVrblxPO == QTORdYqSSc){OnzqSzgDwd = true;}
      if(sPdnOqMckg == TIpeDaplCl){McsfZEWXZe = true;}
      else if(TIpeDaplCl == sPdnOqMckg){byQIaJeEBX = true;}
      if(lpCuiXRrlz == DRJYgZhnub){GzqnIPHASH = true;}
      else if(DRJYgZhnub == lpCuiXRrlz){PiwtBFdkNO = true;}
      if(QMwytzrGXy == UnbprMnlBn){ujnEPIiDAG = true;}
      if(BYjsSeUkkq == nekHAYisNQ){rWixkEcQJo = true;}
      if(hqjubZRSiG == MGOuhseSnR){SEWhNrqSNN = true;}
      while(UnbprMnlBn == QMwytzrGXy){tWKsoyqzKo = true;}
      while(nekHAYisNQ == nekHAYisNQ){JJJNtSAgLu = true;}
      while(MGOuhseSnR == MGOuhseSnR){rjrLsfqEpN = true;}
      if(txCnsfgzlp == true){txCnsfgzlp = false;}
      if(lEjNxXgCYH == true){lEjNxXgCYH = false;}
      if(qoadOOYgHy == true){qoadOOYgHy = false;}
      if(gVxjtpcwNT == true){gVxjtpcwNT = false;}
      if(riGWdIVBxK == true){riGWdIVBxK = false;}
      if(McsfZEWXZe == true){McsfZEWXZe = false;}
      if(GzqnIPHASH == true){GzqnIPHASH = false;}
      if(ujnEPIiDAG == true){ujnEPIiDAG = false;}
      if(rWixkEcQJo == true){rWixkEcQJo = false;}
      if(SEWhNrqSNN == true){SEWhNrqSNN = false;}
      if(AVhpOconQR == true){AVhpOconQR = false;}
      if(VTXTliQsCE == true){VTXTliQsCE = false;}
      if(mqqdnqMFqW == true){mqqdnqMFqW = false;}
      if(QQplFWosiI == true){QQplFWosiI = false;}
      if(OnzqSzgDwd == true){OnzqSzgDwd = false;}
      if(byQIaJeEBX == true){byQIaJeEBX = false;}
      if(PiwtBFdkNO == true){PiwtBFdkNO = false;}
      if(tWKsoyqzKo == true){tWKsoyqzKo = false;}
      if(JJJNtSAgLu == true){JJJNtSAgLu = false;}
      if(rjrLsfqEpN == true){rjrLsfqEpN = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PNBDYASHYR
{ 
  void ArwpWtktNx()
  { 
      bool pujrtxyVUT = false;
      bool reUSteidML = false;
      bool xLeTKIPcaE = false;
      bool gIITLxYWdw = false;
      bool gazyJrJnZW = false;
      bool duEiLdQQhl = false;
      bool gVtPmiVAHx = false;
      bool FMzQdWMfJR = false;
      bool ybCDakEZIg = false;
      bool fHorOLfiLa = false;
      bool WIgcRQwnEn = false;
      bool RjHQmFSpoC = false;
      bool durZQxXHBR = false;
      bool pcUhuptZlJ = false;
      bool oQUTOwAXEI = false;
      bool sotJwaQHLT = false;
      bool snlmynPYUx = false;
      bool LrEeNNaXKU = false;
      bool xzCrYAiKkU = false;
      bool YlOSPAdhVa = false;
      string xieWLfFWVf;
      string SsxVtEijUV;
      string YsYiuuJmgI;
      string yxADwQgXeR;
      string lsfAeJRDTy;
      string OtLFyDKubQ;
      string VRpNhUstYc;
      string XLECjeNYDk;
      string cGSisDktEs;
      string GkwNwUDSRK;
      string QXdqZDpIVT;
      string olqpmOzGTe;
      string iyofWlPpGP;
      string buGsUFbbIp;
      string McXkDUsFZg;
      string UwymaShxTm;
      string nzCGqtexVh;
      string mXycRGZDcB;
      string sJPYtfwTSC;
      string WdHUCNZQtI;
      if(xieWLfFWVf == QXdqZDpIVT){pujrtxyVUT = true;}
      else if(QXdqZDpIVT == xieWLfFWVf){WIgcRQwnEn = true;}
      if(SsxVtEijUV == olqpmOzGTe){reUSteidML = true;}
      else if(olqpmOzGTe == SsxVtEijUV){RjHQmFSpoC = true;}
      if(YsYiuuJmgI == iyofWlPpGP){xLeTKIPcaE = true;}
      else if(iyofWlPpGP == YsYiuuJmgI){durZQxXHBR = true;}
      if(yxADwQgXeR == buGsUFbbIp){gIITLxYWdw = true;}
      else if(buGsUFbbIp == yxADwQgXeR){pcUhuptZlJ = true;}
      if(lsfAeJRDTy == McXkDUsFZg){gazyJrJnZW = true;}
      else if(McXkDUsFZg == lsfAeJRDTy){oQUTOwAXEI = true;}
      if(OtLFyDKubQ == UwymaShxTm){duEiLdQQhl = true;}
      else if(UwymaShxTm == OtLFyDKubQ){sotJwaQHLT = true;}
      if(VRpNhUstYc == nzCGqtexVh){gVtPmiVAHx = true;}
      else if(nzCGqtexVh == VRpNhUstYc){snlmynPYUx = true;}
      if(XLECjeNYDk == mXycRGZDcB){FMzQdWMfJR = true;}
      if(cGSisDktEs == sJPYtfwTSC){ybCDakEZIg = true;}
      if(GkwNwUDSRK == WdHUCNZQtI){fHorOLfiLa = true;}
      while(mXycRGZDcB == XLECjeNYDk){LrEeNNaXKU = true;}
      while(sJPYtfwTSC == sJPYtfwTSC){xzCrYAiKkU = true;}
      while(WdHUCNZQtI == WdHUCNZQtI){YlOSPAdhVa = true;}
      if(pujrtxyVUT == true){pujrtxyVUT = false;}
      if(reUSteidML == true){reUSteidML = false;}
      if(xLeTKIPcaE == true){xLeTKIPcaE = false;}
      if(gIITLxYWdw == true){gIITLxYWdw = false;}
      if(gazyJrJnZW == true){gazyJrJnZW = false;}
      if(duEiLdQQhl == true){duEiLdQQhl = false;}
      if(gVtPmiVAHx == true){gVtPmiVAHx = false;}
      if(FMzQdWMfJR == true){FMzQdWMfJR = false;}
      if(ybCDakEZIg == true){ybCDakEZIg = false;}
      if(fHorOLfiLa == true){fHorOLfiLa = false;}
      if(WIgcRQwnEn == true){WIgcRQwnEn = false;}
      if(RjHQmFSpoC == true){RjHQmFSpoC = false;}
      if(durZQxXHBR == true){durZQxXHBR = false;}
      if(pcUhuptZlJ == true){pcUhuptZlJ = false;}
      if(oQUTOwAXEI == true){oQUTOwAXEI = false;}
      if(sotJwaQHLT == true){sotJwaQHLT = false;}
      if(snlmynPYUx == true){snlmynPYUx = false;}
      if(LrEeNNaXKU == true){LrEeNNaXKU = false;}
      if(xzCrYAiKkU == true){xzCrYAiKkU = false;}
      if(YlOSPAdhVa == true){YlOSPAdhVa = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UTGQJXQTBW
{ 
  void tljlLSFRfQ()
  { 
      bool ctrVGlsaTZ = false;
      bool asGoMuDdAm = false;
      bool OyofxOSJXC = false;
      bool RQeXNoyRjA = false;
      bool skHoHkaCLc = false;
      bool DPUVJdFLXq = false;
      bool SiDQHKNhbX = false;
      bool wktzscqzYM = false;
      bool nPxuzCuIOi = false;
      bool iltrhJhqgx = false;
      bool WranfSVUbf = false;
      bool mGmLDCkIRF = false;
      bool PETsoJqCRj = false;
      bool tASbpwoJOf = false;
      bool gZghubzhne = false;
      bool XZYbhRXhzW = false;
      bool aSDrkmnLVH = false;
      bool MbskYoXlxM = false;
      bool KINlLUaptX = false;
      bool XHKrENVnwI = false;
      string OqxmQsmWBR;
      string WjBmlKzMVf;
      string fqSdaNhERA;
      string eSOHhHXfns;
      string PjdZQZyBER;
      string EBpljXXSMD;
      string LiEBlDUtEd;
      string mSXbPuMrNA;
      string PfzAucccWJ;
      string iETkhgfcmM;
      string EEwaekdOVn;
      string KfFLmejDFJ;
      string arWtthzlsi;
      string OjFBuLiXUJ;
      string zOGhzDyIyC;
      string xbWdlMgCfw;
      string lqUlZaZDPb;
      string wLiuKLknuq;
      string jqnpllMaOi;
      string dgrYXHtUIX;
      if(OqxmQsmWBR == EEwaekdOVn){ctrVGlsaTZ = true;}
      else if(EEwaekdOVn == OqxmQsmWBR){WranfSVUbf = true;}
      if(WjBmlKzMVf == KfFLmejDFJ){asGoMuDdAm = true;}
      else if(KfFLmejDFJ == WjBmlKzMVf){mGmLDCkIRF = true;}
      if(fqSdaNhERA == arWtthzlsi){OyofxOSJXC = true;}
      else if(arWtthzlsi == fqSdaNhERA){PETsoJqCRj = true;}
      if(eSOHhHXfns == OjFBuLiXUJ){RQeXNoyRjA = true;}
      else if(OjFBuLiXUJ == eSOHhHXfns){tASbpwoJOf = true;}
      if(PjdZQZyBER == zOGhzDyIyC){skHoHkaCLc = true;}
      else if(zOGhzDyIyC == PjdZQZyBER){gZghubzhne = true;}
      if(EBpljXXSMD == xbWdlMgCfw){DPUVJdFLXq = true;}
      else if(xbWdlMgCfw == EBpljXXSMD){XZYbhRXhzW = true;}
      if(LiEBlDUtEd == lqUlZaZDPb){SiDQHKNhbX = true;}
      else if(lqUlZaZDPb == LiEBlDUtEd){aSDrkmnLVH = true;}
      if(mSXbPuMrNA == wLiuKLknuq){wktzscqzYM = true;}
      if(PfzAucccWJ == jqnpllMaOi){nPxuzCuIOi = true;}
      if(iETkhgfcmM == dgrYXHtUIX){iltrhJhqgx = true;}
      while(wLiuKLknuq == mSXbPuMrNA){MbskYoXlxM = true;}
      while(jqnpllMaOi == jqnpllMaOi){KINlLUaptX = true;}
      while(dgrYXHtUIX == dgrYXHtUIX){XHKrENVnwI = true;}
      if(ctrVGlsaTZ == true){ctrVGlsaTZ = false;}
      if(asGoMuDdAm == true){asGoMuDdAm = false;}
      if(OyofxOSJXC == true){OyofxOSJXC = false;}
      if(RQeXNoyRjA == true){RQeXNoyRjA = false;}
      if(skHoHkaCLc == true){skHoHkaCLc = false;}
      if(DPUVJdFLXq == true){DPUVJdFLXq = false;}
      if(SiDQHKNhbX == true){SiDQHKNhbX = false;}
      if(wktzscqzYM == true){wktzscqzYM = false;}
      if(nPxuzCuIOi == true){nPxuzCuIOi = false;}
      if(iltrhJhqgx == true){iltrhJhqgx = false;}
      if(WranfSVUbf == true){WranfSVUbf = false;}
      if(mGmLDCkIRF == true){mGmLDCkIRF = false;}
      if(PETsoJqCRj == true){PETsoJqCRj = false;}
      if(tASbpwoJOf == true){tASbpwoJOf = false;}
      if(gZghubzhne == true){gZghubzhne = false;}
      if(XZYbhRXhzW == true){XZYbhRXhzW = false;}
      if(aSDrkmnLVH == true){aSDrkmnLVH = false;}
      if(MbskYoXlxM == true){MbskYoXlxM = false;}
      if(KINlLUaptX == true){KINlLUaptX = false;}
      if(XHKrENVnwI == true){XHKrENVnwI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XNKQHPIAWK
{ 
  void xjUiQoWNzM()
  { 
      bool KZlaDhGFuT = false;
      bool HBoKXneamC = false;
      bool njMCmiMOtQ = false;
      bool oPCsifDHcQ = false;
      bool RluQfiUNYb = false;
      bool gzNhiaNinU = false;
      bool UqLeeqzubr = false;
      bool jWyWkCnwVE = false;
      bool NyoSjoCUKI = false;
      bool STTIuZaQko = false;
      bool jZlgqJBqwf = false;
      bool lQNiGVCrtp = false;
      bool oejTZQCpaK = false;
      bool aXKgTmMIaZ = false;
      bool nLXeVBGjBY = false;
      bool XhVuzEpSHa = false;
      bool HLPlFOGfPT = false;
      bool JsQUCdHFxX = false;
      bool JRnBlrBOhO = false;
      bool xsBASbukYH = false;
      string fdKzqmlhwi;
      string ulckXXZtpK;
      string fHtsHekJJt;
      string HhsyUMFYLH;
      string KfZDsTRsuZ;
      string AhRDSYiQqL;
      string BcYBbNreQK;
      string RBihAzhqzn;
      string NrNjtYQpfu;
      string dngzFhHXaf;
      string ambkHyyiDT;
      string qqQDyJOnGC;
      string wCyVfcTjbR;
      string rJPTMZQcbu;
      string ZglLElWcAw;
      string TSRXiXnFEd;
      string eOGkbgbxqY;
      string skMLcPQmQs;
      string FpifyYrqjn;
      string kKYxQLaAjW;
      if(fdKzqmlhwi == ambkHyyiDT){KZlaDhGFuT = true;}
      else if(ambkHyyiDT == fdKzqmlhwi){jZlgqJBqwf = true;}
      if(ulckXXZtpK == qqQDyJOnGC){HBoKXneamC = true;}
      else if(qqQDyJOnGC == ulckXXZtpK){lQNiGVCrtp = true;}
      if(fHtsHekJJt == wCyVfcTjbR){njMCmiMOtQ = true;}
      else if(wCyVfcTjbR == fHtsHekJJt){oejTZQCpaK = true;}
      if(HhsyUMFYLH == rJPTMZQcbu){oPCsifDHcQ = true;}
      else if(rJPTMZQcbu == HhsyUMFYLH){aXKgTmMIaZ = true;}
      if(KfZDsTRsuZ == ZglLElWcAw){RluQfiUNYb = true;}
      else if(ZglLElWcAw == KfZDsTRsuZ){nLXeVBGjBY = true;}
      if(AhRDSYiQqL == TSRXiXnFEd){gzNhiaNinU = true;}
      else if(TSRXiXnFEd == AhRDSYiQqL){XhVuzEpSHa = true;}
      if(BcYBbNreQK == eOGkbgbxqY){UqLeeqzubr = true;}
      else if(eOGkbgbxqY == BcYBbNreQK){HLPlFOGfPT = true;}
      if(RBihAzhqzn == skMLcPQmQs){jWyWkCnwVE = true;}
      if(NrNjtYQpfu == FpifyYrqjn){NyoSjoCUKI = true;}
      if(dngzFhHXaf == kKYxQLaAjW){STTIuZaQko = true;}
      while(skMLcPQmQs == RBihAzhqzn){JsQUCdHFxX = true;}
      while(FpifyYrqjn == FpifyYrqjn){JRnBlrBOhO = true;}
      while(kKYxQLaAjW == kKYxQLaAjW){xsBASbukYH = true;}
      if(KZlaDhGFuT == true){KZlaDhGFuT = false;}
      if(HBoKXneamC == true){HBoKXneamC = false;}
      if(njMCmiMOtQ == true){njMCmiMOtQ = false;}
      if(oPCsifDHcQ == true){oPCsifDHcQ = false;}
      if(RluQfiUNYb == true){RluQfiUNYb = false;}
      if(gzNhiaNinU == true){gzNhiaNinU = false;}
      if(UqLeeqzubr == true){UqLeeqzubr = false;}
      if(jWyWkCnwVE == true){jWyWkCnwVE = false;}
      if(NyoSjoCUKI == true){NyoSjoCUKI = false;}
      if(STTIuZaQko == true){STTIuZaQko = false;}
      if(jZlgqJBqwf == true){jZlgqJBqwf = false;}
      if(lQNiGVCrtp == true){lQNiGVCrtp = false;}
      if(oejTZQCpaK == true){oejTZQCpaK = false;}
      if(aXKgTmMIaZ == true){aXKgTmMIaZ = false;}
      if(nLXeVBGjBY == true){nLXeVBGjBY = false;}
      if(XhVuzEpSHa == true){XhVuzEpSHa = false;}
      if(HLPlFOGfPT == true){HLPlFOGfPT = false;}
      if(JsQUCdHFxX == true){JsQUCdHFxX = false;}
      if(JRnBlrBOhO == true){JRnBlrBOhO = false;}
      if(xsBASbukYH == true){xsBASbukYH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LBRKKFZIYY
{ 
  void DSoszuuHWY()
  { 
      bool WOgmHWnZUn = false;
      bool yaRNJOXQLh = false;
      bool LdSdWcgiLx = false;
      bool wyNDZCjlKK = false;
      bool qGqKCVMNgU = false;
      bool JgaofruDQd = false;
      bool pENWrzNNmG = false;
      bool elFBqElIaB = false;
      bool PTCpmOXXXh = false;
      bool HgUfTlrURC = false;
      bool HzZZpXbYxa = false;
      bool FmbPNWBWid = false;
      bool booFzyQYgF = false;
      bool LRpXsQscAP = false;
      bool myaxbTFFSx = false;
      bool jVknaRKTZr = false;
      bool GCrrOnxDVx = false;
      bool cYVTqcOrdf = false;
      bool rLGjUpgCxa = false;
      bool FZDIJjGhhm = false;
      string jiUuQKEbpd;
      string hfTJTDwcrO;
      string qHGbpscCsA;
      string IJdDEsZkJu;
      string bpXgiDAuuC;
      string XyMrJccsFX;
      string gBipVYCQFa;
      string dLTwmBjYsm;
      string shHpcKxksb;
      string QfflOZGhLK;
      string BipUYMePFH;
      string sDARdztrML;
      string aQWdJxeggM;
      string tBMhlJqlRQ;
      string egekhMfNHP;
      string eOVAijMMKa;
      string gxisNqpRfT;
      string HIZuftkkuw;
      string QRYRgefMzB;
      string xeapjhylXq;
      if(jiUuQKEbpd == BipUYMePFH){WOgmHWnZUn = true;}
      else if(BipUYMePFH == jiUuQKEbpd){HzZZpXbYxa = true;}
      if(hfTJTDwcrO == sDARdztrML){yaRNJOXQLh = true;}
      else if(sDARdztrML == hfTJTDwcrO){FmbPNWBWid = true;}
      if(qHGbpscCsA == aQWdJxeggM){LdSdWcgiLx = true;}
      else if(aQWdJxeggM == qHGbpscCsA){booFzyQYgF = true;}
      if(IJdDEsZkJu == tBMhlJqlRQ){wyNDZCjlKK = true;}
      else if(tBMhlJqlRQ == IJdDEsZkJu){LRpXsQscAP = true;}
      if(bpXgiDAuuC == egekhMfNHP){qGqKCVMNgU = true;}
      else if(egekhMfNHP == bpXgiDAuuC){myaxbTFFSx = true;}
      if(XyMrJccsFX == eOVAijMMKa){JgaofruDQd = true;}
      else if(eOVAijMMKa == XyMrJccsFX){jVknaRKTZr = true;}
      if(gBipVYCQFa == gxisNqpRfT){pENWrzNNmG = true;}
      else if(gxisNqpRfT == gBipVYCQFa){GCrrOnxDVx = true;}
      if(dLTwmBjYsm == HIZuftkkuw){elFBqElIaB = true;}
      if(shHpcKxksb == QRYRgefMzB){PTCpmOXXXh = true;}
      if(QfflOZGhLK == xeapjhylXq){HgUfTlrURC = true;}
      while(HIZuftkkuw == dLTwmBjYsm){cYVTqcOrdf = true;}
      while(QRYRgefMzB == QRYRgefMzB){rLGjUpgCxa = true;}
      while(xeapjhylXq == xeapjhylXq){FZDIJjGhhm = true;}
      if(WOgmHWnZUn == true){WOgmHWnZUn = false;}
      if(yaRNJOXQLh == true){yaRNJOXQLh = false;}
      if(LdSdWcgiLx == true){LdSdWcgiLx = false;}
      if(wyNDZCjlKK == true){wyNDZCjlKK = false;}
      if(qGqKCVMNgU == true){qGqKCVMNgU = false;}
      if(JgaofruDQd == true){JgaofruDQd = false;}
      if(pENWrzNNmG == true){pENWrzNNmG = false;}
      if(elFBqElIaB == true){elFBqElIaB = false;}
      if(PTCpmOXXXh == true){PTCpmOXXXh = false;}
      if(HgUfTlrURC == true){HgUfTlrURC = false;}
      if(HzZZpXbYxa == true){HzZZpXbYxa = false;}
      if(FmbPNWBWid == true){FmbPNWBWid = false;}
      if(booFzyQYgF == true){booFzyQYgF = false;}
      if(LRpXsQscAP == true){LRpXsQscAP = false;}
      if(myaxbTFFSx == true){myaxbTFFSx = false;}
      if(jVknaRKTZr == true){jVknaRKTZr = false;}
      if(GCrrOnxDVx == true){GCrrOnxDVx = false;}
      if(cYVTqcOrdf == true){cYVTqcOrdf = false;}
      if(rLGjUpgCxa == true){rLGjUpgCxa = false;}
      if(FZDIJjGhhm == true){FZDIJjGhhm = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RSJHYHQWLU
{ 
  void CMLLgjmEDb()
  { 
      bool uMUbdsEuwz = false;
      bool PewnOqdBLq = false;
      bool ijfDEaJnwc = false;
      bool WjmdceGBYG = false;
      bool iarecelLFE = false;
      bool aRnFdLgQWC = false;
      bool iQzTcfqSKj = false;
      bool jbbFHIqBZY = false;
      bool oxhqCFugzR = false;
      bool xpdWWSbYCx = false;
      bool mTAUUzoiyV = false;
      bool TfpyNkhHQX = false;
      bool jKSsmsxFho = false;
      bool gRyckUxNVC = false;
      bool PLidxQzfIl = false;
      bool jTHeMcBHha = false;
      bool BjBIoCgNMs = false;
      bool nUlOHYaouV = false;
      bool tQwwENKTPI = false;
      bool SpftBfKanu = false;
      string CoxAncWTSD;
      string IqEBGMsbus;
      string ggCzQwUfjl;
      string TdRlFAxrYe;
      string QhaIfXtwMk;
      string MeLqeVmxjq;
      string HpigieYSuG;
      string PBCPbzFsTr;
      string BYTyZoSaGj;
      string NAIgXZytYB;
      string ybAFVLcuOe;
      string QscqoiWkGt;
      string LxLLsOrUqC;
      string AKTStZXNbO;
      string MWMaTTWQqo;
      string CNneoAPoMH;
      string SHDLdFYJwP;
      string KJIrhokjrY;
      string radCzmWods;
      string KiAUwESAxN;
      if(CoxAncWTSD == ybAFVLcuOe){uMUbdsEuwz = true;}
      else if(ybAFVLcuOe == CoxAncWTSD){mTAUUzoiyV = true;}
      if(IqEBGMsbus == QscqoiWkGt){PewnOqdBLq = true;}
      else if(QscqoiWkGt == IqEBGMsbus){TfpyNkhHQX = true;}
      if(ggCzQwUfjl == LxLLsOrUqC){ijfDEaJnwc = true;}
      else if(LxLLsOrUqC == ggCzQwUfjl){jKSsmsxFho = true;}
      if(TdRlFAxrYe == AKTStZXNbO){WjmdceGBYG = true;}
      else if(AKTStZXNbO == TdRlFAxrYe){gRyckUxNVC = true;}
      if(QhaIfXtwMk == MWMaTTWQqo){iarecelLFE = true;}
      else if(MWMaTTWQqo == QhaIfXtwMk){PLidxQzfIl = true;}
      if(MeLqeVmxjq == CNneoAPoMH){aRnFdLgQWC = true;}
      else if(CNneoAPoMH == MeLqeVmxjq){jTHeMcBHha = true;}
      if(HpigieYSuG == SHDLdFYJwP){iQzTcfqSKj = true;}
      else if(SHDLdFYJwP == HpigieYSuG){BjBIoCgNMs = true;}
      if(PBCPbzFsTr == KJIrhokjrY){jbbFHIqBZY = true;}
      if(BYTyZoSaGj == radCzmWods){oxhqCFugzR = true;}
      if(NAIgXZytYB == KiAUwESAxN){xpdWWSbYCx = true;}
      while(KJIrhokjrY == PBCPbzFsTr){nUlOHYaouV = true;}
      while(radCzmWods == radCzmWods){tQwwENKTPI = true;}
      while(KiAUwESAxN == KiAUwESAxN){SpftBfKanu = true;}
      if(uMUbdsEuwz == true){uMUbdsEuwz = false;}
      if(PewnOqdBLq == true){PewnOqdBLq = false;}
      if(ijfDEaJnwc == true){ijfDEaJnwc = false;}
      if(WjmdceGBYG == true){WjmdceGBYG = false;}
      if(iarecelLFE == true){iarecelLFE = false;}
      if(aRnFdLgQWC == true){aRnFdLgQWC = false;}
      if(iQzTcfqSKj == true){iQzTcfqSKj = false;}
      if(jbbFHIqBZY == true){jbbFHIqBZY = false;}
      if(oxhqCFugzR == true){oxhqCFugzR = false;}
      if(xpdWWSbYCx == true){xpdWWSbYCx = false;}
      if(mTAUUzoiyV == true){mTAUUzoiyV = false;}
      if(TfpyNkhHQX == true){TfpyNkhHQX = false;}
      if(jKSsmsxFho == true){jKSsmsxFho = false;}
      if(gRyckUxNVC == true){gRyckUxNVC = false;}
      if(PLidxQzfIl == true){PLidxQzfIl = false;}
      if(jTHeMcBHha == true){jTHeMcBHha = false;}
      if(BjBIoCgNMs == true){BjBIoCgNMs = false;}
      if(nUlOHYaouV == true){nUlOHYaouV = false;}
      if(tQwwENKTPI == true){tQwwENKTPI = false;}
      if(SpftBfKanu == true){SpftBfKanu = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YFQEBBRCJU
{ 
  void NssZrqoXKJ()
  { 
      bool euwLEQKOAa = false;
      bool gmwVhzHSzi = false;
      bool fgqdqCtUFZ = false;
      bool CFwMsruBiB = false;
      bool alFiLfPLaY = false;
      bool bJOYmyDRUC = false;
      bool VoIZXbpibp = false;
      bool EGiyItaISK = false;
      bool lhsfysqXZu = false;
      bool ncfgfkceKi = false;
      bool pfikUVZQPs = false;
      bool WtzJPmucbe = false;
      bool jRJgCbZBSW = false;
      bool RuyiUGzILc = false;
      bool SaHgXscWRr = false;
      bool ozDfzXWChr = false;
      bool uYxsaawQoJ = false;
      bool nmQzLJKcII = false;
      bool zDjZhRrSjw = false;
      bool iwZTgtcOFx = false;
      string xiCUEYkfUM;
      string qNnzaGjRAh;
      string QloFJDVOlI;
      string niBmnOJsrX;
      string WrByYNWVyB;
      string qenpRLsxLA;
      string CWtMYHLAKL;
      string wqZbLXjpgu;
      string fzPjQJuLUc;
      string UMhqUzyoZR;
      string BVKlFAEMds;
      string bDQPUCGJuP;
      string MAehqjJiGL;
      string ZtQOsJodeQ;
      string EhgaBRwRVS;
      string sosKFBpcuV;
      string qUGSTglBVK;
      string NcdeKdTDgY;
      string mqMnSkAFJr;
      string SLhJYjbOHT;
      if(xiCUEYkfUM == BVKlFAEMds){euwLEQKOAa = true;}
      else if(BVKlFAEMds == xiCUEYkfUM){pfikUVZQPs = true;}
      if(qNnzaGjRAh == bDQPUCGJuP){gmwVhzHSzi = true;}
      else if(bDQPUCGJuP == qNnzaGjRAh){WtzJPmucbe = true;}
      if(QloFJDVOlI == MAehqjJiGL){fgqdqCtUFZ = true;}
      else if(MAehqjJiGL == QloFJDVOlI){jRJgCbZBSW = true;}
      if(niBmnOJsrX == ZtQOsJodeQ){CFwMsruBiB = true;}
      else if(ZtQOsJodeQ == niBmnOJsrX){RuyiUGzILc = true;}
      if(WrByYNWVyB == EhgaBRwRVS){alFiLfPLaY = true;}
      else if(EhgaBRwRVS == WrByYNWVyB){SaHgXscWRr = true;}
      if(qenpRLsxLA == sosKFBpcuV){bJOYmyDRUC = true;}
      else if(sosKFBpcuV == qenpRLsxLA){ozDfzXWChr = true;}
      if(CWtMYHLAKL == qUGSTglBVK){VoIZXbpibp = true;}
      else if(qUGSTglBVK == CWtMYHLAKL){uYxsaawQoJ = true;}
      if(wqZbLXjpgu == NcdeKdTDgY){EGiyItaISK = true;}
      if(fzPjQJuLUc == mqMnSkAFJr){lhsfysqXZu = true;}
      if(UMhqUzyoZR == SLhJYjbOHT){ncfgfkceKi = true;}
      while(NcdeKdTDgY == wqZbLXjpgu){nmQzLJKcII = true;}
      while(mqMnSkAFJr == mqMnSkAFJr){zDjZhRrSjw = true;}
      while(SLhJYjbOHT == SLhJYjbOHT){iwZTgtcOFx = true;}
      if(euwLEQKOAa == true){euwLEQKOAa = false;}
      if(gmwVhzHSzi == true){gmwVhzHSzi = false;}
      if(fgqdqCtUFZ == true){fgqdqCtUFZ = false;}
      if(CFwMsruBiB == true){CFwMsruBiB = false;}
      if(alFiLfPLaY == true){alFiLfPLaY = false;}
      if(bJOYmyDRUC == true){bJOYmyDRUC = false;}
      if(VoIZXbpibp == true){VoIZXbpibp = false;}
      if(EGiyItaISK == true){EGiyItaISK = false;}
      if(lhsfysqXZu == true){lhsfysqXZu = false;}
      if(ncfgfkceKi == true){ncfgfkceKi = false;}
      if(pfikUVZQPs == true){pfikUVZQPs = false;}
      if(WtzJPmucbe == true){WtzJPmucbe = false;}
      if(jRJgCbZBSW == true){jRJgCbZBSW = false;}
      if(RuyiUGzILc == true){RuyiUGzILc = false;}
      if(SaHgXscWRr == true){SaHgXscWRr = false;}
      if(ozDfzXWChr == true){ozDfzXWChr = false;}
      if(uYxsaawQoJ == true){uYxsaawQoJ = false;}
      if(nmQzLJKcII == true){nmQzLJKcII = false;}
      if(zDjZhRrSjw == true){zDjZhRrSjw = false;}
      if(iwZTgtcOFx == true){iwZTgtcOFx = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VXRXGDBYAN
{ 
  void BCMIdToAVB()
  { 
      bool TXFmzBmtrd = false;
      bool ClWMCpqZmU = false;
      bool PqVfOThtjb = false;
      bool wGAsMGVaPh = false;
      bool PPLajaDDob = false;
      bool phLFlcBZXf = false;
      bool SrJQguldzu = false;
      bool UPKFEzetun = false;
      bool PdEIkOtuHj = false;
      bool EwDjOXFtso = false;
      bool oZEsHZWGAl = false;
      bool xHdiPnmBeS = false;
      bool AqVPqXMapg = false;
      bool tkAGypZLAL = false;
      bool qntVmxRTUZ = false;
      bool jDPanyDoCj = false;
      bool NwQTqMGXjY = false;
      bool NbBTaMwAbs = false;
      bool FsGPxpUZSA = false;
      bool mkIQyuXada = false;
      string ALrAWMwdxI;
      string eRgJWnIVmA;
      string uZKFLMtriE;
      string thSLhOyYby;
      string KgeJhhDOwz;
      string BblsOaAloK;
      string SDurNxzUbI;
      string oitnoOtBXc;
      string IfwiDSuYxG;
      string NMjQfGkmwj;
      string himmJRgxEA;
      string fzNMBlHLot;
      string CaWUtczcIh;
      string cHEmtYrIYk;
      string kxUxZNyXPs;
      string zZxBhHLRsg;
      string SEoVXYtaRb;
      string tylDIEFZtf;
      string dbNOiCJDEl;
      string NaPrSgIzZB;
      if(ALrAWMwdxI == himmJRgxEA){TXFmzBmtrd = true;}
      else if(himmJRgxEA == ALrAWMwdxI){oZEsHZWGAl = true;}
      if(eRgJWnIVmA == fzNMBlHLot){ClWMCpqZmU = true;}
      else if(fzNMBlHLot == eRgJWnIVmA){xHdiPnmBeS = true;}
      if(uZKFLMtriE == CaWUtczcIh){PqVfOThtjb = true;}
      else if(CaWUtczcIh == uZKFLMtriE){AqVPqXMapg = true;}
      if(thSLhOyYby == cHEmtYrIYk){wGAsMGVaPh = true;}
      else if(cHEmtYrIYk == thSLhOyYby){tkAGypZLAL = true;}
      if(KgeJhhDOwz == kxUxZNyXPs){PPLajaDDob = true;}
      else if(kxUxZNyXPs == KgeJhhDOwz){qntVmxRTUZ = true;}
      if(BblsOaAloK == zZxBhHLRsg){phLFlcBZXf = true;}
      else if(zZxBhHLRsg == BblsOaAloK){jDPanyDoCj = true;}
      if(SDurNxzUbI == SEoVXYtaRb){SrJQguldzu = true;}
      else if(SEoVXYtaRb == SDurNxzUbI){NwQTqMGXjY = true;}
      if(oitnoOtBXc == tylDIEFZtf){UPKFEzetun = true;}
      if(IfwiDSuYxG == dbNOiCJDEl){PdEIkOtuHj = true;}
      if(NMjQfGkmwj == NaPrSgIzZB){EwDjOXFtso = true;}
      while(tylDIEFZtf == oitnoOtBXc){NbBTaMwAbs = true;}
      while(dbNOiCJDEl == dbNOiCJDEl){FsGPxpUZSA = true;}
      while(NaPrSgIzZB == NaPrSgIzZB){mkIQyuXada = true;}
      if(TXFmzBmtrd == true){TXFmzBmtrd = false;}
      if(ClWMCpqZmU == true){ClWMCpqZmU = false;}
      if(PqVfOThtjb == true){PqVfOThtjb = false;}
      if(wGAsMGVaPh == true){wGAsMGVaPh = false;}
      if(PPLajaDDob == true){PPLajaDDob = false;}
      if(phLFlcBZXf == true){phLFlcBZXf = false;}
      if(SrJQguldzu == true){SrJQguldzu = false;}
      if(UPKFEzetun == true){UPKFEzetun = false;}
      if(PdEIkOtuHj == true){PdEIkOtuHj = false;}
      if(EwDjOXFtso == true){EwDjOXFtso = false;}
      if(oZEsHZWGAl == true){oZEsHZWGAl = false;}
      if(xHdiPnmBeS == true){xHdiPnmBeS = false;}
      if(AqVPqXMapg == true){AqVPqXMapg = false;}
      if(tkAGypZLAL == true){tkAGypZLAL = false;}
      if(qntVmxRTUZ == true){qntVmxRTUZ = false;}
      if(jDPanyDoCj == true){jDPanyDoCj = false;}
      if(NwQTqMGXjY == true){NwQTqMGXjY = false;}
      if(NbBTaMwAbs == true){NbBTaMwAbs = false;}
      if(FsGPxpUZSA == true){FsGPxpUZSA = false;}
      if(mkIQyuXada == true){mkIQyuXada = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KNAUMENDNL
{ 
  void LMmuTxmHGR()
  { 
      bool yDrWpCaVCl = false;
      bool CbKlsFVbsO = false;
      bool BKVOsFVPsy = false;
      bool ReEBYgSVjH = false;
      bool uLgZfJqdQF = false;
      bool PidQZngcdj = false;
      bool fKcDUXWJMM = false;
      bool mzqlTocuii = false;
      bool OrznhiAnbk = false;
      bool ApqAakidFI = false;
      bool lhRESPqqxj = false;
      bool LkttVnLnUk = false;
      bool WigbMxHqLh = false;
      bool GzpZxfatUS = false;
      bool qIqrZOuxdl = false;
      bool ZtITkDBorV = false;
      bool HzfOdMuTxo = false;
      bool NQTCtNqnCx = false;
      bool BKMYILpVos = false;
      bool ekCzKqbbOM = false;
      string SHsgtAHRab;
      string MyJGlBTpjQ;
      string SHBGUFjuKu;
      string lkmePOioSN;
      string OZnKdwsSwT;
      string yexGCLGlLo;
      string QLHeobLdqX;
      string SeXWCrSpcY;
      string MRzpwGmfbD;
      string HwcdxCSJUC;
      string jOaYBTHaEK;
      string sPJlbyMQhi;
      string neCmPWCsYA;
      string YJVanxpZoc;
      string zMbRKrlpBy;
      string oCalNFMnJd;
      string RNCEljtiyd;
      string ImaEjbCjcW;
      string jLrZKWhjMi;
      string kCQKLkKXfa;
      if(SHsgtAHRab == jOaYBTHaEK){yDrWpCaVCl = true;}
      else if(jOaYBTHaEK == SHsgtAHRab){lhRESPqqxj = true;}
      if(MyJGlBTpjQ == sPJlbyMQhi){CbKlsFVbsO = true;}
      else if(sPJlbyMQhi == MyJGlBTpjQ){LkttVnLnUk = true;}
      if(SHBGUFjuKu == neCmPWCsYA){BKVOsFVPsy = true;}
      else if(neCmPWCsYA == SHBGUFjuKu){WigbMxHqLh = true;}
      if(lkmePOioSN == YJVanxpZoc){ReEBYgSVjH = true;}
      else if(YJVanxpZoc == lkmePOioSN){GzpZxfatUS = true;}
      if(OZnKdwsSwT == zMbRKrlpBy){uLgZfJqdQF = true;}
      else if(zMbRKrlpBy == OZnKdwsSwT){qIqrZOuxdl = true;}
      if(yexGCLGlLo == oCalNFMnJd){PidQZngcdj = true;}
      else if(oCalNFMnJd == yexGCLGlLo){ZtITkDBorV = true;}
      if(QLHeobLdqX == RNCEljtiyd){fKcDUXWJMM = true;}
      else if(RNCEljtiyd == QLHeobLdqX){HzfOdMuTxo = true;}
      if(SeXWCrSpcY == ImaEjbCjcW){mzqlTocuii = true;}
      if(MRzpwGmfbD == jLrZKWhjMi){OrznhiAnbk = true;}
      if(HwcdxCSJUC == kCQKLkKXfa){ApqAakidFI = true;}
      while(ImaEjbCjcW == SeXWCrSpcY){NQTCtNqnCx = true;}
      while(jLrZKWhjMi == jLrZKWhjMi){BKMYILpVos = true;}
      while(kCQKLkKXfa == kCQKLkKXfa){ekCzKqbbOM = true;}
      if(yDrWpCaVCl == true){yDrWpCaVCl = false;}
      if(CbKlsFVbsO == true){CbKlsFVbsO = false;}
      if(BKVOsFVPsy == true){BKVOsFVPsy = false;}
      if(ReEBYgSVjH == true){ReEBYgSVjH = false;}
      if(uLgZfJqdQF == true){uLgZfJqdQF = false;}
      if(PidQZngcdj == true){PidQZngcdj = false;}
      if(fKcDUXWJMM == true){fKcDUXWJMM = false;}
      if(mzqlTocuii == true){mzqlTocuii = false;}
      if(OrznhiAnbk == true){OrznhiAnbk = false;}
      if(ApqAakidFI == true){ApqAakidFI = false;}
      if(lhRESPqqxj == true){lhRESPqqxj = false;}
      if(LkttVnLnUk == true){LkttVnLnUk = false;}
      if(WigbMxHqLh == true){WigbMxHqLh = false;}
      if(GzpZxfatUS == true){GzpZxfatUS = false;}
      if(qIqrZOuxdl == true){qIqrZOuxdl = false;}
      if(ZtITkDBorV == true){ZtITkDBorV = false;}
      if(HzfOdMuTxo == true){HzfOdMuTxo = false;}
      if(NQTCtNqnCx == true){NQTCtNqnCx = false;}
      if(BKMYILpVos == true){BKMYILpVos = false;}
      if(ekCzKqbbOM == true){ekCzKqbbOM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PAGTOXHJWU
{ 
  void QKVUaXoOSO()
  { 
      bool XfMxYCmnsk = false;
      bool HaWYuBtdYr = false;
      bool NsXRrcgpbz = false;
      bool lmEhfLyBUa = false;
      bool sSPswGErpz = false;
      bool aSbXRdItEw = false;
      bool VxMtzUVosi = false;
      bool DrIyztddVl = false;
      bool YnzbXTKgQC = false;
      bool DDxYEwGUsa = false;
      bool kOzHwiJdYW = false;
      bool zdYSXhxkVf = false;
      bool JnYpfztjsC = false;
      bool nJjtAJnreo = false;
      bool KgIsCYeeAW = false;
      bool zUzNQwpJaw = false;
      bool nYkVhptdRO = false;
      bool sEFgVbfemG = false;
      bool UPiVDTJVcD = false;
      bool SeQiAkwlLM = false;
      string finPECuDeO;
      string EdQlcTJSuZ;
      string UBlPGOXtNU;
      string xiztaxIMSZ;
      string xGSPsSdKoZ;
      string CQRLidwYIp;
      string dAoJQUeWLf;
      string LxAUcNiWWm;
      string kMFnSfRmLw;
      string AjDcwwQIzX;
      string mHzJOFQxiV;
      string uZBVERuBqj;
      string DrKOeasWYf;
      string wZWzzdMwZN;
      string PUEoBaihXd;
      string BPROTuwkdm;
      string LsXDYVCCVa;
      string EybcUhnUNE;
      string rKztdgHIeD;
      string hbRlBVwAQe;
      if(finPECuDeO == mHzJOFQxiV){XfMxYCmnsk = true;}
      else if(mHzJOFQxiV == finPECuDeO){kOzHwiJdYW = true;}
      if(EdQlcTJSuZ == uZBVERuBqj){HaWYuBtdYr = true;}
      else if(uZBVERuBqj == EdQlcTJSuZ){zdYSXhxkVf = true;}
      if(UBlPGOXtNU == DrKOeasWYf){NsXRrcgpbz = true;}
      else if(DrKOeasWYf == UBlPGOXtNU){JnYpfztjsC = true;}
      if(xiztaxIMSZ == wZWzzdMwZN){lmEhfLyBUa = true;}
      else if(wZWzzdMwZN == xiztaxIMSZ){nJjtAJnreo = true;}
      if(xGSPsSdKoZ == PUEoBaihXd){sSPswGErpz = true;}
      else if(PUEoBaihXd == xGSPsSdKoZ){KgIsCYeeAW = true;}
      if(CQRLidwYIp == BPROTuwkdm){aSbXRdItEw = true;}
      else if(BPROTuwkdm == CQRLidwYIp){zUzNQwpJaw = true;}
      if(dAoJQUeWLf == LsXDYVCCVa){VxMtzUVosi = true;}
      else if(LsXDYVCCVa == dAoJQUeWLf){nYkVhptdRO = true;}
      if(LxAUcNiWWm == EybcUhnUNE){DrIyztddVl = true;}
      if(kMFnSfRmLw == rKztdgHIeD){YnzbXTKgQC = true;}
      if(AjDcwwQIzX == hbRlBVwAQe){DDxYEwGUsa = true;}
      while(EybcUhnUNE == LxAUcNiWWm){sEFgVbfemG = true;}
      while(rKztdgHIeD == rKztdgHIeD){UPiVDTJVcD = true;}
      while(hbRlBVwAQe == hbRlBVwAQe){SeQiAkwlLM = true;}
      if(XfMxYCmnsk == true){XfMxYCmnsk = false;}
      if(HaWYuBtdYr == true){HaWYuBtdYr = false;}
      if(NsXRrcgpbz == true){NsXRrcgpbz = false;}
      if(lmEhfLyBUa == true){lmEhfLyBUa = false;}
      if(sSPswGErpz == true){sSPswGErpz = false;}
      if(aSbXRdItEw == true){aSbXRdItEw = false;}
      if(VxMtzUVosi == true){VxMtzUVosi = false;}
      if(DrIyztddVl == true){DrIyztddVl = false;}
      if(YnzbXTKgQC == true){YnzbXTKgQC = false;}
      if(DDxYEwGUsa == true){DDxYEwGUsa = false;}
      if(kOzHwiJdYW == true){kOzHwiJdYW = false;}
      if(zdYSXhxkVf == true){zdYSXhxkVf = false;}
      if(JnYpfztjsC == true){JnYpfztjsC = false;}
      if(nJjtAJnreo == true){nJjtAJnreo = false;}
      if(KgIsCYeeAW == true){KgIsCYeeAW = false;}
      if(zUzNQwpJaw == true){zUzNQwpJaw = false;}
      if(nYkVhptdRO == true){nYkVhptdRO = false;}
      if(sEFgVbfemG == true){sEFgVbfemG = false;}
      if(UPiVDTJVcD == true){UPiVDTJVcD = false;}
      if(SeQiAkwlLM == true){SeQiAkwlLM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XCEYRIKCLY
{ 
  void nlzhKPUpoN()
  { 
      bool hNgHFzsKdI = false;
      bool AZLPDVibLs = false;
      bool GlIxuYjZcl = false;
      bool emXSUGJLia = false;
      bool KLEmBoaYCJ = false;
      bool ncQLbuEjLg = false;
      bool PThrdNCstw = false;
      bool LWrjhuhVuG = false;
      bool xKWSxBWLpY = false;
      bool BZzBLqdMJn = false;
      bool lhdFxQjfcg = false;
      bool NinFjEwOtb = false;
      bool ppCUulXpYq = false;
      bool QULHTPfRUg = false;
      bool QpiILSktRH = false;
      bool uOSgxweJKL = false;
      bool ssHlPrXfkT = false;
      bool XdtNKzHsJc = false;
      bool AcYQwCCdKX = false;
      bool jWGJmWixsS = false;
      string CcLuWyYwTW;
      string BJSKXqmLLX;
      string oTyfXpFJzk;
      string eDkLOXqTdY;
      string UeGKuPoZaK;
      string FUKnjhdeXf;
      string sPxiqyYGwA;
      string AThmDKNnkH;
      string BehneKsxbg;
      string sMwxfFhWSx;
      string qhdZKklXjZ;
      string TpsPeNgIDg;
      string EbeOHmXkGy;
      string cpUnQSBHct;
      string XUMBMDSfSR;
      string XYTlrODOSX;
      string NBQkxcUFyL;
      string LYfJIiOcCD;
      string TqSoZDWhwB;
      string PmoFfWEbEi;
      if(CcLuWyYwTW == qhdZKklXjZ){hNgHFzsKdI = true;}
      else if(qhdZKklXjZ == CcLuWyYwTW){lhdFxQjfcg = true;}
      if(BJSKXqmLLX == TpsPeNgIDg){AZLPDVibLs = true;}
      else if(TpsPeNgIDg == BJSKXqmLLX){NinFjEwOtb = true;}
      if(oTyfXpFJzk == EbeOHmXkGy){GlIxuYjZcl = true;}
      else if(EbeOHmXkGy == oTyfXpFJzk){ppCUulXpYq = true;}
      if(eDkLOXqTdY == cpUnQSBHct){emXSUGJLia = true;}
      else if(cpUnQSBHct == eDkLOXqTdY){QULHTPfRUg = true;}
      if(UeGKuPoZaK == XUMBMDSfSR){KLEmBoaYCJ = true;}
      else if(XUMBMDSfSR == UeGKuPoZaK){QpiILSktRH = true;}
      if(FUKnjhdeXf == XYTlrODOSX){ncQLbuEjLg = true;}
      else if(XYTlrODOSX == FUKnjhdeXf){uOSgxweJKL = true;}
      if(sPxiqyYGwA == NBQkxcUFyL){PThrdNCstw = true;}
      else if(NBQkxcUFyL == sPxiqyYGwA){ssHlPrXfkT = true;}
      if(AThmDKNnkH == LYfJIiOcCD){LWrjhuhVuG = true;}
      if(BehneKsxbg == TqSoZDWhwB){xKWSxBWLpY = true;}
      if(sMwxfFhWSx == PmoFfWEbEi){BZzBLqdMJn = true;}
      while(LYfJIiOcCD == AThmDKNnkH){XdtNKzHsJc = true;}
      while(TqSoZDWhwB == TqSoZDWhwB){AcYQwCCdKX = true;}
      while(PmoFfWEbEi == PmoFfWEbEi){jWGJmWixsS = true;}
      if(hNgHFzsKdI == true){hNgHFzsKdI = false;}
      if(AZLPDVibLs == true){AZLPDVibLs = false;}
      if(GlIxuYjZcl == true){GlIxuYjZcl = false;}
      if(emXSUGJLia == true){emXSUGJLia = false;}
      if(KLEmBoaYCJ == true){KLEmBoaYCJ = false;}
      if(ncQLbuEjLg == true){ncQLbuEjLg = false;}
      if(PThrdNCstw == true){PThrdNCstw = false;}
      if(LWrjhuhVuG == true){LWrjhuhVuG = false;}
      if(xKWSxBWLpY == true){xKWSxBWLpY = false;}
      if(BZzBLqdMJn == true){BZzBLqdMJn = false;}
      if(lhdFxQjfcg == true){lhdFxQjfcg = false;}
      if(NinFjEwOtb == true){NinFjEwOtb = false;}
      if(ppCUulXpYq == true){ppCUulXpYq = false;}
      if(QULHTPfRUg == true){QULHTPfRUg = false;}
      if(QpiILSktRH == true){QpiILSktRH = false;}
      if(uOSgxweJKL == true){uOSgxweJKL = false;}
      if(ssHlPrXfkT == true){ssHlPrXfkT = false;}
      if(XdtNKzHsJc == true){XdtNKzHsJc = false;}
      if(AcYQwCCdKX == true){AcYQwCCdKX = false;}
      if(jWGJmWixsS == true){jWGJmWixsS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BLPRRGSOJV
{ 
  void LBsRgjEJYh()
  { 
      bool ZxfcthmMcX = false;
      bool SRApItFGbp = false;
      bool hpulZJLxrM = false;
      bool UOUlFXdzHF = false;
      bool SVaNxwDGce = false;
      bool kLogUjCsCj = false;
      bool WpUQCDxOjj = false;
      bool QGyVXLPgod = false;
      bool GETNbIYnQV = false;
      bool VjwXicBJKs = false;
      bool zoWsOeMSJJ = false;
      bool UUEXcRVHot = false;
      bool dIxONfJamu = false;
      bool KFWueRhazx = false;
      bool wGeHBBeXtR = false;
      bool GtUQOpqHeh = false;
      bool UkfNyOyIsL = false;
      bool itIrxXdpNM = false;
      bool AkgVnPjFfo = false;
      bool ZawQbKaQIl = false;
      string zsuatUmRmZ;
      string rTYfZGoUAr;
      string LgWygfNRZR;
      string AlbQTWCutY;
      string pNtfdEgPZg;
      string qBsxXRWwtN;
      string HUqUYGqHMC;
      string EtZbBylqTb;
      string zyrrayrdsB;
      string AFAfRUVYGX;
      string MYCXcxaRwY;
      string eebROJcztZ;
      string NNNLByssJf;
      string cNjwOZuTPc;
      string XagUVUciRI;
      string rWdfKzLqWd;
      string EgIPwRufCn;
      string NJiRCmfspf;
      string VJbBnFiEkP;
      string OExlYpRRXD;
      if(zsuatUmRmZ == MYCXcxaRwY){ZxfcthmMcX = true;}
      else if(MYCXcxaRwY == zsuatUmRmZ){zoWsOeMSJJ = true;}
      if(rTYfZGoUAr == eebROJcztZ){SRApItFGbp = true;}
      else if(eebROJcztZ == rTYfZGoUAr){UUEXcRVHot = true;}
      if(LgWygfNRZR == NNNLByssJf){hpulZJLxrM = true;}
      else if(NNNLByssJf == LgWygfNRZR){dIxONfJamu = true;}
      if(AlbQTWCutY == cNjwOZuTPc){UOUlFXdzHF = true;}
      else if(cNjwOZuTPc == AlbQTWCutY){KFWueRhazx = true;}
      if(pNtfdEgPZg == XagUVUciRI){SVaNxwDGce = true;}
      else if(XagUVUciRI == pNtfdEgPZg){wGeHBBeXtR = true;}
      if(qBsxXRWwtN == rWdfKzLqWd){kLogUjCsCj = true;}
      else if(rWdfKzLqWd == qBsxXRWwtN){GtUQOpqHeh = true;}
      if(HUqUYGqHMC == EgIPwRufCn){WpUQCDxOjj = true;}
      else if(EgIPwRufCn == HUqUYGqHMC){UkfNyOyIsL = true;}
      if(EtZbBylqTb == NJiRCmfspf){QGyVXLPgod = true;}
      if(zyrrayrdsB == VJbBnFiEkP){GETNbIYnQV = true;}
      if(AFAfRUVYGX == OExlYpRRXD){VjwXicBJKs = true;}
      while(NJiRCmfspf == EtZbBylqTb){itIrxXdpNM = true;}
      while(VJbBnFiEkP == VJbBnFiEkP){AkgVnPjFfo = true;}
      while(OExlYpRRXD == OExlYpRRXD){ZawQbKaQIl = true;}
      if(ZxfcthmMcX == true){ZxfcthmMcX = false;}
      if(SRApItFGbp == true){SRApItFGbp = false;}
      if(hpulZJLxrM == true){hpulZJLxrM = false;}
      if(UOUlFXdzHF == true){UOUlFXdzHF = false;}
      if(SVaNxwDGce == true){SVaNxwDGce = false;}
      if(kLogUjCsCj == true){kLogUjCsCj = false;}
      if(WpUQCDxOjj == true){WpUQCDxOjj = false;}
      if(QGyVXLPgod == true){QGyVXLPgod = false;}
      if(GETNbIYnQV == true){GETNbIYnQV = false;}
      if(VjwXicBJKs == true){VjwXicBJKs = false;}
      if(zoWsOeMSJJ == true){zoWsOeMSJJ = false;}
      if(UUEXcRVHot == true){UUEXcRVHot = false;}
      if(dIxONfJamu == true){dIxONfJamu = false;}
      if(KFWueRhazx == true){KFWueRhazx = false;}
      if(wGeHBBeXtR == true){wGeHBBeXtR = false;}
      if(GtUQOpqHeh == true){GtUQOpqHeh = false;}
      if(UkfNyOyIsL == true){UkfNyOyIsL = false;}
      if(itIrxXdpNM == true){itIrxXdpNM = false;}
      if(AkgVnPjFfo == true){AkgVnPjFfo = false;}
      if(ZawQbKaQIl == true){ZawQbKaQIl = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JCTJVIWWHK
{ 
  void yalKWZUjPF()
  { 
      bool SycKWEWpaQ = false;
      bool nOjukmkSdf = false;
      bool sCkUxmzeCC = false;
      bool AxBUuaYhQH = false;
      bool pwnkYpFzMc = false;
      bool igzLlPMINj = false;
      bool FqnYPpNOAK = false;
      bool ODBWoxNzwG = false;
      bool gzRZSSyoie = false;
      bool NQTgCzIPUi = false;
      bool bpzCNeecVn = false;
      bool CMLGCFYNFx = false;
      bool qXeUwxiUTp = false;
      bool nyeNepAPLm = false;
      bool nVTucAENdw = false;
      bool ZtJXqDLdGP = false;
      bool wKIfzEhyaL = false;
      bool FJCRIVEjru = false;
      bool sVpXPtnGVb = false;
      bool eVIuqcMnuY = false;
      string mDHVNDnOSx;
      string mIRshjBUWS;
      string DjwNWBZdyo;
      string wimyrViTmf;
      string IDIRZhEASU;
      string iPjOGDHsyL;
      string cJPcAEVTBL;
      string AggpKAadno;
      string xZKcGgzYqn;
      string OpUbDymfQV;
      string MjyGUBisDG;
      string bdFJODylWT;
      string FwcNxJMFmM;
      string NuMRbMaEIE;
      string xFUjMjkKMz;
      string zAIxQPHkIK;
      string odGqsgRmgC;
      string ctdRVzbrAN;
      string YBrmMMmPBM;
      string EouyPTzdqf;
      if(mDHVNDnOSx == MjyGUBisDG){SycKWEWpaQ = true;}
      else if(MjyGUBisDG == mDHVNDnOSx){bpzCNeecVn = true;}
      if(mIRshjBUWS == bdFJODylWT){nOjukmkSdf = true;}
      else if(bdFJODylWT == mIRshjBUWS){CMLGCFYNFx = true;}
      if(DjwNWBZdyo == FwcNxJMFmM){sCkUxmzeCC = true;}
      else if(FwcNxJMFmM == DjwNWBZdyo){qXeUwxiUTp = true;}
      if(wimyrViTmf == NuMRbMaEIE){AxBUuaYhQH = true;}
      else if(NuMRbMaEIE == wimyrViTmf){nyeNepAPLm = true;}
      if(IDIRZhEASU == xFUjMjkKMz){pwnkYpFzMc = true;}
      else if(xFUjMjkKMz == IDIRZhEASU){nVTucAENdw = true;}
      if(iPjOGDHsyL == zAIxQPHkIK){igzLlPMINj = true;}
      else if(zAIxQPHkIK == iPjOGDHsyL){ZtJXqDLdGP = true;}
      if(cJPcAEVTBL == odGqsgRmgC){FqnYPpNOAK = true;}
      else if(odGqsgRmgC == cJPcAEVTBL){wKIfzEhyaL = true;}
      if(AggpKAadno == ctdRVzbrAN){ODBWoxNzwG = true;}
      if(xZKcGgzYqn == YBrmMMmPBM){gzRZSSyoie = true;}
      if(OpUbDymfQV == EouyPTzdqf){NQTgCzIPUi = true;}
      while(ctdRVzbrAN == AggpKAadno){FJCRIVEjru = true;}
      while(YBrmMMmPBM == YBrmMMmPBM){sVpXPtnGVb = true;}
      while(EouyPTzdqf == EouyPTzdqf){eVIuqcMnuY = true;}
      if(SycKWEWpaQ == true){SycKWEWpaQ = false;}
      if(nOjukmkSdf == true){nOjukmkSdf = false;}
      if(sCkUxmzeCC == true){sCkUxmzeCC = false;}
      if(AxBUuaYhQH == true){AxBUuaYhQH = false;}
      if(pwnkYpFzMc == true){pwnkYpFzMc = false;}
      if(igzLlPMINj == true){igzLlPMINj = false;}
      if(FqnYPpNOAK == true){FqnYPpNOAK = false;}
      if(ODBWoxNzwG == true){ODBWoxNzwG = false;}
      if(gzRZSSyoie == true){gzRZSSyoie = false;}
      if(NQTgCzIPUi == true){NQTgCzIPUi = false;}
      if(bpzCNeecVn == true){bpzCNeecVn = false;}
      if(CMLGCFYNFx == true){CMLGCFYNFx = false;}
      if(qXeUwxiUTp == true){qXeUwxiUTp = false;}
      if(nyeNepAPLm == true){nyeNepAPLm = false;}
      if(nVTucAENdw == true){nVTucAENdw = false;}
      if(ZtJXqDLdGP == true){ZtJXqDLdGP = false;}
      if(wKIfzEhyaL == true){wKIfzEhyaL = false;}
      if(FJCRIVEjru == true){FJCRIVEjru = false;}
      if(sVpXPtnGVb == true){sVpXPtnGVb = false;}
      if(eVIuqcMnuY == true){eVIuqcMnuY = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IUQEJCLYKP
{ 
  void naFXIxwkfb()
  { 
      bool YCwQKWmEmD = false;
      bool BYZRdtuCMT = false;
      bool eRbnqWSRmM = false;
      bool teihugQdGw = false;
      bool aKEsYJxzoH = false;
      bool huikoHaHZm = false;
      bool DVQaIVprVw = false;
      bool dRwPrTMhPh = false;
      bool YTcedzwxrn = false;
      bool FJDSsFsckC = false;
      bool TFcrdPySrV = false;
      bool usqrOirlxI = false;
      bool ZrAhpTOcVm = false;
      bool ajwNOTqjAN = false;
      bool AcEWrMRKEK = false;
      bool MHfbbaEtBK = false;
      bool BVwFtKwddu = false;
      bool qIkiHwmcod = false;
      bool BDYLZSDTkm = false;
      bool OwipMefzbh = false;
      string AqikpImMSW;
      string cndERRXyIz;
      string osDSosTDmY;
      string QOGAXALWEn;
      string eNotTKPZIf;
      string gUJZtGmQuw;
      string njYsGOgdgQ;
      string GAsUUaEJye;
      string dsuGrwFfFQ;
      string MtbIzuounF;
      string cksqZgbnFT;
      string CdSQRtnRBK;
      string JBYQDLYkUZ;
      string lIUsBsAEgK;
      string TQalNhbKpE;
      string GXtcwbLcmN;
      string IpSfgsJYOY;
      string tjjpprZmCU;
      string jNuQSNOqxZ;
      string hXQSCdxuEU;
      if(AqikpImMSW == cksqZgbnFT){YCwQKWmEmD = true;}
      else if(cksqZgbnFT == AqikpImMSW){TFcrdPySrV = true;}
      if(cndERRXyIz == CdSQRtnRBK){BYZRdtuCMT = true;}
      else if(CdSQRtnRBK == cndERRXyIz){usqrOirlxI = true;}
      if(osDSosTDmY == JBYQDLYkUZ){eRbnqWSRmM = true;}
      else if(JBYQDLYkUZ == osDSosTDmY){ZrAhpTOcVm = true;}
      if(QOGAXALWEn == lIUsBsAEgK){teihugQdGw = true;}
      else if(lIUsBsAEgK == QOGAXALWEn){ajwNOTqjAN = true;}
      if(eNotTKPZIf == TQalNhbKpE){aKEsYJxzoH = true;}
      else if(TQalNhbKpE == eNotTKPZIf){AcEWrMRKEK = true;}
      if(gUJZtGmQuw == GXtcwbLcmN){huikoHaHZm = true;}
      else if(GXtcwbLcmN == gUJZtGmQuw){MHfbbaEtBK = true;}
      if(njYsGOgdgQ == IpSfgsJYOY){DVQaIVprVw = true;}
      else if(IpSfgsJYOY == njYsGOgdgQ){BVwFtKwddu = true;}
      if(GAsUUaEJye == tjjpprZmCU){dRwPrTMhPh = true;}
      if(dsuGrwFfFQ == jNuQSNOqxZ){YTcedzwxrn = true;}
      if(MtbIzuounF == hXQSCdxuEU){FJDSsFsckC = true;}
      while(tjjpprZmCU == GAsUUaEJye){qIkiHwmcod = true;}
      while(jNuQSNOqxZ == jNuQSNOqxZ){BDYLZSDTkm = true;}
      while(hXQSCdxuEU == hXQSCdxuEU){OwipMefzbh = true;}
      if(YCwQKWmEmD == true){YCwQKWmEmD = false;}
      if(BYZRdtuCMT == true){BYZRdtuCMT = false;}
      if(eRbnqWSRmM == true){eRbnqWSRmM = false;}
      if(teihugQdGw == true){teihugQdGw = false;}
      if(aKEsYJxzoH == true){aKEsYJxzoH = false;}
      if(huikoHaHZm == true){huikoHaHZm = false;}
      if(DVQaIVprVw == true){DVQaIVprVw = false;}
      if(dRwPrTMhPh == true){dRwPrTMhPh = false;}
      if(YTcedzwxrn == true){YTcedzwxrn = false;}
      if(FJDSsFsckC == true){FJDSsFsckC = false;}
      if(TFcrdPySrV == true){TFcrdPySrV = false;}
      if(usqrOirlxI == true){usqrOirlxI = false;}
      if(ZrAhpTOcVm == true){ZrAhpTOcVm = false;}
      if(ajwNOTqjAN == true){ajwNOTqjAN = false;}
      if(AcEWrMRKEK == true){AcEWrMRKEK = false;}
      if(MHfbbaEtBK == true){MHfbbaEtBK = false;}
      if(BVwFtKwddu == true){BVwFtKwddu = false;}
      if(qIkiHwmcod == true){qIkiHwmcod = false;}
      if(BDYLZSDTkm == true){BDYLZSDTkm = false;}
      if(OwipMefzbh == true){OwipMefzbh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AIMFCSGRXO
{ 
  void JGNHBekYUI()
  { 
      bool PgiHXOtRte = false;
      bool nrcAhGIgDR = false;
      bool KbmgMEYNfa = false;
      bool KcZTRdcymp = false;
      bool CiWhcLkFEH = false;
      bool sMdWKnsBtT = false;
      bool pflSUKSIhE = false;
      bool dKtMOaFFDB = false;
      bool TTJXhxzDJd = false;
      bool jaDSAVwEMS = false;
      bool scXzdhSRHi = false;
      bool hmxszVWOjX = false;
      bool LPyDTCGaLr = false;
      bool dBskHpwJsz = false;
      bool dguawwcKTe = false;
      bool zhTnGddUea = false;
      bool arGOEgtMkJ = false;
      bool ZNUjseTXGX = false;
      bool ZwHgswgEdd = false;
      bool MbdSYlafVP = false;
      string oFrzsEFUQi;
      string qPBOJbWuAD;
      string gLUWRglYaw;
      string gTQbkaCeOH;
      string sSIQyAxSJK;
      string DYmSTcjVhM;
      string SgtUBXzYdi;
      string YqfrKOCDXR;
      string iTOPsIgyIH;
      string pfxBoLXnPa;
      string AXnakLrtHF;
      string agyPcySdas;
      string pBQVrrsdCy;
      string pOmxQLfneg;
      string PlpCgJhhYq;
      string ssMWjzGRir;
      string SNrIOLsAyR;
      string SdztcEKNoh;
      string KLFLFwBQNR;
      string ENFlOBtgnU;
      if(oFrzsEFUQi == AXnakLrtHF){PgiHXOtRte = true;}
      else if(AXnakLrtHF == oFrzsEFUQi){scXzdhSRHi = true;}
      if(qPBOJbWuAD == agyPcySdas){nrcAhGIgDR = true;}
      else if(agyPcySdas == qPBOJbWuAD){hmxszVWOjX = true;}
      if(gLUWRglYaw == pBQVrrsdCy){KbmgMEYNfa = true;}
      else if(pBQVrrsdCy == gLUWRglYaw){LPyDTCGaLr = true;}
      if(gTQbkaCeOH == pOmxQLfneg){KcZTRdcymp = true;}
      else if(pOmxQLfneg == gTQbkaCeOH){dBskHpwJsz = true;}
      if(sSIQyAxSJK == PlpCgJhhYq){CiWhcLkFEH = true;}
      else if(PlpCgJhhYq == sSIQyAxSJK){dguawwcKTe = true;}
      if(DYmSTcjVhM == ssMWjzGRir){sMdWKnsBtT = true;}
      else if(ssMWjzGRir == DYmSTcjVhM){zhTnGddUea = true;}
      if(SgtUBXzYdi == SNrIOLsAyR){pflSUKSIhE = true;}
      else if(SNrIOLsAyR == SgtUBXzYdi){arGOEgtMkJ = true;}
      if(YqfrKOCDXR == SdztcEKNoh){dKtMOaFFDB = true;}
      if(iTOPsIgyIH == KLFLFwBQNR){TTJXhxzDJd = true;}
      if(pfxBoLXnPa == ENFlOBtgnU){jaDSAVwEMS = true;}
      while(SdztcEKNoh == YqfrKOCDXR){ZNUjseTXGX = true;}
      while(KLFLFwBQNR == KLFLFwBQNR){ZwHgswgEdd = true;}
      while(ENFlOBtgnU == ENFlOBtgnU){MbdSYlafVP = true;}
      if(PgiHXOtRte == true){PgiHXOtRte = false;}
      if(nrcAhGIgDR == true){nrcAhGIgDR = false;}
      if(KbmgMEYNfa == true){KbmgMEYNfa = false;}
      if(KcZTRdcymp == true){KcZTRdcymp = false;}
      if(CiWhcLkFEH == true){CiWhcLkFEH = false;}
      if(sMdWKnsBtT == true){sMdWKnsBtT = false;}
      if(pflSUKSIhE == true){pflSUKSIhE = false;}
      if(dKtMOaFFDB == true){dKtMOaFFDB = false;}
      if(TTJXhxzDJd == true){TTJXhxzDJd = false;}
      if(jaDSAVwEMS == true){jaDSAVwEMS = false;}
      if(scXzdhSRHi == true){scXzdhSRHi = false;}
      if(hmxszVWOjX == true){hmxszVWOjX = false;}
      if(LPyDTCGaLr == true){LPyDTCGaLr = false;}
      if(dBskHpwJsz == true){dBskHpwJsz = false;}
      if(dguawwcKTe == true){dguawwcKTe = false;}
      if(zhTnGddUea == true){zhTnGddUea = false;}
      if(arGOEgtMkJ == true){arGOEgtMkJ = false;}
      if(ZNUjseTXGX == true){ZNUjseTXGX = false;}
      if(ZwHgswgEdd == true){ZwHgswgEdd = false;}
      if(MbdSYlafVP == true){MbdSYlafVP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GZQPKULYRR
{ 
  void LTIOSbRWLH()
  { 
      bool PbArAiqSbI = false;
      bool VbKMWZXmMC = false;
      bool ENRDsHoZiH = false;
      bool owgiEFZEZk = false;
      bool SypIRApCet = false;
      bool UmhiYBJxys = false;
      bool tRiTTYubIR = false;
      bool zSNajFXIls = false;
      bool QluomCOyJC = false;
      bool ucGgqBPWos = false;
      bool FOtMtBZGzS = false;
      bool dALFbHWtfQ = false;
      bool JwRFVWMfii = false;
      bool pATsmCrhul = false;
      bool cIzpYtRAEj = false;
      bool dVpXiIwopX = false;
      bool nsWPccgSSZ = false;
      bool PnytnBxbfb = false;
      bool FMbzEUKuJd = false;
      bool GDqXxikiNw = false;
      string jMkMfRWeYW;
      string AoOcaTCQNe;
      string ZqxnsAWdnK;
      string HKFKDmkBrl;
      string hsIITOPLjh;
      string UIYmWrPwTf;
      string tspNPpjEYm;
      string iAYaxfhnUd;
      string jLsGoylRmK;
      string WnhLqnDtsc;
      string eLlxzTjxpp;
      string KabpuhAiYQ;
      string ESQNPtSNiI;
      string cmuqzYZwHU;
      string OnaxAwMqPI;
      string nTVcYMuXgE;
      string BVNgMWfjpW;
      string IEbDBPqSMT;
      string gaxZUJRRRm;
      string cBOCTsZtkd;
      if(jMkMfRWeYW == eLlxzTjxpp){PbArAiqSbI = true;}
      else if(eLlxzTjxpp == jMkMfRWeYW){FOtMtBZGzS = true;}
      if(AoOcaTCQNe == KabpuhAiYQ){VbKMWZXmMC = true;}
      else if(KabpuhAiYQ == AoOcaTCQNe){dALFbHWtfQ = true;}
      if(ZqxnsAWdnK == ESQNPtSNiI){ENRDsHoZiH = true;}
      else if(ESQNPtSNiI == ZqxnsAWdnK){JwRFVWMfii = true;}
      if(HKFKDmkBrl == cmuqzYZwHU){owgiEFZEZk = true;}
      else if(cmuqzYZwHU == HKFKDmkBrl){pATsmCrhul = true;}
      if(hsIITOPLjh == OnaxAwMqPI){SypIRApCet = true;}
      else if(OnaxAwMqPI == hsIITOPLjh){cIzpYtRAEj = true;}
      if(UIYmWrPwTf == nTVcYMuXgE){UmhiYBJxys = true;}
      else if(nTVcYMuXgE == UIYmWrPwTf){dVpXiIwopX = true;}
      if(tspNPpjEYm == BVNgMWfjpW){tRiTTYubIR = true;}
      else if(BVNgMWfjpW == tspNPpjEYm){nsWPccgSSZ = true;}
      if(iAYaxfhnUd == IEbDBPqSMT){zSNajFXIls = true;}
      if(jLsGoylRmK == gaxZUJRRRm){QluomCOyJC = true;}
      if(WnhLqnDtsc == cBOCTsZtkd){ucGgqBPWos = true;}
      while(IEbDBPqSMT == iAYaxfhnUd){PnytnBxbfb = true;}
      while(gaxZUJRRRm == gaxZUJRRRm){FMbzEUKuJd = true;}
      while(cBOCTsZtkd == cBOCTsZtkd){GDqXxikiNw = true;}
      if(PbArAiqSbI == true){PbArAiqSbI = false;}
      if(VbKMWZXmMC == true){VbKMWZXmMC = false;}
      if(ENRDsHoZiH == true){ENRDsHoZiH = false;}
      if(owgiEFZEZk == true){owgiEFZEZk = false;}
      if(SypIRApCet == true){SypIRApCet = false;}
      if(UmhiYBJxys == true){UmhiYBJxys = false;}
      if(tRiTTYubIR == true){tRiTTYubIR = false;}
      if(zSNajFXIls == true){zSNajFXIls = false;}
      if(QluomCOyJC == true){QluomCOyJC = false;}
      if(ucGgqBPWos == true){ucGgqBPWos = false;}
      if(FOtMtBZGzS == true){FOtMtBZGzS = false;}
      if(dALFbHWtfQ == true){dALFbHWtfQ = false;}
      if(JwRFVWMfii == true){JwRFVWMfii = false;}
      if(pATsmCrhul == true){pATsmCrhul = false;}
      if(cIzpYtRAEj == true){cIzpYtRAEj = false;}
      if(dVpXiIwopX == true){dVpXiIwopX = false;}
      if(nsWPccgSSZ == true){nsWPccgSSZ = false;}
      if(PnytnBxbfb == true){PnytnBxbfb = false;}
      if(FMbzEUKuJd == true){FMbzEUKuJd = false;}
      if(GDqXxikiNw == true){GDqXxikiNw = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YMBRCYOAPW
{ 
  void AfzulNFjGr()
  { 
      bool pCJcIKdzdl = false;
      bool zUaNouaGbd = false;
      bool ocXVqNTnGp = false;
      bool autYEmCRDp = false;
      bool hoBtHCfhiy = false;
      bool pDsAqMemku = false;
      bool posnLDaCIC = false;
      bool sIDkuAbcNo = false;
      bool glEHzXBshr = false;
      bool HOfIAruNiD = false;
      bool adYfFhAIEp = false;
      bool kCZuNEjkon = false;
      bool CgUQudbtjD = false;
      bool EOsJNDMYCQ = false;
      bool WsnqHFoktw = false;
      bool IKXuBsJIka = false;
      bool FFYBlVIPhK = false;
      bool qLhXQJscJo = false;
      bool oqAnuspyIT = false;
      bool sDmaLRUmFk = false;
      string ZehJhXrVQw;
      string arSkFjhSMk;
      string OLLfCxcUYD;
      string JZZNqpUuIi;
      string QERQmxrqQr;
      string tQmXBNSCGk;
      string uXxMlBRFtP;
      string PmJdTJttrE;
      string EkEfJslCLd;
      string WRnXmsHEdl;
      string fmxAcGGIDq;
      string TYKBfJSQDs;
      string OPsPsxylpa;
      string CoPFYRnDDC;
      string lcyTKyHmqd;
      string ZpAkXQtgHe;
      string HUScodIEcO;
      string UPMBGkQYFt;
      string oLtMBLnegH;
      string WHuYtQKVef;
      if(ZehJhXrVQw == fmxAcGGIDq){pCJcIKdzdl = true;}
      else if(fmxAcGGIDq == ZehJhXrVQw){adYfFhAIEp = true;}
      if(arSkFjhSMk == TYKBfJSQDs){zUaNouaGbd = true;}
      else if(TYKBfJSQDs == arSkFjhSMk){kCZuNEjkon = true;}
      if(OLLfCxcUYD == OPsPsxylpa){ocXVqNTnGp = true;}
      else if(OPsPsxylpa == OLLfCxcUYD){CgUQudbtjD = true;}
      if(JZZNqpUuIi == CoPFYRnDDC){autYEmCRDp = true;}
      else if(CoPFYRnDDC == JZZNqpUuIi){EOsJNDMYCQ = true;}
      if(QERQmxrqQr == lcyTKyHmqd){hoBtHCfhiy = true;}
      else if(lcyTKyHmqd == QERQmxrqQr){WsnqHFoktw = true;}
      if(tQmXBNSCGk == ZpAkXQtgHe){pDsAqMemku = true;}
      else if(ZpAkXQtgHe == tQmXBNSCGk){IKXuBsJIka = true;}
      if(uXxMlBRFtP == HUScodIEcO){posnLDaCIC = true;}
      else if(HUScodIEcO == uXxMlBRFtP){FFYBlVIPhK = true;}
      if(PmJdTJttrE == UPMBGkQYFt){sIDkuAbcNo = true;}
      if(EkEfJslCLd == oLtMBLnegH){glEHzXBshr = true;}
      if(WRnXmsHEdl == WHuYtQKVef){HOfIAruNiD = true;}
      while(UPMBGkQYFt == PmJdTJttrE){qLhXQJscJo = true;}
      while(oLtMBLnegH == oLtMBLnegH){oqAnuspyIT = true;}
      while(WHuYtQKVef == WHuYtQKVef){sDmaLRUmFk = true;}
      if(pCJcIKdzdl == true){pCJcIKdzdl = false;}
      if(zUaNouaGbd == true){zUaNouaGbd = false;}
      if(ocXVqNTnGp == true){ocXVqNTnGp = false;}
      if(autYEmCRDp == true){autYEmCRDp = false;}
      if(hoBtHCfhiy == true){hoBtHCfhiy = false;}
      if(pDsAqMemku == true){pDsAqMemku = false;}
      if(posnLDaCIC == true){posnLDaCIC = false;}
      if(sIDkuAbcNo == true){sIDkuAbcNo = false;}
      if(glEHzXBshr == true){glEHzXBshr = false;}
      if(HOfIAruNiD == true){HOfIAruNiD = false;}
      if(adYfFhAIEp == true){adYfFhAIEp = false;}
      if(kCZuNEjkon == true){kCZuNEjkon = false;}
      if(CgUQudbtjD == true){CgUQudbtjD = false;}
      if(EOsJNDMYCQ == true){EOsJNDMYCQ = false;}
      if(WsnqHFoktw == true){WsnqHFoktw = false;}
      if(IKXuBsJIka == true){IKXuBsJIka = false;}
      if(FFYBlVIPhK == true){FFYBlVIPhK = false;}
      if(qLhXQJscJo == true){qLhXQJscJo = false;}
      if(oqAnuspyIT == true){oqAnuspyIT = false;}
      if(sDmaLRUmFk == true){sDmaLRUmFk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IIMFZUPJTV
{ 
  void GnhdXBdqbQ()
  { 
      bool KdnIFCtDDx = false;
      bool rlbwLInCqn = false;
      bool qNRnegksCn = false;
      bool VBfcdoFAoV = false;
      bool WOYQMWnzUj = false;
      bool RsTopxMzcX = false;
      bool QBBObpFzSV = false;
      bool CmMVQbnAbI = false;
      bool skXRWchqlg = false;
      bool cGXRFVNKoW = false;
      bool QogyhdfyYO = false;
      bool ILWUeeciry = false;
      bool HOMkswPtQt = false;
      bool NdLFqDDZjr = false;
      bool wjSXXRiCmZ = false;
      bool sdSpOCcuFk = false;
      bool OkhdAdARca = false;
      bool aDzzCXunMb = false;
      bool MMHpBiLUpo = false;
      bool aHTydKCoXt = false;
      string uKVuckQcjO;
      string dVXSMQEUtd;
      string ENXPnsusWR;
      string IhfHzEWrYs;
      string WMHCRjCFjC;
      string qAwfihlFHi;
      string oXGRGuoPSI;
      string sMwBHrGVyE;
      string poIUaHlAYR;
      string RQjoDyGouW;
      string tlmbIeEtYq;
      string aQHDrAOlqy;
      string IoqBpcbGxI;
      string kQZbJECgLu;
      string DrbhqBJgxD;
      string jTyxNFulke;
      string CxwPFJpesn;
      string xgZutQNIJo;
      string TtJtXRSbNE;
      string IQwgsFuLcg;
      if(uKVuckQcjO == tlmbIeEtYq){KdnIFCtDDx = true;}
      else if(tlmbIeEtYq == uKVuckQcjO){QogyhdfyYO = true;}
      if(dVXSMQEUtd == aQHDrAOlqy){rlbwLInCqn = true;}
      else if(aQHDrAOlqy == dVXSMQEUtd){ILWUeeciry = true;}
      if(ENXPnsusWR == IoqBpcbGxI){qNRnegksCn = true;}
      else if(IoqBpcbGxI == ENXPnsusWR){HOMkswPtQt = true;}
      if(IhfHzEWrYs == kQZbJECgLu){VBfcdoFAoV = true;}
      else if(kQZbJECgLu == IhfHzEWrYs){NdLFqDDZjr = true;}
      if(WMHCRjCFjC == DrbhqBJgxD){WOYQMWnzUj = true;}
      else if(DrbhqBJgxD == WMHCRjCFjC){wjSXXRiCmZ = true;}
      if(qAwfihlFHi == jTyxNFulke){RsTopxMzcX = true;}
      else if(jTyxNFulke == qAwfihlFHi){sdSpOCcuFk = true;}
      if(oXGRGuoPSI == CxwPFJpesn){QBBObpFzSV = true;}
      else if(CxwPFJpesn == oXGRGuoPSI){OkhdAdARca = true;}
      if(sMwBHrGVyE == xgZutQNIJo){CmMVQbnAbI = true;}
      if(poIUaHlAYR == TtJtXRSbNE){skXRWchqlg = true;}
      if(RQjoDyGouW == IQwgsFuLcg){cGXRFVNKoW = true;}
      while(xgZutQNIJo == sMwBHrGVyE){aDzzCXunMb = true;}
      while(TtJtXRSbNE == TtJtXRSbNE){MMHpBiLUpo = true;}
      while(IQwgsFuLcg == IQwgsFuLcg){aHTydKCoXt = true;}
      if(KdnIFCtDDx == true){KdnIFCtDDx = false;}
      if(rlbwLInCqn == true){rlbwLInCqn = false;}
      if(qNRnegksCn == true){qNRnegksCn = false;}
      if(VBfcdoFAoV == true){VBfcdoFAoV = false;}
      if(WOYQMWnzUj == true){WOYQMWnzUj = false;}
      if(RsTopxMzcX == true){RsTopxMzcX = false;}
      if(QBBObpFzSV == true){QBBObpFzSV = false;}
      if(CmMVQbnAbI == true){CmMVQbnAbI = false;}
      if(skXRWchqlg == true){skXRWchqlg = false;}
      if(cGXRFVNKoW == true){cGXRFVNKoW = false;}
      if(QogyhdfyYO == true){QogyhdfyYO = false;}
      if(ILWUeeciry == true){ILWUeeciry = false;}
      if(HOMkswPtQt == true){HOMkswPtQt = false;}
      if(NdLFqDDZjr == true){NdLFqDDZjr = false;}
      if(wjSXXRiCmZ == true){wjSXXRiCmZ = false;}
      if(sdSpOCcuFk == true){sdSpOCcuFk = false;}
      if(OkhdAdARca == true){OkhdAdARca = false;}
      if(aDzzCXunMb == true){aDzzCXunMb = false;}
      if(MMHpBiLUpo == true){MMHpBiLUpo = false;}
      if(aHTydKCoXt == true){aHTydKCoXt = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WDBETAJYDZ
{ 
  void EHcWtwKyrF()
  { 
      bool amSiGVdqgx = false;
      bool aKVbIlybhw = false;
      bool YZfkAJPHZu = false;
      bool cwpPwSYONW = false;
      bool PclpNukydY = false;
      bool meSwaBHhhx = false;
      bool FVRlUeTFfo = false;
      bool NjOKfunoWF = false;
      bool XQdAHxctot = false;
      bool KpfPQsLzVi = false;
      bool afSnKCQIPe = false;
      bool NSLcZorRdX = false;
      bool BZQkeXEysm = false;
      bool jnSmdrypoF = false;
      bool fUdJgreCpb = false;
      bool BsUygAOgJm = false;
      bool ySPcuYzXyh = false;
      bool pBbYqgwHEl = false;
      bool rBIQkpSUGd = false;
      bool cQgCGNFANH = false;
      string iQdkGRhrUY;
      string lECbYuzDwy;
      string dnbullYxTn;
      string oFYpgkhfRY;
      string OqkOCbYmep;
      string iVMQwuoiyT;
      string sjXgiffMtn;
      string NSkXjlCNrJ;
      string MkScFSrHsl;
      string wyhxfdSEeV;
      string wMsqcPjnfJ;
      string rbqnHHRtaP;
      string nXSIqOknrF;
      string SyhWNAqaAp;
      string iDaisTHspX;
      string MYcOhXtjzO;
      string dwwaqtsTaG;
      string QqtkgjwiOW;
      string GqxUstUTno;
      string GGalLlHArz;
      if(iQdkGRhrUY == wMsqcPjnfJ){amSiGVdqgx = true;}
      else if(wMsqcPjnfJ == iQdkGRhrUY){afSnKCQIPe = true;}
      if(lECbYuzDwy == rbqnHHRtaP){aKVbIlybhw = true;}
      else if(rbqnHHRtaP == lECbYuzDwy){NSLcZorRdX = true;}
      if(dnbullYxTn == nXSIqOknrF){YZfkAJPHZu = true;}
      else if(nXSIqOknrF == dnbullYxTn){BZQkeXEysm = true;}
      if(oFYpgkhfRY == SyhWNAqaAp){cwpPwSYONW = true;}
      else if(SyhWNAqaAp == oFYpgkhfRY){jnSmdrypoF = true;}
      if(OqkOCbYmep == iDaisTHspX){PclpNukydY = true;}
      else if(iDaisTHspX == OqkOCbYmep){fUdJgreCpb = true;}
      if(iVMQwuoiyT == MYcOhXtjzO){meSwaBHhhx = true;}
      else if(MYcOhXtjzO == iVMQwuoiyT){BsUygAOgJm = true;}
      if(sjXgiffMtn == dwwaqtsTaG){FVRlUeTFfo = true;}
      else if(dwwaqtsTaG == sjXgiffMtn){ySPcuYzXyh = true;}
      if(NSkXjlCNrJ == QqtkgjwiOW){NjOKfunoWF = true;}
      if(MkScFSrHsl == GqxUstUTno){XQdAHxctot = true;}
      if(wyhxfdSEeV == GGalLlHArz){KpfPQsLzVi = true;}
      while(QqtkgjwiOW == NSkXjlCNrJ){pBbYqgwHEl = true;}
      while(GqxUstUTno == GqxUstUTno){rBIQkpSUGd = true;}
      while(GGalLlHArz == GGalLlHArz){cQgCGNFANH = true;}
      if(amSiGVdqgx == true){amSiGVdqgx = false;}
      if(aKVbIlybhw == true){aKVbIlybhw = false;}
      if(YZfkAJPHZu == true){YZfkAJPHZu = false;}
      if(cwpPwSYONW == true){cwpPwSYONW = false;}
      if(PclpNukydY == true){PclpNukydY = false;}
      if(meSwaBHhhx == true){meSwaBHhhx = false;}
      if(FVRlUeTFfo == true){FVRlUeTFfo = false;}
      if(NjOKfunoWF == true){NjOKfunoWF = false;}
      if(XQdAHxctot == true){XQdAHxctot = false;}
      if(KpfPQsLzVi == true){KpfPQsLzVi = false;}
      if(afSnKCQIPe == true){afSnKCQIPe = false;}
      if(NSLcZorRdX == true){NSLcZorRdX = false;}
      if(BZQkeXEysm == true){BZQkeXEysm = false;}
      if(jnSmdrypoF == true){jnSmdrypoF = false;}
      if(fUdJgreCpb == true){fUdJgreCpb = false;}
      if(BsUygAOgJm == true){BsUygAOgJm = false;}
      if(ySPcuYzXyh == true){ySPcuYzXyh = false;}
      if(pBbYqgwHEl == true){pBbYqgwHEl = false;}
      if(rBIQkpSUGd == true){rBIQkpSUGd = false;}
      if(cQgCGNFANH == true){cQgCGNFANH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RORWVDLZTG
{ 
  void QYJhVZHyAU()
  { 
      bool ZtgqSaQcrO = false;
      bool fsauIzDsrj = false;
      bool JXhjPYEsKs = false;
      bool HJnOWIaZNe = false;
      bool YbPLHCGPhc = false;
      bool hOrmSCLxbs = false;
      bool WWRyGVELXz = false;
      bool YdfMKgIWbM = false;
      bool SoLhLsQBwT = false;
      bool wcxUIBKREk = false;
      bool ytqhEDifbl = false;
      bool toVOZgLilN = false;
      bool FCPDDufKEI = false;
      bool VqRpOYxcdH = false;
      bool JkYJZGHita = false;
      bool pbeqfuemeI = false;
      bool XXbTPDXeAA = false;
      bool jiEeDTdFdx = false;
      bool mxQcStLaJS = false;
      bool tJxUjlJFtW = false;
      string JXTDfQePDr;
      string rEmVghGYYk;
      string dTZYtDINIc;
      string OeoOONLBqm;
      string bwMlpWnIOn;
      string YUQGzoxuJb;
      string HRHZdMPYHS;
      string WEhkIgGmhW;
      string PAyTDsOiPY;
      string PQqOfKolIc;
      string HOAuRTmfxR;
      string iKMmMGSmWk;
      string yfJhDGMSFy;
      string rUeEOBzDCi;
      string FhkxpFDECY;
      string qBPWdaFSrL;
      string HTZzZNeIXL;
      string GPkmePuLqx;
      string DHQfGpEHZn;
      string ugKqbkIkWb;
      if(JXTDfQePDr == HOAuRTmfxR){ZtgqSaQcrO = true;}
      else if(HOAuRTmfxR == JXTDfQePDr){ytqhEDifbl = true;}
      if(rEmVghGYYk == iKMmMGSmWk){fsauIzDsrj = true;}
      else if(iKMmMGSmWk == rEmVghGYYk){toVOZgLilN = true;}
      if(dTZYtDINIc == yfJhDGMSFy){JXhjPYEsKs = true;}
      else if(yfJhDGMSFy == dTZYtDINIc){FCPDDufKEI = true;}
      if(OeoOONLBqm == rUeEOBzDCi){HJnOWIaZNe = true;}
      else if(rUeEOBzDCi == OeoOONLBqm){VqRpOYxcdH = true;}
      if(bwMlpWnIOn == FhkxpFDECY){YbPLHCGPhc = true;}
      else if(FhkxpFDECY == bwMlpWnIOn){JkYJZGHita = true;}
      if(YUQGzoxuJb == qBPWdaFSrL){hOrmSCLxbs = true;}
      else if(qBPWdaFSrL == YUQGzoxuJb){pbeqfuemeI = true;}
      if(HRHZdMPYHS == HTZzZNeIXL){WWRyGVELXz = true;}
      else if(HTZzZNeIXL == HRHZdMPYHS){XXbTPDXeAA = true;}
      if(WEhkIgGmhW == GPkmePuLqx){YdfMKgIWbM = true;}
      if(PAyTDsOiPY == DHQfGpEHZn){SoLhLsQBwT = true;}
      if(PQqOfKolIc == ugKqbkIkWb){wcxUIBKREk = true;}
      while(GPkmePuLqx == WEhkIgGmhW){jiEeDTdFdx = true;}
      while(DHQfGpEHZn == DHQfGpEHZn){mxQcStLaJS = true;}
      while(ugKqbkIkWb == ugKqbkIkWb){tJxUjlJFtW = true;}
      if(ZtgqSaQcrO == true){ZtgqSaQcrO = false;}
      if(fsauIzDsrj == true){fsauIzDsrj = false;}
      if(JXhjPYEsKs == true){JXhjPYEsKs = false;}
      if(HJnOWIaZNe == true){HJnOWIaZNe = false;}
      if(YbPLHCGPhc == true){YbPLHCGPhc = false;}
      if(hOrmSCLxbs == true){hOrmSCLxbs = false;}
      if(WWRyGVELXz == true){WWRyGVELXz = false;}
      if(YdfMKgIWbM == true){YdfMKgIWbM = false;}
      if(SoLhLsQBwT == true){SoLhLsQBwT = false;}
      if(wcxUIBKREk == true){wcxUIBKREk = false;}
      if(ytqhEDifbl == true){ytqhEDifbl = false;}
      if(toVOZgLilN == true){toVOZgLilN = false;}
      if(FCPDDufKEI == true){FCPDDufKEI = false;}
      if(VqRpOYxcdH == true){VqRpOYxcdH = false;}
      if(JkYJZGHita == true){JkYJZGHita = false;}
      if(pbeqfuemeI == true){pbeqfuemeI = false;}
      if(XXbTPDXeAA == true){XXbTPDXeAA = false;}
      if(jiEeDTdFdx == true){jiEeDTdFdx = false;}
      if(mxQcStLaJS == true){mxQcStLaJS = false;}
      if(tJxUjlJFtW == true){tJxUjlJFtW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UDNFXOCEPV
{ 
  void TyQCkqzhyq()
  { 
      bool ENohokQGNW = false;
      bool qhyzALaZHZ = false;
      bool mjTYdNryan = false;
      bool AGpSzuLYLI = false;
      bool VmXaeeWcJR = false;
      bool oYMcwTsPDb = false;
      bool sSfYWkrfzE = false;
      bool zBOyDAMstz = false;
      bool pPnJzasNVf = false;
      bool CwVjPDGyEs = false;
      bool mqdFRmKNYu = false;
      bool GAUDkdeWZu = false;
      bool qdJSCDfdiz = false;
      bool tOnAMzkhqL = false;
      bool QfoZYBaRyl = false;
      bool YiXJCIgKDN = false;
      bool GfeYBqLUql = false;
      bool rweOdLoisA = false;
      bool cOfwaKqeyD = false;
      bool RaedFkBPYh = false;
      string qGaeJniaWh;
      string pKVXZhKyia;
      string KuJUqEnWdo;
      string QklYzwYFLd;
      string SyllCnftKJ;
      string BCHLkuwFmE;
      string zMwgQEXwXc;
      string JEmEHMJXSD;
      string YsMUmNdanD;
      string zSSGeGBEUx;
      string PjVgAxPCDY;
      string lYmifFAbtC;
      string axIUdpkgZB;
      string LEDUnnWeTp;
      string yaWPRZfjZY;
      string GYKRWRaBCJ;
      string MhbCMgVRuq;
      string iUIDEzasDD;
      string JAHBtmskyV;
      string NVbKcPzUCj;
      if(qGaeJniaWh == PjVgAxPCDY){ENohokQGNW = true;}
      else if(PjVgAxPCDY == qGaeJniaWh){mqdFRmKNYu = true;}
      if(pKVXZhKyia == lYmifFAbtC){qhyzALaZHZ = true;}
      else if(lYmifFAbtC == pKVXZhKyia){GAUDkdeWZu = true;}
      if(KuJUqEnWdo == axIUdpkgZB){mjTYdNryan = true;}
      else if(axIUdpkgZB == KuJUqEnWdo){qdJSCDfdiz = true;}
      if(QklYzwYFLd == LEDUnnWeTp){AGpSzuLYLI = true;}
      else if(LEDUnnWeTp == QklYzwYFLd){tOnAMzkhqL = true;}
      if(SyllCnftKJ == yaWPRZfjZY){VmXaeeWcJR = true;}
      else if(yaWPRZfjZY == SyllCnftKJ){QfoZYBaRyl = true;}
      if(BCHLkuwFmE == GYKRWRaBCJ){oYMcwTsPDb = true;}
      else if(GYKRWRaBCJ == BCHLkuwFmE){YiXJCIgKDN = true;}
      if(zMwgQEXwXc == MhbCMgVRuq){sSfYWkrfzE = true;}
      else if(MhbCMgVRuq == zMwgQEXwXc){GfeYBqLUql = true;}
      if(JEmEHMJXSD == iUIDEzasDD){zBOyDAMstz = true;}
      if(YsMUmNdanD == JAHBtmskyV){pPnJzasNVf = true;}
      if(zSSGeGBEUx == NVbKcPzUCj){CwVjPDGyEs = true;}
      while(iUIDEzasDD == JEmEHMJXSD){rweOdLoisA = true;}
      while(JAHBtmskyV == JAHBtmskyV){cOfwaKqeyD = true;}
      while(NVbKcPzUCj == NVbKcPzUCj){RaedFkBPYh = true;}
      if(ENohokQGNW == true){ENohokQGNW = false;}
      if(qhyzALaZHZ == true){qhyzALaZHZ = false;}
      if(mjTYdNryan == true){mjTYdNryan = false;}
      if(AGpSzuLYLI == true){AGpSzuLYLI = false;}
      if(VmXaeeWcJR == true){VmXaeeWcJR = false;}
      if(oYMcwTsPDb == true){oYMcwTsPDb = false;}
      if(sSfYWkrfzE == true){sSfYWkrfzE = false;}
      if(zBOyDAMstz == true){zBOyDAMstz = false;}
      if(pPnJzasNVf == true){pPnJzasNVf = false;}
      if(CwVjPDGyEs == true){CwVjPDGyEs = false;}
      if(mqdFRmKNYu == true){mqdFRmKNYu = false;}
      if(GAUDkdeWZu == true){GAUDkdeWZu = false;}
      if(qdJSCDfdiz == true){qdJSCDfdiz = false;}
      if(tOnAMzkhqL == true){tOnAMzkhqL = false;}
      if(QfoZYBaRyl == true){QfoZYBaRyl = false;}
      if(YiXJCIgKDN == true){YiXJCIgKDN = false;}
      if(GfeYBqLUql == true){GfeYBqLUql = false;}
      if(rweOdLoisA == true){rweOdLoisA = false;}
      if(cOfwaKqeyD == true){cOfwaKqeyD = false;}
      if(RaedFkBPYh == true){RaedFkBPYh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MEXPXNFLLE
{ 
  void cZROmAUxew()
  { 
      bool UCVLiyrKbR = false;
      bool CoqzlWseaS = false;
      bool bGZwjjeesP = false;
      bool taTLqYmGQb = false;
      bool jpqEebgFSK = false;
      bool qDRTVObZby = false;
      bool TNRMWbVfHN = false;
      bool ilNzilPVQI = false;
      bool DYMhYqIzoc = false;
      bool wNeRHVXdgD = false;
      bool FBxEaEoSjj = false;
      bool zFISutAimf = false;
      bool SVpeQVHuqM = false;
      bool NknNCCbHGU = false;
      bool BpPFdpELWM = false;
      bool UnqBkjKSnF = false;
      bool wsbWVNRuQs = false;
      bool SscSdbGkAZ = false;
      bool RwYdrZtmcS = false;
      bool qITBXIOrjP = false;
      string yGRdYUDlYa;
      string tgonQIzDlE;
      string SZWGDJaUei;
      string qVIQNwWHsU;
      string OkPGHLJNlY;
      string fCmBhXiLZO;
      string thHpjXXXaT;
      string lqKQphKQzr;
      string zMiFCniHio;
      string MLhdZgdYWi;
      string mlmSmorVLA;
      string NSMhPhzBnu;
      string oQokGZIgqI;
      string EktPENZPRN;
      string uMwiatyzNq;
      string hbkatWwBLN;
      string fLeCucYwdB;
      string IiiZzbUGKM;
      string aJSzabASVg;
      string RXEEgjhsmy;
      if(yGRdYUDlYa == mlmSmorVLA){UCVLiyrKbR = true;}
      else if(mlmSmorVLA == yGRdYUDlYa){FBxEaEoSjj = true;}
      if(tgonQIzDlE == NSMhPhzBnu){CoqzlWseaS = true;}
      else if(NSMhPhzBnu == tgonQIzDlE){zFISutAimf = true;}
      if(SZWGDJaUei == oQokGZIgqI){bGZwjjeesP = true;}
      else if(oQokGZIgqI == SZWGDJaUei){SVpeQVHuqM = true;}
      if(qVIQNwWHsU == EktPENZPRN){taTLqYmGQb = true;}
      else if(EktPENZPRN == qVIQNwWHsU){NknNCCbHGU = true;}
      if(OkPGHLJNlY == uMwiatyzNq){jpqEebgFSK = true;}
      else if(uMwiatyzNq == OkPGHLJNlY){BpPFdpELWM = true;}
      if(fCmBhXiLZO == hbkatWwBLN){qDRTVObZby = true;}
      else if(hbkatWwBLN == fCmBhXiLZO){UnqBkjKSnF = true;}
      if(thHpjXXXaT == fLeCucYwdB){TNRMWbVfHN = true;}
      else if(fLeCucYwdB == thHpjXXXaT){wsbWVNRuQs = true;}
      if(lqKQphKQzr == IiiZzbUGKM){ilNzilPVQI = true;}
      if(zMiFCniHio == aJSzabASVg){DYMhYqIzoc = true;}
      if(MLhdZgdYWi == RXEEgjhsmy){wNeRHVXdgD = true;}
      while(IiiZzbUGKM == lqKQphKQzr){SscSdbGkAZ = true;}
      while(aJSzabASVg == aJSzabASVg){RwYdrZtmcS = true;}
      while(RXEEgjhsmy == RXEEgjhsmy){qITBXIOrjP = true;}
      if(UCVLiyrKbR == true){UCVLiyrKbR = false;}
      if(CoqzlWseaS == true){CoqzlWseaS = false;}
      if(bGZwjjeesP == true){bGZwjjeesP = false;}
      if(taTLqYmGQb == true){taTLqYmGQb = false;}
      if(jpqEebgFSK == true){jpqEebgFSK = false;}
      if(qDRTVObZby == true){qDRTVObZby = false;}
      if(TNRMWbVfHN == true){TNRMWbVfHN = false;}
      if(ilNzilPVQI == true){ilNzilPVQI = false;}
      if(DYMhYqIzoc == true){DYMhYqIzoc = false;}
      if(wNeRHVXdgD == true){wNeRHVXdgD = false;}
      if(FBxEaEoSjj == true){FBxEaEoSjj = false;}
      if(zFISutAimf == true){zFISutAimf = false;}
      if(SVpeQVHuqM == true){SVpeQVHuqM = false;}
      if(NknNCCbHGU == true){NknNCCbHGU = false;}
      if(BpPFdpELWM == true){BpPFdpELWM = false;}
      if(UnqBkjKSnF == true){UnqBkjKSnF = false;}
      if(wsbWVNRuQs == true){wsbWVNRuQs = false;}
      if(SscSdbGkAZ == true){SscSdbGkAZ = false;}
      if(RwYdrZtmcS == true){RwYdrZtmcS = false;}
      if(qITBXIOrjP == true){qITBXIOrjP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NYCDBSVBUN
{ 
  void LPwuMEubMi()
  { 
      bool DWUWClsImQ = false;
      bool sEQreIYGEU = false;
      bool yVWEQbaCkg = false;
      bool krWaXwfVcM = false;
      bool ERjEMHdfhY = false;
      bool PDNtBgzQFa = false;
      bool LqWGeBgoUh = false;
      bool bDdqQflRqI = false;
      bool ytTMQhebZp = false;
      bool xPPCBMudZL = false;
      bool ZnLAwMYVdU = false;
      bool ugYwcQXJow = false;
      bool VFOejMmboD = false;
      bool kIreiJiSPE = false;
      bool IIxFtPpSZj = false;
      bool okncoTYgPi = false;
      bool YxgZimnsNS = false;
      bool hEtkmNHJjT = false;
      bool WUBENATOpO = false;
      bool jCCfeLbWWH = false;
      string eXmpEDfPAe;
      string xzCIucEtHG;
      string oRlYXIRKxH;
      string pQJXLFEUYU;
      string wAdXkbrneF;
      string ZcdzONlOSu;
      string lVRrnfpexp;
      string YiUmKomOmw;
      string QcfqhNmJIy;
      string TpCdhZiliF;
      string kNqogpHpnZ;
      string ZXaVeJoZKa;
      string rfVzVigqEj;
      string GBehKClqKq;
      string FwViXuPfyY;
      string kIaIMEqqSy;
      string JQlDSXrrKb;
      string WyOSUwKmaa;
      string TArezxZWVG;
      string LtXMhnKRPz;
      if(eXmpEDfPAe == kNqogpHpnZ){DWUWClsImQ = true;}
      else if(kNqogpHpnZ == eXmpEDfPAe){ZnLAwMYVdU = true;}
      if(xzCIucEtHG == ZXaVeJoZKa){sEQreIYGEU = true;}
      else if(ZXaVeJoZKa == xzCIucEtHG){ugYwcQXJow = true;}
      if(oRlYXIRKxH == rfVzVigqEj){yVWEQbaCkg = true;}
      else if(rfVzVigqEj == oRlYXIRKxH){VFOejMmboD = true;}
      if(pQJXLFEUYU == GBehKClqKq){krWaXwfVcM = true;}
      else if(GBehKClqKq == pQJXLFEUYU){kIreiJiSPE = true;}
      if(wAdXkbrneF == FwViXuPfyY){ERjEMHdfhY = true;}
      else if(FwViXuPfyY == wAdXkbrneF){IIxFtPpSZj = true;}
      if(ZcdzONlOSu == kIaIMEqqSy){PDNtBgzQFa = true;}
      else if(kIaIMEqqSy == ZcdzONlOSu){okncoTYgPi = true;}
      if(lVRrnfpexp == JQlDSXrrKb){LqWGeBgoUh = true;}
      else if(JQlDSXrrKb == lVRrnfpexp){YxgZimnsNS = true;}
      if(YiUmKomOmw == WyOSUwKmaa){bDdqQflRqI = true;}
      if(QcfqhNmJIy == TArezxZWVG){ytTMQhebZp = true;}
      if(TpCdhZiliF == LtXMhnKRPz){xPPCBMudZL = true;}
      while(WyOSUwKmaa == YiUmKomOmw){hEtkmNHJjT = true;}
      while(TArezxZWVG == TArezxZWVG){WUBENATOpO = true;}
      while(LtXMhnKRPz == LtXMhnKRPz){jCCfeLbWWH = true;}
      if(DWUWClsImQ == true){DWUWClsImQ = false;}
      if(sEQreIYGEU == true){sEQreIYGEU = false;}
      if(yVWEQbaCkg == true){yVWEQbaCkg = false;}
      if(krWaXwfVcM == true){krWaXwfVcM = false;}
      if(ERjEMHdfhY == true){ERjEMHdfhY = false;}
      if(PDNtBgzQFa == true){PDNtBgzQFa = false;}
      if(LqWGeBgoUh == true){LqWGeBgoUh = false;}
      if(bDdqQflRqI == true){bDdqQflRqI = false;}
      if(ytTMQhebZp == true){ytTMQhebZp = false;}
      if(xPPCBMudZL == true){xPPCBMudZL = false;}
      if(ZnLAwMYVdU == true){ZnLAwMYVdU = false;}
      if(ugYwcQXJow == true){ugYwcQXJow = false;}
      if(VFOejMmboD == true){VFOejMmboD = false;}
      if(kIreiJiSPE == true){kIreiJiSPE = false;}
      if(IIxFtPpSZj == true){IIxFtPpSZj = false;}
      if(okncoTYgPi == true){okncoTYgPi = false;}
      if(YxgZimnsNS == true){YxgZimnsNS = false;}
      if(hEtkmNHJjT == true){hEtkmNHJjT = false;}
      if(WUBENATOpO == true){WUBENATOpO = false;}
      if(jCCfeLbWWH == true){jCCfeLbWWH = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XEYWHJCYPH
{ 
  void KwgTPmMhqL()
  { 
      bool bSozlurwzy = false;
      bool KbAcMeHGAG = false;
      bool LslbXjGukb = false;
      bool WguNRnbUxu = false;
      bool lAlIWKjkWr = false;
      bool kGsCthgMbC = false;
      bool nxLWacglKX = false;
      bool DmJETLHcEM = false;
      bool AVYrqQnrTi = false;
      bool HOruHooOgs = false;
      bool dxayoxxnii = false;
      bool aEIOCSYSeK = false;
      bool onSaQotOxd = false;
      bool WsuKhCtZDs = false;
      bool SzJkIDGdrn = false;
      bool QViWEodzgz = false;
      bool DiqEMhIxfz = false;
      bool XyPlqpehSN = false;
      bool iPVJNBJYGs = false;
      bool YurEojHFYa = false;
      string deMIxJpUkR;
      string PNEodKJukQ;
      string gdiNNOaZud;
      string DnPXbluUYO;
      string lbqWzwNQQE;
      string zcDdpzuyth;
      string iPYHOayfMq;
      string PoVVdEmMOR;
      string BJJqdZkkgc;
      string pwZqfWDiHY;
      string ygEYnKGgzl;
      string MijbfYGgcn;
      string oMzjBTOepH;
      string ADEpSUEkDT;
      string TEktPGMyKG;
      string QVFTilgUWT;
      string ubpqkJesJL;
      string TUdJUkaETt;
      string HQTjqexkHg;
      string mjNiXrNsJG;
      if(deMIxJpUkR == ygEYnKGgzl){bSozlurwzy = true;}
      else if(ygEYnKGgzl == deMIxJpUkR){dxayoxxnii = true;}
      if(PNEodKJukQ == MijbfYGgcn){KbAcMeHGAG = true;}
      else if(MijbfYGgcn == PNEodKJukQ){aEIOCSYSeK = true;}
      if(gdiNNOaZud == oMzjBTOepH){LslbXjGukb = true;}
      else if(oMzjBTOepH == gdiNNOaZud){onSaQotOxd = true;}
      if(DnPXbluUYO == ADEpSUEkDT){WguNRnbUxu = true;}
      else if(ADEpSUEkDT == DnPXbluUYO){WsuKhCtZDs = true;}
      if(lbqWzwNQQE == TEktPGMyKG){lAlIWKjkWr = true;}
      else if(TEktPGMyKG == lbqWzwNQQE){SzJkIDGdrn = true;}
      if(zcDdpzuyth == QVFTilgUWT){kGsCthgMbC = true;}
      else if(QVFTilgUWT == zcDdpzuyth){QViWEodzgz = true;}
      if(iPYHOayfMq == ubpqkJesJL){nxLWacglKX = true;}
      else if(ubpqkJesJL == iPYHOayfMq){DiqEMhIxfz = true;}
      if(PoVVdEmMOR == TUdJUkaETt){DmJETLHcEM = true;}
      if(BJJqdZkkgc == HQTjqexkHg){AVYrqQnrTi = true;}
      if(pwZqfWDiHY == mjNiXrNsJG){HOruHooOgs = true;}
      while(TUdJUkaETt == PoVVdEmMOR){XyPlqpehSN = true;}
      while(HQTjqexkHg == HQTjqexkHg){iPVJNBJYGs = true;}
      while(mjNiXrNsJG == mjNiXrNsJG){YurEojHFYa = true;}
      if(bSozlurwzy == true){bSozlurwzy = false;}
      if(KbAcMeHGAG == true){KbAcMeHGAG = false;}
      if(LslbXjGukb == true){LslbXjGukb = false;}
      if(WguNRnbUxu == true){WguNRnbUxu = false;}
      if(lAlIWKjkWr == true){lAlIWKjkWr = false;}
      if(kGsCthgMbC == true){kGsCthgMbC = false;}
      if(nxLWacglKX == true){nxLWacglKX = false;}
      if(DmJETLHcEM == true){DmJETLHcEM = false;}
      if(AVYrqQnrTi == true){AVYrqQnrTi = false;}
      if(HOruHooOgs == true){HOruHooOgs = false;}
      if(dxayoxxnii == true){dxayoxxnii = false;}
      if(aEIOCSYSeK == true){aEIOCSYSeK = false;}
      if(onSaQotOxd == true){onSaQotOxd = false;}
      if(WsuKhCtZDs == true){WsuKhCtZDs = false;}
      if(SzJkIDGdrn == true){SzJkIDGdrn = false;}
      if(QViWEodzgz == true){QViWEodzgz = false;}
      if(DiqEMhIxfz == true){DiqEMhIxfz = false;}
      if(XyPlqpehSN == true){XyPlqpehSN = false;}
      if(iPVJNBJYGs == true){iPVJNBJYGs = false;}
      if(YurEojHFYa == true){YurEojHFYa = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IDSFTUVRUC
{ 
  void xGVfJydWGi()
  { 
      bool VMQgDHaIVQ = false;
      bool BhXSQjheLz = false;
      bool hKDalTcwGD = false;
      bool EmhOGJpEPY = false;
      bool fhCKokdXMm = false;
      bool AcXCWKIotj = false;
      bool edBSMNzIMG = false;
      bool sdOlTAIslB = false;
      bool oEiTCbOfqu = false;
      bool BBJetBgiya = false;
      bool kULFsWNliW = false;
      bool CPCstZuhPk = false;
      bool zSRRFqTjqR = false;
      bool SYuKPTrcZf = false;
      bool NbxUskyPjT = false;
      bool GySMbSDtAC = false;
      bool CctQDcUrdZ = false;
      bool gRIjWWUwYT = false;
      bool rEzXKTGhiw = false;
      bool QycWdjhfQP = false;
      string YrtyTHMcOQ;
      string hnATKJlUVr;
      string zSCIfcHqRd;
      string jTOimJPgTn;
      string DBNaIudWcx;
      string kqXnLGpkRp;
      string zULjfNYRfA;
      string tEAgoxEaAk;
      string zkOUsrwaWg;
      string TdPcnhpcuu;
      string VMqbDfqpzN;
      string BOkgqOGpqm;
      string RytzgPoXrH;
      string GbfFOlslQN;
      string BFaJeEshlY;
      string aAkHcXSrum;
      string pTxqGYifRF;
      string VTYckLWkTb;
      string WGkSfGpXbp;
      string SBqcWhHjCF;
      if(YrtyTHMcOQ == VMqbDfqpzN){VMQgDHaIVQ = true;}
      else if(VMqbDfqpzN == YrtyTHMcOQ){kULFsWNliW = true;}
      if(hnATKJlUVr == BOkgqOGpqm){BhXSQjheLz = true;}
      else if(BOkgqOGpqm == hnATKJlUVr){CPCstZuhPk = true;}
      if(zSCIfcHqRd == RytzgPoXrH){hKDalTcwGD = true;}
      else if(RytzgPoXrH == zSCIfcHqRd){zSRRFqTjqR = true;}
      if(jTOimJPgTn == GbfFOlslQN){EmhOGJpEPY = true;}
      else if(GbfFOlslQN == jTOimJPgTn){SYuKPTrcZf = true;}
      if(DBNaIudWcx == BFaJeEshlY){fhCKokdXMm = true;}
      else if(BFaJeEshlY == DBNaIudWcx){NbxUskyPjT = true;}
      if(kqXnLGpkRp == aAkHcXSrum){AcXCWKIotj = true;}
      else if(aAkHcXSrum == kqXnLGpkRp){GySMbSDtAC = true;}
      if(zULjfNYRfA == pTxqGYifRF){edBSMNzIMG = true;}
      else if(pTxqGYifRF == zULjfNYRfA){CctQDcUrdZ = true;}
      if(tEAgoxEaAk == VTYckLWkTb){sdOlTAIslB = true;}
      if(zkOUsrwaWg == WGkSfGpXbp){oEiTCbOfqu = true;}
      if(TdPcnhpcuu == SBqcWhHjCF){BBJetBgiya = true;}
      while(VTYckLWkTb == tEAgoxEaAk){gRIjWWUwYT = true;}
      while(WGkSfGpXbp == WGkSfGpXbp){rEzXKTGhiw = true;}
      while(SBqcWhHjCF == SBqcWhHjCF){QycWdjhfQP = true;}
      if(VMQgDHaIVQ == true){VMQgDHaIVQ = false;}
      if(BhXSQjheLz == true){BhXSQjheLz = false;}
      if(hKDalTcwGD == true){hKDalTcwGD = false;}
      if(EmhOGJpEPY == true){EmhOGJpEPY = false;}
      if(fhCKokdXMm == true){fhCKokdXMm = false;}
      if(AcXCWKIotj == true){AcXCWKIotj = false;}
      if(edBSMNzIMG == true){edBSMNzIMG = false;}
      if(sdOlTAIslB == true){sdOlTAIslB = false;}
      if(oEiTCbOfqu == true){oEiTCbOfqu = false;}
      if(BBJetBgiya == true){BBJetBgiya = false;}
      if(kULFsWNliW == true){kULFsWNliW = false;}
      if(CPCstZuhPk == true){CPCstZuhPk = false;}
      if(zSRRFqTjqR == true){zSRRFqTjqR = false;}
      if(SYuKPTrcZf == true){SYuKPTrcZf = false;}
      if(NbxUskyPjT == true){NbxUskyPjT = false;}
      if(GySMbSDtAC == true){GySMbSDtAC = false;}
      if(CctQDcUrdZ == true){CctQDcUrdZ = false;}
      if(gRIjWWUwYT == true){gRIjWWUwYT = false;}
      if(rEzXKTGhiw == true){rEzXKTGhiw = false;}
      if(QycWdjhfQP == true){QycWdjhfQP = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KTIMCJJBUN
{ 
  void MNCGNedmsc()
  { 
      bool tNBYnPWcSm = false;
      bool YymFioOblT = false;
      bool cAobjRtOiA = false;
      bool ZWzytMAiNQ = false;
      bool kbwNrNdXBq = false;
      bool FfMIksYZoV = false;
      bool VIsSNBzoLV = false;
      bool nfOUiBEbiW = false;
      bool nVMrttJpMA = false;
      bool natquchdAp = false;
      bool NoMfCZQrZj = false;
      bool oxpShAuBXT = false;
      bool qIbKhMkXZG = false;
      bool TZbsIyJgoU = false;
      bool cRQcLqkbIu = false;
      bool gfGfzFMBxo = false;
      bool nsRSQPUxKi = false;
      bool pNMCpYrzzq = false;
      bool bJOeZmTusC = false;
      bool lIizfjzxLw = false;
      string WfmlllHEAj;
      string uDLSdwzUoG;
      string oMJLiEOqay;
      string dmEPjAuClt;
      string AthGyjGpYp;
      string kjyuCJVbDh;
      string XPdBGIzwid;
      string AwiuGudCMy;
      string yXSjNsoIMa;
      string dTzEWjQgAo;
      string kKHtpoBCoH;
      string eSOFizwsnM;
      string TpiIzfxdYV;
      string exCWMmQYtX;
      string rlSFuKFtUW;
      string nWrmVubztJ;
      string rXdTbgQqrw;
      string iXzPUhWcMo;
      string aCIBgHEPBQ;
      string LQdjfUHzcN;
      if(WfmlllHEAj == kKHtpoBCoH){tNBYnPWcSm = true;}
      else if(kKHtpoBCoH == WfmlllHEAj){NoMfCZQrZj = true;}
      if(uDLSdwzUoG == eSOFizwsnM){YymFioOblT = true;}
      else if(eSOFizwsnM == uDLSdwzUoG){oxpShAuBXT = true;}
      if(oMJLiEOqay == TpiIzfxdYV){cAobjRtOiA = true;}
      else if(TpiIzfxdYV == oMJLiEOqay){qIbKhMkXZG = true;}
      if(dmEPjAuClt == exCWMmQYtX){ZWzytMAiNQ = true;}
      else if(exCWMmQYtX == dmEPjAuClt){TZbsIyJgoU = true;}
      if(AthGyjGpYp == rlSFuKFtUW){kbwNrNdXBq = true;}
      else if(rlSFuKFtUW == AthGyjGpYp){cRQcLqkbIu = true;}
      if(kjyuCJVbDh == nWrmVubztJ){FfMIksYZoV = true;}
      else if(nWrmVubztJ == kjyuCJVbDh){gfGfzFMBxo = true;}
      if(XPdBGIzwid == rXdTbgQqrw){VIsSNBzoLV = true;}
      else if(rXdTbgQqrw == XPdBGIzwid){nsRSQPUxKi = true;}
      if(AwiuGudCMy == iXzPUhWcMo){nfOUiBEbiW = true;}
      if(yXSjNsoIMa == aCIBgHEPBQ){nVMrttJpMA = true;}
      if(dTzEWjQgAo == LQdjfUHzcN){natquchdAp = true;}
      while(iXzPUhWcMo == AwiuGudCMy){pNMCpYrzzq = true;}
      while(aCIBgHEPBQ == aCIBgHEPBQ){bJOeZmTusC = true;}
      while(LQdjfUHzcN == LQdjfUHzcN){lIizfjzxLw = true;}
      if(tNBYnPWcSm == true){tNBYnPWcSm = false;}
      if(YymFioOblT == true){YymFioOblT = false;}
      if(cAobjRtOiA == true){cAobjRtOiA = false;}
      if(ZWzytMAiNQ == true){ZWzytMAiNQ = false;}
      if(kbwNrNdXBq == true){kbwNrNdXBq = false;}
      if(FfMIksYZoV == true){FfMIksYZoV = false;}
      if(VIsSNBzoLV == true){VIsSNBzoLV = false;}
      if(nfOUiBEbiW == true){nfOUiBEbiW = false;}
      if(nVMrttJpMA == true){nVMrttJpMA = false;}
      if(natquchdAp == true){natquchdAp = false;}
      if(NoMfCZQrZj == true){NoMfCZQrZj = false;}
      if(oxpShAuBXT == true){oxpShAuBXT = false;}
      if(qIbKhMkXZG == true){qIbKhMkXZG = false;}
      if(TZbsIyJgoU == true){TZbsIyJgoU = false;}
      if(cRQcLqkbIu == true){cRQcLqkbIu = false;}
      if(gfGfzFMBxo == true){gfGfzFMBxo = false;}
      if(nsRSQPUxKi == true){nsRSQPUxKi = false;}
      if(pNMCpYrzzq == true){pNMCpYrzzq = false;}
      if(bJOeZmTusC == true){bJOeZmTusC = false;}
      if(lIizfjzxLw == true){lIizfjzxLw = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SALBMANDXD
{ 
  void mdJCWngyIV()
  { 
      bool aqmyOsKQMU = false;
      bool qwXQDyiUrB = false;
      bool PFtahUdsVU = false;
      bool LobcaDVdqd = false;
      bool fadFqHPJpC = false;
      bool DLppTUwLPs = false;
      bool PQtqIBTtpA = false;
      bool bFcNVoaoja = false;
      bool ETDTRAzlIF = false;
      bool GJNyOOgmwo = false;
      bool mVTpZyhcUe = false;
      bool sGaoouNZZI = false;
      bool ChkksSqezG = false;
      bool HnsunhANtT = false;
      bool RVQdLzhYJT = false;
      bool BGNmlEkiwG = false;
      bool GDdOWFNfAG = false;
      bool tAfGqfQHYs = false;
      bool jSVeFBZGIi = false;
      bool xRQmMNrCGG = false;
      string wdsaelRzqe;
      string LIjxZJHziI;
      string TNlfhfofZr;
      string DyyONWhxrB;
      string kCQBcxpZte;
      string XfkLyiqMsy;
      string wkZybNJUjD;
      string aKGgXRwjDk;
      string pOkuUWMmQd;
      string eUEsEzcRYP;
      string mUpqDMYEng;
      string lHJHHgAddF;
      string BWDFYFFNcX;
      string oeKnyUYtqH;
      string xHyjqWLxkW;
      string zymTNPQkSx;
      string GLaPrHwUJl;
      string IFmWHsYTNB;
      string tIbFXpqqZh;
      string EnasAlgiqj;
      if(wdsaelRzqe == mUpqDMYEng){aqmyOsKQMU = true;}
      else if(mUpqDMYEng == wdsaelRzqe){mVTpZyhcUe = true;}
      if(LIjxZJHziI == lHJHHgAddF){qwXQDyiUrB = true;}
      else if(lHJHHgAddF == LIjxZJHziI){sGaoouNZZI = true;}
      if(TNlfhfofZr == BWDFYFFNcX){PFtahUdsVU = true;}
      else if(BWDFYFFNcX == TNlfhfofZr){ChkksSqezG = true;}
      if(DyyONWhxrB == oeKnyUYtqH){LobcaDVdqd = true;}
      else if(oeKnyUYtqH == DyyONWhxrB){HnsunhANtT = true;}
      if(kCQBcxpZte == xHyjqWLxkW){fadFqHPJpC = true;}
      else if(xHyjqWLxkW == kCQBcxpZte){RVQdLzhYJT = true;}
      if(XfkLyiqMsy == zymTNPQkSx){DLppTUwLPs = true;}
      else if(zymTNPQkSx == XfkLyiqMsy){BGNmlEkiwG = true;}
      if(wkZybNJUjD == GLaPrHwUJl){PQtqIBTtpA = true;}
      else if(GLaPrHwUJl == wkZybNJUjD){GDdOWFNfAG = true;}
      if(aKGgXRwjDk == IFmWHsYTNB){bFcNVoaoja = true;}
      if(pOkuUWMmQd == tIbFXpqqZh){ETDTRAzlIF = true;}
      if(eUEsEzcRYP == EnasAlgiqj){GJNyOOgmwo = true;}
      while(IFmWHsYTNB == aKGgXRwjDk){tAfGqfQHYs = true;}
      while(tIbFXpqqZh == tIbFXpqqZh){jSVeFBZGIi = true;}
      while(EnasAlgiqj == EnasAlgiqj){xRQmMNrCGG = true;}
      if(aqmyOsKQMU == true){aqmyOsKQMU = false;}
      if(qwXQDyiUrB == true){qwXQDyiUrB = false;}
      if(PFtahUdsVU == true){PFtahUdsVU = false;}
      if(LobcaDVdqd == true){LobcaDVdqd = false;}
      if(fadFqHPJpC == true){fadFqHPJpC = false;}
      if(DLppTUwLPs == true){DLppTUwLPs = false;}
      if(PQtqIBTtpA == true){PQtqIBTtpA = false;}
      if(bFcNVoaoja == true){bFcNVoaoja = false;}
      if(ETDTRAzlIF == true){ETDTRAzlIF = false;}
      if(GJNyOOgmwo == true){GJNyOOgmwo = false;}
      if(mVTpZyhcUe == true){mVTpZyhcUe = false;}
      if(sGaoouNZZI == true){sGaoouNZZI = false;}
      if(ChkksSqezG == true){ChkksSqezG = false;}
      if(HnsunhANtT == true){HnsunhANtT = false;}
      if(RVQdLzhYJT == true){RVQdLzhYJT = false;}
      if(BGNmlEkiwG == true){BGNmlEkiwG = false;}
      if(GDdOWFNfAG == true){GDdOWFNfAG = false;}
      if(tAfGqfQHYs == true){tAfGqfQHYs = false;}
      if(jSVeFBZGIi == true){jSVeFBZGIi = false;}
      if(xRQmMNrCGG == true){xRQmMNrCGG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DKQIXODQXM
{ 
  void VkpobjrirV()
  { 
      bool CExwNNkPxD = false;
      bool kAisttYELZ = false;
      bool XqbGtzLrVg = false;
      bool xWzJeJnGFU = false;
      bool CCyOgAfqPV = false;
      bool hLfzFLcpCN = false;
      bool BrtQFNAYNj = false;
      bool GRzpXVrNYs = false;
      bool TUcAylpzET = false;
      bool gxEqTJIgIT = false;
      bool AGdeOPLpbi = false;
      bool SORItgPsRT = false;
      bool XBUtDJNIWO = false;
      bool NLEFsMqyki = false;
      bool ZAXUBRfuxl = false;
      bool tOOnuykiaF = false;
      bool CDTMUGJLOg = false;
      bool pQIitXcAsl = false;
      bool cRKjTzwPJt = false;
      bool bHWegVfKzt = false;
      string neJSRnJBXS;
      string zJfousFDED;
      string sSXdDjcouB;
      string oqfsBPFftk;
      string oMKkbgYPdI;
      string OgkDTUGePg;
      string LTGWdIdlmf;
      string mZITOfImCV;
      string LMKmlHFbZf;
      string IaQNZUGmGT;
      string ClcyUoXRom;
      string LSbsUyJTjV;
      string CCMGWMIutt;
      string ZVfiTZjMRx;
      string inmmToyWra;
      string KmDkQAYVaI;
      string fcXtMpmsZu;
      string NxBmmNOYmh;
      string OkDSimRuRS;
      string AOkJQGQrZc;
      if(neJSRnJBXS == ClcyUoXRom){CExwNNkPxD = true;}
      else if(ClcyUoXRom == neJSRnJBXS){AGdeOPLpbi = true;}
      if(zJfousFDED == LSbsUyJTjV){kAisttYELZ = true;}
      else if(LSbsUyJTjV == zJfousFDED){SORItgPsRT = true;}
      if(sSXdDjcouB == CCMGWMIutt){XqbGtzLrVg = true;}
      else if(CCMGWMIutt == sSXdDjcouB){XBUtDJNIWO = true;}
      if(oqfsBPFftk == ZVfiTZjMRx){xWzJeJnGFU = true;}
      else if(ZVfiTZjMRx == oqfsBPFftk){NLEFsMqyki = true;}
      if(oMKkbgYPdI == inmmToyWra){CCyOgAfqPV = true;}
      else if(inmmToyWra == oMKkbgYPdI){ZAXUBRfuxl = true;}
      if(OgkDTUGePg == KmDkQAYVaI){hLfzFLcpCN = true;}
      else if(KmDkQAYVaI == OgkDTUGePg){tOOnuykiaF = true;}
      if(LTGWdIdlmf == fcXtMpmsZu){BrtQFNAYNj = true;}
      else if(fcXtMpmsZu == LTGWdIdlmf){CDTMUGJLOg = true;}
      if(mZITOfImCV == NxBmmNOYmh){GRzpXVrNYs = true;}
      if(LMKmlHFbZf == OkDSimRuRS){TUcAylpzET = true;}
      if(IaQNZUGmGT == AOkJQGQrZc){gxEqTJIgIT = true;}
      while(NxBmmNOYmh == mZITOfImCV){pQIitXcAsl = true;}
      while(OkDSimRuRS == OkDSimRuRS){cRKjTzwPJt = true;}
      while(AOkJQGQrZc == AOkJQGQrZc){bHWegVfKzt = true;}
      if(CExwNNkPxD == true){CExwNNkPxD = false;}
      if(kAisttYELZ == true){kAisttYELZ = false;}
      if(XqbGtzLrVg == true){XqbGtzLrVg = false;}
      if(xWzJeJnGFU == true){xWzJeJnGFU = false;}
      if(CCyOgAfqPV == true){CCyOgAfqPV = false;}
      if(hLfzFLcpCN == true){hLfzFLcpCN = false;}
      if(BrtQFNAYNj == true){BrtQFNAYNj = false;}
      if(GRzpXVrNYs == true){GRzpXVrNYs = false;}
      if(TUcAylpzET == true){TUcAylpzET = false;}
      if(gxEqTJIgIT == true){gxEqTJIgIT = false;}
      if(AGdeOPLpbi == true){AGdeOPLpbi = false;}
      if(SORItgPsRT == true){SORItgPsRT = false;}
      if(XBUtDJNIWO == true){XBUtDJNIWO = false;}
      if(NLEFsMqyki == true){NLEFsMqyki = false;}
      if(ZAXUBRfuxl == true){ZAXUBRfuxl = false;}
      if(tOOnuykiaF == true){tOOnuykiaF = false;}
      if(CDTMUGJLOg == true){CDTMUGJLOg = false;}
      if(pQIitXcAsl == true){pQIitXcAsl = false;}
      if(cRKjTzwPJt == true){cRKjTzwPJt = false;}
      if(bHWegVfKzt == true){bHWegVfKzt = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TOONNJCLEE
{ 
  void LtYexZgKSG()
  { 
      bool gEurmAPlpj = false;
      bool XnoCSiMYgn = false;
      bool PkmZQPkgtG = false;
      bool IInNfslnbX = false;
      bool uPolWGnhkD = false;
      bool cFbBnwRUbX = false;
      bool PiqiaOzbVY = false;
      bool wXwJsAwBDJ = false;
      bool bCupdDglZj = false;
      bool bRLEILXGKH = false;
      bool HMLLHEIRIn = false;
      bool zPRSuMmBEX = false;
      bool gMBkweLrBg = false;
      bool bpquCOpiiZ = false;
      bool tPdkjMzsIu = false;
      bool XWBlZtKZPb = false;
      bool rQQbLJYVQg = false;
      bool VERddRiPBj = false;
      bool eSjFIEnEUH = false;
      bool EQTGgcyCZa = false;
      string kQkhZPxSCC;
      string tqxKrYGMNf;
      string hbxJUDMxLB;
      string HmzdRzskyK;
      string rODMkNEkUK;
      string jslldJSbNW;
      string DzHBtHrSsr;
      string eVNCmSXjXG;
      string yacAQOBMzH;
      string gRWbHsHCRd;
      string VkrbeOeizK;
      string liDGXbWGjf;
      string FeTlLNykut;
      string LURtkMiAVC;
      string iSNoUyOQDf;
      string VASlXpRFPR;
      string PtnEzRQJVK;
      string rVkgwmRdQN;
      string yggOXKBeVu;
      string tckDlodIpD;
      if(kQkhZPxSCC == VkrbeOeizK){gEurmAPlpj = true;}
      else if(VkrbeOeizK == kQkhZPxSCC){HMLLHEIRIn = true;}
      if(tqxKrYGMNf == liDGXbWGjf){XnoCSiMYgn = true;}
      else if(liDGXbWGjf == tqxKrYGMNf){zPRSuMmBEX = true;}
      if(hbxJUDMxLB == FeTlLNykut){PkmZQPkgtG = true;}
      else if(FeTlLNykut == hbxJUDMxLB){gMBkweLrBg = true;}
      if(HmzdRzskyK == LURtkMiAVC){IInNfslnbX = true;}
      else if(LURtkMiAVC == HmzdRzskyK){bpquCOpiiZ = true;}
      if(rODMkNEkUK == iSNoUyOQDf){uPolWGnhkD = true;}
      else if(iSNoUyOQDf == rODMkNEkUK){tPdkjMzsIu = true;}
      if(jslldJSbNW == VASlXpRFPR){cFbBnwRUbX = true;}
      else if(VASlXpRFPR == jslldJSbNW){XWBlZtKZPb = true;}
      if(DzHBtHrSsr == PtnEzRQJVK){PiqiaOzbVY = true;}
      else if(PtnEzRQJVK == DzHBtHrSsr){rQQbLJYVQg = true;}
      if(eVNCmSXjXG == rVkgwmRdQN){wXwJsAwBDJ = true;}
      if(yacAQOBMzH == yggOXKBeVu){bCupdDglZj = true;}
      if(gRWbHsHCRd == tckDlodIpD){bRLEILXGKH = true;}
      while(rVkgwmRdQN == eVNCmSXjXG){VERddRiPBj = true;}
      while(yggOXKBeVu == yggOXKBeVu){eSjFIEnEUH = true;}
      while(tckDlodIpD == tckDlodIpD){EQTGgcyCZa = true;}
      if(gEurmAPlpj == true){gEurmAPlpj = false;}
      if(XnoCSiMYgn == true){XnoCSiMYgn = false;}
      if(PkmZQPkgtG == true){PkmZQPkgtG = false;}
      if(IInNfslnbX == true){IInNfslnbX = false;}
      if(uPolWGnhkD == true){uPolWGnhkD = false;}
      if(cFbBnwRUbX == true){cFbBnwRUbX = false;}
      if(PiqiaOzbVY == true){PiqiaOzbVY = false;}
      if(wXwJsAwBDJ == true){wXwJsAwBDJ = false;}
      if(bCupdDglZj == true){bCupdDglZj = false;}
      if(bRLEILXGKH == true){bRLEILXGKH = false;}
      if(HMLLHEIRIn == true){HMLLHEIRIn = false;}
      if(zPRSuMmBEX == true){zPRSuMmBEX = false;}
      if(gMBkweLrBg == true){gMBkweLrBg = false;}
      if(bpquCOpiiZ == true){bpquCOpiiZ = false;}
      if(tPdkjMzsIu == true){tPdkjMzsIu = false;}
      if(XWBlZtKZPb == true){XWBlZtKZPb = false;}
      if(rQQbLJYVQg == true){rQQbLJYVQg = false;}
      if(VERddRiPBj == true){VERddRiPBj = false;}
      if(eSjFIEnEUH == true){eSjFIEnEUH = false;}
      if(EQTGgcyCZa == true){EQTGgcyCZa = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PKZXXEARQV
{ 
  void CImyqYtizu()
  { 
      bool qcQJjglHLe = false;
      bool bSHdHSNPyG = false;
      bool BmLshRGEFg = false;
      bool RyPdmdboGX = false;
      bool eTCKzExcRe = false;
      bool zAJdTERXaR = false;
      bool ZgBeMdDcqd = false;
      bool pYIZahkAsd = false;
      bool UcmagQnlKi = false;
      bool LEOnyxRXyU = false;
      bool BTlnnaqjCk = false;
      bool TRdwxwtaxu = false;
      bool pQSHRlJUbc = false;
      bool PMCYcApzlF = false;
      bool ZDiYPtZRqB = false;
      bool DpZCdjBeVC = false;
      bool QWIYUGOFTx = false;
      bool FuoqVmhsqW = false;
      bool LrcZZZlztr = false;
      bool yLcinCtJEJ = false;
      string LJrjhZHTET;
      string CYMoZdHAch;
      string IppSrSbZLq;
      string zVrnfiIJan;
      string HqiuygAMZW;
      string pzsdXUSYqO;
      string ygHMUgfYIp;
      string VyYnaRHuUL;
      string SPAoBAbOPL;
      string QiRLpaiVaR;
      string AbDiBrZFDG;
      string NWEfwhfSLU;
      string ERIxYlfNyz;
      string hkUCKpLRnJ;
      string ZWLIxtelOH;
      string uQJGMiSGJO;
      string LMGDXcCBUF;
      string WOdCthGAze;
      string HsyqERaOpb;
      string WEHYxNPIjU;
      if(LJrjhZHTET == AbDiBrZFDG){qcQJjglHLe = true;}
      else if(AbDiBrZFDG == LJrjhZHTET){BTlnnaqjCk = true;}
      if(CYMoZdHAch == NWEfwhfSLU){bSHdHSNPyG = true;}
      else if(NWEfwhfSLU == CYMoZdHAch){TRdwxwtaxu = true;}
      if(IppSrSbZLq == ERIxYlfNyz){BmLshRGEFg = true;}
      else if(ERIxYlfNyz == IppSrSbZLq){pQSHRlJUbc = true;}
      if(zVrnfiIJan == hkUCKpLRnJ){RyPdmdboGX = true;}
      else if(hkUCKpLRnJ == zVrnfiIJan){PMCYcApzlF = true;}
      if(HqiuygAMZW == ZWLIxtelOH){eTCKzExcRe = true;}
      else if(ZWLIxtelOH == HqiuygAMZW){ZDiYPtZRqB = true;}
      if(pzsdXUSYqO == uQJGMiSGJO){zAJdTERXaR = true;}
      else if(uQJGMiSGJO == pzsdXUSYqO){DpZCdjBeVC = true;}
      if(ygHMUgfYIp == LMGDXcCBUF){ZgBeMdDcqd = true;}
      else if(LMGDXcCBUF == ygHMUgfYIp){QWIYUGOFTx = true;}
      if(VyYnaRHuUL == WOdCthGAze){pYIZahkAsd = true;}
      if(SPAoBAbOPL == HsyqERaOpb){UcmagQnlKi = true;}
      if(QiRLpaiVaR == WEHYxNPIjU){LEOnyxRXyU = true;}
      while(WOdCthGAze == VyYnaRHuUL){FuoqVmhsqW = true;}
      while(HsyqERaOpb == HsyqERaOpb){LrcZZZlztr = true;}
      while(WEHYxNPIjU == WEHYxNPIjU){yLcinCtJEJ = true;}
      if(qcQJjglHLe == true){qcQJjglHLe = false;}
      if(bSHdHSNPyG == true){bSHdHSNPyG = false;}
      if(BmLshRGEFg == true){BmLshRGEFg = false;}
      if(RyPdmdboGX == true){RyPdmdboGX = false;}
      if(eTCKzExcRe == true){eTCKzExcRe = false;}
      if(zAJdTERXaR == true){zAJdTERXaR = false;}
      if(ZgBeMdDcqd == true){ZgBeMdDcqd = false;}
      if(pYIZahkAsd == true){pYIZahkAsd = false;}
      if(UcmagQnlKi == true){UcmagQnlKi = false;}
      if(LEOnyxRXyU == true){LEOnyxRXyU = false;}
      if(BTlnnaqjCk == true){BTlnnaqjCk = false;}
      if(TRdwxwtaxu == true){TRdwxwtaxu = false;}
      if(pQSHRlJUbc == true){pQSHRlJUbc = false;}
      if(PMCYcApzlF == true){PMCYcApzlF = false;}
      if(ZDiYPtZRqB == true){ZDiYPtZRqB = false;}
      if(DpZCdjBeVC == true){DpZCdjBeVC = false;}
      if(QWIYUGOFTx == true){QWIYUGOFTx = false;}
      if(FuoqVmhsqW == true){FuoqVmhsqW = false;}
      if(LrcZZZlztr == true){LrcZZZlztr = false;}
      if(yLcinCtJEJ == true){yLcinCtJEJ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MLXRKAAKCJ
{ 
  void pgAFjfutwy()
  { 
      bool fzqPVzzztm = false;
      bool lAIDGDfKYz = false;
      bool HDmcrRQcbA = false;
      bool ZDhHptOuUR = false;
      bool yWfQRUHCgG = false;
      bool RJdrHcLNXD = false;
      bool hWWsNiSAML = false;
      bool okoGAkWWLX = false;
      bool KOuViQZozN = false;
      bool hAXRjbzmCi = false;
      bool BGpUQgTXrW = false;
      bool GoCzsTaBKx = false;
      bool JdBydoMXWX = false;
      bool kjTcWDXaEC = false;
      bool wBxcHsxYXP = false;
      bool BybFZXFqpb = false;
      bool TuATkVUZco = false;
      bool uzwIAHgXqk = false;
      bool qnzGWmHOCz = false;
      bool yfjchBBcqk = false;
      string rBfZnsblqD;
      string glIKyjEPac;
      string XGATFqbkLc;
      string nuOrQIfWky;
      string PrGyjPFnxn;
      string UNZqqlAMTw;
      string RpZNPlWPsp;
      string EgebMfACjp;
      string lnmXHZUWBd;
      string ITCngYRriy;
      string rotHjniydL;
      string lxcdKcCCGo;
      string QzOOgjdpun;
      string NHTkxwtfmT;
      string pWsorKqYdE;
      string VPlsgsaeZz;
      string JrCpxddqgN;
      string hWPlytGZWW;
      string sFLqRyVaDD;
      string VPFmKyUCqO;
      if(rBfZnsblqD == rotHjniydL){fzqPVzzztm = true;}
      else if(rotHjniydL == rBfZnsblqD){BGpUQgTXrW = true;}
      if(glIKyjEPac == lxcdKcCCGo){lAIDGDfKYz = true;}
      else if(lxcdKcCCGo == glIKyjEPac){GoCzsTaBKx = true;}
      if(XGATFqbkLc == QzOOgjdpun){HDmcrRQcbA = true;}
      else if(QzOOgjdpun == XGATFqbkLc){JdBydoMXWX = true;}
      if(nuOrQIfWky == NHTkxwtfmT){ZDhHptOuUR = true;}
      else if(NHTkxwtfmT == nuOrQIfWky){kjTcWDXaEC = true;}
      if(PrGyjPFnxn == pWsorKqYdE){yWfQRUHCgG = true;}
      else if(pWsorKqYdE == PrGyjPFnxn){wBxcHsxYXP = true;}
      if(UNZqqlAMTw == VPlsgsaeZz){RJdrHcLNXD = true;}
      else if(VPlsgsaeZz == UNZqqlAMTw){BybFZXFqpb = true;}
      if(RpZNPlWPsp == JrCpxddqgN){hWWsNiSAML = true;}
      else if(JrCpxddqgN == RpZNPlWPsp){TuATkVUZco = true;}
      if(EgebMfACjp == hWPlytGZWW){okoGAkWWLX = true;}
      if(lnmXHZUWBd == sFLqRyVaDD){KOuViQZozN = true;}
      if(ITCngYRriy == VPFmKyUCqO){hAXRjbzmCi = true;}
      while(hWPlytGZWW == EgebMfACjp){uzwIAHgXqk = true;}
      while(sFLqRyVaDD == sFLqRyVaDD){qnzGWmHOCz = true;}
      while(VPFmKyUCqO == VPFmKyUCqO){yfjchBBcqk = true;}
      if(fzqPVzzztm == true){fzqPVzzztm = false;}
      if(lAIDGDfKYz == true){lAIDGDfKYz = false;}
      if(HDmcrRQcbA == true){HDmcrRQcbA = false;}
      if(ZDhHptOuUR == true){ZDhHptOuUR = false;}
      if(yWfQRUHCgG == true){yWfQRUHCgG = false;}
      if(RJdrHcLNXD == true){RJdrHcLNXD = false;}
      if(hWWsNiSAML == true){hWWsNiSAML = false;}
      if(okoGAkWWLX == true){okoGAkWWLX = false;}
      if(KOuViQZozN == true){KOuViQZozN = false;}
      if(hAXRjbzmCi == true){hAXRjbzmCi = false;}
      if(BGpUQgTXrW == true){BGpUQgTXrW = false;}
      if(GoCzsTaBKx == true){GoCzsTaBKx = false;}
      if(JdBydoMXWX == true){JdBydoMXWX = false;}
      if(kjTcWDXaEC == true){kjTcWDXaEC = false;}
      if(wBxcHsxYXP == true){wBxcHsxYXP = false;}
      if(BybFZXFqpb == true){BybFZXFqpb = false;}
      if(TuATkVUZco == true){TuATkVUZco = false;}
      if(uzwIAHgXqk == true){uzwIAHgXqk = false;}
      if(qnzGWmHOCz == true){qnzGWmHOCz = false;}
      if(yfjchBBcqk == true){yfjchBBcqk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LBJJWLUALR
{ 
  void tjTtXVlPjm()
  { 
      bool iVFJiEuTMj = false;
      bool zPCsJlBqun = false;
      bool xjaOdXRsua = false;
      bool SLPksSWXKb = false;
      bool nmgOHZyfhI = false;
      bool HUCTJyupfK = false;
      bool mCXkQnAQzq = false;
      bool PdgSAKCMHD = false;
      bool YlLmTyIwbt = false;
      bool XfqzokOuxT = false;
      bool YhfqsdmiSy = false;
      bool whibkBqkRS = false;
      bool eDnFSilgdO = false;
      bool uGPneqFUtp = false;
      bool KZcdyoxPNn = false;
      bool tddenUtaXJ = false;
      bool rFFEDpSuxt = false;
      bool uWpbIFaKGc = false;
      bool lVTpUQbjzJ = false;
      bool wewjxEUXZI = false;
      string CADNMxiiek;
      string TOORztZhZk;
      string mGKLSrgqeG;
      string TFymeZaIut;
      string ZMfqGJUlkD;
      string hgTQPworuU;
      string QrDXQOQblZ;
      string TQlfKZZSFI;
      string WuFaYmkgWh;
      string jiNadhJIar;
      string rszKjQtmGp;
      string VWmlxUlbyc;
      string cjEEokDLCc;
      string QRrMPdwtqI;
      string rxagLNuacI;
      string ugJxOIntVz;
      string NikcYEeZVT;
      string YqkpjeJXrb;
      string obhsgjHtyH;
      string pBYrtJMxIG;
      if(CADNMxiiek == rszKjQtmGp){iVFJiEuTMj = true;}
      else if(rszKjQtmGp == CADNMxiiek){YhfqsdmiSy = true;}
      if(TOORztZhZk == VWmlxUlbyc){zPCsJlBqun = true;}
      else if(VWmlxUlbyc == TOORztZhZk){whibkBqkRS = true;}
      if(mGKLSrgqeG == cjEEokDLCc){xjaOdXRsua = true;}
      else if(cjEEokDLCc == mGKLSrgqeG){eDnFSilgdO = true;}
      if(TFymeZaIut == QRrMPdwtqI){SLPksSWXKb = true;}
      else if(QRrMPdwtqI == TFymeZaIut){uGPneqFUtp = true;}
      if(ZMfqGJUlkD == rxagLNuacI){nmgOHZyfhI = true;}
      else if(rxagLNuacI == ZMfqGJUlkD){KZcdyoxPNn = true;}
      if(hgTQPworuU == ugJxOIntVz){HUCTJyupfK = true;}
      else if(ugJxOIntVz == hgTQPworuU){tddenUtaXJ = true;}
      if(QrDXQOQblZ == NikcYEeZVT){mCXkQnAQzq = true;}
      else if(NikcYEeZVT == QrDXQOQblZ){rFFEDpSuxt = true;}
      if(TQlfKZZSFI == YqkpjeJXrb){PdgSAKCMHD = true;}
      if(WuFaYmkgWh == obhsgjHtyH){YlLmTyIwbt = true;}
      if(jiNadhJIar == pBYrtJMxIG){XfqzokOuxT = true;}
      while(YqkpjeJXrb == TQlfKZZSFI){uWpbIFaKGc = true;}
      while(obhsgjHtyH == obhsgjHtyH){lVTpUQbjzJ = true;}
      while(pBYrtJMxIG == pBYrtJMxIG){wewjxEUXZI = true;}
      if(iVFJiEuTMj == true){iVFJiEuTMj = false;}
      if(zPCsJlBqun == true){zPCsJlBqun = false;}
      if(xjaOdXRsua == true){xjaOdXRsua = false;}
      if(SLPksSWXKb == true){SLPksSWXKb = false;}
      if(nmgOHZyfhI == true){nmgOHZyfhI = false;}
      if(HUCTJyupfK == true){HUCTJyupfK = false;}
      if(mCXkQnAQzq == true){mCXkQnAQzq = false;}
      if(PdgSAKCMHD == true){PdgSAKCMHD = false;}
      if(YlLmTyIwbt == true){YlLmTyIwbt = false;}
      if(XfqzokOuxT == true){XfqzokOuxT = false;}
      if(YhfqsdmiSy == true){YhfqsdmiSy = false;}
      if(whibkBqkRS == true){whibkBqkRS = false;}
      if(eDnFSilgdO == true){eDnFSilgdO = false;}
      if(uGPneqFUtp == true){uGPneqFUtp = false;}
      if(KZcdyoxPNn == true){KZcdyoxPNn = false;}
      if(tddenUtaXJ == true){tddenUtaXJ = false;}
      if(rFFEDpSuxt == true){rFFEDpSuxt = false;}
      if(uWpbIFaKGc == true){uWpbIFaKGc = false;}
      if(lVTpUQbjzJ == true){lVTpUQbjzJ = false;}
      if(wewjxEUXZI == true){wewjxEUXZI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TDRJGQAENX
{ 
  void deHzbgDBfo()
  { 
      bool HTOGXlFCTK = false;
      bool BNRjYDQTIa = false;
      bool iBJjXegyLq = false;
      bool hBhAFdNXBj = false;
      bool fXXefJDVES = false;
      bool LxcIWFJdir = false;
      bool EHpcbAfQet = false;
      bool orSYuKdeDN = false;
      bool bLsrekXZdA = false;
      bool UjoIeYgtDf = false;
      bool dlqJdTeENL = false;
      bool adZUPsNxyq = false;
      bool bGldZFDJtV = false;
      bool cibWjoTjOL = false;
      bool FTLRYpJTrk = false;
      bool LiwLEXAMjY = false;
      bool PUKqiqCCka = false;
      bool YdimkItUFj = false;
      bool wiQIKsmPSL = false;
      bool oGzpukJUig = false;
      string feTFpTUSjL;
      string ObhtDTCnNm;
      string zcqzComOFJ;
      string PEgomOkXDy;
      string YWwcemoAzT;
      string SNEoKCnTDA;
      string gGMauKusGn;
      string hdjOOcgNUO;
      string AmOkpCzqdk;
      string aRUDRRJUhU;
      string rusKlncoag;
      string VZznAsrajs;
      string KDOVNPIgww;
      string TLfzVEsDzB;
      string YKQxOASfCX;
      string ZhknKnfXxV;
      string fureNJszRw;
      string AnbhTAmRGM;
      string xbeYVlaqoh;
      string XUGcVcRljF;
      if(feTFpTUSjL == rusKlncoag){HTOGXlFCTK = true;}
      else if(rusKlncoag == feTFpTUSjL){dlqJdTeENL = true;}
      if(ObhtDTCnNm == VZznAsrajs){BNRjYDQTIa = true;}
      else if(VZznAsrajs == ObhtDTCnNm){adZUPsNxyq = true;}
      if(zcqzComOFJ == KDOVNPIgww){iBJjXegyLq = true;}
      else if(KDOVNPIgww == zcqzComOFJ){bGldZFDJtV = true;}
      if(PEgomOkXDy == TLfzVEsDzB){hBhAFdNXBj = true;}
      else if(TLfzVEsDzB == PEgomOkXDy){cibWjoTjOL = true;}
      if(YWwcemoAzT == YKQxOASfCX){fXXefJDVES = true;}
      else if(YKQxOASfCX == YWwcemoAzT){FTLRYpJTrk = true;}
      if(SNEoKCnTDA == ZhknKnfXxV){LxcIWFJdir = true;}
      else if(ZhknKnfXxV == SNEoKCnTDA){LiwLEXAMjY = true;}
      if(gGMauKusGn == fureNJszRw){EHpcbAfQet = true;}
      else if(fureNJszRw == gGMauKusGn){PUKqiqCCka = true;}
      if(hdjOOcgNUO == AnbhTAmRGM){orSYuKdeDN = true;}
      if(AmOkpCzqdk == xbeYVlaqoh){bLsrekXZdA = true;}
      if(aRUDRRJUhU == XUGcVcRljF){UjoIeYgtDf = true;}
      while(AnbhTAmRGM == hdjOOcgNUO){YdimkItUFj = true;}
      while(xbeYVlaqoh == xbeYVlaqoh){wiQIKsmPSL = true;}
      while(XUGcVcRljF == XUGcVcRljF){oGzpukJUig = true;}
      if(HTOGXlFCTK == true){HTOGXlFCTK = false;}
      if(BNRjYDQTIa == true){BNRjYDQTIa = false;}
      if(iBJjXegyLq == true){iBJjXegyLq = false;}
      if(hBhAFdNXBj == true){hBhAFdNXBj = false;}
      if(fXXefJDVES == true){fXXefJDVES = false;}
      if(LxcIWFJdir == true){LxcIWFJdir = false;}
      if(EHpcbAfQet == true){EHpcbAfQet = false;}
      if(orSYuKdeDN == true){orSYuKdeDN = false;}
      if(bLsrekXZdA == true){bLsrekXZdA = false;}
      if(UjoIeYgtDf == true){UjoIeYgtDf = false;}
      if(dlqJdTeENL == true){dlqJdTeENL = false;}
      if(adZUPsNxyq == true){adZUPsNxyq = false;}
      if(bGldZFDJtV == true){bGldZFDJtV = false;}
      if(cibWjoTjOL == true){cibWjoTjOL = false;}
      if(FTLRYpJTrk == true){FTLRYpJTrk = false;}
      if(LiwLEXAMjY == true){LiwLEXAMjY = false;}
      if(PUKqiqCCka == true){PUKqiqCCka = false;}
      if(YdimkItUFj == true){YdimkItUFj = false;}
      if(wiQIKsmPSL == true){wiQIKsmPSL = false;}
      if(oGzpukJUig == true){oGzpukJUig = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JCQDWXLVGM
{ 
  void aLeNuqDkRL()
  { 
      bool EzWcJKoPgz = false;
      bool lxEPhMCakd = false;
      bool yhjMEubZXe = false;
      bool WwyrwTsxEu = false;
      bool gccwVnBZUR = false;
      bool sJCOAVPKYN = false;
      bool iOqBcozMSO = false;
      bool SEVowVFmpP = false;
      bool sBEGkMtiqZ = false;
      bool EiXqMSkbSf = false;
      bool ooGGyonMRe = false;
      bool QcTJTgKkQP = false;
      bool tPUrKifdcd = false;
      bool jdTKaDuxHw = false;
      bool YgRLJaPOXe = false;
      bool HALDkYUNwO = false;
      bool IJimumIJhP = false;
      bool kMdTqNyIii = false;
      bool TRGxViSQYB = false;
      bool igArnOxdQW = false;
      string BIPYGYaQZl;
      string HVlKtwuZnS;
      string nNZsOrnkXY;
      string roFRhpjzxz;
      string mwbBEAsHRO;
      string fyCDhniWKO;
      string josBtSQTcE;
      string MubnerkYnN;
      string ygSIrtmCit;
      string GNniOyCaHl;
      string jYeNCkVNXp;
      string CJhxPDVjJz;
      string fNjixzHsYg;
      string oKRCcZBFGH;
      string YYdsqwepSK;
      string bSRHGVnuPC;
      string ccnpArrfTp;
      string BQdBaGjqqA;
      string TQZNFWsJuH;
      string lGoDdwjmrz;
      if(BIPYGYaQZl == jYeNCkVNXp){EzWcJKoPgz = true;}
      else if(jYeNCkVNXp == BIPYGYaQZl){ooGGyonMRe = true;}
      if(HVlKtwuZnS == CJhxPDVjJz){lxEPhMCakd = true;}
      else if(CJhxPDVjJz == HVlKtwuZnS){QcTJTgKkQP = true;}
      if(nNZsOrnkXY == fNjixzHsYg){yhjMEubZXe = true;}
      else if(fNjixzHsYg == nNZsOrnkXY){tPUrKifdcd = true;}
      if(roFRhpjzxz == oKRCcZBFGH){WwyrwTsxEu = true;}
      else if(oKRCcZBFGH == roFRhpjzxz){jdTKaDuxHw = true;}
      if(mwbBEAsHRO == YYdsqwepSK){gccwVnBZUR = true;}
      else if(YYdsqwepSK == mwbBEAsHRO){YgRLJaPOXe = true;}
      if(fyCDhniWKO == bSRHGVnuPC){sJCOAVPKYN = true;}
      else if(bSRHGVnuPC == fyCDhniWKO){HALDkYUNwO = true;}
      if(josBtSQTcE == ccnpArrfTp){iOqBcozMSO = true;}
      else if(ccnpArrfTp == josBtSQTcE){IJimumIJhP = true;}
      if(MubnerkYnN == BQdBaGjqqA){SEVowVFmpP = true;}
      if(ygSIrtmCit == TQZNFWsJuH){sBEGkMtiqZ = true;}
      if(GNniOyCaHl == lGoDdwjmrz){EiXqMSkbSf = true;}
      while(BQdBaGjqqA == MubnerkYnN){kMdTqNyIii = true;}
      while(TQZNFWsJuH == TQZNFWsJuH){TRGxViSQYB = true;}
      while(lGoDdwjmrz == lGoDdwjmrz){igArnOxdQW = true;}
      if(EzWcJKoPgz == true){EzWcJKoPgz = false;}
      if(lxEPhMCakd == true){lxEPhMCakd = false;}
      if(yhjMEubZXe == true){yhjMEubZXe = false;}
      if(WwyrwTsxEu == true){WwyrwTsxEu = false;}
      if(gccwVnBZUR == true){gccwVnBZUR = false;}
      if(sJCOAVPKYN == true){sJCOAVPKYN = false;}
      if(iOqBcozMSO == true){iOqBcozMSO = false;}
      if(SEVowVFmpP == true){SEVowVFmpP = false;}
      if(sBEGkMtiqZ == true){sBEGkMtiqZ = false;}
      if(EiXqMSkbSf == true){EiXqMSkbSf = false;}
      if(ooGGyonMRe == true){ooGGyonMRe = false;}
      if(QcTJTgKkQP == true){QcTJTgKkQP = false;}
      if(tPUrKifdcd == true){tPUrKifdcd = false;}
      if(jdTKaDuxHw == true){jdTKaDuxHw = false;}
      if(YgRLJaPOXe == true){YgRLJaPOXe = false;}
      if(HALDkYUNwO == true){HALDkYUNwO = false;}
      if(IJimumIJhP == true){IJimumIJhP = false;}
      if(kMdTqNyIii == true){kMdTqNyIii = false;}
      if(TRGxViSQYB == true){TRGxViSQYB = false;}
      if(igArnOxdQW == true){igArnOxdQW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OYRAOSGIPD
{ 
  void XZxYDnRxak()
  { 
      bool RRnnnxgOmf = false;
      bool wpRhdbfMAf = false;
      bool XZmJDcVkOX = false;
      bool mEHNpIhORg = false;
      bool TZdlRqTUeM = false;
      bool ZiQnFqddjK = false;
      bool bwAPsrhDqV = false;
      bool sQuTMywArL = false;
      bool JBFBxDmTIz = false;
      bool dDdGEWZNsZ = false;
      bool IPcISImxbH = false;
      bool udlYAZqySo = false;
      bool EOurrpmnOp = false;
      bool YhHRCtXxin = false;
      bool PprBEARFGd = false;
      bool fauDVtTkQb = false;
      bool VzhkhMVTVG = false;
      bool CtOdYNWtnr = false;
      bool YhhjKxlDwI = false;
      bool ZmfGRQIGSq = false;
      string xGXTWGgkGK;
      string XpXTbrofZx;
      string MVPrwmMMRz;
      string tjwggZmotF;
      string IBLzzenbKE;
      string gwuTUaqeWj;
      string sTTGPmtqmB;
      string EAlTnKKQsR;
      string yjCMSyobMG;
      string iIRKLnhFiM;
      string kxoIAfJaeJ;
      string QZlFkJRGnR;
      string xRefRwibIA;
      string cEhggAtsMl;
      string nOwCoJWMYC;
      string VaQQOSSIpD;
      string xhSegLqEXk;
      string OYsCdiVcqI;
      string ZVCFdBnQRO;
      string hkCInFwRUc;
      if(xGXTWGgkGK == kxoIAfJaeJ){RRnnnxgOmf = true;}
      else if(kxoIAfJaeJ == xGXTWGgkGK){IPcISImxbH = true;}
      if(XpXTbrofZx == QZlFkJRGnR){wpRhdbfMAf = true;}
      else if(QZlFkJRGnR == XpXTbrofZx){udlYAZqySo = true;}
      if(MVPrwmMMRz == xRefRwibIA){XZmJDcVkOX = true;}
      else if(xRefRwibIA == MVPrwmMMRz){EOurrpmnOp = true;}
      if(tjwggZmotF == cEhggAtsMl){mEHNpIhORg = true;}
      else if(cEhggAtsMl == tjwggZmotF){YhHRCtXxin = true;}
      if(IBLzzenbKE == nOwCoJWMYC){TZdlRqTUeM = true;}
      else if(nOwCoJWMYC == IBLzzenbKE){PprBEARFGd = true;}
      if(gwuTUaqeWj == VaQQOSSIpD){ZiQnFqddjK = true;}
      else if(VaQQOSSIpD == gwuTUaqeWj){fauDVtTkQb = true;}
      if(sTTGPmtqmB == xhSegLqEXk){bwAPsrhDqV = true;}
      else if(xhSegLqEXk == sTTGPmtqmB){VzhkhMVTVG = true;}
      if(EAlTnKKQsR == OYsCdiVcqI){sQuTMywArL = true;}
      if(yjCMSyobMG == ZVCFdBnQRO){JBFBxDmTIz = true;}
      if(iIRKLnhFiM == hkCInFwRUc){dDdGEWZNsZ = true;}
      while(OYsCdiVcqI == EAlTnKKQsR){CtOdYNWtnr = true;}
      while(ZVCFdBnQRO == ZVCFdBnQRO){YhhjKxlDwI = true;}
      while(hkCInFwRUc == hkCInFwRUc){ZmfGRQIGSq = true;}
      if(RRnnnxgOmf == true){RRnnnxgOmf = false;}
      if(wpRhdbfMAf == true){wpRhdbfMAf = false;}
      if(XZmJDcVkOX == true){XZmJDcVkOX = false;}
      if(mEHNpIhORg == true){mEHNpIhORg = false;}
      if(TZdlRqTUeM == true){TZdlRqTUeM = false;}
      if(ZiQnFqddjK == true){ZiQnFqddjK = false;}
      if(bwAPsrhDqV == true){bwAPsrhDqV = false;}
      if(sQuTMywArL == true){sQuTMywArL = false;}
      if(JBFBxDmTIz == true){JBFBxDmTIz = false;}
      if(dDdGEWZNsZ == true){dDdGEWZNsZ = false;}
      if(IPcISImxbH == true){IPcISImxbH = false;}
      if(udlYAZqySo == true){udlYAZqySo = false;}
      if(EOurrpmnOp == true){EOurrpmnOp = false;}
      if(YhHRCtXxin == true){YhHRCtXxin = false;}
      if(PprBEARFGd == true){PprBEARFGd = false;}
      if(fauDVtTkQb == true){fauDVtTkQb = false;}
      if(VzhkhMVTVG == true){VzhkhMVTVG = false;}
      if(CtOdYNWtnr == true){CtOdYNWtnr = false;}
      if(YhhjKxlDwI == true){YhhjKxlDwI = false;}
      if(ZmfGRQIGSq == true){ZmfGRQIGSq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FZUYSRAYDF
{ 
  void cTdwadhhlg()
  { 
      bool onUTBXNWwS = false;
      bool kBphPIZmgZ = false;
      bool oxAwfcoAhY = false;
      bool JZesNMyWZA = false;
      bool zPDzTiwZUa = false;
      bool cPuoKrmYmY = false;
      bool iNoXIKWxTD = false;
      bool JDNgaZsAbx = false;
      bool fOfGutdphU = false;
      bool YZiARkafqJ = false;
      bool MzGQQYBONk = false;
      bool ylworThhbO = false;
      bool ffOzdiTNOo = false;
      bool yNJSkgIOoV = false;
      bool rPcxVmcsHx = false;
      bool DsDccfkGmg = false;
      bool FnzXlkbdEL = false;
      bool LTrFVQCQcp = false;
      bool GaFRlFKgpE = false;
      bool wBaHQEPEKk = false;
      string FdygzAEjJY;
      string dEwWRhrZPa;
      string jMRWPXjtbD;
      string RakwWTAiMT;
      string WpVZNtuGYE;
      string aFCDsrLaQb;
      string QKHFHDTAAy;
      string gIMuEtpqIU;
      string cdUXcMYgCd;
      string HYSkDLhQSK;
      string atGaXRrGyz;
      string BURwzlpjbu;
      string elyVpHpbrs;
      string LZXythWsAL;
      string TRiDbmHWkQ;
      string UynXfnwjkP;
      string bQOFnbxTBR;
      string PxjQaMDttD;
      string ATpaXNlzSF;
      string klIjiCDcPA;
      if(FdygzAEjJY == atGaXRrGyz){onUTBXNWwS = true;}
      else if(atGaXRrGyz == FdygzAEjJY){MzGQQYBONk = true;}
      if(dEwWRhrZPa == BURwzlpjbu){kBphPIZmgZ = true;}
      else if(BURwzlpjbu == dEwWRhrZPa){ylworThhbO = true;}
      if(jMRWPXjtbD == elyVpHpbrs){oxAwfcoAhY = true;}
      else if(elyVpHpbrs == jMRWPXjtbD){ffOzdiTNOo = true;}
      if(RakwWTAiMT == LZXythWsAL){JZesNMyWZA = true;}
      else if(LZXythWsAL == RakwWTAiMT){yNJSkgIOoV = true;}
      if(WpVZNtuGYE == TRiDbmHWkQ){zPDzTiwZUa = true;}
      else if(TRiDbmHWkQ == WpVZNtuGYE){rPcxVmcsHx = true;}
      if(aFCDsrLaQb == UynXfnwjkP){cPuoKrmYmY = true;}
      else if(UynXfnwjkP == aFCDsrLaQb){DsDccfkGmg = true;}
      if(QKHFHDTAAy == bQOFnbxTBR){iNoXIKWxTD = true;}
      else if(bQOFnbxTBR == QKHFHDTAAy){FnzXlkbdEL = true;}
      if(gIMuEtpqIU == PxjQaMDttD){JDNgaZsAbx = true;}
      if(cdUXcMYgCd == ATpaXNlzSF){fOfGutdphU = true;}
      if(HYSkDLhQSK == klIjiCDcPA){YZiARkafqJ = true;}
      while(PxjQaMDttD == gIMuEtpqIU){LTrFVQCQcp = true;}
      while(ATpaXNlzSF == ATpaXNlzSF){GaFRlFKgpE = true;}
      while(klIjiCDcPA == klIjiCDcPA){wBaHQEPEKk = true;}
      if(onUTBXNWwS == true){onUTBXNWwS = false;}
      if(kBphPIZmgZ == true){kBphPIZmgZ = false;}
      if(oxAwfcoAhY == true){oxAwfcoAhY = false;}
      if(JZesNMyWZA == true){JZesNMyWZA = false;}
      if(zPDzTiwZUa == true){zPDzTiwZUa = false;}
      if(cPuoKrmYmY == true){cPuoKrmYmY = false;}
      if(iNoXIKWxTD == true){iNoXIKWxTD = false;}
      if(JDNgaZsAbx == true){JDNgaZsAbx = false;}
      if(fOfGutdphU == true){fOfGutdphU = false;}
      if(YZiARkafqJ == true){YZiARkafqJ = false;}
      if(MzGQQYBONk == true){MzGQQYBONk = false;}
      if(ylworThhbO == true){ylworThhbO = false;}
      if(ffOzdiTNOo == true){ffOzdiTNOo = false;}
      if(yNJSkgIOoV == true){yNJSkgIOoV = false;}
      if(rPcxVmcsHx == true){rPcxVmcsHx = false;}
      if(DsDccfkGmg == true){DsDccfkGmg = false;}
      if(FnzXlkbdEL == true){FnzXlkbdEL = false;}
      if(LTrFVQCQcp == true){LTrFVQCQcp = false;}
      if(GaFRlFKgpE == true){GaFRlFKgpE = false;}
      if(wBaHQEPEKk == true){wBaHQEPEKk = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YUPHAKWLQK
{ 
  void LjohbOeLws()
  { 
      bool bIDujlfNbM = false;
      bool gDiTiJkjfE = false;
      bool cRGqlYTXTV = false;
      bool lsQAChhZQz = false;
      bool zWafedgfTu = false;
      bool CmnoBKCMJK = false;
      bool RHtGanlhQA = false;
      bool bLSgFGpZLI = false;
      bool rnDYTzEazr = false;
      bool fVPFgqcTEq = false;
      bool qyOTSpTHjt = false;
      bool TbABSdDUmb = false;
      bool iLiYUGuNzN = false;
      bool dWUTQtZjFc = false;
      bool exqEXEUOmD = false;
      bool nHgcHfOoyM = false;
      bool uFHVPxGfjX = false;
      bool DTPJwlTeAQ = false;
      bool kfawZWorMk = false;
      bool fjksgYoUna = false;
      string joxKKwSRKe;
      string oWOCUQsGCd;
      string YOEdxwJcGs;
      string qDXWzorgrg;
      string NYdbZtuqFA;
      string CFhaVVjSNO;
      string moLRnlyqDf;
      string YMNVPzjbDC;
      string ASXFqkUxBd;
      string EYSctXMMnn;
      string uEnZapqMKW;
      string kIPKOTRFZC;
      string RApYaAsjar;
      string lWnPHueydB;
      string tImeswIHOZ;
      string GHmkVtOOya;
      string wGkSjNTzDS;
      string oTaYIINLfe;
      string qlVBlejCsM;
      string gPzDFmWUte;
      if(joxKKwSRKe == uEnZapqMKW){bIDujlfNbM = true;}
      else if(uEnZapqMKW == joxKKwSRKe){qyOTSpTHjt = true;}
      if(oWOCUQsGCd == kIPKOTRFZC){gDiTiJkjfE = true;}
      else if(kIPKOTRFZC == oWOCUQsGCd){TbABSdDUmb = true;}
      if(YOEdxwJcGs == RApYaAsjar){cRGqlYTXTV = true;}
      else if(RApYaAsjar == YOEdxwJcGs){iLiYUGuNzN = true;}
      if(qDXWzorgrg == lWnPHueydB){lsQAChhZQz = true;}
      else if(lWnPHueydB == qDXWzorgrg){dWUTQtZjFc = true;}
      if(NYdbZtuqFA == tImeswIHOZ){zWafedgfTu = true;}
      else if(tImeswIHOZ == NYdbZtuqFA){exqEXEUOmD = true;}
      if(CFhaVVjSNO == GHmkVtOOya){CmnoBKCMJK = true;}
      else if(GHmkVtOOya == CFhaVVjSNO){nHgcHfOoyM = true;}
      if(moLRnlyqDf == wGkSjNTzDS){RHtGanlhQA = true;}
      else if(wGkSjNTzDS == moLRnlyqDf){uFHVPxGfjX = true;}
      if(YMNVPzjbDC == oTaYIINLfe){bLSgFGpZLI = true;}
      if(ASXFqkUxBd == qlVBlejCsM){rnDYTzEazr = true;}
      if(EYSctXMMnn == gPzDFmWUte){fVPFgqcTEq = true;}
      while(oTaYIINLfe == YMNVPzjbDC){DTPJwlTeAQ = true;}
      while(qlVBlejCsM == qlVBlejCsM){kfawZWorMk = true;}
      while(gPzDFmWUte == gPzDFmWUte){fjksgYoUna = true;}
      if(bIDujlfNbM == true){bIDujlfNbM = false;}
      if(gDiTiJkjfE == true){gDiTiJkjfE = false;}
      if(cRGqlYTXTV == true){cRGqlYTXTV = false;}
      if(lsQAChhZQz == true){lsQAChhZQz = false;}
      if(zWafedgfTu == true){zWafedgfTu = false;}
      if(CmnoBKCMJK == true){CmnoBKCMJK = false;}
      if(RHtGanlhQA == true){RHtGanlhQA = false;}
      if(bLSgFGpZLI == true){bLSgFGpZLI = false;}
      if(rnDYTzEazr == true){rnDYTzEazr = false;}
      if(fVPFgqcTEq == true){fVPFgqcTEq = false;}
      if(qyOTSpTHjt == true){qyOTSpTHjt = false;}
      if(TbABSdDUmb == true){TbABSdDUmb = false;}
      if(iLiYUGuNzN == true){iLiYUGuNzN = false;}
      if(dWUTQtZjFc == true){dWUTQtZjFc = false;}
      if(exqEXEUOmD == true){exqEXEUOmD = false;}
      if(nHgcHfOoyM == true){nHgcHfOoyM = false;}
      if(uFHVPxGfjX == true){uFHVPxGfjX = false;}
      if(DTPJwlTeAQ == true){DTPJwlTeAQ = false;}
      if(kfawZWorMk == true){kfawZWorMk = false;}
      if(fjksgYoUna == true){fjksgYoUna = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RNMXBVHQWA
{ 
  void tUjpImzneD()
  { 
      bool pQqxNzEAeW = false;
      bool WqPXFMDcqd = false;
      bool mapqoAnXRw = false;
      bool gMYzSCEjlz = false;
      bool doQbofeudV = false;
      bool LnWaPPZAHM = false;
      bool LgihoJSPED = false;
      bool ITprzxJOBI = false;
      bool UrtuksoxiD = false;
      bool cDbZmwGimf = false;
      bool NPpZNrxeaR = false;
      bool GLzyqTmmxu = false;
      bool UUfBaEqPFC = false;
      bool qabVtFBThr = false;
      bool uKVRxREchT = false;
      bool JTkqKsiozJ = false;
      bool BgZXaeidEl = false;
      bool RTEEzKzOEJ = false;
      bool ANhksWojKB = false;
      bool VMTYqNjxpR = false;
      string nqxGHYYKiD;
      string MISCXiJoNI;
      string UamLErCHnt;
      string NpYAPcyKfm;
      string iykthOhnAh;
      string AjLVcMElCP;
      string WRWnoVjSmu;
      string jzIZYVpsdo;
      string JOGGtceCWD;
      string XMHQQRbxEL;
      string ePBExAblxG;
      string WKHMGHKPtM;
      string qguiWXqqIG;
      string trnSJsMCIs;
      string AsFQgNEtKI;
      string HLCqUyURhY;
      string JkcpLdWnCN;
      string uIHBMybdCJ;
      string SwgAyhpwyL;
      string XtTjiPQOED;
      if(nqxGHYYKiD == ePBExAblxG){pQqxNzEAeW = true;}
      else if(ePBExAblxG == nqxGHYYKiD){NPpZNrxeaR = true;}
      if(MISCXiJoNI == WKHMGHKPtM){WqPXFMDcqd = true;}
      else if(WKHMGHKPtM == MISCXiJoNI){GLzyqTmmxu = true;}
      if(UamLErCHnt == qguiWXqqIG){mapqoAnXRw = true;}
      else if(qguiWXqqIG == UamLErCHnt){UUfBaEqPFC = true;}
      if(NpYAPcyKfm == trnSJsMCIs){gMYzSCEjlz = true;}
      else if(trnSJsMCIs == NpYAPcyKfm){qabVtFBThr = true;}
      if(iykthOhnAh == AsFQgNEtKI){doQbofeudV = true;}
      else if(AsFQgNEtKI == iykthOhnAh){uKVRxREchT = true;}
      if(AjLVcMElCP == HLCqUyURhY){LnWaPPZAHM = true;}
      else if(HLCqUyURhY == AjLVcMElCP){JTkqKsiozJ = true;}
      if(WRWnoVjSmu == JkcpLdWnCN){LgihoJSPED = true;}
      else if(JkcpLdWnCN == WRWnoVjSmu){BgZXaeidEl = true;}
      if(jzIZYVpsdo == uIHBMybdCJ){ITprzxJOBI = true;}
      if(JOGGtceCWD == SwgAyhpwyL){UrtuksoxiD = true;}
      if(XMHQQRbxEL == XtTjiPQOED){cDbZmwGimf = true;}
      while(uIHBMybdCJ == jzIZYVpsdo){RTEEzKzOEJ = true;}
      while(SwgAyhpwyL == SwgAyhpwyL){ANhksWojKB = true;}
      while(XtTjiPQOED == XtTjiPQOED){VMTYqNjxpR = true;}
      if(pQqxNzEAeW == true){pQqxNzEAeW = false;}
      if(WqPXFMDcqd == true){WqPXFMDcqd = false;}
      if(mapqoAnXRw == true){mapqoAnXRw = false;}
      if(gMYzSCEjlz == true){gMYzSCEjlz = false;}
      if(doQbofeudV == true){doQbofeudV = false;}
      if(LnWaPPZAHM == true){LnWaPPZAHM = false;}
      if(LgihoJSPED == true){LgihoJSPED = false;}
      if(ITprzxJOBI == true){ITprzxJOBI = false;}
      if(UrtuksoxiD == true){UrtuksoxiD = false;}
      if(cDbZmwGimf == true){cDbZmwGimf = false;}
      if(NPpZNrxeaR == true){NPpZNrxeaR = false;}
      if(GLzyqTmmxu == true){GLzyqTmmxu = false;}
      if(UUfBaEqPFC == true){UUfBaEqPFC = false;}
      if(qabVtFBThr == true){qabVtFBThr = false;}
      if(uKVRxREchT == true){uKVRxREchT = false;}
      if(JTkqKsiozJ == true){JTkqKsiozJ = false;}
      if(BgZXaeidEl == true){BgZXaeidEl = false;}
      if(RTEEzKzOEJ == true){RTEEzKzOEJ = false;}
      if(ANhksWojKB == true){ANhksWojKB = false;}
      if(VMTYqNjxpR == true){VMTYqNjxpR = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BIRXZAXFHY
{ 
  void tXzrywPSUo()
  { 
      bool yOxpjSkfCo = false;
      bool SDAbXHhSwQ = false;
      bool WjYtjdKAVr = false;
      bool BRjqmKDCOA = false;
      bool tBhlCsLMaX = false;
      bool NqmQoOpCFG = false;
      bool HjKJmBTIoN = false;
      bool EfEwWUBlqp = false;
      bool RyZxeKdSCX = false;
      bool UAwHrKqYyB = false;
      bool YDWNShmqtT = false;
      bool MDLPnJKMec = false;
      bool WBGQsEiwDM = false;
      bool dcnoROaLdw = false;
      bool WmYhLQngTi = false;
      bool zdsUeWwFUO = false;
      bool zqnhpjOWxY = false;
      bool iyhhFicYFb = false;
      bool QCbUMAiSJt = false;
      bool SUqPxfzprb = false;
      string XixNcOZPTN;
      string TKhtJKauns;
      string uPHGqqREqQ;
      string oQGuqqgrIf;
      string nCabqbFbjM;
      string FbyFEHbqWl;
      string xUYCYDDmDS;
      string cilHEsneOO;
      string koTdLMRBpi;
      string OpUWhSdYDm;
      string XSYRiWppAL;
      string SwbHAHBtqL;
      string lCwTDmNPZk;
      string OPbbqSQCuo;
      string WaAePSNNWa;
      string RjhqAUVENw;
      string IaQPwDTzPg;
      string pzzBoLCRDk;
      string YArySegjaV;
      string xMnyfwJjhL;
      if(XixNcOZPTN == XSYRiWppAL){yOxpjSkfCo = true;}
      else if(XSYRiWppAL == XixNcOZPTN){YDWNShmqtT = true;}
      if(TKhtJKauns == SwbHAHBtqL){SDAbXHhSwQ = true;}
      else if(SwbHAHBtqL == TKhtJKauns){MDLPnJKMec = true;}
      if(uPHGqqREqQ == lCwTDmNPZk){WjYtjdKAVr = true;}
      else if(lCwTDmNPZk == uPHGqqREqQ){WBGQsEiwDM = true;}
      if(oQGuqqgrIf == OPbbqSQCuo){BRjqmKDCOA = true;}
      else if(OPbbqSQCuo == oQGuqqgrIf){dcnoROaLdw = true;}
      if(nCabqbFbjM == WaAePSNNWa){tBhlCsLMaX = true;}
      else if(WaAePSNNWa == nCabqbFbjM){WmYhLQngTi = true;}
      if(FbyFEHbqWl == RjhqAUVENw){NqmQoOpCFG = true;}
      else if(RjhqAUVENw == FbyFEHbqWl){zdsUeWwFUO = true;}
      if(xUYCYDDmDS == IaQPwDTzPg){HjKJmBTIoN = true;}
      else if(IaQPwDTzPg == xUYCYDDmDS){zqnhpjOWxY = true;}
      if(cilHEsneOO == pzzBoLCRDk){EfEwWUBlqp = true;}
      if(koTdLMRBpi == YArySegjaV){RyZxeKdSCX = true;}
      if(OpUWhSdYDm == xMnyfwJjhL){UAwHrKqYyB = true;}
      while(pzzBoLCRDk == cilHEsneOO){iyhhFicYFb = true;}
      while(YArySegjaV == YArySegjaV){QCbUMAiSJt = true;}
      while(xMnyfwJjhL == xMnyfwJjhL){SUqPxfzprb = true;}
      if(yOxpjSkfCo == true){yOxpjSkfCo = false;}
      if(SDAbXHhSwQ == true){SDAbXHhSwQ = false;}
      if(WjYtjdKAVr == true){WjYtjdKAVr = false;}
      if(BRjqmKDCOA == true){BRjqmKDCOA = false;}
      if(tBhlCsLMaX == true){tBhlCsLMaX = false;}
      if(NqmQoOpCFG == true){NqmQoOpCFG = false;}
      if(HjKJmBTIoN == true){HjKJmBTIoN = false;}
      if(EfEwWUBlqp == true){EfEwWUBlqp = false;}
      if(RyZxeKdSCX == true){RyZxeKdSCX = false;}
      if(UAwHrKqYyB == true){UAwHrKqYyB = false;}
      if(YDWNShmqtT == true){YDWNShmqtT = false;}
      if(MDLPnJKMec == true){MDLPnJKMec = false;}
      if(WBGQsEiwDM == true){WBGQsEiwDM = false;}
      if(dcnoROaLdw == true){dcnoROaLdw = false;}
      if(WmYhLQngTi == true){WmYhLQngTi = false;}
      if(zdsUeWwFUO == true){zdsUeWwFUO = false;}
      if(zqnhpjOWxY == true){zqnhpjOWxY = false;}
      if(iyhhFicYFb == true){iyhhFicYFb = false;}
      if(QCbUMAiSJt == true){QCbUMAiSJt = false;}
      if(SUqPxfzprb == true){SUqPxfzprb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class WAFNEWPGVA
{ 
  void RUuRJjYwJl()
  { 
      bool aEoQQmsamK = false;
      bool wwaizjGmjM = false;
      bool gkXxeWEulJ = false;
      bool wXbhzhFyOH = false;
      bool lFMtLXqlNM = false;
      bool AtWmepfqks = false;
      bool hyocNXUriQ = false;
      bool dIXcaaYdlH = false;
      bool fShZJCBnDJ = false;
      bool BNMVYnQigm = false;
      bool CyCwhrVtMD = false;
      bool QCFcNPPOTK = false;
      bool WfoZOQuCDm = false;
      bool JpwyEgQkrR = false;
      bool gGbArzIiIJ = false;
      bool XCPVQVJchR = false;
      bool wSqYsaFGXg = false;
      bool zeWCdPEogP = false;
      bool HLttPHaagi = false;
      bool WmDaUNPCGR = false;
      string tMJakatyjj;
      string ZaPtfPUUPX;
      string JeUXKKTNlh;
      string Olketrpyhr;
      string yUUTJiaokg;
      string oldLOOqkPa;
      string HgLCVnpjzn;
      string ZWlWYoJBOb;
      string idbFsbHYQe;
      string BEhHjRISxU;
      string sqpxGSzGmY;
      string FoWnqdEGJs;
      string fRSUmUnelH;
      string gqEsirneya;
      string bBlrZLhtPG;
      string MBJEoaXUJl;
      string dUTerOkcFz;
      string pZHhQdeata;
      string mtOftegzEH;
      string TSUMwGhGzL;
      if(tMJakatyjj == sqpxGSzGmY){aEoQQmsamK = true;}
      else if(sqpxGSzGmY == tMJakatyjj){CyCwhrVtMD = true;}
      if(ZaPtfPUUPX == FoWnqdEGJs){wwaizjGmjM = true;}
      else if(FoWnqdEGJs == ZaPtfPUUPX){QCFcNPPOTK = true;}
      if(JeUXKKTNlh == fRSUmUnelH){gkXxeWEulJ = true;}
      else if(fRSUmUnelH == JeUXKKTNlh){WfoZOQuCDm = true;}
      if(Olketrpyhr == gqEsirneya){wXbhzhFyOH = true;}
      else if(gqEsirneya == Olketrpyhr){JpwyEgQkrR = true;}
      if(yUUTJiaokg == bBlrZLhtPG){lFMtLXqlNM = true;}
      else if(bBlrZLhtPG == yUUTJiaokg){gGbArzIiIJ = true;}
      if(oldLOOqkPa == MBJEoaXUJl){AtWmepfqks = true;}
      else if(MBJEoaXUJl == oldLOOqkPa){XCPVQVJchR = true;}
      if(HgLCVnpjzn == dUTerOkcFz){hyocNXUriQ = true;}
      else if(dUTerOkcFz == HgLCVnpjzn){wSqYsaFGXg = true;}
      if(ZWlWYoJBOb == pZHhQdeata){dIXcaaYdlH = true;}
      if(idbFsbHYQe == mtOftegzEH){fShZJCBnDJ = true;}
      if(BEhHjRISxU == TSUMwGhGzL){BNMVYnQigm = true;}
      while(pZHhQdeata == ZWlWYoJBOb){zeWCdPEogP = true;}
      while(mtOftegzEH == mtOftegzEH){HLttPHaagi = true;}
      while(TSUMwGhGzL == TSUMwGhGzL){WmDaUNPCGR = true;}
      if(aEoQQmsamK == true){aEoQQmsamK = false;}
      if(wwaizjGmjM == true){wwaizjGmjM = false;}
      if(gkXxeWEulJ == true){gkXxeWEulJ = false;}
      if(wXbhzhFyOH == true){wXbhzhFyOH = false;}
      if(lFMtLXqlNM == true){lFMtLXqlNM = false;}
      if(AtWmepfqks == true){AtWmepfqks = false;}
      if(hyocNXUriQ == true){hyocNXUriQ = false;}
      if(dIXcaaYdlH == true){dIXcaaYdlH = false;}
      if(fShZJCBnDJ == true){fShZJCBnDJ = false;}
      if(BNMVYnQigm == true){BNMVYnQigm = false;}
      if(CyCwhrVtMD == true){CyCwhrVtMD = false;}
      if(QCFcNPPOTK == true){QCFcNPPOTK = false;}
      if(WfoZOQuCDm == true){WfoZOQuCDm = false;}
      if(JpwyEgQkrR == true){JpwyEgQkrR = false;}
      if(gGbArzIiIJ == true){gGbArzIiIJ = false;}
      if(XCPVQVJchR == true){XCPVQVJchR = false;}
      if(wSqYsaFGXg == true){wSqYsaFGXg = false;}
      if(zeWCdPEogP == true){zeWCdPEogP = false;}
      if(HLttPHaagi == true){HLttPHaagi = false;}
      if(WmDaUNPCGR == true){WmDaUNPCGR = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NRTAHRXZCC
{ 
  void TlKqxqmeOA()
  { 
      bool iouNjszFpl = false;
      bool JqMFSgqHgC = false;
      bool jAsRRTRpGZ = false;
      bool FmdFqjlfZX = false;
      bool zphtLeKyYc = false;
      bool fLcodiwcwP = false;
      bool bIzQgVNGOh = false;
      bool uLgJCsRyGm = false;
      bool JTQdttJnbH = false;
      bool nJiJuqqQNo = false;
      bool QEiqhUdTmj = false;
      bool QyWJGIqVVl = false;
      bool MYeHtkbWNL = false;
      bool CBjRVEjZuZ = false;
      bool PpBqpomZDm = false;
      bool zVtbbqYbww = false;
      bool rGFahPOEmB = false;
      bool YQZwVWXmRE = false;
      bool yBTZLgGHoe = false;
      bool eHdzgIjZJZ = false;
      string QgOzTuwQqi;
      string EYjZLrIfih;
      string Vnpgyhypcu;
      string pFbrUiRxKV;
      string cnIkSJUzqJ;
      string pBQlPcphBk;
      string wGmusywqEb;
      string ZKVMEheENn;
      string NzzTEnopbq;
      string sjuZZWAMrU;
      string MctOiefEgh;
      string ykTNXxGSpE;
      string JGPNipPumh;
      string gRMsajEjkZ;
      string HjJssIPhBc;
      string PhVnLGNgfr;
      string PkRyBfYaln;
      string rVhQxYxVTZ;
      string ZhsKRzuYgU;
      string VXErKWSKrN;
      if(QgOzTuwQqi == MctOiefEgh){iouNjszFpl = true;}
      else if(MctOiefEgh == QgOzTuwQqi){QEiqhUdTmj = true;}
      if(EYjZLrIfih == ykTNXxGSpE){JqMFSgqHgC = true;}
      else if(ykTNXxGSpE == EYjZLrIfih){QyWJGIqVVl = true;}
      if(Vnpgyhypcu == JGPNipPumh){jAsRRTRpGZ = true;}
      else if(JGPNipPumh == Vnpgyhypcu){MYeHtkbWNL = true;}
      if(pFbrUiRxKV == gRMsajEjkZ){FmdFqjlfZX = true;}
      else if(gRMsajEjkZ == pFbrUiRxKV){CBjRVEjZuZ = true;}
      if(cnIkSJUzqJ == HjJssIPhBc){zphtLeKyYc = true;}
      else if(HjJssIPhBc == cnIkSJUzqJ){PpBqpomZDm = true;}
      if(pBQlPcphBk == PhVnLGNgfr){fLcodiwcwP = true;}
      else if(PhVnLGNgfr == pBQlPcphBk){zVtbbqYbww = true;}
      if(wGmusywqEb == PkRyBfYaln){bIzQgVNGOh = true;}
      else if(PkRyBfYaln == wGmusywqEb){rGFahPOEmB = true;}
      if(ZKVMEheENn == rVhQxYxVTZ){uLgJCsRyGm = true;}
      if(NzzTEnopbq == ZhsKRzuYgU){JTQdttJnbH = true;}
      if(sjuZZWAMrU == VXErKWSKrN){nJiJuqqQNo = true;}
      while(rVhQxYxVTZ == ZKVMEheENn){YQZwVWXmRE = true;}
      while(ZhsKRzuYgU == ZhsKRzuYgU){yBTZLgGHoe = true;}
      while(VXErKWSKrN == VXErKWSKrN){eHdzgIjZJZ = true;}
      if(iouNjszFpl == true){iouNjszFpl = false;}
      if(JqMFSgqHgC == true){JqMFSgqHgC = false;}
      if(jAsRRTRpGZ == true){jAsRRTRpGZ = false;}
      if(FmdFqjlfZX == true){FmdFqjlfZX = false;}
      if(zphtLeKyYc == true){zphtLeKyYc = false;}
      if(fLcodiwcwP == true){fLcodiwcwP = false;}
      if(bIzQgVNGOh == true){bIzQgVNGOh = false;}
      if(uLgJCsRyGm == true){uLgJCsRyGm = false;}
      if(JTQdttJnbH == true){JTQdttJnbH = false;}
      if(nJiJuqqQNo == true){nJiJuqqQNo = false;}
      if(QEiqhUdTmj == true){QEiqhUdTmj = false;}
      if(QyWJGIqVVl == true){QyWJGIqVVl = false;}
      if(MYeHtkbWNL == true){MYeHtkbWNL = false;}
      if(CBjRVEjZuZ == true){CBjRVEjZuZ = false;}
      if(PpBqpomZDm == true){PpBqpomZDm = false;}
      if(zVtbbqYbww == true){zVtbbqYbww = false;}
      if(rGFahPOEmB == true){rGFahPOEmB = false;}
      if(YQZwVWXmRE == true){YQZwVWXmRE = false;}
      if(yBTZLgGHoe == true){yBTZLgGHoe = false;}
      if(eHdzgIjZJZ == true){eHdzgIjZJZ = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CHYOQIRZCP
{ 
  void zhpIPUiHFr()
  { 
      bool iNCAIznbSo = false;
      bool UFlyoLkjFH = false;
      bool SDRKwdSxbd = false;
      bool kUsYmpzpmo = false;
      bool JPgmJUeGpT = false;
      bool ZfBcuewrEe = false;
      bool gpznEAMNKz = false;
      bool LtwQyCVdAa = false;
      bool yLGIEESPcH = false;
      bool sHkIISPsiJ = false;
      bool IRwqHeiker = false;
      bool IRqdfUiqiz = false;
      bool gxjCFEInIe = false;
      bool lcuqSfOPBY = false;
      bool UhkjybQHbm = false;
      bool jYKicLhtlD = false;
      bool VuczFfWAKz = false;
      bool FToKdyLzDd = false;
      bool mBLUtNSLNH = false;
      bool yUcyCZEkeS = false;
      string CgpKDzswZu;
      string artgIaxbxE;
      string gDeUgDkqQU;
      string pAANJAroGG;
      string WypnNWlImm;
      string QMXoCzBUcz;
      string OBeFFZqfWI;
      string iHVqhhMpHJ;
      string RbEcDccKKK;
      string yWMGqUTkin;
      string eFkeEJqfHT;
      string lLNThlxBBb;
      string xgULJMuOAM;
      string ObHTdLqZXt;
      string lwDpYmIrXE;
      string XRjHpyjYUr;
      string ObfuFKDtoW;
      string uXcHoBcemW;
      string DCqwoxrAce;
      string aREfeKPHkN;
      if(CgpKDzswZu == eFkeEJqfHT){iNCAIznbSo = true;}
      else if(eFkeEJqfHT == CgpKDzswZu){IRwqHeiker = true;}
      if(artgIaxbxE == lLNThlxBBb){UFlyoLkjFH = true;}
      else if(lLNThlxBBb == artgIaxbxE){IRqdfUiqiz = true;}
      if(gDeUgDkqQU == xgULJMuOAM){SDRKwdSxbd = true;}
      else if(xgULJMuOAM == gDeUgDkqQU){gxjCFEInIe = true;}
      if(pAANJAroGG == ObHTdLqZXt){kUsYmpzpmo = true;}
      else if(ObHTdLqZXt == pAANJAroGG){lcuqSfOPBY = true;}
      if(WypnNWlImm == lwDpYmIrXE){JPgmJUeGpT = true;}
      else if(lwDpYmIrXE == WypnNWlImm){UhkjybQHbm = true;}
      if(QMXoCzBUcz == XRjHpyjYUr){ZfBcuewrEe = true;}
      else if(XRjHpyjYUr == QMXoCzBUcz){jYKicLhtlD = true;}
      if(OBeFFZqfWI == ObfuFKDtoW){gpznEAMNKz = true;}
      else if(ObfuFKDtoW == OBeFFZqfWI){VuczFfWAKz = true;}
      if(iHVqhhMpHJ == uXcHoBcemW){LtwQyCVdAa = true;}
      if(RbEcDccKKK == DCqwoxrAce){yLGIEESPcH = true;}
      if(yWMGqUTkin == aREfeKPHkN){sHkIISPsiJ = true;}
      while(uXcHoBcemW == iHVqhhMpHJ){FToKdyLzDd = true;}
      while(DCqwoxrAce == DCqwoxrAce){mBLUtNSLNH = true;}
      while(aREfeKPHkN == aREfeKPHkN){yUcyCZEkeS = true;}
      if(iNCAIznbSo == true){iNCAIznbSo = false;}
      if(UFlyoLkjFH == true){UFlyoLkjFH = false;}
      if(SDRKwdSxbd == true){SDRKwdSxbd = false;}
      if(kUsYmpzpmo == true){kUsYmpzpmo = false;}
      if(JPgmJUeGpT == true){JPgmJUeGpT = false;}
      if(ZfBcuewrEe == true){ZfBcuewrEe = false;}
      if(gpznEAMNKz == true){gpznEAMNKz = false;}
      if(LtwQyCVdAa == true){LtwQyCVdAa = false;}
      if(yLGIEESPcH == true){yLGIEESPcH = false;}
      if(sHkIISPsiJ == true){sHkIISPsiJ = false;}
      if(IRwqHeiker == true){IRwqHeiker = false;}
      if(IRqdfUiqiz == true){IRqdfUiqiz = false;}
      if(gxjCFEInIe == true){gxjCFEInIe = false;}
      if(lcuqSfOPBY == true){lcuqSfOPBY = false;}
      if(UhkjybQHbm == true){UhkjybQHbm = false;}
      if(jYKicLhtlD == true){jYKicLhtlD = false;}
      if(VuczFfWAKz == true){VuczFfWAKz = false;}
      if(FToKdyLzDd == true){FToKdyLzDd = false;}
      if(mBLUtNSLNH == true){mBLUtNSLNH = false;}
      if(yUcyCZEkeS == true){yUcyCZEkeS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XLMIUGULKR
{ 
  void fCCkATGMTh()
  { 
      bool TwJJIbzbbB = false;
      bool HCDcszmKPo = false;
      bool ZeajnEKGsu = false;
      bool rhqQDxsRMT = false;
      bool DrUsGOIIfJ = false;
      bool pfLPGnVZVD = false;
      bool RsifIYLmYu = false;
      bool GVxwkhKgdY = false;
      bool uBZiFLWaKP = false;
      bool bSuIBKInkg = false;
      bool DVikDUiOdY = false;
      bool cnwwIPSAaw = false;
      bool CjcJTmZnta = false;
      bool YVwwVZSnfj = false;
      bool cHxllmWzAl = false;
      bool qINndTsZRQ = false;
      bool ttUDzGrzah = false;
      bool xeUpStmpWf = false;
      bool PanHiQIHSC = false;
      bool grWOohmsle = false;
      string DsjtoPBmQC;
      string ToKwyQFrNW;
      string bSDNXLBMkf;
      string jRZxEKmIOR;
      string uYblrdEPGa;
      string IjVGbjpVWc;
      string OnCjSBPNqm;
      string cwcwaDqQVs;
      string HkFKWzwSUT;
      string URkkCGWqTp;
      string NqXTyMGZfY;
      string sYAdVdirMS;
      string FwrXSLKqtT;
      string gzxeeQMlMK;
      string sVmwauRHPY;
      string XeeBjRyAAm;
      string BPeLqtbUyF;
      string QweCYiJwxX;
      string sCbUuCUwtN;
      string RMlYIdqaEf;
      if(DsjtoPBmQC == NqXTyMGZfY){TwJJIbzbbB = true;}
      else if(NqXTyMGZfY == DsjtoPBmQC){DVikDUiOdY = true;}
      if(ToKwyQFrNW == sYAdVdirMS){HCDcszmKPo = true;}
      else if(sYAdVdirMS == ToKwyQFrNW){cnwwIPSAaw = true;}
      if(bSDNXLBMkf == FwrXSLKqtT){ZeajnEKGsu = true;}
      else if(FwrXSLKqtT == bSDNXLBMkf){CjcJTmZnta = true;}
      if(jRZxEKmIOR == gzxeeQMlMK){rhqQDxsRMT = true;}
      else if(gzxeeQMlMK == jRZxEKmIOR){YVwwVZSnfj = true;}
      if(uYblrdEPGa == sVmwauRHPY){DrUsGOIIfJ = true;}
      else if(sVmwauRHPY == uYblrdEPGa){cHxllmWzAl = true;}
      if(IjVGbjpVWc == XeeBjRyAAm){pfLPGnVZVD = true;}
      else if(XeeBjRyAAm == IjVGbjpVWc){qINndTsZRQ = true;}
      if(OnCjSBPNqm == BPeLqtbUyF){RsifIYLmYu = true;}
      else if(BPeLqtbUyF == OnCjSBPNqm){ttUDzGrzah = true;}
      if(cwcwaDqQVs == QweCYiJwxX){GVxwkhKgdY = true;}
      if(HkFKWzwSUT == sCbUuCUwtN){uBZiFLWaKP = true;}
      if(URkkCGWqTp == RMlYIdqaEf){bSuIBKInkg = true;}
      while(QweCYiJwxX == cwcwaDqQVs){xeUpStmpWf = true;}
      while(sCbUuCUwtN == sCbUuCUwtN){PanHiQIHSC = true;}
      while(RMlYIdqaEf == RMlYIdqaEf){grWOohmsle = true;}
      if(TwJJIbzbbB == true){TwJJIbzbbB = false;}
      if(HCDcszmKPo == true){HCDcszmKPo = false;}
      if(ZeajnEKGsu == true){ZeajnEKGsu = false;}
      if(rhqQDxsRMT == true){rhqQDxsRMT = false;}
      if(DrUsGOIIfJ == true){DrUsGOIIfJ = false;}
      if(pfLPGnVZVD == true){pfLPGnVZVD = false;}
      if(RsifIYLmYu == true){RsifIYLmYu = false;}
      if(GVxwkhKgdY == true){GVxwkhKgdY = false;}
      if(uBZiFLWaKP == true){uBZiFLWaKP = false;}
      if(bSuIBKInkg == true){bSuIBKInkg = false;}
      if(DVikDUiOdY == true){DVikDUiOdY = false;}
      if(cnwwIPSAaw == true){cnwwIPSAaw = false;}
      if(CjcJTmZnta == true){CjcJTmZnta = false;}
      if(YVwwVZSnfj == true){YVwwVZSnfj = false;}
      if(cHxllmWzAl == true){cHxllmWzAl = false;}
      if(qINndTsZRQ == true){qINndTsZRQ = false;}
      if(ttUDzGrzah == true){ttUDzGrzah = false;}
      if(xeUpStmpWf == true){xeUpStmpWf = false;}
      if(PanHiQIHSC == true){PanHiQIHSC = false;}
      if(grWOohmsle == true){grWOohmsle = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FRXTSIDPRV
{ 
  void XJcnzaDcfB()
  { 
      bool gQTomGUUYK = false;
      bool VhpOeHRUgC = false;
      bool tujrhWWUAK = false;
      bool LZmgxLWPlT = false;
      bool wUQcIkTDcL = false;
      bool UDdKBlfbHQ = false;
      bool jzTNywuIQS = false;
      bool jChMFZeHjx = false;
      bool DTqopIwbfK = false;
      bool euHVjxBCdL = false;
      bool oKlJaSUQFb = false;
      bool HYrKNQHdXp = false;
      bool ATcGnotMcH = false;
      bool XbcXHrggOW = false;
      bool meVEDFCxHT = false;
      bool pWemLUhfVl = false;
      bool XImxDewUCU = false;
      bool iZPADUxXCh = false;
      bool lxEJRBxpLl = false;
      bool JBLczerYAX = false;
      string pntwFLJQzR;
      string jjLbQDcFQP;
      string pTBBxitGdM;
      string iUnLTCoxPt;
      string yfNLQEQlEn;
      string MHLttWmrzV;
      string QCllhOuzIN;
      string NfoWQJGoks;
      string SHCpzaJtRx;
      string PwxscutacB;
      string IQdqZgRocj;
      string PxHGDJAulG;
      string FBhKxRhusV;
      string PeLikBcLQS;
      string JURhgSnppT;
      string uyoWwGiTsI;
      string zWuGTkkXVB;
      string fndTxkjAde;
      string cqEZZRpomd;
      string SJRISlYKNw;
      if(pntwFLJQzR == IQdqZgRocj){gQTomGUUYK = true;}
      else if(IQdqZgRocj == pntwFLJQzR){oKlJaSUQFb = true;}
      if(jjLbQDcFQP == PxHGDJAulG){VhpOeHRUgC = true;}
      else if(PxHGDJAulG == jjLbQDcFQP){HYrKNQHdXp = true;}
      if(pTBBxitGdM == FBhKxRhusV){tujrhWWUAK = true;}
      else if(FBhKxRhusV == pTBBxitGdM){ATcGnotMcH = true;}
      if(iUnLTCoxPt == PeLikBcLQS){LZmgxLWPlT = true;}
      else if(PeLikBcLQS == iUnLTCoxPt){XbcXHrggOW = true;}
      if(yfNLQEQlEn == JURhgSnppT){wUQcIkTDcL = true;}
      else if(JURhgSnppT == yfNLQEQlEn){meVEDFCxHT = true;}
      if(MHLttWmrzV == uyoWwGiTsI){UDdKBlfbHQ = true;}
      else if(uyoWwGiTsI == MHLttWmrzV){pWemLUhfVl = true;}
      if(QCllhOuzIN == zWuGTkkXVB){jzTNywuIQS = true;}
      else if(zWuGTkkXVB == QCllhOuzIN){XImxDewUCU = true;}
      if(NfoWQJGoks == fndTxkjAde){jChMFZeHjx = true;}
      if(SHCpzaJtRx == cqEZZRpomd){DTqopIwbfK = true;}
      if(PwxscutacB == SJRISlYKNw){euHVjxBCdL = true;}
      while(fndTxkjAde == NfoWQJGoks){iZPADUxXCh = true;}
      while(cqEZZRpomd == cqEZZRpomd){lxEJRBxpLl = true;}
      while(SJRISlYKNw == SJRISlYKNw){JBLczerYAX = true;}
      if(gQTomGUUYK == true){gQTomGUUYK = false;}
      if(VhpOeHRUgC == true){VhpOeHRUgC = false;}
      if(tujrhWWUAK == true){tujrhWWUAK = false;}
      if(LZmgxLWPlT == true){LZmgxLWPlT = false;}
      if(wUQcIkTDcL == true){wUQcIkTDcL = false;}
      if(UDdKBlfbHQ == true){UDdKBlfbHQ = false;}
      if(jzTNywuIQS == true){jzTNywuIQS = false;}
      if(jChMFZeHjx == true){jChMFZeHjx = false;}
      if(DTqopIwbfK == true){DTqopIwbfK = false;}
      if(euHVjxBCdL == true){euHVjxBCdL = false;}
      if(oKlJaSUQFb == true){oKlJaSUQFb = false;}
      if(HYrKNQHdXp == true){HYrKNQHdXp = false;}
      if(ATcGnotMcH == true){ATcGnotMcH = false;}
      if(XbcXHrggOW == true){XbcXHrggOW = false;}
      if(meVEDFCxHT == true){meVEDFCxHT = false;}
      if(pWemLUhfVl == true){pWemLUhfVl = false;}
      if(XImxDewUCU == true){XImxDewUCU = false;}
      if(iZPADUxXCh == true){iZPADUxXCh = false;}
      if(lxEJRBxpLl == true){lxEJRBxpLl = false;}
      if(JBLczerYAX == true){JBLczerYAX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BRLXOPFYXL
{ 
  void MAuKgFWzTH()
  { 
      bool rkdRSUmnsI = false;
      bool XSHlsTLaTB = false;
      bool IuPuWYfknG = false;
      bool GqCcqEYSNj = false;
      bool DfkKbLigqH = false;
      bool wKENNwUitt = false;
      bool oBzmBgQeto = false;
      bool trNixbKFSL = false;
      bool oztfJulnOt = false;
      bool ipXSuCgxIu = false;
      bool tElCNExUPk = false;
      bool lOPiEpHTcF = false;
      bool tcQzFDTtmu = false;
      bool uMxXQkMrRF = false;
      bool yQXcITaoOF = false;
      bool tMUCBRnmwK = false;
      bool jCgUpYRrSS = false;
      bool cPBXHcEmqk = false;
      bool cefhAhrtWz = false;
      bool xZuHUujDmi = false;
      string TmMUBiQZxU;
      string MhJAlIOUfh;
      string nYMDyJORcj;
      string wFXkyEeddR;
      string EsfzuKiJFT;
      string kqmbltZJVr;
      string TyxNXGJXCg;
      string ckpOdurNHn;
      string aFMIzFSwyD;
      string eUHlyFwSan;
      string AKGXpAAYxq;
      string CMBhROlnOS;
      string mNYzCIcEQX;
      string yYlTmPynSy;
      string EtyHARSZTi;
      string SIEHBmkHVh;
      string WwHZrdZhuC;
      string PyVPOsPRzj;
      string oDkgFpUdSz;
      string eqSjbHlYPW;
      if(TmMUBiQZxU == AKGXpAAYxq){rkdRSUmnsI = true;}
      else if(AKGXpAAYxq == TmMUBiQZxU){tElCNExUPk = true;}
      if(MhJAlIOUfh == CMBhROlnOS){XSHlsTLaTB = true;}
      else if(CMBhROlnOS == MhJAlIOUfh){lOPiEpHTcF = true;}
      if(nYMDyJORcj == mNYzCIcEQX){IuPuWYfknG = true;}
      else if(mNYzCIcEQX == nYMDyJORcj){tcQzFDTtmu = true;}
      if(wFXkyEeddR == yYlTmPynSy){GqCcqEYSNj = true;}
      else if(yYlTmPynSy == wFXkyEeddR){uMxXQkMrRF = true;}
      if(EsfzuKiJFT == EtyHARSZTi){DfkKbLigqH = true;}
      else if(EtyHARSZTi == EsfzuKiJFT){yQXcITaoOF = true;}
      if(kqmbltZJVr == SIEHBmkHVh){wKENNwUitt = true;}
      else if(SIEHBmkHVh == kqmbltZJVr){tMUCBRnmwK = true;}
      if(TyxNXGJXCg == WwHZrdZhuC){oBzmBgQeto = true;}
      else if(WwHZrdZhuC == TyxNXGJXCg){jCgUpYRrSS = true;}
      if(ckpOdurNHn == PyVPOsPRzj){trNixbKFSL = true;}
      if(aFMIzFSwyD == oDkgFpUdSz){oztfJulnOt = true;}
      if(eUHlyFwSan == eqSjbHlYPW){ipXSuCgxIu = true;}
      while(PyVPOsPRzj == ckpOdurNHn){cPBXHcEmqk = true;}
      while(oDkgFpUdSz == oDkgFpUdSz){cefhAhrtWz = true;}
      while(eqSjbHlYPW == eqSjbHlYPW){xZuHUujDmi = true;}
      if(rkdRSUmnsI == true){rkdRSUmnsI = false;}
      if(XSHlsTLaTB == true){XSHlsTLaTB = false;}
      if(IuPuWYfknG == true){IuPuWYfknG = false;}
      if(GqCcqEYSNj == true){GqCcqEYSNj = false;}
      if(DfkKbLigqH == true){DfkKbLigqH = false;}
      if(wKENNwUitt == true){wKENNwUitt = false;}
      if(oBzmBgQeto == true){oBzmBgQeto = false;}
      if(trNixbKFSL == true){trNixbKFSL = false;}
      if(oztfJulnOt == true){oztfJulnOt = false;}
      if(ipXSuCgxIu == true){ipXSuCgxIu = false;}
      if(tElCNExUPk == true){tElCNExUPk = false;}
      if(lOPiEpHTcF == true){lOPiEpHTcF = false;}
      if(tcQzFDTtmu == true){tcQzFDTtmu = false;}
      if(uMxXQkMrRF == true){uMxXQkMrRF = false;}
      if(yQXcITaoOF == true){yQXcITaoOF = false;}
      if(tMUCBRnmwK == true){tMUCBRnmwK = false;}
      if(jCgUpYRrSS == true){jCgUpYRrSS = false;}
      if(cPBXHcEmqk == true){cPBXHcEmqk = false;}
      if(cefhAhrtWz == true){cefhAhrtWz = false;}
      if(xZuHUujDmi == true){xZuHUujDmi = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KGDQZAYQUK
{ 
  void xIBBflGgtu()
  { 
      bool SsmciBJMlT = false;
      bool pVQMbJWXHC = false;
      bool MrJmnmAqVW = false;
      bool rxFLajGJlc = false;
      bool QfWpADwPuf = false;
      bool SwFdWajzou = false;
      bool BRqPbsPFlr = false;
      bool CMkdlfUSmj = false;
      bool qjwoDwqboA = false;
      bool zHUpbyoHOH = false;
      bool nqdDdGKbqp = false;
      bool FQMEERUBLf = false;
      bool BmQkruruCm = false;
      bool qNDewzGsLx = false;
      bool lPkytitPkK = false;
      bool dmuiFAZrKh = false;
      bool NPYDyrMhIE = false;
      bool KbpOskCRpJ = false;
      bool BbJrJDRUYu = false;
      bool YNhciVqgAC = false;
      string HKxtCykuso;
      string MEEVytOgOO;
      string rpgoUVGRgb;
      string oZxYbHfNSA;
      string LduLrSjnMJ;
      string RBMAnRdHZg;
      string qSOQzlHfsq;
      string OAONDNuUxC;
      string lSFWKmMxjK;
      string cXnkIrPdLF;
      string KEtUuQmzxa;
      string VJKWuYNqFM;
      string ZKuiNpMcFK;
      string yQIqrhXjbt;
      string zNzJnTNRpj;
      string eXXzfAILHE;
      string EJsmcFBTeN;
      string AcOLMLKltt;
      string ctmVeZrIKq;
      string YMcWppTMgi;
      if(HKxtCykuso == KEtUuQmzxa){SsmciBJMlT = true;}
      else if(KEtUuQmzxa == HKxtCykuso){nqdDdGKbqp = true;}
      if(MEEVytOgOO == VJKWuYNqFM){pVQMbJWXHC = true;}
      else if(VJKWuYNqFM == MEEVytOgOO){FQMEERUBLf = true;}
      if(rpgoUVGRgb == ZKuiNpMcFK){MrJmnmAqVW = true;}
      else if(ZKuiNpMcFK == rpgoUVGRgb){BmQkruruCm = true;}
      if(oZxYbHfNSA == yQIqrhXjbt){rxFLajGJlc = true;}
      else if(yQIqrhXjbt == oZxYbHfNSA){qNDewzGsLx = true;}
      if(LduLrSjnMJ == zNzJnTNRpj){QfWpADwPuf = true;}
      else if(zNzJnTNRpj == LduLrSjnMJ){lPkytitPkK = true;}
      if(RBMAnRdHZg == eXXzfAILHE){SwFdWajzou = true;}
      else if(eXXzfAILHE == RBMAnRdHZg){dmuiFAZrKh = true;}
      if(qSOQzlHfsq == EJsmcFBTeN){BRqPbsPFlr = true;}
      else if(EJsmcFBTeN == qSOQzlHfsq){NPYDyrMhIE = true;}
      if(OAONDNuUxC == AcOLMLKltt){CMkdlfUSmj = true;}
      if(lSFWKmMxjK == ctmVeZrIKq){qjwoDwqboA = true;}
      if(cXnkIrPdLF == YMcWppTMgi){zHUpbyoHOH = true;}
      while(AcOLMLKltt == OAONDNuUxC){KbpOskCRpJ = true;}
      while(ctmVeZrIKq == ctmVeZrIKq){BbJrJDRUYu = true;}
      while(YMcWppTMgi == YMcWppTMgi){YNhciVqgAC = true;}
      if(SsmciBJMlT == true){SsmciBJMlT = false;}
      if(pVQMbJWXHC == true){pVQMbJWXHC = false;}
      if(MrJmnmAqVW == true){MrJmnmAqVW = false;}
      if(rxFLajGJlc == true){rxFLajGJlc = false;}
      if(QfWpADwPuf == true){QfWpADwPuf = false;}
      if(SwFdWajzou == true){SwFdWajzou = false;}
      if(BRqPbsPFlr == true){BRqPbsPFlr = false;}
      if(CMkdlfUSmj == true){CMkdlfUSmj = false;}
      if(qjwoDwqboA == true){qjwoDwqboA = false;}
      if(zHUpbyoHOH == true){zHUpbyoHOH = false;}
      if(nqdDdGKbqp == true){nqdDdGKbqp = false;}
      if(FQMEERUBLf == true){FQMEERUBLf = false;}
      if(BmQkruruCm == true){BmQkruruCm = false;}
      if(qNDewzGsLx == true){qNDewzGsLx = false;}
      if(lPkytitPkK == true){lPkytitPkK = false;}
      if(dmuiFAZrKh == true){dmuiFAZrKh = false;}
      if(NPYDyrMhIE == true){NPYDyrMhIE = false;}
      if(KbpOskCRpJ == true){KbpOskCRpJ = false;}
      if(BbJrJDRUYu == true){BbJrJDRUYu = false;}
      if(YNhciVqgAC == true){YNhciVqgAC = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PIUDRBJSZG
{ 
  void EsjcQtpsXH()
  { 
      bool FUMRGlfXHq = false;
      bool FqpQQBsmkt = false;
      bool qaHMSELjdq = false;
      bool ZXtGVnanuK = false;
      bool SRqotnuubt = false;
      bool JbVaWeNzyC = false;
      bool ZusQQgxjub = false;
      bool ZFqZbZiSSe = false;
      bool JQHjgxCSip = false;
      bool aaoskWXaXN = false;
      bool qKAATEkuyx = false;
      bool wugTsImJjS = false;
      bool ddBEByPxoE = false;
      bool BItEmyQHPc = false;
      bool MHUktftVAb = false;
      bool iNpFjNJuyU = false;
      bool nmFtunAcft = false;
      bool rftGFWJPKV = false;
      bool EtxMVnRoUZ = false;
      bool oUJtnZBZoF = false;
      string ZGiVIpztdt;
      string EFnNCTracO;
      string oHbykDeShd;
      string UQKphmsHEt;
      string TVKZMyhHOU;
      string osXhWRekjE;
      string KgMAYmuxfP;
      string akWhtSxCkm;
      string VXDNoasbSP;
      string IIWzimJJAZ;
      string DjazsDWonE;
      string UbzximcyLy;
      string sSdPHSNWzG;
      string JnCLjkxETk;
      string DEnOufIZPw;
      string KKyCfjFLra;
      string oAogtgZmMy;
      string pYdZtoeOUa;
      string JnGCwzozfz;
      string ekMsuDOGUO;
      if(ZGiVIpztdt == DjazsDWonE){FUMRGlfXHq = true;}
      else if(DjazsDWonE == ZGiVIpztdt){qKAATEkuyx = true;}
      if(EFnNCTracO == UbzximcyLy){FqpQQBsmkt = true;}
      else if(UbzximcyLy == EFnNCTracO){wugTsImJjS = true;}
      if(oHbykDeShd == sSdPHSNWzG){qaHMSELjdq = true;}
      else if(sSdPHSNWzG == oHbykDeShd){ddBEByPxoE = true;}
      if(UQKphmsHEt == JnCLjkxETk){ZXtGVnanuK = true;}
      else if(JnCLjkxETk == UQKphmsHEt){BItEmyQHPc = true;}
      if(TVKZMyhHOU == DEnOufIZPw){SRqotnuubt = true;}
      else if(DEnOufIZPw == TVKZMyhHOU){MHUktftVAb = true;}
      if(osXhWRekjE == KKyCfjFLra){JbVaWeNzyC = true;}
      else if(KKyCfjFLra == osXhWRekjE){iNpFjNJuyU = true;}
      if(KgMAYmuxfP == oAogtgZmMy){ZusQQgxjub = true;}
      else if(oAogtgZmMy == KgMAYmuxfP){nmFtunAcft = true;}
      if(akWhtSxCkm == pYdZtoeOUa){ZFqZbZiSSe = true;}
      if(VXDNoasbSP == JnGCwzozfz){JQHjgxCSip = true;}
      if(IIWzimJJAZ == ekMsuDOGUO){aaoskWXaXN = true;}
      while(pYdZtoeOUa == akWhtSxCkm){rftGFWJPKV = true;}
      while(JnGCwzozfz == JnGCwzozfz){EtxMVnRoUZ = true;}
      while(ekMsuDOGUO == ekMsuDOGUO){oUJtnZBZoF = true;}
      if(FUMRGlfXHq == true){FUMRGlfXHq = false;}
      if(FqpQQBsmkt == true){FqpQQBsmkt = false;}
      if(qaHMSELjdq == true){qaHMSELjdq = false;}
      if(ZXtGVnanuK == true){ZXtGVnanuK = false;}
      if(SRqotnuubt == true){SRqotnuubt = false;}
      if(JbVaWeNzyC == true){JbVaWeNzyC = false;}
      if(ZusQQgxjub == true){ZusQQgxjub = false;}
      if(ZFqZbZiSSe == true){ZFqZbZiSSe = false;}
      if(JQHjgxCSip == true){JQHjgxCSip = false;}
      if(aaoskWXaXN == true){aaoskWXaXN = false;}
      if(qKAATEkuyx == true){qKAATEkuyx = false;}
      if(wugTsImJjS == true){wugTsImJjS = false;}
      if(ddBEByPxoE == true){ddBEByPxoE = false;}
      if(BItEmyQHPc == true){BItEmyQHPc = false;}
      if(MHUktftVAb == true){MHUktftVAb = false;}
      if(iNpFjNJuyU == true){iNpFjNJuyU = false;}
      if(nmFtunAcft == true){nmFtunAcft = false;}
      if(rftGFWJPKV == true){rftGFWJPKV = false;}
      if(EtxMVnRoUZ == true){EtxMVnRoUZ = false;}
      if(oUJtnZBZoF == true){oUJtnZBZoF = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BAEEUSRSCI
{ 
  void blZLiYWxNR()
  { 
      bool OQKkBxtoTk = false;
      bool jfwfBlNbzo = false;
      bool gyHESMJQpq = false;
      bool NYXUqPNiua = false;
      bool TuLUrIJwVc = false;
      bool ZdilWQIFSn = false;
      bool nczcXLHbok = false;
      bool YefYVkmlMJ = false;
      bool DMuNRiokxZ = false;
      bool GHQHlpwhxf = false;
      bool AgGjEtxKQk = false;
      bool cWsIjahlKn = false;
      bool WWdOdROYlf = false;
      bool oEofIVFKJX = false;
      bool keUMxjNHWK = false;
      bool IKXIJdDrGl = false;
      bool eYXjwYgMzO = false;
      bool VWbmDrBQRt = false;
      bool DGBaWdggWe = false;
      bool SNEWrMGjQX = false;
      string TjbKQbpGRK;
      string LQRzpMiNnm;
      string uMwmhGjlMl;
      string srPFeotXeH;
      string pWFRXGYnkM;
      string lcWcYHnocK;
      string qDteRTBMUA;
      string RLhgwYHeOa;
      string huAAAQUmfX;
      string BFtUPmcZAD;
      string FMTktaupgA;
      string AIZCbuCteA;
      string yNfxyMQMQI;
      string wgeLUUbPFY;
      string VeGfXrerrT;
      string jiuzErfVlh;
      string KhYXDSdOgH;
      string SmnFeTHAYF;
      string gXTwXNkeGc;
      string nGcWOmGibE;
      if(TjbKQbpGRK == FMTktaupgA){OQKkBxtoTk = true;}
      else if(FMTktaupgA == TjbKQbpGRK){AgGjEtxKQk = true;}
      if(LQRzpMiNnm == AIZCbuCteA){jfwfBlNbzo = true;}
      else if(AIZCbuCteA == LQRzpMiNnm){cWsIjahlKn = true;}
      if(uMwmhGjlMl == yNfxyMQMQI){gyHESMJQpq = true;}
      else if(yNfxyMQMQI == uMwmhGjlMl){WWdOdROYlf = true;}
      if(srPFeotXeH == wgeLUUbPFY){NYXUqPNiua = true;}
      else if(wgeLUUbPFY == srPFeotXeH){oEofIVFKJX = true;}
      if(pWFRXGYnkM == VeGfXrerrT){TuLUrIJwVc = true;}
      else if(VeGfXrerrT == pWFRXGYnkM){keUMxjNHWK = true;}
      if(lcWcYHnocK == jiuzErfVlh){ZdilWQIFSn = true;}
      else if(jiuzErfVlh == lcWcYHnocK){IKXIJdDrGl = true;}
      if(qDteRTBMUA == KhYXDSdOgH){nczcXLHbok = true;}
      else if(KhYXDSdOgH == qDteRTBMUA){eYXjwYgMzO = true;}
      if(RLhgwYHeOa == SmnFeTHAYF){YefYVkmlMJ = true;}
      if(huAAAQUmfX == gXTwXNkeGc){DMuNRiokxZ = true;}
      if(BFtUPmcZAD == nGcWOmGibE){GHQHlpwhxf = true;}
      while(SmnFeTHAYF == RLhgwYHeOa){VWbmDrBQRt = true;}
      while(gXTwXNkeGc == gXTwXNkeGc){DGBaWdggWe = true;}
      while(nGcWOmGibE == nGcWOmGibE){SNEWrMGjQX = true;}
      if(OQKkBxtoTk == true){OQKkBxtoTk = false;}
      if(jfwfBlNbzo == true){jfwfBlNbzo = false;}
      if(gyHESMJQpq == true){gyHESMJQpq = false;}
      if(NYXUqPNiua == true){NYXUqPNiua = false;}
      if(TuLUrIJwVc == true){TuLUrIJwVc = false;}
      if(ZdilWQIFSn == true){ZdilWQIFSn = false;}
      if(nczcXLHbok == true){nczcXLHbok = false;}
      if(YefYVkmlMJ == true){YefYVkmlMJ = false;}
      if(DMuNRiokxZ == true){DMuNRiokxZ = false;}
      if(GHQHlpwhxf == true){GHQHlpwhxf = false;}
      if(AgGjEtxKQk == true){AgGjEtxKQk = false;}
      if(cWsIjahlKn == true){cWsIjahlKn = false;}
      if(WWdOdROYlf == true){WWdOdROYlf = false;}
      if(oEofIVFKJX == true){oEofIVFKJX = false;}
      if(keUMxjNHWK == true){keUMxjNHWK = false;}
      if(IKXIJdDrGl == true){IKXIJdDrGl = false;}
      if(eYXjwYgMzO == true){eYXjwYgMzO = false;}
      if(VWbmDrBQRt == true){VWbmDrBQRt = false;}
      if(DGBaWdggWe == true){DGBaWdggWe = false;}
      if(SNEWrMGjQX == true){SNEWrMGjQX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SCIYSCOBPA
{ 
  void rPGfSzdTiu()
  { 
      bool HxKFnMLEWc = false;
      bool oAuKCAwOrN = false;
      bool KUHIOAAhsR = false;
      bool UIWTlTZBqj = false;
      bool UrbjeUCXPx = false;
      bool BdEAUmpBqn = false;
      bool LdLEHgdbYi = false;
      bool ZtXHsSsWcs = false;
      bool ycRwQqEVyz = false;
      bool cXKDYqgWby = false;
      bool qZzgWGFwHC = false;
      bool sTYxohLOnQ = false;
      bool cHcYFBjUiB = false;
      bool lVueCXdkYJ = false;
      bool kHOwFJzMNp = false;
      bool fRhKUjwZaV = false;
      bool WkMFPXXtuR = false;
      bool EaoefUrgTu = false;
      bool CWGrVUnjJR = false;
      bool aDJDmzuJfh = false;
      string PzYrePJEUo;
      string QaSjVsceAG;
      string YzGzzFseYE;
      string ZMOcFWRYEC;
      string wfkSzyUOVg;
      string MZLsfPrZsZ;
      string WefQPEMpey;
      string bDayKsNnzA;
      string jdJrqzBUwp;
      string erfeeRAUCt;
      string ldjWEJzNim;
      string BEZrorVnYP;
      string CqsnTjXwSs;
      string rskFOFslmh;
      string AnlInZRmom;
      string uLumrtkuDU;
      string kCtotISUci;
      string yiAqPOEkAs;
      string RjGexfxyNU;
      string UyhSxxswOf;
      if(PzYrePJEUo == ldjWEJzNim){HxKFnMLEWc = true;}
      else if(ldjWEJzNim == PzYrePJEUo){qZzgWGFwHC = true;}
      if(QaSjVsceAG == BEZrorVnYP){oAuKCAwOrN = true;}
      else if(BEZrorVnYP == QaSjVsceAG){sTYxohLOnQ = true;}
      if(YzGzzFseYE == CqsnTjXwSs){KUHIOAAhsR = true;}
      else if(CqsnTjXwSs == YzGzzFseYE){cHcYFBjUiB = true;}
      if(ZMOcFWRYEC == rskFOFslmh){UIWTlTZBqj = true;}
      else if(rskFOFslmh == ZMOcFWRYEC){lVueCXdkYJ = true;}
      if(wfkSzyUOVg == AnlInZRmom){UrbjeUCXPx = true;}
      else if(AnlInZRmom == wfkSzyUOVg){kHOwFJzMNp = true;}
      if(MZLsfPrZsZ == uLumrtkuDU){BdEAUmpBqn = true;}
      else if(uLumrtkuDU == MZLsfPrZsZ){fRhKUjwZaV = true;}
      if(WefQPEMpey == kCtotISUci){LdLEHgdbYi = true;}
      else if(kCtotISUci == WefQPEMpey){WkMFPXXtuR = true;}
      if(bDayKsNnzA == yiAqPOEkAs){ZtXHsSsWcs = true;}
      if(jdJrqzBUwp == RjGexfxyNU){ycRwQqEVyz = true;}
      if(erfeeRAUCt == UyhSxxswOf){cXKDYqgWby = true;}
      while(yiAqPOEkAs == bDayKsNnzA){EaoefUrgTu = true;}
      while(RjGexfxyNU == RjGexfxyNU){CWGrVUnjJR = true;}
      while(UyhSxxswOf == UyhSxxswOf){aDJDmzuJfh = true;}
      if(HxKFnMLEWc == true){HxKFnMLEWc = false;}
      if(oAuKCAwOrN == true){oAuKCAwOrN = false;}
      if(KUHIOAAhsR == true){KUHIOAAhsR = false;}
      if(UIWTlTZBqj == true){UIWTlTZBqj = false;}
      if(UrbjeUCXPx == true){UrbjeUCXPx = false;}
      if(BdEAUmpBqn == true){BdEAUmpBqn = false;}
      if(LdLEHgdbYi == true){LdLEHgdbYi = false;}
      if(ZtXHsSsWcs == true){ZtXHsSsWcs = false;}
      if(ycRwQqEVyz == true){ycRwQqEVyz = false;}
      if(cXKDYqgWby == true){cXKDYqgWby = false;}
      if(qZzgWGFwHC == true){qZzgWGFwHC = false;}
      if(sTYxohLOnQ == true){sTYxohLOnQ = false;}
      if(cHcYFBjUiB == true){cHcYFBjUiB = false;}
      if(lVueCXdkYJ == true){lVueCXdkYJ = false;}
      if(kHOwFJzMNp == true){kHOwFJzMNp = false;}
      if(fRhKUjwZaV == true){fRhKUjwZaV = false;}
      if(WkMFPXXtuR == true){WkMFPXXtuR = false;}
      if(EaoefUrgTu == true){EaoefUrgTu = false;}
      if(CWGrVUnjJR == true){CWGrVUnjJR = false;}
      if(aDJDmzuJfh == true){aDJDmzuJfh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class MLGFEJMLBL
{ 
  void gKoyYWPOqI()
  { 
      bool RRtJJwGctU = false;
      bool jkXGkpbgAQ = false;
      bool dsgAbIxCjy = false;
      bool KatWGpnoLm = false;
      bool drHRzzSeml = false;
      bool VJXROYjshB = false;
      bool zEruKIpAAu = false;
      bool BmVNlllPBb = false;
      bool pROycBDcHh = false;
      bool FuHKmZzLHp = false;
      bool JRerjhmKec = false;
      bool lUqFLhotSJ = false;
      bool FNrRoGfLaM = false;
      bool lZIKRXlWNt = false;
      bool NrxeukDBzF = false;
      bool kEsykwYMCc = false;
      bool CbjSpMMgmm = false;
      bool uSfXuMPNWE = false;
      bool DeLUbgfJZV = false;
      bool yZTkrkIenh = false;
      string fwMqOcNARK;
      string xUsZbXuYGE;
      string XMFLZaahBS;
      string TaCXSqPAVo;
      string KbBBlLyBBu;
      string gzzsRSpBRn;
      string CezzpVRUGk;
      string IuDRtwLDYN;
      string bYfFlUaIGz;
      string AOhhlecBLQ;
      string scuPmkwhzV;
      string HmbELcztMU;
      string tJOHksMUeO;
      string TJrITzdQRA;
      string YsJkqMldVJ;
      string ehopVjOmes;
      string YaCjByuTjM;
      string zibMwBAdPX;
      string HhyVjkUBli;
      string jpkYbxpYis;
      if(fwMqOcNARK == scuPmkwhzV){RRtJJwGctU = true;}
      else if(scuPmkwhzV == fwMqOcNARK){JRerjhmKec = true;}
      if(xUsZbXuYGE == HmbELcztMU){jkXGkpbgAQ = true;}
      else if(HmbELcztMU == xUsZbXuYGE){lUqFLhotSJ = true;}
      if(XMFLZaahBS == tJOHksMUeO){dsgAbIxCjy = true;}
      else if(tJOHksMUeO == XMFLZaahBS){FNrRoGfLaM = true;}
      if(TaCXSqPAVo == TJrITzdQRA){KatWGpnoLm = true;}
      else if(TJrITzdQRA == TaCXSqPAVo){lZIKRXlWNt = true;}
      if(KbBBlLyBBu == YsJkqMldVJ){drHRzzSeml = true;}
      else if(YsJkqMldVJ == KbBBlLyBBu){NrxeukDBzF = true;}
      if(gzzsRSpBRn == ehopVjOmes){VJXROYjshB = true;}
      else if(ehopVjOmes == gzzsRSpBRn){kEsykwYMCc = true;}
      if(CezzpVRUGk == YaCjByuTjM){zEruKIpAAu = true;}
      else if(YaCjByuTjM == CezzpVRUGk){CbjSpMMgmm = true;}
      if(IuDRtwLDYN == zibMwBAdPX){BmVNlllPBb = true;}
      if(bYfFlUaIGz == HhyVjkUBli){pROycBDcHh = true;}
      if(AOhhlecBLQ == jpkYbxpYis){FuHKmZzLHp = true;}
      while(zibMwBAdPX == IuDRtwLDYN){uSfXuMPNWE = true;}
      while(HhyVjkUBli == HhyVjkUBli){DeLUbgfJZV = true;}
      while(jpkYbxpYis == jpkYbxpYis){yZTkrkIenh = true;}
      if(RRtJJwGctU == true){RRtJJwGctU = false;}
      if(jkXGkpbgAQ == true){jkXGkpbgAQ = false;}
      if(dsgAbIxCjy == true){dsgAbIxCjy = false;}
      if(KatWGpnoLm == true){KatWGpnoLm = false;}
      if(drHRzzSeml == true){drHRzzSeml = false;}
      if(VJXROYjshB == true){VJXROYjshB = false;}
      if(zEruKIpAAu == true){zEruKIpAAu = false;}
      if(BmVNlllPBb == true){BmVNlllPBb = false;}
      if(pROycBDcHh == true){pROycBDcHh = false;}
      if(FuHKmZzLHp == true){FuHKmZzLHp = false;}
      if(JRerjhmKec == true){JRerjhmKec = false;}
      if(lUqFLhotSJ == true){lUqFLhotSJ = false;}
      if(FNrRoGfLaM == true){FNrRoGfLaM = false;}
      if(lZIKRXlWNt == true){lZIKRXlWNt = false;}
      if(NrxeukDBzF == true){NrxeukDBzF = false;}
      if(kEsykwYMCc == true){kEsykwYMCc = false;}
      if(CbjSpMMgmm == true){CbjSpMMgmm = false;}
      if(uSfXuMPNWE == true){uSfXuMPNWE = false;}
      if(DeLUbgfJZV == true){DeLUbgfJZV = false;}
      if(yZTkrkIenh == true){yZTkrkIenh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZLJGIHRREW
{ 
  void YOHouaGMpW()
  { 
      bool wTRRxEsTuF = false;
      bool jeKwphPKkd = false;
      bool phxynqSIbu = false;
      bool sqkmsEcHjV = false;
      bool xLFLXpBNkV = false;
      bool qFjFoOVSdF = false;
      bool ljrzMrtXCl = false;
      bool EHZPfdbkax = false;
      bool pFXFEmGexe = false;
      bool hKdeiNTFze = false;
      bool wdKyQjfUYK = false;
      bool lkqfPgLluY = false;
      bool lOaXDtctfS = false;
      bool KVkwBMtYnT = false;
      bool HlFlrxSyPC = false;
      bool BgKZshCmuW = false;
      bool GkoGAQzudC = false;
      bool lmyaRAIneF = false;
      bool beecQdLLCT = false;
      bool JGdRNVwzlb = false;
      string zICtMpRyYX;
      string keWaXZylFt;
      string GCeskmIqPf;
      string lliDBpnteb;
      string fkHifmrVnF;
      string ByDhKxcJJY;
      string gairlzwSYh;
      string kEHcNVuuNx;
      string tmCJdmhBsE;
      string MekFuFBOwf;
      string qYxGUwqZTn;
      string ChwMdtXYNz;
      string QtZTUGTBhI;
      string TfygFUouoh;
      string wHgEBeiYbB;
      string gcAxOxzclN;
      string coqLUViOGJ;
      string EcjfCJMUSp;
      string XuZgaqfARH;
      string UMJaEGPVje;
      if(zICtMpRyYX == qYxGUwqZTn){wTRRxEsTuF = true;}
      else if(qYxGUwqZTn == zICtMpRyYX){wdKyQjfUYK = true;}
      if(keWaXZylFt == ChwMdtXYNz){jeKwphPKkd = true;}
      else if(ChwMdtXYNz == keWaXZylFt){lkqfPgLluY = true;}
      if(GCeskmIqPf == QtZTUGTBhI){phxynqSIbu = true;}
      else if(QtZTUGTBhI == GCeskmIqPf){lOaXDtctfS = true;}
      if(lliDBpnteb == TfygFUouoh){sqkmsEcHjV = true;}
      else if(TfygFUouoh == lliDBpnteb){KVkwBMtYnT = true;}
      if(fkHifmrVnF == wHgEBeiYbB){xLFLXpBNkV = true;}
      else if(wHgEBeiYbB == fkHifmrVnF){HlFlrxSyPC = true;}
      if(ByDhKxcJJY == gcAxOxzclN){qFjFoOVSdF = true;}
      else if(gcAxOxzclN == ByDhKxcJJY){BgKZshCmuW = true;}
      if(gairlzwSYh == coqLUViOGJ){ljrzMrtXCl = true;}
      else if(coqLUViOGJ == gairlzwSYh){GkoGAQzudC = true;}
      if(kEHcNVuuNx == EcjfCJMUSp){EHZPfdbkax = true;}
      if(tmCJdmhBsE == XuZgaqfARH){pFXFEmGexe = true;}
      if(MekFuFBOwf == UMJaEGPVje){hKdeiNTFze = true;}
      while(EcjfCJMUSp == kEHcNVuuNx){lmyaRAIneF = true;}
      while(XuZgaqfARH == XuZgaqfARH){beecQdLLCT = true;}
      while(UMJaEGPVje == UMJaEGPVje){JGdRNVwzlb = true;}
      if(wTRRxEsTuF == true){wTRRxEsTuF = false;}
      if(jeKwphPKkd == true){jeKwphPKkd = false;}
      if(phxynqSIbu == true){phxynqSIbu = false;}
      if(sqkmsEcHjV == true){sqkmsEcHjV = false;}
      if(xLFLXpBNkV == true){xLFLXpBNkV = false;}
      if(qFjFoOVSdF == true){qFjFoOVSdF = false;}
      if(ljrzMrtXCl == true){ljrzMrtXCl = false;}
      if(EHZPfdbkax == true){EHZPfdbkax = false;}
      if(pFXFEmGexe == true){pFXFEmGexe = false;}
      if(hKdeiNTFze == true){hKdeiNTFze = false;}
      if(wdKyQjfUYK == true){wdKyQjfUYK = false;}
      if(lkqfPgLluY == true){lkqfPgLluY = false;}
      if(lOaXDtctfS == true){lOaXDtctfS = false;}
      if(KVkwBMtYnT == true){KVkwBMtYnT = false;}
      if(HlFlrxSyPC == true){HlFlrxSyPC = false;}
      if(BgKZshCmuW == true){BgKZshCmuW = false;}
      if(GkoGAQzudC == true){GkoGAQzudC = false;}
      if(lmyaRAIneF == true){lmyaRAIneF = false;}
      if(beecQdLLCT == true){beecQdLLCT = false;}
      if(JGdRNVwzlb == true){JGdRNVwzlb = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class INSWXAERXU
{ 
  void zQUTMQjXXw()
  { 
      bool bAhxunqCHe = false;
      bool jOhsuoTwLo = false;
      bool LtlFRjqzXV = false;
      bool DxNMTROZlY = false;
      bool rUAkOZjWLR = false;
      bool DukPWgObeK = false;
      bool xqelamAUZd = false;
      bool atKmwptApq = false;
      bool AUnGRILypl = false;
      bool CltaaApNJb = false;
      bool YOxhpJkxSf = false;
      bool mNlnJbDXTS = false;
      bool aCdmanziLu = false;
      bool AfqNRlWGPI = false;
      bool WOeKynzGRz = false;
      bool xRtqAWjJGB = false;
      bool JBkWeGWXVX = false;
      bool ExILuMWTom = false;
      bool osRMyilzNu = false;
      bool BNzNyUcIfr = false;
      string tABEhVnZdt;
      string IdTKrloJGK;
      string yzUYsHmFzq;
      string EMHoxYWiEe;
      string qwqAoRICjx;
      string cWiCGtfbLZ;
      string ihalpwVeNT;
      string PGEaSBITja;
      string zjVBOgdFhC;
      string rtWdwJVEKs;
      string udnQhVRHiE;
      string lMrjayMYXI;
      string RQwKaUiZdJ;
      string fDJpeUuETs;
      string PVWdPeCmDx;
      string wwgBaegcoN;
      string lqcYYhCBFF;
      string CHFeswtSuw;
      string SwWgNiWIPz;
      string nSPGGXrxEh;
      if(tABEhVnZdt == udnQhVRHiE){bAhxunqCHe = true;}
      else if(udnQhVRHiE == tABEhVnZdt){YOxhpJkxSf = true;}
      if(IdTKrloJGK == lMrjayMYXI){jOhsuoTwLo = true;}
      else if(lMrjayMYXI == IdTKrloJGK){mNlnJbDXTS = true;}
      if(yzUYsHmFzq == RQwKaUiZdJ){LtlFRjqzXV = true;}
      else if(RQwKaUiZdJ == yzUYsHmFzq){aCdmanziLu = true;}
      if(EMHoxYWiEe == fDJpeUuETs){DxNMTROZlY = true;}
      else if(fDJpeUuETs == EMHoxYWiEe){AfqNRlWGPI = true;}
      if(qwqAoRICjx == PVWdPeCmDx){rUAkOZjWLR = true;}
      else if(PVWdPeCmDx == qwqAoRICjx){WOeKynzGRz = true;}
      if(cWiCGtfbLZ == wwgBaegcoN){DukPWgObeK = true;}
      else if(wwgBaegcoN == cWiCGtfbLZ){xRtqAWjJGB = true;}
      if(ihalpwVeNT == lqcYYhCBFF){xqelamAUZd = true;}
      else if(lqcYYhCBFF == ihalpwVeNT){JBkWeGWXVX = true;}
      if(PGEaSBITja == CHFeswtSuw){atKmwptApq = true;}
      if(zjVBOgdFhC == SwWgNiWIPz){AUnGRILypl = true;}
      if(rtWdwJVEKs == nSPGGXrxEh){CltaaApNJb = true;}
      while(CHFeswtSuw == PGEaSBITja){ExILuMWTom = true;}
      while(SwWgNiWIPz == SwWgNiWIPz){osRMyilzNu = true;}
      while(nSPGGXrxEh == nSPGGXrxEh){BNzNyUcIfr = true;}
      if(bAhxunqCHe == true){bAhxunqCHe = false;}
      if(jOhsuoTwLo == true){jOhsuoTwLo = false;}
      if(LtlFRjqzXV == true){LtlFRjqzXV = false;}
      if(DxNMTROZlY == true){DxNMTROZlY = false;}
      if(rUAkOZjWLR == true){rUAkOZjWLR = false;}
      if(DukPWgObeK == true){DukPWgObeK = false;}
      if(xqelamAUZd == true){xqelamAUZd = false;}
      if(atKmwptApq == true){atKmwptApq = false;}
      if(AUnGRILypl == true){AUnGRILypl = false;}
      if(CltaaApNJb == true){CltaaApNJb = false;}
      if(YOxhpJkxSf == true){YOxhpJkxSf = false;}
      if(mNlnJbDXTS == true){mNlnJbDXTS = false;}
      if(aCdmanziLu == true){aCdmanziLu = false;}
      if(AfqNRlWGPI == true){AfqNRlWGPI = false;}
      if(WOeKynzGRz == true){WOeKynzGRz = false;}
      if(xRtqAWjJGB == true){xRtqAWjJGB = false;}
      if(JBkWeGWXVX == true){JBkWeGWXVX = false;}
      if(ExILuMWTom == true){ExILuMWTom = false;}
      if(osRMyilzNu == true){osRMyilzNu = false;}
      if(BNzNyUcIfr == true){BNzNyUcIfr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BVLTLKVUNH
{ 
  void kEiSfNcreX()
  { 
      bool KWpUszchGi = false;
      bool smdIFauWeV = false;
      bool KuzlmACyQT = false;
      bool dPegCRMoEh = false;
      bool NlzmlkJIJC = false;
      bool qAgsiPXEhr = false;
      bool xeQlEdIwGm = false;
      bool NGSxIXwMzC = false;
      bool qtnOcdhlyl = false;
      bool tIHLkiUMhI = false;
      bool bMCWnjhjkK = false;
      bool SLEeBDpCIa = false;
      bool uVyTsFzHMY = false;
      bool DidqZdqRZs = false;
      bool uyBnTiDbJh = false;
      bool rSDGMGOich = false;
      bool jZWLHigMEW = false;
      bool LXbPexUKAK = false;
      bool CWxefjeXeT = false;
      bool PInqIVaofj = false;
      string PacjaZSZhL;
      string aTGzgpLKlm;
      string XLcKoCxfPf;
      string YlcOCcnWWr;
      string MZiyVOtAyd;
      string fjsorDxStY;
      string cmJCmnpqxE;
      string ConsaycdoZ;
      string HwgkznrnVy;
      string eVbxOZPfLg;
      string YCDtaqwzOt;
      string aTcTQgFYDy;
      string uCUyYgMpeB;
      string hwSHBFrlJI;
      string rsZaXLrIgg;
      string TqcMlVgEVt;
      string eFUMMdAURL;
      string nTOtGCbtaM;
      string oDJxhpTFVf;
      string XmQPdlXMOs;
      if(PacjaZSZhL == YCDtaqwzOt){KWpUszchGi = true;}
      else if(YCDtaqwzOt == PacjaZSZhL){bMCWnjhjkK = true;}
      if(aTGzgpLKlm == aTcTQgFYDy){smdIFauWeV = true;}
      else if(aTcTQgFYDy == aTGzgpLKlm){SLEeBDpCIa = true;}
      if(XLcKoCxfPf == uCUyYgMpeB){KuzlmACyQT = true;}
      else if(uCUyYgMpeB == XLcKoCxfPf){uVyTsFzHMY = true;}
      if(YlcOCcnWWr == hwSHBFrlJI){dPegCRMoEh = true;}
      else if(hwSHBFrlJI == YlcOCcnWWr){DidqZdqRZs = true;}
      if(MZiyVOtAyd == rsZaXLrIgg){NlzmlkJIJC = true;}
      else if(rsZaXLrIgg == MZiyVOtAyd){uyBnTiDbJh = true;}
      if(fjsorDxStY == TqcMlVgEVt){qAgsiPXEhr = true;}
      else if(TqcMlVgEVt == fjsorDxStY){rSDGMGOich = true;}
      if(cmJCmnpqxE == eFUMMdAURL){xeQlEdIwGm = true;}
      else if(eFUMMdAURL == cmJCmnpqxE){jZWLHigMEW = true;}
      if(ConsaycdoZ == nTOtGCbtaM){NGSxIXwMzC = true;}
      if(HwgkznrnVy == oDJxhpTFVf){qtnOcdhlyl = true;}
      if(eVbxOZPfLg == XmQPdlXMOs){tIHLkiUMhI = true;}
      while(nTOtGCbtaM == ConsaycdoZ){LXbPexUKAK = true;}
      while(oDJxhpTFVf == oDJxhpTFVf){CWxefjeXeT = true;}
      while(XmQPdlXMOs == XmQPdlXMOs){PInqIVaofj = true;}
      if(KWpUszchGi == true){KWpUszchGi = false;}
      if(smdIFauWeV == true){smdIFauWeV = false;}
      if(KuzlmACyQT == true){KuzlmACyQT = false;}
      if(dPegCRMoEh == true){dPegCRMoEh = false;}
      if(NlzmlkJIJC == true){NlzmlkJIJC = false;}
      if(qAgsiPXEhr == true){qAgsiPXEhr = false;}
      if(xeQlEdIwGm == true){xeQlEdIwGm = false;}
      if(NGSxIXwMzC == true){NGSxIXwMzC = false;}
      if(qtnOcdhlyl == true){qtnOcdhlyl = false;}
      if(tIHLkiUMhI == true){tIHLkiUMhI = false;}
      if(bMCWnjhjkK == true){bMCWnjhjkK = false;}
      if(SLEeBDpCIa == true){SLEeBDpCIa = false;}
      if(uVyTsFzHMY == true){uVyTsFzHMY = false;}
      if(DidqZdqRZs == true){DidqZdqRZs = false;}
      if(uyBnTiDbJh == true){uyBnTiDbJh = false;}
      if(rSDGMGOich == true){rSDGMGOich = false;}
      if(jZWLHigMEW == true){jZWLHigMEW = false;}
      if(LXbPexUKAK == true){LXbPexUKAK = false;}
      if(CWxefjeXeT == true){CWxefjeXeT = false;}
      if(PInqIVaofj == true){PInqIVaofj = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PNFQGPGMMB
{ 
  void ueOtFCEVYV()
  { 
      bool NsiYhyqQGu = false;
      bool EDhkqRMKZT = false;
      bool wuhpgJHNgH = false;
      bool nppQIXcTjj = false;
      bool nRFcCKGzUV = false;
      bool YTrTnSLrqY = false;
      bool pdjzGOMRbS = false;
      bool nsZpeeoEnW = false;
      bool CIrKhUxVcp = false;
      bool MkbLsKgFWn = false;
      bool xoXBgfsCoI = false;
      bool cGEUVhkYRl = false;
      bool QqokGDROwU = false;
      bool xjgzYNjqcC = false;
      bool augTNFfGoL = false;
      bool YInjXGAluc = false;
      bool NeGUKrZEwH = false;
      bool xtuOLUKUXA = false;
      bool WLZkuowxaS = false;
      bool MyuAnwnLIU = false;
      string bsFNhIxPao;
      string fmqnmbqSIy;
      string FlbgUwJWcD;
      string wSqoGGRuwD;
      string AxtdDxBifi;
      string RnhgEpRXrk;
      string yamXsMwCgg;
      string KuWFyHqxdW;
      string taqslIkKjP;
      string osCKFZKWkp;
      string AzBzYVQchw;
      string zTtUpXiskj;
      string blRRqQsLzY;
      string xTMmWuYqTQ;
      string qHbmPUzgTg;
      string pQlAOOgjHI;
      string cEcJzeHGyu;
      string yqYUsTJdBW;
      string QLrSifhMYH;
      string NojLmleSnk;
      if(bsFNhIxPao == AzBzYVQchw){NsiYhyqQGu = true;}
      else if(AzBzYVQchw == bsFNhIxPao){xoXBgfsCoI = true;}
      if(fmqnmbqSIy == zTtUpXiskj){EDhkqRMKZT = true;}
      else if(zTtUpXiskj == fmqnmbqSIy){cGEUVhkYRl = true;}
      if(FlbgUwJWcD == blRRqQsLzY){wuhpgJHNgH = true;}
      else if(blRRqQsLzY == FlbgUwJWcD){QqokGDROwU = true;}
      if(wSqoGGRuwD == xTMmWuYqTQ){nppQIXcTjj = true;}
      else if(xTMmWuYqTQ == wSqoGGRuwD){xjgzYNjqcC = true;}
      if(AxtdDxBifi == qHbmPUzgTg){nRFcCKGzUV = true;}
      else if(qHbmPUzgTg == AxtdDxBifi){augTNFfGoL = true;}
      if(RnhgEpRXrk == pQlAOOgjHI){YTrTnSLrqY = true;}
      else if(pQlAOOgjHI == RnhgEpRXrk){YInjXGAluc = true;}
      if(yamXsMwCgg == cEcJzeHGyu){pdjzGOMRbS = true;}
      else if(cEcJzeHGyu == yamXsMwCgg){NeGUKrZEwH = true;}
      if(KuWFyHqxdW == yqYUsTJdBW){nsZpeeoEnW = true;}
      if(taqslIkKjP == QLrSifhMYH){CIrKhUxVcp = true;}
      if(osCKFZKWkp == NojLmleSnk){MkbLsKgFWn = true;}
      while(yqYUsTJdBW == KuWFyHqxdW){xtuOLUKUXA = true;}
      while(QLrSifhMYH == QLrSifhMYH){WLZkuowxaS = true;}
      while(NojLmleSnk == NojLmleSnk){MyuAnwnLIU = true;}
      if(NsiYhyqQGu == true){NsiYhyqQGu = false;}
      if(EDhkqRMKZT == true){EDhkqRMKZT = false;}
      if(wuhpgJHNgH == true){wuhpgJHNgH = false;}
      if(nppQIXcTjj == true){nppQIXcTjj = false;}
      if(nRFcCKGzUV == true){nRFcCKGzUV = false;}
      if(YTrTnSLrqY == true){YTrTnSLrqY = false;}
      if(pdjzGOMRbS == true){pdjzGOMRbS = false;}
      if(nsZpeeoEnW == true){nsZpeeoEnW = false;}
      if(CIrKhUxVcp == true){CIrKhUxVcp = false;}
      if(MkbLsKgFWn == true){MkbLsKgFWn = false;}
      if(xoXBgfsCoI == true){xoXBgfsCoI = false;}
      if(cGEUVhkYRl == true){cGEUVhkYRl = false;}
      if(QqokGDROwU == true){QqokGDROwU = false;}
      if(xjgzYNjqcC == true){xjgzYNjqcC = false;}
      if(augTNFfGoL == true){augTNFfGoL = false;}
      if(YInjXGAluc == true){YInjXGAluc = false;}
      if(NeGUKrZEwH == true){NeGUKrZEwH = false;}
      if(xtuOLUKUXA == true){xtuOLUKUXA = false;}
      if(WLZkuowxaS == true){WLZkuowxaS = false;}
      if(MyuAnwnLIU == true){MyuAnwnLIU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class QQYCCJZQTS
{ 
  void BlHqTfQLDM()
  { 
      bool LSwSxZBBcS = false;
      bool cMJeyHKtJx = false;
      bool wSuTGxiRKH = false;
      bool ZWycfVlcwV = false;
      bool ONwnuGQLrK = false;
      bool olzXmrWcMz = false;
      bool pjqVrFNgzc = false;
      bool VgxmhhWHXz = false;
      bool fXYzSnXZom = false;
      bool zIQwfeWaup = false;
      bool sjMPoHnMpE = false;
      bool VOEMTSKqCE = false;
      bool hmqLdMPAHD = false;
      bool LuMhGuLJKL = false;
      bool zKjkRjDEcM = false;
      bool UfYqIFsBkb = false;
      bool rnFlsZCWGX = false;
      bool HAfqEWDGst = false;
      bool ILWtifWUYB = false;
      bool CKYhTLFxqq = false;
      string RhmNLSPAhV;
      string eNUiZdJoAj;
      string puUZCzSzkW;
      string GhBqYVQYfr;
      string COfMuQNoVJ;
      string yEKjRWMMLL;
      string APPLOwLcKE;
      string MkIHhaPIel;
      string kcdaXBWQZC;
      string IDNTrlzTxy;
      string EjeRwNuDBp;
      string GEhYaWlnnM;
      string ZuDHuJWoPw;
      string buKfgAyMeJ;
      string FTOiuEUnsV;
      string zLfkYJgWVm;
      string pGnFNLsCrZ;
      string YZJdHFfTMq;
      string niwswQPZGX;
      string TapqSzfIVi;
      if(RhmNLSPAhV == EjeRwNuDBp){LSwSxZBBcS = true;}
      else if(EjeRwNuDBp == RhmNLSPAhV){sjMPoHnMpE = true;}
      if(eNUiZdJoAj == GEhYaWlnnM){cMJeyHKtJx = true;}
      else if(GEhYaWlnnM == eNUiZdJoAj){VOEMTSKqCE = true;}
      if(puUZCzSzkW == ZuDHuJWoPw){wSuTGxiRKH = true;}
      else if(ZuDHuJWoPw == puUZCzSzkW){hmqLdMPAHD = true;}
      if(GhBqYVQYfr == buKfgAyMeJ){ZWycfVlcwV = true;}
      else if(buKfgAyMeJ == GhBqYVQYfr){LuMhGuLJKL = true;}
      if(COfMuQNoVJ == FTOiuEUnsV){ONwnuGQLrK = true;}
      else if(FTOiuEUnsV == COfMuQNoVJ){zKjkRjDEcM = true;}
      if(yEKjRWMMLL == zLfkYJgWVm){olzXmrWcMz = true;}
      else if(zLfkYJgWVm == yEKjRWMMLL){UfYqIFsBkb = true;}
      if(APPLOwLcKE == pGnFNLsCrZ){pjqVrFNgzc = true;}
      else if(pGnFNLsCrZ == APPLOwLcKE){rnFlsZCWGX = true;}
      if(MkIHhaPIel == YZJdHFfTMq){VgxmhhWHXz = true;}
      if(kcdaXBWQZC == niwswQPZGX){fXYzSnXZom = true;}
      if(IDNTrlzTxy == TapqSzfIVi){zIQwfeWaup = true;}
      while(YZJdHFfTMq == MkIHhaPIel){HAfqEWDGst = true;}
      while(niwswQPZGX == niwswQPZGX){ILWtifWUYB = true;}
      while(TapqSzfIVi == TapqSzfIVi){CKYhTLFxqq = true;}
      if(LSwSxZBBcS == true){LSwSxZBBcS = false;}
      if(cMJeyHKtJx == true){cMJeyHKtJx = false;}
      if(wSuTGxiRKH == true){wSuTGxiRKH = false;}
      if(ZWycfVlcwV == true){ZWycfVlcwV = false;}
      if(ONwnuGQLrK == true){ONwnuGQLrK = false;}
      if(olzXmrWcMz == true){olzXmrWcMz = false;}
      if(pjqVrFNgzc == true){pjqVrFNgzc = false;}
      if(VgxmhhWHXz == true){VgxmhhWHXz = false;}
      if(fXYzSnXZom == true){fXYzSnXZom = false;}
      if(zIQwfeWaup == true){zIQwfeWaup = false;}
      if(sjMPoHnMpE == true){sjMPoHnMpE = false;}
      if(VOEMTSKqCE == true){VOEMTSKqCE = false;}
      if(hmqLdMPAHD == true){hmqLdMPAHD = false;}
      if(LuMhGuLJKL == true){LuMhGuLJKL = false;}
      if(zKjkRjDEcM == true){zKjkRjDEcM = false;}
      if(UfYqIFsBkb == true){UfYqIFsBkb = false;}
      if(rnFlsZCWGX == true){rnFlsZCWGX = false;}
      if(HAfqEWDGst == true){HAfqEWDGst = false;}
      if(ILWtifWUYB == true){ILWtifWUYB = false;}
      if(CKYhTLFxqq == true){CKYhTLFxqq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class IPUTRPMFGX
{ 
  void rNoRSEgowB()
  { 
      bool fQeMTuywmg = false;
      bool OzIXXdTGiF = false;
      bool bDbICfDJtp = false;
      bool gXywoJEheI = false;
      bool jYFosGVjuA = false;
      bool DMstEcnVJm = false;
      bool rnCwzpAMiN = false;
      bool ZkaaVDxmOw = false;
      bool KSOtmTeErM = false;
      bool VhYqPwVIoD = false;
      bool FJBgZRlSYA = false;
      bool eBHNcYYIJe = false;
      bool KwTWrqMjKZ = false;
      bool goxKpBRgTU = false;
      bool hMfqgTaLOq = false;
      bool WNzJrJoeoi = false;
      bool FCIzTpDwWa = false;
      bool HrnnxyQboX = false;
      bool QAUSCSonSg = false;
      bool MmqkPfEfXq = false;
      string guVyANCAuK;
      string OPjJMrrhfH;
      string yPLEHxbwBt;
      string senLjzjbjs;
      string bTmRqFIRDw;
      string JcVcaRiQpC;
      string lGzBQiaXYN;
      string IQxlfdctNB;
      string oJrEkQILkS;
      string QSkhHUUkAI;
      string dkspnsejxx;
      string noDoMbNtQz;
      string ZOmGwoePeK;
      string IqsEXEQCQq;
      string lYOndbOwUe;
      string HaUAhOylsy;
      string NOFlnxuyQI;
      string LLcYNAAycU;
      string aiuzDhVrWI;
      string FeISdWTbAf;
      if(guVyANCAuK == dkspnsejxx){fQeMTuywmg = true;}
      else if(dkspnsejxx == guVyANCAuK){FJBgZRlSYA = true;}
      if(OPjJMrrhfH == noDoMbNtQz){OzIXXdTGiF = true;}
      else if(noDoMbNtQz == OPjJMrrhfH){eBHNcYYIJe = true;}
      if(yPLEHxbwBt == ZOmGwoePeK){bDbICfDJtp = true;}
      else if(ZOmGwoePeK == yPLEHxbwBt){KwTWrqMjKZ = true;}
      if(senLjzjbjs == IqsEXEQCQq){gXywoJEheI = true;}
      else if(IqsEXEQCQq == senLjzjbjs){goxKpBRgTU = true;}
      if(bTmRqFIRDw == lYOndbOwUe){jYFosGVjuA = true;}
      else if(lYOndbOwUe == bTmRqFIRDw){hMfqgTaLOq = true;}
      if(JcVcaRiQpC == HaUAhOylsy){DMstEcnVJm = true;}
      else if(HaUAhOylsy == JcVcaRiQpC){WNzJrJoeoi = true;}
      if(lGzBQiaXYN == NOFlnxuyQI){rnCwzpAMiN = true;}
      else if(NOFlnxuyQI == lGzBQiaXYN){FCIzTpDwWa = true;}
      if(IQxlfdctNB == LLcYNAAycU){ZkaaVDxmOw = true;}
      if(oJrEkQILkS == aiuzDhVrWI){KSOtmTeErM = true;}
      if(QSkhHUUkAI == FeISdWTbAf){VhYqPwVIoD = true;}
      while(LLcYNAAycU == IQxlfdctNB){HrnnxyQboX = true;}
      while(aiuzDhVrWI == aiuzDhVrWI){QAUSCSonSg = true;}
      while(FeISdWTbAf == FeISdWTbAf){MmqkPfEfXq = true;}
      if(fQeMTuywmg == true){fQeMTuywmg = false;}
      if(OzIXXdTGiF == true){OzIXXdTGiF = false;}
      if(bDbICfDJtp == true){bDbICfDJtp = false;}
      if(gXywoJEheI == true){gXywoJEheI = false;}
      if(jYFosGVjuA == true){jYFosGVjuA = false;}
      if(DMstEcnVJm == true){DMstEcnVJm = false;}
      if(rnCwzpAMiN == true){rnCwzpAMiN = false;}
      if(ZkaaVDxmOw == true){ZkaaVDxmOw = false;}
      if(KSOtmTeErM == true){KSOtmTeErM = false;}
      if(VhYqPwVIoD == true){VhYqPwVIoD = false;}
      if(FJBgZRlSYA == true){FJBgZRlSYA = false;}
      if(eBHNcYYIJe == true){eBHNcYYIJe = false;}
      if(KwTWrqMjKZ == true){KwTWrqMjKZ = false;}
      if(goxKpBRgTU == true){goxKpBRgTU = false;}
      if(hMfqgTaLOq == true){hMfqgTaLOq = false;}
      if(WNzJrJoeoi == true){WNzJrJoeoi = false;}
      if(FCIzTpDwWa == true){FCIzTpDwWa = false;}
      if(HrnnxyQboX == true){HrnnxyQboX = false;}
      if(QAUSCSonSg == true){QAUSCSonSg = false;}
      if(MmqkPfEfXq == true){MmqkPfEfXq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DZFVSXRZCX
{ 
  void lFJfZttNWl()
  { 
      bool UwPpLieXEE = false;
      bool ewrwFNKAjP = false;
      bool FzmAIBucsq = false;
      bool KSlFUWhkBW = false;
      bool bSfYaCFwkB = false;
      bool leoVXVIPhK = false;
      bool mBeMsMKmbh = false;
      bool eZyNnmTNZF = false;
      bool wzqrFfcPhy = false;
      bool NSDqiamSfN = false;
      bool rSTCVLfbhk = false;
      bool CupbOnJmGN = false;
      bool jtEcUMVHdN = false;
      bool zAhLMdenVs = false;
      bool zAhxwSbact = false;
      bool QzLRdyNqhJ = false;
      bool pYxTkBMdPk = false;
      bool JVRNNiLHgC = false;
      bool juejxznPgO = false;
      bool PnGLDaCOLt = false;
      string ihnSqchYyV;
      string pKVpjlUYdp;
      string dibubrsDRK;
      string EbUDqXyuYU;
      string VpAhrTUirt;
      string EKwmQAGspz;
      string jeHmROoSzL;
      string mYzJTnisCH;
      string WKLWFtehlp;
      string RFEZkqiMQd;
      string eCjfHfKlSc;
      string RMpibZTjgW;
      string iHgIthTYYc;
      string FVcYcUZePZ;
      string irOJYEOVxL;
      string XyestWUblY;
      string LlNoWuiRFH;
      string znTHnprXio;
      string TyaNFGpjdF;
      string zysqOweZlc;
      if(ihnSqchYyV == eCjfHfKlSc){UwPpLieXEE = true;}
      else if(eCjfHfKlSc == ihnSqchYyV){rSTCVLfbhk = true;}
      if(pKVpjlUYdp == RMpibZTjgW){ewrwFNKAjP = true;}
      else if(RMpibZTjgW == pKVpjlUYdp){CupbOnJmGN = true;}
      if(dibubrsDRK == iHgIthTYYc){FzmAIBucsq = true;}
      else if(iHgIthTYYc == dibubrsDRK){jtEcUMVHdN = true;}
      if(EbUDqXyuYU == FVcYcUZePZ){KSlFUWhkBW = true;}
      else if(FVcYcUZePZ == EbUDqXyuYU){zAhLMdenVs = true;}
      if(VpAhrTUirt == irOJYEOVxL){bSfYaCFwkB = true;}
      else if(irOJYEOVxL == VpAhrTUirt){zAhxwSbact = true;}
      if(EKwmQAGspz == XyestWUblY){leoVXVIPhK = true;}
      else if(XyestWUblY == EKwmQAGspz){QzLRdyNqhJ = true;}
      if(jeHmROoSzL == LlNoWuiRFH){mBeMsMKmbh = true;}
      else if(LlNoWuiRFH == jeHmROoSzL){pYxTkBMdPk = true;}
      if(mYzJTnisCH == znTHnprXio){eZyNnmTNZF = true;}
      if(WKLWFtehlp == TyaNFGpjdF){wzqrFfcPhy = true;}
      if(RFEZkqiMQd == zysqOweZlc){NSDqiamSfN = true;}
      while(znTHnprXio == mYzJTnisCH){JVRNNiLHgC = true;}
      while(TyaNFGpjdF == TyaNFGpjdF){juejxznPgO = true;}
      while(zysqOweZlc == zysqOweZlc){PnGLDaCOLt = true;}
      if(UwPpLieXEE == true){UwPpLieXEE = false;}
      if(ewrwFNKAjP == true){ewrwFNKAjP = false;}
      if(FzmAIBucsq == true){FzmAIBucsq = false;}
      if(KSlFUWhkBW == true){KSlFUWhkBW = false;}
      if(bSfYaCFwkB == true){bSfYaCFwkB = false;}
      if(leoVXVIPhK == true){leoVXVIPhK = false;}
      if(mBeMsMKmbh == true){mBeMsMKmbh = false;}
      if(eZyNnmTNZF == true){eZyNnmTNZF = false;}
      if(wzqrFfcPhy == true){wzqrFfcPhy = false;}
      if(NSDqiamSfN == true){NSDqiamSfN = false;}
      if(rSTCVLfbhk == true){rSTCVLfbhk = false;}
      if(CupbOnJmGN == true){CupbOnJmGN = false;}
      if(jtEcUMVHdN == true){jtEcUMVHdN = false;}
      if(zAhLMdenVs == true){zAhLMdenVs = false;}
      if(zAhxwSbact == true){zAhxwSbact = false;}
      if(QzLRdyNqhJ == true){QzLRdyNqhJ = false;}
      if(pYxTkBMdPk == true){pYxTkBMdPk = false;}
      if(JVRNNiLHgC == true){JVRNNiLHgC = false;}
      if(juejxznPgO == true){juejxznPgO = false;}
      if(PnGLDaCOLt == true){PnGLDaCOLt = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CMHVVLYWVW
{ 
  void ulYZwcrNKB()
  { 
      bool ZNMGQLARBC = false;
      bool oGJODZReXr = false;
      bool wlbwSwVioG = false;
      bool MyBesyABba = false;
      bool WaCacuMZnK = false;
      bool CNocMnlQmU = false;
      bool pRbtppEdVE = false;
      bool YQCwUUiATX = false;
      bool jwudEyJYSF = false;
      bool MjNHkFzHkl = false;
      bool phEilkKrHq = false;
      bool EkwPjsWxpq = false;
      bool ffLProDuLP = false;
      bool VaPggCrLrS = false;
      bool nRQbAamxXq = false;
      bool hBRtGKVnqx = false;
      bool hPgrVcVOig = false;
      bool BKFXDNOjOQ = false;
      bool BFWTgTJUaV = false;
      bool fEUujXkYGX = false;
      string NxBzoyGdZp;
      string PFijLSrITs;
      string zNxpOTEhsa;
      string TXcXYnYsxR;
      string drnSWMzxqp;
      string aSEjWzALjV;
      string tYjyVqbLLX;
      string syKBOGZdSC;
      string UGpDABTsBY;
      string mMJbXFIoxf;
      string jnlSwbHCUP;
      string ZWsgBmhgxU;
      string WiJmjgohdJ;
      string yjglSfUXGX;
      string sRXlyQTbaj;
      string WaqbIAdyiP;
      string mYYKQopGOU;
      string SKZZaFLXnT;
      string MNHsmXrEdR;
      string BVnqjTsMVJ;
      if(NxBzoyGdZp == jnlSwbHCUP){ZNMGQLARBC = true;}
      else if(jnlSwbHCUP == NxBzoyGdZp){phEilkKrHq = true;}
      if(PFijLSrITs == ZWsgBmhgxU){oGJODZReXr = true;}
      else if(ZWsgBmhgxU == PFijLSrITs){EkwPjsWxpq = true;}
      if(zNxpOTEhsa == WiJmjgohdJ){wlbwSwVioG = true;}
      else if(WiJmjgohdJ == zNxpOTEhsa){ffLProDuLP = true;}
      if(TXcXYnYsxR == yjglSfUXGX){MyBesyABba = true;}
      else if(yjglSfUXGX == TXcXYnYsxR){VaPggCrLrS = true;}
      if(drnSWMzxqp == sRXlyQTbaj){WaCacuMZnK = true;}
      else if(sRXlyQTbaj == drnSWMzxqp){nRQbAamxXq = true;}
      if(aSEjWzALjV == WaqbIAdyiP){CNocMnlQmU = true;}
      else if(WaqbIAdyiP == aSEjWzALjV){hBRtGKVnqx = true;}
      if(tYjyVqbLLX == mYYKQopGOU){pRbtppEdVE = true;}
      else if(mYYKQopGOU == tYjyVqbLLX){hPgrVcVOig = true;}
      if(syKBOGZdSC == SKZZaFLXnT){YQCwUUiATX = true;}
      if(UGpDABTsBY == MNHsmXrEdR){jwudEyJYSF = true;}
      if(mMJbXFIoxf == BVnqjTsMVJ){MjNHkFzHkl = true;}
      while(SKZZaFLXnT == syKBOGZdSC){BKFXDNOjOQ = true;}
      while(MNHsmXrEdR == MNHsmXrEdR){BFWTgTJUaV = true;}
      while(BVnqjTsMVJ == BVnqjTsMVJ){fEUujXkYGX = true;}
      if(ZNMGQLARBC == true){ZNMGQLARBC = false;}
      if(oGJODZReXr == true){oGJODZReXr = false;}
      if(wlbwSwVioG == true){wlbwSwVioG = false;}
      if(MyBesyABba == true){MyBesyABba = false;}
      if(WaCacuMZnK == true){WaCacuMZnK = false;}
      if(CNocMnlQmU == true){CNocMnlQmU = false;}
      if(pRbtppEdVE == true){pRbtppEdVE = false;}
      if(YQCwUUiATX == true){YQCwUUiATX = false;}
      if(jwudEyJYSF == true){jwudEyJYSF = false;}
      if(MjNHkFzHkl == true){MjNHkFzHkl = false;}
      if(phEilkKrHq == true){phEilkKrHq = false;}
      if(EkwPjsWxpq == true){EkwPjsWxpq = false;}
      if(ffLProDuLP == true){ffLProDuLP = false;}
      if(VaPggCrLrS == true){VaPggCrLrS = false;}
      if(nRQbAamxXq == true){nRQbAamxXq = false;}
      if(hBRtGKVnqx == true){hBRtGKVnqx = false;}
      if(hPgrVcVOig == true){hPgrVcVOig = false;}
      if(BKFXDNOjOQ == true){BKFXDNOjOQ = false;}
      if(BFWTgTJUaV == true){BFWTgTJUaV = false;}
      if(fEUujXkYGX == true){fEUujXkYGX = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KNNGQWAUFJ
{ 
  void ozCKZRnDjh()
  { 
      bool baLZjxySZH = false;
      bool jtYwlgPnVk = false;
      bool auFlOsAnRw = false;
      bool zTuuyVgFYZ = false;
      bool bIpcpxmawa = false;
      bool EPhbbgVYKp = false;
      bool xJBdHPzcyP = false;
      bool zJOwISEbwQ = false;
      bool GZrprjlTSQ = false;
      bool AIuicRHMVZ = false;
      bool yJbfatAkSI = false;
      bool csbsjWRKkW = false;
      bool deKgOqOfTq = false;
      bool LqGWSxxLOy = false;
      bool jABKnHfBzF = false;
      bool zwbywSzuRj = false;
      bool eURaxklFal = false;
      bool YGummcdArG = false;
      bool KNfNqPPIZz = false;
      bool xCsdSOiZMD = false;
      string EzjCHTxpog;
      string gJFDdYKRIY;
      string XKCjuZTujE;
      string dbDtWFFZTd;
      string qRmYVjuILS;
      string cGbTOsxAGh;
      string zCGkCFFLgX;
      string TjydgGWcNL;
      string eKiQaWzjfc;
      string IESkeoKCVQ;
      string dibmyrIxlH;
      string WTmtDoSoJe;
      string FVfEPORMzr;
      string wTHwhuiuRi;
      string OBxXFnRXhp;
      string VVspLHCWXi;
      string auysLCLpMH;
      string FXWZQDXHya;
      string xseJWZaQkZ;
      string rLWhITDEMD;
      if(EzjCHTxpog == dibmyrIxlH){baLZjxySZH = true;}
      else if(dibmyrIxlH == EzjCHTxpog){yJbfatAkSI = true;}
      if(gJFDdYKRIY == WTmtDoSoJe){jtYwlgPnVk = true;}
      else if(WTmtDoSoJe == gJFDdYKRIY){csbsjWRKkW = true;}
      if(XKCjuZTujE == FVfEPORMzr){auFlOsAnRw = true;}
      else if(FVfEPORMzr == XKCjuZTujE){deKgOqOfTq = true;}
      if(dbDtWFFZTd == wTHwhuiuRi){zTuuyVgFYZ = true;}
      else if(wTHwhuiuRi == dbDtWFFZTd){LqGWSxxLOy = true;}
      if(qRmYVjuILS == OBxXFnRXhp){bIpcpxmawa = true;}
      else if(OBxXFnRXhp == qRmYVjuILS){jABKnHfBzF = true;}
      if(cGbTOsxAGh == VVspLHCWXi){EPhbbgVYKp = true;}
      else if(VVspLHCWXi == cGbTOsxAGh){zwbywSzuRj = true;}
      if(zCGkCFFLgX == auysLCLpMH){xJBdHPzcyP = true;}
      else if(auysLCLpMH == zCGkCFFLgX){eURaxklFal = true;}
      if(TjydgGWcNL == FXWZQDXHya){zJOwISEbwQ = true;}
      if(eKiQaWzjfc == xseJWZaQkZ){GZrprjlTSQ = true;}
      if(IESkeoKCVQ == rLWhITDEMD){AIuicRHMVZ = true;}
      while(FXWZQDXHya == TjydgGWcNL){YGummcdArG = true;}
      while(xseJWZaQkZ == xseJWZaQkZ){KNfNqPPIZz = true;}
      while(rLWhITDEMD == rLWhITDEMD){xCsdSOiZMD = true;}
      if(baLZjxySZH == true){baLZjxySZH = false;}
      if(jtYwlgPnVk == true){jtYwlgPnVk = false;}
      if(auFlOsAnRw == true){auFlOsAnRw = false;}
      if(zTuuyVgFYZ == true){zTuuyVgFYZ = false;}
      if(bIpcpxmawa == true){bIpcpxmawa = false;}
      if(EPhbbgVYKp == true){EPhbbgVYKp = false;}
      if(xJBdHPzcyP == true){xJBdHPzcyP = false;}
      if(zJOwISEbwQ == true){zJOwISEbwQ = false;}
      if(GZrprjlTSQ == true){GZrprjlTSQ = false;}
      if(AIuicRHMVZ == true){AIuicRHMVZ = false;}
      if(yJbfatAkSI == true){yJbfatAkSI = false;}
      if(csbsjWRKkW == true){csbsjWRKkW = false;}
      if(deKgOqOfTq == true){deKgOqOfTq = false;}
      if(LqGWSxxLOy == true){LqGWSxxLOy = false;}
      if(jABKnHfBzF == true){jABKnHfBzF = false;}
      if(zwbywSzuRj == true){zwbywSzuRj = false;}
      if(eURaxklFal == true){eURaxklFal = false;}
      if(YGummcdArG == true){YGummcdArG = false;}
      if(KNfNqPPIZz == true){KNfNqPPIZz = false;}
      if(xCsdSOiZMD == true){xCsdSOiZMD = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DZAVIUJMLH
{ 
  void ZUZEGthYxr()
  { 
      bool IlTXDVsDBH = false;
      bool DExQTGHkDe = false;
      bool SSXFAuLdTH = false;
      bool AZqaeFqqBa = false;
      bool PeXhLJkNwh = false;
      bool OGQVQpONXI = false;
      bool JOjsGcNWlN = false;
      bool OPjbPmtosp = false;
      bool JyhlssIUJn = false;
      bool qdTrRAueah = false;
      bool wObhSyWnFc = false;
      bool uYUgyLfyzN = false;
      bool UrMxWkwqKO = false;
      bool qPNKtuVASt = false;
      bool WUOfsRJmsF = false;
      bool UxeNNDBdKh = false;
      bool CecqleKJlD = false;
      bool oeeFqzcfEF = false;
      bool bIxEstkAwp = false;
      bool srpkqqqXtN = false;
      string iHDoubDWpK;
      string WjRPmQwFEf;
      string KWDhibQDYH;
      string RhmLqBWbkN;
      string kGFuQJjZcu;
      string kwTGmLEUGT;
      string LzyhrowFYk;
      string sUUpuDQHRY;
      string qqacVUNgXe;
      string xCVQCkIWCb;
      string MeArCQbdLi;
      string twDgbTYUMX;
      string bPgsnqzEmJ;
      string usEoMMphaF;
      string siVojAznnh;
      string MmKeAxxGDK;
      string RWpBnBrNUP;
      string dptHdrtrJm;
      string qKPFxIiYoO;
      string OidPnVrUBi;
      if(iHDoubDWpK == MeArCQbdLi){IlTXDVsDBH = true;}
      else if(MeArCQbdLi == iHDoubDWpK){wObhSyWnFc = true;}
      if(WjRPmQwFEf == twDgbTYUMX){DExQTGHkDe = true;}
      else if(twDgbTYUMX == WjRPmQwFEf){uYUgyLfyzN = true;}
      if(KWDhibQDYH == bPgsnqzEmJ){SSXFAuLdTH = true;}
      else if(bPgsnqzEmJ == KWDhibQDYH){UrMxWkwqKO = true;}
      if(RhmLqBWbkN == usEoMMphaF){AZqaeFqqBa = true;}
      else if(usEoMMphaF == RhmLqBWbkN){qPNKtuVASt = true;}
      if(kGFuQJjZcu == siVojAznnh){PeXhLJkNwh = true;}
      else if(siVojAznnh == kGFuQJjZcu){WUOfsRJmsF = true;}
      if(kwTGmLEUGT == MmKeAxxGDK){OGQVQpONXI = true;}
      else if(MmKeAxxGDK == kwTGmLEUGT){UxeNNDBdKh = true;}
      if(LzyhrowFYk == RWpBnBrNUP){JOjsGcNWlN = true;}
      else if(RWpBnBrNUP == LzyhrowFYk){CecqleKJlD = true;}
      if(sUUpuDQHRY == dptHdrtrJm){OPjbPmtosp = true;}
      if(qqacVUNgXe == qKPFxIiYoO){JyhlssIUJn = true;}
      if(xCVQCkIWCb == OidPnVrUBi){qdTrRAueah = true;}
      while(dptHdrtrJm == sUUpuDQHRY){oeeFqzcfEF = true;}
      while(qKPFxIiYoO == qKPFxIiYoO){bIxEstkAwp = true;}
      while(OidPnVrUBi == OidPnVrUBi){srpkqqqXtN = true;}
      if(IlTXDVsDBH == true){IlTXDVsDBH = false;}
      if(DExQTGHkDe == true){DExQTGHkDe = false;}
      if(SSXFAuLdTH == true){SSXFAuLdTH = false;}
      if(AZqaeFqqBa == true){AZqaeFqqBa = false;}
      if(PeXhLJkNwh == true){PeXhLJkNwh = false;}
      if(OGQVQpONXI == true){OGQVQpONXI = false;}
      if(JOjsGcNWlN == true){JOjsGcNWlN = false;}
      if(OPjbPmtosp == true){OPjbPmtosp = false;}
      if(JyhlssIUJn == true){JyhlssIUJn = false;}
      if(qdTrRAueah == true){qdTrRAueah = false;}
      if(wObhSyWnFc == true){wObhSyWnFc = false;}
      if(uYUgyLfyzN == true){uYUgyLfyzN = false;}
      if(UrMxWkwqKO == true){UrMxWkwqKO = false;}
      if(qPNKtuVASt == true){qPNKtuVASt = false;}
      if(WUOfsRJmsF == true){WUOfsRJmsF = false;}
      if(UxeNNDBdKh == true){UxeNNDBdKh = false;}
      if(CecqleKJlD == true){CecqleKJlD = false;}
      if(oeeFqzcfEF == true){oeeFqzcfEF = false;}
      if(bIxEstkAwp == true){bIxEstkAwp = false;}
      if(srpkqqqXtN == true){srpkqqqXtN = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TTFFYGPDDA
{ 
  void ddNfbdOiTo()
  { 
      bool XfmPsDlymd = false;
      bool gPZDTgQyzY = false;
      bool gYVNQXYuAE = false;
      bool IqrBXEAYqK = false;
      bool mkDqejCGZk = false;
      bool BbSNAyKYzV = false;
      bool eqyhUoWYeP = false;
      bool grHnlwUdhg = false;
      bool URUODgYeAZ = false;
      bool taZUfulVWo = false;
      bool MdbKEITebE = false;
      bool cfpRAMWidn = false;
      bool QrXFVGSmHp = false;
      bool JzGsUqpKKu = false;
      bool MsCwwNjHTT = false;
      bool YDqiXTFVny = false;
      bool ogWkucZxxJ = false;
      bool VMYibXpbII = false;
      bool VLSnRFIXUA = false;
      bool lUVMFeVQBT = false;
      string NlqPoYFfmj;
      string PhEKbZubAz;
      string jSJWCUNTeV;
      string owuDMjTENp;
      string QRqAcSNmaT;
      string MpXyRubUNT;
      string LkAQtTlByr;
      string GqOJhSbIsi;
      string HZrLJzYwkR;
      string HyqsefQZGJ;
      string fLoiMUaNQz;
      string STadMkrnHu;
      string uoQgjHAVdC;
      string BCCjDukJzn;
      string XKfkynWNgt;
      string mbRKOpuIzE;
      string UFiNyQQqZQ;
      string AioTWjJehu;
      string jmnjOYjqVM;
      string zcJtSRaRmg;
      if(NlqPoYFfmj == fLoiMUaNQz){XfmPsDlymd = true;}
      else if(fLoiMUaNQz == NlqPoYFfmj){MdbKEITebE = true;}
      if(PhEKbZubAz == STadMkrnHu){gPZDTgQyzY = true;}
      else if(STadMkrnHu == PhEKbZubAz){cfpRAMWidn = true;}
      if(jSJWCUNTeV == uoQgjHAVdC){gYVNQXYuAE = true;}
      else if(uoQgjHAVdC == jSJWCUNTeV){QrXFVGSmHp = true;}
      if(owuDMjTENp == BCCjDukJzn){IqrBXEAYqK = true;}
      else if(BCCjDukJzn == owuDMjTENp){JzGsUqpKKu = true;}
      if(QRqAcSNmaT == XKfkynWNgt){mkDqejCGZk = true;}
      else if(XKfkynWNgt == QRqAcSNmaT){MsCwwNjHTT = true;}
      if(MpXyRubUNT == mbRKOpuIzE){BbSNAyKYzV = true;}
      else if(mbRKOpuIzE == MpXyRubUNT){YDqiXTFVny = true;}
      if(LkAQtTlByr == UFiNyQQqZQ){eqyhUoWYeP = true;}
      else if(UFiNyQQqZQ == LkAQtTlByr){ogWkucZxxJ = true;}
      if(GqOJhSbIsi == AioTWjJehu){grHnlwUdhg = true;}
      if(HZrLJzYwkR == jmnjOYjqVM){URUODgYeAZ = true;}
      if(HyqsefQZGJ == zcJtSRaRmg){taZUfulVWo = true;}
      while(AioTWjJehu == GqOJhSbIsi){VMYibXpbII = true;}
      while(jmnjOYjqVM == jmnjOYjqVM){VLSnRFIXUA = true;}
      while(zcJtSRaRmg == zcJtSRaRmg){lUVMFeVQBT = true;}
      if(XfmPsDlymd == true){XfmPsDlymd = false;}
      if(gPZDTgQyzY == true){gPZDTgQyzY = false;}
      if(gYVNQXYuAE == true){gYVNQXYuAE = false;}
      if(IqrBXEAYqK == true){IqrBXEAYqK = false;}
      if(mkDqejCGZk == true){mkDqejCGZk = false;}
      if(BbSNAyKYzV == true){BbSNAyKYzV = false;}
      if(eqyhUoWYeP == true){eqyhUoWYeP = false;}
      if(grHnlwUdhg == true){grHnlwUdhg = false;}
      if(URUODgYeAZ == true){URUODgYeAZ = false;}
      if(taZUfulVWo == true){taZUfulVWo = false;}
      if(MdbKEITebE == true){MdbKEITebE = false;}
      if(cfpRAMWidn == true){cfpRAMWidn = false;}
      if(QrXFVGSmHp == true){QrXFVGSmHp = false;}
      if(JzGsUqpKKu == true){JzGsUqpKKu = false;}
      if(MsCwwNjHTT == true){MsCwwNjHTT = false;}
      if(YDqiXTFVny == true){YDqiXTFVny = false;}
      if(ogWkucZxxJ == true){ogWkucZxxJ = false;}
      if(VMYibXpbII == true){VMYibXpbII = false;}
      if(VLSnRFIXUA == true){VLSnRFIXUA = false;}
      if(lUVMFeVQBT == true){lUVMFeVQBT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KIAUZYDKNP
{ 
  void HHftkLHxhY()
  { 
      bool iwellGsjul = false;
      bool taTLaYYGaD = false;
      bool BdsTqTYOLI = false;
      bool oPUSTexnGw = false;
      bool SnJUJHeDeF = false;
      bool xSDUjECKuS = false;
      bool gdYlweFArw = false;
      bool bjzdkMUYJi = false;
      bool nUiGLWMAiO = false;
      bool blsnOnPWXd = false;
      bool CeKicMSYiW = false;
      bool jdAMdMuSaZ = false;
      bool SFRdToqKOx = false;
      bool xxAefJJzcy = false;
      bool DmQMdylfJC = false;
      bool rxwyLwMgPn = false;
      bool VIKlYmcUfU = false;
      bool CuobWfoFAU = false;
      bool CKAMEAACTj = false;
      bool wcjncucdmp = false;
      string rEmAAzqwae;
      string UHtHFdQAmO;
      string VYkmDRAGjr;
      string WMYiFBDQXt;
      string AEdgtWMooA;
      string fPpoFTSzxh;
      string zkMhHIiKBE;
      string azcsNQtEsf;
      string FKQiMHVick;
      string hDTotPquYl;
      string nCnkZgmxbo;
      string YOGQcoTdVz;
      string lAhPVXemyy;
      string YyeGrDWeIf;
      string UXHnLSwDMF;
      string uNZZzmTXOC;
      string BzzcPajAYZ;
      string SoUpdNssEC;
      string abasCLYFGE;
      string iyPGdVYdYk;
      if(rEmAAzqwae == nCnkZgmxbo){iwellGsjul = true;}
      else if(nCnkZgmxbo == rEmAAzqwae){CeKicMSYiW = true;}
      if(UHtHFdQAmO == YOGQcoTdVz){taTLaYYGaD = true;}
      else if(YOGQcoTdVz == UHtHFdQAmO){jdAMdMuSaZ = true;}
      if(VYkmDRAGjr == lAhPVXemyy){BdsTqTYOLI = true;}
      else if(lAhPVXemyy == VYkmDRAGjr){SFRdToqKOx = true;}
      if(WMYiFBDQXt == YyeGrDWeIf){oPUSTexnGw = true;}
      else if(YyeGrDWeIf == WMYiFBDQXt){xxAefJJzcy = true;}
      if(AEdgtWMooA == UXHnLSwDMF){SnJUJHeDeF = true;}
      else if(UXHnLSwDMF == AEdgtWMooA){DmQMdylfJC = true;}
      if(fPpoFTSzxh == uNZZzmTXOC){xSDUjECKuS = true;}
      else if(uNZZzmTXOC == fPpoFTSzxh){rxwyLwMgPn = true;}
      if(zkMhHIiKBE == BzzcPajAYZ){gdYlweFArw = true;}
      else if(BzzcPajAYZ == zkMhHIiKBE){VIKlYmcUfU = true;}
      if(azcsNQtEsf == SoUpdNssEC){bjzdkMUYJi = true;}
      if(FKQiMHVick == abasCLYFGE){nUiGLWMAiO = true;}
      if(hDTotPquYl == iyPGdVYdYk){blsnOnPWXd = true;}
      while(SoUpdNssEC == azcsNQtEsf){CuobWfoFAU = true;}
      while(abasCLYFGE == abasCLYFGE){CKAMEAACTj = true;}
      while(iyPGdVYdYk == iyPGdVYdYk){wcjncucdmp = true;}
      if(iwellGsjul == true){iwellGsjul = false;}
      if(taTLaYYGaD == true){taTLaYYGaD = false;}
      if(BdsTqTYOLI == true){BdsTqTYOLI = false;}
      if(oPUSTexnGw == true){oPUSTexnGw = false;}
      if(SnJUJHeDeF == true){SnJUJHeDeF = false;}
      if(xSDUjECKuS == true){xSDUjECKuS = false;}
      if(gdYlweFArw == true){gdYlweFArw = false;}
      if(bjzdkMUYJi == true){bjzdkMUYJi = false;}
      if(nUiGLWMAiO == true){nUiGLWMAiO = false;}
      if(blsnOnPWXd == true){blsnOnPWXd = false;}
      if(CeKicMSYiW == true){CeKicMSYiW = false;}
      if(jdAMdMuSaZ == true){jdAMdMuSaZ = false;}
      if(SFRdToqKOx == true){SFRdToqKOx = false;}
      if(xxAefJJzcy == true){xxAefJJzcy = false;}
      if(DmQMdylfJC == true){DmQMdylfJC = false;}
      if(rxwyLwMgPn == true){rxwyLwMgPn = false;}
      if(VIKlYmcUfU == true){VIKlYmcUfU = false;}
      if(CuobWfoFAU == true){CuobWfoFAU = false;}
      if(CKAMEAACTj == true){CKAMEAACTj = false;}
      if(wcjncucdmp == true){wcjncucdmp = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PPHAGEAAMP
{ 
  void FMzpewMySA()
  { 
      bool xSpIuASnsL = false;
      bool lTWSSugfey = false;
      bool hcPqacdwTs = false;
      bool ZETlaVLtHa = false;
      bool KfzqxpFsGD = false;
      bool QOkuZTDzgS = false;
      bool VWUAlgAkqw = false;
      bool jlYEmBdYsk = false;
      bool XjpQbKTYMH = false;
      bool SMNnEnLyFQ = false;
      bool pUCJSdogHX = false;
      bool pdFwulfDXa = false;
      bool MaXjdXYkzy = false;
      bool oeTctCgfxy = false;
      bool VmQnQnGQsR = false;
      bool HHadDOwSfu = false;
      bool EEgJhKJbBr = false;
      bool EzGZFYJiSo = false;
      bool QHqhphEoYe = false;
      bool GOABsQGHGF = false;
      string JZZTPwSgDF;
      string bwRETRnhVR;
      string QllKQagkOd;
      string QKOSLHClMQ;
      string RIrywdQPwy;
      string SBrWCWmPDn;
      string PhSYnQhzsM;
      string OBUEAIsmNl;
      string KQnPLPRqPn;
      string TmhXAjpSJe;
      string wRWjiQgmVt;
      string czBQdzsCNY;
      string zpiFraTwEQ;
      string RadhprtDgu;
      string hdUVKTlRsB;
      string pnFjfGhRqH;
      string xjiFbCVSak;
      string FpURqUUNtY;
      string oBCCadUsCi;
      string KbMYyyuPsH;
      if(JZZTPwSgDF == wRWjiQgmVt){xSpIuASnsL = true;}
      else if(wRWjiQgmVt == JZZTPwSgDF){pUCJSdogHX = true;}
      if(bwRETRnhVR == czBQdzsCNY){lTWSSugfey = true;}
      else if(czBQdzsCNY == bwRETRnhVR){pdFwulfDXa = true;}
      if(QllKQagkOd == zpiFraTwEQ){hcPqacdwTs = true;}
      else if(zpiFraTwEQ == QllKQagkOd){MaXjdXYkzy = true;}
      if(QKOSLHClMQ == RadhprtDgu){ZETlaVLtHa = true;}
      else if(RadhprtDgu == QKOSLHClMQ){oeTctCgfxy = true;}
      if(RIrywdQPwy == hdUVKTlRsB){KfzqxpFsGD = true;}
      else if(hdUVKTlRsB == RIrywdQPwy){VmQnQnGQsR = true;}
      if(SBrWCWmPDn == pnFjfGhRqH){QOkuZTDzgS = true;}
      else if(pnFjfGhRqH == SBrWCWmPDn){HHadDOwSfu = true;}
      if(PhSYnQhzsM == xjiFbCVSak){VWUAlgAkqw = true;}
      else if(xjiFbCVSak == PhSYnQhzsM){EEgJhKJbBr = true;}
      if(OBUEAIsmNl == FpURqUUNtY){jlYEmBdYsk = true;}
      if(KQnPLPRqPn == oBCCadUsCi){XjpQbKTYMH = true;}
      if(TmhXAjpSJe == KbMYyyuPsH){SMNnEnLyFQ = true;}
      while(FpURqUUNtY == OBUEAIsmNl){EzGZFYJiSo = true;}
      while(oBCCadUsCi == oBCCadUsCi){QHqhphEoYe = true;}
      while(KbMYyyuPsH == KbMYyyuPsH){GOABsQGHGF = true;}
      if(xSpIuASnsL == true){xSpIuASnsL = false;}
      if(lTWSSugfey == true){lTWSSugfey = false;}
      if(hcPqacdwTs == true){hcPqacdwTs = false;}
      if(ZETlaVLtHa == true){ZETlaVLtHa = false;}
      if(KfzqxpFsGD == true){KfzqxpFsGD = false;}
      if(QOkuZTDzgS == true){QOkuZTDzgS = false;}
      if(VWUAlgAkqw == true){VWUAlgAkqw = false;}
      if(jlYEmBdYsk == true){jlYEmBdYsk = false;}
      if(XjpQbKTYMH == true){XjpQbKTYMH = false;}
      if(SMNnEnLyFQ == true){SMNnEnLyFQ = false;}
      if(pUCJSdogHX == true){pUCJSdogHX = false;}
      if(pdFwulfDXa == true){pdFwulfDXa = false;}
      if(MaXjdXYkzy == true){MaXjdXYkzy = false;}
      if(oeTctCgfxy == true){oeTctCgfxy = false;}
      if(VmQnQnGQsR == true){VmQnQnGQsR = false;}
      if(HHadDOwSfu == true){HHadDOwSfu = false;}
      if(EEgJhKJbBr == true){EEgJhKJbBr = false;}
      if(EzGZFYJiSo == true){EzGZFYJiSo = false;}
      if(QHqhphEoYe == true){QHqhphEoYe = false;}
      if(GOABsQGHGF == true){GOABsQGHGF = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YXVKPKEMGY
{ 
  void VXsqSzIdmM()
  { 
      bool oVQKOGmPcM = false;
      bool qrENQhRRVl = false;
      bool axuRwmtNGy = false;
      bool VMQbwREFkt = false;
      bool rBAeKwTUWc = false;
      bool YjwxrqZzOb = false;
      bool LYJeqeabQy = false;
      bool cLgbtZxPGz = false;
      bool SZqdZlPSjF = false;
      bool qFqJhSqVnf = false;
      bool VqEXzMDUDS = false;
      bool tKrOIlhfEE = false;
      bool OHrKGWUXsa = false;
      bool haaZdbUSdM = false;
      bool iuORKgNbAw = false;
      bool kLQiQNysQc = false;
      bool qeacjSiRkf = false;
      bool QPCqVTraAZ = false;
      bool JrWxpWKqIP = false;
      bool caBAVDTlgr = false;
      string XhJrMLQBUV;
      string nBNJUUckHS;
      string RXlleQwWQR;
      string nkiXAgxrKY;
      string nROLfLlPnX;
      string tUZlkzwWNw;
      string PskCgCSpIm;
      string WkZieAJZGS;
      string dfirXTKxmB;
      string kaeNJtgLec;
      string swHBAJtqHk;
      string pVQtSiKreN;
      string rVDHBwFMZj;
      string TxucZgYhqN;
      string YTpBLPJjuZ;
      string ingRkXsAzE;
      string DhRWRramTk;
      string KcBQnXfBul;
      string mVQzAeQcrC;
      string nXhmKpCSjT;
      if(XhJrMLQBUV == swHBAJtqHk){oVQKOGmPcM = true;}
      else if(swHBAJtqHk == XhJrMLQBUV){VqEXzMDUDS = true;}
      if(nBNJUUckHS == pVQtSiKreN){qrENQhRRVl = true;}
      else if(pVQtSiKreN == nBNJUUckHS){tKrOIlhfEE = true;}
      if(RXlleQwWQR == rVDHBwFMZj){axuRwmtNGy = true;}
      else if(rVDHBwFMZj == RXlleQwWQR){OHrKGWUXsa = true;}
      if(nkiXAgxrKY == TxucZgYhqN){VMQbwREFkt = true;}
      else if(TxucZgYhqN == nkiXAgxrKY){haaZdbUSdM = true;}
      if(nROLfLlPnX == YTpBLPJjuZ){rBAeKwTUWc = true;}
      else if(YTpBLPJjuZ == nROLfLlPnX){iuORKgNbAw = true;}
      if(tUZlkzwWNw == ingRkXsAzE){YjwxrqZzOb = true;}
      else if(ingRkXsAzE == tUZlkzwWNw){kLQiQNysQc = true;}
      if(PskCgCSpIm == DhRWRramTk){LYJeqeabQy = true;}
      else if(DhRWRramTk == PskCgCSpIm){qeacjSiRkf = true;}
      if(WkZieAJZGS == KcBQnXfBul){cLgbtZxPGz = true;}
      if(dfirXTKxmB == mVQzAeQcrC){SZqdZlPSjF = true;}
      if(kaeNJtgLec == nXhmKpCSjT){qFqJhSqVnf = true;}
      while(KcBQnXfBul == WkZieAJZGS){QPCqVTraAZ = true;}
      while(mVQzAeQcrC == mVQzAeQcrC){JrWxpWKqIP = true;}
      while(nXhmKpCSjT == nXhmKpCSjT){caBAVDTlgr = true;}
      if(oVQKOGmPcM == true){oVQKOGmPcM = false;}
      if(qrENQhRRVl == true){qrENQhRRVl = false;}
      if(axuRwmtNGy == true){axuRwmtNGy = false;}
      if(VMQbwREFkt == true){VMQbwREFkt = false;}
      if(rBAeKwTUWc == true){rBAeKwTUWc = false;}
      if(YjwxrqZzOb == true){YjwxrqZzOb = false;}
      if(LYJeqeabQy == true){LYJeqeabQy = false;}
      if(cLgbtZxPGz == true){cLgbtZxPGz = false;}
      if(SZqdZlPSjF == true){SZqdZlPSjF = false;}
      if(qFqJhSqVnf == true){qFqJhSqVnf = false;}
      if(VqEXzMDUDS == true){VqEXzMDUDS = false;}
      if(tKrOIlhfEE == true){tKrOIlhfEE = false;}
      if(OHrKGWUXsa == true){OHrKGWUXsa = false;}
      if(haaZdbUSdM == true){haaZdbUSdM = false;}
      if(iuORKgNbAw == true){iuORKgNbAw = false;}
      if(kLQiQNysQc == true){kLQiQNysQc = false;}
      if(qeacjSiRkf == true){qeacjSiRkf = false;}
      if(QPCqVTraAZ == true){QPCqVTraAZ = false;}
      if(JrWxpWKqIP == true){JrWxpWKqIP = false;}
      if(caBAVDTlgr == true){caBAVDTlgr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YOYJFDPPNE
{ 
  void RcmPHVoLLN()
  { 
      bool iPFbbRPLXw = false;
      bool LWUQlbIyIJ = false;
      bool uBdznsuDVH = false;
      bool CLihbYDxtH = false;
      bool QnllNjknkF = false;
      bool LxXZNglGni = false;
      bool WMpPtzmloM = false;
      bool XKEAsGPqkE = false;
      bool HGRPwFnJUt = false;
      bool mwIKkDyVrN = false;
      bool wyzXhArWgG = false;
      bool RtVjaQuZTo = false;
      bool LcRtZaBsPo = false;
      bool IszWHJGshu = false;
      bool MQbNbraYjU = false;
      bool IXCCCTgFyH = false;
      bool yyGryYuycR = false;
      bool JBrXbtBDRh = false;
      bool zcJufjpwUn = false;
      bool bBuawOJjPD = false;
      string WbgtNUOxAa;
      string pSRooqmtQs;
      string ZGVzDqiQck;
      string SqQmfUCUjb;
      string ykDjKOMOBi;
      string rnHXBPFgpV;
      string kxUPHzBdJT;
      string nSKntnDkoo;
      string eUOtrejslP;
      string OVeNqBQmbK;
      string MhEgtYdScM;
      string LTrAqPhZWc;
      string PIrtFSnwxs;
      string WQIecwQgzS;
      string BQPYDtcdWa;
      string SqPzuDpKbb;
      string kgUiouLNXt;
      string zAaDbyrSpK;
      string oJYVZfAJiY;
      string GFFXhTwgiI;
      if(WbgtNUOxAa == MhEgtYdScM){iPFbbRPLXw = true;}
      else if(MhEgtYdScM == WbgtNUOxAa){wyzXhArWgG = true;}
      if(pSRooqmtQs == LTrAqPhZWc){LWUQlbIyIJ = true;}
      else if(LTrAqPhZWc == pSRooqmtQs){RtVjaQuZTo = true;}
      if(ZGVzDqiQck == PIrtFSnwxs){uBdznsuDVH = true;}
      else if(PIrtFSnwxs == ZGVzDqiQck){LcRtZaBsPo = true;}
      if(SqQmfUCUjb == WQIecwQgzS){CLihbYDxtH = true;}
      else if(WQIecwQgzS == SqQmfUCUjb){IszWHJGshu = true;}
      if(ykDjKOMOBi == BQPYDtcdWa){QnllNjknkF = true;}
      else if(BQPYDtcdWa == ykDjKOMOBi){MQbNbraYjU = true;}
      if(rnHXBPFgpV == SqPzuDpKbb){LxXZNglGni = true;}
      else if(SqPzuDpKbb == rnHXBPFgpV){IXCCCTgFyH = true;}
      if(kxUPHzBdJT == kgUiouLNXt){WMpPtzmloM = true;}
      else if(kgUiouLNXt == kxUPHzBdJT){yyGryYuycR = true;}
      if(nSKntnDkoo == zAaDbyrSpK){XKEAsGPqkE = true;}
      if(eUOtrejslP == oJYVZfAJiY){HGRPwFnJUt = true;}
      if(OVeNqBQmbK == GFFXhTwgiI){mwIKkDyVrN = true;}
      while(zAaDbyrSpK == nSKntnDkoo){JBrXbtBDRh = true;}
      while(oJYVZfAJiY == oJYVZfAJiY){zcJufjpwUn = true;}
      while(GFFXhTwgiI == GFFXhTwgiI){bBuawOJjPD = true;}
      if(iPFbbRPLXw == true){iPFbbRPLXw = false;}
      if(LWUQlbIyIJ == true){LWUQlbIyIJ = false;}
      if(uBdznsuDVH == true){uBdznsuDVH = false;}
      if(CLihbYDxtH == true){CLihbYDxtH = false;}
      if(QnllNjknkF == true){QnllNjknkF = false;}
      if(LxXZNglGni == true){LxXZNglGni = false;}
      if(WMpPtzmloM == true){WMpPtzmloM = false;}
      if(XKEAsGPqkE == true){XKEAsGPqkE = false;}
      if(HGRPwFnJUt == true){HGRPwFnJUt = false;}
      if(mwIKkDyVrN == true){mwIKkDyVrN = false;}
      if(wyzXhArWgG == true){wyzXhArWgG = false;}
      if(RtVjaQuZTo == true){RtVjaQuZTo = false;}
      if(LcRtZaBsPo == true){LcRtZaBsPo = false;}
      if(IszWHJGshu == true){IszWHJGshu = false;}
      if(MQbNbraYjU == true){MQbNbraYjU = false;}
      if(IXCCCTgFyH == true){IXCCCTgFyH = false;}
      if(yyGryYuycR == true){yyGryYuycR = false;}
      if(JBrXbtBDRh == true){JBrXbtBDRh = false;}
      if(zcJufjpwUn == true){zcJufjpwUn = false;}
      if(bBuawOJjPD == true){bBuawOJjPD = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XDIOSZJWXS
{ 
  void iDjwFYmXjp()
  { 
      bool nKNpHwpUKP = false;
      bool adjByutfIQ = false;
      bool TYcJkqqyIE = false;
      bool KRQFciqYmZ = false;
      bool LQktCQFslx = false;
      bool OcTYhOhDFP = false;
      bool EqLGEINeYO = false;
      bool DVshFCnhGU = false;
      bool lyMKohyqXN = false;
      bool BnTXUTFeos = false;
      bool CaTWtsikTf = false;
      bool azxjcGeBQW = false;
      bool KVHLNPBUNl = false;
      bool wIsNRLdoPw = false;
      bool yMXqlBpitN = false;
      bool nDOShgSIIe = false;
      bool qNXgiRGjSR = false;
      bool cpkUXyxPYM = false;
      bool YSazxrtaek = false;
      bool rUfkEkopHU = false;
      string VOofAjkhkl;
      string sEknBRZxzc;
      string TiqXdGddNr;
      string jtMOFYwkcK;
      string oGKLnLYoWD;
      string sSzWJlXQBt;
      string FHUsBzqWhc;
      string VnUomNYfiR;
      string hoHJyRtpFT;
      string BeihFhkQJo;
      string UCGshFxKiu;
      string MqhjLQzLui;
      string EAQOapEliE;
      string bHVtxfhNbs;
      string UjGJGJxzWo;
      string hZVZjTkrSq;
      string AKaeTFVmlf;
      string hliBOTsGHQ;
      string sFPtVClwKc;
      string tptjsJJbpB;
      if(VOofAjkhkl == UCGshFxKiu){nKNpHwpUKP = true;}
      else if(UCGshFxKiu == VOofAjkhkl){CaTWtsikTf = true;}
      if(sEknBRZxzc == MqhjLQzLui){adjByutfIQ = true;}
      else if(MqhjLQzLui == sEknBRZxzc){azxjcGeBQW = true;}
      if(TiqXdGddNr == EAQOapEliE){TYcJkqqyIE = true;}
      else if(EAQOapEliE == TiqXdGddNr){KVHLNPBUNl = true;}
      if(jtMOFYwkcK == bHVtxfhNbs){KRQFciqYmZ = true;}
      else if(bHVtxfhNbs == jtMOFYwkcK){wIsNRLdoPw = true;}
      if(oGKLnLYoWD == UjGJGJxzWo){LQktCQFslx = true;}
      else if(UjGJGJxzWo == oGKLnLYoWD){yMXqlBpitN = true;}
      if(sSzWJlXQBt == hZVZjTkrSq){OcTYhOhDFP = true;}
      else if(hZVZjTkrSq == sSzWJlXQBt){nDOShgSIIe = true;}
      if(FHUsBzqWhc == AKaeTFVmlf){EqLGEINeYO = true;}
      else if(AKaeTFVmlf == FHUsBzqWhc){qNXgiRGjSR = true;}
      if(VnUomNYfiR == hliBOTsGHQ){DVshFCnhGU = true;}
      if(hoHJyRtpFT == sFPtVClwKc){lyMKohyqXN = true;}
      if(BeihFhkQJo == tptjsJJbpB){BnTXUTFeos = true;}
      while(hliBOTsGHQ == VnUomNYfiR){cpkUXyxPYM = true;}
      while(sFPtVClwKc == sFPtVClwKc){YSazxrtaek = true;}
      while(tptjsJJbpB == tptjsJJbpB){rUfkEkopHU = true;}
      if(nKNpHwpUKP == true){nKNpHwpUKP = false;}
      if(adjByutfIQ == true){adjByutfIQ = false;}
      if(TYcJkqqyIE == true){TYcJkqqyIE = false;}
      if(KRQFciqYmZ == true){KRQFciqYmZ = false;}
      if(LQktCQFslx == true){LQktCQFslx = false;}
      if(OcTYhOhDFP == true){OcTYhOhDFP = false;}
      if(EqLGEINeYO == true){EqLGEINeYO = false;}
      if(DVshFCnhGU == true){DVshFCnhGU = false;}
      if(lyMKohyqXN == true){lyMKohyqXN = false;}
      if(BnTXUTFeos == true){BnTXUTFeos = false;}
      if(CaTWtsikTf == true){CaTWtsikTf = false;}
      if(azxjcGeBQW == true){azxjcGeBQW = false;}
      if(KVHLNPBUNl == true){KVHLNPBUNl = false;}
      if(wIsNRLdoPw == true){wIsNRLdoPw = false;}
      if(yMXqlBpitN == true){yMXqlBpitN = false;}
      if(nDOShgSIIe == true){nDOShgSIIe = false;}
      if(qNXgiRGjSR == true){qNXgiRGjSR = false;}
      if(cpkUXyxPYM == true){cpkUXyxPYM = false;}
      if(YSazxrtaek == true){YSazxrtaek = false;}
      if(rUfkEkopHU == true){rUfkEkopHU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DQELRBHEYN
{ 
  void HpIzaqKQZG()
  { 
      bool uzeVExhDHN = false;
      bool KDdjsGIARP = false;
      bool LuoYCyqYuk = false;
      bool IzcMJjCHBU = false;
      bool FuTsJfrdNm = false;
      bool aaLKozKFTG = false;
      bool GJHVFLZQZI = false;
      bool DBzxJQbwOV = false;
      bool FMKIKgmSXJ = false;
      bool ZwntNwmxmy = false;
      bool FbDFiZbOBX = false;
      bool XNpxAtzyLX = false;
      bool XHrQiHMFiM = false;
      bool ceIQPdIEZj = false;
      bool FYQAlMGqtT = false;
      bool xokPZyimWb = false;
      bool MWfotrEZgW = false;
      bool ktYbSzprdW = false;
      bool AVLOOnsspR = false;
      bool uVhxWisbFs = false;
      string bREHszUnDS;
      string hJkHDBuNKS;
      string QTVeUkktfE;
      string jtCUUmWNrW;
      string llNAHjFEcm;
      string bqYkkHuYmI;
      string OZIzhnHQmP;
      string dgUmgeFUXk;
      string ffQrbrURiM;
      string rxajILcORS;
      string coRNItWlEF;
      string BZBBciIPWp;
      string TogZXfGkeU;
      string CqCFHpODjg;
      string ViMdLSwYHY;
      string CNrbymHroc;
      string nlGiDKWwtl;
      string UFfoUkcDNF;
      string jgcFRnyaGU;
      string iVOGsibcZB;
      if(bREHszUnDS == coRNItWlEF){uzeVExhDHN = true;}
      else if(coRNItWlEF == bREHszUnDS){FbDFiZbOBX = true;}
      if(hJkHDBuNKS == BZBBciIPWp){KDdjsGIARP = true;}
      else if(BZBBciIPWp == hJkHDBuNKS){XNpxAtzyLX = true;}
      if(QTVeUkktfE == TogZXfGkeU){LuoYCyqYuk = true;}
      else if(TogZXfGkeU == QTVeUkktfE){XHrQiHMFiM = true;}
      if(jtCUUmWNrW == CqCFHpODjg){IzcMJjCHBU = true;}
      else if(CqCFHpODjg == jtCUUmWNrW){ceIQPdIEZj = true;}
      if(llNAHjFEcm == ViMdLSwYHY){FuTsJfrdNm = true;}
      else if(ViMdLSwYHY == llNAHjFEcm){FYQAlMGqtT = true;}
      if(bqYkkHuYmI == CNrbymHroc){aaLKozKFTG = true;}
      else if(CNrbymHroc == bqYkkHuYmI){xokPZyimWb = true;}
      if(OZIzhnHQmP == nlGiDKWwtl){GJHVFLZQZI = true;}
      else if(nlGiDKWwtl == OZIzhnHQmP){MWfotrEZgW = true;}
      if(dgUmgeFUXk == UFfoUkcDNF){DBzxJQbwOV = true;}
      if(ffQrbrURiM == jgcFRnyaGU){FMKIKgmSXJ = true;}
      if(rxajILcORS == iVOGsibcZB){ZwntNwmxmy = true;}
      while(UFfoUkcDNF == dgUmgeFUXk){ktYbSzprdW = true;}
      while(jgcFRnyaGU == jgcFRnyaGU){AVLOOnsspR = true;}
      while(iVOGsibcZB == iVOGsibcZB){uVhxWisbFs = true;}
      if(uzeVExhDHN == true){uzeVExhDHN = false;}
      if(KDdjsGIARP == true){KDdjsGIARP = false;}
      if(LuoYCyqYuk == true){LuoYCyqYuk = false;}
      if(IzcMJjCHBU == true){IzcMJjCHBU = false;}
      if(FuTsJfrdNm == true){FuTsJfrdNm = false;}
      if(aaLKozKFTG == true){aaLKozKFTG = false;}
      if(GJHVFLZQZI == true){GJHVFLZQZI = false;}
      if(DBzxJQbwOV == true){DBzxJQbwOV = false;}
      if(FMKIKgmSXJ == true){FMKIKgmSXJ = false;}
      if(ZwntNwmxmy == true){ZwntNwmxmy = false;}
      if(FbDFiZbOBX == true){FbDFiZbOBX = false;}
      if(XNpxAtzyLX == true){XNpxAtzyLX = false;}
      if(XHrQiHMFiM == true){XHrQiHMFiM = false;}
      if(ceIQPdIEZj == true){ceIQPdIEZj = false;}
      if(FYQAlMGqtT == true){FYQAlMGqtT = false;}
      if(xokPZyimWb == true){xokPZyimWb = false;}
      if(MWfotrEZgW == true){MWfotrEZgW = false;}
      if(ktYbSzprdW == true){ktYbSzprdW = false;}
      if(AVLOOnsspR == true){AVLOOnsspR = false;}
      if(uVhxWisbFs == true){uVhxWisbFs = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YQQOMRFJZT
{ 
  void juwpTrnZPC()
  { 
      bool ZEHJtSydhl = false;
      bool HTwXSqGPyM = false;
      bool bmYsgHdGhH = false;
      bool YImKEMsrhR = false;
      bool pdDBmnNIaf = false;
      bool XqDywmfNbm = false;
      bool orwaNyflTG = false;
      bool yOgfWyjWFs = false;
      bool tVeBihFnNQ = false;
      bool fePucxgbno = false;
      bool HanUhJJWby = false;
      bool DIHisSMtqf = false;
      bool rrXDZSylDY = false;
      bool QHiQkSgKTY = false;
      bool XiLPNWXfCJ = false;
      bool jljIwPDktO = false;
      bool LXSDguZqyZ = false;
      bool kCaSIJfIoD = false;
      bool uSjsndWaTU = false;
      bool PwQENzPIhS = false;
      string smtsccISIx;
      string nyXfHlULwX;
      string WWMTQDJypk;
      string bZUIFCOZqm;
      string iFaONquwjq;
      string hdqlFWaabZ;
      string TJcWVOdPZE;
      string XtTJnQuxGZ;
      string HMOyCxhhCi;
      string bmyRTYcDry;
      string MfWeTnKhyo;
      string acUBRHudUY;
      string fuMqcjwCoO;
      string mqFHMbojdO;
      string mzjNHySwAM;
      string dyiTSRSfCj;
      string qIsiBiXNnp;
      string RLAFlfKzid;
      string RABckoHknu;
      string IJgtXdGBLX;
      if(smtsccISIx == MfWeTnKhyo){ZEHJtSydhl = true;}
      else if(MfWeTnKhyo == smtsccISIx){HanUhJJWby = true;}
      if(nyXfHlULwX == acUBRHudUY){HTwXSqGPyM = true;}
      else if(acUBRHudUY == nyXfHlULwX){DIHisSMtqf = true;}
      if(WWMTQDJypk == fuMqcjwCoO){bmYsgHdGhH = true;}
      else if(fuMqcjwCoO == WWMTQDJypk){rrXDZSylDY = true;}
      if(bZUIFCOZqm == mqFHMbojdO){YImKEMsrhR = true;}
      else if(mqFHMbojdO == bZUIFCOZqm){QHiQkSgKTY = true;}
      if(iFaONquwjq == mzjNHySwAM){pdDBmnNIaf = true;}
      else if(mzjNHySwAM == iFaONquwjq){XiLPNWXfCJ = true;}
      if(hdqlFWaabZ == dyiTSRSfCj){XqDywmfNbm = true;}
      else if(dyiTSRSfCj == hdqlFWaabZ){jljIwPDktO = true;}
      if(TJcWVOdPZE == qIsiBiXNnp){orwaNyflTG = true;}
      else if(qIsiBiXNnp == TJcWVOdPZE){LXSDguZqyZ = true;}
      if(XtTJnQuxGZ == RLAFlfKzid){yOgfWyjWFs = true;}
      if(HMOyCxhhCi == RABckoHknu){tVeBihFnNQ = true;}
      if(bmyRTYcDry == IJgtXdGBLX){fePucxgbno = true;}
      while(RLAFlfKzid == XtTJnQuxGZ){kCaSIJfIoD = true;}
      while(RABckoHknu == RABckoHknu){uSjsndWaTU = true;}
      while(IJgtXdGBLX == IJgtXdGBLX){PwQENzPIhS = true;}
      if(ZEHJtSydhl == true){ZEHJtSydhl = false;}
      if(HTwXSqGPyM == true){HTwXSqGPyM = false;}
      if(bmYsgHdGhH == true){bmYsgHdGhH = false;}
      if(YImKEMsrhR == true){YImKEMsrhR = false;}
      if(pdDBmnNIaf == true){pdDBmnNIaf = false;}
      if(XqDywmfNbm == true){XqDywmfNbm = false;}
      if(orwaNyflTG == true){orwaNyflTG = false;}
      if(yOgfWyjWFs == true){yOgfWyjWFs = false;}
      if(tVeBihFnNQ == true){tVeBihFnNQ = false;}
      if(fePucxgbno == true){fePucxgbno = false;}
      if(HanUhJJWby == true){HanUhJJWby = false;}
      if(DIHisSMtqf == true){DIHisSMtqf = false;}
      if(rrXDZSylDY == true){rrXDZSylDY = false;}
      if(QHiQkSgKTY == true){QHiQkSgKTY = false;}
      if(XiLPNWXfCJ == true){XiLPNWXfCJ = false;}
      if(jljIwPDktO == true){jljIwPDktO = false;}
      if(LXSDguZqyZ == true){LXSDguZqyZ = false;}
      if(kCaSIJfIoD == true){kCaSIJfIoD = false;}
      if(uSjsndWaTU == true){uSjsndWaTU = false;}
      if(PwQENzPIhS == true){PwQENzPIhS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BGZEEOBVSO
{ 
  void RckmlGlsnE()
  { 
      bool lWJpVKrWUL = false;
      bool BSmTjTWAWf = false;
      bool dLpTNDnFci = false;
      bool xPuyTtGVKn = false;
      bool JrYIqmmuSo = false;
      bool gdUqSafobR = false;
      bool WmskOlWozp = false;
      bool NBphWkmgbZ = false;
      bool PslnCTtEfr = false;
      bool NQssRVzmtF = false;
      bool QyucafmlTh = false;
      bool nzMBBcAEit = false;
      bool rRJGbwIwQA = false;
      bool lzmWJRyJMV = false;
      bool fHjxEODqrn = false;
      bool UtEzMzonBe = false;
      bool WqVTwoLpri = false;
      bool eqrMOjgLCN = false;
      bool rQEbequmJK = false;
      bool cJIhzolQBO = false;
      string gjnTpMhhLG;
      string aPJjjSJXgb;
      string yAqeYcrdNP;
      string mLytpoNihJ;
      string JhlErsaNRH;
      string ytkBCgrIuM;
      string TDTwTOQPtt;
      string FapWSQOlme;
      string BdMGQrZwqI;
      string wEmUpFNojc;
      string cKTXCXEOik;
      string SFVPKoqMfb;
      string nTYkxAmVgN;
      string DYBJCqKnEs;
      string mLNFpqiWJA;
      string NAJInIkiZB;
      string jigVhsELNT;
      string eCIlnbGSuG;
      string RPLfzCPsUs;
      string FaODIEeqWw;
      if(gjnTpMhhLG == cKTXCXEOik){lWJpVKrWUL = true;}
      else if(cKTXCXEOik == gjnTpMhhLG){QyucafmlTh = true;}
      if(aPJjjSJXgb == SFVPKoqMfb){BSmTjTWAWf = true;}
      else if(SFVPKoqMfb == aPJjjSJXgb){nzMBBcAEit = true;}
      if(yAqeYcrdNP == nTYkxAmVgN){dLpTNDnFci = true;}
      else if(nTYkxAmVgN == yAqeYcrdNP){rRJGbwIwQA = true;}
      if(mLytpoNihJ == DYBJCqKnEs){xPuyTtGVKn = true;}
      else if(DYBJCqKnEs == mLytpoNihJ){lzmWJRyJMV = true;}
      if(JhlErsaNRH == mLNFpqiWJA){JrYIqmmuSo = true;}
      else if(mLNFpqiWJA == JhlErsaNRH){fHjxEODqrn = true;}
      if(ytkBCgrIuM == NAJInIkiZB){gdUqSafobR = true;}
      else if(NAJInIkiZB == ytkBCgrIuM){UtEzMzonBe = true;}
      if(TDTwTOQPtt == jigVhsELNT){WmskOlWozp = true;}
      else if(jigVhsELNT == TDTwTOQPtt){WqVTwoLpri = true;}
      if(FapWSQOlme == eCIlnbGSuG){NBphWkmgbZ = true;}
      if(BdMGQrZwqI == RPLfzCPsUs){PslnCTtEfr = true;}
      if(wEmUpFNojc == FaODIEeqWw){NQssRVzmtF = true;}
      while(eCIlnbGSuG == FapWSQOlme){eqrMOjgLCN = true;}
      while(RPLfzCPsUs == RPLfzCPsUs){rQEbequmJK = true;}
      while(FaODIEeqWw == FaODIEeqWw){cJIhzolQBO = true;}
      if(lWJpVKrWUL == true){lWJpVKrWUL = false;}
      if(BSmTjTWAWf == true){BSmTjTWAWf = false;}
      if(dLpTNDnFci == true){dLpTNDnFci = false;}
      if(xPuyTtGVKn == true){xPuyTtGVKn = false;}
      if(JrYIqmmuSo == true){JrYIqmmuSo = false;}
      if(gdUqSafobR == true){gdUqSafobR = false;}
      if(WmskOlWozp == true){WmskOlWozp = false;}
      if(NBphWkmgbZ == true){NBphWkmgbZ = false;}
      if(PslnCTtEfr == true){PslnCTtEfr = false;}
      if(NQssRVzmtF == true){NQssRVzmtF = false;}
      if(QyucafmlTh == true){QyucafmlTh = false;}
      if(nzMBBcAEit == true){nzMBBcAEit = false;}
      if(rRJGbwIwQA == true){rRJGbwIwQA = false;}
      if(lzmWJRyJMV == true){lzmWJRyJMV = false;}
      if(fHjxEODqrn == true){fHjxEODqrn = false;}
      if(UtEzMzonBe == true){UtEzMzonBe = false;}
      if(WqVTwoLpri == true){WqVTwoLpri = false;}
      if(eqrMOjgLCN == true){eqrMOjgLCN = false;}
      if(rQEbequmJK == true){rQEbequmJK = false;}
      if(cJIhzolQBO == true){cJIhzolQBO = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class EXEMQTMNLD
{ 
  void BHlMMBqeCT()
  { 
      bool HUetEEfjUE = false;
      bool FQrxDItthw = false;
      bool qlfmLgqXwg = false;
      bool MfjumGVyWZ = false;
      bool xDbQjArWaW = false;
      bool TsOoFVoqiX = false;
      bool NHnnqYgwJw = false;
      bool lwWIznKPDK = false;
      bool nUwYNSRupR = false;
      bool RePzLoOmYK = false;
      bool TuNekWWFpR = false;
      bool KYnXjrKOaY = false;
      bool HlGqeHfXwJ = false;
      bool YWqrpsGRnp = false;
      bool wLFYGrpqOS = false;
      bool eAMzDhoVzd = false;
      bool qzIgrnhuhp = false;
      bool lrakcuggjm = false;
      bool QoIcdXKqoI = false;
      bool EkSKzwDYfS = false;
      string lxCLSRHAUZ;
      string uMOjYBWaem;
      string uffoRPDrEt;
      string aHIlKCIDGW;
      string DpDJBYtcBT;
      string lWcqpGQMtC;
      string NOwZynDbYW;
      string rSTIjFcWmr;
      string VZqjIDdRwE;
      string kuJDJoscKN;
      string UREdbSCUHk;
      string dtZBgmXLRu;
      string JlfTlxJPyu;
      string LzLYXDVMIo;
      string AJJKapVIWV;
      string QhLPEhlkrd;
      string JmVMZdhAcc;
      string RLCbtzcUru;
      string aBrbdcmwUo;
      string RkTdfNmwkw;
      if(lxCLSRHAUZ == UREdbSCUHk){HUetEEfjUE = true;}
      else if(UREdbSCUHk == lxCLSRHAUZ){TuNekWWFpR = true;}
      if(uMOjYBWaem == dtZBgmXLRu){FQrxDItthw = true;}
      else if(dtZBgmXLRu == uMOjYBWaem){KYnXjrKOaY = true;}
      if(uffoRPDrEt == JlfTlxJPyu){qlfmLgqXwg = true;}
      else if(JlfTlxJPyu == uffoRPDrEt){HlGqeHfXwJ = true;}
      if(aHIlKCIDGW == LzLYXDVMIo){MfjumGVyWZ = true;}
      else if(LzLYXDVMIo == aHIlKCIDGW){YWqrpsGRnp = true;}
      if(DpDJBYtcBT == AJJKapVIWV){xDbQjArWaW = true;}
      else if(AJJKapVIWV == DpDJBYtcBT){wLFYGrpqOS = true;}
      if(lWcqpGQMtC == QhLPEhlkrd){TsOoFVoqiX = true;}
      else if(QhLPEhlkrd == lWcqpGQMtC){eAMzDhoVzd = true;}
      if(NOwZynDbYW == JmVMZdhAcc){NHnnqYgwJw = true;}
      else if(JmVMZdhAcc == NOwZynDbYW){qzIgrnhuhp = true;}
      if(rSTIjFcWmr == RLCbtzcUru){lwWIznKPDK = true;}
      if(VZqjIDdRwE == aBrbdcmwUo){nUwYNSRupR = true;}
      if(kuJDJoscKN == RkTdfNmwkw){RePzLoOmYK = true;}
      while(RLCbtzcUru == rSTIjFcWmr){lrakcuggjm = true;}
      while(aBrbdcmwUo == aBrbdcmwUo){QoIcdXKqoI = true;}
      while(RkTdfNmwkw == RkTdfNmwkw){EkSKzwDYfS = true;}
      if(HUetEEfjUE == true){HUetEEfjUE = false;}
      if(FQrxDItthw == true){FQrxDItthw = false;}
      if(qlfmLgqXwg == true){qlfmLgqXwg = false;}
      if(MfjumGVyWZ == true){MfjumGVyWZ = false;}
      if(xDbQjArWaW == true){xDbQjArWaW = false;}
      if(TsOoFVoqiX == true){TsOoFVoqiX = false;}
      if(NHnnqYgwJw == true){NHnnqYgwJw = false;}
      if(lwWIznKPDK == true){lwWIznKPDK = false;}
      if(nUwYNSRupR == true){nUwYNSRupR = false;}
      if(RePzLoOmYK == true){RePzLoOmYK = false;}
      if(TuNekWWFpR == true){TuNekWWFpR = false;}
      if(KYnXjrKOaY == true){KYnXjrKOaY = false;}
      if(HlGqeHfXwJ == true){HlGqeHfXwJ = false;}
      if(YWqrpsGRnp == true){YWqrpsGRnp = false;}
      if(wLFYGrpqOS == true){wLFYGrpqOS = false;}
      if(eAMzDhoVzd == true){eAMzDhoVzd = false;}
      if(qzIgrnhuhp == true){qzIgrnhuhp = false;}
      if(lrakcuggjm == true){lrakcuggjm = false;}
      if(QoIcdXKqoI == true){QoIcdXKqoI = false;}
      if(EkSKzwDYfS == true){EkSKzwDYfS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FHXKOYPAMU
{ 
  void qLEqHWqSax()
  { 
      bool RByWiSJHLJ = false;
      bool WnBOZBZjGn = false;
      bool XHbXRxwKcY = false;
      bool aDpCfAeFgN = false;
      bool QriHgyKXst = false;
      bool PQXaxJZaFl = false;
      bool WzemhSyyhZ = false;
      bool WTCsXeXUqS = false;
      bool lXQiIJfOEV = false;
      bool aEAtqRMwYN = false;
      bool zLpTWuNeDR = false;
      bool kFilAyiwdX = false;
      bool mjsLzRTICF = false;
      bool SbudTahwLN = false;
      bool wBKmkVUOtr = false;
      bool iQRBaPReUE = false;
      bool bFbxzJKFSY = false;
      bool PlkQbkYGad = false;
      bool wFNJWBKtCD = false;
      bool IDaGlqYMma = false;
      string WRbKetthQZ;
      string yYHdnqVWwV;
      string nEoPLicdkG;
      string TMhOPXCIHC;
      string ALqlAxWDxO;
      string wVCrGRbLzw;
      string RPxKFlprOP;
      string oMOsUcNCPO;
      string LYgVKyJoIx;
      string uqJLkkOfgN;
      string GeogZiGIWc;
      string EZujkriaAG;
      string cAjQUxDDIi;
      string OgVCIgjzwa;
      string rASesVuZar;
      string CnfZkFtYMb;
      string eCAnOXDsaD;
      string IidYNbhFXy;
      string CdcQgaAuQD;
      string lLlTuoAePU;
      if(WRbKetthQZ == GeogZiGIWc){RByWiSJHLJ = true;}
      else if(GeogZiGIWc == WRbKetthQZ){zLpTWuNeDR = true;}
      if(yYHdnqVWwV == EZujkriaAG){WnBOZBZjGn = true;}
      else if(EZujkriaAG == yYHdnqVWwV){kFilAyiwdX = true;}
      if(nEoPLicdkG == cAjQUxDDIi){XHbXRxwKcY = true;}
      else if(cAjQUxDDIi == nEoPLicdkG){mjsLzRTICF = true;}
      if(TMhOPXCIHC == OgVCIgjzwa){aDpCfAeFgN = true;}
      else if(OgVCIgjzwa == TMhOPXCIHC){SbudTahwLN = true;}
      if(ALqlAxWDxO == rASesVuZar){QriHgyKXst = true;}
      else if(rASesVuZar == ALqlAxWDxO){wBKmkVUOtr = true;}
      if(wVCrGRbLzw == CnfZkFtYMb){PQXaxJZaFl = true;}
      else if(CnfZkFtYMb == wVCrGRbLzw){iQRBaPReUE = true;}
      if(RPxKFlprOP == eCAnOXDsaD){WzemhSyyhZ = true;}
      else if(eCAnOXDsaD == RPxKFlprOP){bFbxzJKFSY = true;}
      if(oMOsUcNCPO == IidYNbhFXy){WTCsXeXUqS = true;}
      if(LYgVKyJoIx == CdcQgaAuQD){lXQiIJfOEV = true;}
      if(uqJLkkOfgN == lLlTuoAePU){aEAtqRMwYN = true;}
      while(IidYNbhFXy == oMOsUcNCPO){PlkQbkYGad = true;}
      while(CdcQgaAuQD == CdcQgaAuQD){wFNJWBKtCD = true;}
      while(lLlTuoAePU == lLlTuoAePU){IDaGlqYMma = true;}
      if(RByWiSJHLJ == true){RByWiSJHLJ = false;}
      if(WnBOZBZjGn == true){WnBOZBZjGn = false;}
      if(XHbXRxwKcY == true){XHbXRxwKcY = false;}
      if(aDpCfAeFgN == true){aDpCfAeFgN = false;}
      if(QriHgyKXst == true){QriHgyKXst = false;}
      if(PQXaxJZaFl == true){PQXaxJZaFl = false;}
      if(WzemhSyyhZ == true){WzemhSyyhZ = false;}
      if(WTCsXeXUqS == true){WTCsXeXUqS = false;}
      if(lXQiIJfOEV == true){lXQiIJfOEV = false;}
      if(aEAtqRMwYN == true){aEAtqRMwYN = false;}
      if(zLpTWuNeDR == true){zLpTWuNeDR = false;}
      if(kFilAyiwdX == true){kFilAyiwdX = false;}
      if(mjsLzRTICF == true){mjsLzRTICF = false;}
      if(SbudTahwLN == true){SbudTahwLN = false;}
      if(wBKmkVUOtr == true){wBKmkVUOtr = false;}
      if(iQRBaPReUE == true){iQRBaPReUE = false;}
      if(bFbxzJKFSY == true){bFbxzJKFSY = false;}
      if(PlkQbkYGad == true){PlkQbkYGad = false;}
      if(wFNJWBKtCD == true){wFNJWBKtCD = false;}
      if(IDaGlqYMma == true){IDaGlqYMma = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class KFIOGCKMNS
{ 
  void fEAQPBnNiQ()
  { 
      bool FnGMiIgfGH = false;
      bool EEuYLJpmUc = false;
      bool HyTErNUXrT = false;
      bool bPrOgsEMXl = false;
      bool fVcMxTOoIR = false;
      bool HxfHlHUBTV = false;
      bool nJnmfoLfrR = false;
      bool EdPixkVGHX = false;
      bool zeOZMdikcZ = false;
      bool YCrCxVhogI = false;
      bool HhxqMchwJA = false;
      bool lOtuVItNaB = false;
      bool FaDSrClNka = false;
      bool CfYyQRgWNi = false;
      bool wmacnJHwgn = false;
      bool bUOfDgeezK = false;
      bool qUKhyFpweD = false;
      bool bWzqFRAZeh = false;
      bool togxzhQruj = false;
      bool mCXDrabsdh = false;
      string ChoiKfjRxU;
      string sVPKkLGZRX;
      string ZdeEfrQIqt;
      string GJSElgrrTJ;
      string fGPSzGymrY;
      string GYSwyYCjCR;
      string PaZCNjDLXE;
      string TNFIrTNMFZ;
      string tcPSnhpVGN;
      string dHpCsEfMEO;
      string CmOnFrTJEd;
      string KpecXlLGOf;
      string shFKaLXDOn;
      string mAHpWGNbNu;
      string NfRbCncMdH;
      string sZyYUUhDMs;
      string UPuPqJhnGQ;
      string XjrSRNQcIm;
      string hKYHuNCiUB;
      string YJOUaykBks;
      if(ChoiKfjRxU == CmOnFrTJEd){FnGMiIgfGH = true;}
      else if(CmOnFrTJEd == ChoiKfjRxU){HhxqMchwJA = true;}
      if(sVPKkLGZRX == KpecXlLGOf){EEuYLJpmUc = true;}
      else if(KpecXlLGOf == sVPKkLGZRX){lOtuVItNaB = true;}
      if(ZdeEfrQIqt == shFKaLXDOn){HyTErNUXrT = true;}
      else if(shFKaLXDOn == ZdeEfrQIqt){FaDSrClNka = true;}
      if(GJSElgrrTJ == mAHpWGNbNu){bPrOgsEMXl = true;}
      else if(mAHpWGNbNu == GJSElgrrTJ){CfYyQRgWNi = true;}
      if(fGPSzGymrY == NfRbCncMdH){fVcMxTOoIR = true;}
      else if(NfRbCncMdH == fGPSzGymrY){wmacnJHwgn = true;}
      if(GYSwyYCjCR == sZyYUUhDMs){HxfHlHUBTV = true;}
      else if(sZyYUUhDMs == GYSwyYCjCR){bUOfDgeezK = true;}
      if(PaZCNjDLXE == UPuPqJhnGQ){nJnmfoLfrR = true;}
      else if(UPuPqJhnGQ == PaZCNjDLXE){qUKhyFpweD = true;}
      if(TNFIrTNMFZ == XjrSRNQcIm){EdPixkVGHX = true;}
      if(tcPSnhpVGN == hKYHuNCiUB){zeOZMdikcZ = true;}
      if(dHpCsEfMEO == YJOUaykBks){YCrCxVhogI = true;}
      while(XjrSRNQcIm == TNFIrTNMFZ){bWzqFRAZeh = true;}
      while(hKYHuNCiUB == hKYHuNCiUB){togxzhQruj = true;}
      while(YJOUaykBks == YJOUaykBks){mCXDrabsdh = true;}
      if(FnGMiIgfGH == true){FnGMiIgfGH = false;}
      if(EEuYLJpmUc == true){EEuYLJpmUc = false;}
      if(HyTErNUXrT == true){HyTErNUXrT = false;}
      if(bPrOgsEMXl == true){bPrOgsEMXl = false;}
      if(fVcMxTOoIR == true){fVcMxTOoIR = false;}
      if(HxfHlHUBTV == true){HxfHlHUBTV = false;}
      if(nJnmfoLfrR == true){nJnmfoLfrR = false;}
      if(EdPixkVGHX == true){EdPixkVGHX = false;}
      if(zeOZMdikcZ == true){zeOZMdikcZ = false;}
      if(YCrCxVhogI == true){YCrCxVhogI = false;}
      if(HhxqMchwJA == true){HhxqMchwJA = false;}
      if(lOtuVItNaB == true){lOtuVItNaB = false;}
      if(FaDSrClNka == true){FaDSrClNka = false;}
      if(CfYyQRgWNi == true){CfYyQRgWNi = false;}
      if(wmacnJHwgn == true){wmacnJHwgn = false;}
      if(bUOfDgeezK == true){bUOfDgeezK = false;}
      if(qUKhyFpweD == true){qUKhyFpweD = false;}
      if(bWzqFRAZeh == true){bWzqFRAZeh = false;}
      if(togxzhQruj == true){togxzhQruj = false;}
      if(mCXDrabsdh == true){mCXDrabsdh = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ASYKDZCWDQ
{ 
  void LUPCWoqkLH()
  { 
      bool oPQRxXVQsi = false;
      bool DWOwjWKWIC = false;
      bool UftMUepBoV = false;
      bool PeRLzrAXmF = false;
      bool XLegElnrcq = false;
      bool rNLkNIVDxS = false;
      bool gUbWSYFCxr = false;
      bool WmjDeJBMIh = false;
      bool pxYycCTgxp = false;
      bool VxZTXDiHOe = false;
      bool gkujTEdHGD = false;
      bool HJtPbjwdsQ = false;
      bool FXFGWZNPea = false;
      bool asxcAECndY = false;
      bool YLasiLCKrn = false;
      bool HFOyZJfnNt = false;
      bool qObecNLROW = false;
      bool gHzUqsxLXK = false;
      bool WCWqTVezCq = false;
      bool XOajgKiuQU = false;
      string ImyNIfbdEE;
      string oEXqowtqQm;
      string jsGKJgkfdG;
      string hhaToTaWoW;
      string CCXThnYlmx;
      string NoSCHPgnDa;
      string twzyWHdDBU;
      string VWBtkgVmbV;
      string IyrfsrPXLZ;
      string mmTOcFDYwa;
      string JUkSapnYQa;
      string stLhRwESFd;
      string qGqahiClQe;
      string dwpALmNZFC;
      string EqGkfsalwf;
      string BjTSjDdJkr;
      string nBHlxxDKVd;
      string idNDZAzGtn;
      string HQpjctbXQI;
      string JDnMNDjUni;
      if(ImyNIfbdEE == JUkSapnYQa){oPQRxXVQsi = true;}
      else if(JUkSapnYQa == ImyNIfbdEE){gkujTEdHGD = true;}
      if(oEXqowtqQm == stLhRwESFd){DWOwjWKWIC = true;}
      else if(stLhRwESFd == oEXqowtqQm){HJtPbjwdsQ = true;}
      if(jsGKJgkfdG == qGqahiClQe){UftMUepBoV = true;}
      else if(qGqahiClQe == jsGKJgkfdG){FXFGWZNPea = true;}
      if(hhaToTaWoW == dwpALmNZFC){PeRLzrAXmF = true;}
      else if(dwpALmNZFC == hhaToTaWoW){asxcAECndY = true;}
      if(CCXThnYlmx == EqGkfsalwf){XLegElnrcq = true;}
      else if(EqGkfsalwf == CCXThnYlmx){YLasiLCKrn = true;}
      if(NoSCHPgnDa == BjTSjDdJkr){rNLkNIVDxS = true;}
      else if(BjTSjDdJkr == NoSCHPgnDa){HFOyZJfnNt = true;}
      if(twzyWHdDBU == nBHlxxDKVd){gUbWSYFCxr = true;}
      else if(nBHlxxDKVd == twzyWHdDBU){qObecNLROW = true;}
      if(VWBtkgVmbV == idNDZAzGtn){WmjDeJBMIh = true;}
      if(IyrfsrPXLZ == HQpjctbXQI){pxYycCTgxp = true;}
      if(mmTOcFDYwa == JDnMNDjUni){VxZTXDiHOe = true;}
      while(idNDZAzGtn == VWBtkgVmbV){gHzUqsxLXK = true;}
      while(HQpjctbXQI == HQpjctbXQI){WCWqTVezCq = true;}
      while(JDnMNDjUni == JDnMNDjUni){XOajgKiuQU = true;}
      if(oPQRxXVQsi == true){oPQRxXVQsi = false;}
      if(DWOwjWKWIC == true){DWOwjWKWIC = false;}
      if(UftMUepBoV == true){UftMUepBoV = false;}
      if(PeRLzrAXmF == true){PeRLzrAXmF = false;}
      if(XLegElnrcq == true){XLegElnrcq = false;}
      if(rNLkNIVDxS == true){rNLkNIVDxS = false;}
      if(gUbWSYFCxr == true){gUbWSYFCxr = false;}
      if(WmjDeJBMIh == true){WmjDeJBMIh = false;}
      if(pxYycCTgxp == true){pxYycCTgxp = false;}
      if(VxZTXDiHOe == true){VxZTXDiHOe = false;}
      if(gkujTEdHGD == true){gkujTEdHGD = false;}
      if(HJtPbjwdsQ == true){HJtPbjwdsQ = false;}
      if(FXFGWZNPea == true){FXFGWZNPea = false;}
      if(asxcAECndY == true){asxcAECndY = false;}
      if(YLasiLCKrn == true){YLasiLCKrn = false;}
      if(HFOyZJfnNt == true){HFOyZJfnNt = false;}
      if(qObecNLROW == true){qObecNLROW = false;}
      if(gHzUqsxLXK == true){gHzUqsxLXK = false;}
      if(WCWqTVezCq == true){WCWqTVezCq = false;}
      if(XOajgKiuQU == true){XOajgKiuQU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class POIROKJFLT
{ 
  void ODtHEJGoib()
  { 
      bool mWVonuPZCZ = false;
      bool ulFBoXeowA = false;
      bool HpICBhhxdK = false;
      bool DmfrnquKSg = false;
      bool ArZEtjtsdN = false;
      bool XAClLxNSmF = false;
      bool NSuqVIFNNy = false;
      bool bTVOAPPEIR = false;
      bool nnSQmDktfp = false;
      bool ccWMTgwiik = false;
      bool dgfekDdmlw = false;
      bool hJKVRYzdDR = false;
      bool XLgWVzxJuS = false;
      bool SfUWmGHmei = false;
      bool YxHjHTRAut = false;
      bool hReQNSAJJl = false;
      bool GUqTcBnniL = false;
      bool RMzUsZDwOB = false;
      bool gwOBjABEIT = false;
      bool GycrcEGmHG = false;
      string ZAKVSgXdJI;
      string TonceqIjKb;
      string cFkfxRrpqX;
      string ngnAufZAMK;
      string EJRfknPDIr;
      string cPXthUOeXA;
      string ngmlUmAngy;
      string RHXzWTmEtJ;
      string CnhyuYIsRa;
      string QtbLWXVfLx;
      string maWKaUqXXO;
      string gKjOyzzopu;
      string dGVFhXeMcz;
      string xHrxVaRWWa;
      string gkVDsFJNMW;
      string XgoPSfERcD;
      string oeuXQZRmrX;
      string hbRPNskSLb;
      string NRzGbQRbdo;
      string VAtTkCoylT;
      if(ZAKVSgXdJI == maWKaUqXXO){mWVonuPZCZ = true;}
      else if(maWKaUqXXO == ZAKVSgXdJI){dgfekDdmlw = true;}
      if(TonceqIjKb == gKjOyzzopu){ulFBoXeowA = true;}
      else if(gKjOyzzopu == TonceqIjKb){hJKVRYzdDR = true;}
      if(cFkfxRrpqX == dGVFhXeMcz){HpICBhhxdK = true;}
      else if(dGVFhXeMcz == cFkfxRrpqX){XLgWVzxJuS = true;}
      if(ngnAufZAMK == xHrxVaRWWa){DmfrnquKSg = true;}
      else if(xHrxVaRWWa == ngnAufZAMK){SfUWmGHmei = true;}
      if(EJRfknPDIr == gkVDsFJNMW){ArZEtjtsdN = true;}
      else if(gkVDsFJNMW == EJRfknPDIr){YxHjHTRAut = true;}
      if(cPXthUOeXA == XgoPSfERcD){XAClLxNSmF = true;}
      else if(XgoPSfERcD == cPXthUOeXA){hReQNSAJJl = true;}
      if(ngmlUmAngy == oeuXQZRmrX){NSuqVIFNNy = true;}
      else if(oeuXQZRmrX == ngmlUmAngy){GUqTcBnniL = true;}
      if(RHXzWTmEtJ == hbRPNskSLb){bTVOAPPEIR = true;}
      if(CnhyuYIsRa == NRzGbQRbdo){nnSQmDktfp = true;}
      if(QtbLWXVfLx == VAtTkCoylT){ccWMTgwiik = true;}
      while(hbRPNskSLb == RHXzWTmEtJ){RMzUsZDwOB = true;}
      while(NRzGbQRbdo == NRzGbQRbdo){gwOBjABEIT = true;}
      while(VAtTkCoylT == VAtTkCoylT){GycrcEGmHG = true;}
      if(mWVonuPZCZ == true){mWVonuPZCZ = false;}
      if(ulFBoXeowA == true){ulFBoXeowA = false;}
      if(HpICBhhxdK == true){HpICBhhxdK = false;}
      if(DmfrnquKSg == true){DmfrnquKSg = false;}
      if(ArZEtjtsdN == true){ArZEtjtsdN = false;}
      if(XAClLxNSmF == true){XAClLxNSmF = false;}
      if(NSuqVIFNNy == true){NSuqVIFNNy = false;}
      if(bTVOAPPEIR == true){bTVOAPPEIR = false;}
      if(nnSQmDktfp == true){nnSQmDktfp = false;}
      if(ccWMTgwiik == true){ccWMTgwiik = false;}
      if(dgfekDdmlw == true){dgfekDdmlw = false;}
      if(hJKVRYzdDR == true){hJKVRYzdDR = false;}
      if(XLgWVzxJuS == true){XLgWVzxJuS = false;}
      if(SfUWmGHmei == true){SfUWmGHmei = false;}
      if(YxHjHTRAut == true){YxHjHTRAut = false;}
      if(hReQNSAJJl == true){hReQNSAJJl = false;}
      if(GUqTcBnniL == true){GUqTcBnniL = false;}
      if(RMzUsZDwOB == true){RMzUsZDwOB = false;}
      if(gwOBjABEIT == true){gwOBjABEIT = false;}
      if(GycrcEGmHG == true){GycrcEGmHG = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HZVKJCCTNK
{ 
  void LOINQqtrUA()
  { 
      bool DiJDyWAMQk = false;
      bool tWlpQtzRXz = false;
      bool TpXseCSmYp = false;
      bool DUFMrEMoKL = false;
      bool ocUCfVuIFd = false;
      bool jMCuwNLQXw = false;
      bool DMmZgZRYxL = false;
      bool hJGMJqUVNI = false;
      bool GJguGqSJPd = false;
      bool swROHcotUG = false;
      bool EVKtuZFokF = false;
      bool soCPoserTp = false;
      bool GIITSlrrFg = false;
      bool BZAHEkymzf = false;
      bool lcXKkEsCYN = false;
      bool xxaezwMqbJ = false;
      bool hMjEESouKE = false;
      bool uqrmDdjyiO = false;
      bool rpSriDzUHH = false;
      bool XHLpyYsSGS = false;
      string CZTLPaWQnY;
      string VokkRxENaH;
      string GrClpBaVJr;
      string iGCVuGxleQ;
      string sFshWFZJXp;
      string juerrJXhVO;
      string zBGuoOmPpK;
      string zRiKOAyUOj;
      string aRySHnaKck;
      string iPQPQrzyrH;
      string SKTNpSLUcq;
      string KDTlnhCmmB;
      string nblEajBTsW;
      string BAbHQQxkco;
      string bFglaeueai;
      string oLKpTpXPlF;
      string QrbGpJaLUX;
      string jRKxZhuAcw;
      string nWrJhppkna;
      string shyWXidauQ;
      if(CZTLPaWQnY == SKTNpSLUcq){DiJDyWAMQk = true;}
      else if(SKTNpSLUcq == CZTLPaWQnY){EVKtuZFokF = true;}
      if(VokkRxENaH == KDTlnhCmmB){tWlpQtzRXz = true;}
      else if(KDTlnhCmmB == VokkRxENaH){soCPoserTp = true;}
      if(GrClpBaVJr == nblEajBTsW){TpXseCSmYp = true;}
      else if(nblEajBTsW == GrClpBaVJr){GIITSlrrFg = true;}
      if(iGCVuGxleQ == BAbHQQxkco){DUFMrEMoKL = true;}
      else if(BAbHQQxkco == iGCVuGxleQ){BZAHEkymzf = true;}
      if(sFshWFZJXp == bFglaeueai){ocUCfVuIFd = true;}
      else if(bFglaeueai == sFshWFZJXp){lcXKkEsCYN = true;}
      if(juerrJXhVO == oLKpTpXPlF){jMCuwNLQXw = true;}
      else if(oLKpTpXPlF == juerrJXhVO){xxaezwMqbJ = true;}
      if(zBGuoOmPpK == QrbGpJaLUX){DMmZgZRYxL = true;}
      else if(QrbGpJaLUX == zBGuoOmPpK){hMjEESouKE = true;}
      if(zRiKOAyUOj == jRKxZhuAcw){hJGMJqUVNI = true;}
      if(aRySHnaKck == nWrJhppkna){GJguGqSJPd = true;}
      if(iPQPQrzyrH == shyWXidauQ){swROHcotUG = true;}
      while(jRKxZhuAcw == zRiKOAyUOj){uqrmDdjyiO = true;}
      while(nWrJhppkna == nWrJhppkna){rpSriDzUHH = true;}
      while(shyWXidauQ == shyWXidauQ){XHLpyYsSGS = true;}
      if(DiJDyWAMQk == true){DiJDyWAMQk = false;}
      if(tWlpQtzRXz == true){tWlpQtzRXz = false;}
      if(TpXseCSmYp == true){TpXseCSmYp = false;}
      if(DUFMrEMoKL == true){DUFMrEMoKL = false;}
      if(ocUCfVuIFd == true){ocUCfVuIFd = false;}
      if(jMCuwNLQXw == true){jMCuwNLQXw = false;}
      if(DMmZgZRYxL == true){DMmZgZRYxL = false;}
      if(hJGMJqUVNI == true){hJGMJqUVNI = false;}
      if(GJguGqSJPd == true){GJguGqSJPd = false;}
      if(swROHcotUG == true){swROHcotUG = false;}
      if(EVKtuZFokF == true){EVKtuZFokF = false;}
      if(soCPoserTp == true){soCPoserTp = false;}
      if(GIITSlrrFg == true){GIITSlrrFg = false;}
      if(BZAHEkymzf == true){BZAHEkymzf = false;}
      if(lcXKkEsCYN == true){lcXKkEsCYN = false;}
      if(xxaezwMqbJ == true){xxaezwMqbJ = false;}
      if(hMjEESouKE == true){hMjEESouKE = false;}
      if(uqrmDdjyiO == true){uqrmDdjyiO = false;}
      if(rpSriDzUHH == true){rpSriDzUHH = false;}
      if(XHLpyYsSGS == true){XHLpyYsSGS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JJGUHQMUGO
{ 
  void RLhUOCElFT()
  { 
      bool icTxLxRPYW = false;
      bool RpTPAJREUg = false;
      bool AKewYReSJW = false;
      bool LGFjhOEaNK = false;
      bool fGzmFNtQkM = false;
      bool cqrbGYxWXs = false;
      bool SyGBLLkjgk = false;
      bool EpqJAsKqqL = false;
      bool WNgxsTNdcJ = false;
      bool QgCTrhAybs = false;
      bool cKizHmAhaN = false;
      bool OFfMVEQIHq = false;
      bool yGGXUWmWRS = false;
      bool rLOxiMegoG = false;
      bool TeDPtKBpxn = false;
      bool QrqkuVDKek = false;
      bool zdQHUCqhdJ = false;
      bool YiSAXSDqcJ = false;
      bool VxYdBAUdCN = false;
      bool JTCEHoOuur = false;
      string txzcQqMssG;
      string pdHwdiYVfy;
      string mGmSuQcDiO;
      string akiOHbcxLN;
      string uOJbRpAVUG;
      string TzOyYmKfhA;
      string NJKefEuroC;
      string GaJRDYxLBE;
      string PGhcuaWkoT;
      string RxPKAkEeuM;
      string uHfSArowtA;
      string pyeckHbgiE;
      string hJDUjJKyRR;
      string USPoJIZPcD;
      string lZpndSRmZr;
      string JWuqdlELhq;
      string gpdcfWioTc;
      string gLEfSHTOqR;
      string qXYYIyWYKW;
      string DOgkVuwNIG;
      if(txzcQqMssG == uHfSArowtA){icTxLxRPYW = true;}
      else if(uHfSArowtA == txzcQqMssG){cKizHmAhaN = true;}
      if(pdHwdiYVfy == pyeckHbgiE){RpTPAJREUg = true;}
      else if(pyeckHbgiE == pdHwdiYVfy){OFfMVEQIHq = true;}
      if(mGmSuQcDiO == hJDUjJKyRR){AKewYReSJW = true;}
      else if(hJDUjJKyRR == mGmSuQcDiO){yGGXUWmWRS = true;}
      if(akiOHbcxLN == USPoJIZPcD){LGFjhOEaNK = true;}
      else if(USPoJIZPcD == akiOHbcxLN){rLOxiMegoG = true;}
      if(uOJbRpAVUG == lZpndSRmZr){fGzmFNtQkM = true;}
      else if(lZpndSRmZr == uOJbRpAVUG){TeDPtKBpxn = true;}
      if(TzOyYmKfhA == JWuqdlELhq){cqrbGYxWXs = true;}
      else if(JWuqdlELhq == TzOyYmKfhA){QrqkuVDKek = true;}
      if(NJKefEuroC == gpdcfWioTc){SyGBLLkjgk = true;}
      else if(gpdcfWioTc == NJKefEuroC){zdQHUCqhdJ = true;}
      if(GaJRDYxLBE == gLEfSHTOqR){EpqJAsKqqL = true;}
      if(PGhcuaWkoT == qXYYIyWYKW){WNgxsTNdcJ = true;}
      if(RxPKAkEeuM == DOgkVuwNIG){QgCTrhAybs = true;}
      while(gLEfSHTOqR == GaJRDYxLBE){YiSAXSDqcJ = true;}
      while(qXYYIyWYKW == qXYYIyWYKW){VxYdBAUdCN = true;}
      while(DOgkVuwNIG == DOgkVuwNIG){JTCEHoOuur = true;}
      if(icTxLxRPYW == true){icTxLxRPYW = false;}
      if(RpTPAJREUg == true){RpTPAJREUg = false;}
      if(AKewYReSJW == true){AKewYReSJW = false;}
      if(LGFjhOEaNK == true){LGFjhOEaNK = false;}
      if(fGzmFNtQkM == true){fGzmFNtQkM = false;}
      if(cqrbGYxWXs == true){cqrbGYxWXs = false;}
      if(SyGBLLkjgk == true){SyGBLLkjgk = false;}
      if(EpqJAsKqqL == true){EpqJAsKqqL = false;}
      if(WNgxsTNdcJ == true){WNgxsTNdcJ = false;}
      if(QgCTrhAybs == true){QgCTrhAybs = false;}
      if(cKizHmAhaN == true){cKizHmAhaN = false;}
      if(OFfMVEQIHq == true){OFfMVEQIHq = false;}
      if(yGGXUWmWRS == true){yGGXUWmWRS = false;}
      if(rLOxiMegoG == true){rLOxiMegoG = false;}
      if(TeDPtKBpxn == true){TeDPtKBpxn = false;}
      if(QrqkuVDKek == true){QrqkuVDKek = false;}
      if(zdQHUCqhdJ == true){zdQHUCqhdJ = false;}
      if(YiSAXSDqcJ == true){YiSAXSDqcJ = false;}
      if(VxYdBAUdCN == true){VxYdBAUdCN = false;}
      if(JTCEHoOuur == true){JTCEHoOuur = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BJYORYUYIJ
{ 
  void QItGHOEIYu()
  { 
      bool KQEEhZygeM = false;
      bool UppjGlASCh = false;
      bool FORsHudXsq = false;
      bool niKQdUjzif = false;
      bool AFgXmpbZHu = false;
      bool ljILkwzKwl = false;
      bool wammXkfZFk = false;
      bool BAjVhzgsGQ = false;
      bool TDPxFEZyfh = false;
      bool VYCwjYRsAB = false;
      bool KCUVslAmdk = false;
      bool OGEfSrKphT = false;
      bool LgtWPcxmkJ = false;
      bool hnrEGRitDS = false;
      bool EmUtTPTWdr = false;
      bool NaLgpcHxGQ = false;
      bool oMBUCaNAYA = false;
      bool IQlOOXMzDz = false;
      bool JZIsuHbLNc = false;
      bool JlmmdqkArs = false;
      string KnyZcepYrS;
      string CCswdulpBA;
      string qosfxGwjYd;
      string DgmUoZcJgK;
      string rdEsGtBtCR;
      string zKqmcVtRcW;
      string hFANOVPXHE;
      string RcIqIMjzZC;
      string FChGesozqS;
      string eLgKbmdpek;
      string eclyzWCAeP;
      string kuqZReXsiV;
      string fPgnYGwxOV;
      string itUoXcQiHI;
      string qnUagjTGsx;
      string OjHJEPuxYs;
      string aEkJgqZCCn;
      string OnladrtFGo;
      string loERGylLkI;
      string gSKNglDLtp;
      if(KnyZcepYrS == eclyzWCAeP){KQEEhZygeM = true;}
      else if(eclyzWCAeP == KnyZcepYrS){KCUVslAmdk = true;}
      if(CCswdulpBA == kuqZReXsiV){UppjGlASCh = true;}
      else if(kuqZReXsiV == CCswdulpBA){OGEfSrKphT = true;}
      if(qosfxGwjYd == fPgnYGwxOV){FORsHudXsq = true;}
      else if(fPgnYGwxOV == qosfxGwjYd){LgtWPcxmkJ = true;}
      if(DgmUoZcJgK == itUoXcQiHI){niKQdUjzif = true;}
      else if(itUoXcQiHI == DgmUoZcJgK){hnrEGRitDS = true;}
      if(rdEsGtBtCR == qnUagjTGsx){AFgXmpbZHu = true;}
      else if(qnUagjTGsx == rdEsGtBtCR){EmUtTPTWdr = true;}
      if(zKqmcVtRcW == OjHJEPuxYs){ljILkwzKwl = true;}
      else if(OjHJEPuxYs == zKqmcVtRcW){NaLgpcHxGQ = true;}
      if(hFANOVPXHE == aEkJgqZCCn){wammXkfZFk = true;}
      else if(aEkJgqZCCn == hFANOVPXHE){oMBUCaNAYA = true;}
      if(RcIqIMjzZC == OnladrtFGo){BAjVhzgsGQ = true;}
      if(FChGesozqS == loERGylLkI){TDPxFEZyfh = true;}
      if(eLgKbmdpek == gSKNglDLtp){VYCwjYRsAB = true;}
      while(OnladrtFGo == RcIqIMjzZC){IQlOOXMzDz = true;}
      while(loERGylLkI == loERGylLkI){JZIsuHbLNc = true;}
      while(gSKNglDLtp == gSKNglDLtp){JlmmdqkArs = true;}
      if(KQEEhZygeM == true){KQEEhZygeM = false;}
      if(UppjGlASCh == true){UppjGlASCh = false;}
      if(FORsHudXsq == true){FORsHudXsq = false;}
      if(niKQdUjzif == true){niKQdUjzif = false;}
      if(AFgXmpbZHu == true){AFgXmpbZHu = false;}
      if(ljILkwzKwl == true){ljILkwzKwl = false;}
      if(wammXkfZFk == true){wammXkfZFk = false;}
      if(BAjVhzgsGQ == true){BAjVhzgsGQ = false;}
      if(TDPxFEZyfh == true){TDPxFEZyfh = false;}
      if(VYCwjYRsAB == true){VYCwjYRsAB = false;}
      if(KCUVslAmdk == true){KCUVslAmdk = false;}
      if(OGEfSrKphT == true){OGEfSrKphT = false;}
      if(LgtWPcxmkJ == true){LgtWPcxmkJ = false;}
      if(hnrEGRitDS == true){hnrEGRitDS = false;}
      if(EmUtTPTWdr == true){EmUtTPTWdr = false;}
      if(NaLgpcHxGQ == true){NaLgpcHxGQ = false;}
      if(oMBUCaNAYA == true){oMBUCaNAYA = false;}
      if(IQlOOXMzDz == true){IQlOOXMzDz = false;}
      if(JZIsuHbLNc == true){JZIsuHbLNc = false;}
      if(JlmmdqkArs == true){JlmmdqkArs = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PRRXROONQI
{ 
  void zgzmiDPAge()
  { 
      bool RPgIsWtLjm = false;
      bool jCJpQwjlrG = false;
      bool TwsjXamTbO = false;
      bool OSMwhEqZeJ = false;
      bool NYQpXlsXts = false;
      bool HjyzCUVwjR = false;
      bool jajVXRoNHM = false;
      bool EJHirufCMk = false;
      bool hOZhHdUNtN = false;
      bool hupMYmntSF = false;
      bool xePgoxcOGG = false;
      bool oTjyjUBCOs = false;
      bool wbStdISuJF = false;
      bool GkjGnCyrrJ = false;
      bool BFGmXOLrcE = false;
      bool klfYIUMVCg = false;
      bool dgFBzEjcJg = false;
      bool VJJErqUfWP = false;
      bool xaRETTEKxS = false;
      bool pcoLeTAEnT = false;
      string NZJYajGPOu;
      string iZBVDtCmgk;
      string BbrqHhSlzL;
      string BbHwgjcknA;
      string ASIxykNNZJ;
      string iVmOelgRQs;
      string QTQDtHREoS;
      string RRzfkiStsV;
      string TFMobensQt;
      string CKTtjfBFDq;
      string TJXGrRWGdw;
      string SmYuYLHbbO;
      string SYmuPGxEwE;
      string hNpBCKgFxJ;
      string IgPUbPlYVX;
      string YYcoXtVcqk;
      string TRKYOExWrN;
      string eBeRFkJSSA;
      string TXhXfIjAcI;
      string oDOLoqWpFW;
      if(NZJYajGPOu == TJXGrRWGdw){RPgIsWtLjm = true;}
      else if(TJXGrRWGdw == NZJYajGPOu){xePgoxcOGG = true;}
      if(iZBVDtCmgk == SmYuYLHbbO){jCJpQwjlrG = true;}
      else if(SmYuYLHbbO == iZBVDtCmgk){oTjyjUBCOs = true;}
      if(BbrqHhSlzL == SYmuPGxEwE){TwsjXamTbO = true;}
      else if(SYmuPGxEwE == BbrqHhSlzL){wbStdISuJF = true;}
      if(BbHwgjcknA == hNpBCKgFxJ){OSMwhEqZeJ = true;}
      else if(hNpBCKgFxJ == BbHwgjcknA){GkjGnCyrrJ = true;}
      if(ASIxykNNZJ == IgPUbPlYVX){NYQpXlsXts = true;}
      else if(IgPUbPlYVX == ASIxykNNZJ){BFGmXOLrcE = true;}
      if(iVmOelgRQs == YYcoXtVcqk){HjyzCUVwjR = true;}
      else if(YYcoXtVcqk == iVmOelgRQs){klfYIUMVCg = true;}
      if(QTQDtHREoS == TRKYOExWrN){jajVXRoNHM = true;}
      else if(TRKYOExWrN == QTQDtHREoS){dgFBzEjcJg = true;}
      if(RRzfkiStsV == eBeRFkJSSA){EJHirufCMk = true;}
      if(TFMobensQt == TXhXfIjAcI){hOZhHdUNtN = true;}
      if(CKTtjfBFDq == oDOLoqWpFW){hupMYmntSF = true;}
      while(eBeRFkJSSA == RRzfkiStsV){VJJErqUfWP = true;}
      while(TXhXfIjAcI == TXhXfIjAcI){xaRETTEKxS = true;}
      while(oDOLoqWpFW == oDOLoqWpFW){pcoLeTAEnT = true;}
      if(RPgIsWtLjm == true){RPgIsWtLjm = false;}
      if(jCJpQwjlrG == true){jCJpQwjlrG = false;}
      if(TwsjXamTbO == true){TwsjXamTbO = false;}
      if(OSMwhEqZeJ == true){OSMwhEqZeJ = false;}
      if(NYQpXlsXts == true){NYQpXlsXts = false;}
      if(HjyzCUVwjR == true){HjyzCUVwjR = false;}
      if(jajVXRoNHM == true){jajVXRoNHM = false;}
      if(EJHirufCMk == true){EJHirufCMk = false;}
      if(hOZhHdUNtN == true){hOZhHdUNtN = false;}
      if(hupMYmntSF == true){hupMYmntSF = false;}
      if(xePgoxcOGG == true){xePgoxcOGG = false;}
      if(oTjyjUBCOs == true){oTjyjUBCOs = false;}
      if(wbStdISuJF == true){wbStdISuJF = false;}
      if(GkjGnCyrrJ == true){GkjGnCyrrJ = false;}
      if(BFGmXOLrcE == true){BFGmXOLrcE = false;}
      if(klfYIUMVCg == true){klfYIUMVCg = false;}
      if(dgFBzEjcJg == true){dgFBzEjcJg = false;}
      if(VJJErqUfWP == true){VJJErqUfWP = false;}
      if(xaRETTEKxS == true){xaRETTEKxS = false;}
      if(pcoLeTAEnT == true){pcoLeTAEnT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class XLYPTQRVBO
{ 
  void OzxABDBHnX()
  { 
      bool VBFdqYGinA = false;
      bool CTQICXwnGN = false;
      bool almesIegoQ = false;
      bool uWRqVzwofD = false;
      bool BCoIeFMpkt = false;
      bool IWtZXjNaki = false;
      bool sZtXEfBYmD = false;
      bool bHIDMYxeAn = false;
      bool wowtHYbrTk = false;
      bool kUEwJaHQDz = false;
      bool DmChSBOqNy = false;
      bool miZwlEdzPf = false;
      bool DJEBRGCLtk = false;
      bool QfaBkIlNaG = false;
      bool JkeVyCuHJE = false;
      bool xHAXusiAPM = false;
      bool yCxbgGHYLd = false;
      bool RPYJVSeiDP = false;
      bool KtBomIeCGq = false;
      bool YlxOHHnCnq = false;
      string zpKgRUaKYK;
      string JVbHEbzBSs;
      string VsryYKEVDm;
      string PwSwFmUsVQ;
      string iwbzykVKtJ;
      string AMNWfFxMHG;
      string QEGluMDfBR;
      string HFfBJZRsAn;
      string osbtlCiwPz;
      string oIkOAeopDk;
      string goIcCtgLMG;
      string nELSRWcMzr;
      string dHPBbyUQAd;
      string jboPGzjyqr;
      string VWiJLkxRxd;
      string CbpLHZZHAD;
      string ZPLhVMPuLS;
      string ldUHWHXFpq;
      string BCTWbIkdIg;
      string YcynRFLBny;
      if(zpKgRUaKYK == goIcCtgLMG){VBFdqYGinA = true;}
      else if(goIcCtgLMG == zpKgRUaKYK){DmChSBOqNy = true;}
      if(JVbHEbzBSs == nELSRWcMzr){CTQICXwnGN = true;}
      else if(nELSRWcMzr == JVbHEbzBSs){miZwlEdzPf = true;}
      if(VsryYKEVDm == dHPBbyUQAd){almesIegoQ = true;}
      else if(dHPBbyUQAd == VsryYKEVDm){DJEBRGCLtk = true;}
      if(PwSwFmUsVQ == jboPGzjyqr){uWRqVzwofD = true;}
      else if(jboPGzjyqr == PwSwFmUsVQ){QfaBkIlNaG = true;}
      if(iwbzykVKtJ == VWiJLkxRxd){BCoIeFMpkt = true;}
      else if(VWiJLkxRxd == iwbzykVKtJ){JkeVyCuHJE = true;}
      if(AMNWfFxMHG == CbpLHZZHAD){IWtZXjNaki = true;}
      else if(CbpLHZZHAD == AMNWfFxMHG){xHAXusiAPM = true;}
      if(QEGluMDfBR == ZPLhVMPuLS){sZtXEfBYmD = true;}
      else if(ZPLhVMPuLS == QEGluMDfBR){yCxbgGHYLd = true;}
      if(HFfBJZRsAn == ldUHWHXFpq){bHIDMYxeAn = true;}
      if(osbtlCiwPz == BCTWbIkdIg){wowtHYbrTk = true;}
      if(oIkOAeopDk == YcynRFLBny){kUEwJaHQDz = true;}
      while(ldUHWHXFpq == HFfBJZRsAn){RPYJVSeiDP = true;}
      while(BCTWbIkdIg == BCTWbIkdIg){KtBomIeCGq = true;}
      while(YcynRFLBny == YcynRFLBny){YlxOHHnCnq = true;}
      if(VBFdqYGinA == true){VBFdqYGinA = false;}
      if(CTQICXwnGN == true){CTQICXwnGN = false;}
      if(almesIegoQ == true){almesIegoQ = false;}
      if(uWRqVzwofD == true){uWRqVzwofD = false;}
      if(BCoIeFMpkt == true){BCoIeFMpkt = false;}
      if(IWtZXjNaki == true){IWtZXjNaki = false;}
      if(sZtXEfBYmD == true){sZtXEfBYmD = false;}
      if(bHIDMYxeAn == true){bHIDMYxeAn = false;}
      if(wowtHYbrTk == true){wowtHYbrTk = false;}
      if(kUEwJaHQDz == true){kUEwJaHQDz = false;}
      if(DmChSBOqNy == true){DmChSBOqNy = false;}
      if(miZwlEdzPf == true){miZwlEdzPf = false;}
      if(DJEBRGCLtk == true){DJEBRGCLtk = false;}
      if(QfaBkIlNaG == true){QfaBkIlNaG = false;}
      if(JkeVyCuHJE == true){JkeVyCuHJE = false;}
      if(xHAXusiAPM == true){xHAXusiAPM = false;}
      if(yCxbgGHYLd == true){yCxbgGHYLd = false;}
      if(RPYJVSeiDP == true){RPYJVSeiDP = false;}
      if(KtBomIeCGq == true){KtBomIeCGq = false;}
      if(YlxOHHnCnq == true){YlxOHHnCnq = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class BPBWUVLHYG
{ 
  void fOlQXIcoeP()
  { 
      bool oArKXycbam = false;
      bool DjJNwfVxrb = false;
      bool kNPcSdYxRP = false;
      bool qMWOiadkfb = false;
      bool ZFQOUsjcno = false;
      bool ARIYBakOrs = false;
      bool ZeJKuqMudU = false;
      bool BjRPCfxAHZ = false;
      bool tuJcPYDyMI = false;
      bool pOslCtuFTd = false;
      bool gnzSnPAsWV = false;
      bool djGRzeVguW = false;
      bool mNhVdlqeNH = false;
      bool jqgdsFupoA = false;
      bool XJapdFIamX = false;
      bool XOKwpSFcoB = false;
      bool XVeOklDcYs = false;
      bool ifruyclqqX = false;
      bool dPrpjVgjma = false;
      bool zSAmbCeMEr = false;
      string hVngQVpdGH;
      string dsBkjZRKzd;
      string ZuRPBlUOOH;
      string WpgZIJCQSa;
      string hNYYELnwfo;
      string SMYEClDXfe;
      string BaOyknLacZ;
      string XAqYuLWsLo;
      string yXiZnJQcWV;
      string FdknYGzZnQ;
      string zMSFRRVAIq;
      string PKMqgcroKa;
      string WiEHTsPDzo;
      string MxEuokFcHW;
      string nQUfFuRsGu;
      string NJPNsQILlX;
      string BRASeSFQBu;
      string XPgmlGNYBY;
      string bVwDaCgqBX;
      string csqjUVPBBm;
      if(hVngQVpdGH == zMSFRRVAIq){oArKXycbam = true;}
      else if(zMSFRRVAIq == hVngQVpdGH){gnzSnPAsWV = true;}
      if(dsBkjZRKzd == PKMqgcroKa){DjJNwfVxrb = true;}
      else if(PKMqgcroKa == dsBkjZRKzd){djGRzeVguW = true;}
      if(ZuRPBlUOOH == WiEHTsPDzo){kNPcSdYxRP = true;}
      else if(WiEHTsPDzo == ZuRPBlUOOH){mNhVdlqeNH = true;}
      if(WpgZIJCQSa == MxEuokFcHW){qMWOiadkfb = true;}
      else if(MxEuokFcHW == WpgZIJCQSa){jqgdsFupoA = true;}
      if(hNYYELnwfo == nQUfFuRsGu){ZFQOUsjcno = true;}
      else if(nQUfFuRsGu == hNYYELnwfo){XJapdFIamX = true;}
      if(SMYEClDXfe == NJPNsQILlX){ARIYBakOrs = true;}
      else if(NJPNsQILlX == SMYEClDXfe){XOKwpSFcoB = true;}
      if(BaOyknLacZ == BRASeSFQBu){ZeJKuqMudU = true;}
      else if(BRASeSFQBu == BaOyknLacZ){XVeOklDcYs = true;}
      if(XAqYuLWsLo == XPgmlGNYBY){BjRPCfxAHZ = true;}
      if(yXiZnJQcWV == bVwDaCgqBX){tuJcPYDyMI = true;}
      if(FdknYGzZnQ == csqjUVPBBm){pOslCtuFTd = true;}
      while(XPgmlGNYBY == XAqYuLWsLo){ifruyclqqX = true;}
      while(bVwDaCgqBX == bVwDaCgqBX){dPrpjVgjma = true;}
      while(csqjUVPBBm == csqjUVPBBm){zSAmbCeMEr = true;}
      if(oArKXycbam == true){oArKXycbam = false;}
      if(DjJNwfVxrb == true){DjJNwfVxrb = false;}
      if(kNPcSdYxRP == true){kNPcSdYxRP = false;}
      if(qMWOiadkfb == true){qMWOiadkfb = false;}
      if(ZFQOUsjcno == true){ZFQOUsjcno = false;}
      if(ARIYBakOrs == true){ARIYBakOrs = false;}
      if(ZeJKuqMudU == true){ZeJKuqMudU = false;}
      if(BjRPCfxAHZ == true){BjRPCfxAHZ = false;}
      if(tuJcPYDyMI == true){tuJcPYDyMI = false;}
      if(pOslCtuFTd == true){pOslCtuFTd = false;}
      if(gnzSnPAsWV == true){gnzSnPAsWV = false;}
      if(djGRzeVguW == true){djGRzeVguW = false;}
      if(mNhVdlqeNH == true){mNhVdlqeNH = false;}
      if(jqgdsFupoA == true){jqgdsFupoA = false;}
      if(XJapdFIamX == true){XJapdFIamX = false;}
      if(XOKwpSFcoB == true){XOKwpSFcoB = false;}
      if(XVeOklDcYs == true){XVeOklDcYs = false;}
      if(ifruyclqqX == true){ifruyclqqX = false;}
      if(dPrpjVgjma == true){dPrpjVgjma = false;}
      if(zSAmbCeMEr == true){zSAmbCeMEr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class VHQGEAQYHT
{ 
  void SZqaKUDdIO()
  { 
      bool XnRSLAzhum = false;
      bool xFBnIJoydZ = false;
      bool OBMMQSlbUY = false;
      bool wulQfsXrzt = false;
      bool GbGZuSMQIB = false;
      bool qTeSdWaFBr = false;
      bool NWMVqCTjIP = false;
      bool kwziTmulQx = false;
      bool GVSThcPSlJ = false;
      bool RSpGfiOjis = false;
      bool zbHsPtTwzq = false;
      bool aonDlrDsmO = false;
      bool mAyzxSGrCk = false;
      bool YKCXyeNZDn = false;
      bool ITTuhcelUm = false;
      bool eVxUGzcIux = false;
      bool nbsORPlPFH = false;
      bool edozFTfDQU = false;
      bool mTXPFDNhRz = false;
      bool BFdGzfZFcS = false;
      string nGKdEQUVxj;
      string gDsommpoVO;
      string nJMUxCwHZc;
      string xDYhuaqELS;
      string trbYRQBikg;
      string BxPfcespfB;
      string TMuUlcdKtM;
      string wlcCUwgnCf;
      string BNWsaAnrFQ;
      string gBXJyhwHlo;
      string lpnYkOTMLf;
      string xFmzAGpCsl;
      string nkFcftOTDm;
      string AYBNtKYyHL;
      string fagZSbNXIr;
      string rtOAABkLrp;
      string AAJXfYDZFp;
      string gqtgFNDfKE;
      string gpKKtFJBMs;
      string YNFRLfRIss;
      if(nGKdEQUVxj == lpnYkOTMLf){XnRSLAzhum = true;}
      else if(lpnYkOTMLf == nGKdEQUVxj){zbHsPtTwzq = true;}
      if(gDsommpoVO == xFmzAGpCsl){xFBnIJoydZ = true;}
      else if(xFmzAGpCsl == gDsommpoVO){aonDlrDsmO = true;}
      if(nJMUxCwHZc == nkFcftOTDm){OBMMQSlbUY = true;}
      else if(nkFcftOTDm == nJMUxCwHZc){mAyzxSGrCk = true;}
      if(xDYhuaqELS == AYBNtKYyHL){wulQfsXrzt = true;}
      else if(AYBNtKYyHL == xDYhuaqELS){YKCXyeNZDn = true;}
      if(trbYRQBikg == fagZSbNXIr){GbGZuSMQIB = true;}
      else if(fagZSbNXIr == trbYRQBikg){ITTuhcelUm = true;}
      if(BxPfcespfB == rtOAABkLrp){qTeSdWaFBr = true;}
      else if(rtOAABkLrp == BxPfcespfB){eVxUGzcIux = true;}
      if(TMuUlcdKtM == AAJXfYDZFp){NWMVqCTjIP = true;}
      else if(AAJXfYDZFp == TMuUlcdKtM){nbsORPlPFH = true;}
      if(wlcCUwgnCf == gqtgFNDfKE){kwziTmulQx = true;}
      if(BNWsaAnrFQ == gpKKtFJBMs){GVSThcPSlJ = true;}
      if(gBXJyhwHlo == YNFRLfRIss){RSpGfiOjis = true;}
      while(gqtgFNDfKE == wlcCUwgnCf){edozFTfDQU = true;}
      while(gpKKtFJBMs == gpKKtFJBMs){mTXPFDNhRz = true;}
      while(YNFRLfRIss == YNFRLfRIss){BFdGzfZFcS = true;}
      if(XnRSLAzhum == true){XnRSLAzhum = false;}
      if(xFBnIJoydZ == true){xFBnIJoydZ = false;}
      if(OBMMQSlbUY == true){OBMMQSlbUY = false;}
      if(wulQfsXrzt == true){wulQfsXrzt = false;}
      if(GbGZuSMQIB == true){GbGZuSMQIB = false;}
      if(qTeSdWaFBr == true){qTeSdWaFBr = false;}
      if(NWMVqCTjIP == true){NWMVqCTjIP = false;}
      if(kwziTmulQx == true){kwziTmulQx = false;}
      if(GVSThcPSlJ == true){GVSThcPSlJ = false;}
      if(RSpGfiOjis == true){RSpGfiOjis = false;}
      if(zbHsPtTwzq == true){zbHsPtTwzq = false;}
      if(aonDlrDsmO == true){aonDlrDsmO = false;}
      if(mAyzxSGrCk == true){mAyzxSGrCk = false;}
      if(YKCXyeNZDn == true){YKCXyeNZDn = false;}
      if(ITTuhcelUm == true){ITTuhcelUm = false;}
      if(eVxUGzcIux == true){eVxUGzcIux = false;}
      if(nbsORPlPFH == true){nbsORPlPFH = false;}
      if(edozFTfDQU == true){edozFTfDQU = false;}
      if(mTXPFDNhRz == true){mTXPFDNhRz = false;}
      if(BFdGzfZFcS == true){BFdGzfZFcS = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DZPFUHOWGR
{ 
  void XVlLFZssPe()
  { 
      bool sykcrZLAti = false;
      bool xWBKapfOuZ = false;
      bool VDDpnidVCg = false;
      bool UliMrOWyJe = false;
      bool XGsYeODNDQ = false;
      bool KiWtEptunI = false;
      bool woxMOLnnRL = false;
      bool ThEQCOKdKZ = false;
      bool fPRlVlOnIC = false;
      bool TbJAihJfKS = false;
      bool ZPwrxcdGro = false;
      bool fPjOcgWKpc = false;
      bool wzfPrqYEgT = false;
      bool EdTqzQSZos = false;
      bool HIwSVDqtXC = false;
      bool LyohWaifAF = false;
      bool EZYXkBwsPM = false;
      bool NxVxUmCNsn = false;
      bool zdfeIFolUX = false;
      bool glZiohjYhT = false;
      string lfwtBuWyRu;
      string HkcZYKLUPt;
      string EzoburqJzk;
      string oLONkWDzzS;
      string MdGtgsWSVU;
      string VGwOempTzI;
      string EMyfFwVOnC;
      string HLUwgartuB;
      string UXnePiOzhy;
      string fiAqNAeaoj;
      string XIeSMJkzgE;
      string twXxmgXSQW;
      string hVgpVYLdig;
      string uRFLdMsVSF;
      string yOSdDjLDDf;
      string bLKSCBanwn;
      string yGQAIcpMSZ;
      string nCAKXSDDml;
      string NHaCEiHmIc;
      string lEYeHqxRUZ;
      if(lfwtBuWyRu == XIeSMJkzgE){sykcrZLAti = true;}
      else if(XIeSMJkzgE == lfwtBuWyRu){ZPwrxcdGro = true;}
      if(HkcZYKLUPt == twXxmgXSQW){xWBKapfOuZ = true;}
      else if(twXxmgXSQW == HkcZYKLUPt){fPjOcgWKpc = true;}
      if(EzoburqJzk == hVgpVYLdig){VDDpnidVCg = true;}
      else if(hVgpVYLdig == EzoburqJzk){wzfPrqYEgT = true;}
      if(oLONkWDzzS == uRFLdMsVSF){UliMrOWyJe = true;}
      else if(uRFLdMsVSF == oLONkWDzzS){EdTqzQSZos = true;}
      if(MdGtgsWSVU == yOSdDjLDDf){XGsYeODNDQ = true;}
      else if(yOSdDjLDDf == MdGtgsWSVU){HIwSVDqtXC = true;}
      if(VGwOempTzI == bLKSCBanwn){KiWtEptunI = true;}
      else if(bLKSCBanwn == VGwOempTzI){LyohWaifAF = true;}
      if(EMyfFwVOnC == yGQAIcpMSZ){woxMOLnnRL = true;}
      else if(yGQAIcpMSZ == EMyfFwVOnC){EZYXkBwsPM = true;}
      if(HLUwgartuB == nCAKXSDDml){ThEQCOKdKZ = true;}
      if(UXnePiOzhy == NHaCEiHmIc){fPRlVlOnIC = true;}
      if(fiAqNAeaoj == lEYeHqxRUZ){TbJAihJfKS = true;}
      while(nCAKXSDDml == HLUwgartuB){NxVxUmCNsn = true;}
      while(NHaCEiHmIc == NHaCEiHmIc){zdfeIFolUX = true;}
      while(lEYeHqxRUZ == lEYeHqxRUZ){glZiohjYhT = true;}
      if(sykcrZLAti == true){sykcrZLAti = false;}
      if(xWBKapfOuZ == true){xWBKapfOuZ = false;}
      if(VDDpnidVCg == true){VDDpnidVCg = false;}
      if(UliMrOWyJe == true){UliMrOWyJe = false;}
      if(XGsYeODNDQ == true){XGsYeODNDQ = false;}
      if(KiWtEptunI == true){KiWtEptunI = false;}
      if(woxMOLnnRL == true){woxMOLnnRL = false;}
      if(ThEQCOKdKZ == true){ThEQCOKdKZ = false;}
      if(fPRlVlOnIC == true){fPRlVlOnIC = false;}
      if(TbJAihJfKS == true){TbJAihJfKS = false;}
      if(ZPwrxcdGro == true){ZPwrxcdGro = false;}
      if(fPjOcgWKpc == true){fPjOcgWKpc = false;}
      if(wzfPrqYEgT == true){wzfPrqYEgT = false;}
      if(EdTqzQSZos == true){EdTqzQSZos = false;}
      if(HIwSVDqtXC == true){HIwSVDqtXC = false;}
      if(LyohWaifAF == true){LyohWaifAF = false;}
      if(EZYXkBwsPM == true){EZYXkBwsPM = false;}
      if(NxVxUmCNsn == true){NxVxUmCNsn = false;}
      if(zdfeIFolUX == true){zdfeIFolUX = false;}
      if(glZiohjYhT == true){glZiohjYhT = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class OONWKTBBIG
{ 
  void NhFlVPlNup()
  { 
      bool UmnaESGCPk = false;
      bool kfSrEuNdlq = false;
      bool zLJopZuDAS = false;
      bool SqlSrpknUo = false;
      bool fTfsgTEjCe = false;
      bool qwPyNJdyjd = false;
      bool WDCwSttAAN = false;
      bool KZIwLfeQIW = false;
      bool VKBUaWbmEi = false;
      bool MlDqRcRjam = false;
      bool YDjnURmSlh = false;
      bool dMpMIMBgUw = false;
      bool RGwQAXVRei = false;
      bool NuoVxSaMAJ = false;
      bool iJBWEzwphF = false;
      bool ajOeUSHSmw = false;
      bool uVmDTiXTPp = false;
      bool kngLMHFZMB = false;
      bool aeiZHdgKAd = false;
      bool KIZDOMVxCU = false;
      string oiEYScPKGg;
      string VtwCxGlLZT;
      string DPPuGLWhmw;
      string mnUzfneRTb;
      string blMmjrAkFM;
      string JydiyZTISG;
      string FhXJthZxWJ;
      string aTqyqQCbLO;
      string KkcZlnJlBG;
      string TWzMVtOCsC;
      string RDOulpGfqg;
      string WqwDwTEyXF;
      string XnwPpDUebx;
      string CAiIwGWgnw;
      string gduTjcauPC;
      string oQdmedHzFh;
      string UbDsdVHmHo;
      string xWJbXsrbfM;
      string cYOZHlVwdR;
      string zWbGaFZBpN;
      if(oiEYScPKGg == RDOulpGfqg){UmnaESGCPk = true;}
      else if(RDOulpGfqg == oiEYScPKGg){YDjnURmSlh = true;}
      if(VtwCxGlLZT == WqwDwTEyXF){kfSrEuNdlq = true;}
      else if(WqwDwTEyXF == VtwCxGlLZT){dMpMIMBgUw = true;}
      if(DPPuGLWhmw == XnwPpDUebx){zLJopZuDAS = true;}
      else if(XnwPpDUebx == DPPuGLWhmw){RGwQAXVRei = true;}
      if(mnUzfneRTb == CAiIwGWgnw){SqlSrpknUo = true;}
      else if(CAiIwGWgnw == mnUzfneRTb){NuoVxSaMAJ = true;}
      if(blMmjrAkFM == gduTjcauPC){fTfsgTEjCe = true;}
      else if(gduTjcauPC == blMmjrAkFM){iJBWEzwphF = true;}
      if(JydiyZTISG == oQdmedHzFh){qwPyNJdyjd = true;}
      else if(oQdmedHzFh == JydiyZTISG){ajOeUSHSmw = true;}
      if(FhXJthZxWJ == UbDsdVHmHo){WDCwSttAAN = true;}
      else if(UbDsdVHmHo == FhXJthZxWJ){uVmDTiXTPp = true;}
      if(aTqyqQCbLO == xWJbXsrbfM){KZIwLfeQIW = true;}
      if(KkcZlnJlBG == cYOZHlVwdR){VKBUaWbmEi = true;}
      if(TWzMVtOCsC == zWbGaFZBpN){MlDqRcRjam = true;}
      while(xWJbXsrbfM == aTqyqQCbLO){kngLMHFZMB = true;}
      while(cYOZHlVwdR == cYOZHlVwdR){aeiZHdgKAd = true;}
      while(zWbGaFZBpN == zWbGaFZBpN){KIZDOMVxCU = true;}
      if(UmnaESGCPk == true){UmnaESGCPk = false;}
      if(kfSrEuNdlq == true){kfSrEuNdlq = false;}
      if(zLJopZuDAS == true){zLJopZuDAS = false;}
      if(SqlSrpknUo == true){SqlSrpknUo = false;}
      if(fTfsgTEjCe == true){fTfsgTEjCe = false;}
      if(qwPyNJdyjd == true){qwPyNJdyjd = false;}
      if(WDCwSttAAN == true){WDCwSttAAN = false;}
      if(KZIwLfeQIW == true){KZIwLfeQIW = false;}
      if(VKBUaWbmEi == true){VKBUaWbmEi = false;}
      if(MlDqRcRjam == true){MlDqRcRjam = false;}
      if(YDjnURmSlh == true){YDjnURmSlh = false;}
      if(dMpMIMBgUw == true){dMpMIMBgUw = false;}
      if(RGwQAXVRei == true){RGwQAXVRei = false;}
      if(NuoVxSaMAJ == true){NuoVxSaMAJ = false;}
      if(iJBWEzwphF == true){iJBWEzwphF = false;}
      if(ajOeUSHSmw == true){ajOeUSHSmw = false;}
      if(uVmDTiXTPp == true){uVmDTiXTPp = false;}
      if(kngLMHFZMB == true){kngLMHFZMB = false;}
      if(aeiZHdgKAd == true){aeiZHdgKAd = false;}
      if(KIZDOMVxCU == true){KIZDOMVxCU = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CZMKWBKVNR
{ 
  void iasSGWyrhR()
  { 
      bool YLTkMItDfi = false;
      bool AaIwBFreme = false;
      bool cqOpPkNfPs = false;
      bool HbtszCkXyD = false;
      bool EkpsYDEOZE = false;
      bool jYWwbhxcVa = false;
      bool uOmlaXfPwy = false;
      bool UUgjZqAVQM = false;
      bool fyFLJiFyQR = false;
      bool FIBWHoxESB = false;
      bool WoSajPrlVj = false;
      bool gbMIuxIbcq = false;
      bool ShyesMSJbj = false;
      bool tuCPopfdbs = false;
      bool qSbgjduraK = false;
      bool gVaahoqmhW = false;
      bool xhJlpkYwTF = false;
      bool QdAjzzFQtP = false;
      bool QZiRDXUbgA = false;
      bool yrwEwJIpqy = false;
      string BJPQAIuJmO;
      string YkaFZfuIon;
      string bcufmgwdkX;
      string wPhQenzrTp;
      string FYmTJnhBQN;
      string CdKcgYOicT;
      string MRXPSJCKak;
      string fDnzhCoaVz;
      string ytMXUSEFiG;
      string ZPOAcnBXBz;
      string kXafgzoLte;
      string imMdURFztz;
      string FxaUKQonVt;
      string BRYWNaTfqU;
      string csKEWmCjeK;
      string hCcCtmxGUP;
      string KAoAcRNpQs;
      string yYHZVLmVxX;
      string OEIKMynWbZ;
      string dNLbJbDKaE;
      if(BJPQAIuJmO == kXafgzoLte){YLTkMItDfi = true;}
      else if(kXafgzoLte == BJPQAIuJmO){WoSajPrlVj = true;}
      if(YkaFZfuIon == imMdURFztz){AaIwBFreme = true;}
      else if(imMdURFztz == YkaFZfuIon){gbMIuxIbcq = true;}
      if(bcufmgwdkX == FxaUKQonVt){cqOpPkNfPs = true;}
      else if(FxaUKQonVt == bcufmgwdkX){ShyesMSJbj = true;}
      if(wPhQenzrTp == BRYWNaTfqU){HbtszCkXyD = true;}
      else if(BRYWNaTfqU == wPhQenzrTp){tuCPopfdbs = true;}
      if(FYmTJnhBQN == csKEWmCjeK){EkpsYDEOZE = true;}
      else if(csKEWmCjeK == FYmTJnhBQN){qSbgjduraK = true;}
      if(CdKcgYOicT == hCcCtmxGUP){jYWwbhxcVa = true;}
      else if(hCcCtmxGUP == CdKcgYOicT){gVaahoqmhW = true;}
      if(MRXPSJCKak == KAoAcRNpQs){uOmlaXfPwy = true;}
      else if(KAoAcRNpQs == MRXPSJCKak){xhJlpkYwTF = true;}
      if(fDnzhCoaVz == yYHZVLmVxX){UUgjZqAVQM = true;}
      if(ytMXUSEFiG == OEIKMynWbZ){fyFLJiFyQR = true;}
      if(ZPOAcnBXBz == dNLbJbDKaE){FIBWHoxESB = true;}
      while(yYHZVLmVxX == fDnzhCoaVz){QdAjzzFQtP = true;}
      while(OEIKMynWbZ == OEIKMynWbZ){QZiRDXUbgA = true;}
      while(dNLbJbDKaE == dNLbJbDKaE){yrwEwJIpqy = true;}
      if(YLTkMItDfi == true){YLTkMItDfi = false;}
      if(AaIwBFreme == true){AaIwBFreme = false;}
      if(cqOpPkNfPs == true){cqOpPkNfPs = false;}
      if(HbtszCkXyD == true){HbtszCkXyD = false;}
      if(EkpsYDEOZE == true){EkpsYDEOZE = false;}
      if(jYWwbhxcVa == true){jYWwbhxcVa = false;}
      if(uOmlaXfPwy == true){uOmlaXfPwy = false;}
      if(UUgjZqAVQM == true){UUgjZqAVQM = false;}
      if(fyFLJiFyQR == true){fyFLJiFyQR = false;}
      if(FIBWHoxESB == true){FIBWHoxESB = false;}
      if(WoSajPrlVj == true){WoSajPrlVj = false;}
      if(gbMIuxIbcq == true){gbMIuxIbcq = false;}
      if(ShyesMSJbj == true){ShyesMSJbj = false;}
      if(tuCPopfdbs == true){tuCPopfdbs = false;}
      if(qSbgjduraK == true){qSbgjduraK = false;}
      if(gVaahoqmhW == true){gVaahoqmhW = false;}
      if(xhJlpkYwTF == true){xhJlpkYwTF = false;}
      if(QdAjzzFQtP == true){QdAjzzFQtP = false;}
      if(QZiRDXUbgA == true){QZiRDXUbgA = false;}
      if(yrwEwJIpqy == true){yrwEwJIpqy = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UAESMMHNRZ
{ 
  void qzUgyJMtmc()
  { 
      bool iRFUUYOANR = false;
      bool elQmbaUHel = false;
      bool sHhRYqsyuo = false;
      bool jaUQSYdfzV = false;
      bool TZqpesBNLT = false;
      bool mFBIEbjmwc = false;
      bool fPFYhDQzTC = false;
      bool xnHIRTQrmf = false;
      bool oelaBdYMLq = false;
      bool opjWoDTRIa = false;
      bool bCsJWzxrpk = false;
      bool KrAPmAddWD = false;
      bool dDnbMGEjTO = false;
      bool zyiLDmkcEC = false;
      bool ggWZfxoTjn = false;
      bool ZouFxXNQwP = false;
      bool tMYalMIoLz = false;
      bool OBZTVxgNwP = false;
      bool QjAenJCdOf = false;
      bool ZUfnITHumT = false;
      string NYGSzVKfFH;
      string fDwdgYrzpm;
      string jWqXAxEsOc;
      string GdyxtIDWND;
      string UJQmCFguNn;
      string wgNFdqhhSC;
      string QQRcACFyiY;
      string UUllRaFDVu;
      string PJfZgYQrSD;
      string ZUELlErmmz;
      string zhLnSauamD;
      string kdMXlHkrwa;
      string lMffyAQZzN;
      string hRQVuhxqGF;
      string coFPRwIgYe;
      string SzqLNlCepS;
      string KUbYUTMpbE;
      string KtkpxwHPOC;
      string qiAUthaFHJ;
      string eSBmuHMfty;
      if(NYGSzVKfFH == zhLnSauamD){iRFUUYOANR = true;}
      else if(zhLnSauamD == NYGSzVKfFH){bCsJWzxrpk = true;}
      if(fDwdgYrzpm == kdMXlHkrwa){elQmbaUHel = true;}
      else if(kdMXlHkrwa == fDwdgYrzpm){KrAPmAddWD = true;}
      if(jWqXAxEsOc == lMffyAQZzN){sHhRYqsyuo = true;}
      else if(lMffyAQZzN == jWqXAxEsOc){dDnbMGEjTO = true;}
      if(GdyxtIDWND == hRQVuhxqGF){jaUQSYdfzV = true;}
      else if(hRQVuhxqGF == GdyxtIDWND){zyiLDmkcEC = true;}
      if(UJQmCFguNn == coFPRwIgYe){TZqpesBNLT = true;}
      else if(coFPRwIgYe == UJQmCFguNn){ggWZfxoTjn = true;}
      if(wgNFdqhhSC == SzqLNlCepS){mFBIEbjmwc = true;}
      else if(SzqLNlCepS == wgNFdqhhSC){ZouFxXNQwP = true;}
      if(QQRcACFyiY == KUbYUTMpbE){fPFYhDQzTC = true;}
      else if(KUbYUTMpbE == QQRcACFyiY){tMYalMIoLz = true;}
      if(UUllRaFDVu == KtkpxwHPOC){xnHIRTQrmf = true;}
      if(PJfZgYQrSD == qiAUthaFHJ){oelaBdYMLq = true;}
      if(ZUELlErmmz == eSBmuHMfty){opjWoDTRIa = true;}
      while(KtkpxwHPOC == UUllRaFDVu){OBZTVxgNwP = true;}
      while(qiAUthaFHJ == qiAUthaFHJ){QjAenJCdOf = true;}
      while(eSBmuHMfty == eSBmuHMfty){ZUfnITHumT = true;}
      if(iRFUUYOANR == true){iRFUUYOANR = false;}
      if(elQmbaUHel == true){elQmbaUHel = false;}
      if(sHhRYqsyuo == true){sHhRYqsyuo = false;}
      if(jaUQSYdfzV == true){jaUQSYdfzV = false;}
      if(TZqpesBNLT == true){TZqpesBNLT = false;}
      if(mFBIEbjmwc == true){mFBIEbjmwc = false;}
      if(fPFYhDQzTC == true){fPFYhDQzTC = false;}
      if(xnHIRTQrmf == true){xnHIRTQrmf = false;}
      if(oelaBdYMLq == true){oelaBdYMLq = false;}
      if(opjWoDTRIa == true){opjWoDTRIa = false;}
      if(bCsJWzxrpk == true){bCsJWzxrpk = false;}
      if(KrAPmAddWD == true){KrAPmAddWD = false;}
      if(dDnbMGEjTO == true){dDnbMGEjTO = false;}
      if(zyiLDmkcEC == true){zyiLDmkcEC = false;}
      if(ggWZfxoTjn == true){ggWZfxoTjn = false;}
      if(ZouFxXNQwP == true){ZouFxXNQwP = false;}
      if(tMYalMIoLz == true){tMYalMIoLz = false;}
      if(OBZTVxgNwP == true){OBZTVxgNwP = false;}
      if(QjAenJCdOf == true){QjAenJCdOf = false;}
      if(ZUfnITHumT == true){ZUfnITHumT = false;}
    } 
}; 
