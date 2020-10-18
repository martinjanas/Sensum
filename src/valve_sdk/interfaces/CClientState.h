#pragma once

#include <cstdint>

// Created with ReClass.NET by KN4CK3R
#pragma pack(push, 1)
class INetChannel
{
public:
	char pad_0000[20];           //0x0000
	bool m_bProcessingMessages;  //0x0014
	bool m_bShouldDelete;        //0x0015
	char pad_0016[2];            //0x0016
	int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
	int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
	int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
	int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
	int32_t m_nChokedPackets;    //0x002C number of choked packets
	char pad_0030[1044];         //0x0030
}; //Size: 0x0444

class CClockDriftMgr {
 
public:
	float m_ClockOffsets[0x10];
	uint32_t m_iCurClockOffset;
	uint32_t m_nServerTick;
	uint32_t m_nClientTick;
};
	
	
	char pad000[0x9C];
	INetChannel* m_NetChannel;
	int	m_nChallengeNr;
	char pad001[0x4];
	double m_connect_time;
	int m_retry_number;
	char pad002[0x54];
	int m_nSignonState;
	char pad003[0x4];
	double m_flNextCmdTime;
	int m_nServerCount;
	int m_nCurrentSequence;
	char pad004[0x8];
	CClockDriftManager m_ClockDriftMgr;
	int m_nDeltaTick;
	char pad005[0x4];
	int m_nViewEntity;
	int m_nPlayerSlot;
	bool m_bPaused;
	char pad006[0x3];
	char m_szLevelName[0x104];
	char m_szLevelNameShort[0x28];
	char pad007[0xD4];
	int m_nMaxClients;
	char pad008[0x4994];
	int oldtickcount;
	float m_tickRemainder;
	float m_frameTime;
	int lastoutgoingcommand;
	int chokedcommands;
	int last_command_ack;
	int m_last_server_tick;
	int command_ack;
	int m_nSoundSequence;
	int m_last_progress_percent;
	bool m_is_hltv;
	char pad009[0x4B];
	QAngle viewangles;
	char pad010[0xCC];
	CEventInfo* m_events;
	
	void ForceFullUpdate() {
		m_nDeltaTick = -1;
	}
};

/*class CClientState {
public:
	/*void ForceFullUpdate() {
		*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
	}*/

	char pad_0000[156];             //0x0000
	INetChannel* m_NetChannel;      //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	uint32_t m_nDeltaTick;          //0x0174
	bool m_bPaused;                 //0x0178
	char pad_017D[3];               //0x017D
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[80];    //0x028C
	char m_szGroupName[80];         //0x02DC
	char pad_032Ñ[92];              //0x032Ñ
	uint32_t m_nMaxClients;         //0x0388
	char pad_0314[18824];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	int lastoutgoingcommand;        //0x4CAC
	int chokedcommands;             //0x4CB0
	int last_command_ack;           //0x4CB4
	int command_ack;                //0x4CB8
	int m_nSoundSequence;           //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	QAngle viewangles;              //0x4D10
	char pad_4D1C[208];             //0x4D1C

	void ForceFullUpdate() {
		m_nDeltaTick = -1;
	}
}; */

#pragma pack(pop)

static_assert(FIELD_OFFSET(CClientState, m_NetChannel) == 0x009C, "Wrong struct offset");
static_assert(FIELD_OFFSET(CClientState, m_nCurrentSequence) == 0x011C, "Wrong struct offset");
static_assert(FIELD_OFFSET(CClientState, m_nDeltaTick) == 0x0174, "Wrong struct offset");
