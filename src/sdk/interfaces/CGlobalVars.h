#pragma once
#include <cstdint>

class CGlobalVarsBase
{
public:
	float m_realtime; //0x0000
	int32_t m_framecount; //0x0004
	float m_frametime; //0x0008 //returns 0
	float m_abs_frametime; //0x000C //returns 0
	int32_t m_maxclients; //0x0010
	char pad_0014[28]; //0x0014
	float m_frametime2; //0x0030
	float m_curtime; //0x0034 //outputs valid data
	float m_curtime2; //0x0038 //outputs valid data, same data as m_curtime
	char pad_003C[20]; //0x003C
	int32_t m_tickcount; //0x0050 //outputs garbage data
	char pad_0054[292]; //0x0054
	uint64_t m_current_map; //0x0178
	uint64_t m_current_mapname; //0x0180
}; //Size: 0x0188
