#pragma once
#include "../systems/BaseSystem.h"
#include "../contexts/EngineContext.h"

//See ISystem for definition of systems:
class EntityCacheSystem : public BaseSystem<EngineContext>
{
public:
	using BaseSystem::BaseSystem;

	void OnCreate() override;
	void OnUpdate() override;

private:
	EngineContext* m_EngineCtx;
};

inline EntityCacheSystem g_EntityCacheSystem;

