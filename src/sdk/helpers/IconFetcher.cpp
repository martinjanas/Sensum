#include "IconFetcher.h"

#include "Icons.h"
#include "utils.h"
#include "../classes/CBasePlayerWeapon.h"

namespace icon_fetcher
{
    std::unordered_map<EIconType, icon_t> icon_data;
    
    void fetch_icon_data(ID3D11Device* device)
    {
        auto populate_data = [&](EIconType icon_type, const uint8_t* data, size_t size, int width, int height)
        {
            icon_t temp;
            temp.w = width;
            temp.h = height;
            temp.texture = utils::create_image_texture(data, size, &temp.w, &temp.h, device);

            icon_data[icon_type] = temp;
        };

        populate_data(EIconType::UNKNOWN, icons::decoy, sizeof(icons::decoy), 30, 32);
        
        populate_data(EIconType::SMOKE, icons::smokegrenade, sizeof(icons::smokegrenade), 15, 32);
        populate_data(EIconType::MOLOTOV, icons::molotov, sizeof(icons::molotov), 19, 32);
        populate_data(EIconType::HEGRENADE, icons::hegrenade, sizeof(icons::hegrenade), 19, 32);
        populate_data(EIconType::FLASHBANG, icons::flashbang, sizeof(icons::flashbang), 30, 32);
        populate_data(EIconType::DECOY, icons::decoy, sizeof(icons::decoy), 30, 32);

        populate_data(EIconType::KEVLAR, icons::kevlar, sizeof(icons::kevlar), 26, 32);
        populate_data(EIconType::KEVLAR_HELMET, icons::helmet, sizeof(icons::helmet), 20, 32);
        populate_data(EIconType::DEFUSE_KIT, icons::defuser, sizeof(icons::defuser), 36, 32);
        populate_data(EIconType::C4, icons::c4, sizeof(icons::c4), 32, 32);
        populate_data(EIconType::PLANTED_C4, icons::planted_c4, sizeof(icons::planted_c4), 30, 29);
        populate_data(EIconType::INFERNO, icons::inferno, sizeof(icons::inferno), 30, 29);
        
        populate_data(EIconType::AK47, icons::ak47, sizeof(icons::ak47), 89, 32);
        populate_data(EIconType::M4A4, icons::m4a1, sizeof(icons::m4a1), 79, 32);
        populate_data(EIconType::M4A1S, icons::m4a1_silencer, sizeof(icons::m4a1_silencer), 97, 32);
        populate_data(EIconType::FAMAS, icons::famas, sizeof(icons::famas), 78, 32);
        populate_data(EIconType::GALIL, icons::galilar, sizeof(icons::galilar), 90, 32);
        populate_data(EIconType::SG556, icons::sg556, sizeof(icons::sg556), 90, 32);
        populate_data(EIconType::AUG, icons::aug, sizeof(icons::aug), 76, 32);
        
        populate_data(EIconType::AWP, icons::awp, sizeof(icons::awp), 110, 32);
        populate_data(EIconType::SSG08, icons::ssg08, sizeof(icons::ssg08), 100, 32);
        populate_data(EIconType::SCAR20, icons::scar20, sizeof(icons::scar20), 98, 32);
        populate_data(EIconType::G3SG1, icons::g3sg1, sizeof(icons::g3sg1), 93, 32);

        populate_data(EIconType::MP9, icons::mp9, sizeof(icons::mp9), 74, 32);
        populate_data(EIconType::MP7, icons::mp7, sizeof(icons::mp7), 49, 32);
        populate_data(EIconType::MP5, icons::mp5sd, sizeof(icons::mp5sd), 93, 39);
        populate_data(EIconType::P90, icons::p90, sizeof(icons::p90), 68, 32);
        populate_data(EIconType::UMP45, icons::ump45, sizeof(icons::ump45), 84, 32);
        populate_data(EIconType::MAC10, icons::mac10, sizeof(icons::mac10), 45, 32);
        populate_data(EIconType::PPBIZON, icons::bizon, sizeof(icons::bizon), 90, 32);

        populate_data(EIconType::NOVA, icons::nova, sizeof(icons::nova), 100, 32);
        populate_data(EIconType::XM1014, icons::xm1014, sizeof(icons::xm1014), 98, 32);
        populate_data(EIconType::MAG7, icons::mag7, sizeof(icons::mag7), 68, 32);
        populate_data(EIconType::SAWEDOFF, icons::sawedoff, sizeof(icons::sawedoff), 85, 32);
        populate_data(EIconType::NEGEV, icons::negev, sizeof(icons::negev), 81, 32);
        populate_data(EIconType::M249, icons::m249, sizeof(icons::m249), 97, 32);

        populate_data(EIconType::USP, icons::usp_silencer, sizeof(icons::usp_silencer), 69, 32);
        populate_data(EIconType::P2000, icons::p2000, sizeof(icons::p2000), 38, 32);
        populate_data(EIconType::GLOCK, icons::glock, sizeof(icons::glock), 45, 32);
        populate_data(EIconType::P250, icons::p250, sizeof(icons::p250), 38, 32);
        populate_data(EIconType::FIVE_SEVEN, icons::fiveseven, sizeof(icons::fiveseven), 39, 32);
        populate_data(EIconType::CZ75, icons::cz75a, sizeof(icons::cz75a), 48, 32);
        populate_data(EIconType::TEC9, icons::tec9, sizeof(icons::tec9), 52, 32);
        populate_data(EIconType::DEAGLE, icons::deagle, sizeof(icons::deagle), 51, 32);
        populate_data(EIconType::REVOLVER, icons::revolver, sizeof(icons::revolver), 53, 32);
        populate_data(EIconType::ELITE, icons::elite, sizeof(icons::elite), 69, 32);
        populate_data(EIconType::ZEUS27, icons::zeus27, sizeof(icons::zeus27), 42, 32);

        populate_data(EIconType::KNIFE_CT, icons::knife, sizeof(icons::knife), 77, 32);
        populate_data(EIconType::KNIFE_T, icons::knife_t, sizeof(icons::knife_t), 79, 32);
        populate_data(EIconType::KNIFE_GG, icons::knife_gg, sizeof(icons::knife_gg), 96, 32);
        populate_data(EIconType::KNIFE_BAYONET, icons::knife_bayonet, sizeof(icons::knife_bayonet), 69, 32);
        populate_data(EIconType::KNIFE_M9_BAYONET, icons::knife_m9_bayonet, sizeof(icons::knife_m9_bayonet), 87, 32);
        populate_data(EIconType::KNIFE_SHADOW_DAGGERS, icons::knife_shadow_daggers, sizeof(icons::knife_shadow_daggers), 83, 32);
        populate_data(EIconType::KNIFE_BUTTERFLY, icons::knife_butterfly, sizeof(icons::knife_butterfly), 80, 32);
        populate_data(EIconType::KNIFE_NOMAD, icons::knife_nomad, sizeof(icons::knife_nomad), 92, 32);
        populate_data(EIconType::KNIFE_KARAMBIT, icons::knife_karambit, sizeof(icons::knife_karambit), 66, 32);
        populate_data(EIconType::KNIFE_SURVIVAL, icons::knife_survival, sizeof(icons::knife_survival), 92, 32);
        populate_data(EIconType::KNIFE_SKELETON, icons::knife_skeleton, sizeof(icons::knife_skeleton), 92, 32);
        populate_data(EIconType::KNIFE_HUNTSMAN, icons::knife_huntsman, sizeof(icons::knife_huntsman), 96, 32);
        populate_data(EIconType::KNIFE_SURVIVAL_BOWIE, icons::knife_survival_bowie, sizeof(icons::knife_survival_bowie), 61, 32);
        populate_data(EIconType::KNIFE_TALON, icons::knife_talon, sizeof(icons::knife_talon), 92, 33);
        populate_data(EIconType::KNIFE_GUT, icons::knife_gut, sizeof(icons::knife_gut), 72, 32);
        populate_data(EIconType::KNIFE_CLASSIC, icons::knife_classic, sizeof(icons::knife_classic), 92, 32);

        populate_data(EIconType::KNIFE_URSUS, icons::knife_ursus, sizeof(icons::knife_ursus), 92, 32);
        populate_data(EIconType::KNIFE_PARACORD, icons::knife_paracord, sizeof(icons::knife_paracord), 91, 33);
        populate_data(EIconType::KNIFE_FLIP, icons::knife_flip, sizeof(icons::knife_flip), 79, 32);
        populate_data(EIconType::KNIFE_STILETTO, icons::knife_stiletto, sizeof(icons::knife_stiletto), 92, 32);
        populate_data(EIconType::KNIFE_FALCHION, icons::knife_falchion, sizeof(icons::knife_falchion), 89, 32);
        populate_data(EIconType::KNIFE_NAVAJA, icons::knife_navaja, sizeof(icons::knife_navaja), 92, 32);

        //populate_data(EIconType::KNIFE_KUKRI, icons::knife_navaja, sizeof(icons::knife_navaja), 92, 32);
    }

    EIconType get_icon_type_by_weapon_index(uint16_t index)
    {
        static std::unordered_map<uint16_t, EIconType> map =
        {
            { WEAPON_AK47, EIconType::AK47 },
            { WEAPON_M4A4, EIconType::M4A4 },
            { WEAPON_M4A1_S, EIconType::M4A1S },
            { WEAPON_FAMAS, EIconType::FAMAS },
            { WEAPON_GALIL, EIconType::GALIL },
            { WEAPON_SG556, EIconType::SG556 },
            { WEAPON_AUG, EIconType::AUG },
            { WEAPON_SSG08, EIconType::SSG08 },
            { WEAPON_AWP, EIconType::AWP },
            { WEAPON_G3SG1, EIconType::G3SG1 },
            { WEAPON_SCAR20, EIconType::SCAR20 },

            { WEAPON_MP9, EIconType::MP9 },
            { WEAPON_MP7, EIconType::MP7 },
            { WEAPON_MP5SD, EIconType::MP5 },
            { WEAPON_P90, EIconType::P90 },
            { WEAPON_MAC10, EIconType::MAC10 },
            { WEAPON_BIZON, EIconType::PPBIZON },
            { WEAPON_UMP, EIconType::UMP45 },

            { WEAPON_NOVA, EIconType::NOVA },
            { WEAPON_XM1014, EIconType::XM1014 },
            { WEAPON_MAG7, EIconType::MAG7 },
            { WEAPON_M249, EIconType::M249 },
            { WEAPON_NEGEV, EIconType::NEGEV },
            { WEAPON_SAWEDOFF, EIconType::SAWEDOFF },

            { WEAPON_GLOCK, EIconType::GLOCK },
            { WEAPON_USP_S, EIconType::USP },
            { WEAPON_HKP2000, EIconType::P2000 },
            { WEAPON_P250, EIconType::P250 },
            { WEAPON_CZ75, EIconType::CZ75 },
            { WEAPON_DEAGLE, EIconType::DEAGLE },
            { WEAPON_TEC9, EIconType::TEC9 },
            { WEAPON_ELITE, EIconType::ELITE },
            { WEAPON_FIVE_SEVEN, EIconType::FIVE_SEVEN },
            { WEAPON_REVOLVER, EIconType::REVOLVER },
            
            { WEAPON_SMOKE, EIconType::SMOKE },
            { WEAPON_FLASHBANG, EIconType::FLASHBANG },
            { WEAPON_MOLOTOV, EIconType::MOLOTOV },
            { WEAPON_INCENDIARY, EIconType::MOLOTOV },
            { WEAPON_HEGRENADE, EIconType::HEGRENADE },
            { WEAPON_DECOY, EIconType::DECOY },

            { WEAPON_HEALTHSHOT, EIconType::HEALTHSHOT },
            { WEAPON_TASER, EIconType::ZEUS27 },
            { WEAPON_C4, EIconType::C4 },
            
            { WEAPON_KNIFE, EIconType::KNIFE_CT },
            { WEAPON_KNIFE_T, EIconType::KNIFE_T },
            { WEAPON_KNIFE_GG, EIconType::KNIFE_GG },
            { WEAPON_BAYONET, EIconType::KNIFE_BAYONET },
            { WEAPON_KNIFE_M9_BAYONET, EIconType::KNIFE_M9_BAYONET },
            { WEAPON_KNIFE_CLASSIC, EIconType::KNIFE_CLASSIC },
            { WEAPON_KNIFE_SHADOW_DAGGERS, EIconType::KNIFE_SHADOW_DAGGERS },
            { WEAPON_KNIFE_GUT, EIconType::KNIFE_GUT },
            { WEAPON_KNIFE_FLIP, EIconType::KNIFE_FLIP },
            { WEAPON_KNIFE_URSUS, EIconType::KNIFE_URSUS },
            { WEAPON_KNIFE_FALCHION, EIconType::KNIFE_FALCHION },
            { WEAPON_KNIFE_KARAMBIT, EIconType::KNIFE_KARAMBIT },
            { WEAPON_KNIFE_STILETTO, EIconType::KNIFE_STILETTO },
            { WEAPON_KNIFE_SKELETON, EIconType::KNIFE_SKELETON },
            { WEAPON_KNIFE_BUTTERFLY, EIconType::KNIFE_BUTTERFLY },
            { WEAPON_KNIFE_SURVIVAL_BOWIE, EIconType::KNIFE_SURVIVAL_BOWIE },
            { WEAPON_KNIFE_NAVAJA, EIconType::KNIFE_NAVAJA },
            { WEAPON_KNIFE_TALON, EIconType::KNIFE_TALON },
            { WEAPON_KNIFE_NOMAD, EIconType::KNIFE_NOMAD },
            { WEAPON_KNIFE_HUNTSMAN, EIconType::KNIFE_HUNTSMAN },
            { WEAPON_KNIFE_SURVIVAL, EIconType::KNIFE_SURVIVAL },
            { WEAPON_KNIFE_PARACORD, EIconType::KNIFE_PARACORD }
        };

        auto it = map.find(index);
        if (it != map.end())
            return it->second;

        return EIconType::UNKNOWN;
    }
    
    icon_t get(const EIconType& type)
    {
        return icon_data[type];
    }
}
