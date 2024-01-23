#pragma once

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS		32

#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"
#define TEXTURE_GROUP_STATIC_PROPS						"StaticProp textures"

#define TEXTURE_GROUP_COMBINED						"Combined Textures"
#define TEXTURE_GROUP_COMPOSITE						"Composited Textures"

struct IDirect3DTexture9;

class IMaterial;
class Texture_t;

struct ITexture
{
	int GetActualWidth()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 3)(this);
	}

	int GetActualHeight()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 4)(this);
	}
};

typedef unsigned short MaterialHandle_t;
DECLARE_POINTER_HANDLE(MaterialLock_t);

class IMatRenderContext
{
public:
	int Release()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 1)(this);
	}

	void SetRenderTarget(ITexture* pTexture)
	{
		CallVFunction<void(__thiscall*)(void*, ITexture*)>(this, 6)(this, pTexture);
	}

	void PushRenderTargetAndViewport()
	{
		CallVFunction<void(__thiscall*)(void*)>(this, 119)(this);
	}

	void PopRenderTargetAndViewport()
	{
		CallVFunction<void(__thiscall*)(void*)>(this, 120)(this);
	}

	void DrawScreenSpaceRectangle(IMaterial* material, ITexture* texture, int x, int y, int width, int height)
	{
		CallVFunction<void(__thiscall*)(void*, void*, int, int, int, int, float, float, float, float, int, int, void*, int, int)>(this, 114)
			(this, material, x, y, width, height, 0, 0, float(width), float(height), texture->GetActualWidth(), texture->GetActualHeight(), nullptr, 1, 1);
	}
};

class IMaterialSystem : public IAppSystem
{
public:
	IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
	{
		return CallVFunction<IMaterial * (__thiscall*)(PVOID, const char*, KeyValues*)>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0)
	{
		return CallVFunction<IMaterial * (__thiscall*)(PVOID, const char*, const char*, bool, const char*)>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	MaterialHandle_t FirstMaterial()
	{
		return CallVFunction<MaterialHandle_t(__thiscall*)(PVOID)>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		return CallVFunction<MaterialHandle_t(__thiscall*)(PVOID, MaterialHandle_t)>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		return CallVFunction<MaterialHandle_t(__thiscall*)(PVOID)>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		return CallVFunction<IMaterial * (__thiscall*)(PVOID, MaterialHandle_t)>(this, 89)(this, h);
	}

	int	GetNumMaterials()
	{
		return CallVFunction<int(__thiscall*)(PVOID)>(this, 90)(this);
	}

	void BeginRenderTargetAllocation()
	{
		CallVFunction<void(__thiscall*)(PVOID)>(this, 94)(this);
	}

	void EndRenderTargetAllocation()
	{
		CallVFunction<void(__thiscall*)(PVOID)>(this, 95)(this);
	}

	int GetBackBufferFormat()
	{
		return CallVFunction<int(__thiscall*)(PVOID)>(this, 36)(this);
	}

	ITexture* CreateNamedRenderTargetTextureEx(int back_buffer, const char* name)
	{
		return CallVFunction<ITexture * (__thiscall*)(PVOID, const char*, int, int, int, int, int, int, int)>(this, 97)(this, name, 1, 1, 4, back_buffer, 0x0, 0x00000004 | 0x00000008, 1);
	}

	IMatRenderContext* GetRenderContext()
	{
		return CallVFunction<IMatRenderContext * (__thiscall*)(PVOID)>(this, 115)(this);
	}

	bool& IsGameStarted()
	{
		return *(bool*)(uintptr_t(this) + 0x2C68);
	}
};