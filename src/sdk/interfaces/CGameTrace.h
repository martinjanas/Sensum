#pragma once

#include "../../sdk/math/Vector.h"
#include "../../sdk/math/QAngle.h"
#include "../../sdk/classes/CBaseEntity.h"
//#include "../../sdk/classes/CCSPlayerPawn.h"

class Ray_t;
class TraceFilter_t;
class Trace_t; //aka GameTrace_t

struct Ray_t
{
	Vector start;
	Vector end;
	Vector mins;
	Vector maxs;
	std::byte pad01[0x4];
	uint8_t unknown;
};

struct TraceHitboxData_t
{
	std::byte pad01[0x38];
	int m_nHitGroup;
	std::byte pad02[0x4];
	int m_nHitboxId;
};

struct SurfaceData_t
{
	std::byte pad01[0x8];
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	std::byte pad02[0x4];
	int m_iMaterial;
};

struct Trace_t
{
	Trace_t() = default;
	SurfaceData_t* GetSurfaceData();

	int GetHitboxID();
	int GetHitgroup();
	bool DidHitWorld() const;

	void* m_pSurface;
	CBaseEntity* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	std::byte pad01[0x38];
	uint32_t m_uContents;
	std::byte pad02[0x24];
	Vector m_vecStartPos;
	Vector m_vecEndPos;
	Vector m_vecNormal;
	Vector m_vecPosition;
	std::byte pad03[0x4];
	float m_flFraction;
	std::byte pad04[0x6];
	bool m_bAllSolid;
	std::byte pad05[0x4D];
};

struct TraceFilter_t
{
	std::byte pad01[0x8];
	std::int64_t m_uTraceMask;
	std::array< std::int64_t, 2 > m_v1;
	std::array< std::int32_t, 4 > m_arrSkipHandles;
	std::array< std::int16_t, 2 > m_arrCollisions;
	std::int16_t m_v2;
	std::uint8_t m_v3;
	std::uint8_t m_v4;
	std::uint8_t m_v5;

	TraceFilter_t() = default;
	//TraceFilter_t(std::uint32_t uMask, CCSPlayerPawn* pSkip1, CCSPlayerPawn* pSkip2, int nLayer);
	//{
	//	m_uTraceMask = uMask;
	//	m_v1[0] = m_v1[1] = 0;
	//	m_v2 = 7;
	//	m_v3 = nLayer;
	//	m_v4 = 0x49;
	//	m_v5 = 0;

	//	m_arrSkipHandles[0] = pSkip1->GetEntityHandle();
	//	m_arrSkipHandles[1] = pSkip2->GetEntityHandle();
	//	m_arrSkipHandles[2] = pSkip1->GetOwnerHandle();
	//	m_arrSkipHandles[3] = pSkip2->GetOwnerHandle();

	//	/*m_arrCollisions[0] = (pSkip1->);
	//	m_arrCollisions[1] = pSkip2->GetCollisionMask();*/
	//}
};

class CGameTrace
{
public:
	bool TraceShape(Ray_t* ray, const Vector& start, const Vector& end, TraceFilter_t* filter, Trace_t* trace)
	{
		using fn = bool(__fastcall*)(void*, Ray_t*, const Vector&, const Vector&, TraceFilter_t*, Trace_t*); //fastcall

		//48 89 54 24 10 48 89 4C 24 08 55 53 56 57 41 54 41 56 41 57 48 8D AC
		static const auto& addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 45").add(0x1).abs().as();

		auto trace_shape = reinterpret_cast<fn>(addr);
		if (trace_shape)
			return trace_shape(this, ray, start, end, filter, trace);

		return false;
	}
};