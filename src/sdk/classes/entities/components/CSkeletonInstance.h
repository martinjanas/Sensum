#pragma once
#include "../../../../helpers/vfunc.h"
#include "../../../../helpers/netvars.h"
#include "../../../../sdk/classes/entities/components/CGameSceneNode.h"
#include "../../../../sdk/classes/structs_and_classes/CBoneSystem.h"
#include "../../../../sdk/classes/structs_and_classes/Hitbox_t.h"


class CSkeletonInstance //: public CGameSceneNode //Compiler error because of line #4: include loop 
{
public:
	NETVAR(CModelState, m_modelState, "CSkeletonInstance", "m_modelState");
	NETVAR(uint8_t, m_nHitboxSet, "CSkeletonInstance", "m_nHitboxSet");
};
