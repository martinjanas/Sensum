#pragma once

#include "../math/QAngle.hpp"
#include "../misc/CUserCmd.hpp"
#include "IMoveHelper.hpp"

class CMoveData {
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	unsigned long	m_nPlayerHandle;
	int				m_nImpulseCommand;
	QAngle			m_vecViewAngles;
	QAngle			m_vecAbsViewAngles;
	int				m_nButtons;
	int				m_nOldButtons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	Vector			m_vecVelocity;
	Vector			m_vecOldVelocity;
	float			somefloat;
	QAngle			m_vecAngles;
	QAngle			m_vecOldAngles;

	float			m_outStepHeight;
	Vector			m_outWishVel;
	Vector			m_outJumpVel;

	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;

	void			SetAbsOrigin(const Vector& vec);
	const Vector&	GetAbsOrigin() const;

private:
	Vector			m_vecAbsOrigin;		// edict::origin
};

class c_base_player;

class IGameMovement {
public:
	virtual			~IGameMovement(void) {}

	virtual void	ProcessMovement(c_base_player* pPlayer, CMoveData* pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(c_base_player* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(c_base_player* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;

	virtual Vector const& GetPlayerMins(bool ducked) const = 0;
	virtual Vector const& GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const& GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual c_base_player*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(c_base_player* pPlayer, c_base_player* pPusher) = 0;

	virtual void SetupMovementBounds(CMoveData* pMove) = 0;
};

class CGameMovement
	: public IGameMovement
{
public:
	virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
	void SetLocalViewAngles(QAngle& va)
	{
		CallVFunction<void(__thiscall*)(void*, QAngle&)>(this, 13)(this, va);
	}

	bool InPrediction()
	{
		typedef bool(__thiscall * oInPrediction)(void*);
		return CallVFunction<oInPrediction>(this, 14)(this);
	}

	void RunCommand(c_base_player* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
	{
		typedef void(__thiscall * oRunCommand)(void*, c_base_player*, CUserCmd*, IMoveHelper*);
		return CallVFunction<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(c_base_player* player, CUserCmd* ucmd, IMoveHelper* moveHelper, void* pMoveData)
	{
		typedef void(__thiscall * oSetupMove)(void*, c_base_player*, CUserCmd*, IMoveHelper*, void*);
		return CallVFunction<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(c_base_player* player, CUserCmd* ucmd, void* pMoveData)
	{
		typedef void(__thiscall * oFinishMove)(void*, c_base_player*, CUserCmd*, void*);
		return CallVFunction<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};