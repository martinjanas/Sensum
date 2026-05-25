#pragma once
#include <stdio.h>
#include "../contexts/BaseContext.h"
#include "../../sdk/interfaces/IVEngineClient.h"
#include "../../sdk/interfaces/IVEngineCVar.h"
#include "../../sdk/interfaces/CSource2Client.h"
#include "../../sdk/interfaces/CGameResourceService.h"
#include "../../sdk/interfaces/CSGOInput.h"
#include "../../sdk/interfaces/CRenderSystem.h"
#include "../../sdk/interfaces/CInputSystem.h"
#include "../../sdk/interfaces/ClientModeCSNormal.h"
#include "../../sdk/interfaces/CGlobalVars.h"
#include "../../sdk/interfaces/CNetworkGameService.h"
#include "../../sdk/interfaces/CGameType.h"
#include "../../sdk/interfaces/CHudChat.h"
#include "../../sdk/interfaces/CGameRules.h"
#include "../../sdk/interfaces/CMaterialSystem.h"
#include "../../sdk/interfaces/IMemAlloc.h"

/*

	EngineContext - class for containing various Source Engine 2 interfaces:
	- such as IVEngineClient, IEngineTrace, IGlobalVars, etc...

*/

class EngineContext : public BaseContext
{
public:
	CSource2Client* GameClient() { return m_GameClient; }
	IVEngineClient* EngineClient() { return m_EngineClient; }
	IVEngineCVar* EngineCvar() { return m_EngineCvar; }
	IMemAlloc* MemAlloc() { return m_MemAlloc; }
	CSchemaSystem* Schemas() { return m_Schemas; }
	CGameResourceService* GameResources() { return m_GameResources; }
	CEntitySystem* GameEntities() { return m_GameEntities; }
	CSGOInput* CSInput() { return m_CSInput; }
	CRenderSystem* GameRender() { return m_GameRender; }
	CInputSystem* GameInput() { return m_GameInput; }
	CClientModeCSNormal* ClientMode() { return m_ClientMode; }
	CGlobalVarsBase* GlobalVars() { return m_GlobalVars; }
	CGameTrace* EngineTrace() { return m_EngineTrace; }
	CNetworkGameService* GameNetwork() { return m_GameNetwork; }
	CGameType* GameType() { return m_GameType; }
	CHudChat* HudChat() { return m_HudChat; }
	CGameRules* GameRules() { return m_GameRules; }
	CMaterialSystem* GameMaterials() { return m_GameMaterials; }

public:
	virtual void ConstructContext() override;

private:
	CSource2Client* m_GameClient;
	IVEngineClient* m_EngineClient = nullptr;
	IVEngineCVar* m_EngineCvar;
	IMemAlloc* m_MemAlloc;
	CSchemaSystem* m_Schemas;
	CGameResourceService* m_GameResources;
	CEntitySystem* m_GameEntities;
	CSGOInput* m_CSInput;
	CRenderSystem* m_GameRender;
	CInputSystem* m_GameInput;
	CClientModeCSNormal* m_ClientMode;
	CGlobalVarsBase* m_GlobalVars;
	CGameTrace* m_EngineTrace;
	CNetworkGameService* m_GameNetwork;
	CGameType* m_GameType;
	CHudChat* m_HudChat;
	CGameRules* m_GameRules;
	CMaterialSystem* m_GameMaterials;
};

