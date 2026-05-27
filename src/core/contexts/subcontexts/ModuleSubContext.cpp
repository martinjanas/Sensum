#include "ModuleSubContext.h"

void ModuleSubContext::ConstructSubContext()
{
	m_Client = Module("client.dll");
	m_Engine = Module("engine2.dll");
	m_Schema = Module("schemasystem.dll");
	m_Tier0 = Module("tier0.dll");
	m_Directx = Module("rendersystemdx11.dll");
	m_InputSys = Module("inputsystem.dll");
	m_Matchmaking = Module("matchmaking.dll");
	m_GameOverlay = Module("GameOverlayRenderer64.dll");
	m_MaterialSys = Module("materialsystem2.dll");
	m_SceneSys = Module("scenesystem.dll");
	m_NavSystem = Module("navsystem.dll");

	m_ModulesInitialized = true;
}
