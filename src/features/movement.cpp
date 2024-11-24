#include <unordered_set>
#include <mutex>
#include <algorithm>

#include "features.h"
#include "../settings/settings.h"
#include "../sdk/math/math.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/sdk.h"

namespace features
{
    namespace movement
    {
        void handle(CUserCmd* cmd) {
            CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
            if (!localplayer)
                return;

            CCSPlayerPawn* localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPlayerPawn());
            if (!localpawn) 
                return;

            if (settings::misc::m_bBHop) 
            {
                if (cmd->nButtons.nValue & IN_JUMP && localpawn->m_fFlags() & FL_ONGROUND) 
                {
                    cmd->nButtons.nValue &= ~IN_JUMP;
                }
            }
        }
    }
}
