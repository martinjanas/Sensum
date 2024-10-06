#include "CGameTrace.h"

#include "../../sdk/classes/CHandle.h"

TraceFilter_t::TraceFilter_t(std::uint32_t uMask, CBaseEntity* localplayer, CBaseEntity* player, int nLayer)
{
	m_uTraceMask = uMask;
	m_v1[0] = m_v1[1] = 0;
	m_v2 = 7; //was 7, 15 used mostly
	m_nLayer = nLayer;

	//auto v6 = m_v4 & 0xC9;
	//m_v4 = v6 | 0x49; //0x49;
	m_v4 = 0x49;
	m_flags = 0;

	if (localplayer)
	{
		m_arrSkipHandles[0] = localplayer->GetHandle().GetEntryIndex();
		m_arrSkipHandles[2] = localplayer->GetOwnerHandle();
		m_arrCollisions[0] = localplayer->m_pCollision()->CollisionMask();
	}
	else
	{
		m_arrSkipHandles[0] = -1;
		m_arrSkipHandles[2] = -1;
		m_arrCollisions[0] = -1;
	}

	if (player)
	{
		m_arrSkipHandles[1] = player->GetHandle().GetEntryIndex();
		m_arrSkipHandles[3] = player->GetOwnerHandle();
		m_arrCollisions[1] = player->m_pCollision()->CollisionMask();

	}
	else
	{
		m_arrSkipHandles[1] = -1;
		m_arrSkipHandles[3] = -1;
		m_arrCollisions[1] = -1;
	}

}