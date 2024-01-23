#pragma once

#include "../math/Vectors.hpp"
#include "../math/QAngle.hpp"
#include "../math/VMatrix.hpp"

#define FLOW_OUTGOING    0
#define FLOW_INCOMING    1

class KeyValues;

typedef struct player_info_s
{
	__int64         unknown;            //0x0000
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuid_low;
			__int32     xuid_high;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID
	char            szFriendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customfiles[4];
	unsigned char   filesdownloaded;
} player_info_t;

class INetChannelInfo
{
public:
	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec
	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback
	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;
	virtual float		GetTimeoutSeconds() const = 0;
};

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		return CallVFunction<void(__thiscall*)(void*, int&, int&)>(this, 5)(this, width, height);
	}

	void ClientCmd(const char* szCmdString)
	{
		return CallVFunction<void(__thiscall*)(void*, const char*)>(this, 7)(this, szCmdString);
	}

	player_info_t GetPlayerInfo(int ent_num)
	{
		player_info_t info;
		CallVFunction<bool(__thiscall*)(void*, int, player_info_t*)>(this, 8)(this, ent_num, &info);

		return info;
	}

	int GetPlayerForUserID(int userID)
	{
		return CallVFunction<int(__thiscall*)(void*, int)>(this, 9)(this, userID);
	}

	bool IsConsoleVisible()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 11)(this);
	}

	int GetLocalPlayer()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 12)(this);
	}

	void GetViewAngles(QAngle& va)
	{
		return CallVFunction<void(__thiscall*)(void*, QAngle&)>(this, 18)(this, va);
	}

	void SetViewAngles(QAngle& va)
	{
		CallVFunction<void(__thiscall*)(void*, QAngle&)>(this, 19)(this, va);
	}

	int GetMaxClients()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 20)(this);
	}

	bool IsInGame()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 26)(this);
	}

	bool IsConnected()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 27)(this);
	}

	bool IsDrawingLoadingImage()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 28)(this);
	}

	const VMatrix& WorldToScreenMatrix()
	{
		return CallVFunction<const VMatrix & (__thiscall*)(void*)>(this, 37)(this);
	}

	const VMatrix& WorldToViewMatrix()
	{
		return CallVFunction<const VMatrix & (__thiscall*)(void*)>(this, 38)(this);
	}

	char const* GetLevelName()
	{
		return CallVFunction<char const* (__thiscall*)(void*)>(this, 52)(this);
	}

	char const* GetLevelNameShort()
	{
		return CallVFunction<char const* (__thiscall*)(void*)>(this, 53)(this);
	}

	char const* GetMapGroupName()
	{
		return CallVFunction<char const* (__thiscall*)(void*)>(this, 54)(this);
	}

	INetChannelInfo* GetNetChannelInfo()
	{
		return CallVFunction<INetChannelInfo * (__thiscall*)(void*)>(this, 78)(this);
	}

	bool IsPlayingDemo()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 82)(this);
	}

	bool IsTakingScreenshot()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 92)(this);
	}

	const char* GetProductVersionString()
	{
		return CallVFunction<const char* (__thiscall*)(void*)>(this, 105)(this);
	}

	void ClientCmd_Unrestricted(const char* command, const bool delayed = false)
	{
		return CallVFunction<void(__thiscall*)(void*, const char*, bool)>(this, 114)(this, command, delayed);
	}

	bool IsVoiceRecording()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 224)(this);
	}
};