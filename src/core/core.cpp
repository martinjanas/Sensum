#include "core.h"
#include "../core/contexts/EngineContext.h"
#include "../core/systems/EntityCacheSystem.h"
#include "../core/registries/registries.h"

namespace core
{
	void ConstructContexts()
	{
		namespace reg = registries;

		reg::g_ContextRegistry.Register<EngineContext>();
		reg::g_ContextRegistry.ConstructContexts();
	}

	void ConstructSystems()
	{
		//namespace sys = systems;

		g_EntityCacheSystem.run();
	}
}