#pragma once
#include <unordered_map>
#include "../../thirdparty/ImGui/imgui_internal.h"
#include <d3d11.h>

struct icon_data_t
{
    int w;
    int h;
    ImVec4 tint = ImVec4(1.f, 1.f, 1.f, 1.f);
    ID3D11ShaderResourceView* texture;
};

enum class EIconType : uint8_t
{
    /* Util icons: */
    SMOKE,
    FLASHBANG,
    HEGRENADE,
    MOLOTOV,
    DECOY,
    DEFUSE_KIT,
    KEVLAR,
    KEVLAR_HELMET,
    /* Gun icons: */
    AK47,
    M4A4,
    M4A1S,
    FAMAS,
    GALIL,
    SG553,
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
    PPBIZON,
    NOVA,
    XM1014,
    MAG7,
    NEGEV,
    M249,
    GLOCK,
    USP,
    P2000,
    P250,
    CZ75,
    DEAGLE,
    TEC9,
    ELITE
};

namespace icon_fetcher
{
    extern std::unordered_map<EIconType, icon_data_t> icon_data;

    void fetch_icon_data(class ID3D11Device* device);
    icon_data_t get(const EIconType& type);
}
