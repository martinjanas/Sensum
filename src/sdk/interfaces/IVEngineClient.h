#pragma once

#include "../helpers/vfunc.h"

enum EClientFrameStage : int
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    // a network packet is being received
    FRAME_NET_UPDATE_START,
    // data has been received and we are going to start calling postdataupdate
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    // data has been received and called postdataupdate on all data recipients
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    // received all packets, we can now do interpolation, prediction, etc
    FRAME_NET_UPDATE_END,
    // start rendering the scene
    FRAME_RENDER_START,
    // finished rendering the scene
    FRAME_RENDER_END,
    FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};


//class IEngineClient
//{
//public:
//	[[nodiscard]] int GetMaxClients()
//	{
//		return Memory::CallVFunc<int, 34U>(this);
//	}
//
//	[[nodiscard]] bool IsInGame()
//	{
//		return Memory::CallVFunc<bool, 35U>(this);
//	}
//
//	[[nodiscard]] bool IsConnected()
//	{
//		return Memory::CallVFunc<bool, 36U>(this);
//	}
//
//	[[nodiscard]] CNetChannelInfo* GetNetChannelInfo(int nSplitScreenSlot = 0)
//	{
//		return Memory::CallVFunc<CNetChannelInfo*, 37U>(this, nSplitScreenSlot);
//	}
//
//	void ExecuteClientCmdUnrestricted(const char* szCmdString)
//	{
//		Memory::CallVFunc<void, 43U>(this, 0, szCmdString, 0x7FFEF001);
//	}
//
//	// return CBaseHandle index
//	[[nodiscard]] int GetLocalPlayer()
//	{
//		int nIndex = -1;
//
//		Memory::CallVFunc<void, 47U>(this, std::ref(nIndex), 0);
//
//		return nIndex + 1;
//	}
//
//	void GetScreenSize(int& iWidth, int& iHeight)
//	{
//		Memory::CallVFunc<void, 53U>(this, std::ref(iWidth), std::ref(iHeight));
//	}
//
//	[[nodiscard]] const char* GetLevelName()
//	{
//		return Memory::CallVFunc<const char*, 56U>(this);
//	}
//
//	[[nodiscard]] const char* GetLevelNameShort()
//	{
//		return Memory::CallVFunc<const char*, 57U>(this);
//	}
//
//	[[nodiscard]] const char* GetProductVersionString()
//	{
//		return Memory::CallVFunc<const char*, 81U>(this);
//	}
//}; //

class IVEngineClient
{
public:
    bool IsPaused() //not returning correct values
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 11)(this); //not 10 11 12 13   15 16 17 18
    }

    int GetMaxClients()
    {
        return GetVirtual<int(__thiscall*)(void*)>(this, 34)(this); //31 -> 34
    }

    bool IsInGame()
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 35)(this);
    }

    bool IsConnected()
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 36)(this);
    }

    void GetScreenSize(int& width, int& height)
    {
        GetVirtual<void(__thiscall*)(void*, int&, int&)>(this, 53)(this, width, height);
    }

    const char* GetLevelName()
    {
        return GetVirtual<const char* (__thiscall*)(void*)>(this, 56)(this);
    }

    const char* GetLevelNameShort()
    {
        return GetVirtual<const char* (__thiscall*)(void*)>(this, 57)(this);
    }
};