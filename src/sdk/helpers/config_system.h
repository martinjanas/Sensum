#pragma once
#include "json.hpp"
#include <map>

namespace config_system
{
    static inline std::map<int, std::string> weapon_names = 
    {
        {1, "DEAGLE"},
        {2, "ELITE"},
        {3, "FIVE_SEVEN"},
        {4, "GLOCK"},
        {7, "AK47"},
        {8, "AUG"},
        {9, "AWP"},
        {10, "FAMAS"},
        {11, "G3SG1"},
        {13, "GALIL"},
        {14, "M249"},
        {16, "M4A4"},
        {17, "MAC10"},
        {19, "P90"},
        {23, "MP5"},
        {24, "UMP"},
        {25, "XM1024"},
        {26, "BIZON"},
        {27, "MAG7"},
        {28, "NEGEV"},
        {29, "SAWEDOFF"},
        {30, "TEC9"},
        {32, "P2000"},
        {33, "MP7"},
        {34, "MP9"},
        {35, "NOVA"},
        {36, "P250"},
        {38, "SCAR20"},
        {39, "SG553"},
        {40, "SSG08"},
        {60, "M4A1S"},
        {61, "USP"},
        {63, "CZ75"},
        {64, "R8"}
    };

    static inline std::map<std::string, int> weapon_ids =
    {
        {"DEAGLE", 1},
        {"ELITE", 2},
        {"FIVE_SEVEN", 3},
        {"GLOCK", 4},
        {"AK47", 7},
        {"AUG", 8},
        {"AWP", 9},
        {"FAMAS", 10},
        {"G3SG1", 11},
        {"GALIL", 13},
        {"M249", 14},
        {"M4A4", 16},
        {"MAC10", 17},
        {"P90", 19},
        {"MP5", 23},
        {"UMP", 24},
        {"XM1024", 25},
        {"BIZON", 26},
        {"MAG7", 27},
        {"NEGEV", 28},
        {"SAWEDOFF", 29},
        {"TEC9", 30},
        {"P2000", 32},
        {"MP7", 33},
        {"MP9", 34},
        {"NOVA", 35},
        {"P250", 36},
        {"SCAR20", 38},
        {"SG553", 39},
        {"SSG08", 40},
        {"M4A1S", 60},
        {"USP", 61},
        {"CZ75", 63},
        {"R8", 64}
    };

	void load();
	void save();
}

