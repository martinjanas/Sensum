#pragma once

#include <Windows.h>

class DemoPlaybackParameters 
{
public:
	char pad_01[16];
	bool IsPlayerIdentityAnonymous;
	char pad_02[23];
};

class CDemoFile 
{
public:
	char* GetHeaderFile() 
	{
		return reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(this) + 0x108);
	}
};

class IDemoPlayer 
{
public:
	CDemoFile* GetDemoFile() 
	{
		return *reinterpret_cast<CDemoFile**>(reinterpret_cast<uintptr_t>(this) + 0x4);
	}

	DemoPlaybackParameters* GetPlaybackParameters() 
	{
		return *reinterpret_cast<DemoPlaybackParameters**>(reinterpret_cast<uintptr_t>(this) + 0x5C8);
	}

	bool IsInOverwatch() 
	{
		auto playback_parameter = GetPlaybackParameters();

		return (playback_parameter && playback_parameter->IsPlayerIdentityAnonymous);
	}

	void SetOverwatchState(bool state) 
	{
		auto playback_parameter = GetPlaybackParameters();

		if (playback_parameter) 
		{
			playback_parameter->IsPlayerIdentityAnonymous = state;
		}
	}

	bool IsPlayingDemo() 
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 4)(this);
	}

	bool IsPlayingTimeDemo() 
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 6)(this);
	}
};