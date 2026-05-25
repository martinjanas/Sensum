#include "EntityCacheSystem.h"
#include "../../sdk/helpers/console.h"

void EntityCacheSystem::OnCreate()
{
	m_EngineCtx = &GetContext<EngineContext>();
}

void EntityCacheSystem::OnUpdate()
{
	if (m_EngineCtx->EngineClient())
	{
		g_Console->println("EntityCacheSystem::run() - EngineClient is valid!, 0x%p", m_EngineCtx->EngineClient());
	}
}
