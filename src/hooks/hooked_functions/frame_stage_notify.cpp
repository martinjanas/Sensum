#include "../hooks.h"
#include "../../settings/globals.h"
#include "../../settings/options.hpp"
#include "../../helpers/utils.h"
#include "../../helpers/console.h"
#include "../../features/features.h"

namespace hooks
{
	void __stdcall hooks::frame_stage_notify::hooked(EClientFrameStage stage) 
	{	
		const bool& is_in_game = g::engine_client->IsInGame();

		visuals::fetch_entities();
		
		if (settings::misc::lefthandknife)
			features::lefthand_knife();
		
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) 
		{
			skins::handle();
		}
		
		if (stage == FRAME_NET_UPDATE_START && is_in_game)
		{
			
		}

		if (stage == FRAME_NET_UPDATE_END && is_in_game)
		{

		}

		original(g::base_client, stage);
	}
}