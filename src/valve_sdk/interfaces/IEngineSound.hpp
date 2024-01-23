#pragma once
#include "../math/Vectors.hpp"
#include "../misc/UtlVector.hpp"
#include "../misc/vfunc.hpp"


struct SndInfo_t
{
	int m_nGuid;
	void* m_filenameHandle;
	int m_nSoundSource;
	int m_nChannel;
	int m_nSpeakerEntity;
	float m_flVolume;
	float m_flLastSpatializedVolume;
	float m_flRadius;
	int m_nPitch;
	Vector *m_pOrigin;
	Vector *m_pDirection;
	bool m_bUpdatePositions;
	bool m_bIsSentence;
	bool m_bDryMix;
	bool m_bSpeaker;
	bool m_bSpecialDSP;
	bool m_bFromServer;
};

class IEngineSound
{
public:
	void GetActiveSounds(CUtlVector<SndInfo_t> & sndlist)
	{
		CallVFunction<void(__thiscall*)(void*, CUtlVector<SndInfo_t> &)>(this, 19)(this, sndlist);
	}
};
