#pragma once
#include "../core/registries/ContextRegistry.h"
#include "../core/registries/SystemsRegistry.h"
#include "../core/managers/FeatureManager.h"
#include "../core/contexts/subcontexts/ModuleSubContext.h"

/*
	Contexts - sets of data - eg: Source Engine 2 interfaces such as IVEngineClient, IEngineTrace, etc... or any other usefull data.
	Systems - logic that works together with contexts - eg: entity caching, visibility checks, etc...
	Features - takes both contexts and systems.

	Feature Manager - takes and registers features and dispatches their respective overridden methods - eg: OnCreateMove().
*/


namespace core
{
	ContextRegistry& contexts();
	SystemsRegistry& systems();
	FeatureManager& features();
	ModuleSubContext& modules();

	void ConstructContexts();
	void ConstructSystems();
}

