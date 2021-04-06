#pragma once

#include "../helpers/singleton.hpp"
#include "iostream"
#include "../valve_sdk/interfaces/ISurface.h"
#include "../valve_sdk/misc/Color.hpp"
#include "../helpers/imdraw.h"

struct dmg_indicator
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	c_base_player* Player;
	Vector Position;
};
inline std::vector<dmg_indicator> indicator;

class VGSHelper : public Singleton<VGSHelper>
{
public:
	void Init();
	void DrawText(std::string text, float x, float y, Color color, int size = 15);
	void DrawIcon(std::string text, float x, float y, Color color, int size);
	void drawring_3d(int16_t x, int16_t y, int16_t z, int16_t radius, uint16_t points, Color color1, float thickness);
	void DrawLine(float x1, float y1, float x2, float y2, Color color, float size = 1.f);
	void DrawBox(float x1, float y1, float x2, float y2, Color clr, float size = 1.f);
	void DrawFilledBox(float x1, float y1, float x2, float y2, Color clr);
	void DrawTriangle(int count, Vertex_t* vertexes, Color c);
	void DrawBoxEdges(float x1, float y1, float x2, float y2, Color clr, float edge_size, float size = 1.f);
	void DrawCircle(float x, float y, float r, int seg, Color clr);
	void DrawCircle(Vector2D position, float r, int seg, Color clr);

	ImVec2 GetSize(std::string text, int size = 15);
private:
	bool Inited = false;
	vgui::HFont fonts[128];
	vgui::HFont icons[128];
};