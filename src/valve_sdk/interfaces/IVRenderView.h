#pragma once

class IVRenderView
{
public:
	void SetBlend(float blend)
	{
		CallVFunction<void(__thiscall*)(void*, float)>(this, 4)(this, blend);
	}

	void SetColorModulation(float const* blend)
	{
		CallVFunction<void(__thiscall*)(void*, float const*)>(this, 6)(this, blend);
	}

	inline void SetColorModulation(float r, float g, float b)
	{
		float clr[3] = { r, g, b };
		SetColorModulation(clr);
	}
};
