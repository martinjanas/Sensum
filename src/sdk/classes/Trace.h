#pragma once
#include "../math/Vector.h"
#include "../classes/CCSPlayerController.h"

struct Ray_t
{
	Vector m_vecStart;
	Vector m_vecEnd;
	Vector m_vecMins;
	Vector m_vecMaxs;
	std::byte pad001[0x4];
	uint8_t m_UnkType;
};

struct SurfaceData_t
{
	std::byte pad001[0x8];
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	std::byte pad002[0x4];
	int m_iMaterial;
};

struct TraceHitboxData_t
{
	std::byte pad001[0x58];
	int m_iHitgroup;
	std::byte pad002[0x4];
	int m_iHitboxID;
};

struct GameTrace_t
{
	GameTrace_t();
	SurfaceData_t* GetSurfaceData();

	int GetHitboxId()
	{
		if (m_pHitboxData)
			return m_pHitboxData->m_iHitboxID;
		return 0;
	}

	int GetHitgroup()
	{
		if (m_pHitboxData)
			return m_pHitboxData->m_iHitgroup;
		return 0;
	}

	void* m_pSurface;
	CCSPlayerController* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	std::byte pad001[0x10];
	uint32_t m_uiContents;
	std::byte pad002[0x58];
	Vector m_vecEndPos;
	std::byte pad003[0x1C];
	float m_flFraction;
	std::byte pad004[0x8];
}; // Size: 0xB8

class TraceFilter
{
public:
	uint64_t m_ui64lTraceMask;
	uint64_t m_ui64lV1[2];
	uint32_t m_ui32SkipHandles[4];
	uint16_t m_ui16Collisions[2];
	uint16_t m_ui16V2;
	uint8_t m_ui8V3;
	uint8_t m_ui8V4;
	uint8_t m_ui8V5;

	virtual ~TraceFilter() {}
	virtual bool ChinaNumberOne()
	{
		return true;
	}

	TraceFilter();
	TraceFilter(uint32_t mask, CCSPlayerController* controller_to_skip_one, CCSPlayerController* controller_to_skip_second, int layer);
};
