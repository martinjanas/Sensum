#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"
#include <memory>
#include "../sdk/helpers/CUtlBuffer.h"

static void get_dxgi(IDXGIFactory*& dxgi_factory)
{
	dxgi_factory = nullptr;

	ID3D11Device* d3d11_device = nullptr;
	if (FAILED(g::render_system->swap_chain->GetDevice(IID_PPV_ARGS(&d3d11_device))))
		return;

	IDXGIDevice* dxgi_device = nullptr;
	if (FAILED(d3d11_device->QueryInterface(IID_PPV_ARGS(&dxgi_device))))
	{
		d3d11_device->Release();

		return;
	}

	IDXGIAdapter* dxgi_adapter = nullptr;
	if (FAILED(dxgi_device->GetAdapter(&dxgi_adapter)))
	{
		dxgi_device->Release();
		d3d11_device->Release();

		return;
	}

	if (FAILED(dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory))))
	{
		dxgi_adapter->Release();
		dxgi_device->Release();
		d3d11_device->Release();

		return;
	}

	dxgi_adapter->Release();
	dxgi_device->Release();
	d3d11_device->Release();
}

static const char* material_latex_vis = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "csgo_character.vfx"
                F_BLEND_MODE = 1
                g_vColorTint = [1.0, 1.0, 1.0, 1.0]
                g_bFogEnabled = 0
                g_flMetalness = 0.000
                g_tMetalness = resource:"materials/default/default_metal_tga_8fbc2820.vtex"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_79a2e0d0.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_1b833b2a.vtex"
			} )#";

IMaterial* create_material(const char* material_vmat, const char* mat_name)
{
	CUtlBuffer buffer{ 0, 0, 0 };
	buffer.PutString(material_vmat);

	KeyValues kv;
	bool loaded = kv.LoadKV3(&buffer, mat_name);
	if (!loaded)
		return nullptr;

	IMaterial** mat_out = nullptr;
	g::mat_system->CreateMaterial(&mat_out, mat_name, kv, 0, 1);

	return *mat_out;
}

namespace hooks
{
	bool init()
	{
		IDXGIFactory* dxgi_factory = nullptr;
		get_dxgi(dxgi_factory);

		if (dxgi_factory)
		{
			dxgi = ShadowVMT(dxgi_factory);
			dxgi_factory->Release();
		}

		entity_system = ShadowVMT(g::entity_system);
		csgo_input = ShadowVMT(g::csgo_input);
		client = ShadowVMT(g::client);
		swap_chain = ShadowVMT(g::render_system->swap_chain);
		client_mode = ShadowVMT(g::client_mode_csnormal);

		entity_system.apply(on_add_entity::index, reinterpret_cast<uintptr_t*>(&on_add_entity::hooked), reinterpret_cast<void**>(&on_add_entity::original_fn));
		entity_system.apply(on_remove_entity::index, reinterpret_cast<uintptr_t*>(&on_remove_entity::hooked), reinterpret_cast<void**>(&on_remove_entity::original_fn));

		csgo_input.apply(createmove_csgoinput::index, reinterpret_cast<uintptr_t*>(&createmove_csgoinput::hooked), reinterpret_cast<void**>(&createmove_csgoinput::original_fn));
		csgo_input.apply(createmove_csgoinput21::index, reinterpret_cast<uintptr_t*>(&createmove_csgoinput21::hooked), reinterpret_cast<void**>(&createmove_csgoinput21::original_fn));

		client.apply(frame_stage_notify::index, reinterpret_cast<uintptr_t*>(&frame_stage_notify::hooked), reinterpret_cast<void**>(&frame_stage_notify::original_fn));

		swap_chain.apply(directx::present::index, reinterpret_cast<uintptr_t*>(&directx::present::hooked), reinterpret_cast<void**>(&directx::present::original_fn));

		client_mode.apply(level_init::index, reinterpret_cast<uintptr_t*>(&level_init::hooked), reinterpret_cast<void**>(&level_init::original_fn));
		client_mode.apply(level_shutdown::index, reinterpret_cast<uintptr_t*>(&level_shutdown::hooked), reinterpret_cast<void**>(&level_shutdown::original_fn));
		
		get_matrices_for_view::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::GetMatricesForView"), __FUNCTION__).as(), reinterpret_cast<void*>(get_matrices_for_view::hooked));
		//calcviewmodel::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::CalcViewModel"), __FUNCTION__).as(), reinterpret_cast<void*>(calcviewmodel::hooked));
		//onrenderstart::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::OnRenderStart"), __FUNCTION__).as(), reinterpret_cast<void*>(onrenderstart::hooked));
		draw_array_ex::safetyhook = safetyhook::create_inline(modules::scenesys.get_sig_addr(FNV("hooks::DrawArrayEx"), __FUNCTION__).as(), reinterpret_cast<void*>(draw_array_ex::hooked));
		
		//swap_chain.Apply(directx::resize_buffers::index, reinterpret_cast<uintptr_t*>(&directx::resize_buffers::hooked), reinterpret_cast<void**>(&directx::resize_buffers::original_fn));
		//dxgi.Apply(directx::create_swapchain::index, reinterpret_cast<uintptr_t*>(&directx::create_swapchain::hooked), reinterpret_cast<void**>(&directx::create_swapchain::original_fn));

		return true;
	}

	bool detach()
	{
		SetWindowLongPtrA(globals::hwnd, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

		dxgi.restore_vtable();
		entity_system.restore_vtable();
		csgo_input.restore_vtable();
		client.restore_vtable();
		swap_chain.restore_vtable();
		client_mode.restore_vtable();

		return true;
	}

	void __fastcall calcviewmodel::hooked(void* rcx, Vector& pos, float* fov, int a3)
	{
		static const auto ret = safetyhook.original<void(__fastcall*)(void*, Vector&, float*, int)>();

		ret(rcx, pos, fov, a3);

		static Convar* viewmodel_fov = g::cvar->find(FNV("viewmodel_fov"));
		*fov = settings::misc::fov_changer ? settings::misc::fov : (viewmodel_fov ? viewmodel_fov->value.as_float : 68.f);
		
		if (g::engine_client->IsInGame() && settings::misc::bhop)
		{
			pos.x += settings::misc::rotation_x == 0 ? 0 : settings::misc::rotation_x;
			pos.y += settings::misc::rotation_y == 0 ? 0 : settings::misc::rotation_y;
			pos.z += settings::misc::rotation_z == 0 ? 0 : settings::misc::rotation_z;
		}
	}

	void __fastcall onrenderstart::hooked(CViewRender* rcx)
	{
		safetyhook.fastcall<void>(rcx);
	}

	void __fastcall draw_array_ex::hooked(void* rcx, void* rdx, CSceneData* scene_data, int a4, void* scene_view, void* scene_layer, void* a7, IMaterial* material)
	{
		//IMaterial* latex_mat = nullptr;
		//if (g::engine_client->IsInGame())
		//{
		//	//static bool done = false;

		//	latex_mat = create_material(material_latex_vis, "material_latex_vis");
		//}

		//if (scene_data && scene_data->material && g::engine_client->IsInGame())
		//{
		//	if (strstr(scene_data->material->GetName(), "characters/models") && !strstr(scene_data->material->GetName(), "characters/models/shared"))
		//	{
		//		/*static IMaterial** m = nullptr;
		//		static auto mat = g::mat_system->FindMaterial(&m, "materials/dev/primary_white.vmat");*/

		//		if (latex_mat)
		//		{
		//			scene_data->material = latex_mat;
		//		}
		//	}
		//}

		safetyhook.fastcall<void>(rcx, rdx, scene_data, a4, scene_view, scene_layer, a7, material);
	}

}



