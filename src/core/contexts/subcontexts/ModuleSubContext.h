#pragma once
#include "BaseSubContext.h"
#include "../../helpers/Module.h"

class ModuleSubContext : public BaseSubContext
{
public:
	Module* Client() { return &m_Client; }
	Module* Engine() { return &m_Engine; }
	Module* Schema() { return &m_Schema; }
	Module* Tier0() { return &m_Tier0; }
	Module* DirectX11() { return &m_Directx; }
	Module* InputSys() { return &m_InputSys; }
	Module* Matchmaking() { return &m_Matchmaking; }
	Module* GameOverlay() { return &m_GameOverlay; }
	Module* MaterialSys() { return &m_MaterialSys; }
	Module* SceneSys() { return &m_SceneSys; }
	Module* NavSystem() { return &m_NavSystem; }

public:
	void ConstructSubContext() override;

	bool IsReady() const { return m_ModulesInitialized; }

private:
	Module m_Client;
	Module m_Engine;
	Module m_Schema;
	Module m_Tier0;
	Module m_Directx;
	Module m_InputSys;
	Module m_Matchmaking;
	Module m_GameOverlay;
	Module m_MaterialSys;
	Module m_SceneSys;
	Module m_NavSystem;

	bool m_ModulesInitialized = false;
};

