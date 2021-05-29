#pragma once
#include <string>
#include "../valve_sdk/csgostructs.hpp"
#include "../features/esp.hpp"
#include "../features/features.h"
#include "../settings/globals.h"

struct hegrenade_data_t
{
	std::string info;
	std::string type;
	Vector pos;
	Vector ang;
	float ring_height;
};

std::vector<hegrenade_data_t> hegrenade_data_cs_agency = {
	{ "Table", "RUN", Vector(-1123.9649658203f, -322.65048217773f, 512.03125f), Vector(-10.129358520508f, 59.272856903076f, 0.f), 0.f },
	{ "Front Hall", "RUN", Vector(-956.72924804688f, 240.17779541016f, 320.03125f), Vector(-28.11051361084f, 47.477611541748f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_cs_italy = {
	{ "Wine Cellar", "RUN", Vector(-835.4000854492188f, 1204.1595458984375f, 0.03125f), Vector(-2.1563305854797363f, 63.26720428466797f, 0.f), 0.f },
	{ "Crouch", "RUN", Vector(855.8480834960938f, 1609.6368408203125f, -33.97166061f), Vector(-4.243151664733887f, 128.7999835205078f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_cs_office = {
	{ "Trash", "RUN", Vector(1013.5485229492f, -667.89953613281f, -159.96875f), Vector(-2.577028274536f, 97.964347839355f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_aztec = {
	{ "Stone ", "RUN", Vector(-687.39367675781f, -790.27362060547f, -376.3038635f), Vector(-20.45291519165f, 113.42259979248f, 0.f), 0.f },
	{ "Stone", "RUN", Vector(-701.8319702148438f, -327.07470703125f, -223.96875f), Vector(-9.834869f, 162.928897f, 0.f), 0.f },
	{ "Stone ", "LCLICK", Vector(-918.66717529297f, -623.76849365234f, -352.3038635f), Vector(-26.354013442993f, 99.820770263672f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_aztec_ht = {
	{ "Stone ", "RUN", Vector(-687.39367675781f, -790.27362060547f, -376.3038635f), Vector(-20.45291519165f, 113.42259979248f, 0.f), 0.f },
	{ "Stone", "RUN", Vector(-701.8319702148438f, -327.07470703125f, -223.96875f), Vector(-9.834869f, 162.928897f, 0.f), 0.f },
	{ "Stone ", "LCLICK", Vector(-918.66717529297f, -623.76849365234f, -352.3038635f), Vector(-26.354013442993f, 99.820770263672f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_bank = {
	{ "White car", "RUN", Vector(-444.82104492188f, -395.07797241211f, 188.2144775f), Vector(-2.1272838115692f, 1.4907883405685f, 0.f), 0.f },
	{ "White car", "RUN", Vector(-532.20855712891f, -243.56062316895f, 146.2144775f), Vector(-3.5713124275208f, -2.9621315002441f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_cache = {
	{ "CT", "RUN", Vector(592.7044677734375f, 515.6970825195313f, 1613.03125f), Vector(-15.962537994384766f, -163.76670532226563f, 0.f), 0.f },
	{ "CT", "RUN", Vector(624.07080078125f, 417.5489196777344f, 1613.031128f), Vector(-16.049680786132813f, -167.93629150390625f, 0.f), 0.f },
	{ "Barrels Crouch", "RUN & JUMP", Vector(85.21552276611328f, 159.5667724609375f, 1613.03125f), Vector(12.351604461669922f, 111.44610595703125f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_cbble = {
	{ "Fountain", "LCLICK", Vector(-64.657661437988f, -355.10583496094f, -31.96875f), Vector(-4.5250657081604f, -66.865000915527f, 0.f), 0.f },
	{ "Fountain", "RUN", Vector(281.586548f, -76.822685f, -12.31446457f), Vector(-2.7429327201843f, -94.529113769531f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_inferno = {
	{ "Mid", "RUN & JUMP", Vector(1643.802490234375f, 1214.0023193359375f, 161.2711945f), Vector(12.230835247039795f, -125.15312042236328f, 0.f), 0.f },
	{ "Onspot/Far Corner", "RUN", Vector(1733.215210f, -44.736149f, 130.0677338f), Vector(-12.087965965271f, 43.120056152344f, 0.f), 0.f },
	{ "A Box", "LCLICK", Vector(1813.5391845703f, 35.910182952881f, 128.0677338f), Vector(-11.584044456482f, 44.004806518555f, 0.f), 0.f },
	{ "A Site", "RUN", Vector(2037.94384765625f, 1109.1815185546875f, 156.6287689f), Vector(-16.537595558166504f, -37.23824005126953f, 0.f), 0.f },
	{ "A Box", "RUN", Vector(2076.50244140625f, 1166.2220458984375f, 163.5778198f), Vector(-10.779888153076172f, -58.55031967163086f, 0.f), 0.f },
	{ "Banana/Barrels", "RUN & JUMP", Vector(442.907470703125f, 2622.138916015625f, 160.093811f), Vector(-49.4201774597168f, -81.290771484375f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_mirage = {
	{ "Short", "RUN", Vector(-1033.08837890625f, -162.3196258544922f, -367.96875f), Vector(-10.437341690063477f, -76.62198638916016f, 0.f), 0.f },
	{ "A Plant", "RUN", Vector(78.44137573242188f, -2347.5078125f, -39.96875f), Vector(17.709062576293945f, 113.60216522216797f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_overpass = {
	{ "Barrels", "LCLICK", Vector(-1017.0237426757813f, -564.472900390625f, 96.03125f), Vector(-11.917777061462402f, 74.4395654296875f, 0.f), 0.f },
	{ "Pit", "RUN", Vector(-1045.7066650390625f, -563.4999389648438f, 96.03125f), Vector(-13.383971214294434f, 86.06208038330078f, 0.f), 0.f },
	{ "Barrels", "RUN & JUMP", Vector(-1543.564697f, -902.474121f, 51.74222565f), Vector(4.0240458488464f, 61.69671585083f, 0.f), 0.f },
	{ "Water", "RUN", Vector(-1580.9578857421875f, -1087.9552001953125f, 136.03125f), Vector(-8.577723503112793f, 76.9768295288086f, 0.f), 0.f },
	{ "Crouch", "LCLICK", Vector(-2441.76806640625f, 95.25958251953125f, 408.5286865f), Vector(-16.950772857666016f, 66.34442291259766f, 0.f), 0.f },
	{ "Toilet", "RUN", Vector(-2474.96875f, -1987.7607421875f, 514.569458f), Vector(-13.23123550415039f, 82.03817749023438f, 0.f), 0.f },
	{ "Short", "RUN & JUMP", Vector(-265.4296569824219f, -1014.823974609375f, 11.08824158f), Vector(-3.481962299346924f, 153.0970001220703f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_shortdust = {
	{ "Long", "RUN", Vector(-176.642471f, 1805.493286f, 32.09381104f), Vector(-6.5653779029846f, -45.643031311035f, 0.f), 0.f },
	{ "CT", "RUN & JUMP", Vector(-768.921936f, 795.664001f, 36.03125f), Vector(6.8205275535583f, -41.455672454834f, 0.f), 0.f },
	{ "Car", "RUN & JUMP", Vector(-910.35992431641f, 970.1240234375f, 35.09381104f), Vector(13.721249580383f, -35.259700775146f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_shortnuke = {
	{ "Ramp", "RUN", Vector(1336.028076171875f, -618.1556396484375f, -639.906189f), Vector(-5.354787826538086f, 149.40060424804688f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_stmarc = {
	{ "Car (Duck)", "LCLICK", Vector(-6840.3720703125f, 6924.4267578125f, 52.46635437f), Vector(-7.845247745513916f, 178.0292510986328f, 0.f), 0.f },
	{ "Crouch", "RUN", Vector(-8175.8369140625f, 6801.56640625f, 40.19894409f), Vector(-12.079442024230957f, 5.280690670013428f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_train = {
	{ "Heaven", "RUN & JUMP", Vector(-628.4265747070313f, 557.722900390625f, -215.96875f), Vector(-1.281444549560547f, -33.17278289794922f, 0.f), 0.f },
	{ "Stop Sign", "LCLICK", Vector(1294.3596191406f, 400.02493286133f, -215.96875f), Vector(-4.9444236755371f, 175.48867797852f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_tulip = {
	{ "Fountain", "RUN & JUMP", Vector(6068.712890625f, 2926.1301269531f, -63.53125f), Vector(14.135782089233f, -136.14909362793f, 0.f), 0.f },
	{ "Boost", "RUN", Vector(6262.509765625f, 4549.84912109375f, -123.96875f), Vector(-25.941469192504883f, -127.37826538085938f, 0.f), 0.f },
	{ "Boost", "RUN & JUMP", Vector(6299.3696289063f, 4425.8061523438f, -119.8841248f), Vector(3.4833682060242f, -138.59098815918f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_tulip_ht = {
	{ "Fountain", "RUN & JUMP", Vector(6068.712890625f, 2926.1301269531f, -63.53125f), Vector(14.135782089233f, -136.14909362793f, 0.f), 0.f },
	{ "Boost", "RUN", Vector(6262.509765625f, 4549.84912109375f, -123.96875f), Vector(-25.941469192504883f, -127.37826538085938f, 0.f), 0.f },
	{ "Boost", "RUN & JUMP", Vector(6299.3696289063f, 4425.8061523438f, -119.8841248f), Vector(3.4833682060242f, -138.59098815918f, 0.f), 0.f }
};

std::vector<hegrenade_data_t> hegrenade_data_de_vertigo = {
	{ "Crouch", "LCLICK", Vector(-1839.6301269531f, -214.72941589355f, 11776.6377f), Vector(-13.315687179565f, 66.154853820801f, 0.f), 0.f }
};