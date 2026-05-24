#pragma once
#include "../interfaces/ISystem.h"
#include "../contexts/EngineContext.h"


//See ISystem for definition of systems:
class EntityCacheSystem : public ISystem<EngineContext>
{
public:
	void run();
};

inline EntityCacheSystem g_EntityCacheSystem;

