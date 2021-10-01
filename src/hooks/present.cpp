#include "hooks.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../imgui/imgui.h"
#include "../render/render.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../features/features.h"

#include <intrin.h>

namespace hooks
{
	void handle(IDirect3DDevice9* device)
	{
		if (!render::is_ready())
			return;

		ImGui_ImplDX9_NewFrame();

		ImGui::GetIO().MouseDrawCursor = render::menu::is_visible();

		globals::draw_list = ImGui::GetOverlayDrawList();

		#ifdef _DEBUG
		{
			static int x, y;

			g::engine_client->GetScreenSize(x, y);

			static int xx = x / 2;
			static int yy = y / 2;

			globals::draw_list->AddText(ImVec2(xx - 100.f, yy + 250.f), utils::to_im32(Color::White), "Debug Mode detected - Expect lags and other problems!");
		}
		#endif

		render::menu::show();
		render::timers::show();
		notifies::handle(globals::draw_list);
		render::spectators::show();
		grenade_prediction::render(globals::draw_list);
		esp::render_helpers();
		esp::render(globals::draw_list);
		visuals::render(globals::draw_list);
		//features::aimlines(globals::draw_list); //Work in progress - I need to figure out how to make the lines not render through walls.

		/*static int x, y; //Work in Progress - draws player avatar

		g::engine_client->GetScreenSize(x, y);

		static int xx = x / 2;
		static int yy = y / 2;

		if (g::engine_client->IsInGame() && g::engine_client->IsConnected()) //TODO: Displays one same avatar, wont draw bot avatars
		{
			for (int i = 1; i <= g::global_vars->maxClients; i++)
			{
				auto player = c_base_player::GetPlayerByIndex(i);

				if (!player)
					continue;

				if (player == g::local_player)
					continue;

				auto info = player->GetPlayerInfo();

				auto p = utils::get_avatar_as_texture(device, CSteamID(info.steamID64));

				ImTextureID id = p;

				globals::draw_list->AddImage(id, ImVec2(xx + (25.f + i * 25.f), yy + (25.f + i * 25.f)), ImVec2(xx - (25.f - i * 25.f), yy - (25.f - i * 25.f)));
				
			}
		}*/

		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	long __stdcall end_scene::hooked(IDirect3DDevice9* device)
	{
		static uintptr_t gameoverlay_return_address = 0;
		if (!gameoverlay_return_address) {
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

			char mod[MAX_PATH];
			GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

			if (strstr(mod, "gameoverlay"))
				gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
		}

		if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress()) && settings::misc::anti_obs)
			return original(device);

		IDirect3DStateBlock9* pixel_state = NULL;
		IDirect3DVertexDeclaration9* vertDec;
		IDirect3DVertexShader9* vertShader;

		device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
		device->GetVertexDeclaration(&vertDec);
		device->GetVertexShader(&vertShader);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

		handle(device);

		pixel_state->Apply();
		pixel_state->Release();
		device->SetVertexDeclaration(vertDec);
		device->SetVertexShader(vertShader);

		return original(device);
	}
}