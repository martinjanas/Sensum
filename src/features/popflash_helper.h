#pragma once
#include <string>
#include "../valve_sdk/csgostructs.hpp"
#include "../features/esp.hpp"
#include "../features/features.h"
#include "../settings/globals.h"

struct popflash_data_t
{
	std::string info;
	std::string type;
	Vector pos;
	Vector ang;
	float ring_height;
};

std::vector<popflash_data_t> popflash_data_mirage = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f },
	{ "Popflash", "L + R", Vector(-496.03f, -1309.13f, -95.90f), Vector(-26.32f, -89.92f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(-2318.40f, -695.97f, -103.90f), Vector(-21.92f, 87.76f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(-197.42f, -344.03f, -99.90f), Vector(-52.44f, -176.65f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(399.922f, 55.97f, -149.65f), Vector(-47.07f, -155.03f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(-1017.02f, -2232.03f, -48.84f), Vector(-75.05f, 73.20f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(-1471.96f, 664.03f, 16.09f), Vector(-7.69f, 109.26f, 0.f), 60.f },
	{ "Popflash", "RUN + L", Vector(-1372.03f, -1095.96f, -103.90f), Vector(-15.90f, 171.88f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(-2199.78f, 88.03f, -95.90f), Vector(-24.23f, 7.79f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_inferno = {

	{ "Popflash", "LClick", Vector(1275.96f, -111.96f, 320.09f), Vector(7.75f, 116.85f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(2150.16f, 313.98f, 224.09f), Vector(-38.77f, -156.95f, 0.f), 60.f },
	{ "Popflash", "RClick", Vector(1821.03f, 184.03f, 286.09f), Vector(-49.09f, 43.95f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(1764.70f, 1303.34f, 224.08f), Vector(-46.25f, 61.56f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(2194.03f, 955.03f, 224.46f), Vector(-24.79f, -104.66f, 0.f), 60.f },
	{ "Popflash", "LClick", Vector(2485.03f, 1311.96f, 224.09f), Vector(-0.74f, -23.81f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_dust2 = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_nuke = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_overpass = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_train = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_cache = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};

std::vector<popflash_data_t> popflash_data_cobblestone = {

	{ "Popflash", "LClick", Vector(150.96f, -1914.03f, -103.90f), Vector(-38.69f, -111.75f, 0.f), 60.f }
};
