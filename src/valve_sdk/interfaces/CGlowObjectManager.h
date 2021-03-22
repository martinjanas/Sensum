#pragma once
#include <vector>
#include "../valve_sdk/math/Vectors.hpp"

class CGlowObjectDefinition 
{
public:
	void* get_entity()
	{
		return entity;
	}

	void color(Color& color)
	{
		this->clr = Vector(color.r() / 255.f, color.g() / 255.f, color.b() / 255.f);
		this->m_flAlpha = color.a();
	}

	void glow_style(int style)
	{
		this->m_nGlowStyle = style;
	}

	void bloom_amount(float amount)
	{
		this->m_flBloomAmount = amount;
	}

	void full_bloom_render(bool value)
	{
		this->m_bFullBloomRender = value;
	}

	void render_when_occluded(bool value)
	{
		this->m_bRenderWhenOccluded = value;
	}

	void render_when_unoccluded(bool value)
	{
		this->m_bRenderWhenUnoccluded = value;
	}

	bool unused()
	{
		return m_nNextFreeSlot != -2;
	}

private:
	void*		  entity;
	Vector        clr;						
	float         m_flAlpha;                 //0x0010
	uint8_t		  pad_0014[4];               //0x0014
	float         m_flSomeFloat;             //0x0018
	uint8_t		  pad_001C[4];               //0x001C
	float         m_flBloomAmount;           //0x0020
	bool          m_bRenderWhenOccluded;     //0x0024
	bool          m_bRenderWhenUnoccluded;   //0x0025
	bool          m_bFullBloomRender;        //0x0026
	uint8_t       pad_0027[5];               //0x0027
	int32_t       m_nGlowStyle;              //0x002C
	int32_t       m_nSplitScreenSlot;        //0x0030
	int32_t       m_nNextFreeSlot;           //0x0034
};

class CGlowManager 
{
public:
	CGlowObjectDefinition* objects;
	char pad[8];
	int size;
};
