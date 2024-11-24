#pragma once
#include "../../sdk/helpers/vfunc.h"

class CGameType
{
public:
	bool IsDeathmatch()
	{
		return GetGameType() == 1 && GetGameMode() == 2;
	}

	bool IsCasual()
	{
		return GetGameType() == 0 && GetGameMode() == 0;
	}

	bool IsCompetitive()
	{
		return GetGameType() == 0 && GetGameMode() == 1;
	}

	bool IsWingman()
	{
		return GetGameType() == 0 && GetGameMode() == 2;
	}

	bool IsArmsrace()
	{
		return GetGameType() == 1 && GetGameMode() == 0;
	}

	bool IsMatchmaking()
	{
		return IsCasual() || IsCompetitive();
	}
private:
	int GetGameType()
	{
		return VTable::GetThiscall<int>(this, 19);
	}

	int GetGameMode()
	{
		return VTable::GetThiscall<int>(this, 20);
	}
};
