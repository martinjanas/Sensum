#pragma once
#include <memory>
#include "../registries/registries.h"

/*
	Will features use systems or contexts?

	Systems should have logic - eg: for calculating hitbox positions, caching entities, or for calculating visibility...
	Contexts should be used for storing interfaces and other data that features/systems might need to access.

	So features should have both contexts and systems.
*/

class IFeature
{
public:
	
	virtual ~IFeature() = default;

	bool IsEnabled() const { return m_Enabled; }
	void SetEnabled(bool enabled) { m_Enabled = enabled; }

	template <typename T>
	T& GetContext()
	{
		return core::registries::g_ContextRegistry.GetContext<T>();
	}

public:
	virtual void OnCreateMove() { }
	virtual void OnFrameStageNotify() { }
	virtual void OnRender() { }


private:
	bool m_Enabled = false;
};
