#pragma once

enum DrawFlags_t
{
	DF_RENDER_REFRACTION = 0x1,
	DF_RENDER_REFLECTION = 0x2,
	DF_CLIP_Z = 0x4,
	DF_CLIP_BELOW = 0x8,
	DF_RENDER_UNDERWATER = 0x10,
	DF_RENDER_ABOVEWATER = 0x20,
	DF_RENDER_WATER = 0x40,
	DF_UNUSED1 = 0x100,
	DF_WATERHEIGHT = 0x200,
	DF_UNUSED2 = 0x400,
	DF_DRAWSKYBOX = 0x800,
	DF_FUDGE_UP = 0x1000,
	DF_DRAW_ENTITITES = 0x2000,
	DF_UNUSED3 = 0x4000,
	DF_UNUSED4 = 0x8000,
	DF_UNUSED5 = 0x10000,
	DF_SAVEGAMESCREENSHOT = 0x20000,
	DF_CLIP_SKYBOX = 0x40000,
	DF_SHADOW_DEPTH_MAP = 0x100000	// Currently rendering a shadow depth map
};

//-----------------------------------------------------------------------------
// Purpose: View setup and rendering
//-----------------------------------------------------------------------------
class CViewSetup;
class c_base_entity;
struct vrect_t;
class c_base_view_model;
class IMaterial;

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};

enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

class IViewRender
{
public:
};