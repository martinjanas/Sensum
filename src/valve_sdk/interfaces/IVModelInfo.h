#pragma once

class IVModelInfo
{
public:
	virtual                                 ~IVModelInfo(void) {}
	virtual const model_t* GetModel(int modelindex) const = 0;
	virtual int                             GetModelIndex(const char* name) const = 0;
	virtual const char* GetModelName(const model_t* model) const = 0;

	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		return CallVFunction<studiohdr_t * (__thiscall*)(void*, const model_t*)>(this, 32)(this, mod);
	}
};