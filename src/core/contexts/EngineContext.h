#pragma once
#include <stdio.h>
#include "../interfaces/IBaseContext.h"
#include "../../sdk/interfaces/IVEngineClient.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/console.h"

class EngineContext : public IBaseContext
{
public:

	IVEngineClient* GetEngineClient() const { return m_EngineClient; }

protected:
	virtual void ConstructContext() override
	{
		m_EngineClient = modules::engine.get_interface_from_list<IVEngineClient*>("Source2EngineToClient001");

		g_Console->println("EngineContext::ConstructContext() called, m_EngineClient: 0x%p", m_EngineClient);
	}

private:
	IVEngineClient* m_EngineClient = nullptr;
};

