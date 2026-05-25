#include "EngineContext.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/console.h"
#include "../helpers/Modules2.h"

void EngineContext::ConstructContext()
{
	m_EngineClient = modules2::engine2.GetInterface<IVEngineClient*>("Source2EngineToClient001");
	m_GameClient = modules2::client2.GetInterface<CSource2Client*>("Source2Client002");
	m_Schemas = modules2::schema2.GetInterface<CSchemaSystem*>("SchemaSystem_001");
	m_GameResources = modules2::engine2.GetInterface<CGameResourceService*>("GameResourceServiceClientV001");
	m_GameInput = modules2::input_sys2.GetInterface<CInputSystem*>("InputSystemVersion001");
	m_GameNetwork = modules2::engine2.GetInterface<CNetworkGameService*>("NetworkClientService_001");
	m_GameType = modules2::matchmaking2.GetInterface<CGameType*>("GameTypes001");
	m_EngineCvar = modules2::tier02.GetInterface<IVEngineCVar*>("VEngineCvar007");
	m_GameMaterials = modules2::materialsys2.GetInterface<CMaterialSystem*>("VMaterialSystem2_001");
	m_GameEntities = m_GameResources->GetEntitySystem();

	//TODO: Implement GetSig
	//m_CSInput = modules2::client2.GetSig(xxh::get_hash("g::csgo_input")).as<CSGOInput*>();
	//m_GameRender = **modules2::directx112.GetSig(xxh::get_hash("g::render_system")).add(0x8).as<CRenderSystem***>();
	//m_GlobalVars = *modules2::client2.GetSig(xxh::get_hash("g::global_vars")).as<CGlobalVarsBase**>();
	//m_EngineTrace = *modules2::client2.GetSig(xxh::get_hash("g::engine_trace")).as<CGameTrace**>();
	//m_ClientMode = modules2::client2.GetSig(xxh::get_hash("g::clientmode_csnormal")).as<CClientModeCSNormal*>();
	//m_GameRules = *modules2::client2.GetSig(xxh::get_hash("g::game_rules")).as<CGameRules**>();

	m_MemAlloc = modules2::tier02.GetExport("g_pMemAlloc").As<IMemAlloc*>();

	//m_HudChat = *reinterpret_cast<CHudChat**>(globals::find_hud_element("HudChatDelegate"));

	g_Console->println("EngineContext::ConstructContext() called, m_EngineClient: 0x%p", m_EngineClient);
}
