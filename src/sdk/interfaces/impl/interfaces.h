#pragma once
#include "Interface.h"
#include <memory>
#include <vector>
#include <functional>

#include "../CGameResourceService.h"
#include "../CGameRules.h"
#include "../CGameType.h"
#include "../CInputSystem.h"
#include "../ClientModeCSNormal.h"
#include "../CMaterialSystem.h"
#include "../CNetworkGameService.h"
#include "../CRenderSystem.h"
#include "../CSGOInput.h"
#include "../CSource2Client.h"
#include "../IMemAlloc.h"
#include "../IVEngineClient.h"
#include "../IVEngineCVar.h"

class Interfaces
{
public:
    void init();
    void print_status() const;
    void validate_interfaces_on_map_load();
    void invalidate_interfaces();
    
    std::unique_ptr<Engine> engine;
    std::unique_ptr<CSource2Client> client;
    std::unique_ptr<SchemaSystemInterface> schema_system;
    std::unique_ptr<CGameResourceService> game_resources;
    std::unique_ptr<CEntitySystem> entity_system;
    std::unique_ptr<CInputSystem> input_system;
    std::unique_ptr<CNetworkGameService> network_service;
    std::unique_ptr<CGameType> game_type;
    std::unique_ptr<CVarInterface> cvar;
    std::unique_ptr<CGameTrace> engine_trace;
    std::unique_ptr<CClientModeCSNormal> client_mode;
    std::unique_ptr<CGameRules> game_rules;
    std::unique_ptr<CMaterialSystem> material_system;
    std::unique_ptr<IMemAlloc> mem_alloc;

    CSGOInput* csgo_input;
    CRenderSystem* render_system;
    CGlobalVarsBase* global_vars;
private:
    std::vector<std::reference_wrapper<Interface>> m_interfaces;
};
