#include "EntityCacheSystem.h"

void EntityCacheSystem::run()
{
	auto& m_EngineCtx = GetContext<EngineContext>();

	if (m_EngineCtx.GetEngineClient())
	{
		m_EngineCtx.GetEngineClient()->ExecuteClientCmd("echo EntityCacheSystem::run() - EngineClient is valid!");
		g_Console->println("EntityCacheSystem::run() - EngineClient is valid!");
	}
}
