#include "EngineContext.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/console.h"

void EngineContext::ConstructContext()
{
	Modules()->ConstructSubContext();
	if (!Modules()->IsReady())
		return;

	m_EngineClient = Modules()->Engine()->GetInterface<IVEngineClient*>("Source2EngineToClient001");
	m_GameClient = Modules()->Client()->GetInterface<CSource2Client*>("Source2Client002");
	m_Schemas = Modules()->Schema()->GetInterface<CSchemaSystem*>("SchemaSystem_001");
	m_GameResources = Modules()->Engine()->GetInterface<CGameResourceService*>("GameResourceServiceClientV001");
	m_GameInput = Modules()->InputSys()->GetInterface<CInputSystem*>("InputSystemVersion001");
	m_GameNetwork = Modules()->Engine()->GetInterface<CNetworkGameService*>("NetworkClientService_001");
	m_GameType = Modules()->Matchmaking()->GetInterface<CGameType*>("GameTypes001");
	m_EngineCvar = Modules()->Tier0()->GetInterface<IVEngineCVar*>("VEngineCvar007");
	m_GameMaterials = Modules()->MaterialSys()->GetInterface<CMaterialSystem*>("VMaterialSystem2_001");
	m_GameEntities = m_GameResources->GetEntitySystem();

	//TODO: Implement GetSig
	//m_CSInput = Modules()->Client()->GetSig(xxh::get_hash("g::csgo_input")).as<CSGOInput*>();
	//m_GameRender = **Modules()->Directx()->GetSig(xxh::get_hash("g::render_system")).add(0x8).as<CRenderSystem***>();
	//m_GlobalVars = *Modules()->Client()->GetSig(xxh::get_hash("g::global_vars")).as<CGlobalVarsBase**>();
	//m_EngineTrace = *Modules()->Client()->GetSig(xxh::get_hash("g::engine_trace")).as<CGameTrace**>();
	//m_ClientMode = Modules()->Client()->GetSig(xxh::get_hash("g::clientmode_csnormal")).as<CClientModeCSNormal*>();
	//m_GameRules = *Modules()->Client()->GetSig(xxh::get_hash("g::game_rules")).as<CGameRules**>();

	m_MemAlloc = Modules()->Tier0()->GetExport("g_pMemAlloc").As<IMemAlloc*>();

	//m_HudChat = *reinterpret_cast<CHudChat**>(globals::find_hud_element("HudChatDelegate"));

	g_Console->println("EngineContext::ConstructContext() called, m_EngineClient: 0x%p", m_EngineClient);
}
