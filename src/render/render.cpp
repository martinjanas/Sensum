#include "render.h"

#include "../globals.h"
#include "../settings.h"
#include "../helpers/input.h"
#include "../helpers/imdraw.h"

#include "fonts/droidsans.hpp"
#include "fonts/undefeated.hpp"

namespace render
{
	auto s_ready = false;

	void* saved_hwnd = nullptr;

	void switch_hwnd()
	{
		if (menu::is_visible())
		{
			if (!saved_hwnd)
				std::swap(saved_hwnd, interfaces::input_system->get_window());
		}
		else if (saved_hwnd)
			std::swap(saved_hwnd, interfaces::input_system->get_window());
	}

	namespace fonts
	{
		ImFont* main;
		ImFont* visuals;
		ImFont* low_size;
		ImFont* notifies;
		ImFont* spectators;
		ImFont* configs_list;
		ImFont* weapon_icon;

		ImFont* header_title;
		ImFont* header_buttons;
	}

	void apply_fonts()
	{
		ImFontConfig font_config;
		font_config.OversampleH = 1;
		font_config.OversampleV = 1;
		font_config.PixelSnapH = true;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF,
			0x0400, 0x044F,
			0,
		};

		auto io = &ImGui::GetIO();

		fonts::main = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 15.f, &font_config, ranges);
		fonts::visuals = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 12.f, &font_config, ranges); //12.f
		fonts::low_size = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 12.f, &font_config, ranges);
		fonts::notifies = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 13.f, &font_config, ranges);
		fonts::spectators = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 14.f, &font_config, ranges);
		fonts::configs_list = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 16.f, &font_config, ranges);
		fonts::weapon_icon = io->Fonts->AddFontFromMemoryCompressedTTF(undefeated_compressed_data, undefeated_compressed_size, 12.f, &font_config, ranges);

		fonts::header_title = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 24.f);
		fonts::header_buttons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 17.f, &font_config, ranges);
	}

	void initialize()
	{
		ImGui::CreateContext();
		ImGui_ImplDX9_Init(input_system::get_main_window(), interfaces::d3_device);

		s_ready = true;

		auto& style = ImGui::GetStyle();
		style.Alpha = 1.f;
		style.WindowRounding = 0.f;
		style.WindowBorderSize = 0.f;
		style.FrameRounding = 0.f;
		style.ScrollbarRounding = 0.f;
		style.ScrollbarSize = 12.f;

		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.FramePadding = ImVec2(4.f, 2.f);
		style.ItemSpacing = ImVec2(5, 5);

		//restyle
		style.FrameRounding = 2.f;
		style.WindowBorderSize = 0.f;
		style.PopupBorderSize = 0.5f;
		style.ItemSpacing = ImVec2(4.f, 6.f);
		style.ItemInnerSpacing = ImVec2(4.f, 4.f);
		style.GrabMinSize = 16.f;
		style.FramePadding = ImVec2(4.f, 1.f);
		style.GrabMinSize = 1.f;

		imdraw::apply_style(settings::visuals::night_mode);

		apply_fonts();
	}

	void destroy()
	{
		s_ready = false;

		ImGui_ImplDX9_Shutdown();
	}

	void device_lost()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void device_reset()
	{
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	bool is_ready()
	{
		return s_ready;
	}

	void tooltip(const char* text)
	{
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(text);
	}


	ImVec2 get_listbox_size(float x, float y_offset)
	{
		return ImVec2(x, (ImGui::GetCurrentWindow()->Size.y - ImGui::GetStyle().WindowPadding.y * 2) - ImGui::GetCursorPosY() - y_offset);
	}

	bool ColorEdit4(const char* label, ImVec4* v)
	{
		return ImGui::ColorEdit4(label, &v->x, false);
	}

	bool ColorEdit4(const char* label, Color* v)
	{
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, false)) {
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}

		return false;
	}

	bool ColorEdit4(const char* label, Color* v, ImGuiColorEditFlags flag)
	{
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, flag)) {
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}

		return false;
	}

	bool ColorEdit4(const char* label, ImVec4* v, ImGuiColorEditFlags flag)
	{
		return ImGui::ColorEdit4(label, &v->x, flag);
	}

	bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v);
	}

	void columns(int count)
	{
		ImGui::Columns(count, nullptr, false);
	}

	void separator(const char* label)
	{
		auto size = ImGui::CalcTextSize(label);
		auto draw_list = ImGui::GetWindowDrawList();
		const auto width = ImGui::GetContentRegionAvailWidth();

		const auto win_pos = ImGui::GetWindowPos();
		const auto cursor = ImVec2(ImGui::GetCursorPosX() + win_pos.x, ImGui::GetCursorPosY() + win_pos.y);
		const auto pos = ImVec2(cursor.x + width / 2 - size.x / 2, cursor.y);
		const auto col = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text));
		draw_list->AddText(pos, col, label);

		draw_list->AddLine(ImVec2(cursor.x, cursor.y + size.y / 2), ImVec2(pos.x - 5.f, cursor.y + size.y / 2), col);
		draw_list->AddLine(ImVec2(pos.x + size.x + 5.f, cursor.y + size.y / 2), ImVec2(cursor.x + width, cursor.y + size.y / 2), col);

		ImGui::NewLine();
	}

	void child(const char* label, const std::function<void()>& body)
	{
		ImGui::BeginChild(label, ImVec2(0.f, 0.f), false);
		{
			separator(label);

			ImGui::PushItemWidth(-1);

			body();

			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}

	void checkbox(const char* label, bool* value)
	{
		ImGui::Checkbox(label, value);
	}

	void combo(const char* label, const std::function<void(std::string&)>& body)
	{
		static auto preview = std::map<const char*, std::string>{};
		if (ImGui::BeginCombo(label, preview[label].c_str()))
		{
			body(preview[label]);

			ImGui::EndCombo();
		}
	}

	bool selectable(const char* label, bool selected)
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_ListBoxItem));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyleColorVec4(ImGuiCol_ListBoxItemActive));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_ListBoxItemHovered));

		const auto state = ImGui::Selectable(label, selected);

		ImGui::PopStyleColor(3);

		return state;
	}
}
