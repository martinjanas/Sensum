#pragma once
#include "BaseSystem.h"
#include "../contexts/EngineContext.h"
#include "../../sdk/helpers/xxhash.h"
#include "../helpers/PatternScanner.h"

class SigCachingSystem : public BaseSystem<EngineContext> //WIP
{
public:
	using BaseSystem::BaseSystem;

	void OnCreate() override;
	void OnUpdate() override;

private:
	std::unordered_map<XXH64_hash_t, DPatternScanner> m_CachedSigs;
};

inline SigCachingSystem g_SigCachingSystem;

