#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

#pragma comment(lib, "d3d11.lib")

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

static const char material_latex_vis[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_character.vfx"
    F_BLEND_MODE = 1
	F_DISABLE_Z_WRITE = 0
    F_RENDER_BACKFACES = 0
    g_vColorTint = [1.0, 1.0, 1.0, 1.0]
    g_bFogEnabled = 0
    g_flMetalness = 0.000
    g_tMetalness = resource:"materials/default/default_metal_tga_8fbc2820.vtex"
    g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_79a2e0d0.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_1b833b2a.vtex"
} )";

static const char material_latex_invis[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_character.vfx"
    F_BLEND_MODE = 1
    F_DISABLE_Z_BUFFERING = 1
	F_DISABLE_Z_WRITE = 0
    F_RENDER_BACKFACES = 0
    g_vColorTint = [1.0, 1.0, 1.0, 1.0]
    g_bFogEnabled = 0
    g_flMetalness = 0.000
    g_tMetalness = resource:"materials/default/default_metal_tga_8fbc2820.vtex"
    g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_79a2e0d0.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_1b833b2a.vtex"
} )";

static const char material_bloom_vis[] = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
                shader = "solidcolor.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [9.0, 9.0, 9.0, 9.0]
			} )#";

static const char material_bloom_invis[] = R"#(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
			format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
			{
				shader = "solidcolor.vfx"
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
				g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
				g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
				F_IGNOREZ = 1
				F_DISABLE_Z_WRITE = 1
				F_DISABLE_Z_BUFFERING = 1
				F_RENDER_BACKFACES = 1
				g_vColorTint = [9.0, 9.0, 9.0, 9.0]
			} )#";

static const char szGlowVmatBuffer[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_effects.vfx"
    g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
    g_tMask1 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tMask2 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tMask3 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
	g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
	g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
 
    g_flOpacityScale = 7.55
    g_flFresnelExponent = 7.75
    g_flFresnelFalloff = 5
    g_flFresnelMax = 0.0
    g_flFresnelMin = 9
    F_ADDITIVE_BLEND = 1
    F_BLEND_MODE = 1
    F_TRANSLUCENT = 1
    F_IGNOREZ = 0
    F_DISABLE_Z_WRITE = 0
    F_DISABLE_Z_BUFFERING = 0
    F_RENDER_BACKFACES = 0
    g_vColorTint = [7.0, 7.0, 7.0, 0.37522]
} )";

const char vmat_buffer[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_unlitgeneric.vfx"
	F_DISABLE_Z_BUFFERING = 0
	g_flBumpStrength = 1
	g_vColorTint = [1.000000, 1.000000, 1.000000, 0.000000]
	g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
	g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
	g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
	g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
	g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
	g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
	g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";

const char vmat_buffer_invis[] = R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_unlitgeneric.vfx"
	F_DISABLE_Z_BUFFERING = 1
	g_flBumpStrength = 1
	g_vColorTint = [1.000000, 1.000000, 1.000000, 0.000000]
	g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
	g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
	g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
	g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
	g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
	g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
	g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";

IMaterial* CreateMaterial(const char* material_vmat, const char* mat_name)
{
	IMaterial** temp{};
	
	//Stack allocated buffer with memset hopefully fixed the nonsense crashes with LoadKV3
	uint8_t buffer[0x100 + sizeof(KeyValues)];
	memset(buffer, 0, sizeof(buffer));

	KeyValues* kv = reinterpret_cast<KeyValues*>(buffer + 0x100);
	if (!KeyValues::LoadKV3(kv, material_vmat, mat_name))
		return nullptr;

	g::mat_system->CreateMaterial(&temp, mat_name, kv, 0, 1);

	return *temp;
}

namespace hooks
{
	IMaterial* g_pLatexMat_vis = nullptr;
	IMaterial* g_pLatexMat_invis = nullptr;
	static bool done = false;
	
	bool CreateDeviceD3D11(HWND hWnd, ID3D11Device*& device, IDXGISwapChain*& swap_chain)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;

		const D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0,
		};

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_NULL, NULL, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
			&swapChainDesc, &swap_chain, &device, nullptr, nullptr);

		if (FAILED(hr))
			return false;
    
		return true;
	}
	
	bool init()
	{
		IDXGIFactory* dxgi_factory = nullptr;
		//get_dxgi(dxgi_factory);

		if (dxgi_factory)
		{
			dxgi = ShadowVMT(dxgi_factory);
			dxgi_factory->Release();
		}

		ID3D11Device* device = nullptr;
		IDXGISwapChain* swapchain = nullptr;
		
		HWND hwnd = FindWindow(nullptr, L"Counter-Strike 2");
		CreateDeviceD3D11(hwnd, device, swapchain);

		//TODO: Add manual reset function to ShadowVMT ?
		entity_system = ShadowVMT(g::entity_system);
		csgo_input = ShadowVMT(g::csgo_input);
		client = ShadowVMT(g::client);
		client_mode = ShadowVMT(g::client_mode_csnormal);

		entity_system.apply(on_add_entity::index, reinterpret_cast<uintptr_t*>(&on_add_entity::hooked), reinterpret_cast<void**>(&on_add_entity::original_fn));
		entity_system.apply(on_remove_entity::index, reinterpret_cast<uintptr_t*>(&on_remove_entity::hooked), reinterpret_cast<void**>(&on_remove_entity::original_fn));

		csgo_input.apply(createmove_csgoinput::index, reinterpret_cast<uintptr_t*>(&createmove_csgoinput::hooked), reinterpret_cast<void**>(&createmove_csgoinput::original_fn));
		csgo_input.apply(createmove_csgoinput21::index, reinterpret_cast<uintptr_t*>(&createmove_csgoinput21::hooked), reinterpret_cast<void**>(&createmove_csgoinput21::original_fn));

		client.apply(frame_stage_notify::index, reinterpret_cast<uintptr_t*>(&frame_stage_notify::hooked), reinterpret_cast<void**>(&frame_stage_notify::original_fn));
		
		client_mode.apply(level_init::index, reinterpret_cast<uintptr_t*>(&level_init::hooked), reinterpret_cast<void**>(&level_init::original_fn));
		client_mode.apply(level_shutdown::index, reinterpret_cast<uintptr_t*>(&level_shutdown::hooked), reinterpret_cast<void**>(&level_shutdown::original_fn));
		
		//dxgi.apply(directx::create_swapchain::index, reinterpret_cast<uintptr_t*>(&directx::create_swapchain::hooked), reinterpret_cast<void**>(&directx::create_swapchain::original_fn));
		
		auto vtable = *reinterpret_cast<void***>(swapchain);
		//directx::present::safetyhook = safetyhook::create_inline((void*)vtable[8], reinterpret_cast<void*>(directx::present::hooked));
		get_matrices_for_view::safetyhook = safetyhook::create_inline(modules::client.get_sig(xxh::get_hash("hooks::GetMatricesForView"), __FUNCTION__).as(), reinterpret_cast<void*>(get_matrices_for_view::hooked));
		//draw_array_ex::safetyhook = safetyhook::create_inline(modules::scenesys.get_sig(xxh::get_hash("hooks::DrawArrayEx"), __FUNCTION__).as(), reinterpret_cast<void*>(draw_array_ex::hooked));
		//calcviewmodel::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::CalcViewModel"), __FUNCTION__).as(), reinterpret_cast<void*>(calcviewmodel::hooked));
		//onrenderstart::safetyhook = safetyhook::create_inline(modules::client.get_sig_addr(FNV("hooks::OnRenderStart"), __FUNCTION__).as(), reinterpret_cast<void*>(onrenderstart::hooked));
		
		return true;
	}

	bool detach()
	{
		SetWindowLongPtrA(globals::hwnd, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

		dxgi.restore_vtable();
		entity_system.restore_vtable();
		csgo_input.restore_vtable();
		client.restore_vtable();
		client_mode.restore_vtable();

		hooks::directx::present::safetyhook.reset();
		hooks::draw_array_ex::safetyhook.reset();
		hooks::get_matrices_for_view::safetyhook.reset();

		//hooks::onrenderstart::safetyhook.reset();
		//hooks::calcviewmodel::safetyhook.reset();
		
		return true;
	}

	void __fastcall calcviewmodel::hooked(void* rcx, Vector& pos, float* fov, int a3)
	{
		static const auto ret = safetyhook.original<void(__fastcall*)(void*, Vector&, float*, int)>();

		ret(rcx, pos, fov, a3);

		static auto hash = xxh::get_hash("viewmodel_fov");
		static Convar* viewmodel_fov = g::cvar->find(hash);
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

	void chams(void* rcx, void* rdx, CSceneData* scene_data, int a4, void* scene_view, void* scene_layer, void* a7, IMaterial* material)
	{
		if (!g::engine_client->IsInGame())
			return;

		static const auto& original_fn = hooks::draw_array_ex::safetyhook.original<void(__fastcall*)(void*, void*, CSceneData*, int, void*, void*, void*, IMaterial*)>();
		
		CCSPlayerController* local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
		if (!local_controller)
			return;

		CCSPlayerPawn* local_pawn = reinterpret_cast<CCSPlayerPawn*>(g::entity_system->GetEntityFromHandle(local_controller->m_hPawn()));
		if (!local_pawn)
			return;

		if (!scene_data || !scene_data->material)
			return;

		//doesnt really return the material's owner, but C_World entity...
		auto entity = reinterpret_cast<CCSPlayerController*>(g::entity_system->GetEntityFromHandle(scene_data->scene_object->owner_handle));
		if (!entity)
			return;

		auto pawn = reinterpret_cast<CCSPlayerPawn*>(g::entity_system->GetEntityFromHandle(entity->m_hPawn()));
		if (!pawn)
			return;
		
		//somehow prevents chams being applied on viewmodel, huh?, but localpawn is being chammed when in thirdperson.
		if (local_pawn == pawn)
			return;
		
		const char* mat_name = scene_data->material->GetName();
		if (!strstr(mat_name, "characters/models/"))
			return;
		
		if (!settings::esp::visible_only)
		{
			if (g_pLatexMat_invis)
			{
				scene_data->material = g_pLatexMat_invis;
				scene_data->color = { 255, 0, 0, 255 };
			}

			original_fn(rcx, rdx, scene_data, a4, scene_view, scene_layer, a7, material);
		}
		
		if (g_pLatexMat_vis)
		{
			scene_data->material = g_pLatexMat_vis;
			scene_data->color = { 0, 255, 0, 255 };
		}
	}
	
	//rcx: CAnimatableSceneObjectDesc, rdx: some custom DX11 related class/manager?
	void __fastcall draw_array_ex::hooked(void* rcx, void* rdx, CSceneData* scene_data, int a4, void* scene_view, void* scene_layer, void* a7, IMaterial* material)
	{
		static const auto original_fn = safetyhook.original<decltype(&draw_array_ex::hooked)>();
		
		if (!done)
		{
			done = true;
			g_pLatexMat_vis = CreateMaterial(material_latex_vis, "material_latex_vis");
			g_pLatexMat_invis = CreateMaterial(material_latex_invis, "material_latex_invis");
		}
		
		//Bug: chams rendering on local team
		chams(rcx, rdx, scene_data, a4, scene_view, scene_layer, a7, material);

		original_fn(rcx, rdx, scene_data, a4, scene_view, scene_layer, a7, material);
	}
}



