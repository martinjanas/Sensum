#include "core.h"
#include "../core/contexts/EngineContext.h"
#include "../core/systems/EntityCacheSystem.h"

namespace core
{
	ContextRegistry& contexts()
	{
		static ContextRegistry instance;

		return instance;
	}

	SystemsRegistry& systems()
	{
		static SystemsRegistry instance;

		return instance;
	}

	FeatureManager& features()
	{
		static FeatureManager instance;

		return instance;
	}

	ModuleSubContext& modules()
	{
		return *contexts().GetContext<EngineContext>().Modules();
	}

	void ConstructContexts()
	{
		contexts().Register<EngineContext>();
		contexts().ConstructContexts();
	}

	void ConstructSystems()
	{
		g_EntityCacheSystem.OnCreate();
		g_EntityCacheSystem.OnUpdate();
	}
}