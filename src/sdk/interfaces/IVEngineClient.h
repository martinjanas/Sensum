#pragma once
#include <functional>
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

class IVEngineClient
{
public:
    bool IsPaused() //not returning correct values
    {
        return VTable::GetThiscall<bool>(this, 11);
    }

    int GetMaxClients()
    {
        return VTable::GetThiscall<int>(this, 34);
    }

    bool IsInGame()
    {
        return VTable::GetThiscall<bool>(this, 37); //was 35
    }

    bool IsConnected()
    {
        return VTable::GetThiscall<bool>(this, 38); //was 36
    }

    void ExecuteClientCmd(const char* cmd)
    {
        VTable::GetThiscall<void>(this, 47, 0, cmd, 0x7FFEF001); //was 45
    }

    int GetLocalPlayer()
    {
        int index = -1;
        VTable::GetThiscall<void>(this, 51, std::ref(index), 0); //was 49

        return index + 1;
    }

    void GetScreenSize(int& width, int& height)
    {
        VTable::GetThiscall<void>(this, 53, std::ref(width), std::ref(height)); //55 if returning nonsense values; Edit 2024-08-07: 57?
    }

    const char* GetLevelName()
    {
        return VTable::GetThiscall<const char*>(this, 60); //was 58
    }

    const char* GetLevelNameShort()
    {
        return VTable::GetThiscall<const char*>(this, 61); //was 59
    }
};