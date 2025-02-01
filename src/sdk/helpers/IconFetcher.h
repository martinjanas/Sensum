#pragma once
#include <unordered_map>
#include "../../thirdparty/ImGui/imgui_internal.h"
#include <d3d11.h>

struct icon_t
{
    int w;
    int h;
    ImVec4 tint = ImVec4(1.f, 1.f, 1.f, 1.f);
    ID3D11ShaderResourceView* texture;

    void SetScale(float scale)
    {
        w *= scale;
        h *= scale;
    }
};

enum class EIconType : uint8_t
{
    UNKNOWN,
    /* Grenade icons: */
    SMOKE,
    FLASHBANG,
    HEGRENADE,
    MOLOTOV,
    DECOY,
    
    /* Util icons: */
    KEVLAR,
    KEVLAR_HELMET,
    DEFUSE_KIT,
    C4,
    PLANTED_C4,
    INFERNO,
    
    /* Gun icons: */
    AK47,
    M4A4,
    M4A1S,
    FAMAS,
    GALIL,
    SG556,
    AUG,
    SSG08,
    AWP,
    G3SG1,
    SCAR20,
    MP9,
    MP7,
    MP5,
    P90,
    MAC10,
    UMP45,
    PPBIZON,
    NOVA,
    XM1014,
    MAG7,
    SAWEDOFF,
    NEGEV,
    M249,
    GLOCK,
    USP,
    P2000,
    P250,
    FIVE_SEVEN,
    CZ75,
    DEAGLE,
    TEC9,
    ELITE,
    REVOLVER,
    ZEUS27,
    HEALTHSHOT,
    
    /*Knife icons: */
    KNIFE_CT,
    KNIFE_T,
    KNIFE_GG,
    KNIFE_BAYONET,
    KNIFE_SURVIVAL_BOWIE,
    KNIFE_BUTTERFLY,
    KNIFE_CLASSIC,
    KNIFE_FALCHION,
    KNIFE_FLIP,
    KNIFE_GUT,
    KNIFE_HUNTSMAN,
    KNIFE_KURKI,
    KNIFE_M9_BAYONET,
    KNIFE_NAVAJA,
    KNIFE_SHADOW_DAGGERS,
    KNIFE_SKELETON,
    KNIFE_TALON,
    KNIFE_URSUS,
    KNIFE_KARAMBIT,
    KNIFE_STILETTO,
    KNIFE_NOMAD,
    KNIFE_SURVIVAL,
    KNIFE_PARACORD,
    KNIFE_KUKRI
};

namespace icon_fetcher
{
    extern std::unordered_map<EIconType, icon_t> icon_data;

    void fetch_icon_data(class ID3D11Device* device);
    EIconType get_icon_type_by_weapon_index(uint16_t index);
    icon_t get(const EIconType& type);
}
