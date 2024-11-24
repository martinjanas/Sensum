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
        return VTable::GetThiscall<bool>(this, 35);
    }

    bool IsConnected()
    {
        return VTable::GetThiscall<bool>(this, 36);
    }

    int GetLocalPlayer()
    {
        int index = -1;
        VTable::GetThiscall<void>(this, 49, std::ref(index), 0);

        return index + 1;
    }

    void GetScreenSize(int& width, int& height)
    {
        VTable::GetThiscall<void>(this, 53, std::ref(width), std::ref(height)); //55 if returning nonsense values
    }

    const char* GetLevelName()
    {
        return VTable::GetThiscall<const char*>(this, 58);
    }

    const char* GetLevelNameShort()
    {
        return VTable::GetThiscall<const char*>(this, 59);
    }
};