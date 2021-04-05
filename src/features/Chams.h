#pragma once

#include <deque>

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/singleton.hpp"

class Chams : public Singleton<Chams>
{
public:
	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
};
