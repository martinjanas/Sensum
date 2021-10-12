#pragma once

#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientUnknown.hpp"
#include "IClientThinkable.hpp"

class CCSWeaponInfo
{
public:
	char pad_0000[4]; //0x0000
	char* szConsoleName; //0x0004
	char pad_0008[12]; //0x0008
	int32_t iMaxReservedAmmo; //0x0014
	char pad_0018[104]; //0x0018
	char* szBulletType; //0x0080
	char _unk[4];
	char* szHudName;
	char* _szHudName; //0x008C
	char pad_0090[56]; //0x0090
	int32_t WeaponType;
	int8_t pad4[4];
	int32_t iWeaponPrice;
	int32_t iKillAward;
	int8_t pad5[20];
	uint8_t bFullAuto;
	int8_t pad6[3];
	int32_t iDamage;
	float_t flArmorRatio;
	int32_t iBullets;
	float_t flPenetration;
	char pad_7[12]; //0x0100
	float flRange; //0x0108
	float flRangeModifier; //0x010C
}; //Size: 0x1040
