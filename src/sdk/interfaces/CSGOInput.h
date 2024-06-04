#pragma once
#define NOMINMAX
#undef max
#include <Windows.h>
#include <stdint.h>
#include <limits>
#include "../math/Vector.h"

#define MULTIPLAYER_BACKUP 150

enum ECommandButtons : int
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_SECOND_ATTACK = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_LEFT_ALT = (1 << 14),
	IN_RIGHT_ALT = (1 << 15),
	IN_SCORE = (1 << 16),
	IN_SPEED = (1 << 17),
	IN_WALK = (1 << 18),
	IN_ZOOM = (1 << 19),
	IN_FIRST_WEAPON = (1 << 20),
	IN_SECOND_WEAPON = (1 << 21),
	IN_BULLRUSH = (1 << 22),
	IN_FIRST_GRENADE = (1 << 23),
	IN_SECOND_GRENADE = (1 << 24),
	IN_MIDDLE_ATTACK = (1 << 25),
	IN_USE_OR_RELOAD = (1 << 26)
};

template <typename T>
struct RepeatedPtrField_t
{
	struct Rep_t
	{
		int nAllocatedSize;
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};

class CBasePB
{
	void* pVTable; // 0x0
	std::uint32_t nHasBits; // 0x8
	std::uint64_t nCachedBits; // 0xC
};
static_assert(sizeof(CBasePB) == 0x18);

class CMsgQAngle : public CBasePB
{
public:
	QAngle angValue; // 0x18
};

class CMsgVector : public CBasePB
{
public:
	void* vecValue; // Vector4D_t 0x18
};

class CCSGOInterpolationInfoPB : public CBasePB
{
public:
	float m_flFraction; // 0x18
	int m_nSrcTick; // 0x1C
	int m_nDstTick; // 0x20
};
static_assert(sizeof(CCSGOInterpolationInfoPB) == 0x28);

// credits: @patoke [uc:3872928-post1311]
class CCSGOInputHistoryEntryPB : public CBasePB
{
public:
	CMsgQAngle* m_pViewAngles; // 0x18
	CMsgVector* m_pShootPosition; // 0x20
	CMsgVector* m_pTargetHeadPositionCheck; // 0x28
	CMsgVector* m_pTargetAbsPositionCheck; // 0x30
	CMsgQAngle* m_pTargetAngPositionCheck; // 0x38
	CCSGOInterpolationInfoPB* m_cl_interp; // 0x40
	CCSGOInterpolationInfoPB* m_sv_interp0; // 0x48
	CCSGOInterpolationInfoPB* m_sv_interp1; // 0x50
	CCSGOInterpolationInfoPB* m_player_interp; // 0x58
	int m_nRenderTickCount; // 0x60
	float m_flRenderTickFraction; // 0x64
	int m_nPlayerTickCount; // 0x68
	float m_flPlayerTickFraction; // 0x6C
	int m_nFrameNumber; // 0x70
	int m_nTargetEntIndex; // 0x74
};
static_assert(sizeof(CCSGOInputHistoryEntryPB) == 0x78);

struct CSubtickMoveStep : CBasePB
{
public:
	uint64_t m_nButton;
	bool m_bPressed;
	float m_flWhen;
	float m_flAnalogForwardDelta;
	float m_flAnalogLeftDelta;
};
static_assert(sizeof(CSubtickMoveStep) == 0x30);

struct CInButtonStatePB : CBasePB
{
	uint64_t m_nValue;
	uint64_t m_nValueChanged;
	uint64_t m_nValueScroll;
};
static_assert(sizeof(CInButtonStatePB) == 0x30);

class CBaseUserCmdPB : public CBasePB
{
public:
	RepeatedPtrField_t<CSubtickMoveStep> m_subtickMovesField;
	const char* m_szMoveCrc;
	CInButtonStatePB* m_pInButtonState;
	CMsgQAngle* m_pViewAngles;
	int32_t m_nCommandNumber;
	int32_t m_nTickCount;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	int32_t m_nImpulse;
	int32_t m_nWeaponSelect;
	int32_t m_nRandomSeed;
	int32_t m_nMousedX;
	int32_t m_nMousedY;
	uint32_t m_nConsumedServerAngleChanges;
	int32_t m_nCmdFlags;
	uint32_t m_nPawnEntityHandle;
};
static_assert(sizeof(CBaseUserCmdPB) == 0x80);

class CCSGOUserCmdPB
{
public:
	std::uint32_t m_nHasBits;
	std::uint64_t m_nCachedSize;
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> m_inputHistoryField;
	CBaseUserCmdPB* m_pBaseCmd;
	bool m_bLeftHandDesired; // new var
	int32_t m_nAttack3StartHistoryIndex;
	int32_t m_nAttack1StartHistoryIndex;
	int32_t m_nAttack2StartHistoryIndex;
};
static_assert(sizeof(CCSGOUserCmdPB) == 0x40);

// not protobufs
struct CInButtonState
{
	void* pVTable; // 0x0
	uint64_t m_nValue; // 0x8
	uint64_t m_nValueChanged; // 0x10
	uint64_t m_nValueScroll; // 0x18
};

class CUserCmd
{
public:
	void* pVTable;
	CCSGOUserCmdPB cmd;
	CInButtonState buttonStates;
	std::byte pad[0x20]; //pad[0x20] //was pad[48]
};

class CSGOInput
{
public:
	void* pVtable;
	std::byte pad001[0x248];
	CUserCmd arrCommands[150];
	std::byte pad002[0x1];
	bool bInThirdPerson;
	std::byte pad003[0x22];
	int32_t nSequenceNumber;

	CUserCmd* GetUserCmd()
	{
		return &arrCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
	}

	static CSGOInput* get()
	{
		static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 48 8B 56 60", "CSGOInput::Get()").add(0x1).abs().as();
		
		const auto& get_input = reinterpret_cast<CSGOInput*(__thiscall*)()>(addr);
		
		if (get_input)
			return get_input();
		
		return nullptr;
	}
};
