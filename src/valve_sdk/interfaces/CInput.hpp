#pragma once

#include "../Misc/CUserCmd.hpp"

constexpr const int MULTIPLAYER_BACKUP = 150;

class CInput
{
public:
    char    pad_0000[0xC];
    bool    m_fTrackIRAvailable;               
    bool    m_fMouseInitialized;               
    bool    m_fMouseActive;                              
    byte    pad_0x08[0x9A];                   
    bool    m_fCameraInThirdPerson;
	byte    pad_00C3[0x2];         
    QAngle  m_vecCameraOffset;                 
    byte    pad_00D1[0x38];
    CUserCmd* m_pCommands;                
    CVerifiedUserCmd* pVerifiedCommands;

    CUserCmd* GetUserCmd(const int nSequenceNumber) const
    {
        return &m_pCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
    }

    CVerifiedUserCmd* GetVerifiedCmd(const int nSequenceNumber) const
    {
        return &pVerifiedCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
    }

    CUserCmd* GetUserCmd(int slot, int sequence_number)
    {
        return &m_pCommands[slot, sequence_number % MULTIPLAYER_BACKUP];
    }
};
