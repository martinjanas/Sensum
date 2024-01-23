#include "../hooks.h"
#include "../../settings/globals.h"
#include "../../settings/options.hpp"
#include "../../imgui/imgui.h"
#include "../../render/render.h"
#include "../../helpers/console.h"
#include "../../helpers/notifies.h"
#include "../../features/features.h"

#include <intrin.h>

namespace hooks
{
	static IDirect3DVertexDeclaration9* vert_dec = nullptr;
	static IDirect3DVertexShader9* vert_shader = nullptr;
	static DWORD old_color_writeenable = 0;



	void handle(IDirect3DDevice9* device)
	{
		if (!render::is_ready())
			return;

		ImGui_ImplDX9_NewFrame();

		auto& io = ImGui::GetIO();

		io.MouseDrawCursor = render::menu::is_visible();

		globals::draw_list = ImGui::GetOverlayDrawList();

		render::menu::show();
		render::timers::show();
		notifies::handle(globals::draw_list);
		render::spectators::show();
		grenade_prediction::render(globals::draw_list);
		esp::render_helpers();
		esp::render(globals::draw_list);
		visuals::render(globals::draw_list);
		features::player_infobox();

		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	
	long __stdcall end_scene::hooked(IDirect3DDevice9* device)
	{
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_color_writeenable);
		device->GetVertexDeclaration(&vert_dec);
		device->GetVertexShader(&vert_shader);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		handle(device);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, old_color_writeenable);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
		device->SetVertexDeclaration(vert_dec);
		device->SetVertexShader(vert_shader);

		return original(device);
	}
}