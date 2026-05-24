#pragma once

/*
	Contexts - sets of data - eg: Source Engine 2 interfaces such as IVEngineClient, IEngineTrace, etc... or any other usefull data.
	Systems - logic that works together with contexts - eg: entity caching, visibility checks, etc...
	Features - takes both contexts and systems.

	Feature Manager - takes and registers features and dispatches their respective overridden methods - eg: OnCreateMove().
*/


namespace core
{
	void ConstructContexts();
	void ConstructSystems();
}

